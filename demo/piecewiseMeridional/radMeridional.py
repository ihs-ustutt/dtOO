
import logging
logging.basicConfig(
  format='[ %(asctime)s - %(levelname)8s - %(filename)s:%(lineno)d ]'
         ' - %(message)s', 
  datefmt='%d-%b-%y %H:%M:%S', 
  level=logging.DEBUG
)

logging.debug("Debug")
logging.info("Info")
logging.warning("Warning")
logging.error("Error")

import dtOOPythonSWIG as dtOO

import re
import numpy as np
import sys
import importlib
from typing import List


class radMeridional:
    
    """Create geometries and meshes of a radial flow machine.
     
    Attributes
    ----------
    container: dtBundle
      Initialization of the dtBundle.
    bC: baseContainer
      dtOO base container.
    cV: lvH_constValue
      Labeled vector handling of constant values.
    aF: lvH_analyticFunction
      Labeled vector handling of analytic functions.
    aG: lvH_analyticGeometry
      Labeled vector handling of analytic geometries.
    bV: lvH_boundedVolume
      Labeled vector handling of bounded volumes.
    dC: lvh_dtCase
      Labeled vector handling of cases.
    dP: lvH_dtPlugin
      Labeled vector handling of plugins.
    radMeridionalContour: analyticGeometry_piecewiseMeridionalRotContour
      Object that creates the meridional contour.

    Examples
    --------
    >>> import dtOOPythonSWIG as dtOO

    This example creates geometries and meshes of a parameterized radial machine.
    The machine consists of the following geometries:

        - Guide Vane Channel
        - Runner Channel
        - Draft Tube Cone

    The following figure shows the resulting meshes of the different geometries:
     
    .. _all:
    .. figure:: bladeFigs/allGeoms.png
       :width: 100%
       :align: center

       Final geometries and meshes of this example, with guide vane (top right),
       runner (top left), and draft tube cone (bottom).

    The meridional contour of the machine is defined with the method :meth:`createMeridional`
    through hub and shroud curves, as well as the interface definition. 

    The guide vane and runner channels are created from regular channels with 
    the method :meth:`createBlade`.

    The draft tube cone is created from the special curves with the method
    :meth:`createLayerRegion`.


    Meridional Contour
    ------------------
    
    The meridional contour of the flow machine is created through its hub and shroud curves.

    The hub and shroud curves are defined in separate lists named ``hubCurves`` and
    ``shroudCurves``. The curves have to be created so that their direction is the same as
    the downstream direction of the flow machine. Furthermore, the sequence of the curves
    in the lists must also correspond to the downstream direction of the machine.

    The following figure shows the hub and shroud curves with their respective numbers
    in the ``hubCurves`` and ``shroudCurves`` lists.

    .. _hsCurves1:
    .. figure:: meridionalFigs/hsCurve_noInterface.png
       :width: 50%
       :align: center

       Hub and shroud curves of the meridional channel.
       Numbering correlates with the numbering in the 
       ``hubCurves`` and ``shroudCurves`` lists.

    In this example, the first two of the hub and the shroud curves form the 
    regular channels. 
    The first curves build the regular channel for the guide vane, and the second 
    curves build the regular channel for the runner. 
    Because the runner channel of a radial turbine is highly curved, a parameterization 
    of those curves is introduced, which enables the generation of different channel 
    geometries.

    The parameterization is shown in the following figure.

    .. _meas:
    .. figure:: meridionalFigs/meas.png
       :width: 100%
       :align: center

       Measurements of the first two defined hub and shroud curves, 
       which build the guide vane and runner channels.

    :numref:`map-fig-var` gives the mapping between math symbols in :numref:`meas`
    and the naming of the variables in this example.

    .. _map-fig-var:
    .. csv-table:: Mapping between math symbols and variable names.
       :header: "Symbol", "Label"
       :widths: 10, 10
       :escape: #
       :align: center

       ":math:`d_{inlet}`"        ,``d_inlet``
       ":math:`l_{in,ext}`"       ,``l_inExt``
       ":math:`d_{out,hub}`"      ,``d_outHub``
       ":math:`d_{out,shroud}`"   ,``d_outShroud``
       ":math:`l_{hub,0}`"        ,``l_hub0``
       ":math:`l_{hub,1}`"        ,``l_hub1``
       ":math:`\gamma_{hub1}`"    ,``angle_hub1``
       ":math:`l_{shroud,0}`"     ,``l_shroud0``
       ":math:`l_{shroud,1}`"     ,``l_shroud1``
       ":math:`h_{inlet}`"        ,``h_inlet``
       ":math:`h_{hub}`"          ,``h_hub``
       ":math:`h_{shroud}`"       ,``h_shroud``

    Define the parameters of the first two hub and shroud curves: 

    >>> d_inlet = 2.58
    >>> l_inExt = 0.27
    >>> d_outHub = 0.4
    >>> d_outShroud = 1.865
    >>> l_hub0 = 0.38
    >>> l_hub1 = 0.38
    >>> angle_hub1 = 65 * np.pi/180
    >>> l_shroud0 = 0.13
    >>> l_shroud1 = 0.18
    >>> h_inlet = 0.36
    >>> h_hub = 0.68
    >>> h_shroud = 0.38

    The contour of the runner's meridional channel has a flow deflection from the radial inlet 
    to the axial outlet. In order to achieve this, the meridional 
    hub and shroud curves of the runner channel are defined from four control points. 

    The first and fourth control points form the start and the end points of the curves.
    Their positions are parameterized through the diameters :math:`d_{inlet}`, 
    :math:`d_{out,hub}` and :math:`d_{out,shroud}` and the heights :math:`h_{inlet}`,
    :math:`h_{hub}` and :math:`h_{shroud}`. 

    Through the second and third control points, the contour of the channel is set. 
    Their positions are defined through offsets from the first and fourth control points
    with the lengths :math:`l_{hub,0}`, :math:`l_{hub,1}`, :math:`l_{shroud,0}`,
    and :math:`l_{shroud,1}`. 
    With the angle :math:`\gamma_{hub1}`, the outlet angle of the hub curve can be set. 

    In the following lines, the displacements for the third control point of the hub curve 
    are calculated:

    >>> dx_hub1 = np.cos(angle_hub1)*l_hub1
    >>> dz_hub1 = np.sin(angle_hub1)*l_hub1

    In the following two code blocks, the hub and shroud curves are defined in the lists
    ``hubCurves`` and ``shroudCurves``. The control points are created as ``dtPoint3`` 
    and the curves as ``bSplineCurve_pointConstructOCC`` objects.
    The parameterization above is used to create the first two hub and shroud curves.

    Four hub curves are defined here. The first two are defined with the parameterization
    shown in :numref:`meas`. The third hub curve extends to an x-coordinate of zero and forms 
    the shaft end of the turbine.
    The fourth hub curve extends in the negative z-direction at a radius of zero.    

    >>> hubCurves = [
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+(d_inlet/2 + l_inExt), +0.00, +h_inlet)
    ...           << dtOO.dtPoint3(+d_inlet/2, +0.00, +h_inlet),
    ...         1
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+d_inlet/2, +0.00, +h_inlet)
    ...           << dtOO.dtPoint3(+d_inlet/2-l_hub0, +0.00, +h_inlet)
    ...           << dtOO.dtPoint3(+d_outHub/2+dx_hub1, +0.00, -h_hub+dz_hub1+h_inlet)
    ...           << dtOO.dtPoint3(+d_outHub/2, +0.00, -h_hub+h_inlet),
    ...         2
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+d_outHub/2, +0.00, -h_hub+h_inlet)
    ...           << dtOO.dtPoint3(+0.00, +0.00, -h_hub+h_inlet),
    ...         1
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+0.00, +0.00, -h_hub+h_inlet)
    ...           << dtOO.dtPoint3(+0.00, +0.00, -2.55),
    ...         1
    ...       ).result()
    ...     )
    ...   ] 

    Four shroud curves are defined. The first two are defined from the parameterization
    in :numref:`meas`. The third shroud curve extends vertically in the negative z-direction.
    The fourth shroud curve forms the opening of the draft tube cone.

    >>> shroudCurves = [
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+(d_inlet/2 + l_inExt), +0.00, +0.00)
    ...           << dtOO.dtPoint3(+d_inlet/2, +0.00, +0.00),
    ...         1
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+d_inlet/2, +0.00, +0.00)
    ...           << dtOO.dtPoint3(+d_inlet/2-l_shroud0, +0.00, +0.00)
    ...           << dtOO.dtPoint3(+d_outShroud/2, +0.00, -h_shroud+l_shroud1)
    ...           << dtOO.dtPoint3(+d_outShroud/2, +0.00, -h_shroud),
    ...         2
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+d_outShroud/2, +0.00, -h_shroud)
    ...           << dtOO.dtPoint3(+d_outShroud/2, +0.00, -0.54),
    ...         1
    ...       ).result()
    ...     ),
    ...     dtOO.analyticCurve(
    ...       dtOO.bSplineCurve_pointConstructOCC(
    ...         dtOO.vectorDtPoint3()
    ...           << dtOO.dtPoint3(+d_outShroud/2, +0.00, -0.54)
    ...           << dtOO.dtPoint3(+1.15, +0.00, -2.55),
    ...         1
    ...       ).result()
    ...     )
    ...   ]
    
    Create a ``generate`` object from the class `radMeridional`:
 
    >>> generate = radMeridional()
    
    The inputs of this method are the lists with the hub and shroud curves
    (``hubCurves`` and ``shroudCurves``)
    and a configuration dictionary, which specifies the positions and 
    curvatures of the interface curves.
  
    Define the configuration dictionary for the meridional contour.

    >>> configMeridional = {
    ...     "label" : "radMeridionalContour",
    ...     
    ...     "interface_hub" : [[1, 0.00],
    ...                        [1, 0.7],],
    ...     "interface_shroud" : [[1, 0.00],
    ...                           [2, 0.5],],
    ...     "interface_curvature" : [[0.0, 0.5, 1],
    ...                              [0.4, 0.5, -1],],
    ... }
 
    Create the meridional channel:
  
    >>> generate.createMeridional(configMeridional, hubCurves, shroudCurves)
    
    
    Bladed Channels
    ---------------
    
    The bladed channel geometries are created as a number of mesh blocks
    surrounding the blade geometry and a grid channel, which connects to
    the mesh blocks.
    The mesh blocks are meshed transfinite, while the grid channel is 
    meshed unstructured.

    The bladed channel is created with the method :meth:`createBlade`. 
    This method takes a configuration dictionary, which contains the necessary
    geometry parameters, as input.

    The key ``regChannel`` in the dictionary specifies in which regular channel 
    the blade is created. The number of blades can be set with ``nBlades``. 
    Based on the number of blades, a periodic segment with one blade is built.
     
    The blade geometry is defined with the parameters ``spanwiseCuts_mp``, 
    ``alpha_1``, ``alpha_2``, ``ratioX``, ``deltaY``, ``offX`` and ``offY``,
    ``spanwiseCuts_td``, ``t_le``, ``u_le``, ``t_mid``, ``u_mid``, ``t_te`` 
    and ``u_te``.
    The parameterization is further described in 
    `[Fraas_2025] <https://doi.org/10.3390/ijtpp10040038>`_.

    The guide vane blade is defined with the following configuration dictionary.
    Here, the lists of blade parameters contain only one value; this results in 
    a constant cross section of the blade along the spanwise direction.
    The parameter ``adjustRadius`` is set to ``False``.

    >>> configGuideVane = {
    ...     "label" : "gv",
    ...     "regChannel" : 0,
    ...     "nBlades" : 24,
    ...     
    ...     "spanwiseCuts_mp" : [0.00, 1.00,],
    ...     "alpha_1" : [round((np.pi/180.) * -55.0, 4)],
    ...     "alpha_2" : [round((np.pi/180.) * -16.0, 4)],
    ...     "ratioX" : [0.5],
    ...     "deltaY" : [0.12],
    ...     "offX" : [-0.046],
    ...     "offY" : [0.077],
    ... 
    ...     "spanwiseCuts_td" : [0.00, 1.00,],
    ...     "t_le" : [0.01],
    ...     "u_le" : [0.00],
    ...     "t_mid" : [0.03],
    ...     "u_mid" : [0.20],
    ...     "t_te" : [0.01],
    ...     "u_te" : [0.80],
    ... 
    ...     "adjustRadius" : False,
    ... }
    
    Create the guide vane geometry:

    >>> generate.createBlade(configGuideVane)

    The runner geometry is created with the method `createBlade`.
    The runner has 15 blades and is mapped onto the second regular channel.
    Here, the lists of blade parameters contain multiple values.  
    The parameter ``adjustRadius`` is set to ``True``.
    
    >>> configRunner = {
    ...     "label" : "ru",
    ...     "regChannel" : 1,
    ...     "nBlades" : 15,
    ...     
    ...     "spanwiseCuts_mp" : [0.00, 0.33,  0.66, 1.00,],
    ...     "alpha_1" : [
    ...              round((np.pi/180.) * 90., 4),
    ...              round((np.pi/180.) * 75., 4),
    ...              round((np.pi/180.) * 52., 4)
    ...          ],
    ...     "alpha_2" : [
    ...              round((np.pi/180.) * 45., 4),
    ...              round((np.pi/180.) * 31., 4),
    ...              round((np.pi/180.) * 32., 4),
    ...              round((np.pi/180.) * 10., 4)
    ...          ],
    ...     "ratioX" : [
    ...              0.65,
    ...              0.70,
    ...              0.35,
    ...              0.22
    ...          ],
    ...     "deltaY" : [
    ...              0.80,
    ...              0.55,
    ...              0.90,
    ...              0.55
    ...          ],
    ...     "offX" : [
    ...              0.125,
    ...              0.125,
    ...              0.0
    ...          ],
    ...     "offY" : [
    ...              0.065,
    ...              0.085,
    ...              0.035
    ...          ],
    ... 
    ...     "spanwiseCuts_td" : [0.00, 1.00,],
    ...     "t_le" : [0.020,0.018],
    ...     "u_le" : [0.00],
    ...     "t_mid" : [0.04,0.03],
    ...     "u_mid" : [0.50],
    ...     "t_te" : [0.02],
    ...     "u_te" : [0.80],
    ... 
    ...     "adjustRadius" : True,
    ... }
    
    Create the runner geometry:

    >>> generate.createBlade(configRunner)
    
    Draft Tube Cone
    ---------------

    The mesh of the draft tube cone is created as a combination of an 
    unstructured region and transfinite regions on the hub and shroud 
    walls.
    The method :meth:`createLayerRegion` is used for the generation and meshing 
    of the geometry. Geometry settings for the layer region are passed to 
    the class through a configuration dictionary.

    Create the configuration dictionary:

    >>> configLayer = {
    ...     "label" : "radMeridionalContour",
    ...     "nSlices" : 15,
    ...     "layer_thickness" : 0.2,
    ...     "layer_supports" : [0.5],
    ... }

    Create the layer region:

    >>> generate.createLayerRegion(configLayer)


    Mesh Generation
    ---------------

    The object ``container`` and the vector-handling objects for bounded 
    volumes ``bV`` and and cases ``dC`` can be returned with the getter methods 
    :meth:`getContainer` and :meth:`getbVAnddC`.
    These can be used to create the mesh files in `GMSH` or `openFOAM` cases.

    Return ``container``, ``bV`` and ``dC`` from the ``generate`` object:

    >>> container = generate.getContainer()
    >>> bV, dC = generate.getbVAnddC()

    With the following commands, the meshes of the geometries are created:

    >>> #bV["gv_mesh"].makeGrid()
    >>> #bV["ru_mesh"].makeGrid()
    >>> #bV["meshLayers"].makeGrid()

    The resulting mesh files are shown in :numref:`all`.

    """
	
    def __init__(
        self,
    ):
        """Initalize the radMeridional object. 

        This constructor:
        - Creates the main dtOO container
        - Initializes logging
        - Configures static dtOO properties
        - Sets up base container and vector handlers
        - Initializes labeled vector handling objects (bC, cV, aF, aG, bV, dC, dP)
        
        Parameters
        ----------
        None

        Returns
        -------
        None

        """ 
        # Create an empty container
        self.container = dtOO.dtBundle()

        # Create log file
        dtOO.logMe.initLog('build.log')

        # Init staticProperties
        dtOO.staticPropertiesHandler.getInstance().jInit(
          dtOO.jsonPrimitive(
            '{'
              '"option" : ['
                '{"name" : "reparamOnFace_precision", "value" : "1.e-05"},'
                '{"name" : "reparamInVolume_precision","value" : "1.e-05"},'
                '{"name" : "invY_precision", "value" : "1.e-04"},'
                '{"name" : "xyz_resolution", "value" : "1.e-05"},'
                '{"name" : "XYZ_resolution", "value" : "1.e-04"},'
                '{"name" : "uvw_resolution", "value" : "1.e-03"},'
                '{"name" : "point_render_diameter", "value" : "0.005"},'
                '{"name" : "vector_render_size", "value" : "0.05"},'
                '{"name" : "function_render_resolution_u", "value" : "50"},'
                '{"name" : "function_render_resolution_v", "value" : "50"},'
                '{"name" : "function_render_resolution_w", "value" : "50"},'
                '{"name" : "geometry_render_resolution_u", "value" : "50"},'
                '{"name" : "geometry_render_resolution_v", "value" : "50"},'
                '{"name" : "geometry_render_resolution_w", "value" : "50"},'
                '{"name" : "ompNumThreads", "value" : "2"},'
                '{"name" : "map1dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "map2dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "map3dTo3d_deltaPer", "value" : "0.01"},'
                #'{"name" : "logLevel", "value" : "99"},'
                '{"name" : "logLevel", "value" : "2"},'
                '{"name" : "isEqualExtendCheck", "value" : "true"}'
              ']'
            '}'
          )
        )

        # Init baseContainer and labeledVectorHandlings
        self.bC = self.container.cptr_bC()
        self.cV = self.container.cptr_cV()

        self.cV.push_back(
            dtOO.sliderFloatParam("QIn", 0.5, 0.0, 1.0).clone()
        )
        # clearing prevents segmentation fault
        dtOO.lVHOstateHandler.clear()
        dtOO.lVHOstateHandler( dtOO.jsonPrimitive(), self.cV ).thisown = False

        self.aF = self.container.cptr_aF()
        self.aG = self.container.cptr_aG()
        self.bV = self.container.cptr_bV()
        self.dC = self.container.cptr_dC()
        self.dP = self.container.cptr_dP()

    def createMeridional(self, configM, hubCurves, shroudCurves): 
        """Create the regular channels and special hub and shroud curves.

        This method:
        - Splits the hub and shroud curves at the interfaces.
        - Creates the interface curves.
        - Creates regular channels between the inlet and the interfaces.
        - Creates the special hub and shroud curves after the last interface.

        Parameters
        ----------
        configM : dict
          Dictionary containing the interface parameters with the
          following keys:

          - **label** (*str*):
            Label.

          - **interface_hub** (*List[Tuple[int, float]]*):
            Positions of the interfaces on the hub curves.
            Each entry represents:

              - ``interface_hub[i]``: Interface number
              - ``interface_hub[i][0]``: Curve number where the interface is located
              - ``interface_hub[i][1]``: Percentage along the curve (0 to 1)

          - **interface_shroud** (*List[Tuple[int, float]]*):
            Positions of the interfaces on the shroud curves.
            Each entry represents:

              - ``interface_shroud[i]``: Interface number
              - ``interface_shroud[i][0]``: Curve number where the interface is located
              - ``interface_shroud[i][1]``: Percentage along the curve (0 to 1)

          - **interface_curvature** (*List[Tuple[float, float, int]]*):
            Curvature of the interface curve from hub to shroud.
            Each entry represents:

              - ``interface_curvature[i]``: Interface number
              - ``interface_curvature[i][0]``: Curvature offset point [%] from hub to shroud
              - ``interface_curvature[i][1]``: Curvature as a percentage of the connection line length
              - ``interface_curvature[i][2]``: Curvature direction

        hubCurves : List[analyticGeometry]
          List of hub curves.

        shroudCurves : List[analyticGeometry]
          List of shroud curves.

        Returns
        -------
        None


        The regular channels and the special hub and shroud curves are created 
        in this method.
        The inputs to this method are lists of hub and shroud curves 
        (``hubCurves`` and ``shroudCurves``) and a configuration dictionary, 
        which specifies the positions and curvatures of the interfaces.

        The following figure shows the hub and shroud curves with their respective 
        indices in the ``hubCurves`` and ``shroudCurves`` lists.

        .. _hsCurves:
        .. figure:: meridionalFigs/hsCurve_noInterface.png
           :width: 50%
           :align: center

           Hub and shroud curves of the meridional channel.
           Numbering corresponds to the indices in the
           ``hubCurves`` and ``shroudCurves`` lists.

        In this method, the builder class `analyticGeometry_piecewiseRadMeridional` 
        is called. An object of this class is created to manage the meridional contour.
        Based on the configuration dictionary, the interface curves are constructed 
        with this object.

        Inlet and outlet interface curves are built independently of the settings in the 
        configuration dictionary as straight lines.
        Between the start points of the first hub and shroud curves in the lists, 
        an inlet curve is created.
        An outlet curve is created between the end points of the last hub and shroud 
        curves.
        
        The interface curves in the flow channel are defined through the nested lists
        in the configuration dictionary.
        The keys to these lists are ``interface_hub``, ``interface_shroud``, 
        and ``interface_curvature`` in the dictionary.

        The following figure shows the creation of the interface curves, 
        with a focus on the curvature of the second interface curve.

        .. _interfaceCalc:
        .. figure:: meridionalFigs/interfaceCalc.png
           :width: 80%
           :align: center

           Creation of the interface curves (red) between the hub and shroud curves 
           (black), with the linear mean-plane curve ``MP,lin`` (green).

        :numref:`map-fig-var_interface` gives the mapping between the mathematical symbols in
        :numref:`interfaceCalc` and the naming of the list keys in this example.

        .. _map-fig-var_interface:
        .. csv-table:: Mapping between mathematical symbols and variable names.
           :header: "Symbol", "Label"
           :widths: 10, 20
           :escape: #
           :align: center

           :math:`a`, ``interface_curvature[i][0]``
           :math:`b`, ``interface_curvature[i][1]``
           :math:`c`, ``interface_curvature[i][2]``

        The lists ``interface_hub`` and ``interface_shroud`` define the positions
        of the start and end points of the interface curves on the hub and shroud curves.

        The lists are unpacked as follows:

            ``interface_hub[i]``: Start point of the i-th interface curve on the hub.

            ``interface_shroud[i]``: End point of the i-th interface curve on the shroud.

        The lower-level lists specify the hub and shroud curves and the percentages of 
        those curves at which the start and end points of the interfaces are located:

            ``interface_hub[i][0]``: Index of the hub curve on which the start point is located.

            ``interface_hub[i][1]``: Percentage along the hub curve where the start point is located.

        The interface curves are calculated by first creating a straight line ``MP,lin`` 
        between the interface start and end points. Using the list ``interface_curvature``, 
        a control point is computed that defines the curvature of the interface curve 
        relative to ``MP,lin``.
        Finally, the interface curve is constructed from the start and end points of the 
        interface and the control point.
        
        The list ``interface_curvature`` is unpacked as follows.
        The highest level of ``interface_curvature`` corresponds to the index of the 
        interface curve:

            ``interface_curvature[i]``: Index of the interface.

        The lower level of the list defines the curvature of the interface as follows:

            ``interface_curvature[i][0]``: Control point offset as a percentage of the length of ``MP,lin``.

            ``interface_curvature[i][1]``: Control point base position as a percentage along ``MP,lin``.

            ``interface_curvature[i][2]``: Direction of the control point offset.

        When the start or end point of an interface lies on the span of a hub or shroud 
        curve, that curve is split into two curves at this point. The resulting two 
        curves then share their end or start points, respectively, with the interface.

        With the interfaces, the meridional contour is partitioned into regular channels
        for the blade geometries and special hub and shroud curves for the draft tube cone.

        The following figure shows the interfaces and the resulting regular channels,
        as well as the special curves that are not part of the regular channels.

        .. _interfaces:
        .. figure:: meridionalFigs/interfaces.png
           :width: 50%
           :align: center

           Hub and shroud curves (black), interface curves (red),
           and inlet and outlet curves (orange). The special curves
           are downstream of the last interface curve.
           The two-dimensional faces of the first regular channel (yellow)
           and the second regular channel (green).

        The first regular channel is created from the inlet curve and the first interface
        curve, as well as the hub and shroud curves extending between them.
        The subsequent regular channels are defined between the sequential interfaces and
        the hub and shroud curves between them.

        The layered region is built from the last interface curve, the outlet curve, and
        the special curves, which are not part of the regular channels.

        The object `radMeridionalContour` of the class
        `analyticGeometry_piecewiseMeridionalRotContour` is instantiated as part of the
        `radMeridional` class. Through this object, the regular channels and the special
        hub and shroud curves can be accessed in other methods.
        """
        
        #
        # Create the interface curves and the regular channels as well as the special 
        # hub and shroud curves.
        # The radMeridionalContour object is used in the other methods to return the 
        # geometries.
        #
        from dtOOPythonApp.builder import analyticGeometry_piecewiseMeridionalRotContour
        self.radMeridionalContour = analyticGeometry_piecewiseMeridionalRotContour( 
          label = configM["label"],
          hubCurves = hubCurves,
          shroudCurves = shroudCurves,
          interface_hub = configM["interface_hub"],
          interface_shroud = configM["interface_shroud"],
          interface_curvature = configM["interface_curvature"],
        ).enableDebug()#.buildExtract( self.container )
        self.container = self.radMeridionalContour.buildExtract(self.container)
    
    def createLayerRegion(self, configL):
        """Create a layered flow channel geometry.

        This method:
        - Takes the special hub and shroud curves from the ``radMeridionalContour`` object.
        - Creates transfinite faces on the hub and shroud curves.
        - Creates a multiple bounded volume inside the flow channel.
        - Applies mesh settings to the geometry regions.

        Parameters
        ----------
        configL : dict
          Dictionary containing the parameters for the layers with the
          following keys:

          - **label** (*str*):
            Label.

          - **nSlices** (*int*):
            Number of total slices. The geometry spans 360° / nSlices.

          - **layer_thickness** (*float*):
            Thickness of the layers on the hub and shroud curves.

          - **layer_supports** (*List[float]*):
            Number and positions of layer support points on each hub and shroud curve.

        Returns
        -------
        None 


        The mesh of the draft tube cone consists of six- or five-sided layer
        regions on the hub and shroud walls, as well as a multiple bounded volume
        inside the flow domain.
        The layered regions are meshed transfinite, while the multiple bounded volume
        is meshed using an unstructured mesh.

        The creation of the layers is specified using a configuration dictionary.

        The basis for the draft tube cone geometry are the special hub and shroud curves,
        as well as the inlet and outlet curves of the region.
        These curves are returned by the object of the class
        `analyticGeometry_piecewiseMeridionalRotContour`, which is created in the
        `createMeridional` method.

        :numref:`speCurves` shows the curves that are passed to the
        `createLayerRegion` method from `createMeridional`.

        .. _speCurves:
        .. figure:: meridionalFigs/speCurves.png
           :width: 50%
           :align: center

           Special hub (speHub) and special shroud (speShroud) curves (black),
           as well as the inlet (red) and outlet (orange) curves of the draft
           tube cone.

        The geometry generation is performed by the class `analyticGeometry_layerRegion`.
        In this class, the layers are created as two-dimensional surfaces and then
        rotated to generate volumetric layers.
        The multiple bounded volume is created from the bounding surfaces in the
        flow domain.

        A two-dimensional layer face is built on each hub and shroud curve
        that is not located at a radius of zero.
        Each layer face has four boundary curves. The first boundary curves
        are the hub or shroud curves themselves.

        The generation of the bounding curves of the hub layer is shown in
        the following figure.

        .. _layerGen:
        .. figure:: meridionalFigs/layerGen.png
           :width: 80%
           :align: center

           Generation of layer bounding curves at the hub (black).
           Second and third bounding curves (left, blue), third bounding
           curve (right, blue).

        The second and fourth boundary curves extend from the walls into the flow channel.
        For the first and last layer faces, at the hub and shroud, these correspond either
        to the inlet or outlet curves, or to a hub curve located at a radius of zero.

        At the intersection points of two wall curves, boundary curves extending into
        the flow channel are constructed so they point in the mean normal direction
        of both curves.
        The length of the generated curves is determined by ``layer_thickness``
        (:math:`t_{Layer}` in :numref:`layer2d` and :numref:`layerGen`)
        in the configuration dictionary.

        The third boundary curves connect the second and fourth boundary curves inside the
        flow channel. They are constructed from the end points of the second and fourth
        boundary curves and support points. These support points are calculated by
        translating points on the hub and shroud curves in the normal direction of the curves.
        The positions of these points on the hub and shroud curves are defined using the list
        ``layer_supports``.

        The following figure shows the resulting layer faces on all special hub and
        shroud curves.

        .. _layer2d:
        .. figure:: meridionalFigs/layers2d.png
           :width: 40%
           :align: center

           Two-dimensional layer faces (blue) in the draft tube cone.

        The layer volumes are created by rotating the layer faces by the angle
        resulting from the specified number of slices ``nSlices``
        (:math:`n_{Slices}` in :numref:`layers3d`) in the configuration dictionary.
        The resulting layer volumes are six-sided, except when a hub curve is
        located at a radius of zero, which results in a five-sided layer.
        The following figure shows the volume of the draft tube cone.

        .. _layers3d:
        .. figure:: meridionalFigs/layers3d.png
           :width: 50%
           :align: center

           Volumes of the draft tube cone. 
           Layer faces (blue), inlet (red), and outlet (orange).
        
        With the method `getLayerList` of the class `analyticGeometry_layerRegion`,
        a nested list containing the layer volumes and information indicating which 
        layers are five-sided is returned. 
        These lists are required to apply the mesh settings.

        With the method ``getUnstructuredRegion``, the multiple bounded volume and a list
        of its bounding surfaces can be returned.
        The bounding faces consist of the inlet and outlet areas, which are not part of 
        the layer volumes, as well as the layer faces connecting to the flow domain.

        The periodic faces are created as multiple bounded surfaces, which are bounded by
        the edges of the bounding faces and a hub curve located at a radius of zero, if 
        it exists.
        The following figure shows the bounding surfaces of the multiple bounded volume;
        the periodic faces are not shown for clarity.

        .. _mbvBounds:
        .. figure:: meridionalFigs/boundingSurfs.png
           :width: 50%
           :align: center

           Bounding faces of the multiple bounded volume with inlet (red), outlet (orange),
           and layer faces (blue).

        The mesh settings for the regions are applied using the class
        `map3dTo3dGmsh_gridFromLayers`. The edges where mesh settings are applied
        are shown in :numref:`layerMs`.

        The layers are meshed with a grading from the wall surfaces toward the connecting
        faces of the multiple bounded volume. The element size on the wall is set using
        ``firstElement``. The number of elements on the layer edges extending from the 
        wall to the multiple bounded volume (green) is set using ``nElementsLayer``.

        The maximum element size on the layer edges extending in the circumferential
        direction (blue) is set using ``elementSize_circ``.
        Similarly, the maximum element size on the edges extending along the walls
        and the edges parallel to them (pink) is set using ``elementSize_sw``.

        The mesh of the unstructured region can be controlled using the parameters
        ``charLengthMin`` and ``charLengthMax``, which define the minimum and maximum
        element sizes.

        .. _layerMS:
        .. figure:: meridionalFigs/layersMeshSetting.png
           :width: 50%
           :align: center

           Edges in the draft tube cone where mesh settings are applied, with
           circumferential edges (blue), streamwise edges (pink), and layer edges
           (green).
        
        """

        #
        # Return the special hub and shroud curves and inlet and outlet curves
        # of the layered region from the radMeridionalContour object, which was 
        # created in the createMeridional method.
        #
        speHub, speShroud, inOutCurves = self.radMeridionalContour.getLayerRegionCurves()
        
        #
        # Build the layer region geometry as transfinite layers and 
        # a multiple bounded volume
        #
        from dtOOPythonApp.builder import analyticGeometry_layerRegion
        layerRegion = analyticGeometry_layerRegion( 
          label = configL["label"],
          speHub = speHub,
          speShroud = speShroud,
          inOutCurves = inOutCurves,
          layer_thickness = configL["layer_thickness"],
          layer_supports = configL["layer_supports"],
        ).enableDebug()#.buildExtract( self.container )
        self.container = layerRegion.buildExtract(self.container)
        
        #
        # Apply mesh settings to the layer region
        #
        
        #
        # Return  the data of the layers a nested list with the following format:
        # layers = Tuple[
        #     Tuple[
        #         List[analyticGeometry], List[bool]
        #     ], 
        #     Tuple[
        #         List[analyticGeometry], List[bool]
        #     ]
        # ]
        # with:
        # layers[0] : Hub Layers
        # layers[1] : Shroud Layers
        # layers[i][0] : List of layer volumes
        # layers[i][1] : List of bool values which tell if the layer is on a radius of zero
        #
        layers = layerRegion.getLayerList(configL["nSlices"])    
        
        #
        # The volume inside the flow channels is retrned as the mutliple bounded volume mv
        # its bounding surfaces are returned in the list bs
        #
        mv, bs = layerRegion.getUnstructuredRegion(configL["nSlices"])
        
        from dtOOPythonApp.builder import ( map3dTo3dGmsh_gridFromLayers ) 
        
        # 
        # Apply the mesh settings to the layers and the multiple bounded volume
        #
        self.container = map3dTo3dGmsh_gridFromLayers(
                mv = mv,
                bs = bs,
                label = "meshLayers",
                layers = layers,
                nElementsLayer = 15,
                firstElement = 0.001,
                elementSize_sw = 0.02,
                elementSize_circ = 0.01,
                charLengthMin = 0.02,
                charLengthMax = 0.04,
            ).buildExtract(self.container)
        #self.container = map3dTo3dGmsh_gridFromLayers(
        #        mv = mv,
        #        bs = bs,
        #        label = "meshLayers",
        #        layers = layers,
        #        nElementsLayer = 15,
        #        firstElement = 0.001,
        #        elementSize_sw = 0.08,
        #        elementSize_circ = 0.06,
        #        charLengthMin = 0.05,
        #        charLengthMax = 0.1,
        #    ).buildExtract(self.container)
       


    def createBlade(self, configB):
        
        """Create blade geometry and the grid channel of the blade.

        This method:

        - Creates the blade in the parameter space by combining the mean plane
          with the thickness distribution.
        - Maps the blade onto a regular channel returned by the
          `radMeridionalContour` object.
        - Creates six-sided mesh blocks surrounding the blade.
        - Creates six-sided mesh blocks at the blade’s trailing edge.
        - Creates mean planes extending from the mesh blocks toward the
          inlet and outlet of the channel.
        - Creates the grid channel from the mesh blocks and a multiple
          bounded volume defined by the mean plane and mesh block faces.
        - Applies mesh settings to the geometry regions.

        The inputs are provided via a configuration dictionary.
        The entries of type ``List[float]`` in this dictionary define the blade
        parameters along the blade span from hub to shroud.

        Parameters
        ----------
        configB : dict
          Dictionary containing the parameters for the layers with the
          following keys:

          - **label** (*str*): 
            Label.

          - **regChannel** (*int*): 
            Number of the regular channel in which the blade is created.

          - **nBlades** (*int*): 
            Number of total blades. Geometry will be 360°/nBlades.

          - **spanwiseCuts_mp** (*List[float]*): 
            Percentages of spanwise cuts where the blade's meanplane is created.

          - **alpha_1** (*List[float]*): 
            Blade inlet angles along spanwise direction from hub to shroud.

          - **alpha_2** (*List[float]*): 
            Blade outlet angles along spanwise direction from hub to shroud.

          - **ratioX** (*List[float]*): 
            Ratio between expanse of the blades' inlet and outlet angles for the x-direction along blade span.

          - **deltaY** (*List[float]*): 
            Blade length in y direction along blade span.

          - **offX** (*List[float]*): 
            Blade offsets in x-direction along blade span.

          - **offY** (*List[float]*): 
            Blade offsets in y-direction along blade span.

          - **spanwiseCuts_td** (*List[float]*): 
            Percentages of spanwise cuts where the blades' thickness distribution is created.

          - **t_le** (*List[float]*): 
            Thicknesses at the blades' leading edge along the blades' span.

          - **u_le** (*List[float]*): 
            Percentage of the u-positions of the leading edge thicknesses along the blades' span.

          - **t_mid** (*List[float]*): 
            Thicknesses at the blades' middle along the blades' span.

          - **u_mid** (*List[float]*): 
            Percentage of the u-positions of the thicknesses in the middle along the blades' span.

          - **t_te** (*List[float]*): 
            Thicknesses at the blades' trailing edge along the blades' span.

          - **u_te** (*List[float]*): 
            Percentage of the u-positions of the trailing edge thicknesses along the blades' span.

          - **adjustRadius** (*bool*): 
            Enables adjusting the blades curvature along the channel radius.
        
        Returns
        -------
        None

        
        This method creates a bladed channel and applies mesh settings to it.
        The channel consists of six-sided mesh blocks which surround the blade geometry
        and a grid channel formed by a multiple bounded volume, connecting to the mesh 
        blocks.

        The blade geometry is defined using a configuration dictionary. The key ``label`` 
        is used as an identifier for the bladed channel. The main geometries created in 
        this method receive the string value stored under ``label`` in their physical 
        names.

        The key ``regChannel`` in the dictionary specifies in which regular channel the 
        blade is created. The regular channel is returned by the object of the class
        `analyticGeometry_piecewiseMeridionalRotContour`, which is created in the 
        method `createMeridional`.

        The mean plane is defined using the parameters ``spanwiseCuts_mp``, ``alpha_1``,
        ``alpha_2``, ``ratioX``, ``deltaY``, ``offX``, and ``offY``.
        The class `analyticSurface_threePointMeanplaneFromRatio` creates the mean plane
        in the uvw-parameter space.

        The thickness distribution is defined using the parameters ``spanwiseCuts_td``,
        ``t_le``, ``u_le``, ``t_mid``, ``u_mid``, ``t_te``, and ``u_te``.
        The class `vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution` defines
        the thickness distribution.
        The parametrization of the blade geometry is further described in
        `[Fraas_2025] <https://doi.org/10.3390/ijtpp10040038>`_.

        The parameters of the mean plane and the thickness distribution are stored in 
        lists. The list entries define the blade parameters along the blade span from hub
        to shroud.

        The creation of the mean plane and the thickness distribution is performed using
        `scaOneD_scaCurve2dOneDPointConstruct` objects, which define a functional 
        relation of the parameters along the spanwise direction.
        The constructor of a `scaOneD_scaCurve2dOneDPointConstruct` object requires a 
        list of `dtPoint2` objects as well as the function order as inputs.

        To create these inputs, the lists are passed to the method :meth:`fillInputList`.
        This method creates a list of `dtPoint2` objects, which in turn contain the 
        parameter values from the list and the percentage of the blade span at which 
        those values are applied. Furthermore, the function order is set in this method.

        The blade is constructed by combining the mean plane contour and the thickness 
        distribution using the `dtOO` class `discreteAddNormal`.
        The resulting blade is represented as a surface in the uvw-parameter space.

        The mesh blocks are created by generating a mesh block surface that surrounds the
        blade at a normal distance of ``meshBlock_thickness`` from its surface.
        This is done similarly to the combination of the mean plane and thickness using 
        the `dtOO` class `discreteAddNormal`.

        The class `vec3dThreeD_skinAndSplit` is used for multiple operations, as follows:

        - Creation of mesh block volumes from the blade and mesh block surfaces.
        - Creation of trailing edge mesh blocks.
        - Creation of mean plane curves.

        The mesh block volumes are created by splitting the blade and mesh block surfaces
        along their u-direction and skinning the resulting faces with each other.
        According to the number of splits and their positions, specified by the input 
        variable ``splits``, multiple mesh blocks are created. The volumes contain the 
        string ``meshBlock`` as part of their label.

        The sequence of mesh block numbering follows the direction of the u-parameter
        of the blade surface, starting with the first mesh block at u = 0% and ending 
        with the last mesh block at u = 100%.

        The first mesh block is labeled with the suffix ``_1`` and the n-th one with the
        suffix ``_n``.
        The split dimension can be changed from u by modifying the input variable 
        ``splitDim``.

        The following figure shows a blade surrounded by the mesh block volumes.

        .. _meshBlock:
        .. figure:: bladeFigs/guideVane_meshBlocks.png
           :width: 50%
           :align: center

           Blade surface (grey), surrounded by mesh blocks.

        In addition to the mesh blocks created on the blade surface, two mesh blocks
        in the downstream direction at the blade’s trailing edge are created.
        The trailing edge mesh blocks are generated by creating curves, which are 
        tangentially offset from the blade surface and the edges of the mesh blocks.
        These curves are skinned with their originating curves to create surfaces.
        By skinning the surfaces with each other the trailing edge mesh block volumes 
        are created.
        The thickness of the trailing edge mesh blocks is set using the input variable
        ``tEMeshBlockThickness``.

        The trailing edge mesh blocks are labeled ``meshBlock``. The block connecting
        to the first trailing edge mesh block receives the suffix ``_0``, and the block
        connecting to the last mesh block receives the suffix ``_n+1``.

        The following figure shows the trailing edge mesh blocks connecting to the mesh 
        blocks.

        .. _TEmeshBlock:
        .. figure:: bladeFigs/guideVane_TEmeshBlocks.png
           :width: 50%
           :align: center

           Blade surface (grey) with trailing edge mesh blocks.

        The periodic faces of the grid channel are created from the specified faces
        of the mesh blocks and four mean plane faces.
        The number of mesh block faces that are part of the periodic faces is defined
        by ``nMeanplaneBlocks``.
        
        Two of the mean plane faces are created by calculating offset curves extending
        in the tangential direction of the mesh blocks and skinning them with the corresponding
        mesh block edges.

        The first of these two faces extends from the first trailing edge mesh block into
        the downstream direction of the channel. The other face extends from the end of the
        mesh block specified by ``nMeanplaneBlocks`` in the w-direction.
        The extension length of the faces can be set using ``meanplaneExtOut`` and
        ``meanplaneExtIn``.

        The skinning is performed so the u- and v-directions of the faces match the
        directions of the mesh block faces.
        The label of the resulting face at the trailing edge mesh block receives the suffix
        ``_out0``, while the one at the mesh block specified by ``nMeanplaneBlocks`` receives
        the suffix ``_in0``.
        The creation of these faces is shown in :numref:`feMean0`.

        .. _feMean0:
        .. figure:: bladeFigs/guideVane_feMeanplane0.png
           :width: 100%
           :align: center

           Creation of the mean plane faces with name suffixes ``_out0`` and ``_in0``.

        All geometries generated up to this point are defined in the uvw-parameter space.
        By performing a conformal mapping inside the regular channel, the geometries are
        reparameterized into xyz-coordinates. The mapping is performed using the `dtOO` class
        `uVw_phirMs`.

        The Boolean value associated with the key ``adjustRadius`` in the configuration
        dictionary controls whether radial shifts during the mapping of the blade in the
        radial channel are activated or deactivated.

        The other two mean plane faces extend from the tangentially offset curves of the
        connecting mean plane faces to the inlet or outlet faces of the regular channel.
        Their labels receive the suffixes ``_out1`` and ``_in1``. Their orientations are also
        consistent with those of the other faces.

        The following figure shows the four mean plane faces with their label suffixes.

        .. _feMean:
        .. figure:: bladeFigs/guideVane_feMeanplane.png
           :width: 100%
           :align: center

           Mean plane faces (yellow).

        Next, the mean plane faces and the mesh block faces are organized into different lists.
        The list ``meanplaneFaces`` contains the faces that will later form the periodic faces of
        the bladed channel.
        These include the mean plane faces (:numref:`gridChannel`, yellow) and the mesh block
        faces specified by ``nMeanplaneBlocks`` (:numref:`gridChannel`, green).

        The remaining mesh block faces connect the mesh block regions to the multiple bounded
        volume. These are stored in the list ``couplingFaces`` (:numref:`gridChannel`, cyan).

        Using the lists ``meanplaneFaces`` and ``couplingFaces``, the grid channel is created as
        a multiple bounded volume in the class `multipleBoundedVolume_gridChannel`.
        The class returns the multiple bounded volume of the grid channel and a list containing 
        its bounding surfaces.
        :numref:`gridChannel` shows the bounding faces of the grid channel and the mesh blocks
        surrounding the blade; the hub and shroud faces are excluded for clarity.

        The grid channel is bounded by the coupling faces (cyan) and the mean plane faces,
        which are created as extensions of the mesh blocks (:numref:`feMean`, yellow) on one side
        of the grid channel. On the other side, it is bounded by the rotated faces in
        ``meanplaneFaces`` (yellow, green). The inlet and outlet faces (red) are created by 
        rotating the edges of the mean plane faces that connect to the inlet and outlet of the 
        regular channels.

        The hub and shroud faces of the grid channel are multiple bounded surfaces, defined by
        the edges of the boundary faces connecting to them.
        The rotation angle of the faces is determined by the number of blades ``nBlades``
        specified in the configuration dictionary.

        .. _gridChannel:
        .. figure:: bladeFigs/guideVane_channel.png
           :width: 100%
           :align: center

           Grid channel with blade (grey), inlet and outlet (red),
           periodic faces (yellow, green), and coupling faces (cyan).

        The mesh settings for the grid channel and the mesh blocks are applied using the class
        `map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks`.
        The edges where mesh settings are applied to the channel are shown in
        :numref:`channelMeshing`.

        The volume of the grid channel is meshed unstructured, with transfinite layers on the
        hub and shroud surfaces. The number of layers can be set using ``nBoundaryLayers``.
        The element size of the unstructured regions can be controlled using
        ``charLengthMax`` and ``charLengthMin``.

        The number of elements in the spanwise direction (``nElementsSpanwise``) of the channel
        is applied to the edges extending from the hub to the shroud (orange).
        These edges are meshed with a grading, with an element size of
        ``firstElementSizeHubToShroud`` at the hub and shroud surfaces.

        The mesh blocks surrounding the blade are meshed transfinite.
        The edges extending from the blade to the mesh block faces (green) are meshed with the
        number of elements specified by ``nElementsNormal`` and a grading with an element size
        of ``firstElementSizeNormalBlade`` at the blade surface.

        The mesh size along the blade edge (blue) is set using ``bladeHubElementSize``.
        This input is an object of the class `scaOneD_scaCurve2dOneDPointConstruct` and
        defines the mesh size distribution along the blade surface. 
        The scale can be adjusted using ``bladeHubElementScale``.

        .. _channelMeshing:
        .. figure:: bladeFigs/guideVane_channelMeshing.png
           :width: 100%
           :align: center

           Edges of the bladed channel where mesh settings are applied:
           hub-to-shroud edges (orange), blade edges (blue), blade-to-block edges (green),
           and trailing edge lines (pink).
        """

        #
        # Assign the entries of the configuration dictionary to variables.
        #
        label = configB["label"]
        regChannel = configB["regChannel"]
        nBlades = configB["nBlades"]

        spanwiseCuts_mp = configB["spanwiseCuts_mp"]
        alpha_1 = configB["alpha_1"]
        alpha_2 = configB["alpha_2"]
        ratioX = configB["ratioX"]
        deltaY = configB["deltaY"]
        offX = configB["offX"]
        offY = configB["offY"]

        spanwiseCuts_td = configB["spanwiseCuts_td"]
        t_le = configB["t_le"]
        u_le = configB["u_le"]
        t_mid = configB["t_mid"]
        u_mid = configB["u_mid"]
        t_te = configB["t_te"]
        u_te = configB["u_te"]
        
        adjustRadius = configB["adjustRadius"]
        
        #
        # Return the regular channel from the radMeridionalContour object,
        # which is created in the method createMeridional.
        #
        self.aG.push_back(
            self.radMeridionalContour.getRegChannel(
                regChannel, 1
            ) << "xyz_"+label+"_channel"
        )

        #
        # Create the blades' meanplane in the parameter space.
        #
        # The method fillInputList transforms the lists form the configuration
        # dictionary into a valid input for the class constructor
        # scaOneD_scaCurve2dOneDPointConstruct.
        #
        from dtOOPythonApp.builder import (
            analyticSurface_threePointMeanplaneFromRatio,
            scaOneD_scaCurve2dOneDPointConstruct
        )
        self.container = analyticSurface_threePointMeanplaneFromRatio(
          label + "_meanplane",
          spanwiseCuts_mp,
          alphaOne = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(alpha_1)
          )(),
          alphaTwo = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(alpha_2)
          )(),
          ratioX = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(ratioX)
          )(),
          deltaY = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(deltaY)
          )(),
          offX = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(offX)
          )(),
          offY = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(offY)
          )(),
        ).buildExtract( self.container )
        
        #
        # Create the blades' thickness distribution in the parameters space.
        #
        from dtOOPythonApp.builder import (
          vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution
        )
        self.container = vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution(
          label + "_thicknessDistribution",
          spanwiseCuts_td,
          tLe = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(t_le)
          )(),
          uLe = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(u_le)
          )(),
          tMid = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(t_mid)
          )(),
          uMid = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(u_mid)
          )(),
          tTe = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(t_te)
          )(),
          uTe = scaOneD_scaCurve2dOneDPointConstruct(
            *self.fillInputList(u_te)
          )()
        ).buildExtract( self.container )

        #
        # Combine the meanplane and the thickness distribution in the parameter space.
        #
        dAdd = dtOO.discreteAddNormal()
        dAdd.jInit(
          dtOO.jsonPrimitive(
            '{"option" : [{"name" : "debug", "value" : "false"}]}'
          )\
            .appendAnalyticFunction("_tt", self.aF[label + "_thicknessDistribution"])\
            .appendInt("_nU", 61)\
            .appendInt("_nV", 41)\
            .appendInt("_order", 3)\
            .appendDtVector3("_nf", dtOO.dtVector3(0,0,1)),
          None, None, self.aF, None
        )
        theAF = dAdd.applyAnalyticFunction(
          self.aF[label + "_meanplane"]
        )
        theAF.setLabel(label + "_blade")
        self.aF.push_back( theAF.clone() )
        
        #
        # Create a conformal mapping object with the regular channel
        # and the adjustRadius setting.
        #
        conMap = dtOO.uVw_phirMs()
        conMap.jInit(
          dtOO.jsonPrimitive()\
            .appendStr("label", "uVw_phirMs")\
            .appendInt("_nV", 31)\
            .appendInt("_nW", 11)\
            .appendBool("_adjustRadius", adjustRadius)\
            .appendAnalyticGeometry(\
              "_rM2d", \
              self.aG.get("xyz_" + label + "_channel")\
            ),\
          None, None, None, self.aG \
        )
        self.bC.dtTransformer().add( conMap.clone() )
         
        ##
        ## depiction in 3d Space of blade and meanplane
        ##
        #for ii in [label+"_meanplane", label+"_blade",]:         
        #  theAG = dtOO.vec3dTwoDInMap3dTo3d(
        #    dtOO.vec3dTwoD.MustConstDownCast(
        #      conMap.applyAnalyticFunction(self.aF[ii].clone())
        #    ),
        #    dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )   
        #  )
        #  self.aG.push_back( theAG << "xyz_"+str(ii) )
        
        #
        # Create a surface around the blade surface with a normal distance
        # specified by meshBlock_thickness.
        #
        meshBlock_thickness = 0.020 
        fRef = dtOO.vec3dMuParserTwoD(
          "1.0*"+str(meshBlock_thickness)+", xx, yy", "xx", "yy"
        )
        fRef.setLabel(label + "_thicknessMeshBlock")
        for i in range(2):
          fRef.setMin(i, +0.0)
          fRef.setMax(i, +1.0)
        self.aF.set( fRef.clone() )
        dAdd = dtOO.discreteAddNormal()
        dAdd.jInit(
          dtOO.jsonPrimitive(
            '{"option" : [{"name" : "debug", "value" : "false"}]}'
            )\
            .appendAnalyticFunction("_tt", self.aF[label+"_thicknessMeshBlock"])\
            .appendInt("_nU", 61)\
            .appendInt("_nV", 41)\
            .appendInt("_order", 3)\
            .appendDtVector3("_nf", dtOO.dtVector3(0,0,1)),
          None, None, self.aF, None
        )
        theAF = dAdd.applyAnalyticFunction( self.aF[label+"_blade"] )
        theAF.setLabel(label+"_meshBlock")
        self.aF.push_back( theAF.clone() )
        
        #
        # The class vec3dThreeD_skinAndSplit does the following tasks:
        #   - Create mesh blocks by splitting the mesh black surface.
        #   - Create trailing edge mesh blocks based on the first and last
        #     mesh block.
        #   - Creates a meanplane curve by offseting mesh block edges in the
        #     tangential directions of the mesh blocks.
        #     These curves will later be used to create peridoc faces of the
        #     grid channel.
        #     Two meanplane curves are created, one offset from the first mesh
        #     block extending towards the outlet and one offset from the meshblock
        #     specified with nMeanplaneBlocks extending towards the inlet of the
        #     regular channel.
        #

        # number of mesh block faces which will be part of the meanplane
        #  starts from 0
        nMeanplaneBlocks = 3

        # split mesh block and create curves for meanplane
        from dtOOPythonApp.builder import vec3dThreeD_skinAndSplit
        self.container = vec3dThreeD_skinAndSplit(
          label =label+"_meshBlock",
          aFOne = self.aF[label+"_blade"],
          aFTwo = self.aF[label+"_meshBlock"],
          splitDim = 0,
          splits = [
            [0.00, 0.10],
            [0.10, 0.30],
            [0.30, 0.45],
            [0.45, 0.55],
            [0.55, 0.70],
            [0.70, 0.90],
            [0.90, 1.00],
          ],
          tEMeshBlockThickness = meshBlock_thickness,
          meanplaneFromBlocks = True,
          meanplaneExtOut = 0.02,
          meanplaneExtIn = 0.03,
          nMeanplaneBlocks = nMeanplaneBlocks
        ).buildExtract(self.container)
        
        #
        # Build the two meanplane faces extending from the mesh block edges 
        # to the tangentially offset meanplane curves.
        #
        for i in range(2):
            #
            # curve seqence is switched with c0 and c1 between inlet and outlet in 
            # order to keep u and v directions consistent with the mesh block faces.
            #
            if i == 0:
                at = "in"
                c0 = 0
                c1 = 1
            elif i == 1:
                at = "out"
                c0 = 1
                c1 = 0
            # 
            # Create the fe_meanplane faces. Those will later be used as part of the
            # periodic faces of the grid channel.
            #
            surf = dtOO.vec3dSurfaceTwoD(
                dtOO.bSplineSurface_exchangeSurfaceConstructOCC(
                    dtOO.bSplineSurface_skinConstructOCC(
                        dtOO.vec3dCurveOneD.MustDownCast(
                            self.aF[label+"_meshBlockCurve_"+at+str(c0)]
                        ).ptrConstDtCurve(),
                        dtOO.vec3dCurveOneD.MustDownCast(
                            self.aF[label+"_meshBlockCurve_"+at+str(c1)]
                        ).ptrConstDtCurve()
                    ).result()
                ).result()
            )
            self.aF.push_back(surf << label+"_fe_meanplane_"+at+str(0))
 
        #
        # Apply the conformal mapping object the following geometries.
        #
        #   - faces of the blade meanplane, the blade and the mesh blocks
        #
        for ii in [label+"_meanplane", label+"_blade", label+"_meshBlock",]:
          theAG = dtOO.vec3dTwoDInMap3dTo3d(
            dtOO.vec3dTwoD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
        #
        #   - fe_meanplane faces extending tangentially from the mesh blocks.
        #
        for iNum in self.aF.getIndices(label+"_fe_meanplane_*"):
          ii = self.aF.getLabel( iNum )
          theAG = dtOO.vec3dTwoDInMap3dTo3d(
            dtOO.vec3dTwoD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
        #
        #   - mesh block volumes
        #
        for iNum in self.aF.getIndices(label+"_meshBlock_*"):
          ii = self.aF.getLabel( iNum )
          theAG = dtOO.vec3dThreeDInMap3dTo3d(
            dtOO.vec3dThreeD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
        #
        #   - mesh block curves which were used to create the fe_meanplane faces
        #
        for iNum in self.aF.getIndices(label+"_meshBlockCurve_*"):
          ii = self.aF.getLabel( iNum )
          theAG = dtOO.vec3dOneDInMap3dTo3d(
            dtOO.vec3dOneD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
        
        #
        # Collect the curves for the fe_meanplane faces.
        # The curves extend form hub to shroud and are tangentially 
        # offset from the specified mesh block faces
        #
        meshBlockCurves = dtOO.labeledVectorHandlingAnalyticGeometry()
        for iNum in self.aG.getIndices("xyz_"+label+"_meshBlockCurve_*"):
            ii = self.aG.getLabel( iNum )
            meshBlockCurves.push_back(self.aG[ii].clone())
        
        #
        # Create the fe_meanplane surfaces extending from the tangentially 
        # offset meshblock curves to the inlet or the outlet.
        #
        from dtOOPythonApp.builder import analyticSurface_inOutFeMeanplane
        self.container = analyticSurface_inOutFeMeanplane(
            prefix = "xyz",
            label = label,
            channel = self.aG["xyz_"+label+"_channel"],
            curves = meshBlockCurves
        ).enableDebug().buildExtract(self.container)
         
        #
        # Order the gemetries for the cration of the grid channel.
        #
        # Collect and organize the mesh block volumes.
        # The mesh blocks are ordered by their number.
        #
        blocks = []
        for iNum in self.aG.getIndices("xyz_"+label+"_meshBlock_*"):
            blocks.append( self.aG[ self.aG.getLabel( iNum ) ] )
        blocks.sort(key=lambda x: int(x.getLabel().split('_')[-1]))
        
        #
        # Collect the coupling and meanplane faces.
        # The list couplingFaces contains the faces which will connect the
        # mesh block to the grid channel.
        # The list meanplaneFaces contains the faces which will form the 
        # periodic boundaries of the grid.
        #
        couplingFaces = []
        meanplaneFaces = []
        
        #
        # Faces extending from outlet to the mesh block trailing edge. 
        #
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_out1"]))
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_out0"]))
        
        #
        # Iterate over the mesh blocks and sort the faces.
        #
        for i, block in enumerate(blocks):
          face = dtOO.map3dTo3d.MustDownCast( block ).segmentConstWPercent( 1.0 )
          
          #
          # Append the specified meanplane and coupling to the lists. 
          #
          if i <= nMeanplaneBlocks:
            meanplaneFaces.append(face)
            self.aG.push_back(face.clone() << "xyz_"+label+"_meanplaneFaceOnBlock_"+str(i))
          else:
            couplingFaces.append(face)
        
        #
        # Faces extending from the mesh block leading edge area to the inlet.
        #
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_in0"]))
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_in1"]))
        
        #
        # Last two coupling faces at the trailing edge.
        #
        couplingFaces.append(dtOO.map3dTo3d.MustDownCast(blocks[-1]).segmentConstUPercent( 1.0 ))
        couplingFaces.append(dtOO.map3dTo3d.MustDownCast(blocks[0]).segmentConstUPercent( 0.0 ))
        
        #
        # Create the grid channel.
        #
        from dtOOPythonApp.builder import multipleBoundedVolume_gridChannel
        gridChannel = multipleBoundedVolume_gridChannel(
            label = label,
            channel = self.aG["xyz_"+label+"_channel"],
            meanplanes = meanplaneFaces,
            couplings = couplingFaces,
            nBlades = nBlades
        ).enableDebug()
        self.container = gridChannel.buildExtract(self.container)
        #
        # Return the multiple bounded volume of the grid channel as well
        # as the list with its boundary faces.
        #
        gc, gcFaces = gridChannel.getGridChannel()
        self.aG.push_back(
            gc << "xyz_"+label+"_gridChannel"
        )
        
        #
        # Apply the mesh settings to the grid channel.
        #
        from dtOOPythonApp.builder import (
          map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks
        )
        self.container = map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks(
            label = label+"_mesh",
            channel = self.aG["xyz_"+label+"_gridChannel"],
            channelFaces = gcFaces,
            blocks = blocks,
            nMeanplaneBlocks = nMeanplaneBlocks,
            blade = self.aG["xyz_"+label+"_blade"],
            nBoundaryLayers = 15,
            nElementsSpanwise = 50,
            nElementsNormal = 6,
            firstElementSizeHubToShroud = 0.001,
            firstElementSizeNormalBlade = 0.001,
            bladeHubElementSize = scaOneD_scaCurve2dOneDPointConstruct(
                [
                   dtOO.dtPoint2(0.00, 0.015),
                   dtOO.dtPoint2(0.45, 0.005),
                   dtOO.dtPoint2(0.50, 0.004),
                   dtOO.dtPoint2(0.55, 0.005),
                   dtOO.dtPoint2(1.00, 0.015),
                ], 1
            )(),
            bladeHubElementScale = 0.3,
            charLengthMax=0.015,
            charLengthMin=0.015,
            meshTEBlocks = True,
        ).enableDebug().buildExtract( self.container )
        #self.container = map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks(
        #    label = label+"_mesh",
        #    channel = self.aG["xyz_"+label+"_gridChannel"],
        #    channelFaces = gcFaces,
        #    blocks = blocks,
        #    nMeanplaneBlocks = nMeanplaneBlocks,
        #    blade = self.aG["xyz_"+label+"_blade"],
        #    nBoundaryLayers = 15,
        #    nElementsSpanwise = 50,
        #    nElementsNormal = 7,
        #    firstElementSizeHubToShroud = 0.001,
        #    firstElementSizeNormalBlade = 0.001,
        #    bladeHubElementSize = scaOneD_scaCurve2dOneDPointConstruct(
        #        [
        #           dtOO.dtPoint2(0.00, 0.04),
        #           dtOO.dtPoint2(0.45, 0.007),
        #           dtOO.dtPoint2(0.50, 0.007),
        #           dtOO.dtPoint2(0.55, 0.007),
        #           dtOO.dtPoint2(1.00, 0.04),
        #        ], 1
        #    )(),
        #    bladeHubElementScale = 0.3,
        #    charLengthMax=0.1,
        #    charLengthMin=0.04,
        #    meshTEBlocks = True,
        #).enableDebug().buildExtract( self.container )
    
    #
    # returns a list with dtPoint2 types and spline orders
    #  with spanwise cut percentage and blade input parameters
    #
    def fillInputList(self, inList): 
        """Create inputs for scaOneD_scaCurve2dOneDPointConstruct class.

        Takes the input lists for the :meth:`createBlade` method and transforms them
        into inputs of the scaOneD_scaCurve2dOneDPointConstruct class.

        Creates a list with dtPoint2 objects which contain the input values from
        the input list and the spanwise percentage on the blade where this value 
        is applied. The percentage is calculated from the length of the input
        list with a normalized index.
        Based on the list length the order of the scaOneD_scaCurve2dOneDPointConstruct
        function is calculated.

        If the input list has only one parameter the outList is written
        so this value will be constant along the blades' span.
        

        Parameters
        ----------
        inList : List[float]
          List containing the input values along the blades' span.

        Returns
        -------
        outList : List[dtPoint2[float, float]]
          Input parameter and its percenatage along the blades' span.
          
        order : int
          Order of the function.

        """
        #
        # If the input list has only one entrie the values are kept 
        # constant along the spanwise direction.
        #
        if len(inList) == 1:
            #
            # The inputs will be the same at spanwiseCut 0 and 1.
            # The odere is one.
            #
            outList = [
                        dtOO.dtPoint2(0.00, inList[0]), 
                        dtOO.dtPoint2(1.00, inList[0]),
                      ]      
            order = 1
        
        #
        # Else the parameter vary along the spanwise direction.
        #
        else:
            outList = []
            #
            # Iterate over the input list.
            #
            for nL in range(len(inList)):
                #
                # Calculate the percentage of the spanwise cut from the normalized index
                #
                cutPercent = nL/(len(inList)-1)
                #
                # Append the percentage and input as a dtPoint2 object to the outList.
                #
                outList.append(dtOO.dtPoint2(cutPercent, inList[nL]),)
            
            #
            # The order is one less than the length of the input list.
            #
            order = len(inList) -1
        
        return outList, order
    
    def getContainer(self):
        """Return the container object.
        
        Is used to create openFOAM cases.

        Parameters
        ----------
        None

        Returns
        -------
        container: dtBundle
          Initialization of the dtBundle.
        """
        return self.container

    def getbVAnddC(self):
        """Return the bV and dC object.
        
        Are used to create the meshes and openFoam cases.

        Parameters
        ----------
        None

        Returns
        -------
        bV: lvH_boundedVolume
          Labeled vector handling of bounded volumes.
        dC: lvh_dtCase
          Labeled vector handling of cases.
        """
        return self.bV, self.dC

