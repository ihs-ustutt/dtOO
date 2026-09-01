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

from dtOOPythonSWIG import jsonPrimitive
from dtOOPythonSWIG import analyticGeometry
from dtOOPythonSWIG import map2dTo3d
from dtOOPythonSWIG import map3dTo3d
from dtOOPythonSWIG import map3dTo3dGmsh
from dtOOPythonSWIG import bVOMeshRule
from dtOOPythonSWIG import bVOWriteMSH
from dtOOPythonSWIG import bVOReadMSH
from dtOOPythonSWIG import bVONameRegions
from dtOOPythonSWIG import bVOOrientCellVolumes
from dtOOPythonSWIG import bool_map1dTo3dInMap2dTo3d
from dtOOPythonSWIG import dtPoint3
from dtOOPythonSWIG import dtVector3
from dtOOPythonSWIG import bVOAnalyticGeometryToFace
from dtOOPythonSWIG import labeledVectorHandlingAnalyticGeometry
from dtOOPythonSWIG import scaOneD
from dtOOPythonSWIG import bVOFaceToPatchRule
from dtOOPythonSWIG import vectorInt
from dtOOPythonSWIG import dtGmshModel
from dtOOPythonSWIG import vectorReal
from dtOOPythonSWIG import scaTanhGradingOneD
from dtOOPythonSWIG import scaTanhGradingOneDCompound
from dtOOPythonSWIG import sca3PPointsBSplineOneD
from dtOOPythonSWIG import sca3PPointsBSplineOneDCompound
from dtOOPythonSWIG import bVOSetPrescribedElementSize
from dtOOPythonSWIG import bVOSetRotationalPeriodicity
from dtOOPythonSWIG import xYz_rPhiZ
from dtOOPythonSWIG import baseContainer
from dtOOPythonSWIG import multipleBoundedSurface 
from dtOOPythonSWIG import bVODumpModel

import numpy as np
from typing import List, Tuple, Union, Dict
import logging
import math

