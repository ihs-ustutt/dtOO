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

from dtOOPythonSWIG import analyticCurve
from dtOOPythonSWIG import bSplineCurve_pointConstructOCC
from dtOOPythonSWIG import bSplineSurface_bSplineCurveFillConstructOCC 
from dtOOPythonSWIG import bSplineSurface_exchangeSurfaceConstructOCC 
from dtOOPythonSWIG import vectorDtPoint3 
from dtOOPythonSWIG import dtPoint3 

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
        tEMeshBlockThickness: Optional[float] = None
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
                    bladeCurve0, blockCurve0, bladeOffset0, blockOffset0 = self.teBlockCurves_vec3dSurfaceTwoD(
                            bladeSurf, blockSurf, 0, self.thickness_
                        )

                if cc == (len(self.splits_)-1):
                    logging.info("Extracting trailing-edge and block curves from mesh block %d" % (cc+1))
                    bladeCurve1, blockCurve1, bladeOffset1, blockOffset1 = self.teBlockCurves_vec3dSurfaceTwoD(
                            bladeSurf, blockSurf, 1, self.thickness_
                        )  

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
    # segPercent is 0 for the first and 1 for the last mesh blocks 
    #
    def teBlockCurves_vec3dSurfaceTwoD(self, bladeSurf, blockSurf, segPercent, blockThickness):
        
        # changing type of the faces from aF to aS
        bladeSurf = analyticSurface(bladeSurf.constPtrDtSurface())
        blockSurf = analyticSurface(blockSurf.constPtrDtSurface())
        
        # setting the directions for the tangents
        # the first blocks u direction is oriented away from the trailing edge
        # the second blocks u direction is oriented to the trailing edge
        if segPercent == 0:
            f = -1
        elif segPercent == 1:
            f = 1
        
        # trailing edge mesh blocks are only possible for the splitDim 0
        if self.splitDim_ == 0:

            # extracting the curves of the blade and the block at the trailing edge
            #  segPercent is needed because of the different directions
            bladeCurve = analyticCurve.MustDownCast(
                    bladeSurf.segmentConstUPercent(segPercent)
                ).ptrConstDtCurve()
            blockCurve = analyticCurve.MustDownCast(
                    blockSurf.segmentConstUPercent(segPercent)
                ).ptrConstDtCurve()
            
            # initializing Point vectors for the offset curves 
            bladeOffsetPoints = vectorDtPoint3()
            blockOffsetPoints = vectorDtPoint3()
            
            # the offset curves will be created from the offsets at those support points
            supports = [0.00, 0.33, 0.66, 1.00]
            for vv in supports:
                # v-direction is the hub-to-shroud direction with splitDim = 0 
                # u-direction is along the blade contour

                # tangential direction of the blade at the trailing edge
                bladeTangent = dtLinearAlgebra.normalize(
                        bladeSurf.firstDerU(bladeSurf.u_percent(segPercent), vv)
                    )
                # appending the blade point offset in tangential direction
                #  the offset will have the specified thickness, f sets the direction
                bladeOffsetPoints.append(
                        bladeSurf.getPoint(
                            bladeSurf.u_percent(segPercent), vv
                        ) + bladeTangent * blockThickness*f
                    )
                
                # doint the same for the block
                blockTangent = dtLinearAlgebra.normalize(
                        blockSurf.firstDerU(blockSurf.u_percent(segPercent), vv)
                    )
                blockOffsetPoints.append(
                        blockSurf.getPoint(
                            blockSurf.u_percent(segPercent), vv
                        ) + blockTangent * blockThickness*f
                    )
        
        # for splitDim_ = 1 not implimented
        else:
            raise ValueError("Not yet implemented")
        
        # returns the curves and the vectors with the offset points
        #  the points and not offset curves are returned, 
        #  because a mean offset has to be calculated for the blade
        return bladeCurve, blockCurve, bladeOffsetPoints, blockOffsetPoints

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

