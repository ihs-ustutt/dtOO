# ------------------------------------------------------------------------------
# dtOO < design tool Object-Oriented >
#
#   Copyright (C) 2024 A. Tismer.
# ------------------------------------------------------------------------------
# License
#   This file is part of dtOO.
#
#   dtOO is distributed in the hope that it will be useful, but WITHOUT
#   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#   FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
#   dtOO root directory for more details.
#
#   You should have received a copy of the License along with dtOO.
#
# ------------------------------------------------------------------------------

from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
    dtLinearAlgebra,
    analyticGeometry,
    map3dTo3d,
    analyticCurve,
    bSplineSurface_skinConstructOCC,
    bSplineSurface_geomCurveFillConstructOCC,
    bSplineCurve_pointConstructOCC,
    analyticSurface,
    dtPoint2,
    dtPoint3,
    dtVector3,
    vectorDtPoint3,
    rotatingMap2dTo3d,
    partRotatingMap2dTo3d,
    trimmedCurve_uBounds,
    geomCurve_curveReverseConstructOCC,
)

from typing import List, Union, Tuple
import logging
import numpy


class analyticGeometry_piecewiseMeridionalRotContour(dtBundleBuilder):
    """Create a grid channel.

    The grid channel is created by rotating the channelSide in the range:

        $[ -0.5*2*pi/numberOfSections, 0.5*2*pi/numberOfSections ]$

    The grid channel is then represented as a rotatingMap2dTo3d within the
    predefined angle.
    """

    def __init__(
        self,
        label: str,
        hubCurves: List[analyticGeometry],
        shroudCurves: List[analyticGeometry],
        hub_splits: List[List[float]],
        shroud_splits: List[List[float]],
        layer_thickness: float,
        layer_supports: List[float] = [],
        internals: List[analyticGeometry] = [],
        rotVector: dtVector3 = dtVector3(0, 0, 1),
        origin: dtPoint3 = dtPoint3(0, 0, 0),
    ) -> None:
        """Constructor.

        Parameters
        ----------
        label: str
          Label.
        hubCurves: List[analyticGeometry]
          Channel side of the grid channel.
        shroudCurves: List[analyticGeometry]
          Number of periodic sections.
        internals: List[analyticGeometry]
          Internal objects.
        rotVector: dtVector3
          Rotation vector.
        origin: dtPoint3
          Origin.

        Returns
        -------
        None

        """
        logging.info("Initializing %s ..." % (label))
        super(analyticGeometry_piecewiseMeridionalRotContour, self).__init__()
        self.label_ = label

        self.origin_ = origin
        self.rotVector_ = dtLinearAlgebra.normalize(rotVector)

        #
        # Create split of hub and shroud curves
        #
        self.hubCurves_ = self.createSplits(hub_splits, hubCurves, "Hub")
        self.shroudCurves_ = self.createSplits(
            shroud_splits, shroudCurves, "Shroud"
        )

        #
        # Extract a bounding box from all start and end points of hub and
        # shroud curves; necessary for calculating the normal direction
        #
        self.normalAxis_ = self.calculateNormalAxis(            # returns normal direction of entire "2D domain"
            self.hubCurves_ + self.shroudCurves_
        )

        #
        # Create regular channels (6-sided regions) by skin construct from
        # hub to shroud
        #
        self.regChannels_ = []
        for ii in range(
            numpy.min([len(self.hubCurves_), len(self.shroudCurves_)]) - 1  # smaller amount of split curves
        ):
            self.regChannels_.append(                                       # iterates over curve list and matches hub and shroud segments 
                analyticSurface(                                            # seqential matches
                    bSplineSurface_skinConstructOCC(
                        analyticCurve.MustDownCast(                         # curves are skinned with each other and surface generated           
                            self.hubCurves_[ii]
                        ).ptrDtCurve(),
                        analyticCurve.MustDownCast(
                            self.shroudCurves_[ii]
                        ).ptrDtCurve(),
                    ).result()
                )
            )
        logging.info("%d regular channels created." % len(self.regChannels_))

        #
        # Create special channels ( rotation of a multiple bounded surfaces)
        #
        ##self.speChannel_ = None

        #
        # Add special hub curves
        #
        self.speHub_ = self.hubCurves_[                                         # slices  hubCurves_ from index determined by min() command
                                                                                # takes the last two hub curves (in example piecewise)
            numpy.min([len(self.hubCurves_), len(self.shroudCurves_)]) - 1 :
        ]
        #
        # Loop over all hub curves of special channel to find curves that have
        # a radius of zero; from those curves no boundary layers will be
        # created
        #
        self.speHubLayer_ = []
        for hub in self.speHub_:                                        # checking if start and end points are on radius = 0
            rz_0 = self.rz_xyz(hub.getPointPercent(0.0))
            rz_1 = self.rz_xyz(hub.getPointPercent(1.0))
            onRotAxis_0 = analyticGeometry.inXYZTolerance(rz_0[0])
            onRotAxis_1 = analyticGeometry.inXYZTolerance(rz_1[0])
            logging.debug(
                "speHub: |<- %5.2f --- %5.2f ->|" % (rz_0[0], rz_1[0])
            )
            if onRotAxis_0 and onRotAxis_1:                             # speHubLayer is appended with booleans 
                self.speHubLayer_.append(False)                         # False : both points are on rotational axis -> no boundary
            else:
                self.speHubLayer_.append(True)
        logging.info("%d special hubs created." % len(self.speHub_))
        logging.info("SpeHubLayer %s" % self.speHubLayer_)

        #
        # Add special shroud curves
        #
        self.speShroud_ = self.shroudCurves_[                                   # slices shroudCurves_ from index determined by min()
            numpy.min([len(self.hubCurves_), len(self.shroudCurves_)]) - 1 :    
        ]
        logging.info("%d special shrouds created." % len(self.speShroud_))

        #
        # Add special radial curves
        #
        self.speRad_ = []
        #
        # Use radial curve of last regular channel to have conform surfaces
        #
        self.speRad_.append(
            self.regChannels_[-1].segmentConstUPercent(1.0).clone()
        )
        #
        # Use radial curve of last curve on hub and shroud
        #
        self.speRad_.append(                                    # generates curve at end of flow channel, from end points of hub and shroud
            analyticCurve(
                bSplineCurve_pointConstructOCC(
                    self.speHub_[-1].getPointPercent(1.0),
                    self.speShroud_[-1].getPointPercent(1.0),
                ).result()
            )
        )

        #
        # Create layer faces on hub and shroud
        #
        self.layerFacesHub_ = []
        self.layerFacesShroud_ = []
        self.layer_supports_ = layer_supports
        self.layer_thickness_ = layer_thickness

        self.layerFacesHub_ = self.checkAndCreateLayer(
            self.speHub_,
            self.speHubLayer_,                              # speHubLayer_ decides if boundary is made (depending on radius = 0) (ln 152)
            self.speRad_,
        )
        self.layerFacesShroud_ = self.checkAndCreateLayer(
            self.speShroud_, [], self.speRad_
        )

        #
        # Handle internals -> possible other objects in the flow channel
        #
        self.internals_ = []
        for internal in internals:
            self.internals_.append(map3dTo3d.MustDownCast(internal))
    
    # Generates boundary layers
    # baseFlags : booleans determinig ig layer should be made (not for curves with radius of 0)
    # radCurves : radial curves
    def checkAndCreateLayer(self, baseCurves, baseFlags, radCurves):    
        layerFaces = []
        if baseFlags == []:
            for curve in baseCurves:
                baseFlags.append(True)

        cc = 0
        for curve, flag in zip(baseCurves, baseFlags):
            if not flag:
                break                                            # breaks loop is a flag is False -> exits loop 
            #
            # initialize left and right curves or directions     # right refers to flow direction in axial machine
            #
            left = None
            right = None
            leftDir = None
            rightDir = None

            ## looking for neighbouring curves to the left and right of the current curve
            #
            # cc is greater than zero; there is an additional curve on the
            # left
            #
            if cc > 0:
                v0 = analyticCurve.MustDownCast(            
                    baseCurves[cc - 1]                      # previous curve
                ).firstDerUPercent(1.0)                     # derivative at 100 percent of curve
                v1 = analyticCurve.MustDownCast(
                    baseCurves[cc]                          # current curve
                ).firstDerUPercent(0.0)                     # derivative at 0 percent -> both derivatives at the same percentage
                leftDir = dtLinearAlgebra.normalize(
                    dtLinearAlgebra.crossProduct(
                        (v0 + v1) * 0.5, self.normalAxis_   # calculates vector normal to averaged direction out of v0 and v1 and normalAxis_
                    )
                )
            #
            # cc is smaller than length of hub curves; there is an additional
            # curve on the right
            #
            if cc < (len(baseCurves) - 1):
                #
                # the next curve is not an hub layer curve; therefore, a
                # direction is necessary
                #
                if baseFlags[cc + 1]:                           # only done when following curve has a boundary layer
                    v0 = analyticCurve.MustDownCast(
                        baseCurves[cc]                          # current curve
                    ).firstDerUPercent(1.0)
                    v1 = analyticCurve.MustDownCast(
                        baseCurves[cc + 1]                      # following curve
                    ).firstDerUPercent(0.0)
                    rightDir = dtLinearAlgebra.normalize(
                        dtLinearAlgebra.crossProduct(
                            (v0 + v1) * 0.5, self.normalAxis_
                        )
                    )

            #
            # first special hub curve; set left special radial curve as left
            # boundary curve
            #
            if cc == 0:
                left = radCurves[0]

            #
            # last special hub curve; set right special radial curve as right
            # boundary curve
            #
            if cc == (len(baseCurves) - 1):
                right = radCurves[1]

            #
            # cc is smaller than max length of special hub curves; so there is
            # at least one more curve
            #
            if cc < (len(baseCurves) - 1):
                #
                # the next curve is not a hub layer curve; so the layer uses
                # this curve as right boundary curve
                #
                if baseFlags[cc + 1] == False:              # following curve has no boundary layer -> the layer wont expand into this section
                    right = baseCurves[cc + 1]
                #
                # next curve is also a layer curve
                #
                else:
                    raise ValueError("Not yet handled")
            layerFaces.append(
                self.createLayer(
                    self.layer_thickness_,
                    curve,                      # current curve in special hub or shroud curves
                    (left, right),              # left and right limits -> boundary layers wont expand past this
                    (leftDir, rightDir),        # can also be none -> createLayer will use radial curves
                    self.normalAxis_,           
                    self.layer_supports_,
                )
            )
            cc = cc + 1

        return layerFaces

    def rz_xyz(self, pp: dtPoint3) -> dtPoint2:
        vv = pp - self.origin_
        zz = dtLinearAlgebra.dotProduct(vv, self.rotVector_)
        rr = dtLinearAlgebra.length(vv - self.rotVector_ * zz)
        return dtPoint2(rr, zz)

    def xyz_rz(self, pp: dtPoint3) -> dtPoint3:
        vv = (
            self.origin_
            + self.rotVector_ * pp[1]
            + dtLinearAlgebra.crossProduct(self.normalAxis_, self.rotVector_)
            * pp[0]
        )
        return vv

    @staticmethod
    def createSplits(splits, inCurves, lab):                                    # inputs (hub and shroud) curves and splits
        outCurves = []
        for cc in range(len(splits)):
            if splits[cc] != []:
                split_0 = 0.0
                curve = analyticCurve.MustDownCast(inCurves[cc]).ptrDtCurve()
                for split_1 in splits[cc]:
                    ac = analyticCurve(
                        trimmedCurve_uBounds(
                            curve,
                            curve.u_uPercent(split_0),
                            curve.u_uPercent(split_1),                          # generates new curve between percentages (0 and input)
                        ).result()
                    )
                    outCurves.append(ac.clone())                                # split curve is  appended to outCurves
                    split_0 = split_1
                    logging.debug(
                        "Split %s[%d] [%5f, %5f]" % (lab, cc, split_0, split_1)
                    )
                split_1 = 1.0
                ac = analyticCurve(                                             # generates new curve between percentages (input and 1)
                    trimmedCurve_uBounds(
                        curve,
                        curve.u_uPercent(split_0),
                        curve.u_uPercent(split_1),
                    ).result()
                )
                outCurves.append(ac.clone())
                logging.debug(
                    "Split %s[%d] [%5f, %5f]" % (lab, cc, split_0, split_1)
                )
            else:
                outCurves.append(inCurves[cc].clone())                          # if no splits just returns curves
                logging.debug("Copy %s[%d]" % (lab, cc))
        return outCurves                                                        # returns split curves

    @staticmethod
    def calculateNormalAxis(curves):
        vv = vectorDtPoint3()               # datatype                                
        for curve in curves:
            vv = vv << curve.getPointPercent(0.0) << curve.getPointPercent(1.0) # appends the start and end points of all curves to vv
        bb = dtLinearAlgebra.boundingBox(vv)                                    # calculate an rectangulat bounding box around the whole domain
        dist = bb[0] - bb[1]
        logging.info("Create bounding box from %d points." % vv.size())
        logging.debug(
            "  bb[0] : %5.2f %5.2f %5.2f" % (bb[0][0], bb[0][1], bb[0][2])
        )
        logging.debug(
            "  bb[1] : %5.2f %5.2f %5.2f" % (bb[1][0], bb[1][1], bb[1][2])
        )
        logging.info(
            "  dist  : %5.2f %5.2f %5.2f" % (dist[0], dist[1], dist[2])
        )
        normalAxis = dtVector3(
            float(analyticGeometry.inXYZTolerance(dist[0])),
            float(analyticGeometry.inXYZTolerance(dist[1])),
            float(analyticGeometry.inXYZTolerance(dist[2])),
        )
        logging.info(
            "  -> normalAxis = (%5.2f %5.2f %5.2f)"
            % (normalAxis[0], normalAxis[1], normalAxis[2])     # normal axis to bounding box
        )
        return normalAxis

    @staticmethod
    def createLayer(
        thickness: float,
        baseCurve: analyticGeometry,
        radCurves: Tuple[                                                       # limits of boundary layers
            Union[analyticGeometry, None], Union[analyticGeometry, None]        # doesnt have to be radial curve, can also be a following curve
        ],
        radDirections: Tuple[Union[dtVector3, None], Union[dtVector3, None]],   # not implemented
        normalAxis: dtVector3,
        supports: List[float] = [],                                             # list of supports specified in piecewiseMeridional
    ):
        left = None
        right = None
        base = analyticCurve.MustDownCast(baseCurve).ptrDtCurve()               # base curve on which layer is added
        direction = 1.0
        #
        # Create first curve in radial direction
        #
        if radCurves[0] != None:                                                # start of layer based on previous curve
            dc = analyticCurve.MustDownCast(radCurves[0]).ptrDtCurve()
            distance = dtLinearAlgebra.distance(
                base.pointPercent(0.0), dc.pointPercent(0.0)
            )
            if not analyticGeometry.inXYZTolerance(distance):                   # check if direction of previous curve is correct
                logging.debug("Reverse direction. distance = %f" % (distance))  # if not curve is taken in reverse and distance is -1
                dc = geomCurve_curveReverseConstructOCC(dc, True).result()
                direction = -1.0
            left = trimmedCurve_uBounds(                                        # creates a trimmed curve between intersect with base curve and thickn
                dc, dc.getUMin(), dc.u_l(thickness)                             # from the previous curve
            ).result()
        elif radDirections[0] != None:
            raise ValueError("Not yet implemented.")
        else:
            raise ValueError("Either give a curve or a direction.")

        #
        # Create second curve in radial direction
        #
        if radCurves[1] != None:
            dc = analyticCurve.MustDownCast(radCurves[1]).ptrDtCurve()
            if not analyticGeometry.inXYZTolerance(
                base.pointPercent(1.0), dc.pointPercent(0.0)
            ):
                dc = geomCurve_curveReverseConstructOCC(dc, True).result()
                direction = -1.0
            right = trimmedCurve_uBounds(
                dc, dc.getUMin(), dc.u_l(thickness)
            ).result()
        elif radDirections[1] != None:
            raise ValueError("Not yet implemented.")
        else:
            raise ValueError("Either give a curve or a direction.")

        #
        # Create point container; first and last point are prescribed; add
        # in-between points defined by supports
        #
        container = vectorDtPoint3()
        # first point
        container = container << left.pointPercent(1.0)             # starting point on previous curve
        # support points
        for support in supports:                                    # adding support points
            container = container << (
                base.pointPercent(support)                          # shifts points on base curve in normal direction of the tangent
                + dtLinearAlgebra.normalize(
                    dtLinearAlgebra.crossProduct(
                        base.firstDerPercent(support), normalAxis
                    )
                )
                * thickness
                * direction
            )
        # last point
        container = container << right.pointPercent(1.0)            # ending point on following curve

        # logging.debug("Container size = %d" % container.size())
        ext = bSplineCurve_pointConstructOCC(container, 2).result()
        for per in [0.0, 0.5, 1.0]:
            logging.info(
                "ext[%3.1f] = %5.2f %5.2f %5.2f"
                % (
                    per,
                    ext.pointPercent(per)[0],
                    ext.pointPercent(per)[1],
                    ext.pointPercent(per)[2],
                )
            )
        # logging.debug("%5.2f" % ext.length())
        # logging.debug("base = %s" % base)
        # logging.debug("left = %s" % left)
        # logging.debug("ext = %s" % ext)
        # logging.debug("right = %s" % right)
        aSurf = analyticSurface(
            bSplineSurface_geomCurveFillConstructOCC(           # generates surface
                base, left, ext, right
            ).result()
        )

        return aSurf.clone()

    def build(self) -> None:
        """Build part.

        Parameters
        ----------
        None

        Returns
        -------
        None

        """
        logging.info("Building %s ..." % (self.label_))

        if self.debug():
            ii = 0
            for regChannel in self.regChannels_:        # regular channels
                self.appendAnalyticGeometry(
                    regChannel.clone(),
                    "debug_regChannel_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
            ii = 0
            for aCurve in self.speHub_:
                self.appendAnalyticGeometry(
                    aCurve.clone(),
                    "debug_speHub_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
            ii = 0
            for aCurve in self.speShroud_:
                self.appendAnalyticGeometry(
                    aCurve.clone(),
                    "debug_speShroud_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
            ii = 0
            for aCurve in self.speRad_:
                self.appendAnalyticGeometry(
                    aCurve.clone(),
                    "debug_speRad_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1

            ii = 0
            for channel in (                # rotates channel surface
                self.regChannels_
                + self.layerFacesHub_
                + self.layerFacesShroud_
            ):
                off = float(ii) * 0.10      # rotational offset of each channel
                self.appendAnalyticGeometry(
                    partRotatingMap2dTo3d(
                        self.rotVector_,
                        channel,
                        off - 0.05,         # min segment in radian 
                        off + 0.05,         # max segment in radian -> the full segment will be 0.1 rad
                    ).clone(),
                    "debug_channel_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1

        return

    def regChannel(self, pos: int) -> analyticGeometry:     # returns the rotated segment of the channel number
        logging.info("Request regChannel[ %d ]" % pos)
        return rotatingMap2dTo3d(
            self.rotVector_, self.regChannels_[pos]
        ).clone()
        # rm.thisown = False
        # logging.info("%s" % rm)
        # return rm

    def addInternals(
        self,
        internals: List[analyticGeometry],
    ) -> None:
        logging.info("Adding %d internals" % len(internals))
        for internal in internals:
            self.internals_.append(map3dTo3d.MustDownCast(internal))
