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
    curveCurveDist,
    dtCurve,
    gslMinFloatAttr,
    vectorReal,
    vectorHandlingConstDtCurve,
    bSplineSurface_bSplineCurveFillConstructOCC,
    bSplineCurve_curveConnectConstructOCC
)

from typing import List, Union, Tuple
import logging
import numpy


class analyticGeometryLayers_piecewiseMeridionalRotContour(dtBundleBuilder):
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
        hub_splits: List[List[float]] = [],
        hub_combine: List[List[bool]] = [],
        shroud_splits: List[List[float]] = [],
        shroud_combine: List[List[bool]] = [],
        layer_thickness: float = 0,
        layer_supports: List[float] = [],
        interface_hub: List[float] = [],             # [curve, percent]
        interface_shroud: List[float] = [],
        interface_curvature: List[float] = [],         # [curvature offset, percent]
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
        super(analyticGeometryLayers_piecewiseMeridionalRotContour, self).__init__()
        self.label_ = label
        
        self.origin_ = origin
        self.rotVector_ = dtLinearAlgebra.normalize(rotVector)
        """
        for ii in range(interface_hub[-1][0]):
            if ii == 0:
                regCurveHub = analyticCurve(
                        bSplineCurve_curveConnectConstructOCC(
                            analyticCurve.MustDownCast(
                                hubCurves[ii]
                            ).ptrDtCurve(), 
                            analyticCurve.MustDownCast(
                                hubCurves[ii+1]
                            ).ptrDtCurve()
                        ).result())

            else:
                regCurveHub = analyticCurve(bSplineCurve_curveConnectConstructOCC(analyticCurve.MustDownCast(regCurveHub).ptrDtCurve(), analyticCurve.MustDownCast(hubCurves[ii+1]).ptrDtCurve()).result())

        self.appendAnalyticGeometry(
            regCurveHub.clone(),
            "debug_regHubCurve_TEST_"  + self.label_,
        )

        for ii in range(interface_shroud[-1][0]):
            if ii == 0:
                regCurveShroud = analyticCurve(
                        bSplineCurve_curveConnectConstructOCC(
                            analyticCurve.MustDownCast(
                                shroudCurves[ii]
                            ).ptrDtCurve(), 
                            analyticCurve.MustDownCast(
                                shroudCurves[ii+1]
                            ).ptrDtCurve()
                        ).result())

            else:
                regCurveShroud = analyticCurve(bSplineCurve_curveConnectConstructOCC(analyticCurve.MustDownCast(regCurveShroud).ptrDtCurve(), analyticCurve.MustDownCast(shroudCurves[ii+1]).ptrDtCurve()).result())

        self.appendAnalyticGeometry(
            regCurveShroud.clone(),
            "debug_regShroudCurve_TEST_"  + self.label_,
        )"""
        #
        # Extract a bounding box from all start and end points of hub and
        # shroud curves; necessary for calculating the normal direction
        #
        self.normalAxis_ = self.calculateNormalAxis(            # returns normal direction of entire "2D domain"
            hubCurves + shroudCurves
        )

        self.interfaces_ = self.createInterface(hubCurves,
                                               shroudCurves,
                                               interface_hub,
                                               interface_shroud,
                                               interface_curvature,
                                               self.normalAxis_)
        #self.hubSplits_ = []
        #for i in range(len(hub_splits)):
        #    for l in range(max(1, len(hub_splits[i]))):
        #        self.hubSplits_[i].append((hub_splits[i][l], None))
        self.hubSplits_ = []

        for i in range(len(hub_splits)):
            splits = []
            for l in range(len(hub_splits[i])):
                splits.append((hub_splits[i][l], None))
            self.hubSplits_.append(splits)
        print("self.hubSplits_ = ", self.hubSplits_)
        
        self.shroudSplits_ = []
        for i in range(len(shroud_splits)):
            splits = []
            for l in range(len(shroud_splits[i])):
                splits.append((shroud_splits[i][l], None))
            self.shroudSplits_.append(splits)
        print("self.hubSplits_ = ", self.shroudSplits_)
        
        # adding interface points to splits        # adding interface points to splits
        for i in range(len(interface_hub)):
            self.hubSplits_[interface_hub[i][0]].append((interface_hub[i][1],i))
            self.hubSplits_[i].sort()
            self.shroudSplits_[interface_shroud[i][0]].append((interface_shroud[i][1],i))
            self.shroudSplits_[i].sort()
        print("self.hubSplits_ = ", self.hubSplits_)
        print("self.shroudSplits_ = ", self.shroudSplits_)

        #for i_interface in range(len(interface_hub)):
        #    hub_splits[interface_hub[i_interface][0]].append([interface_hub[i_interface][1],1])
        #    hub_splits[i_interface].sort()
        #    shroud_splits[interface_shroud[i_interface][0]].append([interface_shroud[i_interface][1],1])
        #    shroud_splits[i_interface].sort()

        #print("hub_splits = ", hub_splits)
        #print("shroud_splits = ", shroud_splits)
        #
        # Create split of hub and shroud curves
        #
        self.hubCurves_, hub_cti = self.createSplits(self.hubSplits_, hubCurves, "Hub")
        self.shroudCurves_, shroud_cti = self.createSplits(self.shroudSplits_, shroudCurves, "Shroud")
        print("hub_cti = ", hub_cti)
        print("shroud_cti = ", shroud_cti)
        
        hub_cti = self.propagate_interface_ids_next(hub_cti)
        shroud_cti = self.propagate_interface_ids_next(shroud_cti)
        print("hub_cti = ", hub_cti)
        print("shroud_cti = ", shroud_cti)
        print("will this show")
        #
        # Create regular channels (6-sided regions) by skin construct from
        # hub to shroud
        #
        """
        #  routine for creation of regular hubs with skips
        self.regChannels_ = []
        vhc = vectorHandlingConstDtCurve()

        i_max = numpy.min([len(self.hubCurves_), len(self.shroudCurves_)]) - 1 
        
        if len(hub_combine) < i_max:
            hub_combine.extend([[] for _ in range(i_max - len(hub_combine))])
        if len(shroud_combine) < i_max:
            shroud_combine.extend([[] for _ in range(i_max - len(shroud_combine))])

        i_h = 0
        i_s = 0
        
        while True:
            
            # checks if a countour line should be skipped
            if hub_combine[i_h]:
                print(type(analyticCurve.MustDownCast(self.hubCurves_[i_h]).ptrDtCurve()))
                #vhc.push_back(analyticCurve.MustDownCast(self.hubCurves_[i_h]).ptrDtCurve())
                i_h = i_h + 1
            
            if shroud_combine[i_h]:
                #vhc.push_back(analyticCurve.MustDownCast(self.shroudCurves_[i_s]).ptrDtCurve())
                i_s = i_s + 1
            
            if ((not hub_combine[i_h]) and (not shroud_combine[i_s])):
                # creates the channel if the contour isnt skipped
                vhc.push_back(analyticCurve.MustDownCast(self.hubCurves_[i_h]).ptrDtCurve())
                vhc.push_back(analyticCurve.MustDownCast(self.shroudCurves_[i_s]).ptrDtCurve())

                self.regChannels_.append(                                       # iterates over curve list and matches hub and shroud segments 
                    analyticSurface(                                            # seqential matches
                        bSplineSurface_skinConstructOCC(vhc).result()
                        #bSplineSurface2d_bSplineCurve2dFillConstructOCC(vhc).result()
                    )
                )           
                del vhc
                vhc = vectorHandlingConstDtCurve()
                i_h = i_h + 1
                i_s = i_s + 1

            if i_h >= i_max or i_s >= i_max:
                break
        """
        #
        # Add special radial curves
        #
        self.speRad_ = []
        #
        # Use radial curve of last regular channel to have conform surfaces
        #
        self.speRad_.append(                                    # generates curve at end of flow channel, from end points of hub and shroud
            analyticCurve(
                bSplineCurve_pointConstructOCC(
                    self.hubCurves_[0].getPointPercent(0),
                    self.shroudCurves_[0].getPointPercent(0),
                ).result()
            )
        )
 

        self.regChannels_ = []
        """
        self.regChannels_.append(                                       # iterates over curve list and matches hub and shroud segments 
            analyticSurface(                                            # seqential matches
                bSplineSurface_bSplineCurveFillConstructOCC(
                    analyticCurve.MustDownCast(                         # curves are skinned with each other and surface generated           
                        self.speRad_[0]
                    ).ptrDtCurve(),
                    analyticCurve.MustDownCast(
                        self.hubCurves_[0]
                    ).ptrDtCurve(),
                    analyticCurve.MustDownCast(
                        self.shroudCurves_[0]
                    ).ptrDtCurve(),
                    analyticCurve.MustDownCast(
                        self.interfaces_[0]
                    ).ptrDtCurve(),
                ).result()
            )
        )
        """
        for ii in range(len(interface_hub)):

            vhc = vectorHandlingConstDtCurve()
            
            print("interface type 1 = ", type(self.interfaces_[ii]))
            print("interface type 2 = ", type(analyticCurve.MustDownCast(self.interfaces_[ii])))
            vhc.push_back(analyticCurve.MustDownCast(self.interfaces_[ii]).ptrDtCurve())


            add_hub = -1
            add_shroud = -1
            
            if ii == 0:
                vhc.push_back(analyticCurve.MustDownCast(self.speRad_[0]).ptrDtCurve())
            else:
                vhc.push_back(analyticCurve.MustDownCast(self.interfaces_[ii-1]).ptrDtCurve())
            
            first = 0
            for jj in range(len(self.hubCurves_)):
                print("#######################")
                print("cti = ", hub_cti[jj])
                print("ii = ", ii)
                if hub_cti[jj] == ii:
                    print("is equal")
                    if first == 0:
                        regCurveHub = self.hubCurves_[jj]
                         
                        first = 1
                    else:
                        regCurveHub = analyticCurve(
                            bSplineCurve_curveConnectConstructOCC(
                                analyticCurve.MustDownCast(
                                    regCurveHub
                                ).ptrDtCurve(),
                                analyticCurve.MustDownCast(
                                    self.hubCurves_[jj]
                                ).ptrDtCurve()
                            ).result()
                        )
            vhc.push_back(analyticCurve.MustDownCast(regCurveHub).ptrDtCurve())
            self.appendAnalyticGeometry(
                regCurveHub.clone(),
                "debug_regCurveHub_TEST1_" + str(ii) + "_" + self.label_,
            )
            first = 0

            for jj in range(len(self.shroudCurves_)):
                if shroud_cti[jj] == ii:
                    if first == 0:
                        regCurveShroud = self.shroudCurves_[jj]
                         
                        first = 1
                    else:
                        regCurveShroud = analyticCurve(
                            bSplineCurve_curveConnectConstructOCC(
                                analyticCurve.MustDownCast(
                                    regCurveShroud
                                ).ptrDtCurve(),
                                analyticCurve.MustDownCast(
                                    self.shroudCurves_[jj]
                                ).ptrDtCurve()
                            ).result()
                        )
            vhc.push_back(analyticCurve.MustDownCast(regCurveShroud).ptrDtCurve())
            self.appendAnalyticGeometry(
                regCurveShroud.clone(),
                "debug_regCurveShroud_TEST1_" + str(ii) + "_" + self.label_,
            )

            """
            for jj in range(interface_shroud[ii][0] + 1 - add_shroud+1):
                print("######### DEBUG ")
                vhc.push_back(analyticCurve.MustDownCast(self.shroudCurves_[jj + add_shroud+1]).ptrDtCurve())
                self.appendAnalyticGeometry(
                    self.shroudCurves_[jj + add_shroud+1].clone(),
                    "debug_shroudCurve_TEST1_" + str(jj) + "_" + "regCh" + str(ii) + "_" + self.label_,
                )
            """
                
            self.regChannels_.append(                                       # iterates over curve list and matches hub and shroud segments 
                analyticSurface(                                            # seqential matches
                    bSplineSurface_bSplineCurveFillConstructOCC(vhc).result()
                    #bSplineSurface2d_bSplineCurve2dFillConstructOCC(vhc).result()
                )
            )  
            
            del vhc
        """
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
        """
        
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
        #self.speRad_ = []
        #
        # Use radial curve of last regular channel to have conform surfaces
        #
        """
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
        """
        """
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
        """        
    #
    # creates an interface curve between two specified points on the hub and the shroud curve
    #
    def createInterface(self, 
                        hubCurves,
                        shroudCurves,
                        interface_hub,
                        interface_shroud, 
                        curve, 
                        normalAxis):    
        
        if (len(interface_hub) != len(interface_shroud)) or (len(interface_hub) != len(curve)):
            raise ValueError("interface arrays need the same no. of entries.")

        interfaces = []
        
        # iterating through all the interfaces that will be created
        for ii in range(len(interface_hub)):
            
            # finds the points at the specified percentage of the specified hub and shroud curves
            pointHub = analyticCurve.MustDownCast(
                    hubCurves[interface_hub[ii][0]]).getPointPercent(
                            interface_hub[ii][1])
            pointShroud = analyticCurve.MustDownCast(
                    shroudCurves[interface_shroud[ii][0]]).getPointPercent(
                            interface_shroud[ii][1])
            
            # creates a straigth line between the two points
            MP_linear = analyticCurve(
                bSplineCurve_pointConstructOCC(
                    vectorDtPoint3()
                    << pointHub
                    << pointShroud,
                    1
                ).result()
            )
            
            direction = curve[ii][2]

            # calculates the curvature point by making an offset in normal 
            # direction of the specified percentage from MP_linear
            # the normal direction is calculated from the crossproduct 
            # of the direction of MP_linear and the normalAxis
            # the offset is a specified percentage of the length of MP_linear
            # times +-1 for direction
            pointCurve = (analyticCurve.MustDownCast(MP_linear).getPointPercent(curve[ii][1])
                + dtLinearAlgebra.normalize(
                    dtLinearAlgebra.crossProduct(
                        MP_linear.firstDerUPercent(curve[ii][1]), normalAxis
                    )
                ) 
                * MP_linear.length() * curve[ii][0] 
                * direction)
       
            # interface as a bSpline curve from the three points
            interface = analyticCurve(
                bSplineCurve_pointConstructOCC(
                    vectorDtPoint3()
                    << pointHub
                    << pointCurve
                    << pointShroud,
                    2
                ).result()
            )
            
            interfaces.append(interface.clone())
            logging.debug("Interface [%d]" %  ii)
       
        # checking if the generated interfaces are intersecting with any hub or shroud curves 
        intersects_hub = self.detectIntersect(interfaces, hubCurves)
        intersects_shroud = self.detectIntersect(interfaces, shroudCurves)

        # logging or warning if intersections occur
        if (not intersects_hub and not intersects_shroud):
            logging.debug("Interfaces created without any illegal intersections")
        else:
            for i_hub in range(len(intersects_hub)):
                logging.warning(
                    "Illegal intersect between interface no. %i and hub curve no. %i",
                    intersects_hub[i_hub][0],
                    intersects_hub[i_hub][1]
                )

            for i_shroud in range(len(intersects_shroud)):
                logging.warning(
                    "Illegal intersect between interface no. %i and shroud curve no. %i",
                    intersects_shroud[i_shroud][0], 
                    intersects_shroud[i_shroud][1]
                )
        
        return interfaces
    #
    # searches for intersections between two lists of curves
    #
    def detectIntersect(self, interfaces, curves):
            
            intersect_list = []

            for ii in range(len(interfaces)):
                
                # trimms the interface curve at 5 and 95 percent 
                # this is done so the planned intersect is not detected 
                # (this would only occur at very high angles) 
                interface = analyticCurve.MustDownCast(interfaces[ii]).ptrDtCurve()
                cut_interface = analyticCurve(
                    trimmedCurve_uBounds(
                        interface,
                        interface.u_uPercent(0.05),
                        interface.u_uPercent(0.95),                          # generates new curve between percentages (0 and input)
                    ).result()
                )
                
                #self.appendAnalyticGeometry(
                #    cut_interface.clone(),
                #    "debug_cut_interface_" + str(ii) + "_" + self.label_,
                #)

                for ic in range(len(curves)):
                    # creates a minimization object to search for the intersect point between the two curves
                    # implemented form the documentation: Transfinite Mesh
                    gmf = gslMinFloatAttr(
                        curveCurveDist(cut_interface.ptrConstDtCurve(), curves[ic].ptrConstDtCurve()),
                        vectorReal([0.5, 0.5,]),
                        vectorReal([0.001, 0.001,]),
                        1.e-8,
                        100
                    )
                    # returns True if a intersect is detected
                    interbool = gmf.perform()

                    del gmf
                    #print("interface ", str(ii), "and hub Curve ", str(ic), "intersect is ", str(interbool))
                    
                    # caches the curves which intersect for return
                    if interbool == True:
                        intersect_list.append([ii, ic, interbool])

            return intersect_list
                    
    def propagate_interface_ids_next(self, curve_to_interface):
        result = curve_to_interface.copy()
        n = len(result)
        
        # find indices of segments with interface IDs
        iface_indices = [i for i, x in enumerate(result) if x is not None]
        
        if not iface_indices:
            return result  # nothing to propagate
        
        # assign first interface to segments before first one
        first_idx = iface_indices[0]
        for i in range(0, first_idx):
            result[i] = result[first_idx]
        
        # assign next interface to segments between interface IDs
        for idx in range(len(iface_indices) - 1):
            start = iface_indices[idx]
            end = iface_indices[idx + 1]
            for i in range(start + 1, end):
                result[i] = result[end]
        
        # segments after the last interface remain None
        return result

    # Generates boundary layers
    # baseFlags : booleans determinig if layer should be made (not for curves with radius of 0)
    # radCurves : radial curves
    def checkAndCreateLayer(self, baseCurves, baseFlags, radCurves):
        layerFaces = []
        if baseFlags == []:
            for curve in baseCurves:
                baseFlags.append(True)

        cc = 0
        for curve, flag in zip(baseCurves, baseFlags):
            if not flag:
                break                                            # breaks loop if a flag is False -> exits loop 
            #
            # initialize left and right curves or directions     
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
                if baseFlags[cc + 1] == False:      # following curve has no boundary layer -> the layer wont expand into this section
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
        curve_to_interface = []
        for cc in range(len(splits)):
            
            curve = analyticCurve.MustDownCast(inCurves[cc]).ptrDtCurve()
            
            if splits[cc]:
                split_pos = [0.0] + [s[0] for s in splits[cc]] + [1.0]
                interface_ids = [s[1] for s in splits[cc]]  # corresponding interface IDs (None if normal split)

                #split_pos = [0]
                #for s in range(len(splits[cc])):
                #    split_pos.append(splits[cc][s])
                #split_pos.append(1)
                print("split_pos = ", split_pos)
                
                for n in range(len(split_pos)-1):
                    ac = analyticCurve(
                        trimmedCurve_uBounds(
                            curve,
                            curve.u_uPercent(split_pos[n]),
                            curve.u_uPercent(split_pos[n+1]),                          # generates new curve between percentages (0 and input)
                        ).result()
                    )
                    outCurves.append(ac.clone())                                # split curve is  appended to outCurves
                    
                    # Determine interface ownership
                    # We assign the interface ID of the *upper bound* of the segment
                    iface_id = interface_ids[n] if n < len(interface_ids) else None
                    curve_to_interface.append(iface_id)

                    logging.debug(
                        "Split %s[%d] [%5f, %5f]" % (lab, cc, split_pos[n], split_pos[n+1])
                    )
            else:
                outCurves.append(inCurves[cc].clone())                          # if no splits just returns curves
                logging.debug("Copy %s[%d]" % (lab, cc))
                curve_to_interface.append(None)
        return outCurves, curve_to_interface                                                        # returns split curves

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
            for aCurve in self.shroudCurves_:
                self.appendAnalyticGeometry(
                    aCurve.clone(),
                    "debug_shroudCurves_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
            ii = 0
            for aCurve in self.hubCurves_:
                self.appendAnalyticGeometry(
                    aCurve.clone(),
                    "debug_hubCurves_" + str(ii) + "_" + self.label_,
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
                #+ self.layerFacesHub_
                #+ self.layerFacesShroud_
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
            ii = 0
            for interface in self.interfaces_:
                self.appendAnalyticGeometry(
                    interface.clone(),
                    "debug_interface_" + str(ii) + "_" + self.label_,
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
