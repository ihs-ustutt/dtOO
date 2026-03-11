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
from dtOOPythonSWIG import bSplineSurface_bSplineCurveFillConstructOCC 
from dtOOPythonSWIG import bSplineSurface_exchangeSurfaceConstructOCC 
from dtOOPythonSWIG import vectorDtPoint3 
from dtOOPythonSWIG import dtPoint3 
from dtOOPythonSWIG import dtVector3 
from dtOOPythonSWIG import vec3dCurveOneD 

from dtOOPythonSWIG import analyticCurve
from dtOOPythonSWIG import analyticFunction
from dtOOPythonSWIG import analyticSurface
from dtOOPythonSWIG import dtLinearAlgebra
from dtOOPythonSWIG import translatingMap2dTo3d 
from dtOOPythonSWIG import geomSurface_surfaceTranslateConstructOCC 
from dtOOPythonSWIG import vectorHandlingConstAnalyticFunction
from dtOOPythonSWIG import vec3dTransVolThreeD_skinBSplineSurfaces
from dtOOPythonSWIG import vec3dSurfaceTwoD
from dtOOPythonSWIG import vec3dTransVolThreeD
from dtOOPythonSWIG import bSplineSurface_bSplineSurfaceSplitConstructOCC
from dtOOPythonSWIG import bSplineSurface_skinConstructOCC
from dtOOPythonSWIG import dtOCCBSplineCurve
from dtOOPythonSWIG import dtOCCBSplineSurface 

from typing import List, Optional
import logging

