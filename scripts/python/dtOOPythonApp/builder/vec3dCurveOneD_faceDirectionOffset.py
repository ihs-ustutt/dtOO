#------------------------------------------------------------------------------
#  dtOO < design tool Object-Oriented >
#    
#    Copyright (C) 2024 A. Tismer.
#------------------------------------------------------------------------------
#License
#    This file is part of dtOO.
#
#    dtOO is distributed in the hope that it will be useful, but WITHOUT
#    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
#    dtOO root directory for more details.
#
#    You should have received a copy of the License along with dtOO.
#
#------------------------------------------------------------------------------

from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import bSplineCurve_pointConstructOCC
from dtOOPythonSWIG import bSplineSurface_exchangeSurfaceConstructOCC 
from dtOOPythonSWIG import vectorDtPoint3 
from dtOOPythonSWIG import dtPoint3 
from dtOOPythonSWIG import vec3dCurveOneD 
from dtOOPythonSWIG import analyticCurve
from dtOOPythonSWIG import analyticFunction
from dtOOPythonSWIG import analyticSurface
from dtOOPythonSWIG import dtLinearAlgebra
from dtOOPythonSWIG import vectorHandlingConstAnalyticFunction
from dtOOPythonSWIG import vec3dTransVolThreeD_skinBSplineSurfaces
from dtOOPythonSWIG import vec3dSurfaceTwoD
from dtOOPythonSWIG import bSplineSurface_bSplineSurfaceSplitConstructOCC
from dtOOPythonSWIG import bSplineSurface_skinConstructOCC
from dtOOPythonSWIG import dtOCCBSplineCurve

from typing import List, Optional
import logging

class vec3dCurveOneD_faceDirectionOffset(dtBundleBuilder):

    def __init__(
        self, 
        label: str, 
        surf: analyticFunction,
        thickness: float,
        segPercent: int = 0,
        splitDim: int = 0,
    ) -> None:
      


        logging.info( "Initializing %s ..." % (label) )
        super(vec3dCurveOneD_faceDirectionOffset, self).__init__()
        self.label_ = label
        self.surf_ = analyticSurface(surf.constPtrDtSurface()) 
        self.splitDim_ = splitDim
        self.segPercent_ = segPercent
        
        self.thickness_ = thickness
        
        #surf.setLabel("TEST_surface_"+label)
        #self.appendAnalyticFunction(
        #        surf 
        #    )

    def build(self) -> None:
   
        # set direction f of the offset based on self.splitDim_
        if self.segPercent_ == 0:
            f = -1
        elif self.segPercent_ == 1:
            f = 1
        
        # convert the self.surf_ace to an analytic self.surf_ace
        
        # 
        # Depending on self.splitDim_ the operations are performed in different 
        # self.surf_ace directions
        #
        # self.surf_ace directions:
        #   self.splitDim_ = 0:
        #     v-direction is the hub-to-shroud direction 
        #     u-direction is along the blade contour
        #   self.splitDim_ = 1:
        #     v-direction is the blade skin to block skin direction
        #     u-direction is the hub to shroud direction

        # according to the split dimension which is set the curve is cut out
        # and self.segPercent_ set on uu or vv
        if self.splitDim_ == 0:
            # u-direction
            curve = analyticCurve.MustDownCast(
                    self.surf_.segmentConstUPercent(self.segPercent_)
                ).ptrConstDtCurve()
            uu = self.segPercent_

        elif self.splitDim_ == 1:
            # v-direction
            curve = analyticCurve.MustDownCast(
                    self.surf_.segmentConstVPercent(self.segPercent_)
                ).ptrConstDtCurve()
            vv = self.segPercent_
        
        # get number of control points of the curve 
        bsc_curve = dtOCCBSplineCurve.MustDownCast(curve)
        n = bsc_curve.nControlPoints()

        # initialize point vectors for the offset curves 
        offsetPoints = vectorDtPoint3()
        
        # iterate over control points
        for i in range(n):
            
            # calculate the percentual vv or uu position by normalizing the number of
            # controlpoints
            # calculate the tangential directions of the faces at the support points
            if self.splitDim_ == 0:    
                vv = i/(n-1)
                # tangential direction of the blade at the trailing edge
                tangent = dtLinearAlgebra.normalize(
                        self.surf_.firstDerU(self.surf_.u_percent(uu), self.surf_.v_percent(vv))
                    )
            elif self.splitDim_ == 1:
                uu = i/(n-1)
                # tangential direction of the face normal between blade and thickness faces
                tangent = dtLinearAlgebra.normalize(
                        self.surf_.firstDerV(self.surf_.u_percent(uu), self.surf_.v_percent(vv))
                    )
            
            # appending the blade point offset in tangential direction
            #  the offset will have the specified thickness, f sets the direction
            offsetPoints.append(
                    self.surf_.getPoint(
                        self.surf_.u_percent(uu), self.surf_.v_percent(vv)
                    ) + tangent * self.thickness_*f
                )
            
            ## Debug statement to plot tangents
            #tangentCurve = bSplineCurve_pointConstructOCC(
            #    vectorDtPoint3()
            #      << self.surf_.getPoint(self.surf_.u_percent(uu), self.surf_.v_percent(vv))
            #      << self.surf_.getPoint(self.surf_.u_percent(uu), self.surf_.v_percent(vv)) + tangent * self.thickness_*f,
            #    1
            #).result()

            #tang = (vec3dCurveOneD( tangentCurve ) << self.self.label__ + "Curve_te"+str(uu)+"_"+str(i))
            #self.appendAnalyticFunction(tang)
                 
         
        self.appendAnalyticFunction(
            vec3dCurveOneD(
                bSplineCurve_pointConstructOCC(
                    offsetPoints,2
                ).result() 
            ) << self.label_ + "1"
        )
        self.appendAnalyticFunction(
            vec3dCurveOneD( 
                curve
            ) << self.label_ + "0"
        )

        return
    
