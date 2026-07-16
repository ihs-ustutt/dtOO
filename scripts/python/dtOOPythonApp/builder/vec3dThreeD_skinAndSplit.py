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

class vec3dThreeD_skinAndSplit(dtBundleBuilder):

    """Create mesh blocks and meanplane curves from blade and surrounding surface.

    This class:

        - Splits blade and the mesh block surfaces at defined positions.
        - Skins mesh block volumes of the corresponding surfaces.
        - Creates a meanplane curves at the inlet and outlet.
        - Creates trailing edge mesh blocks.

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
    splits_: List[Tuple[float]]
      Positions for splitting.
    thickness_: float
      Thickness of the trailing edge mesh blocks.
    meanplaneFromBlocks_: bool
      Activates creation of meanplane curve.
    meanplaneExtOut_: float
      Extention of meanplane curve towards outlet.
    meanplaneExtIn_: float
      Extention of meanplane curve towards inlet.
    nMeanplaneBlocks_: int
      Number of block faces used for the meanplane.

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
    
    The main method of this class is :meth:`build`.

    The analytic functions of the blade surface ``aFOne_`` and the surrounding
    surface ``aFTwo_`` are split at the positions specified in ``splits_``.
    The input parameter ``splitDim_`` specifies the parameter direction in which
    the surfaces are split. A value of ``0`` corresponds to splitting along the
    u-parameter direction.

    The format of ``splits_`` is ``List[Tuple[float, float]]``. Each entry
    ``splits_[i]`` corresponds to one resulting mesh block. A mesh block is
    created by splitting the surfaces ``aFOne_`` and ``aFTwo_`` between the
    normalized minimum and maximum parameter values specified by
    ``splits_[i][0]`` and ``splits_[i][1]``.

    Depending on whether ``splitDim_`` specifies the u- or v-direction, the
    values in ``splits_`` are converted into the corresponding surface parameter
    values.

    The mesh block volumes are created by skinning the resulting surfaces.

    Two trailing edge mesh blocks can additionally be created with a thickness
    specified by ``teMeshBlockThickness_``. If this value is set to ``None``,
    no trailing edge mesh blocks are generated.

    Otherwise, the trailing edge mesh blocks are created by computing a
    tangential offset of the edges of the first and last mesh blocks surrounding
    the blade. The offset edges are computed using the method
    :meth:`teOffsetCurves_vec3dSurfaceTwoD`. This method returns the base curve
    and a list of points, derived from the base curve, from which the offset curve 
    can be constructed.

    By skinning the base and offset curves, faces extending from the first and
    last trailing edge mesh blocks are generated. A second skinning operation 
    is then used to create the trailing edge mesh block volumes. To maintain 
    consistent parameter orientations between the trailing edge mesh blocks and 
    the other mesh blocks, the skinning operation is performed in
    :meth:`createBlockFaces`.

    Meanplane curves are generated if the Boolean parameter
    ``meanplaneFromBlocks`` is set to ``True``. Similar to the trailing edge
    mesh block edges, the meanplane curves are computed as tangential offsets of
    the blade mesh blocks using :meth:`teOffsetCurves_vec3dSurfaceTwoD`.

    The first meanplane curve is offset from the first blade mesh block towards
    the outlet of the regular channel.

    The second meanplane curve is offset from the ``n``-th blade mesh block,
    specified by ``nMeanplaneBlocks_``, towards the inlet.
    ``nMeanplaneBlocks_`` defines the index of the mesh block from which the
    meanplane curve is generated, starting at zero.

    The extension lengths of the offsets for the two meanplane curves are
    specified by ``meanplaneExtOut_`` and ``meanplaneExtIn_``.

    The string value stored in ``label_`` is used to label the generated
    analytic functions.

    The analytic function objects of the blade mesh blocks, trailing edge mesh
    blocks, and meanplane curves are added to the container object of the main
    class using :meth:`appendAnalyticFunction`.
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
        
        This method:

            - Splits the blade and surrounding mesh block surfaces at predefined
              positions.
            - Creates mesh block volumes by skinning the corresponding surfaces.
            - Generates meanplane curves at the inlet and outlet.
            - Creates trailing edge mesh blocks.

        Parameters
        ----------
        None

        Returns
        -------
        None


        If no split positions are defined (``splits_ == [[]]``), the two surfaces
        are skinned directly, resulting in a single continuous mesh block.

        If split positions are defined, the mesh blocks are created by iterating
        over the entries in ``splits_``. Within this loop, the following entities
        are generated:

            - Mesh block volumes around the blade.
            - Edge curves for the trailing edge mesh blocks.
            - Meanplane curves extending towards the inlet and outlet.

        All geometries are constructed as analytic functions. The loop iteration is
        tracked using the counter ``cc``.

        The following figure illustrates the workflow of the operations performed
        within this loop.

        .. _vec3d_activity0:
        .. figure:: bladeFigs/vec3dThreeD_skinAndSplit.png
           :width: 100%
           :align: center

           Workflow of the main loop in :meth:`vec3dThreeD_skinAndSplit`.

        **Iterate over Splits**
            
            ::

                for split in splits_
            
            All the following operations are performed in the main loop.

            **Split the Surfaces and Skin the Volumes**

                For each entry in ``splits_``, the surface functions ``aFOne_`` and
                ``aFTwo_`` are split within the parameter range defined by
                ``split[0]`` and ``split[1]``. This operation is performed using the
                `dtOO` class `bSplineSurface_bSplineSurfaceSplitConstructOCC`.

                The resulting surfaces are stored in the variables ``bladeSurf`` and
                ``blockSurf`` as ``vec3dSurfaceTwoD`` objects. These surfaces are then
                skinned into a volume using the `dtOO` class
                `vec3dTransVolThreeD_skinBSplineSurfaces` and appended to the
                container.

                The following figure illustrates the blade and the surrounding blade
                mesh blocks. In the following documentation, the direction specified by
                ``splitDim_`` corresponds to the u-direction of the blade surface.

                .. _meshBlockMeth:
                .. figure:: bladeFigs/guideVane_meshBlocks.png
                   :width: 50%
                   :align: center

                   Blade surface (grey) surrounded by mesh blocks.

                By skinning the blade surfaces with the surrounding surfaces, the
                resulting volume parameter directions are defined as follows:

                    - u: direction along the blade surface
                    - v: direction from hub to shroud
                    - w: direction from the blade surface towards the surrounding
                      surface

                The volumes surrounding the blade are labeled according to the following
                convention:

                ::

                    label_ + "_" + str(cc + 1)

                With this convention, the first blade mesh block in the direction of
                ``splitDim_`` is assigned the suffix ``_1``.
            
            
            **Creation of Offset Curves**

                The generation of the trailing edge mesh block curves, as well as the
                creation of the meanplane curves, is performed using the method
                :meth:`teOffsetCurves_vec3dSurfaceTwoD`.

                The method takes a mesh block surface as input. The first input argument
                defines the normalized parametric position of the base curve on the
                surface. An integer argument specifies the parameter direction in which
                the base curve is extracted and offset. By providing a thickness value,
                the offset distance is defined.

                The method returns the base curve together with a ``vectorDtPoint3``
                object containing the points of the offset curve.

                Several conditional checks are performed within the loop to ensure that
                the correct curves are generated.

            **Trailing-edge mesh block curves**

                The following condition applies:

                ::

                    thickness_ != None

                Trailing-edge mesh blocks are generated only if
                ``thickness_ != None``. In this case, offset curves are computed for the
                surfaces ``bladeSurf`` and ``blockSurf`` of the first mesh block
                (``cc == 0``) and the last mesh block
                (``cc == len(splits_) - 1``).

                The following variable names are assigned to the geometry objects
                returned by :meth:`teOffsetCurves_vec3dSurfaceTwoD`:

                    - ``bladeCurve0``: Base curve on the blade surface of the first blade mesh block.
                    - ``bladeOffset0``: ``vectorDtPoint3`` object containing the offset points of ``bladeCurve0``.
                    - ``blockCurve0``: Base curve on the block surface of the first blade mesh block.
                    - ``blockOffset0``: ``vectorDtPoint3`` object containing the offset points of ``blockCurve0``.
                    - ``bladeCurve1``: Base curve on the blade surface of the last blade mesh block.
                    - ``bladeOffset1``: ``vectorDtPoint3`` object containing the offset points of ``bladeCurve1``.
                    - ``blockCurve1``: Base curve on the block surface of the last blade mesh block.
                    - ``blockOffset1``: ``vectorDtPoint3`` object containing the offset points of ``blockCurve1``.

                The following figure illustrates the resulting curves for the first mesh
                block surrounding the blade. The variable :math:`t_{TE}` corresponds to
                the parameter ``thickness_``.

                .. _meshBlockCurves:
                .. figure:: bladeFigs/tEMesBlock_Curves.png
                   :width: 70%
                   :align: center

                   Trailing-edge mesh block curves of the first mesh block
                   (``cc == 0``).


            **Meanplane curves**

                The following condition applies:

                ::

                    meanplaneFromBlocks_ == True

                The creation of the meanplane curves differs between two main cases.

                **Meanplane extending towards the inlet**

                    In the first case, the meanplane curves extending towards the inlet are
                    generated. These curves are created during the iteration for which
                    ``cc == nMeanplaneBlocks_`` applies.

                    The curves are based on the third face (``face``) of the mesh 
                    block. The offset curve is constructed from the base curve located 
                    at 100 percent of the v-direction of this face.

                    The offset length of the curve is specified by ``meanplaneExtIn_``,
                    corresponding to :math:`E_{MP,in}` in :numref:`meanplaneCurves`.

                    The returned geometry objects are assigned to the variables
                    ``mPBlockCurve`` and ``mPBlockOffset``. The ``vectorDtPoint3`` object
                    ``mPBlockOffset`` is converted into a curve and stored in
                    ``mPBlockOffsetCurve``.

                    The curves are labeled and appended to the container using the following
                    naming convention:

                        - ``mPBlockCurve``: ``label_ + "Curve_in0"``
                        - ``mPBlockOffsetCurve``: ``label_ + "Curve_in1"``

                    The following figure illustrates the creation of the meanplane curve
                    geometries for the meanplanes extending towards the inlet and the
                    outlet. In this example, the inlet meanplane is located at 
                    ``nMeanplaneBlocks_ = 3``. Trailing-edge mesh blocks are enabled.

                    .. _meanplaneCurves:
                    .. figure:: bladeFigs/meanplane_Curves.png
                       :width: 100%
                       :align: center

                       Creation of the meanplane curves extending towards the inlet and the
                       outlet. The block numbers correspond to the mesh block indices and
                       the value of ``cc + 1`` within the loop.

                **Meanplane extending towards the outlet**

                    The curves for the meanplane extending towards the outlet are created on
                    the first blade mesh block, for which ``cc == 0`` applies.

                    The creation procedure differs depending on whether trailing edge mesh
                    blocks are generated.

                    If no trailing edge mesh blocks are created
                    (``thickness_ == None``), the curves are generated from the curve at
                    0 percent of the u-direction of ``blockSurf``. The offset length is
                    specified by ``meanplaneExtOut_``, corresponding to
                    :math:`E_{MP,out}` in :numref:`meanplaneCurves`.

                    The generated curve objects are assigned to the variables
                    ``mPBlockCurve`` and ``mPBlockOffset``.

                    If trailing edge mesh blocks are generated, the offset length of the
                    meanplane curve created from the first blade mesh block is 
                    ``meanplaneExtOut_ + thickness_``

                    In this case, the base curve returned by
                    :meth:`teOffsetCurves_vec3dSurfaceTwoD` is not used directly as the
                    meanplane curve. Instead, the base curve is reconstructed from the
                    offset points of the first trailing edge mesh block,
                    ``blockOffset0``. The resulting curve overwrites ``mPBlockCurve``.

                    In both cases, the curve ``mPBlockOffsetCurve`` is generated from the
                    ``vectorDtPoint3`` object ``mPBlockOffset``.

                    The curves are appended to the container as ``vec3dCurveOneD`` objects
                    using the following naming convention:

                        - ``mPBlockCurve``: ``label_ + "Curve_out0"``
                        - ``mPBlockOffsetCurve``: ``label_ + "Curve_out1"``

                The meanplane curves appended to the container in the selected example
                are shown in the following figure.

                .. _meanplaneCP:
                .. figure:: bladeFigs/meanplane_CurvesPushed.png
                   :width: 90%
                   :align: center

                   Final meanplane curves generated in this method. The labels
                   ``in0``, ``in1``, ``out0``, and ``out1`` correspond to the naming
                   convention used for the curves in the implementation.

            After each iteration of the loop, the iterator is incremented 
            (``cc = cc + 1``)
        
        **Trailing Edge Mesh Blocks**

            The trailing edge volumes are generated from the trailing edge curves.
            The following operations are performed only if ``thickness_ != None`` 
            applies.
            
            **Offset Curves at Trailing Edge**

                From the ``vectorDtPoint3`` objects of the two trailing edge mesh blocks
                on the blade side, ``bladeOffset0`` and ``bladeOffset1``, the mean
                points ``meanPoints`` are computed. Using these points, a mean offset
                curve ``meanBladeOffsetCurve`` is generated, which defines the offset
                surface of the trailing edge.

                The ``vectorDtPoint3`` objects of the block offset curves are converted into
                the curves ``blockOffsetCurve_0`` and ``blockOffsetCurve_1``.
            
            **Trailing Edge Mesh Block Surfaces**

                The trailing edge mesh block surfaces extending from the blade and from the mesh block surfaces 
                are generated using the method :meth:`createBlockFaces`. The method expects an input 
                list with the following structure:

                .. code-block:: python

                    blockEdges = Tuple[
                        Tuple[
                            analyticGeometry, analyticGeometry
                        ],
                        Tuple[
                            analyticGeometry, analyticGeometry
                        ]
                    ]

                The method returns a
                ``vectorHandlingConstAnalyticFunction`` object ``vh_aF`` containing
                analytic surface functions generated by skinning the curves within each
                second-level ``Tuple`` entry.

                The skinning directions are defined as follows:

                    - ``vh_aF[0]``: from ``blockEdges[0][0]`` to ``blockEdges[0][1]``
                    - ``vh_aF[1]``: from ``blockEdges[1][0]`` to ``blockEdges[1][1]``

                By applying the `dtOO` class `bSplineSurface_exchangeSurfaceConstructOCC` 
                on the skinned surface in the method, the parameter directions of the
                surfaces are kept consistent with the blade and mesh block surfaces.
            
            **Trailing Edge Mesh Block Volumes**

                The returned vector handler is passed to the ``dtOO`` class
                ``vec3dTransVolThreeD_skinBSplineSurfaces`` to create the trailing edge
                mesh block volumes ``theRef``.

                The volume skinning direction is defined as follows:

                    - ``theRef``: from ``vh_aF[0]`` to ``vh_aF[1]``

                The following figure illustrates the skinning of the curves and surfaces.

                .. _meshBlockSkinning:
                .. figure:: bladeFigs/tEMesBlock_skinning.png
                   :width: 80%
                   :align: center

                   Skinning of the first trailing edge mesh block. Surface skinning is
                   shown on the left, and volume skinning on the right. The arrows
                   indicate the skinning directions.

                By arranging the order of the curves in the input list passed to
                :meth:`createBlockFaces`, the parameter directions of the trailing edge
                mesh block volumes remain consistent with those of the blade mesh
                blocks.

                The generated volumes are appended to the analytic function container
                using the same naming convention as the blade mesh blocks.

                The first trailing edge mesh block is labeled with the index zero:

                    ``label_ + "_0"``

                The last trailing edge mesh block receives the label:

                    ``label_ + "_" + str(len(splits_) + 1)``

                The following figure shows the resulting mesh block volumes.

                .. _TEmeshBlockMeth:
                .. figure:: bladeFigs/guideVane_TEmeshBlocks.png
                   :width: 50%
                   :align: center

                   Blade surface (grey) with blade mesh blocks and trailing edge mesh
                   blocks.

        """
        logging.info( "Building %s ..." % (self.label_) )
                                        
        if self.splits_ == [[]]: 
          #
          # if no split positions are defined, one volume is created
          #
          vh_aF = vectorHandlingConstAnalyticFunction()
          vh_aF.push_back( self.aFOne_ )
          vh_aF.push_back( self.aFTwo_ )
          theRef = vec3dTransVolThreeD_skinBSplineSurfaces( vh_aF ).result()
          theRef.setLabel( self.label_ )
          self.appendAnalyticFunction( theRef )
        else:
          #
          # splits are defined
          #
          cc = 0

          #
          # iterate over the split positions
          #
          for split in self.splits_:
            logging.info("meshBlock nr : %d" % cc)
            logging.info( 
              "Split between %f <--> %f in direction %d" 
              % 
              (split[0], split[1], self.splitDim_) 
            )
            vh_aF = vectorHandlingConstAnalyticFunction()
            
            #
            # split the blade surface
            #
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
            
            #
            # split the surrounding surface
            #
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
            
            #
            # skin the split surfaces to create the mesh block volume
            #
            theRef = vec3dTransVolThreeD_skinBSplineSurfaces( vh_aF ).result()
            # trailing edge active
            if self.thickness_ != None:
                theRef.setLabel( self.label_+"_"+str(cc + 1) )
            # trailing edge not active
            else:
                theRef.setLabel( self.label_+"_"+str(cc) )
            self.appendAnalyticFunction( theRef )
            
            #
            # generate trailing edge mesh block
            # only done, if thickness_ is defined
            #
            if self.thickness_ != None:
                
                #
                # te mesh blocks will be located downstream of the first and last mesh blocks
                #
                # first mesh block
                if cc == 0:
                    logging.info("Extracting trailing edge and block curves from mesh block %d" % (cc+1))
                    #
                    # needed curves:
                    # curve on blade and block,
                    #  their offsets in the tangential direction of the blade and block surfaces
                    # segPercent specifies the u direction where the trailing edge is located on the faces,
                    #  this is different for the first and last blocks because the u direction "wraps"
                    #  around the blade
                    # offset curves for block and blade are calculated in two function calls
                    #
                    bladeCurve0, bladeOffset0 = self.teOffsetCurves_vec3dSurfaceTwoD(
                            bladeSurf, 0, self.thickness_, 0
                        )
                    blockCurve0, blockOffset0 = self.teOffsetCurves_vec3dSurfaceTwoD(
                            blockSurf, 0, self.thickness_, 0
                        )
                    
                # last mesh block
                if cc == (len(self.splits_)-1):
                    logging.info("Extracting trailing edge and block curves from mesh block %d" % (cc+1))
                    bladeCurve1, bladeOffset1, = self.teOffsetCurves_vec3dSurfaceTwoD(
                            bladeSurf, 1, self.thickness_, 0
                        )  
                    blockCurve1, blockOffset1 = self.teOffsetCurves_vec3dSurfaceTwoD(
                            blockSurf, 1, self.thickness_, 0
                        )
            
            #
            # meanplane extention curves
            #
            if self.meanplaneFromBlocks_ == True: 
                
                #
                # meanplane curves extending to the inlet
                #
                if cc == self.nMeanplaneBlocks_:
                
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
                    
                    # calculate the tangentially offset curve based on the face
                    mPBlockCurve, mPBlockOffset = self.teOffsetCurves_vec3dSurfaceTwoD(
                            face, 1, (self.meanplaneExtIn_), 1
                        ) 
                    # create the curve from a vectorDtPoint3 and append AF
                    mPBlockOffsetCurve = bSplineCurve_pointConstructOCC(mPBlockOffset,2).result()
                    self.appendAnalyticFunction(vec3dCurveOneD( mPBlockOffsetCurve ) << self.label_ + "Curve_in1")
                    self.appendAnalyticFunction(vec3dCurveOneD( mPBlockCurve ) << self.label_ + "Curve_in0")
                
                #
                # meanplane curves extending to the outlet
                #
                elif cc == 0: 

                    if self.thickness_ == None:
                        # calculating offset curve
                        mPBlockCurve, mPBlockOffset = self.teOffsetCurves_vec3dSurfaceTwoD(
                            blockSurf, 0, (self.meanplaneExtOut_), 0
                        )
                    
                    else:
                        # the meanplane offset curve is calculated from the same blockSurf which is used
                        #  for the trailing edge mesh block
                        # the offset distance is (self.thickness_+self.meanplaneExtOut_)
                        mPBlockCurve, mPBlockOffset = self.teOffsetCurves_vec3dSurfaceTwoD(
                            blockSurf, 0, (self.thickness_+self.meanplaneExtOut_), 0
                        )
                        # mPBlockCurve is overwritten with blockOfsset of the trailing edge mesh block
                        mPBlockCurve = bSplineCurve_pointConstructOCC(blockOffset0,2).result()
                    
                    # create the curve from a vectorDtPoint3 and appending AF
                    mPBlockOffsetCurve = bSplineCurve_pointConstructOCC(mPBlockOffset,2).result()
                    self.appendAnalyticFunction(vec3dCurveOneD( mPBlockOffsetCurve ) << self.label_ + "Curve_out1")
                    self.appendAnalyticFunction(vec3dCurveOneD( mPBlockCurve ) << self.label_ + "Curve_out0")
                
                else:
                    pass

            cc = cc + 1
        
        #
        # Create the trailing edge mesh block volumes
        #
        if self.thickness_ != None:
            #
            # calculate mean offset of the blade trailing edge.
            #  trailing edge converges at u = 0 and u = 1 in one curve,
            #  here two tangential directions are returned by the two calls 
            #  of teBlockCurves_vec3dSurfaceTwoD() (at the first and last meshBlocks)
            # from the two offsets a mean is calculated
            #
            meanPoints = vectorDtPoint3()
            for i in range(len(bladeOffset0)):
                meanPoints.append(
                    dtPoint3(
                        (bladeOffset0[i][0]+bladeOffset1[i][0])*0.5,
                        (bladeOffset0[i][1]+bladeOffset1[i][1])*0.5,
                        (bladeOffset0[i][2]+bladeOffset1[i][2])*0.5,    
                    )
                )
            # create the curve of the blade offset
            meanBladeOffsetCurve = bSplineCurve_pointConstructOCC(meanPoints,2).result()

            # create the curves of the block offsets
            blockOffsetCurve_1 = bSplineCurve_pointConstructOCC(blockOffset1,2).result()
            blockOffsetCurve_0 = bSplineCurve_pointConstructOCC(blockOffset0,2).result()
            
            logging.info("Creating trailing edge mesh blocks")
            # create the block faces by skinning the curves in the list
            #  the structure of the list is important for the directions (see description in function)
            vh_aF = self.createBlockFaces(
                    [[meanBladeOffsetCurve, bladeCurve0],
                     [blockOffsetCurve_0, blockCurve0]]
                ) 
            
            # generate the volume from the faces
            theRef = vec3dTransVolThreeD_skinBSplineSurfaces( vh_aF ).result()
            theRef.setLabel( self.label_+"_0" )
            self.appendAnalyticFunction( theRef )
            
            # do the same for the te mesh block at u = 1
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
        """Extract a curve on a face and calculate points, which are tangentially offset
        to the face.

        This method:

            - Extracts a curve from a surface at a constant parameter value.
            - Computes tangential offset points along the curve span.
            - Returns the extracted curve together with the offset points.

        Parameters
        ----------
        surf : vec3dSurfaceTwoD
            Input surface.

        segPercent : float
            Normalized parameter position at which the curve is extracted from
            the surface.

        blockThickness : float
            Offset distance used for the tangential offset curve.

        splitDim : int
            Parameter direction in which the curve is extracted
            (u- or v-direction).

        Returns
        -------
        curve : analyticGeometry
            Curve extracted from the surface at ``segPercent`` in the direction
            specified by ``splitDim``.

        offsetPoints : vectorDtPoint3
            Container holding the points of the tangentially offset curve.


        The surface from which the curve is extracted is provided through the
        parameter ``surf``. The normalized parameter position ``segPercent``
        defines where the curve is extracted. It can be either ``0`` or
        ``1``, corresponding to the minimum or maximum parameter boundary of the
        surface.

        The parameter ``splitDim`` specifies whether the extraction is performed
        in the u- or v-direction of the surface. The following convention is
        used:

            - u-direction: ``0``
            - v-direction: ``1``

        The parameter ``blockThickness`` defines the offset distance.

        The following figure illustrates the workflow of this method.

        .. _createOffset_activity0:
        .. figure:: bladeFigs/createOffsetCurves.png
           :width: 60%
           :align: center

           Workflow of method :meth:`teOffsetCurves_vec3dSurfaceTwoD`.
        
        **Set Direction**

            Depending on the value of ``segPercent``, the offset direction factor
            ``f`` is assigned either ``-1`` or ``1``.
        
        **Check splitDim**

            Depending on ``splitDim``, the base curve ``curve`` is extracted from
            ``surf`` at either a constant u-parameter or a constant v-parameter.
            The normalized position ``segPercent`` is assigned to either ``uu`` or
            ``vv`` as the constant parameter value.
        
        **Get Number of Control Points and Create Container**

            The extracted ``curve`` is converted into a B-spline curve using the
            ``dtOO`` class ``dtOCCBSplineCurve``. This allows the control point
            count of the curve to be queried and stored in ``n``.

            The output container ``offsetPoints`` of type ``vectorDtPoint3`` is then
            initialized.
        
        **Iterate over Control Points**

            The offset points are computed by iterating over the control point
            indices. Depending on ``splitDim``, tangent vectors in either the
            u-direction or the v-direction of the surface are evaluated at the
            surface coordinates defined by ``uu`` and ``vv``.

            One parameter value remains constant, while the second parameter value
            is computed from the normalized iterator expression ``i / (n - 1)``.

            The offset point is computed as the surface point evaluated at
            ``(uu, vv)`` plus the corresponding tangent vector multiplied by
            ``blockThickness`` and the direction factor ``f`` (see :numref:`meshBlockCurves`).

            Each computed point is appended to ``offsetPoints``.

            The method returns ``curve`` and ``offsetPoints``.
        """
   
        # set direction f of the offset based on splitDim
        if segPercent == 0:
            f = -1
        elif segPercent == 1:
            f = 1
        
        # convert the surface to an analytic surface
        surf = analyticSurface(surf.constPtrDtSurface()) 
        
        # 
        # Depending on splitDim the operations are performed in different 
        # surface directions
        #
        # surface directions:
        #   splitDim = 0:
        #     v-direction is the hub-to-shroud direction 
        #     u-direction is along the blade contour
        #   splitDim = 1:
        #     v-direction is the blade skin to block skin direction
        #     u-direction is the hub to shroud direction

        # according to the split dimension which is set the curve is cut out
        # and segPercent set on uu or vv
        if splitDim == 0:
            # u-direction
            curve = analyticCurve.MustDownCast(
                    surf.segmentConstUPercent(segPercent)
                ).ptrConstDtCurve()
            uu = segPercent

        elif splitDim == 1:
            # v-direction
            curve = analyticCurve.MustDownCast(
                    surf.segmentConstVPercent(segPercent)
                ).ptrConstDtCurve()
            vv = segPercent
        
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
                 
        
        # return the curve on the surface and the vector with the offset points
        # the offset points and not the offset curves are returned, because a 
        # mean offset has to be calculated for trailing edge at the blade
        return curve, offsetPoints
    
    #
    # creating the faces for the trailing edge meshBlocks
    #
    def createBlockFaces(self, curves):
        """Create trailing edge block faces by skinning edge curves.
        
        This method:

            - Iterates over the first-level curve list.
            - Skins the curve pairs defined in the second-level lists.
            - Returns the generated surfaces in a container.

        Parameters
        ----------
        curves : Tuple[Tuple[analyticGeometry, analyticGeometry],Tuple[analyticGeometry, analyticGeometry]]
            Collection of curve pairs to be skinned with each other.

        Returns
        -------
        vh_aF : vectorHandlingConstAnalyticFunction
            Container holding the analytic functions of the generated surfaces.


        The curves to be skinned are provided to the method in the following
        format:

        .. code-block:: python

            curves = Tuple[
                Tuple[
                    analyticGeometry, analyticGeometry
                ],
                Tuple[
                    analyticGeometry, analyticGeometry
                ]
            ]

        The method initializes a
        ``vectorHandlingConstAnalyticFunction`` object named ``vh_aF``.

        The skinning operation is performed by iterating over the first-level
        ``Tuple`` using:

        .. code-block:: python

            for curves0 in curves:

        For each entry ``curves0``, the skinning operation is performed from
        ``curves0[0]`` to ``curves0[1]``.

        After skinning, the ``dtOO`` class
        ``bSplineSurface_exchangeSurfaceConstructOCC`` is applied in order to
        exchange the parameter directions of the resulting surface.

        This produces the following parameter directions on the surface:

            - u: direction from ``curves0[0]`` to ``curves0[1]``
            - v: direction from hub to shroud

        The resulting surfaces are converted into ``vec3dSurfaceTwoD`` objects
        and appended to ``vh_aF``.

        Finally, the generated surfaces are returned through ``vh_aF``.
        """
        
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