class map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks(dtBundleBuilder):
    """Create mesh's topology as map3dTo3dGmsh.
    
    This class:

        - Creates the mesh toploogy of bladed channels
        - Adds the geometries of the unstructured region as a multiple bounded volume
          and a list of bounding surfaces.
        - Adds the mesh blocks.
        - Applies mesh settings.
        - Returns the toplology into the container of the calling class.
      
    Attributes
    ----------
    label_: str
      Label.
    channel_: multipleBoundedVolume
      Channel.
    channelFaces_: List[ map2dTo3d ]
      List of bounding faces surrounding the channel
    blocks_: List[ map3dTo3d ]
      List of mesh blocks surrounding the blade.
    nMeanplaneBlocks_: int 
      Number of block faces which are part of the meanplane
    blade_: map2dTo3d
      Blade.
    nBoundaryLayers_: int
      Number of boundary layers.
    nElementsSpanwise_: int
      Number of elements in spanwise direction.
    nElementsNormal_: int
      Number of elements on the blade surface.
    firstElementSizeHubToShroud_: float
      Size of first element on hub and shroud.
    firstElementSizeNormalBlade_: float
      Size of first element at the blade in normal to the blade direction.
    bladeHubElementSize_: scaOneD
      Function describing the element size versus the standardized unwrapped length of the blade at the hub.
    bladeHubElementScale_: float
      Factor defining the number of elements at the hub for each mesh block.
    bladeShroudElementSize_: scaOneD
      Function describing the element size versus the standardized unwrapped length of the blade at the shroud.
    bladeShroudElementScale_: float
      Factor defining the number of elements at the shroud for each mesh block.
    meshTEBlocks_: Bool
      Marker if trailing edge mesh blocks should be meshed
    map3dTo3dGmshJson_: jsonPrimitive
      JSON structure for map3dTo3dGmsh.

    Returns
    -------
    None

    Examples
    --------
    

    The main method of this class is :meth:`build`.

    The mesh topology is constructed from the channel, represented as a
    multiple bounded volume ``channel_``, and the blade mesh blocks
    provided in the list ``blocks_``. Throughout this documentation, ``N``
    denotes the number of mesh blocks, i.e., ``len(blocks_)``.

    The mesh regions are numbered as follows:

        - ``R_0``     : Channel

        - ``R_1``     : First mesh block

        - ...

        - ``R_(N-1)`` : (N-1)th mesh block

    The multiple bounded volume is meshed using an unstructured mesh with
    prismatic boundary layers. The mesh block volumes are meshed as
    transfinite regions with recursive recombination.

    The topology settings are initialized from ``map3dTo3dGmshJson_``.
    The model is created as ``m3dGmsh``.

    The bounding surfaces of the multiple bounded volume are provided through
    the list ``channelFaces_``.

    The surfaces of the mesh topology are either taken directly from
    ``channelFaces_`` or extracted from the block volumes in ``blocks_`` using
    the method :meth:`detectFirstAndSecond`. This method takes a block volume
    as a ``map3dTo3d`` object and a parameter direction as an integer input.
    It returns the faces located at the normalized positions 0 and 1 along
    the specified parameter direction.

    The blade surface is provided through ``blade_``.

    The following figure illustrates the surfaces in the mesh topology. The
    hub and shroud surfaces are not shown.

    .. _meshFaces:
    .. figure:: bladeFigs/meshFaces_labeled.png
       :width: 100%
       :align: center

       Topology faces and their labels used in this class. Interfaces (red),
       periodic meanplane faces (yellow/green), coupling faces (cyan), and
       blade surfaces (gray) are highlighted.

    By iterating over ``blocks_``, the corresponding surfaces are added to
    the topology. For each mesh block, the blade surface is added and labeled
    with the identifier ``"blade"``.

    The number of mesh block surfaces that belong to the mean plane is
    specified by the input ``nMeanplaneBlocks_``. These surfaces do not
    connect to the multiple bounded volume channel. In this class, they are
    labeled with the identifier ``"block"``.

    The remaining faces shown in :numref:`meshFaces`, together with the hub,
    shroud, and coupling surfaces, are added to the topology through
    ``channelFaces_`` and labeled according to the strings shown in the
    figure.

    The coupling faces between the mesh blocks and the channel multiple
    bounded volume are labeled with the identifier ``"coupling"``. The hub
    and shroud bounding surfaces are labeled with the identifiers ``"hub"``
    and ``"shroud"``, respectively.
    
    The periodic faces of the topology are those labeled ``"suction"``,
    ``"pressure"``, and ``"block"``. The labels ``"suction"`` and
    ``"pressure"`` correspond to the suction and pressure sides of a turbine
    blade.

    Periodically connected faces that are meshed using an unstructured mesh
    are labeled with the identifier ``"tri"``. The faces periodic to the
    ``"block"`` faces are meshed as transfinite surfaces and are assigned the
    identifier ``"quad"``. Periodicity on these faces is defined using the
    observer ``bVOSetRotationalPeriodicity``.

    The boolean value ``meshTEBlocks_`` controls whether trailing edge mesh
    blocks are created. If trailing edge mesh blocks are required, this
    parameter must be set to ``True``.

    Each face identifier is assigned an integer suffix for unique
    identification. The following faces are added to the topology when
    trailing edge mesh blocks are enabled:

        - ``"hub_0"`` ... ``"hub_"+str(N)``

        - ``"shroud_0"`` ... ``"shroud_"+str(N)``

        - ``"inlet_0"``

        - ``"outlet_0"``

        - ``"suction_tri_0"``, ``"suction_tri_1"``,
          ``"suction_tri_"+str(nMeanplaneBlocks_+3)``,
          ``"suction_tri_"+str(nMeanplaneBlocks_+4)``

        - ``"block_0"`` ... ``"block_"+str(nMeanplaneBlocks_)``

        - ``"pressure_tri_0"``, ``"pressure_tri_1"``,
          ``"pressure_tri_"+str(nMeanplaneBlocks_+3)``,
          ``"pressure_tri_"+str(nMeanplaneBlocks_+4)``

        - ``"pressure_quad_2"`` ...
          ``"pressure_quad_"+str(nMeanplaneBlocks_+2)``

        - ``"blade_0"`` ... ``"blade_"+str(N-1)``

        - ``"coupling_0"`` ...
          ``"coupling_"+str(N+2-nMeanplaneBlocks_-1)``

    Prismatic boundary layers are generated on the ``"hub"`` and
    ``"shroud"`` faces. The number of elements in these layers is specified
    by ``nBoundaryLayers_``. The boundary layers extend onto the
    ``"inlet"``, ``"outlet"``, ``"suction"``, ``"pressure"``, and
    ``"coupling"`` faces of the multiple bounded volume.

    The minimum and maximum characteristic mesh sizes of the unstructured
    mesh are specified by the inputs ``charLengthMin`` and
    ``charLengthMax``.

    The method :meth:`extractEdgesInFirstAndSecond` is used to identify the
    edges of a surface that lie on two other surfaces.

    The following figure illustrates the edge groups to which mesh settings
    are applied. Each group is highlighted using a uniform color.

    .. _channelMeshing0:
    .. figure:: bladeFigs/guideVane_channelMeshing.png
       :width: 95%
       :align: center

       Edges of the bladed channel to which mesh settings are applied:
       ``hubToShroudLines`` (orange), ``bladeHubLines`` and
       ``bladeShroudLines`` (blue), ``bladeToBlockLines`` (green), and the
       trailing edge mesh block edges contained in ``tEMeshList`` (pink).
       No explicit mesh settings are applied to the gray edges.

    Gradings are applied to refine the mesh near the hub, shroud, and blade
    walls. The method :meth:`addGrading` is used to create the grading
    functions. The methods :meth:`gradingsTypeTransfinite` and
    :meth:`gradingsGradingFunctions` are then used to apply these gradings to
    the mesh setting observer ``bVOMeshRule``.

    The following table summarizes the edge groups and their corresponding
    mesh parameters and settings:

    .. _edgeMeshTab:
    .. list-table:: Edges and their mesh parameters
       :header-rows: 1
       :align: center

       * - Edge group
         - Number of elements /
           
           Element size
         - Grading label /
           
           First element size

       * - ``hubToShroudLines``
         - ``nElementsSpanwise_``
         - ``"hubToShroud"``
           
           ``firstElementSizeHubToShroud_``

       * - ``bladeHubLines``
         - ``bladeHubElementSize_``
         - ``"tangentialBlade_*"``

       * - ``bladeShroudLines``
         - ``bladeShroudElementSize_``
         - ``"tangentialBlade_*"``

       * - ``bladeToBlockLines``
         - ``nElementsNormal_``
         - ``"normalBlade"``
           
           ``firstElementSizeNormalBlade_``

       * - ``tEMeshList``
         - ``nElementsNormal_``
         - No grading

    The mesh parameters beginning with ``nElements...`` specify a fixed
    number of elements along each edge. The parameters beginning with
    ``firstElementSize...`` define the size of the first element adjacent to
    the wall on which the grading is applied.
   
    The mesh sizes along the blade contour (``bladeHubLines`` and
    ``bladeShroudLines``) are controlled by the functions
    ``bladeHubElementSize_`` and ``bladeShroudElementSize_``, respectively.
    For each edge, a minimum number of elements is first determined from its
    start and end vertices. The blending factors
    ``bladeHubElementScale_`` and ``bladeShroudElementScale_`` are then used
    to interpolate between the element counts at both ends, thereby defining
    the number of elements assigned to the edge.

    To ensure a smooth transition in element sizes between consecutive
    edges, the ``"tangentialBlade_*"`` grading is adjusted at both the start
    and end vertices.

    Trailing edge mesh blocks are generated only if ``meshTEBlocks_`` is set
    to ``True``. The corresponding edges are collected in the list
    ``tEMeshList``. No grading functions are applied to these edges.

    The observer ``bVOFaceToPatchRule`` is used to rename the following
    faces so that they match the boundary naming convention of the `OpenFOAM`
    case:
    
    .. _faceToPatchTable:
    .. list-table:: Renaming of the faces for `openFOAM` case setup.
       :header-rows: 1
       :align: center

       * - Original name
         - Boundary name

       * - ``"*hub*"``
         - ``label_+"_hub"``

       * - ``"*shroud*"``
         - ``label_+"_shroud"``

       * - ``"*blade*"``
         - ``label_+"_blade"``

       * - ``"*inlet*"``
         - ``label_+"_inlet"``

       * - ``"*outlet*"``
         - ``label_+"_outlet"``

       * - ``"*suction*"``
         - ``label_+"_suction"``

       * - ``"*block*"``
         - ``label_+"_suction"``

       * - ``"*pressure*"``
         - ``label_+"_pressure"``

    If debug mode is enabled the geometries are plotted and prefixed with
    ``"debug_"``.

    The following observers are also added:

        - ``bVOReadMSH``

        - ``bVODumpModel``

        - ``bVOWriteMSH``

        - ``bVOOrientCellVolumes``

    Finally, the mesh topology is returned to the calling class through
    ``appendBoundedVolume``.
    
    A mesh resulting from this topology is shown in the following figure.

    .. _gvMesh:
    .. figure:: bladeFigs/guideVane_mesh.png
       :width: 95%
       :align: center

       Mesh of a bladed channel resultuing from the described topology.
    """
    def __init__( 
        self, 
        label: str, 
        channel: analyticGeometry,
        channelFaces: List[analyticGeometry],
        blocks: List[analyticGeometry],
        nMeanplaneBlocks: int,
        blade: analyticGeometry,
        nBoundaryLayers: int,
        nElementsSpanwise: int,
        nElementsNormal: int,
        firstElementSizeHubToShroud: float,
        firstElementSizeNormalBlade: float,
        bladeHubElementSize: scaOneD = None,
        bladeHubElementScale: float = None,
        bladeShroudElementSize: scaOneD = None,
        bladeShroudElementScale: float = None,
        charLengthMin: float = 0.05,
        charLengthMax: float = 0.10,
        meshTEBlocks: bool = False,
        bladeHubShroudDirection: int = None,
        boundaryLayerDirection: int = None
    ) -> None:

        """
        Parameters 
        ----------
        label: str
          Label.
        channel: multipleBoundedVolume
          Channel.
        channelFaces: List[ map2dTo3d ]
          List of bounding faces surrounding the channel
        blocks: List[ map3dTo3d ]
          List of mesh blocks surrounding the blade.
        nMeanplaneBlocks: int
          Number of block faces which are part of the meanplane
        blade: map2dTo3d
          Blade.
        nBoundaryLayers: int
          Number of boundary layers.
        nElementsSpanwise: int
          Number of elements in spanwise direction.
        nElementsNormal: int
          Number of elements in normal to the blade direction.
        firstElementSizeHubToShroud: float
          Size of first element on hub and shroud.
        firstElementSizeNormalBlade: float
          Size of first element at the blade in normal to the blade direction.
        bladeHubElementSize: scaOneD
          Function describing the element size versus the standardized unwrapped length of the blade at the hub.
        bladeHubElementScale: float
          Factor defining the number of elements at the hub for each mesh block.
        bladeShroudElementSize: scaOneD
          Function describing the element size versus the standardized unwrapped length of the blade at the shroud.
        bladeShroudElementScale: float
          Factor defining the number of elements at the shroud for each mesh block.
        charLengthMin: float
          Sets gmsh attribute `Mesh.CharacteristicLengthMin`.
        charLengthMax: float
          Sets gmsh attribute `Mesh.CharacteristicLengthMax`.
        meshTEBlocks: Bool
          Marker if trailing edge mesh blocks should be meshed
        """
        logging.info( "Initializing %s ..." % (label) )
        super(map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks, self).__init__()
        
        self.label_ = label
        self.channel_ =  channel.clone() 
        self.channelFaces_ = channelFaces
        
        self.blocks_ = []
        for block in blocks:
            self.blocks_.append( map3dTo3d.MustDownCast(block.clone()) )
        
        self.nMeanplaneBlocks_ = nMeanplaneBlocks
        self.blade_ = map2dTo3d.MustDownCast( blade.clone() )

        self.nBoundaryLayers_ = nBoundaryLayers
        self.nElementsSpanwise_ = nElementsSpanwise
        self.nElementsNormal_ = nElementsNormal
        self.firstElementSizeHubToShroud_ = firstElementSizeHubToShroud
        self.firstElementSizeNormalBlade_ = firstElementSizeNormalBlade
        self.bladeHubElementSize_ = bladeHubElementSize
        self.bladeHubElementScale_ = bladeHubElementScale
        self.bladeShroudElementSize_ = bladeShroudElementSize
        self.bladeShroudElementScale_ = bladeShroudElementScale
        
        self.bladeHubShroudDirection = bladeHubShroudDirection
        self.boundaryLayerDir = boundaryLayerDirection

        self.meshTEBlocks_ = meshTEBlocks

        self.map3dTo3dGmshJson_ = jsonPrimitive(
          '{'
            '"label" : "'+self.label_+'", '
            '"option" : ['
              '{"name" : "[gmsh]General.Terminal", "value" : "1."},'
              '{"name" : "[gmsh]General.Verbosity", "value" : "100."},'
              '{"name" : "[gmsh]General.ExpertMode", "value" : "1."},'
              '{'
                '"name" : "[gmsh]Mesh.LcIntegrationPrecision", '
                '"value" : "1.0E-04"'
              '},'
              '{'
                '"name" : "[gmsh]Mesh.CharacteristicLengthMin", '
                '"value" : "'+str(charLengthMin)+'"'
              '},'
              '{'
                '"name" : "[gmsh]Mesh.CharacteristicLengthMax", '
                '"value" : "'+str(charLengthMax)+'"'
              '},'
              '{"name" : "[gmsh]Mesh.Algorithm", "value" : "1"},'
              '{'
                '"name" : "[gmsh]Mesh.MeshSizeExtendFromBoundary", '
                '"value" : "1"'
              '},'
              '{"name" : "[gmsh]Mesh.MeshSizeFromPoints", "value" : "1"}'
            '],'
            '"analyticGeometry" : []'
          '}'
        )

    def build(self) -> None:
        """Build part.

        Parameters
        ----------
        None

        Returns
        -------
        None
       

        The model is initialized as ``m3dGmsh`` from ``map3dTo3dGmshJson_``.
        The container ``aG`` of type ``labeledVectorHandlingAnalyticGeometry``
        is created to manage the analytic geometries.
        
        **Add the Grid Channel Volume and Faces**

            The multiple bounded volume  of the grid channel``channel_`` is added to the model.
            Its bounding faces are added by iterating over the list
            ``channelFaces_``. Regular faces, for which
            ``multipleBoundedSurface.ConstDownCast(face) == None`` applies, are added
            directly to ``aG``. The hub and shroud faces are of the type
            ``multipleBoundedSurface``. For these faces, the ``else`` branch is
            executed and their bounding surfaces are added individually.

            The face labels are assigned during the generation of the multiple
            bounded volume in the class `multipleBoundedVolume_gridChannel`.
        
        **Add the Mesh Block Volumes and Faces**

            The block volumes and their faces are added by iterating over
            ``blocks_`` using ``i, block in enumerate(blocks_)``.
            The block faces on the blade wall and the surrounding surfaces are
            extracted using the method :meth:`detectFirstAndSecond`.
            The faces on the blade wall are labeled ``"blade_" + str(i)``, while the
            surrounding surfaces are labeled ``"block_" + str(i)``.

            The block volumes are ordered so that their sequence in ``blocks_``
            follows the u-direction of the blade surface ``blade_``.
            If trailing edge mesh blocks exist, they correspond to the first and
            last entries of ``blocks_``.

            If trailing edge mesh blocks are enabled
            (``meshTEBlocks_ == True``), the blade faces of the first and last mesh
            blocks are not added to ``aG``. Only block faces that are part of the
            mean plane are added to ``aG`` (``i <= nMeanplaneBlocks_``),
            corresponding to the ``"block"`` faces shown in :numref:`meshFaces`.

            The block volumes are added to the model as ``dtRegion`` objects and
            configured to be meshed using transfinite meshing with recursive
            recombination.

            The observer ``bVONameRegions`` is added to establish the naming
            convention of the regions.
        
        **Organize Edges of the Trailing Edge Mesh Blocks**

            The edges of the trailing edge mesh blocks (shown in pink in
            :numref:`channelMeshing0`) are extracted by first obtaining the blade
            and block faces of ``blocks_[0]`` and ``blocks_[-1]`` using
            :meth:`detectFirstAndSecond`. Their hub and shroud edges are then
            identified using :meth:`extractEdgesInFirstAndSecond`.

            Using these edges, the list ``tEMeshList`` is constructed with the
            following structure:

            .. code-block:: python

                tEMeshList = List[
                    Tuple[
                        Tuple[List[int], List[int]],
                        int
                    ]
                ]

            The top level list entries have the following meaning:

                - ``tEMeshList[0]`` : Edges extending directly from the blade

                - ``tEMeshList[1]`` : Edges extending from the outer wall of the first mesh block

                - ``tEMeshList[2]`` : Edges extending from the outer wall of the last mesh block

            The lower level entries of ``tEMeshList`` are defined as follows:

                - ``tEMeshList[i][0]`` : Tuple containing lists of edge identifiers

                - ``tEMeshList[i][0][0]`` : List of edge identifiers on the hub

                - ``tEMeshList[i][0][1]`` : List of edge identifiers on the shroud

                - ``tEMeshList[i][1]`` : Integer specifying the edge direction
        
        **Manage Faces**

            The observer ``bVOAnalyticGeometryToFace`` is added to implement the
            faces stored in ``aG`` within ``m3dGmsh``.

            The periodic faces (shown in yellow and green in :numref:`meshFaces`)
            are organized in the list ``periodics``. The list is constructed such
            that each entry ``periodics[i]`` is a ``Tuple`` containing a pair of
            periodic faces. The suction side boundary is stored in
            ``periodics[i][0]`` and the corresponding pressure side boundary in
            ``periodics[i][1]``.

            The faces that are meshed unstructured, and
            their hub-to-shroud edges, are stored in the list
            ``unstrFacesAndh2sLines``. These faces are identified in ``aG`` by
            their physical labels ``"inlet"``, ``"outlet"``, ``"suction_tri"``,
            and ``"pressure_tri"`` (see :numref:`meshFaces`).

            The list has the following structure:

            .. code-block:: python

                unstrFacesAndh2sLines = List[
                    List[
                        map2dTo3d,
                        List[int]
                    ]
                ]

            Each entry contains an unstructured face,
            ``unstrFacesAndh2sLines[i][0]``, and the list of its edges that extend
            from hub to shroud, ``unstrFacesAndh2sLines[i][1]``.
        
        **Manage Edges and Set Number of Elements**

            The edges to which mesh settings are applied (see
            :numref:`channelMeshing0`) are identified by extracting and organizing
            lists of edge identifiers returned by the ``dtGmshModel``.

            The following edge identifier lists are used to define the mesh
            settings:

                - ``hubToShroudLines``

                - ``bladeToBlockLines``

                - ``bladeHubLines``

                - ``bladeShroudLines``
            
            The dictionary ``gradings`` is created, and grading functions for the
            edges in ``hubToShroudLines`` and ``bladeToBlockLines`` are added using
            the method :meth:`addGrading`.

            This method takes the ``gradings`` dictionary, a grading function, a
            label, the model, and the size of the first element in the grading as
            input.

            The grading associated with ``hubToShroudLines`` is assigned the label
            ``"hubToShroud"`` and uses the first element size
            ``firstElementSizeHubToShroud_``. The grading associated with
            ``bladeToBlockLines`` is assigned the label ``"normalBlade"`` and uses
            the first element size ``firstElementSizeNormalBlade_``.

            The number of elements and the grading functions are then applied to the
            edges according to the specifications listed in :numref:`edgeMeshTab`.

            The mesh settings for the blade edges ``bladeShroudLines`` and
            ``bladeHubLines`` are applied by iterating over the corresponding edge
            lists.
        
            Mesh settings for the trailing edge mesh block edges are applied only if
            ``meshTEBlocks_ == True``.
        
        **Mesh Settings along the Blade**

            The orientation of the blade edges is determined using
            :meth:`boundaryEdgeDirection`. The method is provided with
            ``m3dGmsh.getModel()`` and a list containing the blade surface
            ``blade_`` together with the corresponding edge lists.

            For each edge, the edge length ``eL`` is computed and the start and end
            vertices ``v0`` and ``v1`` are identified. These points are
            reparameterized onto the blade surface ``blade_``, yielding the surface
            parameter coordinates ``p0_uv`` and ``p1_uv``.

            Depending on the iteration the element size functions 
            ``bladeHubElementSize_`` or ``bladeShroudElementSize_`` are then 
            evaluated at the corresponding parameter coordinates to obtain the 
            local element sizes ``ms_0`` and ``ms_1`` in the appropriate parameter 
            direction.

            Using these element sizes and the edge length ``eL``, the required
            numbers of elements, ``nE_0`` and ``nE_1``, are computed and rounded up
            to the next integer.

            The final number of elements assigned to the blade edge is calculated as

            .. code-block:: python

                nE = math.ceil(
                    min(nE_0, nE_1) + elementScale * abs(nE_1 - nE_0)
                )

            The floating point value ``elementScale`` corresponds to either
            ``bladeShroudElementScale_`` or ``bladeHubElementScale_``, depending on
            the current iteration.

            For each edge, a grading function with the label
            ``"tangentialBlade_*"`` is created. The start and end element sizes of
            the grading are set to ``ms_0`` and ``ms_1``, respectively.
 
        **Set Mesh Rules**

            The boundary layer directions of the unstructured faces stored in
            ``unstrFacesAndh2sLines`` are determined using
            :meth:`detectBoundaryLayerDir`, which returns the list
            ``boundaryLayerDir``.

            The mesh rules are defined using the observer ``bVOMeshRule``.
            The methods :meth:`gradingsTypeTransfinite` and
            :meth:`gradingsGradingFunction` are used to retrieve the appropriate
            grading information from the ``gradings`` dictionary and pass it to the
            observer.

            Meshing of the unstructured region is performed using the rules
            ``"dtMeshGFaceWithTransfiniteLayer"`` and
            ``"dtMeshGRegionWithBoundaryLayer"``.

            The rule ``"dtMeshGFaceWithTransfiniteLayer"`` is applied to the faces
            that are meshed unstructured, namely ``"*inlet*"``,
            ``"*outlet*"``, ``"*suction_tri*"`` and ``"*pressure_tri*"``.

            The rule ``"dtMeshGRegionWithBoundaryLayer"`` is applied to the region
            corresponding to the multiple bounded volume, ``"R_0"``.
            The hub and shroud faces, ``"hub_0"`` and ``"shroud_0"``, on which the
            boundary layers are generated, are added to the ``"_faceLabel"``
            entry.

            The boundary layers extend onto the faces of ``"R_0"`` labeled
            ``"*inlet*"``, ``"*outlet*"``, ``"*pressure_*"``,
            ``"*suction_*"``, and ``"*coupling_*"``. These faces are added to the
            ``"_slidableFaceLabel"`` entry.

            The number of boundary layer elements is specified by
            ``nBoundaryLayers_``, while the boundary layer orientation is defined by
            ``boundaryLayerDir``.


        **Define Observers**

            The observers ``bVOReadMSH`` and ``bVODumpModel`` are then added.

            To define rotational periodicity, a reference coordinate system
            ``theT`` is created and added to the base container object ``bC``.
            The periodic boundary conditions are established by iterating over
            ``periodics`` and creating a ``bVOSetRotationalPeriodicity`` observer
            for each pair of periodic faces.
        
            The faces are renamed to match the boundary condition naming convention
            used in an OpenFOAM case through the observer
            ``bVOFaceToPatchRule``. The corresponding renaming rules are summarized
            in :numref:`faceToPatchTable`.

            The observer ``bVOWriteMSH`` controls the generation of the mesh file.
            The observer ``bVOOrientCellVolumes`` ensures that all mesh cell volumes
            have a positive orientation.

        The created mesh topology ``m3dGmsh`` is returned to the
        calling class using the method ``appendBoundedVolume``. 
        """
        logging.info( "Building %s ..." % (self.label_) )

        m3dGmsh = map3dTo3dGmsh()
        m3dGmsh.jInit(
          self.map3dTo3dGmshJson_, None, None, None, None, None
        )

        #
        # create lvh
        #
        aG = labeledVectorHandlingAnalyticGeometry()   
        
        #
        # add channel (multiple bounded volume (MBV))
        #
        channelId = m3dGmsh.getModel().addIfToGmshModel( self.channel_ )
        
        #
        # add bounding faces of the multiple bounded volume
        #
        for face in self.channelFaces_: 
            # not multi bounded faces
            #  those are: inlet, outlet suction, pressure and coupling faces
            if multipleBoundedSurface.ConstDownCast(face) == None:
                aG.push_back(face.clone())
            # multi bounded faces
            #  those are: hub and shroud faces
            else:
                # surfaceConstPtr returns the rectangular bounding box in which the mbs was created
                scp = multipleBoundedSurface.MustDownCast(face).surfaceConstPtr()
                scp.setLabel(face.getLabel())
                aG.push_back(scp.clone())
        
        #
        # add mesh blocks
        #
        for i, block in enumerate(self.blocks_):
            
            # find face on blade and block (parallel to blade)
            bladeFace, blockFace = self.detectFirstAndSecond(block, 3)
            
            if self.meshTEBlocks_ == True: 
                # bladeFace at i == 0 and i == len(self.blocks_)-1 are the same
                #  when trailing edge blocks were build.
                # this face is not added
                if i != 0 and i < len(self.blocks_)-1:
                    aG.push_back(bladeFace << "blade_"+str(i))
            else:
                aG.push_back(bladeFace << "blade_"+str(i))

            # only the block faces which are part of the meanplane are pushed
            if i <= self.nMeanplaneBlocks_:
                aG.push_back(blockFace << "block_"+str(i))
            
            # add block volumes, those are 6-sided and meshed transfinite
            rid = m3dGmsh.getModel().addIfRegionToGmshModel(block)
            m3dGmsh.getModel().getDtGmshRegionByTag( rid ).meshTransfiniteRecursive()
            m3dGmsh.getModel().getDtGmshRegionByTag( rid ).meshWNElements(1,1,1)
            m3dGmsh.getModel().getDtGmshRegionByTag( rid ).meshRecombineRecursive()
        
        #
        # name regions
        #
        #  - R_0    : channel
        #  - R_1     : First mesh block
        #  - ...
        #  - R_(N-1) : (N-1)^th mesh block
        #  - R_(N) : (N)^th mesh block
        #
        ob = bVONameRegions()
        ob.jInit( jsonPrimitive('{ "_regionLabel" : [] }'), m3dGmsh )
        ob.preUpdate()

        #
        # trailing edge mesh block edges extending in flow direction 
        #  from the blade, the first and the last block
        #
        if self.meshTEBlocks_ == True:
            # faces from hub to shroud
            #   first block
            tEBlade, tEBlock0 = self.detectFirstAndSecond(
              self.blocks_[0], 3
            )
            #   last block
            tEBlade, tEBlock1 = self.detectFirstAndSecond(
              self.blocks_[-1], 3
            )
            # edges on hub and shroud
            #   trailing edge
            tEHub, tEShroud = self.extractEdgesInFirstAndSecond(
              m3dGmsh.getModel(), 
              [tEBlade], 
              map2dTo3d.MustDownCast(aG["hub"]), 
              map2dTo3d.MustDownCast(aG["shroud"])
            )
            #   first block
            tEBlock0_Hub, tEBlock0_Shroud = self.extractEdgesInFirstAndSecond(
              m3dGmsh.getModel(), 
              [tEBlock0], 
              map2dTo3d.MustDownCast(aG["hub"]), 
              map2dTo3d.MustDownCast(aG["shroud"])
            )
            #   last block
            tEBlock1_Hub, tEBlock1_Shroud = self.extractEdgesInFirstAndSecond(
              m3dGmsh.getModel(), 
              [tEBlock1], 
              map2dTo3d.MustDownCast(aG["hub"]), 
              map2dTo3d.MustDownCast(aG["shroud"])
            )
            # create a mesh list containing the edges and a float 
            #  >/<0 for grading directions
            tEMeshList = [
                    [[tEHub, tEShroud], -1.0],
                    [[tEBlock0_Hub, tEBlock0_Shroud], -1.0],
                    [[tEBlock1_Hub, tEBlock1_Shroud], 1.0]
                ]  
        
        #
        # name faces
        #
        ob = bVOAnalyticGeometryToFace()
        ob.jInit( 
          jsonPrimitive(
            '{'
              '"analyticGeometry" : ['
                '{"label" : "hub"},'
                '{"label" : "shroud"},'
                '{"label" : "inlet"},'
                '{"label" : "outlet"},'
                '{"labels" : "suction_*"},'
                '{"labels" : "pressure_*"},'
                '{"labels" : "blade_*"},'
                '{"labels" : "block_*"},'
                '{"labels" : "coupling_*"}'
              '],'
              '"_inc" : 10.0,'
              '"_facesPerEntry" : []'
            '}'
          ), 
          None, None, None, aG, None, m3dGmsh 
        )
        ob.preUpdate()
         
        #
        # Manage Faces and Edges
        #
         
        # organize periodic faces
        #  the order is set by adding the faces seqentially and the naming
        periodic_pressure = m3dGmsh.getModel().getDtGmshFaceListByPhysical("*pressure_*")
        
        # suction faces
        periodic_suction = []
        periodic_suction.append(m3dGmsh.getModel().getDtGmshFaceByPhysical("*suction_tri_0*"))
        periodic_suction.append(m3dGmsh.getModel().getDtGmshFaceByPhysical("*suction_tri_1*"))
        periodic_block = m3dGmsh.getModel().getDtGmshFaceListByPhysical("*block_*")
        for face in periodic_block:
            periodic_suction.append(face)
        l = len(periodic_suction)
        periodic_suction.append(m3dGmsh.getModel().getDtGmshFaceByPhysical("*suction_tri_"+str(l)+"*"))
        periodic_suction.append(m3dGmsh.getModel().getDtGmshFaceByPhysical("*suction_tri_"+str(l+1)+"*"))
        
        # list with periodic faces
        periodics = list(zip(periodic_suction, periodic_pressure))
        
        # all hub to shroud lines
        hubToShroudLines = \
            m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*hub*", "*shroud*"
            )
        
        # unstructured faces and their lines, they will later get a boundary layer
        # structure of the resulting list:
        #  unstrFacesAndh2sLines = List[List[face0, List[edges0]]]
        unstrFacesAndh2sLines = []
        for i, face in enumerate(aG):
            # faces with boundary layer
            if face.getLabel().startswith(
                    ("inlet", "outlet", "suction_tri_", "pressure_tri_")
                ):

                f = map2dTo3d.MustDownCast(face)
                # get all edges on the face
                faceLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                    "*"+face.getLabel()+"*", "*"+face.getLabel()+"*"
                )
                # keep the edges extending from hub to shroud
                hsLines = (
                    set(faceLines)&set(hubToShroudLines)
                  ).union(
                    set(faceLines)&set(-np.array(hubToShroudLines))
                  )

                unstrFacesAndh2sLines.append([f, hsLines])
        
        # getting other relevant edges
        hubLines = \
            m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*hub*", "*hub*"
            )
        shroudLines = \
       m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*shroud*", "*shroud*"
            )
        hAndSLines = set(hubLines).union(set(shroudLines))
        
        bladeToBlockLines_0 = \
            m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*blade_*", "*block_*"
            )
        bladeToBlockLines_1 = \
            m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*blade_*", "*coupling_*"
            )
        bladeToBlockLines = set(bladeToBlockLines_0).union(set(bladeToBlockLines_1))
        bladeToBlockLines = set(bladeToBlockLines)&set(hAndSLines)

        bladeLines = \
            m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*blade_*", "*blade_*"
            )

        bladeHubLines = set(bladeLines)&set(hubLines)
        bladeShroudLines = set(bladeLines)&set(shroudLines)
        
        # trailing edge hub and shroud edges have to be removed here if
        #  trailing edge mesh blocks exist
        if self.meshTEBlocks_ == True:
            bladeHubLines = \
              set(bladeHubLines)-set(-np.array(tEHub))-set(tEHub)
            bladeShroudLines = \
              set(bladeShroudLines)-set(-np.array(tEShroud))-set(tEShroud)
            
        
        #
        # add debug faces and line
        #
        
        if self.debug():
            
            ## curves for visualisation purposes only
            #tEList = [tEHub[0], tEShroud[0], tEBlock0_Hub[0], tEBlock0_Shroud[0], tEBlock1_Hub[0], tEBlock1_Shroud[0]]

            #otherHubLines = set(hubLines) \
            #    - set([-x for x in bladeHubLines]) \
            #    - set(bladeHubLines)
            #otherHubLines = set(otherHubLines) \
            #    - set([-x for x in bladeToBlockLines]) \
            #    - set(bladeToBlockLines)
            #otherHubLines = set(otherHubLines) \
            #    - set([-x for x in tEList]) \
            #    - set(tEList)
            #
            #otherShroudLines = set(shroudLines) \
            #    - set([-x for x in bladeShroudLines]) \
            #    - set(bladeShroudLines)
            #otherShroudLines = set(otherShroudLines) \
            #    - set([-x for x in bladeToBlockLines]) \
            #    - set(bladeToBlockLines)
            #otherShroudLines = set(otherShroudLines) \
            #    - set([-x for x in tEList]) \
            #    - set(tEList)
            
            # all faces
            for faceLabel in [
                  "*hub*", "*shroud*", 
                  "*inlet*", "*outlet*", 
                  "*suction_*", "*pressure_*", 
                  "*blade_*", "*block_*",
                  "*coupling_*"
                ]:
                
                faces = m3dGmsh.getModel().getDtGmshFaceListByPhysical(faceLabel)
                for face in faces:
                    self.appendAnalyticGeometry(
                        face.getMap2dTo3d(), 
                        "debug_faceAll_"+self.label_+"_"+face.getPhysicalString()
                    )
            # slidable faces for boundary layer meshing of MBV
            for faceLabel in [
                "*inlet*", "*outlet*", "*pressure_*", "*suction_*", "*coupling_*"
                ]:
                
                faces = m3dGmsh.getModel().getDtGmshFaceListByPhysical(faceLabel)
                for face in faces:
                    self.appendAnalyticGeometry(
                        face.getMap2dTo3d(), 
                        "debug_faceMBV_"+self.label_+"_"+face.getPhysicalString()
                    )
            # lines to which element numbers and gradings are assigned 
            for lab, lines in zip(
                [
                  "hubToShroudLines",
                  "bladeToBlockLines",
                  "bladeHubLines",
                  #"TrailingEdgeLines",
                  "bladeShroudLines",
                  #"otherHubLines",
                  #"otherShroudLines"
                ],
                [ 
                  hubToShroudLines,
                  bladeToBlockLines,
                  bladeHubLines,
                  #tEList,
                  bladeShroudLines,
                  #otherHubLines,
                  #otherShroudLines
                ]
            ):
                for ii in lines:
                  self.appendAnalyticGeometry(
                    m3dGmsh.getModel().getDtGmshEdgeByTag( ii ).getMap1dTo3d(),
                    "debug_lineNE_"+self.label_+"_"+lab+"_"+str(ii)
                  )
            # faces and lines which will be meshed unstructured with boundary layer
            for faceAndLines in unstrFacesAndh2sLines:

                self.appendAnalyticGeometry(
                        faceAndLines[0],
                        "debug_faceUnstr_"+self.label_+"_"+faceAndLines[0].getLabel()
                    )
                for i, line in enumerate(faceAndLines[1]):
                    self.appendAnalyticGeometry(
                        m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                        "debug_lineUnstr_"+self.label_+"_"+faceAndLines[0].getLabel()+"_"+str(i)
                    )
            # periodic boundary faces
            for i, periodic in enumerate(periodics):
                self.appendAnalyticGeometry(
                    periodic[0].getMap2dTo3d(),
                    "debug_periodic0_"+str(i)+"_"+periodic[0].getPhysicalString()
                )
                self.appendAnalyticGeometry(
                    periodic[1].getMap2dTo3d(),
                    "debug_periodic1_"+str(i)+"_"+periodic[1].getPhysicalString()
                )

        #
        # gradings
        #
        gradings = {}
        # hub to shroud
        gradings = self.addGrading( 
          gradings,
          scaTanhGradingOneDCompound(
            scaTanhGradingOneD(
              vectorReal([0.5, 0.5, -1.0, 2.0]),
              1.0,
              0.1, 5.0
            )
          ),
          "hubToShroud",
          m3dGmsh,
          self.firstElementSizeHubToShroud_
        )
        # blade normal
        gradings = self.addGrading(
          gradings,
          scaTanhGradingOneDCompound(
            scaTanhGradingOneD(
              vectorReal([1.0, -1.0, 1.0, -1.0]),
              1.0,
              0.1, 5.0
            )
          ),
          "normalBlade",
          m3dGmsh,
          self.firstElementSizeNormalBlade_
        )
         
        if self.bladeHubShroudDirection == None:
            # getting blade direction for grading along the blade
            self.bladeHubShroudDirection = self.boundaryEdgeDirection(
              m3dGmsh.getModel(),
              [
                [self.blade_, bladeHubLines],
                [self.blade_, bladeShroudLines],
              ]
            )
            logging.info("bladeHubShroudDirection = %d" % self.bladeHubShroudDirection)

        #
        # mesh settings
        #
        
        # hub to shroud
        for line in hubToShroudLines:
            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            theEdge.setNElements( self.nElementsSpanwise_ )
            theEdge.setGrading( 1.0, gradings["hubToShroud"][0] )
        
        # along blade
        for lines, elementSize, elementScale, outStr in zip(
            [bladeShroudLines, bladeHubLines,],
            [self.bladeShroudElementSize_, self.bladeHubElementSize_,],
            [self.bladeShroudElementScale_, self.bladeHubElementScale_,],
            ["BladeShroud", "BladeHub",],
        ):
            direction = self.bladeHubShroudDirection
            for line in lines:
                theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
                if elementSize!=None:
                    eL = theEdge.getMap1dTo3d().length()
                    v0 = theEdge.getMap1dTo3d().getPointPercent(0.0)
                    v1 = theEdge.getMap1dTo3d().getPointPercent(1.0)
                    p_0_uv = self.blade_.reparamPercentOnFace( v0 )
                    p_1_uv = self.blade_.reparamPercentOnFace( v1 )
                    ms_0 = elementSize( p_0_uv[direction] )[0]
                    ms_1 = elementSize( p_1_uv[direction] )[0]
                    nE_0 = math.ceil(eL/ms_0)
                    nE_1 = math.ceil(eL/ms_1)
                    nE = math.ceil(
                      min(nE_0,nE_1) + elementScale * abs(nE_1 - nE_0)
                    )

                    logging.info(
                        "\n  %s Edge %d / length %f "
                        "\n    p_uv: (%f, %f) -> (%f, %f) "
                        "\n    meshLength: (%f) -> (%f) "
                        "\n    nElements (%f -> %d) -> (%f -> %d) => %d"
                      %
                      (
                        outStr,
                        line,
                        eL,
                        p_0_uv[0], p_0_uv[1],
                        p_1_uv[0], p_1_uv[1],
                        ms_0, ms_1,
                        eL/ms_0, nE_0, eL/ms_1, nE_1, nE
                      )
                    )

                    theEdge.meshTransfiniteWNElements( 1, 1.0, nE )
                    gradings = self.addGrading(
                      gradings,
                      sca3PPointsBSplineOneDCompound(
                        sca3PPointsBSplineOneD(
                          1./nE, ms_0 / eL,
                          1.-1./nE, (eL-ms_1) / eL,
                        )
                      ),
                      "tangentialBlade_"+str(line),
                      m3dGmsh,
                      ms_0,
                      ms_1
                    )
                    theEdge.setGrading( 1.0, gradings["tangentialBlade_"+str(line)][0] )
                
                else:
                    theEdge.meshTransfiniteWNElements( 1, 1.0, 5 )
        
        # from blade to block
        for line in bladeToBlockLines:
            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            theEdge.setNElements( self.nElementsNormal_ )
            theEdge.setGrading( 1.0, gradings["normalBlade"][0] )
        
        # makes mesh settings for Trailing edge mesh blocks
        if self.meshTEBlocks_ == True:

            # meshing trailing edge lines, tEMeshList has the following format:
            #   tEMeshList = [
            #        [[[tEHub], [tEShroud]], -1.0],
            #        ...
            #    ]
            for meshList in tEMeshList:
                for edgeTag in meshList[0]:
                    edge = m3dGmsh.getModel().getDtGmshEdgeByTag(edgeTag[0])
                    edge.setNElements( self.nElementsNormal_ )
                    #edge.setGrading( meshList[1], gradings["normalBlade"][0] )
        
        #
        # add observers
        #

        # read mesh
        ob = bVOReadMSH()
        ob.thisown = False
        ob.jInit(
          jsonPrimitive(
            '{'
              '"_filename" : "",'
              '"_mustRead" : false'
            '}'
          ),
          None, None, None, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
       
        #
        # rotational periodicity
        #
        theT = xYz_rPhiZ()
        theT.jInit( 
          jsonPrimitive()\
            .appendStr("label", "xYz_rPhiZ")\
            .appendDtPoint3("_origin", dtPoint3(0,0,0))\
            .appendDtVector3("_rotAxis", dtVector3(0,0,-1))\
            .appendDtVector3("_refAxis", dtVector3(1,0,0)), 
          None, 
          None, 
          None, 
          None 
        )
        bC = baseContainer()
        bC.ptrTransformerContainer().add( theT )
        
        # add observers for all periodic faces
        for periodic in periodics:
            ob = bVOSetRotationalPeriodicity()
            ob.jInit(
              jsonPrimitive()\
                .appendStr("_faceMaster", periodic[0].getPhysicalString())\
                .appendStr("_faceSlave", periodic[1].getPhysicalString())\
                .appendDtTransformer( "_dtT", theT ), 
              bC, None, None, None, None, m3dGmsh 
            )
            ob.preUpdate()
        
        # dump model information
        ob = bVODumpModel()
        ob.thisown = False
        ob.jInit(
          jsonPrimitive(),
          None, None, None, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
        
        if self.boundaryLayerDir == None:
            # getting boundary layer direction for unstructured faces
            #  with transfinite layers
            self.boundaryLayerDir = self.detectBoundaryLayerDirection(
              m3dGmsh.getModel(),
              unstrFacesAndh2sLines
            )
        #
        # setting mesh rules
        #
        ob = bVOMeshRule()
        ob.thisown = False
        ob.jInit(
          jsonPrimitive(
            '{'
              '"option" : ['
                '{"name" : "debug", "value" : "true"}'
              '],'
              '"_rule1D" : ['
                '"dtMeshFreeGradingGEdge(*)"'
              '],'
              '"_rule2D" : ['
                '"dtMeshGFaceWithTransfiniteLayer(*inlet*)",'
                '"dtMeshGFaceWithTransfiniteLayer(*outlet*)",'
                '"dtMeshGFaceWithTransfiniteLayer(*suction_tri_*)",'
                '"dtMeshGFaceWithTransfiniteLayer(*pressure_tri_*)",'
                '"dtMeshTransfiniteGFace(*)",'
                '"dtMeshGFace(*)"'
              '],'
              '"_rule3D" : ['
                '"dtMeshGRegionWithBoundaryLayer(R_0)",'
                '"dtMeshGRegion(*)"'
              '],'
              '"_only" : [],'
              '"dtMeshOperator" : ['
                '{'
                  '"name" : "dtMeshFreeGradingGEdge",'
                  '"label" : "dtMeshFreeGradingGEdge",'
                  '"typeTransfinite" : '+self.gradingsTypeTransfinite(gradings)+','
                  '"gradingFunctions" : {'
                    '"analyticFunction" '
                      ': '+self.gradingsGradingFunctions(gradings)+' '
                  '}'
                '},'
               '{'
                  '"name" : "dtMeshGEdge",'
                  '"label" : "dtMeshGEdge"'
                '},'
                '{'
                  '"name" : "dtMeshGFace",'
                  '"label" : "dtMeshGFace"'
                '},'
                '{'
                  '"name" : "dtMeshTransfiniteGFace",'
                  '"label" : "dtMeshTransfiniteGFace"'
                '},'
                '{'
                  '"name" : "dtMeshGRegion",'
                  '"label" : "dtMeshGRegion",'
                  '"_minQShapeMetric" : 0.0,'
                  '"_relax" : 0.1,'
                  '"_nPyramidOpenSteps" : 10,'
                  '"_nSmooths" : 3'
                '},'
                '{'
                  '"option" : ['
                    '{"name" : "debug", "value" : "true"}'
                  '],'
                  '"name" : "dtMeshGRegionWithBoundaryLayer",'
                  '"label" : "dtMeshGRegionWithBoundaryLayer",'
                  #'"_nSpacingSteps" : ['
                  #+str(self.nBoundaryLayers_-1)+', '
                  #  +str(self.nBoundaryLayers_-1)+
                  #'],'
                  '"_nSpacingSteps" '
                    ': '+str(
                      [-(self.nBoundaryLayers_-1), -(self.nBoundaryLayers_-1)]
                    )+','
                  '"_nNormalSmoothingSteps" : 1000,'
                  '"_nGrowingSmoothingSteps" : 1000,'
                  '"_maxGrowingRatePerStep" : 1.10,'
                  '"_maxDihedralAngle" : '+str(float(np.pi*360./180.))+','
                  '"_faceLabel" : ['
                    '"hub_0", "shroud_0"'
                  '],"'
                  '_slidableFaceLabel" : ['
                    '"*inlet*", "*outlet*", "*pressure_*", "*suction_*", "*coupling_*"'
                  '],'
                  '"dtMesh3DOperator" : "dtMeshGRegion",'
                  '"_fixedFaceLabel" : ['
                  ']'
              '},'
                +jsonPrimitive()\
                  .appendStr("name", "dtMeshGFaceWithTransfiniteLayer")\
                  .appendStr("label", "dtMeshGFaceWithTransfiniteLayer")\
                  .appendVectorInt("_nLayers",
                    vectorInt(
                      [
                        self.nBoundaryLayers_,
                        self.nBoundaryLayers_
                      ]
                    )
                  )\
                  .appendInt("_direction", self.boundaryLayerDir)\
                  .appendInt("_nSmooth", 3)\
                  .toStdString()+
              ']'
            '}'
          ),
          None, None, self.lVH_aF(), None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
        
        # face to patch rule
        ob = bVOFaceToPatchRule()
        ob.thisown = False
        ob.jInit(
          jsonPrimitive(
            '{'
              '"_patchRule" : ['
                '":*hub*::'+self.label_+'_hub:",'
                '":*shroud*::'+self.label_+'_shroud:",'
                '":*blade*::'+self.label_+'_blade:",'
                '":*inlet*::'+self.label_+'_inlet:",'
                '":*outlet*::'+self.label_+'_outlet:",'
                '":*suction*::'+self.label_+'_suction:",'
                '":*block*::'+self.label_+'_suction:",'
                '":*pressure*::'+self.label_+'_pressure:",'
                '":*coupling*:::"'
              '],'
              '"_regRule" : ['
                '":*::'+self.label_+':"'
              ']'
            '}'
          ),
          m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)

        # write mesh
        ob = bVOWriteMSH()
        ob.thisown = False
        ob.jInit(
          jsonPrimitive('{"_filename" : "", "_saveAll" : true}'),
          None, None, None, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
        #ob.postUpdate()
        
        # orient volumes
        ob = bVOOrientCellVolumes()
        ob.thisown = False
        ob.jInit(
            jsonPrimitive('{"_positive" : true}'), m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)

        m3dGmsh.thisown = False

        self.appendBoundedVolume(m3dGmsh)
        
        return
    @staticmethod
    def detectFirstAndSecond(
        channel: map3dTo3d, direction: int
    ) -> Tuple[map2dTo3d, map2dTo3d]:
        """Detect first and second faces in a volume's parameter direction.
        
        This method returns the faces of a `map3dTo3d` object at 
        0 and 100 percent of the u-, v-, or w-paramter.

        Parameters
        ----------
        channel: map3dTo3d
          Volume.
        direction: int
          Direction in uvw

            - 1 -> U
            - 2 -> V
            - 3 -> W

        Returns
        -------
        first: map2dTo3d
          First segment in direction
        second: map2dTo3d
          Second segment in direction
        """

        firstPar = 0.0
        secondPar = 1.0
        if direction<0:
          firstPar = 1.0
          secondPar = 0.0

        if direction == 1:
          first = channel.segmentConstUPercent(firstPar)
          second = channel.segmentConstUPercent(secondPar)
        elif direction == 2:
          first = channel.segmentConstVPercent(firstPar)
          second = channel.segmentConstVPercent(secondPar)
        elif direction == 3:
          first = channel.segmentConstWPercent(firstPar)
          second = channel.segmentConstWPercent(secondPar)
        else:
          raise ValueError("Wrong direction.")

        return first, second

    @staticmethod
    def extractEdgesInFirstAndSecond(
        theModel: dtGmshModel, 
        faces: List[map2dTo3d], 
        first: map2dTo3d, 
        second: map2dTo3d
    ) -> Tuple[List[int], List[int]]:
        """Extracts edges of faces which lie on another first or second face.
        
        This method: 

            - Iterates over ``faces`` and extracts the edges of each face
            - Checks if any of the edges are located on the ``first`` face
            - Appends the edge to ``firstEdges`` if the check applies
            - Checks if any of the edges are located on the ``second`` face
            - Appends the edge to ``secondEdges`` if the check applies
            - Returns ``firstEdges`` and ``secondEdges``

        Parameters
        ----------
        theModel: dtGmshModel
          Gmsh model
        faces: List[map2dTo3d]
          List of faces
        first: map2dTo3d
          First face
        second: map2dTo3d
          Second face

        Returns
        -------
        firstEdges: List[int]
          Edges of on first face
        secondEdges: List[int]
          Edges of on second face
        """

        firstEdges = []
        secondEdges = []
        # iterate over faces
        for face in faces:
          # iterate over the edges
          for edge in [
            face.segmentConstUPercent(0.0),
            face.segmentConstVPercent(1.0),
            face.segmentConstUPercent(1.0),
            face.segmentConstVPercent(0.0),
          ]:
            # check if edge lies on the first face
            if bool_map1dTo3dInMap2dTo3d(edge, first).result():
              firstEdges.append( 
                theModel.addIfEdgeToGmshModel( edge )
              ) 
              logging.debug("Detect first edge tag = %d" % firstEdges[-1] )
              
              # check if the face lies on the second face
            elif bool_map1dTo3dInMap2dTo3d(edge, second).result():
              secondEdges.append( 
                theModel.addIfEdgeToGmshModel( edge )
              )
              logging.debug("Detect second edge tag = %d" % secondEdges[-1])
        logging.info(
          "firstEdges = %s, secondEdges = %s" 
          % 
          (
            str(firstEdges), str(secondEdges)
          ) 
        )
        # return the edge lists
        return firstEdges, secondEdges

    @staticmethod
    def boundaryEdgeDirection( 
        theModel: dtGmshModel, 
        boundaryLayerDirCheck: List[ List[ Union[ map2dTo3d, List[int] ] ] ]
    ) -> int:
        """Determine the boundary layer direction of a face from two edges.

        This method:

            - Iterates over the faces and edges in the input list.
            - Checks whether the u- or v-parameters of the start and end points
              of the edges on a face are equal within a specified tolerance.
            - Determines and returns the corresponding boundary layer direction.

        Parameters
        ----------
        theModel : dtGmshModel
            Gmsh model.

        boundaryLayerDirCheck : List[List[map2dTo3d, List[int]]]
            List containing faces and their corresponding edges.

            The entries are organized as follows:

                - ``boundaryLayerDirCheck[i]`` : One face and its associated edges.
                - ``boundaryLayerDirCheck[i][0]`` : Face.
                - ``boundaryLayerDirCheck[i][1]`` : List of edges.

        Returns
        -------
        int
            Integer encoding the boundary layer direction.


        The boundary layer direction is determined by iterating over
        ``faceLines in boundaryLayerDirCheck`` in an outer loop and over the
        edges ``line in faceLines[1]`` in the inner loop.

        During each iteration of the inner loop, the start and end points
        ``p0_uv`` and ``p1_uv`` of the current edge are reparameterized in the
        parameter space of the surface ``face``.

        The method :meth:`inTolerance` is used to determine whether the
        difference between the u- or v-parameters lies within the tolerance
        specified by ``tol``. Depending on the result, the value of
        ``boundaryLayerDirT`` is incremented. The resulting value is appended to
        the list ``boundaryLayerDir``.

        At the end of each outer loop iteration, duplicate entries in
        ``boundaryLayerDir`` are removed using 
        ``boundaryLayerDir = list(dict.fromkeys(boundaryLayerDir))``.

        Within this workflow, all boundary layer directions must be oriented
        consistently. The condition ``len(boundaryLayerDir) != 1`` is therefore
        used to verify that all detected directions are identical. If this
        condition is not satisfied, an exception is raised.

        Finally, the boundary layer direction is encoded as an integer return
        value. If ``boundaryLayerDir[0]`` equals ``1``, the method returns
        ``0``. If ``boundaryLayerDir[0]`` equals ``2``, the method returns
        ``1``. Any other value results in an exception.
        """

        # set tolerance
        tol = 0.01
        # prepare direction list
        boundaryLayerDir = []

        # iterate over entires
        for faceLines in boundaryLayerDirCheck:
          # iterate over the edges of the current entry
          for line in faceLines[1]:
            theEdge = theModel.getDtGmshEdgeByTag( line )
            
            #
            # reparametrize the start and end points of the edge 
            # in the uv-parameters of the face (faceLines[0])
            #
            # start point
            p0_uv = faceLines[0].reparamPercentOnFace( 
              theEdge.getMap1dTo3d().getPointPercent(0.0)
            )
            # end point
            p1_uv = faceLines[0].reparamPercentOnFace(
              theEdge.getMap1dTo3d().getPointPercent(1.0)
            )

            boundaryLayerDirT = 0
            #
            # Check the tolerance of the start and end points in u and v direction
            # Increment boundaryLayerDirT accordingly
            #
            ## Original code with dtOO tolerance check
            ## some geometries fail due to high tolerances
            #if ( not analyticGeometry.inUVWTolerance( p0_uv.x(), p1_uv.x() ) ):
            #  boundaryLayerDirT = boundaryLayerDirT + 1
            #if ( not analyticGeometry.inUVWTolerance( p0_uv.y(), p1_uv.y() ) ):
            #  boundaryLayerDirT = boundaryLayerDirT + 2
            if ( not map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks.inTolerance(tol, p0_uv.x(), p1_uv.x() ) ):
              boundaryLayerDirT = boundaryLayerDirT + 1
            if ( not map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks.inTolerance(tol, p0_uv.y(), p1_uv.y() ) ):
              boundaryLayerDirT = boundaryLayerDirT + 2
            logging.debug("face: %s" % faceLines[0].getLabel())
            logging.debug("p0_uv = (%f, %f)" % (p0_uv.x(), p0_uv.y()))
            logging.debug("p1_uv = (%f, %f)" % (p1_uv.x(), p1_uv.y()))
            logging.debug("boundaryLayerDirT = %d" % (boundaryLayerDirT))

            # append the value to the list
            boundaryLayerDir.append( boundaryLayerDirT )

          # delete duplicate entries
          boundaryLayerDir = list(dict.fromkeys(boundaryLayerDir))

        # check that all directions are uniform
        if len(boundaryLayerDir)!=1:
          raise ValueError("BoundaryLayerDirection is not equal in all faces.")
        
        boundaryLayerDir = boundaryLayerDir[0]
        logging.info("boundaryLayerDir = %d" % boundaryLayerDir)
        
        # return the direction depending on the value
        if boundaryLayerDir==1:
          return 0
        elif boundaryLayerDir ==2:
          return 1
        else:
          raise ValueError("Direction should be 1 or 2.")

    @staticmethod
    def detectBoundaryLayerDirection( 
        theModel: dtGmshModel, 
        boundaryLayerDirCheck: List[ List[ Union[ map2dTo3d, List[int] ] ] ]
    ) -> int:
        """Determine the boundary layer direction of a face from two edges.
        
        This method: 

            - Runs :meth:`boundaryEdgeDirection`
            - Returns integer value encoding the boudary edge direction

        Parameters
        ----------
        theModel: dtGmshModel
          Gmsh model
        boundaryLayerDirCheck: List[ List[ map2dTo3d, List[int]]]
          List containing faces and corresponding edges.
          Entries correlate to:

                - boundaryLayerDirCheck[i] : Set of one face and its edges
                - boundaryLayerDirCheck[i][0] : Face
                - boundaryLayerDirCheck[i][1] : List of edges

        Returns
        -------
        int
        """
        boundaryLayerDir = \
          map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks.boundaryEdgeDirection(
            theModel, boundaryLayerDirCheck
          )
        # (1,0)[boundaryLayerDir==1])
        if boundaryLayerDir==0:
          return 1
        elif boundaryLayerDir ==1:
          return 0
        else:
          return -1

    def inTolerance(
            tol: float,
            p0: float,
            p1: float
        ) -> bool:
        """Check if two float values lie within a tolerance.
        
        This method: 

            - Calculates the difference between two float values 
            - Returns boolean depending if tolerance is met

        Parameters
        ----------
        tol: float
          Tolerance
        p0: float
          First value
        p1: float
          Second value

        Returns
        -------
        bool
        """
        if p0 >= p1:
            diff = p0-p1
        else:
            diff = p1-p0

        if diff > tol:
            return False
        else:
            return True


    def addGrading(
        self, 
        gradings: Dict, 
        theRef: scaOneD, 
        gradingLabel: str, 
        m3dGmsh: map3dTo3dGmsh = None, 
        firstElementSize: float = 0.0,
        lastElementSize: float = 0.0
    ) -> Dict:
        """Create a grading and add it to the grading dictionary.

        This method:

            - Creates an entry in the grading dictionary.
            - Labels the grading function and appends it to the container.
            - Creates an observer for the grading.
            - Returns the updated grading dictionary.

        Parameters
        ----------
        gradings : Dict
            Grading dictionary.

            Each key corresponds to a grading identifier and contains the
            following information:

                - ``gradingLabel`` (``List[int, str]``): A list containing the
                  grading number and a string identifier.

        theRef : scaOneD
            Grading function.

        gradingLabel : str
            Grading label.

        m3dGmsh : map3dTo3dGmsh
            Gmsh topology object.

        firstElementSize : float
            Size of the first element in the grading.

        lastElementSize : float
            Size of the last element in the grading.

        Returns
        -------
        Dict
            Updated grading dictionary.

            Each key corresponds to a grading identifier and contains the
            following information:

                - ``gradingLabel`` (``List[int, str]``): A list containing the
                  grading number and a string identifier.


        For a new grading, an entry is created in the dictionary ``gradings``
        using ``gradingLabel`` as the key. The corresponding value is a list
        containing a unique grading number and identifier string of the form

        ::

            label_ + "_gradings_" + str(gradingNumber) + "_" + gradingLabel

        The grading function ``theRef`` is assigned this identifier string and
        added to the analytic function container of the calling class.

        Depending on whether the grading specifies only the first element size,
        ``(firstElementSize > 0.0) and (lastElementSize == 0.0)``,
        or both the first and last element sizes,
        ``(firstElementSize > 0.0) and (lastElementSize > 0.0)``,
        an observer of type ``bVOSetPrescribedElementSize`` is created using
        the specified element sizes and the grading function.

        The observer is then added to the topology.

        Finally, the updated grading dictionary is returned.
        """

        # set grading number
        gradingNumber = 2
        for ii in gradings:
          gradingNumber = max( gradingNumber, gradings[ii][0] )
        gradingNumber = gradingNumber + 1
        
        # create entry to grading dictionary
        gradings[gradingLabel] = [
          gradingNumber, 
          self.label_+"_gradings_"+str(gradingNumber)+"_"+gradingLabel
        ]
        logging.info(
          "%d -> %s" % (
            gradings[gradingLabel][0], gradings[gradingLabel][1]
          )
        )

        # label and push grading function
        theRef.setLabel( gradings[gradingLabel][1] )
        self.appendAnalyticFunction( theRef.clone() )
        
        # create observer
        if ( (firstElementSize>0.0) and (lastElementSize==0.0) ):
          ob = bVOSetPrescribedElementSize()
          ob.thisown = False
          ob.jInit( 
            jsonPrimitive()\
              .appendInt("_type", gradings[gradingLabel][0])\
              .appendReal("_firstElementSize", firstElementSize)\
              .appendAnalyticFunction(
                "_grading", self.lVH_aF()[ gradings[gradingLabel][1] ]
              ), 
            None, None, self.lVH_aF(), None, None, m3dGmsh 
          )
          m3dGmsh.attachBVObserver(ob)
        elif ( (firstElementSize>0.0) and (lastElementSize>0.0) ):
          ob = bVOSetPrescribedElementSize()
          ob.thisown = False
          ob.jInit( 
            jsonPrimitive()\
              .appendInt("_type", gradings[gradingLabel][0])\
              .appendReal("_firstElementSize", firstElementSize)\
              .appendReal("_lastElementSize", lastElementSize)\
              .appendAnalyticFunction(
                "_grading", self.lVH_aF()[ gradings[gradingLabel][1] ]
              ), 
            None, None, self.lVH_aF(), None, None, m3dGmsh 
          )
          m3dGmsh.attachBVObserver(ob)
         
        return gradings
  
    @staticmethod
    def gradingsTypeTransfinite(gradings: Dict) -> str:
        """Get the list of grading numbers and return it as a string.
        
        This method: 
        
            - Iterates over the entries of the grading dictionary
            - Appends the grading numbers to a list
            - Converts the list to a string and returns it

        Parameters
        ----------
        gradings: Dict
          Grading dictionary
          with each key containing identifiers for a grading:

                - gradingLabel (List[int, str]): Grading label key contains a list with 
                  grading number and a identifier string

        Returns
        -------
        str
        """
        retList = list()
        for ii in gradings:
          retList.append( gradings[ii][0] ) 
        logging.debug("Convert to %s" % str(retList))
        return str(retList)

    @staticmethod
    def gradingsGradingFunctions(gradings: Dict) -> str:
        """Create a string of grading identifiers in a jsonPrimitive format.
        
        This method: 
        
            - Iterates over the entries of the grading dictionary
            - Appends the grading identifiers in the format of a jsonPrimitive
            - Returns the string

        Parameters
        ----------
        gradings: Dict
          Grading dictionary
          with each key containing identifiers for a grading:

                - gradingLabel (List[int, str]): Grading label key contains a list with 
                  grading number and a identifier string

        Returns
        -------
        retStr: str
          Return string
        """
        retStr = '['
        first = True
        for ii in gradings:
          if first:
            retStr = retStr + '{ "label" : "'+gradings[ii][1]+'" }'
            first = False
          else:
            retStr = retStr + ', { "label" : "'+gradings[ii][1]+'" }'
        retStr = retStr + ']'
        logging.debug("Convert to %s" % retStr)
        return retStr 
