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
    infinityMap3dTo3d,
    multipleBoundedVolume,
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
        faces: List[analyticGeometry],
        nBlades: int,
        rotVector: dtVector3 = dtVector3(0, 0, 1),

      ) -> None:

        super(
          multipleBoundedVolume_gridChannel, self
        ).__init__()

        logging.debug("Initializing %s ..." % (label))
        self.label_ = label
        self.channel_ = map3dTo3d.MustDownCast( channel )
        self.faces_ = faces 
        self.nBlades_ = nBlades
        self.rotVector_ = dtLinearAlgebra.normalize(rotVector)

    def build(self) -> None:
        
        # vecor handler fpr boundary surfaces
        self.boundSurf_ = labeledVectorHandlingAnalyticGeometry()
        
        #hubCurves = vectorHandlingAnalyticGeometry()
        #angle = 2*np.pi*0.996 
        hub = self.channel_.segmentConstWPercent(0)#.segmentConstUPercent(0)
        #hub = analyticSurface(
        #            rectangularTrimmedSurface_curveRotateConstructOCC(
        #                analyticCurve.MustDownCast(hub).ptrDtCurve(),
        #                dtPoint3(0, 0, 0),
        #                dtVector3(0, 0, 1),
        #                angle
        #            ).result()
        #        )

        #hub = analyticSurface(
        #        bSplineSurface_bSplineSurfaceSplitConstructOCC(
        #            analyticSurface.MustDownCast(hub).ptrDtSurface(),
        #            0,
        #            hub.u_percent(0.75),
        #            hub.u_percent(1.00)
        #        ).result()
        #    )

        self.boundSurf_.push_back(hub << "hub")
        
        shr = self.channel_.segmentConstWPercent(1)
        self.boundSurf_.push_back(shr << "shroud")

        # iterating over meanplane faces
        for i, face in enumerate(self.faces_):
            
            # creating a volume by rotating the surface
            vol = partRotatingMap2dTo3d(
                    self.rotVector_,
                    map2dTo3d.MustDownCast( face ),
                    0.00,
                    (1/self.nBlades_),
                )
            
            ## Faces are found by cutting segments from the volume
            # outlet boundary of region (first meanplane face)
            if i == 0:
                out = vol.segmentConstVPercent(0)
                self.boundSurf_.push_back(out << "outlet")
            
            # inlet boundary of region (last meanplane face)
            elif i == len(self.faces_)-1:
                inl = vol.segmentConstVPercent(1)
                self.boundSurf_.push_back(inl << "inlet")
            
            # pressure boundary (first periodic)
            per0 = vol.segmentConstUPercent(1)
            self.boundSurf_.push_back(per0 << "pressure_"+str(i))
            
            # suction boundary (second periodic)
            per1 = vol.segmentConstUPercent(0)
            self.boundSurf_.push_back(per1 << "suction_"+str(i))
            
            ## hub boundary
            #hub = vol.segmentConstWPercent(0).segmentConstUPercent(0)
            ##self.boundSurf_.push_back(hub << "hub_"+str(i))
            #hubCurves.push_back(analyticCurve.MustDownCast(hub).ptrDtCurve())

            ## shroud boundary
            #shr = vol.segmentConstWPercent(1).segmentConstUPercent(0)
            #self.boundSurf_.push_back(shr << "shroud_"+str(i))
    

        #hub = analyticCurve(
        #        bSplineCurve_curveConnectConstructOCC(
        #            hubCurves
        #        ).result()
        #    )
        #self.boundSurf_.push_back(hub << "hub_"+str(i))
        
        # appending boundaries if debug is enabeled
        if self.debug():
            for face in self.boundSurf_:
                self.appendAnalyticGeometry(
                        face,
                        "debug_"+self.label_+"_"+face.getLabel()
                    )
        
        # creating grid channel as multi bounded volume
        self.gridChannel_ = multipleBoundedVolume(infinityMap3dTo3d(), self.boundSurf_)
    #
    # return method for grid channel and its faces
    #
    def getGridChannel(self) -> Tuple[analyticGeometry, List[analyticGeometry]]:
        return self.gridChannel_, self.boundSurf_ 
