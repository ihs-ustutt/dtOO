from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import jsonPrimitive
from dtOOPythonSWIG import bVOMeshRule
from dtOOPythonSWIG import analyticGeometry
from dtOOPythonSWIG import map2dTo3d
from dtOOPythonSWIG import map3dTo3d
from dtOOPythonSWIG import map3dTo3dGmsh
from dtOOPythonSWIG import jsonPrimitive
from dtOOPythonSWIG import bVOWriteMSH
from dtOOPythonSWIG import bVONameRegions
from dtOOPythonSWIG import bVOFaceToPatchRule
from dtOOPythonSWIG import bVOAnalyticGeometryToFace
from dtOOPythonSWIG import labeledVectorHandlingAnalyticGeometry
from dtOOPythonSWIG import labeledVectorHandlingAnalyticFunction
from dtOOPythonSWIG import vectorReal
from dtOOPythonSWIG import scaTanhGradingOneD
from dtOOPythonSWIG import scaTanhGradingOneDCompound
from dtOOPythonSWIG import bVOSetPrescribedElementSize
from dtOOPythonSWIG import bVOSetPrescribedMeshSizeAtPoints
from dtOOPythonSWIG import bVOOrientCellVolumes

import logging
import numpy
from typing import List, Tuple

class map3dTo3dGmsh_gridFromLayers (dtBundleBuilder):
    """Create mesh topology as map3dTo3dGmsh.

    This class:

        - Creates a map3dTo3dGmsh topology object.
        - Adds the unstructured region to the topology.
        - Adds layer volumes to the topology.
        - Manages layer faces in the topology.
        - Applies mesh settings to the edges.
        - Applies gradings and mesh rules.
        - Renames faces.
        - Applies mesh settings to topology.


    Attributes
    ----------
    label_: str
      Label.
    layerList_: List[List[List[analyticGeometry] | List[bool]]]
      Layer lists for hub and shroud with 3d regions and bool list.
    nLayers_: int
      Number of elements normal to the walls in the layer volumes.
    firstElement_: float
      Size of first element on the walls.
    elementSizeSw_: float
      Element size in streamwise direction.
    elementSizeCirc_: float
      Element size in circumferential direction.
    unstructured_: analyticGeometry
      Multi bounded volume of the unstructured region.
    unstructuredSurfaces_: List[analyticGeometry]
      Bounding faces of the mult bounded volume.
    map3dTo3dGmshJson_: jsonPrimitive
      JSON structure for map3dTo3dGmsh.

    Examples
    --------
    None 


    The class is used to create the mesh topology of a flow channel consisting 
    of five- or six-sided layer volumes on the hub and shroud walls and a 
    multiple bounded volume inside the flow domain, connecting to the layer 
    volumes.

    The layer volumes are meshed transfinite and the multiple bounded volume is 
    meshed unstructured.

    In the constructor, the input parameters are instantiated. The multiple 
    bounded volume is instantiated as ``unstructured_``. The list with its
    bounding surfaces is instantiated as ``unstructuredSurfaces_``.

    The layer list is instantiated as ``layerList``. Its structure is as 
    follows:

    .. code-block:: python

        layerList = List[
            List[
                List[analyticGeometry],
                List[bool]
            ]
        ]

    The entries correspond to the following values:

    - ``layerList[0]``: Hub layers
    - ``layerList[1]``: Shroud layers
    - ``layerList[i][0]``: List of layer volumes.
    - ``layerList[i][1]``: List of Boolean values indicating whether the corresponding
      layer is located on a radius of zero.

    The layers are meshed with a grading extending from the wall faces into the
    flow domain. The number of elements in the grading is defined as ``nLayers_``.
    The size of the first element on the wall is specified with ``firstElement_``.

    The number of elements in streamwise and circumferential direction is set 
    with ``elementSizeSw_`` and ``elementSizeCirc_``. These parameters correspond 
    to maximal element sizes on the hub and shroud walls.

    The size of the elements in the unstructured region is set with the minimal 
    and maximal characteristic lengths ``charLengthMin`` and ``charLengthMax``.

    The topology settings are defined with a ``jsonPrimitive`` object instantiated 
    as ``map3dTo3dGmshJson_``. Here, the characteristic lengths of the unstructured 
    mesh elements are applied.

    With the :meth:`build` method, the mesh settings are applied to the topology. 
    The methods :meth:`detectFirstAndSecond` and 
    :meth:`getCommonEdgesByPhysicalFaces` are used to organize the faces and edges 
    of the layer volumes.

    The mesh topology is appended to the bounded volume container at the end of 
    the :meth:`build` method.  
    """
    def __init__(self,
                 mv: analyticGeometry, 
                 bs: List[analyticGeometry],
                 label: str,
                 layers: List[List[List[analyticGeometry] | List[bool]]],
                 nElementsLayer: int,
                 firstElement: float,
                 elementSize_sw: float,
                 elementSize_circ: float,
                 charLengthMin: float = 0.05,
                 charLengthMax: float = 0.10
        ) -> None :
        
        """Constructor

        Parameters
        ----------
        mv: analyticGeometry
          Multi bounded volume of the unstructured region
        bs: List[analyticGeometry]
          Bounding faces of the mult bounded volume
        label: str
          Label.
        layers: List[List[List[analyticGeometry] | List[bool]]]
          Layer lists for hub and shroud with 3d regions and bool list.
        nElementLayer: int
          Number of elements in grading.
        firstElement: float
          size of first element in grading.
        elementSize_sw: float
          Element size in streamwise direction
        elementSize_circ: float
          Element size in circumferential direction
        charLengthMin: float
          Minimal characteristic length of elements in unstructured region.
        charLengthMax: float
          Maximal characteristic length of elements in unstructured region.
        
        Returns
        -------
        None
        """
        super(map3dTo3dGmsh_gridFromLayers, self).__init__()
        
        # setting global params
        self.label_ = label
        self.layerList_ = layers
        self.nLayers_ = nElementsLayer
        self.firstElement_ = firstElement
        self.elementSizeSW_ = elementSize_sw
        self.elementSizeCIRC_ = elementSize_circ
        self.unstructured_ = mv
        self.unstructuredSurfaces_ = bs

        logging.info( "Building %s ..." % (self.label_) )
        
        #logMe.initLog('layerMesh.log')
        
        # setting up volume
        self.map3dTo3dGmshJson_ = jsonPrimitive(
            '{"label" : "'+self.label_+'",'
                '"option" : ['
                '{"name" : "[gmsh]General.Terminal", "value" : "1."},'
                '{"name" : "[gmsh]General.Verbosity", "value" : "100."},'
                '{"name" : "[gmsh]General.ExpertMode", "value" : "1."},'
                '{'
                  '"name" : "[gmsh]Mesh.CharacteristicLengthMin", '
                  '"value" : "'+str(charLengthMin)+'"'
                '},'
                '{'
                  '"name" : "[gmsh]Mesh.CharacteristicLengthMax", '
                  '"value" : "'+str(charLengthMax)+'"'
                '},'
                '{"name" : "[gmsh]Mesh.Algorithm", "value" : "1"},'
                '{"name" : "[gmsh]Mesh.MeshSizeExtendFromBoundary", "value" : "1"},'
                '{"name" : "[gmsh]Mesh.MeshSizeFromPoints", "value" : "1"}'
              '],'
              '"analyticGeometry" : []'
            '}'
        )

    def build(self) -> None:
        """Build part.
        
        This method is the main method of the class.

            - Creates a ``map3dTo3dGmsh`` topology object.
            - Adds the unstructured region to the topology.
            - Adds layer volumes to the topology.
            - Manages layer faces in the topology.
            - Applies mesh settings to the edges.
            - Applies gradings and mesh rules.
            - Renames faces.
            - Applies mesh settings to the topology.

        Parameters
        ----------
        None

        Returns
        -------
        None


        The topology object ``m3Gmsh`` is created with the settings in
        ``map3dTo3dGmsh_``.

        The labeled vector handling objects ``aG`` and ``aF`` are created to handle
        analytic geometries and functions in this method.

        The bounding faces of the unstructured region in ``unstructuredSurfaces_`` are
        labeled in the getter method ``getUnstructuredRegion`` of the class
        ``analyticGeometry_layerRegion``. The labels are as follows:

            - ``"periodicUnstruct_0"`` : First periodic surface
            - ``"periodicUnstruct_1"`` : Second periodic surface
            - ``"interface_unstruct"`` : Inlet of the flow domain's unstructured region
            - ``"outlet_unstruct"`` : Outlet of the flow domain's unstructured region
            - ``"para" + str(i)`` : Connecting faces to the layer volumes

        The bounding faces that are not labeled with the string ``para`` are pushed
        into ``aG`` by iterating over the list.

        :numref:`gridLayersMeshFaces` shows the face ``outlet_unstruct`` in orange and
        ``interface_unstruct`` in red. The faces labeled with ``para`` are equal to
        the wall parallel faces of the layer volumes (purple). The periodic faces of
        the unstructured region are not shown.

        .. _gridLayersMeshFaces:
        .. figure:: meridionalFigs/layersMeshFaces.png
           :width: 50%
           :align: center

           Faces in the layered flow domain.
        
        The multiple bounded volume of the unstructured region is added to the topology
        and allocated in ``unstrct3d``.

        In a nested loop over ``layerList_``, the layer volumes of the hub and shroud
        are added to the topology. The faces of the layers are added to ``aG``.
        The activities inside the loop are illustrated in the following figure.

        .. _gridLayers_activity0:
        .. figure:: meridionalFigs/gridLayers_activity0.png
           :width: 70%
           :align: center

           Activities for adding the layer volumes to the topology.

        **Loop over Hub and Shroud**

            ::

                i_hs in range(len(layerList_))

            The first level loop iterates over the hub and shroud data in
            ``layerList_``. According to the iterator ``i_hs``, the ``label``
            variable is set to the strings ``hub`` or ``shroud``.
            
            **Loop over Layers**

                ::

                    i_l in range(len(layerList[i_hs][0]))

                The second level loop iterates over the hub or shroud layers in
                ``layerList_[i_hs]``. The iterator is ``i_l``.

                With the method :meth:`detectFirstAndSecond`, the faces of each layer are
                identified. The method takes the layer volume as a ``map3dTo3d`` object
                and the parameter direction as an integer input. By calling the function
                once for each parameter direction, all faces of the layer volume are
                returned. The returned faces are allocated to the following parameters:

                    - ``ortho0`` and ``ortho1`` : Faces orthogonal to the wall (:numref:`gridLayersMeshFaces` dark blue)
                    - ``periodic0`` and ``periodic1`` : Periodic faces on the flow domain segment (:numref:`gridLayersMeshFaces` light blue (``periodic0`` not shown))
                    - ``channel`` : Wall faces on hub and shroud (:numref:`gridLayersMeshFaces` green)
                    - ``parallel`` : Faces extending parallel to the wall (:numref:`gridLayersMeshFaces` purple)

                Here, ``ortho0`` is the upstream face of each layer volume and
                ``ortho1`` is the downstream face. The faces ``ortho0`` and ``ortho1``
                are labeled as follows:

                    - ``ortho0`` : ``"ortho_"+label+str(i_l)``
                    - ``ortho1`` : ``"ortho_"+label+str(i_l)+1``

                The layer volumes are added to the topology ``m3dGmsh`` and receive the
                region ID ``rID``.

                The labeling of the other faces depends on whether the layer volume is
                five- or six-sided. A layer is five-sided if it is located on the radius
                of zero. This is checked with the list in ``layerList_[i_hs][1]``.
                This differentiation is necessary, because different meshing strategies 
                have to be applied.
                

                **The layer is six sided:**

                    The layer is six sided when
                    ``layerList_[i_hs][1][i_l] == False`` applies. In this case, the naming
                    of the faces is as follows:

                        - ``periodic1`` : ``"periodic0_"+label+str(i_l)``
                        - ``periodic0`` : ``"periodic1_"+label+str(i_l)``
                        - ``channel`` : ``"channel_"+label+str(i_l)``
                        - ``parallel`` : ``"parallel_"+label+str(i_l)``

                    The mesh settings are set to transfinite and recursive.

                **The layer is five sided:**

                    If the layer is five sided, the string ``5s`` is added to the face names:

                        - ``periodic1`` : ``"periodic05s_"+label+str(i_l)``
                        - ``periodic0`` : ``"periodic15s_"+label+str(i_l)``
                        - ``channel`` : ``"channel5s_"+label+str(i_l)``
                        - ``parallel`` : ``"parallel5s_"+label+str(i_l)``

                    In this case, the layer cannot be meshed completely transfinite recursive.
                    Its mesh settings have to be applied directly to the edges.

                The labeled faces are pushed into ``aG``.
                ``ortho0`` is pushed into ``aG`` in every iteration. ``ortho1`` is only
                pushed in the last iteration
                ``i_l == len(layerList_[i_hs][0])-1`` if the last layer is not on a radius
                of zero ``layerList_[i_hs][1][i_l] == False``.
        
        **Apply Mesh Sizing**

        With an observer of the class ``bVOAnalyticGeometryToFace``, the labeled
        faces in ``aG`` are added to the topology ``m3dGmsh``.
        
        The mesh settings are applied to the edges of the layer volumes. The
        following figure shows the edges on which mesh settings are applied.

        .. _layersMeshSetting:
        .. figure:: meridionalFigs/layersMeshSetting.png
           :width: 50%
           :align: center

           Edges in the flow domain on which mesh settings are applied.
           ``channelToParallelLines`` (green), ``swLines`` (magenta), and
           ``circLines`` (blue). On grey edges no mesh settings are applied.
        
        **Apply Gradings on the Walls**

            The grading on the hub and shroud layers is set on the edges extending from
            the ``channel`` to ``parallel`` faces. The edges are stored in
            ``channelToParallelLines``. By iterating over these edges, the grading is
            applied with the method ``setGrading``. The first input of this method is
            the direction of the grading, and the second one is an identifier for the
            grading function. The number of elements is set to ``nLayers_``.

            While iterating over the edges, the sum of their edge lengths is calculated.
            Using the number of edges in ``channelToParallelLines``, the mean edge
            length is calculated. By dividing this value by ``nLayers_``, the mesh size
            ``meshSizeAtPoints`` at the connection face between the layer and the
            unstructured volume is estimated.

        To set the number of elements on the edges in the streamwise
        (:numref:`layersMeshSetting` (magenta)) and circumferential directions
        (:numref:`layersMeshSetting` (blue)), an iteration over the layer list is
        performed. The following figure illustrates the processes in an activity
        diagram.

        .. _gridLayers_activity1:
        .. figure:: meridionalFigs/gridLayers_activity1.png
           :width: 90%
           :align: center

           Activities for the mesh settings in the streamwise and circumferential directions.
           Pink action blocks correspond to operations on the streamwise edges, blue action
           blocks correspond to operations on circumferential edges.
        
        **Prepare List**

            The list ``lChannel_circ = [0,0]`` is prepared. The two values in this list
            are used in the loop to store the maximum lengths of the circumferential
            edges on the hub and shroud walls.
 
        **Loop over Hub And Shroud**

            ::

                i_hs in range(len(layerList_))

            Similarly to the loop in :numref:`gridLayers_activity0`, a nested loop
            over the two levels of ``layerList_`` is performed. The first level loop
            iterates over the hub and shroud layers. The string value of ``label`` is
            set accordingly.

            **Loop over Layers**

                ``i_l in range(len(layerList_[i_hs][0]))``

                The second level loop iterates over the specific layers in the hub or
                shroud set.
                
                **Edges in Streamwise Direction**

                    The length ``lChannel_sw`` of the wall edge of the current layer in
                    the streamwise direction is calculated. The edge is returned as the
                    common edge between the ``channel`` and the ``periodic0`` faces with
                    the method :meth:`getCommonEdgesByPhysicalFaces`.

                    With this length, the number of elements ``nE`` is calculated by
                    dividing the length through the element size ``elementSizeSW_``. By
                    rounding ``nE`` to the next highest integer value, it is ensured that
                    ``elementSizeSW_`` is the maximal element size on the wall face in
                    the streamwise direction.

                    The handling of six- and five-sided layers differs here. This is
                    checked with the value in ``layerList_[i_hs][1][i_l]``.

                **Layer is six sided**

                    ``layerList_[i_hs][1][i_l] == False``

                    The streamwise edges (:numref:`layersMeshSetting` (magenta))
                    extending between the ``ortho`` faces of the layer are returned
                    by the method ``getDtGmshEdgeTagListByFromToPhysical`` of the
                    Gmsh model.

                **Layer is five sided**

                    ``layerList_[i_hs][1][i_l] == True``

                    The four edges are returned by the method
                    :meth:`getCommonEdgesByPhysicalFaces`. Here, the edges of the
                    ``channel`` or the ``parallel`` faces are compared with the
                    ``periodic0`` and ``periodic1`` faces. The common edges of these
                    faces are stored in the list ``swLines``.

                    The meshing of the four sided faces ``ortho``, ``periodic0``,
                    and ``periodic1`` of the layer is set to transfinite with a
                    recombine.
                
                **Apply Mesh Settings on Edges in Streamwise Direction**

                    By iterating over ``swLines``, the number of elements ``nE`` is set
                    on the edges.

                **Edges in Cricumferential Direction**

                    The length ``l_ortho`` of the upstream circumferential edge of the
                    layer volume is returned. If the length is greater than the value
                    stored in ``lChannel_circ``, the value replaces the current entry in
                    the list.

                    If the iteration is at the last layer and the layer is not located on
                    a radius of zero, the length ``l_ortho`` is calculated and compared
                    for the downstream circumferential edge.
            
            
            **Apply Mesh Settings on Edges in Circumferential Direction**

                After the second level loop for a layer region has concluded, the number of
                elements according to the maximal edge length is set on the circumferential
                edges.

                The number of elements ``nE`` is calculated with the maximal edge length
                ``lChannel_circ[i_hs]`` and the element size ``elementSizeCirc_``. By
                rounding the value of ``nE`` to the next higher integer, it is ensured that
                the specified element size represents a maximum size along the
                circumferential direction of the layer wall.

                The circumferential edges ``circLines``
                (:numref:`layersMeshSetting` (blue)) extend between the ``periodic`` faces.
                By iterating over ``circLines``, the number of elements is set.
        
        **Create Observers**

            The observer ``bVOSetPrescribedMeshSizeAtPoints`` is applied. This observer
            is used to set the mesh size of the unstructured region at the connection
            points with the layer volumes to the value calculated in
            ``meshSizeAtPoints``.

            The object of the grading function ``theRef`` is created with the `dtOO`
            class `scaTanhGradingOneDCompound`. It is labeled ``aF_grading`` and
            pushed into ``aF``.

            An observer of the class ``bVOSetPrescribedElementSize`` is created. This
            observer combines the analytic grading function specified in ``theRef`` with
            the ``_type`` identifier and the size of the first element in the grading
            ``firstElement_``.

            The mesh rules are applied with the observer ``bVOMeshRule``. The rules
            ``dtMeshFreeGradingGEdge``, ``dtMeshGFace``, and ``dtMeshGRegion`` are used
            for all edges, surfaces, and volumes.

            The observer of the class ``bVOFaceToPatchRule`` is used to rename the
            faces. This is done so that the naming of the face regions is consistent
            with the setup rules of the simulation.

            The faces are renamed with the label ``label_`` and a string for the region.
            The following names are assigned to the faces:

                - ``label_ + '_hub'`` : hub walls
                - ``label_ + '_shroud'`` : shroud walls
                - ``label_ + '_inlet'`` : inlet surfaces
                - ``label_ + '_outlet'`` : outlet surfaces
                - ``label_ + '_periodic0'`` : first periodic segment faces
                - ``label_ + '_periodic1'`` : second periodic segment faces

            With an if-condition, it is checked whether the last hub wall extends to a
            radius of zero. If this is the case (see
            :numref:`gridLayersMeshFaces`), the hub layer regions are not part of the
            outlet.

            If the last hub wall is not on zero, the last ``ortho`` face of the hub
            regions is added as an outlet face.

            With the observer of the class ``bVOWriteMSH``, the settings for the created
            ``.msh`` file are defined.

            The observer ``bVOOrientCellVolumes`` is applied with the setting
            ``"_positive" : true``.
        
        **Returns**

            The method `appendBoundedVolume` is used to append the topology object
            ``m3dGmsh`` to the container objects in the main class.

            A mesh resulting from this topology is shown in the following figure.

            .. _layerMesh:
            .. figure:: meridionalFigs/layersMesh.png
               :width: 40%
               :align: center

               Mesh of a draft tube cone resulting from the described topology.
        """
        
        #
        # create the topology object
        #
        m3dGmsh = map3dTo3dGmsh()
        m3dGmsh.jInit(
          self.map3dTo3dGmshJson_, None, None, None, None, None
        )
        
        #
        # create the labeled vector handling objects
        #
        aF = labeledVectorHandlingAnalyticFunction()
        aG = labeledVectorHandlingAnalyticGeometry()
        logging.info("Creating Layer Mesh on hub and shroud")
        
        #
        # add the unstructured region to the model
        #
        unstruct3d = m3dGmsh.getModel().addIfToGmshModel(self.unstructured_)
        logging.info("unstruct3d = %d" % unstruct3d)
        self.appendAnalyticGeometry(
               self.unstructured_.clone(),
               "debug_unstructuredRegion_"+self.label_)
        
        #
        # iterate over the surfaces of the unstructured region
        # the faces were already labeled in getUnstructuredRegion()
        #
        for face in self.unstructuredSurfaces_:

            # the interface and the outlet surfaces as well as the periodic surfaces
            #  are added. Not the faces parallel to the hub and shroud curves ("para")
            if not (face.getLabel().startswith("para")):
                aG.push_back(face.clone())
                 
        # self.layerList_ has the following format:
        # self.layerList_ = [[hub layer lists],[shroud layer list]]
        # with:
        # [hub layer lists] = [[3d layer domain], [bool list radius zero]]
        # [bool list radius zero] tracks which layer segment has a radius of zero
        # those layers are meshed five sided the entrie for those layers is True 
        
        #
        # iterate over the layers and pushing their respective surfaces
        # six and five sided layers are treated differently
        #
        for i_hs in range(len(self.layerList_)):

            if i_hs == 0:
                label = "hub"
            else:
                label = "shroud"
            logging.info("Adding %s Layers. Number of Layers: %i" % (label, len(self.layerList_[i_hs][0])))
            
            #
            # iterate over the hub and shroud layers
            #
            for i_l in range(len(self.layerList_[i_hs][0])):
                
                # current 3d region
                layer3d = self.layerList_[i_hs][0][i_l]
                
                self.appendAnalyticGeometry(
                    layer3d.clone(),
                    "debug_layer_"+label+str(i_l)+"_"+self.label_
                )
                #
                # find the correct surfaces on the region by using the uvw direction
                # naming of faces:
                #   ortho    -> faces orthogonal to streamwise direction (connecting the layers)
                #   periodic -> periodic faces (extend from channel curve radially inside the channel)
                #   channel  -> faces on channel side
                #   parallel -> faces parallel to channel
                #
                ortho0, ortho1 = self.detectFirstAndSecond(layer3d, 2)
                periodic0, periodic1 = self.detectFirstAndSecond(layer3d, 1)
                channel, parallel = self.detectFirstAndSecond(layer3d, 3)
                
                # set the labels of the ortho faces
                ortho0.setLabel("ortho_"+label+str(i_l))
                ortho1.setLabel("ortho_"+label+str(i_l+1))

                # add the 3d layer as a region
                rID = m3dGmsh.getModel().addIfRegionToGmshModel(layer3d)
                
                # the labels are different for six and five sided regions
                # this is done becuase they have to be treated differently during meshing
                #
                # check in the layer list if the current layer is on a radius of zero
                if self.layerList_[i_hs][1][i_l] == False:
                    logging.info("%s layer no. %i is six sided" % (label, i_l))
                    periodic0.setLabel("periodic0_"+label+str(i_l))
                    periodic1.setLabel("periodic1_"+label+str(i_l))
                    channel.setLabel("channel_"+label+str(i_l))
                    parallel.setLabel("parallel_"+label+str(i_l))

                    # only six sided regions can be meshed completely transfinite
                    # Recursive means that all the edges and faces of the region are also tramsfinite
                    m3dGmsh.getModel().getDtGmshRegionByTag( rID ).meshTransfiniteRecursive()
                    m3dGmsh.getModel().getDtGmshRegionByTag( rID ).meshRecombineRecursive()
                else:
                    logging.info("%s layer no. %i is five sided" % (label, i_l))
                    periodic0.setLabel("periodic05s_"+label+str(i_l))
                    periodic1.setLabel("periodic15s_"+label+str(i_l))
                    channel.setLabel("channel5s_"+label+str(i_l))
                    parallel.setLabel("parallel5s_"+label+str(i_l))
                
                # push back the faces
                aG.push_back(ortho0)
                # ortho1 is only pushed in the last region if it is six sided
                # (ortho1 in a five sided region is rotated surface with radius of zero
                # this results in a degenerated face)
                if i_l == len(self.layerList_[i_hs][0])-1 and self.layerList_[i_hs][1][i_l] == False:
                    aG.push_back(ortho1)
                aG.push_back(periodic0)
                aG.push_back(periodic1)
                aG.push_back(channel)
                aG.push_back(parallel)
                                 
        # initialize the region labels
        ob = bVONameRegions()
        ob.jInit( jsonPrimitive('{ "_regionLabel" : [] }'), m3dGmsh )
        ob.preUpdate() 
        
        #
        # initialize the surface labels in gmsh
        #
        ob = bVOAnalyticGeometryToFace()
        ob.jInit(
          jsonPrimitive(
            '{'
              '"analyticGeometry" : ['
                '{"labels" : "ortho_*"},'
                '{"labels" : "channel*"},'
                '{"labels" : "periodic1*"},'
                '{"labels" : "periodic0*"},'
                '{"label" : "periodicUnstruct_1"},'
                '{"label" : "periodicUnstruct_0"},'
                '{"label" : "interface_unstruct"},'
                '{"label" : "outlet_unstruct"},'
                '{"labels" : "parallel*"}'
              '],'
              '"_inc" : 10.0,'
              '"_facesPerEntry" : []'
            '}'
          ),
          None, None, None, aG, None, m3dGmsh
        ) 
        ob.preUpdate()
        
        #
        # mesh the lines orthogonal to the channel lines
        # these lines connect the channel and parallel faces
        #
        channelToParallelLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("channel*","parallel*")
        # channel to parallel length
        c2pLength = 0
        
        #
        # iterate over the edges and apply mesh settings
        #
        for line in channelToParallelLines:
            logging.info( "meshing graded Line : ID: %i, number of elements: %i" % (line, self.nLayers_) )
            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            # mesh the lines with the specified number of elements and set the grading
            theEdge.meshTransfiniteWNElements( 1, 1, self.nLayers_ )
            theEdge.setGrading(1.0, 3)

            self.appendAnalyticGeometry(
                m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                "debug_layerEdge_channelToParallelLine_"+"_ID"+str(line)+"_"+self.label_
            )
            # sum up the channel to parallel lengths of the edges
            c2pLength = c2pLength + theEdge.length()
        
        # calculate the mean value of the edge lengths
        c2pLength = c2pLength / len(channelToParallelLines)
        
        # estimate the mesh size at the points
        meshSizeAtPoints =  c2pLength / self.nLayers_

        # list containing the lengths of the mesh lines in circumferential direction
        lChannel_circ = [0, 0]
        
        #
        # iterate over the hub and shroud layers
        # the mesh settings for the edges in streamwise and circumferential directions are set
        # regions with five sides are specially treated in this loop
        #
        for i_hs in range(len(self.layerList_)):

            if i_hs == 0:
                label = "hub"
            else:
                label = "shroud"
            
            # iterate over the individual layers in the hub and shroud layer list
            for i_l in range(len(self.layerList_[i_hs][0])):
                
                # find an edge on the channel in streamwise direction
                # this edge is the common edge between the channel and the periodic 
                # face in this layere region
                edges = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "channel*"+label+str(i_l)+"*",
                        "periodic0*"+label+str(i_l)+"*"
                    )
                # calculate the length and number of elements on this line
                # this number of elements is used for all streamwise lines in this layer region
                lChannel_sw = edges[0].getMap1dTo3d().length()
                nE = int(numpy.ceil(lChannel_sw/self.elementSizeSW_))                  
                logging.info("layer %s%i has a channel length of %.4f, meshing with %i elements" % (
                    label, i_l, lChannel_sw, nE))
                
                # find all streamwise layer curves for a six sided region
                if self.layerList_[i_hs][1][i_l] == False:
                    # the curves are found by searching the connecting lines of the orthogonal faces
                    swLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                            "ortho_"+label+str(i_l)+"*",
                            "ortho_"+label+str(i_l+1)+"*")
                
                # find streamwise layercurves for five sided regions
                #  needs special treatment because this region doesnt have a second ortho face
                elif self.layerList_[i_hs][1][i_l] == True:
                    # lines have to be found manually as the common lines 
                    #  between channel/parallel face and the periodic faces
                    line0 = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "channel5s_"+label+str(i_l)+"*",
                        "periodic05s_"+label+str(i_l)+"*"
                    )
                    line1 = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "channel5s_"+label+str(i_l)+"*",
                        "periodic15s_"+label+str(i_l)+"*"
                    )
                    line2 = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "parallel5s_"+label+str(i_l)+"*",
                        "periodic05s_"+label+str(i_l)+"*"
                    )
                    line3 = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "parallel5s_"+label+str(i_l)+"*",
                        "periodic15s_"+label+str(i_l)+"*"
                    )
                    swLines = [line0[0].tag(), line1[0].tag(), line2[0].tag(), line3[0].tag()]

                    # set the orthogonal and periodic faces of the five sided region as transfinite
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("ortho_"+label+str(i_l)+"*").meshTransfinite()
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("ortho_"+label+str(i_l)+"*").meshRecombine()
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic05s_"+label+str(i_l)+"*").meshTransfinite()
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic05s_"+label+str(i_l)+"*").meshRecombine()
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic15s_"+label+str(i_l)+"*").meshTransfinite()
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic15s_"+label+str(i_l)+"*").meshRecombine()
                
                #
                # set the mesh size of the streamwise layer lines of the region
                #
                for line in swLines:
                    self.appendAnalyticGeometry(
                        m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                        "debug_layerEdge_swLine_"+label+str(i_l)+"_ID"+str(line) +"_"+self.label_
                    )
                    logging.info( "meshing streamwise Line : %s%i, ID: %i, number of elements: %i" % (label, i_l, line, nE) )
                    m3dGmsh.getModel().getDtGmshEdgeByTag(line).meshTransfiniteWNElements(1,1,nE)
                   
                # find a layer curve in circumferential direction 
                #  as the common edge between the channel and the ortho face
                edges = self.getCommonEdgesByPhysicalFaces(
                    m3dGmsh,
                    "channel*"+label+str(i_l)+"*",
                    "ortho_"+label+str(i_l)+"*"
                )
                # length of circumferential curve
                l_ortho = numpy.abs(edges[0].getMap1dTo3d().length())
                logging.info("Circumferential edge : %s%i, ID: %i, length: %.4f" % (label, i_l, edges[0].tag(), l_ortho))
                # find the longest circumferential edge in the hub and shroud layers
                #  based on this length the number of elements in circ direction will be calculated
                if l_ortho > lChannel_circ[i_hs]:
                    lChannel_circ[i_hs] = l_ortho
                
                # find the last layer curve in circumferential direction of the last region
                #  this is only done for six sided regions 
                #  (the radius of the last circ line of five sided regions is zero)
                if i_l == len(self.layerList_[i_hs][0])-1 and self.layerList_[i_hs][1][i_l] == False:
                    edges = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "channel_"+label+str(i_l)+"*",
                        "ortho_"+label+str(i_l+1)+"*"
                    )
                    l_ortho = numpy.abs(edges[0].getMap1dTo3d().length())
                    logging.info("Circumferential edge : %s%i, ID: %i, length: %.4f" % (label, i_l+1, edges[0].tag(), l_ortho))
                    if l_ortho > lChannel_circ[i_hs]:
                        lChannel_circ[i_hs] = l_ortho                
                    
            # calculate the number of elements for the edges in circ direction from the longest edge length
            nE = int(numpy.ceil(lChannel_circ[i_hs]/self.elementSizeCIRC_))
            
            # find all circ edges on hub or shroud
            circLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                            "periodic0_"+label+"*",
                            "periodic1_"+label+"*"
                        ) 

            # set the number of elements on the circ edges for hub or shroud
            # number of elements have to be constant for these edges for transfinte meshing
            for line in circLines:
                self.appendAnalyticGeometry(
                    m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                    "debug_layerEdge_circLine_"+label+"_ID"+str(line) +"_"+self.label_
                )

                logging.info( "meshing circumferential Line : %s, ID: %i, number of elements: %i" % (label, line, nE) )
                m3dGmsh.getModel().getDtGmshEdgeByTag(line).meshTransfiniteWNElements(1,1,nE)
        
        # set an observer which prescribes meshsizes
        # meshSizeAtPoints is calculatate from the mean length of the channel to parallel
        # and the number of layers between them 
        ob = bVOSetPrescribedMeshSizeAtPoints()
        ob.thisown = False
        ob.jInit(
          jsonPrimitive()\
            .appendReal("_meshSize", meshSizeAtPoints),
          None, None, None, None, None, m3dGmsh
        )
        #m3dGmsh.attachBVObserver(ob)

        # initiale the grading function 
        # scaTanhGradingOneD(c, g, gMin, gMax) -> f(x)=c[0]+c[1]*tanh(g*(c[2]+c[3]*x))/tanh(g)
        theRef = scaTanhGradingOneDCompound(
                    scaTanhGradingOneD(
                        vectorReal([1.0, -1.0, 1.0, -1.0]),
                                        1.0,
                                        0.1, 5.0
                        )
                    )
        theRef.setLabel( "aF_grading" )
        aF.push_back( theRef.clone() )
        
        # set the first element size specified as a class variable 
        ob = bVOSetPrescribedElementSize()
        ob.thisown = False
        ob.jInit(
            jsonPrimitive(
                '{'
                '"_type": 3,'
                '"_firstElementSize": '+str(self.firstElement_)+','
                '"_grading" : {'
                    '"analyticFunction" : {"label" : "aF_grading"}'
                  '}'
                '}'
            ),
            None, None, aF, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
        
        # set mesh rules
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
                '"dtMeshGFace(*)"'
              '],'
              '"_rule3D" : ['
                '"dtMeshGRegion(*)"'
              '],'
              '"_only" : [],'
              '"dtMeshOperator" : ['
                '{'
                  '"name" : "dtMeshFreeGradingGEdge",'
                  '"label" : "dtMeshFreeGradingGEdge",'
                  '"typeTransfinite" : [3],'
                  '"gradingFunctions" : { "analyticFunction" : ['
                    '{"label" : "aF_grading"}'
                  ']}'
                '},'
                '{'
                  '"name" : "dtMeshGFace",'
                  '"label" : "dtMeshGFace"'
                '},'
                '{'
                  '"name" : "dtMeshGRegion",'   
                  '"label" : "dtMeshGRegion",'
                  '"_minQShapeMetric" : 0.0,'
                  '"_relax" : 0.1,'
                  '"_nPyramidOpenSteps" : 10,' 
                  '"_nSmooths" : 3'
                '}'
              ']'
            '}'
          ),
          None, None, aF, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
         
        ## useful debug statement to understand the naming of the layers and vizualize them
        ##  especially for the bVOFaceToPatchRule
        #for face in m3dGmsh.getModel().getDtGmshFaceListByPhysical("*"):
        #    print(face.getPhysicalString())
        #    self.appendAnalyticGeometry(
        #        face.getMap2dTo3d(),
        #        "debug_allFaces_"+face.getPhysicalString()
        #    )
        #print("length hub : ", str(len(self.layerList_[0][0])))
        #print("length shroud : ", str(len(self.layerList_[1][0])))
        
        #
        # setting bVOFaceToPatchRule, renames all the added faces
        # this is done to set boundary conditions in the of case later
        #
        ob = bVOFaceToPatchRule()
        ob.thisown = False
        # if the last hub layer is on radius 0 the unstructured region goes to radius zero
        # there is no hub layer on the outlet
        if self.layerList_[0][1][-1] == True:
            ob.jInit(
              jsonPrimitive(
                '{'
                  '"_patchRule" : ['
                    '":*channel*hub*::'+self.label_+'_hub:",'
                    '":*channel*shroud*::'+self.label_+'_shroud:",'
                    '":*ortho_*0_*::'+self.label_+'_inlet:",'
                    '":*interface_unstruct*::'+self.label_+'_inlet:",'
                    # outlet part of last layer
                    '":*ortho_shroud'+str(len(self.layerList_[1][0]))+'*::'+self.label_+'_outlet:",'
                    '":*outlet_unstruct*::'+self.label_+'_outlet:",'
                    '":*periodic0*::'+self.label_+'_periodic0:",'
                    '":*periodic1*::'+self.label_+'_periodic1:",'
                    '":*periodicUnstruct_0*::'+self.label_+'_periodic0:",'
                    '":*periodicUnstruct_1*::'+self.label_+'_periodic1:"'
                  '],'
                  '"_regRule" : ['
                    '":*::'+self.label_+':"'
                  ']'
                '}'
              ),
              m3dGmsh
            )
        # if the radius is not zero in the last hub layer the last ortho segment
        #  of the hub is also part of the outlet bondary
        else:
            ob.jInit(
              jsonPrimitive(
                '{'
                  '"_patchRule" : ['
                    '":*channel*_hub*::'+self.label_+'_hub:",'
                    '":*channel*_shroud*::'+self.label_+'_shroud:",'
                    '":*ortho_*0_*::'+self.label_+'_inlet:",'
                    '":*interface_unstruct*::'+self.label_+'_inlet:",'
                    '":*ortho_shroud'+str(len(self.layerList_[1][0]))+'*::'+self.label_+'_outlet:",'
                    '":*ortho_hub'+str(len(self.layerList_[0][0]))+'*::'+self.label_+'_outlet:",'
                    '":*outlet_unstruct*::'+self.label_+'_outlet:",'
                    '":*periodic0*::'+self.label_+'_periodic0:",'
                    '":*periodic1*::'+self.label_+'_periodic1:",'
                    '":*periodicUnstruct_0*::'+self.label_+'_periodic0:",'
                    '":*periodicUnstruct_1*::'+self.label_+'_periodic1:"'
                  '],'
                  '"_regRule" : ['
                    '":*::'+self.label_+':"'
                  ']'
                '}'
              ),
              m3dGmsh
            )

        m3dGmsh.attachBVObserver(ob)
        
        #m3dGmsh.makeGrid()    

        ob = bVOWriteMSH()
        ob.thisown = False
        ob.jInit(
          jsonPrimitive('{"_filename" : "", "_saveAll" : true}'),
          None, None, None, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
        #ob.postUpdate()

        ob = bVOOrientCellVolumes()
        ob.thisown = False
        ob.jInit(
            jsonPrimitive('{"_positive" : true}'), m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
        
        m3dGmsh.thisown = False

        self.appendBoundedVolume(m3dGmsh)

        return

    def detectFirstAndSecond(self,
                             channel: map3dTo3d, 
                             direction: int
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
          first segment in direction
        second: map2dTo3d
          second segment in direction
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
    
    def getCommonEdgesByPhysicalFaces(self,
                                      m3dGmsh,
                                      face0: str,
                                      face1: str):
        """Return the common edges between two faces.
        
        This method:

            - Iterates over the edges on both faces.
            - Finds faces with the same tag.
            - Returns them in a list.

        Parameters
        ----------
        channel: map3dTo3d
          Volume.
        face0: str
          Pyhsical name of first face
        face1: str
          Pyhsical name of second face

        Returns
        -------
        commonEdges: List[dtGmshEdge]
          List of edges on both faces
        """
        commonEdges = []
        edges0 = m3dGmsh.getModel().getDtGmshFaceByPhysical(face0).dtEdges()
        edges1 = m3dGmsh.getModel().getDtGmshFaceByPhysical(face1).dtEdges()

        for e0 in edges0:
            for e1 in edges1:
                if numpy.abs(e0.tag()) == numpy.abs(e1.tag()):
                    commonEdges.append(e0)

        return commonEdges
