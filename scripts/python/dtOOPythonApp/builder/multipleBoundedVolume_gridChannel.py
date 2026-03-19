from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
    analyticGeometry,
    analyticSurface,
    rectangularTrimmedSurface_curveRotateConstructOCC,
    bSplineSurface_bSplineSurfaceSplitConstructOCC,
    dtPoint3,
    analyticCurve,
    map2dTo3d,
    map3dTo3d,
    dtVector3,
    dtLinearAlgebra,
    labeledVectorHandlingAnalyticGeometry,
    vectorHandlingAnalyticGeometry,
    infinityMap3dTo3d,
    multipleBoundedVolume,
    multipleBoundedSurface,
    partRotatingMap2dTo3d,
    #bSplineCurve_curveConnectConstructOCC,
)

from typing import List, Tuple
import logging

import numpy as np

class multipleBoundedVolume_gridChannel(dtBundleBuilder):
    """
    
    Examples
    --------

    >>> import dtOOPythonSWIG as dtOO
    
    Build channel geometry
    >>> channel = dtOO.rotatingMap2dTo3d(
    ...     dtOO.dtVector3(0,0,1),
    ...     dtOO.analyticSurface(
    ...         dtOO.bSplineSurface_bSplineCurveFillConstructOCC(
    ...             dtOO.bSplineCurve_pointConstructOCC(
    ...                 dtOO.vectorDtPoint3()
    ...                   << dtOO.dtPoint3(+1.00, +0.00, 0.50)
    ...                   << dtOO.dtPoint3(+0.50, +0.00, 0.50),
    ...                 1
    ...             ).result(),
    ...         dtOO.bSplineCurve_pointConstructOCC(
    ...              dtOO.vectorDtPoint3()
    ...                << dtOO.dtPoint3(+0.50, +0.00, 0.50)
    ...                << dtOO.dtPoint3(+0.50, +0.00, 0.00),
    ...                1
    ...            ).result(),
    ...         dtOO.bSplineCurve_pointConstructOCC(
    ...              dtOO.vectorDtPoint3()
    ...                << dtOO.dtPoint3(+0.50, +0.00, 0.00)
    ...                << dtOO.dtPoint3(+1.00, +0.00, 0.00),
    ...                1
    ...            ).result(),
    ...         dtOO.bSplineCurve_pointConstructOCC(
    ...                   dtOO.vectorDtPoint3()
    ...                     << dtOO.dtPoint3(+1.00, +0.00, 0.00)
    ...                     << dtOO.dtPoint3(+1.00, +0.00, 0.50),
    ...                     1
    ...             ).result(),
    ...         ).result()
    ...     )
    ... )

    Define meanplane curves
    >>> c_mp0 = dtOO.bSplineCurve_pointConstructOCC(
    ...           dtOO.vectorDtPoint3()
    ...             << dtOO.dtPoint3(+0.50, +0.00, 0.00)
    ...             << dtOO.dtPoint3(+0.50, +0.00, 0.50),
    ...             1
    ...         ).result()
    >>> c_mp1 = dtOO.bSplineCurve_pointConstructOCC(
    ...           dtOO.vectorDtPoint3()
    ...             << dtOO.dtPoint3(+0.60, +0.10, 0.00)
    ...             << dtOO.dtPoint3(+0.60, +0.20, 0.50),
    ...             1
    ...         ).result()
    >>> c_mp2 = dtOO.bSplineCurve_pointConstructOCC(
    ...           dtOO.vectorDtPoint3()
    ...             << dtOO.dtPoint3(+0.80, +0.20, 0.00)
    ...             << dtOO.dtPoint3(+0.70, +0.10, 0.50),
    ...             1
    ...         ).result()
    >>> c_mp3 = dtOO.bSplineCurve_pointConstructOCC(
    ...           dtOO.vectorDtPoint3()
    ...             << dtOO.dtPoint3(+1.00, +0.00, 0.00)
    ...             << dtOO.dtPoint3(+1.00, +0.00, 0.50),
    ...             1
    ...         ).result()

    Create meanplane faces in the channel from the curves
    >>> mp0 = dtOO.analyticSurface(
    ...         dtOO.bSplineSurface_exchangeSurfaceConstructOCC(
    ...             dtOO.bSplineSurface_skinConstructOCC(
    ...                 c_mp0, c_mp1
    ...             ).result()
    ...         ).result()
    ...     )
    >>> mp1 = dtOO.analyticSurface(
    ...         dtOO.bSplineSurface_exchangeSurfaceConstructOCC(
    ...             dtOO.bSplineSurface_skinConstructOCC(
    ...                 c_mp1, c_mp2
    ...             ).result()
    ...         ).result()
    ...     )
    >>> mp2 = dtOO.analyticSurface(
    ...         dtOO.bSplineSurface_exchangeSurfaceConstructOCC(
    ...             dtOO.bSplineSurface_skinConstructOCC(
    ...                 c_mp2, c_mp3
    ...             ).result()
    ...         ).result()
    ...     )
    
    Make a list containing the meanplane faces
    >>> meanplaneFaces = [
    ...        dtOO.map2dTo3d.MustDownCast(mp0),
    ...        dtOO.map2dTo3d.MustDownCast(mp1),
    ...        dtOO.map2dTo3d.MustDownCast(mp2),
    ...    ]

    create the grid channel
    >>> from dtOOPythonApp.builder import multipleBoundedVolume_gridChannel
    >>> gridChannel = multipleBoundedVolume_gridChannel(
    ...     label = "test",
    ...     channel = channel,
    ...     faces = meanplaneFaces,
    ...     nBlades = 12
    ... )
    >>> gridChannel.build()
    
    return the multiple bounded volume and a list with the faces
    >>> gc, gcFaces = gridChannel.getGridChannel()
    
    Check the class of the returned volume
    >>> gc.virtualClassName()
    'multipleBoundedVolume'

    """
    def __init__(
        self,
        label: str,
        channel: analyticGeometry,
        meanplanes: List[analyticGeometry],
        couplings: List[analyticGeometry],
        nBlades: int,
        rotVector: dtVector3 = dtVector3(0, 0, 1),

      ) -> None:

        super(
          multipleBoundedVolume_gridChannel, self
        ).__init__()

        logging.debug("Initializing %s ..." % (label))
        self.label_ = label
        self.channel_ = map3dTo3d.MustDownCast( channel )
        self.meanplanes_ = meanplanes
        self.couplings_ = couplings
        self.nBlades_ = nBlades
        self.rotVector_ = dtLinearAlgebra.normalize(rotVector)

    def build(self) -> None:
        
        # vetcor handler for boundary surfaces
        self.boundSurf_ = labeledVectorHandlingAnalyticGeometry()
        
        # vector handlers for multiple bounded surfaces of hub and shroud
        hubCurves = vectorHandlingAnalyticGeometry()
        shroudCurves = vectorHandlingAnalyticGeometry()
        
        # generating bounding faces for multiple bounded surfaces
        # the face is a circular segment and not the whol rotating face
        #  because the rotation of 2*pi would be detected as degenerated face
        #  (starting and ending curve would be at the same point)
        f = 0.66
        angle = 2*np.pi*f 
        m1d_hub = self.channel_.segmentConstWPercent(0).segmentConstUPercent(f)
        m2d_hub = analyticSurface(
                    rectangularTrimmedSurface_curveRotateConstructOCC(
                        analyticCurve.MustDownCast(m1d_hub).ptrDtCurve(),
                        dtPoint3(0, 0, 0),
                        dtVector3(0, 0, 1),
                        angle
                    ).result()
                ) 
        m1d_shr = self.channel_.segmentConstWPercent(1).segmentConstUPercent(f)
        m2d_shr = analyticSurface(
                    rectangularTrimmedSurface_curveRotateConstructOCC(
                        analyticCurve.MustDownCast(m1d_shr).ptrDtCurve(),
                        dtPoint3(0, 0, 0),
                        dtVector3(0, 0, 1),
                        angle
                    ).result()
                )

        # iterating over meanplane faces
        for i, face in enumerate(self.meanplanes_):
            
            # creating a volume by rotating the surface
            vol = partRotatingMap2dTo3d(
                    self.rotVector_,
                    map2dTo3d.MustDownCast( face ),
                    0.00,
                    (1/self.nBlades_),
                )
            
            # label "quad" for faces which have to be meshed quadratic
            #  (faces with rotational periodicity to mesh blocks)
            lab = "quad"

            ## Faces are found by cutting segments from the volume
            # outlet boundary of region (first meanplane face)
            if i == 0:
                out = vol.segmentConstVPercent(0)
                self.boundSurf_.push_back(out << "outlet")
                
                # boundary curves for multiple bounded surfaces at hub and shroud
                hub = vol.segmentConstVPercent(0).segmentConstVPercent(0)
                hubCurves.push_back(hub)
                shr = vol.segmentConstVPercent(0).segmentConstVPercent(1)
                shroudCurves.push_back(shr)

            # inlet boundary of region (last meanplane face)
            elif i == len(self.meanplanes_)-1:
                inl = vol.segmentConstVPercent(1)
                self.boundSurf_.push_back(inl << "inlet")
                
                # boundary curves for multiple bounded surfaces at hub and shroud
                hub = vol.segmentConstVPercent(1).segmentConstVPercent(0)
                hubCurves.push_back(hub)
                shr = vol.segmentConstVPercent(1).segmentConstVPercent(1)
                shroudCurves.push_back(shr)
            
            # special treatment for suction boundaries
            #  only the first two and last two faces are taken from the meanplane
            #  the other faces are the coupling faces
            if i <= 1 or i >= len(self.meanplanes_)-2:
                
                # label "tri" for faces which have to be meshed triangular
                #  (faces with rotational periodicity to suction and pressure)
                lab = "tri"

                # suction boundary (second periodic)
                per1 = vol.segmentConstUPercent(0)
                self.boundSurf_.push_back(per1 << "suction_"+lab+"_"+str(i))
                
                # boundary curves for multiple bounded surfaces at hub and shroud
                hub = vol.segmentConstWPercent(0).segmentConstUPercent(0)
                hubCurves.push_back(hub)
                shr = vol.segmentConstWPercent(1).segmentConstUPercent(0)
                shroudCurves.push_back(shr)
            
            # pressure boundary (first periodic)
            #  here always the meanpalne is used
            #  lab changes from "quad" to "tri" depending on the periodicity
            per0 = vol.segmentConstUPercent(1)
            self.boundSurf_.push_back(per0 << "pressure_"+lab+"_"+str(i))
            
            # boundary curves for multiple bounded surfaces at hub and shroud
            # hub boundary
            hub = vol.segmentConstWPercent(0).segmentConstUPercent(1)
            hubCurves.push_back(hub)
            # shroud boundary
            shr = vol.segmentConstWPercent(1).segmentConstUPercent(1)
            shroudCurves.push_back(shr)
        
        # adding coupling faces
        for i, face in enumerate(self.couplings_):
            self.boundSurf_.push_back(face << "coupling_"+str(i))
            
            # boundary curves for multiple bounded surfaces at hub and shroud
            if i >= len(self.couplings_)-2: 
                hubCurves.push_back(face.segmentConstUPercent(0))
                shroudCurves.push_back(face.segmentConstUPercent(1))
            else:
                hubCurves.push_back(face.segmentConstVPercent(0))
                shroudCurves.push_back(face.segmentConstVPercent(1))
        
        # creating multiple bounded surfaces for hub and shroud
        # pushing them into the vector handler for the multiple bounded volume
        mbs_hub = multipleBoundedSurface(m2d_hub, hubCurves)
        mbs_shroud = multipleBoundedSurface(m2d_shr, shroudCurves)
        
        self.boundSurf_.push_back(mbs_hub.clone() << "hub")
        self.boundSurf_.push_back(mbs_shroud.clone() << "shroud")
        
        # appending boundaries if debug is enabeled
        if self.debug():
            for face in self.boundSurf_:
                self.appendAnalyticGeometry(
                        face,
                        "debug_gridChannelFace_"+self.label_+"_"+face.getLabel()
                    )
            for i, curve in enumerate(hubCurves):
                self.appendAnalyticGeometry(
                        curve,
                        "debug_"+self.label_+"_hubCurve_"+str(i)
                    )
            for i, curve in enumerate(shroudCurves):
                self.appendAnalyticGeometry(
                        curve,
                        "debug_"+self.label_+"_shroudCurve_"+str(i)
                    )
                
        # creating grid channel as multi bounded volume
        self.gridChannel_ = multipleBoundedVolume(infinityMap3dTo3d(), self.boundSurf_)
    #
    # return method for grid channel and its faces
    #
    def getGridChannel(self) -> Tuple[analyticGeometry, List[analyticGeometry]]:
        return self.gridChannel_, self.boundSurf_ 
