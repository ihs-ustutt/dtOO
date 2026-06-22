# ------------------------------------------------------------------------------
#  dtOO < design tool Object-Oriented >
#
#    Copyright (C) 2024 A. Tismer.
# ------------------------------------------------------------------------------
# License
#    This file is part of dtOO.
#
#    dtOO is distributed in the hope that it will be useful, but WITHOUT
#    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
#    dtOO root directory for more details.
#
#    You should have received a copy of the License along with dtOO.
#
# ------------------------------------------------------------------------------

from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
    dtPoint2,
    dtPoint3,
    vectorDtPoint3,
    vectorHandlingConstDtCurve,
    bSplineCurve_pointConstructOCC,
    analyticCurve,
    analyticSurface,
    bSplineSurface_skinConstructOCC,
    rotatingMap2dTo3d,
    map3dTo3d,
    analyticGeometry,
)

from typing import List
import logging
import numpy
from math import nan


class rotatingMap2dTo3d_splitFitted(dtBundleBuilder):
    """Create a channel by skinning hub and shroud.

    Attributes
    ----------
    label_: str
      Label.
    channel_: analyticGeometry
      Channel to split.
    internals_: List[analyticGeometry]
      Internal parts that should be included.
    splitDir_: int
      Channel's direction to split.
    segmentDir_: int
      Internal's direction of the segments.
    segments_: List[float]
      Segments to create.
    deltaPer_: float
      Offset of internals for extension.
    """

    def __init__(
        self,
        label: str,
        channel: analyticGeometry,
        internals: List[analyticGeometry],
        splitDir: int = 1,
        segmentDir: int = 2,
        segments: List[float] = [0.0, 0.5, 1.0],
        deltaPer: float = 0.0,
    ) -> None:
        """Constructor.

        Parameters
        ----------
        label: str
          Label.
        channel: analyticGeometry
          Channel to split.
        internals: List[analyticGeometry]
          Internal parts that should be included.
        splitDir: int
          Channel's direction to split.
        segmentDir: int
          Internal's direction of the segments.
        segments: List[float]
          Segments to create.
        deltaPer: float
          Offset of internals for extension.

        Returns
        -------
        None
        """
        logging.info("Initializing %s ..." % (label))
        super(rotatingMap2dTo3d_splitFitted, self).__init__()
        self.label_ = label
        self.channel_ = rotatingMap2dTo3d.MustDownCast(channel)
        self.internals_ = []
        for internal in internals:
            logging.debug(
                "Add internal label : %s ( %s )"
                % (internal.getLabel(), internal.virtualClassName())
            )
            self.internals_.append(map3dTo3d.MustDownCast(internal))
        self.splitDir_ = splitDir
        self.segmentDir_ = segmentDir
        self.segments_ = []
        if self.segmentDir_ == 2:
            for segment in segments:
                # self.segments_.append( self.channel_.w_percent( segment ) )
                self.segments_.append(segment)
        else:
            ValueError("Segment direction must be 2")
        self.deltaPer_ = deltaPer

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
        # get max values
        gMax = self.channel_.getMax(self.splitDir_)
        gMin = self.channel_.getMin(self.splitDir_)
        nSegments = numpy.size(self.segments_)
        # split values
        split_max = numpy.full(nSegments, gMin)
        split_min = numpy.full(nSegments, gMax)
        # segment values
        segment_val = numpy.full(nSegments, 0.0)
        jj = 0
        for internal in self.internals_:
            ii = 0
            for segment in self.segments_:
                midPoint = dtPoint3(nan, nan, nan)
                xyzs = vectorDtPoint3()
                #
                # get bounding box of constant segment and reparameterize it in
                # channel
                #
                if self.segmentDir_ == 2:
                    internalDir = self.detectDirection(
                        self.channel_, self.segmentDir_, internal
                    )
                    if internalDir == 1:
                        checkSeg = internal.segmentConstVPercent(segment)
                        for u in [0.0, 0.5, 1.0]:
                            for v in [0.0, 0.5, 1.0]:
                                xyzs.push_back(
                                    checkSeg.getPointPercent(dtPoint2(u, v))
                                )
                        # mid point is used to define the segement's
                        # coordinate in segmentDir_
                        midPoint = checkSeg.getPointPercent(dtPoint2(0.5, 0.5))
                    else:
                        ValueError("Internal direction must be 1")
                else:
                    ValueError("Segment direction must be 2")
                #
                # reparam in channel
                #
                uvws = vectorDtPoint3()
                for xyz in xyzs:
                    uvws.push_back(self.channel_.reparamInVolume(xyz))
                p_mid = self.channel_.reparamInVolume(midPoint)
                # determine min and max values for split and store segment
                # value of mid point in segment_val
                for uvw in uvws:
                    split_min[ii] = numpy.min(
                        [uvw[self.splitDir_], split_min[ii]]
                    )
                    split_max[ii] = numpy.max(
                        [uvw[self.splitDir_], split_max[ii]]
                    )
                segment_val[ii] = p_mid[self.segmentDir_]
                logging.debug(
                    "[%3d][%3d] "
                    "segment_val = %f / split_min = %f / split_max = %f )"
                    % (
                        jj,
                        ii,
                        segment_val[ii],
                        split_min[ii],
                        split_max[ii],
                    )
                )
                ii = ii + 1
            ii = 0
            jj = jj + 1
        #
        # adjust splits according to deltaPer
        #
        for ii in range(nSegments):
            split_min[ii] = numpy.max(
                [
                    split_min[ii] - self.deltaPer_ * (gMax - gMin),
                    gMin + 0.50 * (split_min[ii] - gMin),
                ]
            )
            split_max[ii] = numpy.min(
                [
                    split_max[ii] + self.deltaPer_ * (gMax - gMin),
                    gMax - 0.50 * (gMax - split_max[ii]),
                ]
            )

        for ii in range(nSegments):  # self.segments_:
            logging.info(
                "segment = %f (%d) -> %f : %f +-- %f -- %f --+ %f"
                % (
                    self.segments_[ii],
                    ii,
                    segment_val[ii],
                    gMin,
                    split_min[ii],
                    split_max[ii],
                    gMax,
                )
            )
            ii = ii + 1

        #
        # create debug curves
        #
        if self.debug():
            pps_min = vectorDtPoint3()
            pps_max = vectorDtPoint3()
            ii = 0
            for segment in self.segments_:
                pps_min.push_back(
                    dtPoint3(
                        self.channel_.getPoint(
                            0.0,
                            split_min[ii],
                            self.channel_.w_percent(segment),
                        )
                    )
                )
                pps_max.push_back(
                    dtPoint3(
                        self.channel_.getPoint(
                            0.0,
                            split_max[ii],
                            self.channel_.w_percent(segment),
                        )
                    )
                )
                ii = ii + 1
            deb_0 = analyticCurve(
                bSplineCurve_pointConstructOCC(pps_min, 2).result()
            )
            deb_1 = analyticCurve(
                bSplineCurve_pointConstructOCC(pps_max, 2).result()
            )
            self.appendAnalyticGeometry(deb_0, self.label_ + "_debug_0")
            self.appendAnalyticGeometry(deb_1, self.label_ + "_debug_1")

        if (self.segmentDir_ == 2) and (self.splitDir_ == 1):
            ccs_0 = vectorHandlingConstDtCurve()
            ccs_1 = vectorHandlingConstDtCurve()
            ccs_2 = vectorHandlingConstDtCurve()
            for ii in range(segment_val.size):
                #
                # extract line
                #
                line_0 = self.channel_.segmentConstW(
                    segment_val[ii]
                ).segmentConstUPercent(
                    0.0, 0.0, self.channel_.percent_v(split_min[ii])
                )
                line_1 = self.channel_.segmentConstW(
                    segment_val[ii]
                ).segmentConstUPercent(
                    0.0,
                    self.channel_.percent_v(split_min[ii]),
                    self.channel_.percent_v(split_max[ii]),
                )
                line_2 = self.channel_.segmentConstW(
                    segment_val[ii]
                ).segmentConstUPercent(
                    0.0, self.channel_.percent_v(split_max[ii]), 1.0
                )

                # logging.debug(
                #  "line_0 %d : %s" % (ii, line.virtualClassName())
                # )
                ccs_0.push_back(
                    analyticCurve.MustDownCast(line_0).ptrDtCurve().clone()
                )
                ccs_1.push_back(
                    analyticCurve.MustDownCast(line_1).ptrDtCurve().clone()
                )
                ccs_2.push_back(
                    analyticCurve.MustDownCast(line_2).ptrDtCurve().clone()
                )
                if self.debug():
                    self.appendAnalyticGeometry(
                        line_0, self.label_ + "_debug_line_0_" + str(ii)
                    )
                    self.appendAnalyticGeometry(
                        line_1, self.label_ + "_debug_line_1_" + str(ii)
                    )
                    self.appendAnalyticGeometry(
                        line_2, self.label_ + "_debug_line_2_" + str(ii)
                    )
            #
            # create surface
            #
            occSurf_0 = bSplineSurface_skinConstructOCC(
                ccs_0, 2, 2, 0
            ).result()
            occSurf_1 = bSplineSurface_skinConstructOCC(
                ccs_1, 2, 2, 0
            ).result()
            occSurf_2 = bSplineSurface_skinConstructOCC(
                ccs_2, 2, 2, 0
            ).result()
            if self.debug():
                self.appendAnalyticGeometry(
                    analyticSurface(occSurf_0),
                    self.label_ + "_debug_surface_0",
                )
                self.appendAnalyticGeometry(
                    analyticSurface(occSurf_1),
                    self.label_ + "_debug_surface_1",
                )
                self.appendAnalyticGeometry(
                    analyticSurface(occSurf_2),
                    self.label_ + "_debug_surface_2",
                )

            self.appendAnalyticGeometry(
                rotatingMap2dTo3d(
                    self.channel_.rotationAxis(), analyticSurface(occSurf_0)
                ),
                self.label_ + "_0",
            )
            self.appendAnalyticGeometry(
                rotatingMap2dTo3d(
                    self.channel_.rotationAxis(), analyticSurface(occSurf_1)
                ),
                self.label_ + "_1",
            )
            self.appendAnalyticGeometry(
                rotatingMap2dTo3d(
                    self.channel_.rotationAxis(), analyticSurface(occSurf_2)
                ),
                self.label_ + "_2",
            )
        else:
            ValueError(
                "Segment direction must be 2 and split direction must be 1."
            )

        return

    @staticmethod
    def detectDirection(
        refMap: map3dTo3d, refDir: int, detMap: map3dTo3d
    ) -> int:
        maxDiff = 0.0
        maxDetDir = -1
        p0 = dtPoint3(0.0, 0.0, 0.0)
        rep_p0 = refMap.reparamPercentInVolume(
            detMap.getPointPercent(p0.x(), p0.y(), p0.z())
        )
        detDir = 0
        for p1 in [
            dtPoint3(1.0, 0.0, 0.0),
            dtPoint3(0.0, 1.0, 0.0),
            dtPoint3(0.0, 0.0, 1.0),
        ]:
            rep_p1 = refMap.reparamPercentInVolume(
                detMap.getPointPercent(p1.x(), p1.y(), p1.z())
            )
            dist = numpy.abs(rep_p1[refDir] - rep_p0[refDir])
            if dist > maxDiff:
                maxDetDir = detDir
                maxDiff = dist
            detDir = detDir + 1
        logging.info("Detect direction %d with %f" % (maxDetDir, maxDiff))
        return maxDetDir
