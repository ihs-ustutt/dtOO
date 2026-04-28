"""
This class is the main class for the generation and meshing of geometries of a
radial flow machine. 
It contains the following creation methods:

    1. `createMeridional`
    2. `createBlade`
    3. `createLayerRegion`

The creation methods are reponsible for the generation of the different geometries.
They in turn call multiple builder classes which perform the specific tasks for
the geometry generation.
The builder classes are located in the following path:

    `./dtOO/scripts/python/dtOOPythonApp/builder`

The meridional contour of the whole machine is defined by hub and shroud curves.
The method `createMeridional` separates those through defined interfaces, 
into regular channels and special curves.
The following builder classes are used:

    - `analyticGeometry_piecewiseMeridionalRotContour`

Bladed channels (i.e. runner and guide vane channels) can be created with 
the `createBlade` method. The defined blades are created inside the regular 
channels which are handed over to the method from `createMeridional`. 
The resulting geometry and mesh is a periodic segment of the blade channel.
The blade is surrounded by transfinite and structured mesh blocks, the rest 
of the channel is meshed unstructured.     
The following builder classes are used:

    - `analyticSurface_threePointMeanplaneFromRatio`
    - `scaOneD_scaCurve2dOneDPointConstruct`
    - `vec3dSurfaceTwoD_fivePointsBSplineThicknessDistribution`
    - `vec3dThreeD_skinAndSplit`
    - `analyticSurface_inOutFeMeanplane`
    - `multipleBoundedVolume_gridChannel`
    - `map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks`

The `createLayerRegion` method takes the special hub and shroud curves and
creates a segment of a flow channel. This flow channel consists of tranfinite 
layer regions on the hub and shroud faces and an unstructured region in between.
The following builder classes are used:

    - `analyticGeometry_layerRegion`
    - `map3dTo3dGmsh_gridFromLayers`

WORKFLOW DESCRIPTION AND CLASS DIAGRAMM
"""

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
    """Create the geometries and their meshes of a radial flow machine.
     
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
      Object which creates the meridional contour.

    Examples
    --------
    >>> import dtOOPythonSWIG as dtOO
    
    This example creates geometries and meshes of a parametrized radial machine.
    The machine consists of the following geometries:

	- Guide Vane Channel
	- Runner Channel
	- Draft Tube Cone
    
    The following figure shows the resulting meshes of the different geometries:

     
    .. _all:
    .. figure:: bladeFigs/allGeoms.png
       :width: 100%
       :align: center

       Final geometries and meshes of this example, with guid vane (top right),
       runner (top left) and draft tube cone (bottom).
    
    The meridional contour of the machine is defined with the method `createMeridional`
    through hub and shroud curves as well as the interface definition. 
    
    The guide vane and runner channels are created from regular channels with 
    the method `createBlade`.
    
    The draft tube cone is created from the special curves with the method
    `createLayerRegion`.
    

    Meridional Contour
    ------------------
    
    The meridional contour of the flow machine is created through its' hub and shroud curves.

    The hub and shroud curves are defined in separate lists named ``hubCurves`` and
    ``shroudCurves``. The curves have to be created so their direction is the same as
    the downstream direction of the flow machine. Furthermore the sequence of the curves
    in the lists must also correspond to the downstream direction of the machine.

    The following figure shows the hub and shroud curves with their respective numbers
    in the ``hubCurves`` and ``shroudCurves`` lists.

    .. _hsCurves:
    .. figure:: meridionalFigs/hsCurve_noInterface.png
       :width: 50%
       :align: center

       Hub and shroud curves of the meridional channel.
       Numbering correlates to the numbering in the 
       ``hubCurves`` and ``shroudCurves`` lists.
    
    In this example the first two of the hub and the shroud curves will form the 
    regluar channels. 
    The first curves build the regular channel for the guide vane and the second 
    curves build the regular channel for the runner. 
    Because the runner channel of a radial turbine is highly curved a parametrization 
    of those curves is introduced, which enables the generation of different channel 
    geometries.

    The parametrization is shown in the following figure.

    .. _meas:
    .. figure:: meridionalFigs/meas.png
       :width: 100%
       :align: center

       Measurements of the first two defined hub and shroud curves, 
       which will build the guide vane and runner channels.

    :numref:`map-fig-var` gives the mapping between math symbols in :numref:`meas`
    and the naming of the variables in this example.
    
    .. _map-fig-var:
    .. csv-table:: Mapping between math symbol and variable names.
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
   
    The contour of the runners meridional channel has a flow deflection from the radial inlet 
    to the axial outlet. In order to achieve this, the meridional 
    hub and shroud curves of the runner channel are defined from four control points. 
    
    The first and forth control points form the start and the end points of the curves.
    Their positions are parametrized through the diameters :math:`d_{inlet}`, 
    :math:`d_{out,hub}` and :math:`d_{out,shroud}` and the heights  :math:`h_{inlet}`,
    :math:`h_{hub}` and :math:`h_{shroud}`. 
    
    Through the second and third control points the contour of the channel is set. 
    Their positions are defined trough offsets from the first and fourth control points
    with the lengths :math:`l_{hub,0}`, :math:`l_{hub,1}`, :math:`l_{shroud,0}`,
    and :math:`l_{shroud,1}`. 
    With the angle :math:`\gamma_{hub1}` the outlet angle of the hub curve can be set. 
     
    In the following lines the displacements for the third control point of the hub curve 
    are calculated:

    >>> dx_hub1 = np.cos(angle_hub1)*l_hub1
    >>> dz_hub1 = np.sin(angle_hub1)*l_hub1
    
    In the following two code blocks the hub and shroud curves are defined in the lists
    ``hubCurves`` and ``shroudCurves``. The control points are created as ``dtPoint3`` 
    and the curves as ``bSplineCurve_pointConstructOCC`` objects.

    Four hub curves are defined here. The first two are defined with the parametrization
    shown in :numref:`meas`. The thrid hub curve extends to an x-coordinate of zero and forms 
    the shaft end of the turbine.
    The fourth hub curve extends in negative z-direction on a radius of zero.    

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
	
    Four shroud curves are defined. The first two are defined from the parametrization
    in :numref:`meas`. The thrid shroud curve extends vertivalli in negative z-direction.
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
    
    The regular channels and the special hub and shroud curves are 
    created with the method `createMeridional`. 
    The inputs of this method are the lists with the hub and shroud curves
    (``hubCurves`` and ``shroudCurves``)
    and the a configuration dictionary, which specifies the positions and 
    curvatures of the interface curves.
    
    In the method the builder class `analyticGeometry_piecewiseRadMeridional` is called. 
    An object of this class is created in order to manage the meridional contour.
    Based on the configuration dictionary the interface curves are built with this object.
    With the interfaces the regular channels for the blade geometries and the special 
    hub and shroud curves for the draft tube cone are created. 
    
    The following figure shows the interfaces and the resulting regular channels,
    as well as the special curves which are not part of the regular channels.
    
    .. _interfaces:
    .. figure:: meridionalFigs/interfaces.png
       :width: 50%
       :align: center

       Hub and shroud curves (black), interface curves (red) 
       and inlet and outlet curves (orange). The special curves
       are downstrem of the last interface curve.
       The two dimensional faces of the first regular channel (yellow) 
       and the second regular channel (green).
    
    Between the start points of the first hub and shroud curves in the lists an inlet
    curve is created. Between the end points of the last hub and shroud curves an outlet 
    curve is created.
 
    In the configuration dictionary nested lists with the keys ``interface_hub``, 
    ``interface_shroud`` and ``interface_curvature`` are defined. 
    Those lists allow the definition of the interface curves.

    The following figure shows the creation of the interface curves whith a focus
    on the curvature of the second interface curve.

    .. _interfaceCalc:
    .. figure:: meridionalFigs/interfaceCalc.png
       :width: 80%
       :align: center

       Creation of the interface curves (red) between the hub and shroud curves (black)
       with the linear meanplane curve `MP,lin` (green).

    :numref:`map-fig-var_interface` gives the mapping between math symbols in :numref:`interfaceCalc`
    and the naming of the list keys in this example.

    .. _map-fig-var_interface:
    .. csv-table:: Mapping between math symbol and variable names.
       :header: "Symbol", "Label"
       :widths: 10, 20
       :escape: #
       :align: center

       :math:`a`        ,``interface_curvature[i][0]``
       :math:`b`        ,``interface_curvature[i][1]``
       :math:`c`	,``interface_curvature[i][2]``

    The lists ``interface_hub`` and ``interface_shroud`` define the positions
    of the start and end points of the interface curves on the hub and shroud curves.
    
    The lists are unpacked as follows:

	``interface_hub[i]`` :		Start point of the i-th interface curve on the hub.
	
        ``interface_shroud[i]`` :	End point of the i-th interface curve on the shroud.
	
    The lower level lists specify the hub and shroud curves and the percentages of those 
    curves where the start and end points of the interfaces are positioned:
	
	``interface_hub[i][0]`` : 	Number of the hub curve on which the start point is located.
	
        ``interface_hub[i][1]`` :	Percentage of the hub curve where the start point is located.
    
    
    The interface curves are calculated by first creating a straight line `MP,lin` between 
    the interfaces' start and end points. With the list ``interface_curvature`` a control 
    point can be calculated which sets the curvature of the interface curve based on ``MP,lin``.
    Finally the interface curve is created from the start and end points of the interface 
    and the control point.
 
    The highest list level of ``interface_curvature`` corresponds to the number of the interface curve:

	``interface_curvature[i]`` :	Number of the interface.

    The lower level of the list sets the curvature of the interface as follows:

	``interface_curvature[i][0]`` :	Control point offset as procentual length of 'MP,lin'.
	
        ``interface_curvature[i][1]`` : Control point base position as percentage on `MP,lin`.
	
        ``interface_curvature[i][2]`` :	Direction of control point offset.	
    
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

    When the start or end point of a interface is on the span of a hub and shroud curve,
    the curve is cut into two curves at this point. Those two curves then share their 
    end or respectively their start points with the interface.
    
    The first regular channel is created from the inlet curve and the first interface curve,
    as well as the hub and shroud curves extending inbetween them.
    The following regular channels are defined between the seqential interfaces and the 
    hub and shroud curves inbetween them.  

    The layered region can be build from the last interface curve, the outlet curve and 
    the special curves, which are not part of the regular channels.
    

    
    Create the meridional channel:
  
    >>> generate.createMeridional(configMeridional, hubCurves, shroudCurves)
    
    
    Bladed Channels
    ---------------
    
    For the generation of the bladed channel a grid channel, which can be meshed is created.
    This grid channel contains transfinite mesh blocks which surround the blade the rest of
    the grid channel is  meshed unstructured.

    The grid channel of the blade is created with the method `createBlade`. 
    This method takes a configuration dictionary, which contains the necessary
    geometry parameters, as input.

    The key ``regChannel`` in the dictionary passes, in which regular channel the blade is created. 
    The number of blades can be set with ``nBlades``. Based on the number of blades a periodic segment 
    with one blade of the bladed channel is build.
    With ``adjustRadius`` shifts during the mapping of the blade in the radial channel can activated or 
    deactivated.  
    
    A blade is built by combining the blade's meanplane contour with a thickness distribution.
    The resulting blade is parametrized in the uvw-parameter space. By doing a conformal mapping inside 
    the regular channel, the blade can defined in xyz-coordinates. 
    The regular channels are returned by the object of the class `analyticGeometry_piecewiseMeridionalRotContour` 
    which was created in the method `createMeridional`.
    
    The meanplane is defined with the parameters ``spanwiseCuts_mp``, ``alpha_1``, ``alpha_2``, ``ratioX``, 
    ``deltaY``, ``offX`` and ``offY``.
    The thickness distribution is defined with the parameters ``spanwiseCuts_td``, ``t_le``, ``u_le``, 
    ``t_mid``, ``u_mid``, ``t_te`` and ``u_te``.
    The parameters are nested in lists. The list items are the blade parameters along the blade's span 
    from hub to shroud.
    The parametrization is further described in `[Fraas_2025] <https://doi.org/10.3390/ijtpp10040038>`_.

    The creation of the meanplane and the thickness distribution is done with
    ``scaOneD_scaCurve2dOneDPointConstruct`` objects which define a functional relation
    of the parameters along the spanwise direction.
    The constructor of a ``scaOneD_scaCurve2dOneDPointConstruct`` object needs a list 
    with ``dtPoint2`` objects as well as an order of the function as inputs.
    To create those inputs the lists are handed to the method `fillInputList`. 
    This method creates the list containing ``dtPoint2`` objects, which in 
    turn contain the parameter values of the list  and the percentage of the blade span 
    where those values are applied. Furthermore the order of the function is set.
    
    The following picture shows the guide vane with the mesh blocks.

    .. _meshBlock:
    .. figure:: bladeFigs/guideVane_meshBlocks.png
       :width: 66%
       :align: center

       Guide vane blade (grey), surrounded by mesh blocks.

   
    The guide vane blade is defined with the following configuration dictionary.
    Here the lists with blade parameters contain only one value, this results in a constant cross section of the
    blade along the spanwise direction.
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
    
    :numref:`gridChannel` shows the final grid channel geometry of the guide vane.
    The periodic faces are built partly from the mesh block faces (green) and meanplane faces
    (yellow) which extend from the mesh block faces to the inlet and outlet (red) of the 
    regular channel.    
     
    .. _gridChannel:
    .. figure:: bladeFigs/guideVane_channel.png
       :width: 100%
       :align: center

       Created grid channel with one guide vane blade (grey) surrounded by transfinite mesh blocks.
       Inlet and outlet boundaries (red) and periodic boundary faces (yellow/green). Hub and shroud
       faces not shown.

    Create the guide vane geometry:

    >>> generate.createBlade(configGuideVane)

    The runner geometry can also be created with the method `createBlade`.
    The runner has 15 blades and is mapped onto the second regular channel.
    Here the lists with blade parameters contain multiple values.  
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
    
    The mesh of the draft tube cone is created as a combination of an unstructured region
    and transfinite regions on the hub and shroud walls.

    The method `createLayerRegion` is used for the generation and the meshing of the geometry. 
    The basis for the draft tube cone geometry are the special hub and shroud curves and the 
    inlet and outlet curves of the draft tube cone. The creation of the layers is specified 
    
    Those curves can are returned by the object of the class 
    `analyticGeometry_piecewiseMeridionalRotContour` which was created in the `createMeridional`
    method.
     
    .. _speCurves:
    .. figure:: meridionalFigs/speCurves.png
       :width: 50%
       :align: center

       Special hub (speHub) and special shroud (speShroud) curves (black), as well as the inlet
       (red) and outlet (orange) curves of the draft tube cone.


    >>> configLayer = {
    ...     "label" : "radMeridionalContour",
    ...     "nSlices" : 15,
    ...     "layer_thickness" : 0.2,
    ...     "layer_supports" : [0.5],
    ... }
    
    >>> generate.createLayerRegion(configLayer)
    
    .. _layer3d:
    .. figure:: meridionalFigs/layers2d.png
       :width: 66%
       :align: center

       Three dimensional section of the draft tube cone.

    >>> container = generate.getContainer()
    >>> bV, dC = generate.getbVAnddC()
    
    .. _layerMesh:
    .. figure:: meridionalFigs/layersMesh.png
       :width: 66%
       :align: center

       Mesh of the draft tube cone.

    >>> bV["gv_mesh"].makeGrid()
    >>> bV["ru_mesh"].makeGrid()
    >>> bV["meshLayers"].makeGrid()

    """
	
    def __init__(
        self,
    ):
	
        """Constructor.

        Parameters
        ----------

        Returns
        -------
        None

        """ 
        # Create an empty self.container
        self.container = dtOO.dtBundle()

        #importlib.reload(builder)
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
         
        # creating the meridional contour
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
        #
        # Building Layer Region
        #
        speHub, speShroud, inOutCurves = self.radMeridionalContour.getLayerRegionCurves()
        # creating the layer region object
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
        # Meshing of layer region
        #
        # returning the hub and shroud layers
        layers = layerRegion.getLayerList(configL["nSlices"])    
        # returns layer data in the following nested list:
        # layers = [[hub layer lists],[shroud layer list]]
        # with:
        # [hub layer lists] = [[3d layer domain], [bool list radius zero]]

        # returns the unstructured region and its surfaces
        mv, bs = layerRegion.getUnstructuredRegion(configL["nSlices"])
        
        from dtOOPythonApp.builder import ( map3dTo3dGmsh_gridFromLayers ) 
        # creating the mesh of the suction area with wall layers and the unstructured region
        #self.container = map3dTo3dGmsh_gridFromLayers(
        #        mv = mv,
        #        bs = bs,
        #        label = "meshLayers",
        #        layers = layers,
        #        nElementsLayer = 15,
        #        firstElement = 0.001,
        #        elementSize_sw = 0.02,
        #        elementSize_circ = 0.01,
        #        charLengthMin = 0.02,
        #        charLengthMax = 0.04,
        #    ).buildExtract(self.container)
        self.container = map3dTo3dGmsh_gridFromLayers(
                mv = mv,
                bs = bs,
                label = "meshLayers",
                layers = layers,
                nElementsLayer = 15,
                firstElement = 0.001,
                elementSize_sw = 0.08,
                elementSize_circ = 0.06,
                charLengthMin = 0.05,
                charLengthMax = 0.1,
            ).buildExtract(self.container)
        
    #
    # build blade function
    #
    def createBlade(self, configB):
        
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

        self.aG.push_back(
            self.radMeridionalContour.getRegChannel(
                regChannel, 1
            ) << "xyz_"+label+"_channel"
        )
        #
        # meanplane
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
        # thickness distribution
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
        # Combination of the Meanplane and the Thickness distribution
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
        # conformalMapping
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
        # mesh block
        #
        meshBlock_thickness = 0.025 
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
        # split mesh block
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
        
        # building the meanplane faces extending from the mesh blocks 
        #  to the tangentially offset meanplane curve
        for i in range(2):
            # curve seqence is switched between inlet and outlet in 
            #  order to keep u and v directions consistent with the 
            #  mesh block faces
            if i == 0:
                at = "in"
                c0 = 0
                c1 = 1
            elif i == 1:
                at = "out"
                c0 = 1
                c1 = 0
            # generating faces
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
        # do conformal mapping
        #
        #  mapping single faces
        for ii in [label+"_meanplane", label+"_blade", label+"_meshBlock",]:
          theAG = dtOO.vec3dTwoDInMap3dTo3d(
            dtOO.vec3dTwoD.MustConstDownCast(
              conMap.applyAnalyticFunction(self.aF[ii].clone())
            ),
            dtOO.map3dTo3d.ConstDownCast( self.aG["xyz_"+label+"_channel"] )
          )
          theAG.setLabel("xyz_"+ii)
          self.aG.push_back( theAG.clone() )
        #  mapping multiple faces (aF of meanplanefaces)
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
        #  mapping multiple volumes
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
        #  mapping multiple curves (meshBlockCurves)
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
        

        # collecting the curves for the meanplane
        # mesh block curves extend form hub to shroud and are tangentially 
        #  offset from the specified mesh block faces
        meshBlockCurves = dtOO.labeledVectorHandlingAnalyticGeometry()
        for iNum in self.aG.getIndices("xyz_"+label+"_meshBlockCurve_*"):
            ii = self.aG.getLabel( iNum )
            meshBlockCurves.push_back(self.aG[ii].clone())
        
        # creating the meanplane surfaces extending from the tangentially 
        #  offset meshblock curves to the inlet or the outlet
        from dtOOPythonApp.builder import analyticSurface_inOutFeMeanplane
        self.container = analyticSurface_inOutFeMeanplane(
            prefix = "xyz",
            label = label,
            channel = self.aG["xyz_"+label+"_channel"],
            curves = meshBlockCurves
        ).enableDebug().buildExtract(self.container)
         
        #
        # ordering geometries for meshing
        #
        # collecting and organizing mesh blocks
        blocks = []
        for iNum in self.aG.getIndices("xyz_"+label+"_meshBlock_*"):
            blocks.append( self.aG[ self.aG.getLabel( iNum ) ] )
        # sorting the blocks by number
        blocks.sort(key=lambda x: int(x.getLabel().split('_')[-1]))
        
        # collecting relevant faces
        # couplingFaces contains the faces which will connect the
        #   mesh block to the grid channel
        # meanplaneFaces are the faces which will form the periodic
        #   boundaries of the grid
        couplingFaces = []
        meanplaneFaces = []

        # faces extending from outlet to the mesh block trailing edge 
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_out1"]))
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_out0"]))

        for i, block in enumerate(blocks):
          face = dtOO.map3dTo3d.MustDownCast( block ).segmentConstWPercent( 1.0 )
          
          # appending the specified meanplane and coupling faces on the mesh blocks 
          if i <= nMeanplaneBlocks:
            meanplaneFaces.append(face)
            self.aG.push_back(face.clone() << "xyz_"+label+"_meanplaneFaceOnBlock_"+str(i))
          else:
            couplingFaces.append(face)
        
        # faces extending from the mesh block leading edge area to the inlet
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_in0"]))
        meanplaneFaces.append(dtOO.map2dTo3d.MustDownCast(self.aG["xyz_"+label+"_fe_meanplane_in1"]))
        
        # last two coupling faces at trailing edge
        couplingFaces.append(dtOO.map3dTo3d.MustDownCast(blocks[-1]).segmentConstUPercent( 1.0 ))
        couplingFaces.append(dtOO.map3dTo3d.MustDownCast(blocks[0]).segmentConstUPercent( 0.0 ))
        
        #
        # create grid channel
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
        
        gc, gcFaces = gridChannel.getGridChannel()
        self.aG.push_back(
            gc << "xyz_"+label+"_gridChannel"
        )
        
        #
        # generating mesh
        #
        from dtOOPythonApp.builder import (
          map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks
        )
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
        #           dtOO.dtPoint2(0.00, 0.015),
        #           dtOO.dtPoint2(0.45, 0.005),
        #           dtOO.dtPoint2(0.50, 0.004),
        #           dtOO.dtPoint2(0.55, 0.005),
        #           dtOO.dtPoint2(1.00, 0.015),
        #        ], 1
        #    )(),
        #    bladeHubElementScale = 0.3,
        #    charLengthMax=0.015,
        #    charLengthMin=0.015,
        #    meshTEBlocks = True,
        #).enableDebug().buildExtract( self.container )
        self.container = map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks(
            label = label+"_mesh",
            channel = self.aG["xyz_"+label+"_gridChannel"],
            channelFaces = gcFaces,
            blocks = blocks,
            nMeanplaneBlocks = nMeanplaneBlocks,
            blade = self.aG["xyz_"+label+"_blade"],
            nBoundaryLayers = 15,
            nElementsSpanwise = 50,
            nElementsNormal = 7,
            firstElementSizeHubToShroud = 0.001,
            firstElementSizeNormalBlade = 0.001,
            bladeHubElementSize = scaOneD_scaCurve2dOneDPointConstruct(
                [
                   dtOO.dtPoint2(0.00, 0.04),
                   dtOO.dtPoint2(0.45, 0.007),
                   dtOO.dtPoint2(0.50, 0.007),
                   dtOO.dtPoint2(0.55, 0.007),
                   dtOO.dtPoint2(1.00, 0.04),
                ], 1
            )(),
            bladeHubElementScale = 0.3,
            charLengthMax=0.1,
            charLengthMin=0.04,
            meshTEBlocks = True,
        ).enableDebug().buildExtract( self.container )
    
    #
    # returns a list with dtPoint2 types and spline orders
    #  with spanwise cut percentage and blade input parameters
    #
    def fillInputList(self, inList):
        
        # only one entry
        if len(inList) == 1:
            # the inputs will be the same at spanwiseCut 0 and 1
            outList = [
                        dtOO.dtPoint2(0.00, inList[0]), 
                        dtOO.dtPoint2(1.00, inList[0]),
                      ]      
            order = 1

        else:
            outList = []
            for nL in range(len(inList)):
                # calculating the percentage of the spanwise cut from normalized index
                cutPercent = nL/(len(inList)-1)
                # appending percentage and input
                outList.append(dtOO.dtPoint2(cutPercent, inList[nL]),)
            
            #the order is one less than the number of inputs
            order = len(inList) -1
        
        return outList, order
    
    def getContainer(self):
        return self.container

    def getbVAnddC(self):
        return self.bV, self.dC