class vec3dThreeD_skinAndSplit(dtBundleBuilder):

    """Create vec3dThreeD function by skining two BSpline surfaces with split.

    Attributes
    ----------
    label_: str
      Label.
    aFOne_: vec3dSurfaceTwoD
      First BSpline surface.
    aFTwo_: vec3dSurfaceTwoD
      Second BSpline surface.
    splitDim_: int
      Dimension where surface is splitted before skinned.
    splits_: List[List[float]]
      Positions for splitting.
    tEMeshBlockThickness: Optional[float] = None
      thickness of the Trailing Edge Mesh Blocks, if not None
    meanplaneFromBlocks: bool = False
      activates creation of meanplane curve
    meanplaneExtOut: Optional[float] = 0.01,
      extention of meanplane curve towards outlet
    meanplaneExtIn: Optional[float] = 0.01,
      extention of meanplane curve towards inlet
    nMeanplaneBlocks: Optional[int] = 3
      number of block faces used for the meanplane

    Examples
    --------

    >>> from dtOOPythonSWIG import dtPoint3
    >>> from dtOOPythonSWIG import bSplineCurve_pointConstructOCC
    >>> from dtOOPythonSWIG import bSplineSurface_skinConstructOCC
    >>> from dtOOPythonSWIG import vec3dSurfaceTwoD

    Create first analyticFunction:

    >>> aFOne = vec3dSurfaceTwoD( 
    ...   bSplineSurface_skinConstructOCC(
    ...     bSplineCurve_pointConstructOCC(
    ...       dtPoint3(0,0,0), dtPoint3(1,0,0)
    ...     ).result(),
    ...     bSplineCurve_pointConstructOCC(
    ...       dtPoint3(0,1,0), dtPoint3(1,1,0)
    ...     ).result()
    ...   ).result()
    ... )

    Create second analyticFunction:

    >>> aFTwo = vec3dSurfaceTwoD( 
    ...   bSplineSurface_skinConstructOCC(
    ...     bSplineCurve_pointConstructOCC(
    ...       dtPoint3(0,0,1), dtPoint3(1,0,1)
    ...     ).result(),
    ...     bSplineCurve_pointConstructOCC(
    ...       dtPoint3(0,1,1), dtPoint3(1,1,1)
    ...     ).result()
    ...   ).result()
    ... )

    Initialize builder:

    >>> builder = vec3dThreeD_skinAndSplit("unitCube", aFOne, aFTwo)

    Build volume:

    >>> builder.build()

    Check label of first analyticFunction:

    >>> builder.lVH_aF().labels()[0]
    'unitCube'

      """
    def __init__(
        self, 
        label: str, 
        aFOne: analyticFunction,
        aFTwo: analyticFunction,
        splitDim: int = 0,
        splits: List[List[float]] = [[]],
        tEMeshBlockThickness: Optional[float] = None,
        meanplaneFromBlocks: bool = False,
        meanplaneExtOut: Optional[float] = 0.01,
        meanplaneExtIn: Optional[float] = 0.01,
        nMeanplaneBlocks: Optional[int] = 3
    ) -> None:
        """Constructor.
      
        Parameters
        ----------
        label: str
          Label.
        aFOne: analyticFunction
          First BSpline surface.
        aFTwo_: analyticFunction
          Second BSpline surface.
        splitDim: int
          Dimension where surface is splitted before it is skinned.
        splits: List[List[float]]
          Positions for splitting.
        tEMeshBlockThickness: Optional[float] = None
          thickness of the Trailing Edge Mesh Blocks, if not None
        meanplaneFromBlocks: bool = False
          activates creation of meanplane curve
        meanplaneExtOut: Optional[float] = 0.01,
          extention of meanplane curve towards outlet
        meanplaneExtIn: Optional[float] = 0.01,
          extention of meanplane curve towards inlet
        nMeanplaneBlocks: Optional[int] = 3
          number of block faces used for the meanplane

        Returns
        -------
        None
      
        """
        logging.info( "Initializing %s ..." % (label) )
        super(vec3dThreeD_skinAndSplit, self).__init__()
        self.label_ = label
        self.aFOne_ = vec3dSurfaceTwoD.MustDownCast( aFOne )
        self.aFTwo_ = vec3dSurfaceTwoD.MustDownCast( aFTwo )
        self.splitDim_ = splitDim
        self.splits_ = splits
        
        self.thickness_ = tEMeshBlockThickness
        self.meanplaneFromBlocks_ = meanplaneFromBlocks
        self.meanplaneExtOut_ = meanplaneExtOut
        self.meanplaneExtIn_ = meanplaneExtIn
        self.nMeanplaneBlocks_ = nMeanplaneBlocks
        if self.thickness_ != None:
            self.nMeanplaneBlocks_ = self.nMeanplaneBlocks_ - 1
        
        # adjust splits to corresponding parameter space
        if splits != [[]]:
          for split in self.splits_:
            cc = 0
            if self.splitDim_ == 0:
              split[0] = self.aFOne_.x_percent( split[0], 0.0 )[0]
              split[1] = self.aFOne_.x_percent( split[1], 0.0 )[0]
            elif self.splitDim_ == 1:
              split[0] = self.aFOne_.x_percent( 0.0, split[0] )[1]
              split[1] = self.aFOne_.x_percent( 0.0, split[1] )[1]

    def build(self) -> None:
        """Build part.

        Parameters
        ----------
        None

        Returns
        -------
        None

        """
        logging.info( "Building %s ..." % (self.label_) )

        if self.splits_ == [[]]: 
          vh_aF = vectorHandlingConstAnalyticFunction()
          vh_aF.push_back( self.aFOne_ )
          vh_aF.push_back( self.aFTwo_ )
          theRef = vec3dTransVolThreeD_skinBSplineSurfaces( vh_aF ).result()
          theRef.setLabel( self.label_ )
          self.appendAnalyticFunction( theRef )
        else:
          cc = 0
          for split in self.splits_:
            print("meshBlock nr : ", cc)
            logging.info( 
              "Split between %f <--> %f in direction %d" 
              % 
              (split[0], split[1], self.splitDim_) 
            )
            vh_aF = vectorHandlingConstAnalyticFunction()
            #vh_aF.push_back( 
            bladeSurf = vec3dSurfaceTwoD(
                bSplineSurface_bSplineSurfaceSplitConstructOCC(
                  self.aFOne_.constPtrDtSurface(), 
                  self.splitDim_, 
                  split[0], 
                  split[1]
                ).result()
            )
            vh_aF.push_back((bladeSurf).clone())
            #vh_aF.push_back( 
            blockSurf = vec3dSurfaceTwoD(
                bSplineSurface_bSplineSurfaceSplitConstructOCC(
                  self.aFTwo_.constPtrDtSurface(), 
                  self.splitDim_, 
                  split[0], 
                  split[1]
                ).result()
            )  
            vh_aF.push_back((blockSurf).clone())

            theRef = vec3dTransVolThreeD_skinBSplineSurfaces( vh_aF ).result()
            theRef.setLabel( self.label_+"_"+str(cc + 1) )
            self.appendAnalyticFunction( theRef )
            
            # generates the meanplaneextiention in sequence to the last defined
            #  meanplane block
            if (self.meanplaneFromBlocks_ == True and cc == self.nMeanplaneBlocks_):
                
                # face 3 of the mesh block is the one normal to the blade
                face = vec3dSurfaceTwoD.DownCast(
                    theRef.constPtrVec3dTwoD( 3 )
                )
                
                ## debug statement to plot analytical face
                #plotFace = vec3dSurfaceTwoD.DownCast(
                #        theRef.constPtrVec3dTwoD(
                #           3 
                #        )
                #    )
                #plotFace.setLabel("TEST_"+self.label_+"_plotFace")
                #self.appendAnalyticFunction(plotFace)
                
                # calculationg the curve tangentially offset curve
                mPBlockCurve, mPBlockOffset = self.teOffsetCurves_vec3dSurfaceTwoD(
                        face, 1, (self.meanplaneExtIn_), 1
                    ) 
                # creating the curve from a vectorDtPoint3 and appending AF
                mPBlockOffsetCurve = bSplineCurve_pointConstructOCC(mPBlockOffset,2).result()
                self.appendAnalyticFunction(vec3dCurveOneD( mPBlockOffsetCurve ) << self.label_ + "Curve_in1")
                self.appendAnalyticFunction(vec3dCurveOneD( mPBlockCurve ) << self.label_ + "Curve_in0")

            # generation of trailing edge mesh block, only done, if tEMeshBlockThickness is defined
            if self.thickness_ != None:
                
                # te mesh blocks will be located downstream of the first and last mesh blocks
                if cc == 0:
                    logging.info("Extracting trailing-edge and block curves from mesh block %d" % (cc+1))
                    # needed curves:
                    # curve on blade and block,
                    #  their offsets in the tangential direction of the blade and block surfaces
                    # segPercent specifies the u direction where the trailing edge is located on the faces,
                    #  this is different for the first and last blocks because the u direction "wraps"
                    #  around the blade
                    # offset curves for block and blade are calculated in two function calls
                    bladeCurve0, bladeOffset0 = self.teOffsetCurves_vec3dSurfaceTwoD(
                            bladeSurf, 0, self.thickness_, 0
                        )
                    blockCurve0, blockOffset0 = self.teOffsetCurves_vec3dSurfaceTwoD(
                            blockSurf, 0, self.thickness_, 0
                        )
                    
                    # special treatment if trailing edge mesh blocks exist
                    if self.meanplaneFromBlocks_ == True:
                        # the meanplane offset curve is calculated from the same blockSurf which was used
                        #  for the trailing edge mesh block
                        # the offset distance is (self.thickness_+self.meanplaneExtOut_)
                        mPBlockCurve, mPBlockOffset = self.teOffsetCurves_vec3dSurfaceTwoD(
                            blockSurf, 0, (self.thickness_+self.meanplaneExtOut_), 0
                        )
                        # mPBlockCurve is overwritten with blockOfsset of the trailing edge mesh block
                        mPBlockCurve = bSplineCurve_pointConstructOCC(blockOffset0,2).result()
                        # creating the curve from a vectorDtPoint3 and appending AF
                        mPBlockOffsetCurve = bSplineCurve_pointConstructOCC(mPBlockOffset,2).result()
                        self.appendAnalyticFunction(vec3dCurveOneD( mPBlockOffsetCurve ) << self.label_ + "Curve_out1")
                        self.appendAnalyticFunction(vec3dCurveOneD( mPBlockCurve ) << self.label_ + "Curve_out0")

                if cc == (len(self.splits_)-1):
                    logging.info("Extracting trailing-edge and block curves from mesh block %d" % (cc+1))
                    bladeCurve1, bladeOffset1, = self.teOffsetCurves_vec3dSurfaceTwoD(
                            bladeSurf, 1, self.thickness_, 0
                        )  
                    blockCurve1, blockOffset1 = self.teOffsetCurves_vec3dSurfaceTwoD(
                            blockSurf, 1, self.thickness_, 0
                        )  

            # first meamplane offset curve if no trailing edge mesh blocks exist
            if (self.thickness_ == None and self.meanplaneFromBlocks_ == True):
                if cc == 0:
                    # calculating offset curve
                    mPBlockCurve, mPBlockOffset = self.teOffsetCurves_vec3dSurfaceTwoD(
                        blockSurf, 0, (self.meanplaneExtOut_), 0
                    )
                    # creating the curve from a vectorDtPoint3 and appending AF
                    mPBlockOffsetCurve = bSplineCurve_pointConstructOCC(mPBlockOffset,2).result()
                    self.appendAnalyticFunction(vec3dCurveOneD( mPBlockOffsetCurve ) << self.label_ + "Curve_out1")
                    self.appendAnalyticFunction(vec3dCurveOneD( mPBlockCurve ) << self.label_ + "Curve_out0")


            cc = cc + 1
        
        if self.thickness_ != None:

            # calculating mean offset of the blade trailing edge.
            #  trailing edge converges at u = 0 and u = 1 in one curve,
            #  here two tangential directions are returned by the two calls 
            #  of teBlockCurves_vec3dSurfaceTwoD() (at the first and last meshBlocks)
            # from the two offsets a mean is calculated
            meanPoints = vectorDtPoint3()
            for i in range(len(bladeOffset0)):
                meanPoints.append(
                    dtPoint3(
                        (bladeOffset0[i][0]+bladeOffset1[i][0])*0.5,
                        (bladeOffset0[i][1]+bladeOffset1[i][1])*0.5,
                        (bladeOffset0[i][2]+bladeOffset1[i][2])*0.5,    
                    )
                )
            # creating the curve of the blade offset
            meanBladeOffsetCurve = bSplineCurve_pointConstructOCC(meanPoints,2).result()

            # creating the curves of the block offsets
            blockOffsetCurve_1 = bSplineCurve_pointConstructOCC(blockOffset1,2).result()
            blockOffsetCurve_0 = bSplineCurve_pointConstructOCC(blockOffset0,2).result()
            
            logging.info("Creating trailing edge mesh blocks")
            # creating the block faces by skinning the curves in the list
            #  the structure of the list is important for the directions (see description in function)
            vh_aF = self.createBlockFaces(
                    [[meanBladeOffsetCurve, bladeCurve0],
                     [blockOffsetCurve_0, blockCurve0]]
                ) 
            
            # generating the volume from the faces
            theRef = vec3dTransVolThreeD_skinBSplineSurfaces( vh_aF ).result()
            theRef.setLabel( self.label_+"_0" )
            self.appendAnalyticFunction( theRef )
            
            # doint the same for the te mesh block at u = 1
            vh_aF = self.createBlockFaces(
                    [[bladeCurve0, meanBladeOffsetCurve],
                     [blockCurve1, blockOffsetCurve_1]
                    ]
                ) 
            
            theRef = vec3dTransVolThreeD_skinBSplineSurfaces( vh_aF ).result()
            theRef.setLabel( self.label_+"_"+str(len(self.splits_)+1) )
            self.appendAnalyticFunction( theRef )
             
        return

    #
    # Extracts the necessary curves and their offsets for the generation of the trailing edge meshBlocks
    #  and the generation of the meanplane curves
    # segPercent is 0 for the first and 1 for the last mesh blocks 
    #
    def teOffsetCurves_vec3dSurfaceTwoD(self, surf, segPercent, blockThickness, splitDim):
        
        # direction of the offset is controlled by f
        if segPercent == 0:
            f = -1
        elif segPercent == 1:
            f = 1
        
        # changing type of the faces from aF to aS
        surf = analyticSurface(surf.constPtrDtSurface()) 
        
        # surface directions:
        #   splitDim = 0:
        #     v-direction is the hub-to-shroud direction 
        #     u-direction is along the blade contour
        #   splitDim = 1:
        #     v-direction is the blade skin to block skin direction
        #     u-direction is the hub to shroud direction

        # according to the split dimension which is set the curve is cut out
        #  and segPercent set on uu or vv
        if splitDim == 0:
            # extracting the curves of the blade and the block at the trailing edge
            #  segPercent is needed because of the different directions
            curve = analyticCurve.MustDownCast(
                    surf.segmentConstUPercent(segPercent)
                ).ptrConstDtCurve()
            uu = segPercent

        elif splitDim == 1:
            curve = analyticCurve.MustDownCast(
                    surf.segmentConstVPercent(segPercent)
                ).ptrConstDtCurve()
            vv = segPercent
        
        # getting number of control points of the curve 
        bsc_curve = dtOCCBSplineCurve.MustDownCast(curve)
        n = bsc_curve.nControlPoints()

        # initializing Point vectors for the offset curves 
        offsetPoints = vectorDtPoint3()
        
        # iterating over control points
        for i in range(n):
            
            # calculating the percentual vv or uu position by normalizing the number of
            #  controlpoints
            # calculating the tangential directions of the faces at the support points
            if splitDim == 0:    
                vv = i/(n-1)
                # tangential direction of the blade at the trailing edge
                tangent = dtLinearAlgebra.normalize(
                        surf.firstDerU(surf.u_percent(uu), surf.v_percent(vv))
                    )
            elif splitDim == 1:
                uu = i/(n-1)
                # tangential direction of the face normal between blade and thickness faces
                tangent = dtLinearAlgebra.normalize(
                        surf.firstDerV(surf.u_percent(uu), surf.v_percent(vv))
                    )
            
            # appending the blade point offset in tangential direction
            #  the offset will have the specified thickness, f sets the direction
            offsetPoints.append(
                    surf.getPoint(
                        surf.u_percent(uu), surf.v_percent(vv)
                    ) + tangent * blockThickness*f
                )
            
            ## Debug statement to plot tangents
            #tangentCurve = bSplineCurve_pointConstructOCC(
            #    vectorDtPoint3()
            #      << surf.getPoint(surf.u_percent(uu), surf.v_percent(vv))
            #      << surf.getPoint(surf.u_percent(uu), surf.v_percent(vv)) + tangent * blockThickness*f,
            #    1
            #).result()

            #tang = (vec3dCurveOneD( tangentCurve ) << self.label_ + "Curve_te"+str(uu)+"_"+str(i))
            #self.appendAnalyticFunction(tang)
                 
        
        # returns the curve on the surface and the vector with the offset points
        #  the offset points and not the offset curves are returned, because a 
        #  mean offset has to be calculated for trailing edge at the blade
        return curve, offsetPoints
    
    #
    # creating the faces for the trailing edge meshBlocks
    #
    def createBlockFaces(self, curves):
        # curves is a list in the following format:
        #  curves = [
        #       [curves0_0, curves0_1],
        #       [curves1_0, curves1_1]
        #     ]
        #  
        # the skinning is done in direction: 
        #   skinX = curvesX_0 -> curvesX_1
        # with bSplineSurface_exchangeSurfaceConstructOCC the u and the v direction
        #  of the resulting surface is exchanged
        # the u direction will extend from curvesX_0 -> curvesX_1 
        # the v direction will extend from the hub to the shroud direction
        #
        # by iterating over the curves list the skins of the blade (skin0) and the skin of the 
        #  block (skin1) are pushed in vh_aF
        # the volume will later be generated from vh_aF[0] (skin0) to vh_aF[1] (skin1)
        #  the resulting w direction will extend from the blade to the block surface
        
        vh_aF = vectorHandlingConstAnalyticFunction()
        for curves0 in curves:
            vh_aF.push_back(
                    vec3dSurfaceTwoD(
                        bSplineSurface_exchangeSurfaceConstructOCC(
                            bSplineSurface_skinConstructOCC(
                                curves0[0], curves0[1]    
                            ).result()
                        ).result()
                    ).clone()
                )

        return vh_aF

