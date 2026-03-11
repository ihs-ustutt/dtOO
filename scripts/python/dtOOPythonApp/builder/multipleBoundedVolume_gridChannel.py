from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
    analyticGeometry
)

from dtOOPythonApp.builder import scaOneD_scaCurve2dOneDPointConstruct
from typing import List
import logging

import numpy as np

class multipleBoundedVolume_gridChannel(dtBundleBuilder):


    def __init__(
        self,
        label: str,
        faces: List[analyticGeometry],
        nBlades: int
      ) -> None:


        logging.info( "Initializing %s ..." % (label) )
        super(
          multipleBoundedVolume_gridChannel, self
        ).__init__()
        self.label_ = label
        self.faces_ = faces

    def build(self) -> None:
        
        for i, face in enumerate(self.faces_):
            

            if i == 0:
                
                self.appendAnalyticGeometry(
                        face.segmentConstUPercent(0),
                        "TEST_"+self.label_+"_couplingFace_outletCurve"
                    )
            
            if i == len(self.faces_)-1:    
                
                self.appendAnalyticGeometry(
                        face.segmentConstUPercent(1),
                        "TEST_"+self.label_+"_couplingFace_inletCurve"
                    )
            
            self.appendAnalyticGeometry(
                    face,
                    "TEST_"+self.label_+"_couplingFace_"+str(i)
                )

            self.appendAnalyticGeometry(
                        face.segmentConstVPercent(0),
                        "TEST_"+self.label_+"_couplingFace_hubCurve_"+str(i)
                    )
            # rotating the boundary curves, pushing them in boundSurf
            
            #bs = analyticSurface(
            #        rectangularTrimmedSurface_curveRotateConstructOCC(
            #            analyticCurve.MustDownCast(bound).ptrDtCurve(),
            #            self.origin_,
            #            self.rotVector_,
            #            rotAngle
            #        ).result()
            #    )

