# ------------------------------------------------------------------------------
# dtOO < design tool Object-Oriented >
#
#   Copyright (C) 2024 A. Tismer.
# ------------------------------------------------------------------------------
# License
#   This file is part of dtOO.
#
#   dtOO is distributed in the hope that it will be useful, but WITHOUT
#   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#   FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
#   dtOO root directory for more details.
#
#   You should have received a copy of the License along with dtOO.
#
# ------------------------------------------------------------------------------

from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
    dtLinearAlgebra,
    analyticGeometry,
    jsonPrimitive,
    rotate,
    analyticCurve,
    bSplineCurve_pointConstructOCC,
    analyticSurface,
    dtPoint2,
    dtPoint3,
    dtVector3,
    vectorDtPoint3,
    rotatingMap2dTo3d,
    partRotatingMap2dTo3d,
    trimmedCurve_uBounds,
    geomCurve_curveReverseConstructOCC,
    multipleBoundedVolume,
    infinityMap3dTo3d,
    curveCurveDist,
    gslMinFloatAttr,
    vectorReal,
    vectorHandlingConstDtCurve,
    vectorHandlingAnalyticGeometry,
    rectangularTrimmedSurface_curveRotateConstructOCC,
    multipleBoundedSurface,
    bSplineSurface_bSplineCurveFillConstructOCC,
    bSplineCurve_curveConnectConstructOCC
)

from typing import List, Union, Tuple
from collections import defaultdict
import logging
import numpy


class analyticGeometry_layerRegion(dtBundleBuilder):
    """Create layered flow channel as fiev or six sided layer regions and a multiple bounded
    volume.
    
    This class:
        
        - Takes hub, shroud, inlet and outlet curves of the layered region.
        - Creates layer boundary curves on the hub and shroud walls.
        - Creates layer faces from the boundary curves.
        - Creates layer volumes by rotating the boundaries.
        - Create a multiple bounded volume in the flow channel connecting to the layers.

    Attributes
    ----------
    label_: str
      Label.
    rotVector_: dtVector3
      Rotation vector.
    origin_: dtPoint3
      Origin.
    normalAxis_: dtVector3
      Normal direction of the 2d region curves.
    speCenter_: dtPoint3
      Center point of the bounding box of the region.
    speBb_: pairDtPoint3
      Bounding box of the whole region.
    hubLayerCurves_: List[analyticGeometry]
      Boundary curves of the hub wall.
    hubRadZero_: List[Bool]
      List containing information about which hub curves are on a radius of zero.
    hubUnstructBounds_: List[analyticGeometry]
      Boundary curves of the multiple bounded volume at the hub.
    shroudLayerCurves_: List[List[analyticGeometry]]
      Boundary curves of the shroud Layer regions.
    shroudRadZero_: List[Bool]
      List containing information about which shroud curves are on a radius of zero.
    shroudUnstructBounds_: List[analyticGeometry]
      Boundary curves of the multiple bounded volume at the shroud.
    interfaceUnstructBound_: analyticGeometry
      Inlet boundary curve of the multiple bounded volume.
    outletUnstructBound_: analyticGeometry
      Outlet boundary curve of the multiple bounded volume.
    unstructVH_: vectorHandlingAnalyticGeometry
      vector handler containing all boundary curves of the multiple bounded volume.
    hubLayers_: List[analyticGeometry]
      List containing the faces of the six sided hub layers.
    shroudLayers_: List[analyticGeometry]
      List containing the faces of the six sided shroud layers.

    Examples
    --------
    >>> import dtOOPythonSWIG as dtOO
    
    Define inlet, outlet, hub and shroud curves of the layer.

    >>> inlet = dtOO.analyticCurve(
    ...   dtOO.bSplineCurve_pointConstructOCC(
    ...     dtOO.vectorDtPoint3()
    ...       << dtOO.dtPoint3(0.50, +0.00, 0.80)
    ...       << dtOO.dtPoint3(0.75, +0.00, 0.80)
    ...       << dtOO.dtPoint3(1.00, +0.00, 1.00),
    ...     2
    ...   ).result()
    ... )
    >>> outlet = dtOO.analyticCurve(
    ...   dtOO.bSplineCurve_pointConstructOCC(
    ...     dtOO.vectorDtPoint3()
    ...       << dtOO.dtPoint3(0.00, +0.00, 0.00)
    ...       << dtOO.dtPoint3(1.10, +0.00, 0.00),
    ...     1
    ...   ).result()
    ... )
    >>> hub0 = dtOO.analyticCurve(
    ...   dtOO.bSplineCurve_pointConstructOCC(
    ...     dtOO.vectorDtPoint3()
    ...       << dtOO.dtPoint3(0.50, +0.00, 0.80)
    ...       << dtOO.dtPoint3(0.25, +0.00, 0.60)
    ...       << dtOO.dtPoint3(0.00, +0.00, 0.40),
    ...     2
    ...   ).result()
    ... )
    >>> hub1 = dtOO.analyticCurve(
    ...   dtOO.bSplineCurve_pointConstructOCC(
    ...     dtOO.vectorDtPoint3()
    ...       << dtOO.dtPoint3(0.00, +0.00, 0.40)
    ...       << dtOO.dtPoint3(0.00, +0.00, 0.00),
    ...     1
    ...   ).result()
    ... )
    >>> shroud0 = dtOO.analyticCurve(
    ...   dtOO.bSplineCurve_pointConstructOCC(
    ...     dtOO.vectorDtPoint3()
    ...       << dtOO.dtPoint3(1.00, +0.00, 1.00)
    ...       << dtOO.dtPoint3(1.10, +0.00, 0.00),
    ...     1
    ...   ).result()
    ... )
    
    Create the input lists with the boundary curves.

    >>> speHub = [hub0, hub1]
    >>> speShroud = [shroud0]
    >>> inOutCurves = [inlet, outlet]
    
    Initialize the builder.

    >>> from dtOOPythonApp.builder import analyticGeometry_layerRegion
    >>> builder = analyticGeometry_layerRegion(
    ...   label = "test",
    ...   speHub = speHub,
    ...   speShroud = speShroud,
    ...   inOutCurves = inOutCurves,
    ...   layer_thickness = 0.2,
    ...   layer_supports = [0.33, 0.66]
    ... ).enableDebug()
    >>> builder.build()
    
    Build the multiple bounded volume of the layer region.

    >>> unstrReg, surf = builder.getUnstructuredRegion(10)
    
    Check the virtual class name.

    >>> unstrReg.virtualClassName()
    'multipleBoundedVolume'
    

    This class is used to create the geometry of a flow channel which consists of five or six 
    sided layer volumes on the hub and shroud walls and a multiple bounded volume, which
    expands inside of the flow domain, connecting to the layers.
    The layer regions can be meshed tansfinite, while the multiple bounded volume can be
    meshed unstructured in another class. In this documentation, the region formed by the
    multiple bounded volume is referred to as unstructured region.

    The main method in this class is the constructor, from which the other methods are
    called. The rotational vector and the point of origin are initialized as 
    ``rotVector_`` and ``origin_``. 

    The method :meth:`calculateNormalAxis` is used to calculate ``normalAxis_`` of the
    cross-section of the flow domain from the hub and shroud curves. 
    The method furthermore returns the cross-section's bounding 
    box ``speBb_`` and its center point ``speCenter_``.

    The bounding curves of the layers are created with the method 
    :meth:`createLayerBounds`. The method is called twice to create the bounds of the hub
    and shroud wall layer faces. The returns of the two calls are instantiated in the lists 
    ``hubLayerCurves_``, ``hubRadZero_`` and ``hubUnstructBounds_`` for the hub layer and
    ``shroudLayerCurves_``, ``shroudRadZero_`` and ``shroudUnstructBounds_`` for the 
    shroud layer.

    The lists ``hubLayerCurves_`` and ``shroudLayerCurves_`` contain the bounding curves
    of the corresponding layer faces. In the method :meth:`createLayerBounds` the method 
    :meth:`layerCurve` is called.

    The lists ``hubRadZero`` and ``shroudRadZero`` contain a number of Booleans
    corresponding to the number on layers which are created on the hub or shroud. The
    Booleans show which of the layers are located on a radius of zero.
    The method :meth:`rz_xyz` is used to check for this.

    The lists ``hubUnstructBounds_`` and ``shroudUnstructBounds_`` contain the layer 
    boundary curves, which extend inside the flow domain and form the connections 
    between the unstrucured multiple bounded volume and the layers.

    The inlet and outlet curves of the flow domain's cross-section are shared between the
    layer regions on hub and shroud and the unstructured region. By splitting the curves,
    the inlet of the unstructured region is appointed to the variable 
    ``interfaceUnstructBound_`` and the outlet to ``outletUnstructBound_``.

    The vector handler ``unstructVH_`` is as a handler of all hub and shroud boundary curves of 
    the unstrucured region. The curves in ``hubUnstructBounds_`` and ``shroudUnstructBounds_``
    as well as the curves ``interfaceUnstructBound_`` and ``outletUnstructBound_``
    are stored in the vector handler.

    From the bounding curves of the hub layers and the shroud layers, layer faces are 
    created and stored in ``hubLayers_`` and ``shroudLayers_``.

    The layer volumes are created by rotating the layer faces around the rotation axis 
    and returned by the method :meth:`getLayerList`. The method returns a list of in 
    following format:

    .. code-block:: python

        layerList = Tuple[
                Tuple[
                    List[analyticGeometry],
                    List[bool]
                ],
                Tuple[
                    List[analyticGeometry],
                    List[bool]
                ]
            ]

    The entries correspond to the following values:

        - ``layerList[0]`` : Hub layers.
        - ``layerList[1]`` : Shroud layers
        - ``layerList[i][0]`` : List of layer volumes
        - ``layerList[i][1]`` : List of bool values which tell if the layer is on a radius of zero.
    
    The unstructured region is created in the method :meth:`getUnstructuredRegion`. The
    method returns the multiple bounded volume of the unstructured region and a list
    containing its boundary surfaces.

    The :meth:`build` method is used to plot the created geometries in `ParaView`.

    """

    def __init__(
        self,
        label: str,
        speHub: List[analyticGeometry],
        speShroud: List[analyticGeometry],
        inOutCurves: List[analyticGeometry],
        layer_thickness: float = 0,
        layer_supports: List[float] = [],
        rotVector: dtVector3 = dtVector3(0, 0, 1),
        origin: dtPoint3 = dtPoint3(0, 0, 0),
    ) -> None:
        """Constructor.
        
        This method:

            - Creates layer boundary curves.
            - Creates layer faces.
            - Creates boundary curves of the unstructured region.

        Parameters
        ----------
        label: str
          Label.
        speHub: List[analyticGeometry]
          Channel side of the grid channel.
        speShroud: List[analyticGeometry]
          Channel side of the grid channel.
        inOutCurves: List[analyticGeometry]
          Inlet and aoutlet Curve of the layer region.
        layer_thickness: float
          Thichnes of mesh layers in unstructured domain.
        layer_supports: List[float]
          Number position of support points on channel curves for layer creation.
        rotVector: dtVector3
          Rotation vector.
        origin: dtPoint3
          Origin.

        Returns
        -------
        None
        

        The boundary curves of the channel are handed over to the constructor with the 
        lists ``speHub``, ``speShroud`` and ``inOutCurves``. They contain the curves of
        the hub and shroud walls an the inlet and outlet curves of the flow channel's 
        cross-section.

        The following figure shows the cross-section and the curves.

        .. _speCurves0:
        .. figure:: meridionalFigs/speCurves.png
           :width: 50%
           :align: center

           Special hub (``speHub``) and special shroud (``speShroud``) curves (black),
           as well as the inlet (red) and outlet (orange) curves of the draft
           tube cone.
        
        The rotation vector and the inputs are instantiated with the variable names 
        ``rotVector_`` and ``origin_``.

        The hub and shroud curves are handed to the method :meth:`calculateNormalAxis`.
        This method creates and returns the bounding box which surrounds the curves 
        ``speBb_``. Furthermore the center point ``speCenter_`` and
        the normal axis ``normalAxis_`` of the bounding box are returned.

        The boundary curves of the layers are created with the method 
        :meth:`createLayerBounds`.  The method is called once for the hub layers and
        once for the shroud layers. 
        As inputs the wall curve lists ``speHub`` or ``speShroud``, the inlet and outlet
        curves ``inOutCurves`` are handed to the method.
        The layer creation is defined with the thickness of the layers ``layer_thickness``
        (:math:`t_{Layer}`) and the list of support point positions along the spans of 
        the wall curves ``layer_supports``.
        The label is used to set the physical names of the created geometries for 
        plotting.

        The returns are the boundary curves of the layers ``hubLayerCurves_`` and 
        ``shroudLayerCurves_``, lists encoding if any of the layers is on a radius of
        zero ``hubRadZero_`` and ``shroudRadZero_`` and the connecting curves to the 
        unstructured region ``hubUnstructBounds_`` and ``shroudUnstructBounds_``.
        
        Each wall curve which is not on a radius of zero gets a layer face.
        Each layer face has four boundary curves. The first boundary curves
        are the individual wall curves of the hub or shroud.

        The second and fourth boundary curves extend from the walls into the flow channel.
        For the first and last layer faces, at the hub and shroud, these correspond either
        to the inlet or outlet curves, or to a hub curve located at a radius of zero.
        At the intersection points of two wall curves, boundary curves extending into
        the flow channel are constructed so they point in the mean normal direction
        of both curves.

        The third boundary curves connect the second and fourth boundary curves inside the
        flow channel. They are constructed from the end points of the second and fourth
        boundary curves and support points. These support points are calculated by
        translating points on the hub and shroud curves in the normal direction of the curves.
        The positions of these points on the hub and shroud curves are defined using the list
        ``layer_supports``.

        The lists ``hubLayerCurves_`` and ``shroudLayerCurves`` have the format 
        ``List[List[analyticGeometry]]``. In this list structure the first level list 
        contains the four bounding curves of one layer face in the second level list. 
        The arrangement of the bounding curves in the second level list are as follows:

            1. ``hubLayerCurves_[i][0]``: Wall curve of the i-th special hub curve.
            2. ``hubLayerCurves_[i][1]``: Downstream curve extending from the wall 
               into the flow channel or, in case of the last layer, along the outlet or
               a wall curve on a radius of zero.
            3. ``hubLayerCurves_[i][2]``: Curve extending inside th flow channel.
            4. ``hubLayerCurves_[i][3]``: Upstream curve extending from the wall into 
               the flow channel or, in case of the first layer along the inlet interface.
        
        This order results in the following parameter directions of the layer volumes
        after their rotation:

            - u : circumferential direction
            - v : streamwise direction
            - w : wall to flow domain direction

        The following figure shows the boundary curves of the layers in black. 
        As an example for the numbering sequence of the bounding curves, the indices of 
        the second level list in ``hubLayerCurves[0]`` are shown.

        .. _layer2d_numbering:
        .. figure:: meridionalFigs/layers2d_numbering.png
           :width: 40%
           :align: center

           Two-dimensional layer faces (blue) in the draft tube cone.

        The lists containing Boolean values ``hubRadZero_`` and ``shroudRadZero_`` have a 
        ``True`` value for the layers which are on a radius of zero. These layers form the
        last layer face on the wall. Their second bounding curve is formed by part of the
        following boundary curve which is located on the radius of zero.
        In the above figures this is the case for the curve in ``speHub[2]``.

        The curves in ``hubUnstructBounds_`` and ``shroudUnstructBounds_`` are copies of
        the third boundary curves. These are used to create the multiple bounded volume of 
        the unstrucured region. 
        
        The inlet of the flow domain created in this class is the curve in 
        ``inOutCurves[0]``. Part of this curve is used as the inlet boundary of the 
        unstructured region.
        The curve is trimmed between the lengths of the layer thicknesses 
        ``layer_thickness`` with the `dtOO` class `trimmedCurve_uBounds`. The trimmed 
        curve is instantiated as ``interfaceUnstructBound_``.

        To trim the outlet curve in ``inOutCurves[1]`` it has to be checked if a layer is
        located on the last hub curve. No layer on the hub curve is created if the hub 
        extends to a radius of zero. In this case ``hubRadZero_[-1] == True`` applies and
        the first trim parameter is set to zero. Else the trim parameter will be
        determined by ``layer_thickness`` for both the trimm at the hub and the shroud. 
        The resulting curve is instantiated in ``outletUnstructBound_``.
        
        The object ``unstructVH_`` of the `dtOO` class `vectorHandlingAnalyticGeometry`
        is instantiated to contain all bounding curves of the unstructured region's
        contour. The curves in ``hubUnstructBounds_`` and ``shroudUnstructBounds_`` are
        added to this object, ``interfaceUnstructBound_`` is prepended and 
        ``outletUnstructBound_`` is appended.

        The faces of the hub and the shroud layers are created by iterating over the 
        lists ``hubLayerCurves_`` and ``shroudLayerCurves_``. In each iteration the
        second level curves are iterated and pushed in a vector handler object named
        ``layer_vhc``.
        The layer faces are created with the `dtOO` class 
        `bSplineSurface_bSplineCurveFillConstructOCC` with ``layer_vhc`` as input.
        The layer faces are stored in the lists ``hubLayers_`` and ``shroudLayers_``.
        """
        logging.info("Initializing %s ..." % (label))
        super(analyticGeometry_layerRegion, self).__init__()
        self.label_ = label
        
        self.rotVector_ = dtLinearAlgebra.normalize(rotVector)
        self.origin_ = origin
        
        #
        # calculating the centerpoint of the special curves
        # will be later used in order to make sure the layer boundaries are pointing inside the domain
        # the bounding box of the special curves self.speBb_ will be used to generate 
        #  the multi bounded volume of the unstructured region
        #
        self.normalAxis_, self.speCenter_, self.speBb_ = self.calculateNormalAxis(speHub + speShroud)
        
        logging.info("### Creating Hub Layer Curves")
        #
        # Creating the boundary curves for the layers
        # each layer is defined by four curves those returned through self.hubLayerCurves_
        # the hubLayerCurves_ list has the following format:
        # self.hubLayerCurve = [
        #                        [channel curve, 
        #                        streamwise boundary 1, 
        #                        parallel to channel curve, 
        #                        streamwise boundary 0],
        #                        ...
        #                      ]
        # 
        # self.hubRadZero_ contains booleans which will show true for layers which have a radius of zero
        #  this is used for meshing later
        #
        # self.hubUnstructBounds_ contains all the hub  boundaries of the unstructured region 
        #  these will be the curves parallel to the channel curves or the channel curves if they are on
        #  a radius of zero
        #
        self.hubLayerCurves_, self.hubRadZero_, self.hubUnstructBounds_ = self.createLayerBounds(
                speHub, 
                inOutCurves,
                layer_thickness, 
                layer_supports,
                'Hub'
            )
        
        logging.info("### Creating Shroud Layer Curves")
        self.shroudLayerCurves_, self.shroudRadZero_, self.shroudUnstructBounds_ = self.createLayerBounds(
                speShroud,  
                inOutCurves,
                layer_thickness,
                layer_supports,
                'Shroud'
            )
        
        #
        # trimming the inlet curve between the layerthicknesses
        #  this curve will be used as a boundary for the unstructured region
        #
        interf = analyticCurve.MustDownCast(inOutCurves[0]).ptrDtCurve()
        self.interfaceUnstructBound_ = analyticCurve(
                trimmedCurve_uBounds(
                    interf, 
                    interf.u_l(layer_thickness), 
                    interf.u_l(interf.length() - layer_thickness)
                ).result()
            )
        
        #
        # trimming the outlet curve
        #
        # deciding the trimm position of the outlet curve at the hub
        outlet_trim = layer_thickness
        # if the outlet curve hub curve at the outlet has a radius of zero
        #  the following condtion will be fulfilled:
        if self.hubRadZero_[-1] == True:
            # sets the trim length to zero
            outlet_trim = 0

        # trimming the outlet curve for the boundary of the unstructured domain
        outl = analyticCurve.MustDownCast(inOutCurves[-1]).ptrDtCurve()
        self.outletUnstructBound_ = analyticCurve(
                trimmedCurve_uBounds(
                    outl,
                    outl.u_l(outlet_trim),
                    outl.u_l(outl.length() - layer_thickness)
                ).result()
            )
        
        # adding the boundary curves of the unstructured regions to a VH
        self.unstructVH_ = vectorHandlingAnalyticGeometry()
        
        # adding the interface curve first
        self.unstructVH_.push_back(self.interfaceUnstructBound_.clone())
        # adding the region bounds (the curves named parallel in the createLayerBounds function)
        for bound in self.hubUnstructBounds_ + self.shroudUnstructBounds_:
            self.unstructVH_.push_back(bound.clone())
        # adding the outlet boundary last
        self.unstructVH_.push_back(self.outletUnstructBound_.clone()) 

        logging.info("###   Creating Hub Layer Faces")
        # creatig the layer faces from the curves 
        self.hubLayers_ = []
        for i in range(len(self.hubLayerCurves_)):
            layer_vhc = vectorHandlingConstDtCurve()
            # pushing the individual curves in a vhc
            for n in range(len(self.hubLayerCurves_[i])):
                layer_vhc.push_back(analyticCurve.MustDownCast(self.hubLayerCurves_[i][n]).ptrDtCurve())
            # creating the faces by filling the curves
            self.hubLayers_.append(                                        
                analyticSurface(                                            
                    bSplineSurface_bSplineCurveFillConstructOCC(layer_vhc).result()
                )
            ) 
            del layer_vhc
        
        logging.info("###   Creating Shroud Layer Faces")
        self.shroudLayers_ = []
        for i in range(len(self.shroudLayerCurves_)):
            layer_vhc = vectorHandlingConstDtCurve()
            
            for n in range(len(self.shroudLayerCurves_[i])):
                layer_vhc.push_back(analyticCurve.MustDownCast(self.shroudLayerCurves_[i][n]).ptrDtCurve())
            
            self.shroudLayers_.append(                                        
                analyticSurface(                                            
                    bSplineSurface_bSplineCurveFillConstructOCC(layer_vhc).result()
                )
            )  
            del layer_vhc
         
    def createLayerBounds(self, layerCurve, inOutCurves, thickness, supports, lab):
        """Create boundary curves of the layer faces.
        
        This method:
            
            - Iterates over the wall curves and checks if they are on a radius of zero.
            - Iterates over curves and combines them if they have a steady transition.
            - Iterates over curves and creates boundary curves of the layer faces.
            - Returns the lists with layer bounds and boundary curves for the 
              unstructured region as well as a list telling which wall curves are on
              a radius of zero.
        
        Parameters
        ----------
        layerCurve: List[analyticGeometry]
          List of wall curves.
        inOutCurves: List[analyticGeometry]
          List with inlet and outlet curve.
        thickness: float
          Layer thickness.
        supports: List[float]
          List with positions of support points in percent along the span of the wall curve.
        lab: string
          label
        
        Returns
        -------
        returnBounds: List[List[analyticGeometry]]
          Boundary curves of each layer on the wall curves.
        on_rad_zero: List[Bool]
          Information for each wall curve, telling if the radius is on zero.
        layerParallel: List[analyticGeometry]
          Inner boundary curves of the layers, used for the multiple bounded volume.


        This method takes a list of wall curves (either hub or shroud) ``layerCurve``
        and the inlet and outlet curves ``inOutCurve`` as inputs. 
        Furthermore the layer thickness ``layer_thickness`` and the list of the positions
        of support points ``supports`` is handed over.
        ``supports`` specifies the number of support points and their positions on each 
        curve in ``layerCurve``. Each entry gives the percentual parameter span on which 
        a support point for the creatino of the third layer boundary curves is located.
        The label ``lab`` is used to name the created geometries.

        The following figure shows the hub curves of the draft tube example 
        in ``layerCurve`` (black) with their numbering in the list. ``layerCurve[2]`` 
        and the inlet curve ``inOutCurves[0]`` (red) are cut.
 
        .. _createLayerBounds0:
        .. figure:: meridionalFigs/createLayerBounds0.png
           :width: 60%
           :align: center

           Hub curves of the `draft tube cone` example in ``layerCurve`` (black) and
           inlet in ``boundsGlob[0]`` (red).
        
        A copy of ``inOutCurves`` is created and allocated to ``boundsGlob``. This list
        contains the global boundary curves , which will be part of the boundary curves 
        of the first and last layer faces. 
        
        The layer faces are created from four boundary curves. 
        The following empty lists for the layer boundary curves are prepared. The 
        numbering first, second, third and fourth corresponds to the definition given in 
        the documentation of the constructor :meth:`__init__`.

            - ``layerStreamOrtho``: List for the second and fourth boundary curves.
              Extending into the flow domain from the wall into the flow.
            
            - ``layerParallel``: List for the third boundary curves.
              Extending approximately parallel to the wall.
            
            - ``returnBounds``: List for all layer boundary curves.

            - ``on_rad_zero``: List tracking, which of the curves in ``layerCurve`` is on
              a radius of zero.

        In a first loop a check is performed, which of the curves in ``layerCurve`` are 
        located on a radius of zero. This check is performed with the method 
        :meth:`rz_xyz`, which takes a point as input and performs a coordinate 
        transformation from the carthesian space where the point is defined into a 
        cylindrical space with the origin point of ``origin_`` and a z-axis of 
        ``rotAxis_``. The method returns the radius and z-position of the point in its 
        cylindric-coordinates.

        The following figure shows the activity diagramm of the first loop.

        .. _createLayerBounds_activity0:
        .. figure:: meridionalFigs/createLayerBounds_activity0.png
           :width: 70%
           :align: center

           Activity diagram of the first loop in :meth:`createLayerBounds`.

        The check is performed on the radii of the start and end points of each curve 
        with the method `inXYZTolerance` of the class `analyticGeometry`. This method 
        returns ``True`` if the radius is within the tolerance of zero.
        
        If the check returns ``True`` for both points the curve is considered to be on
        a radius of zero. In this case the outlet curve in ``boundsGlob[-1]`` is replaced 
        with the current curve. This is done because this curve will form the bounding 
        curve for the last layer face on this wall.
        The last entry of the list ``on_rad_zero`` is set as ``True``.

        A curve is created extending from the layer thickness to the end of the curve on
        radius zero. This curve is appointed to the variable ``unstructOnRotAxis``. It is
        needed as bounding curve of the unstructured region.

        The curve is removed from the list of layer curves and the loop is broken.

        If the curve is not on a radius of zero, the list ``on_rad_zero`` is appended 
        with a ``False`` value.
        
        The following figure shows the hub curves with their appointed Boolean values in
        ``on_rad_zero`` and the global layer boundaries in ``boundsGlob``.
        
        .. _createLayerBounds1:
        .. figure:: meridionalFigs/createLayerBounds1.png
           :width: 60%
           :align: center

           Hub curves of the `draft tube cone` example with their appointed Bool values
           in the ``on_rad_zero`` list. The curves which are part of the global layer
           boundaries are red.

        In the second loop curves who have a, within boundaries, steady transition in their
        connecting points are combined.
        The following figure shows the activity diagramm of this loop.

        .. _createLayerBounds_activity1:
        .. figure:: meridionalFigs/createLayerBounds_activity1.png
           :width: 70%
           :align: center

           Activity diagram of the second loop in :meth:`createLayerBounds`.

        In the loop the tangent vectors of the consecutive curves ``v0`` and ``v1`` are
        calculated at their transition points. These points are the end point of ``v0``
        and the start point of ``v1``.
        
        With the angle between the tangent vectors ``v0_firstDer`` and ``v1_firstDer``
        a condition for steadiness ``is_steady`` is formulated.
        This condition returns ``True`` if the angle change between the two curves is 
        less or equal of two degrees.

        If the condition is ``True`` the curves are combined and allocated to the 
        variable ``current_curve``, which is used in the next iteration. Furthermore the
        flag in the ``on_rad_zero`` list of the curve is deleted.

        If the condition is not ``True`` the curve is appended to the list ``speCurve``.
        The consecutive curve is set as ``current curve``

        After the loop has finished the curves which get a layer are stored in 
        ``speCurve``.
        
        The bounding curves for the layers are created in the third loop, iterating over
        ``speCurve``. The following figure shows the activity diagram of this loop.

        .. _createLayerBounds_activity2:
        .. figure:: meridionalFigs/createLayerBounds_activity2.png
           :width: 100%
           :align: center

           Activity diagram of the third loop in :meth:`createLayerBounds`.
           Orange action block are part of the creation of the second and fourth boundary 
           curves, blue action blocks are part of the creation of the third boundary 
           curves.

        Three conditions can apply for each curve in the loop. 

        1. **The first layer face along the wall**

            The case ``i == 0`` applies.
            
            The fourth boundary curve of the first layer face is formed by the 
            inlet curve in ``boundsGlob[0]``. The boundary is created by trimming the curve 
            from the shared point with the layer curve ``speCurve[i]`` to the layer thickness 
            specified in ``layer_thickness``. The connection point has to be at 0 percent of 
            the parameter span of the global bounding curve. Depending on if the hub or the 
            shroud wall layers are created in this method this is not always the case.

            That the start points are the same is ensured by computing the distance between 
            the start points of both curves and checking if they are within the 
            tolerance. The distance between the points is calculated with the method `distance` of the
            `dtOO` class `dtLinearAlgebra`. The tolerance is checked with the class 
            `analyticGeometry.inXYZTolerance`.
            If the distance is out of tolerance the global bounding curve is reversed.
            
            The trimmed curve is appended to the list ``layerStreamOrtho``.

        2. **The last layer face along the wall**
        
            The case ``i == len(speCurve)`` applies.
            
            The second bounding curve of the last layer face is formed by the second global boundary 
            ``boundsGlob[-1]``. This can be the outlet of the layered region or a curve on the
            radius of zero.

            The layer bound is created similar to case 1. Here the distance of the end point
            of the wall curve ``speCurve[i-1]`` and the start point of the global boundary 
            curve are used to orient the global boundary curve correctly.
            
            In this case the third layer boundary curve which extends parallel to the wall
            is created with the method :meth:`layerCurve`. It is appended to the list 
            ``layerParallel``.
            
            The list ``newLayer`` is created. This list contains the four bounding curves
            of the last layer face.

        3. **Regular layer face**

            The cases 1. and 2. do not apply.

            In this case the wall curve in each iteration is ``speCurve[i-1]``. For each 
            wall curve the second and third layer bound is created. The second bound is
            created directly in the loop, for the creation of the third layer bound the method 
            :meth:`layerCurve` is used. As the fourth layer bound the second layer bound of the 
            previous iteration is used.

            The second layer bound is created extending in the mean normal direction
            :math:`\\mathbf{v_{mean}}` between the curves ``speCurve[i-1]`` and 
            ``speCurve[i]`` at the shared point :math:`P_0`. The curves are allocated to the 
            variables ``v0`` and ``v1``. The mean normal direction is calculated as follows.
        
            .. math::

               \\mathbf{v_{mean}} = \\frac{\\mathbf{n_0} + \\mathbf{n_1}}{\\|\\mathbf{n_0} + \\mathbf{n_1}\\|}

            The normal directions of the curves (:math:`\\mathbf{n_{0}}` and :math:`\\mathbf{n_{1}}`) at 
            the shared point are calculated as a cross product of the normal axis of the
            special channel's cross section ``normalAxis_`` (:math:`\\mathbf{n_{global}}`) and the 
            curve's tangential direction at the shared points (:math:`\\mathbf{t_{0}}` and 
            :math:`\\mathbf{t_{1}}`)

            .. math::
               
               \\mathbf{n} = \\frac{\\mathbf{t} \\times \\mathbf{n_{global}}}{\\|\\mathbf{t} \\times \\mathbf{n_{global}}\\|}

            The resulting vector :math:`\\mathbf{v_{mean}}` is allocated to the variable 
            name ``layerVec``.
            The following figure shows the generation of the second layer bounds.

            .. _createLayerBounds2:
            .. figure:: meridionalFigs/createLayerBounds2.png
               :width: 70%
               :align: center

               Creation of the second and fourth layer bounds (blue) extending from the wall 
               curves into the flow domain.

            The vector ``insideVec`` is created by subtracting the global center point 
            ``speCenter_`` from the shared point between the two wall curves. This results 
            in a vector which has a direction inside of the flow domain.

            With the normal vector ``normalVec`` of curve ``v0`` at the shared point, the 
            thickness (:math:`t_{Layer}`) of the layer bound is set. By calculating the dot 
            product of the vectors ``normalVec`` and ``insidVec`` it can be checked if 
            ``normalVec`` points inside or outside of the flow domain. If the result is 
            negative the direction of ``normalVec`` is reversed by multiplying it with ``-1``.

            The length of the layer bound is calculated from the length of ``normalVec`` and 
            the angle between ``normalVec`` and ``layerVec`` (see :numref:`createLayerBounds2`).

            The layer bound is created between the points :math:`P_0` and :math:`P_1`.
            :math:`P_1` is calculated from the point :math:`P_0` plus the direction of 
            ``layerVec`` (:math:`\\mathbf{v_{mean}}`) times the defined length.

            .. math::

               P_1 = P_0 + \\mathbf{v_{mean}} * t_{Layer} / cos(\lambda)

            The resulting curve is stored in the list ``layerStreamOtho``.
            
            The third boundary curve is created with the method :meth:`layerCurve`.
            The method takes the list ``layerStreamOrtho``, the current wall curve
            ``v0`` and the number of the current iteration ``i`` as inputs.
            The generation of the curve is controlled by the layer thickeness 
            ``thickness`` and the support point positions defined in ``supports``.
            The returned curve ``ext`` is stored in the list ``layerParallel``.
            The creation of the third layer bound is shown in the following figure.

            .. _createLayerBounds3:
            .. figure:: meridionalFigs/createLayerBounds3.png
               :width: 75%
               :align: center

               Creation of the third layer bound (blue) extending inside the 
               flow domain.

           The boundary curves of the current layer are stored in the list 
           ``newLayer``. In this list the boundary curves have entry numbers 
           according to their numbering in this documentation, with the wall curve 
           ``v0`` forming the first entry.
           In the second entry the curve in ``layerStreamOrtho`` of the current iteration 
           is stored. In the last entry the curve in ``layerStreamOrtho`` of the previous 
           iteration is stored.
           The third entry is filled with the curve in ``layerParallel``.

           The following figure shows the layer faces surrounded by their layer bounds
           with their directions. For the first layer face the numbering of the curves
           is shown.

            .. _createLayerBounds4:
            .. figure:: meridionalFigs/createLayerBounds4.png
               :width: 75%
               :align: center

               All layer bounds surrounding the layer face (blue).
            
            The list is appended to the list ``returnBounds``.
        
        After the loop has concluded, the curve ``unstructOnRotAxis`` is appended to the
        list ``layerParallel``, if it exists.

        The method returns the lists ``returnBounds``, ``on_rad_zero`` and ``layerParallel``.
        """
        
        #
        # preparing lists
        #

        # global boundaries of the flow domain
        boundsGlob = inOutCurves.copy()
        
        # second and fourth layer boundaries between the special curves
        # will extend from the channel curves inside the domain, orthogonally to streamwise direction
        layerStreamOrtho = []
        
        # third layer boundaries
        # layerboundaries which will be parallel to channel curves
        layerParallel = []

        # list for return of the boundaries (will be explained later)
        returnBounds = []
        
        # list which tracks if the layer will end on a radius of zero 
        # will be used for mesh generation later
        on_rad_zero = []

        unstructOnRotAxis = None
        
        #
        # first loop
        # iterating over all curves and deciding which curves get Layers
        #
        for i in range(len(layerCurve)):
            #
            # checking if the curve has a radius of zero
            #
            rz_0 = self.rz_xyz(layerCurve[i].getPointPercent(0.0))
            rz_1 = self.rz_xyz(layerCurve[i].getPointPercent(1.0))
            onRotAxis_0 = analyticGeometry.inXYZTolerance(rz_0[0])
            onRotAxis_1 = analyticGeometry.inXYZTolerance(rz_1[0])
            logging.debug(
                "speHub: |<- %5.2f --- %5.2f ->|" % (rz_0[0], rz_1[0])
            )
            #
            # if the radius is zero the curve will be assigned as the second global boundary
            # the layers will end here in streamwise direction
            #
            if onRotAxis_0 and onRotAxis_1:                    
                
                # overwrites the outlet curve as global boundary
                boundsGlob[-1] = layerCurve[i]
                logging.info("Set %s curve no. %i as the global layer boundary" % (lab, i))
                on_rad_zero[-1] = True
                
                # saving the segment of the curve from the thickness to the maximum 
                #  will later be a curve for the segment of the unstructured mesh
                lc = analyticCurve.MustDownCast(layerCurve[i]).ptrDtCurve()
                unstructOnRotAxis = analyticCurve(
                    trimmedCurve_uBounds(
                        lc, lc.u_l(thickness), lc.getUMax()
                    ).result()
                )

                del layerCurve[i]
                break

            #
            # else False is appended to on_rad_zero
            #
            else:
                on_rad_zero.append(False)
        
        #
        # second loop
        # iterating over curves and deciding between which curves a layerStreamOrtho curve 
        # should be created and which curves should be combined
        #
        speCurve = []
        current_curve = analyticCurve.MustDownCast(layerCurve[0])

        # iterating over the curves
        # if defined curve segments have a steady transition and are not created as a split
        #  the curves will be combined and no layer boundary is defined between them
        for i in range(len(layerCurve) - 1):

            v0 = analyticCurve.MustDownCast(layerCurve[i])
            v1 = analyticCurve.MustDownCast(layerCurve[i + 1])

            v0_firstDer = dtLinearAlgebra.normalize(v0.firstDerUPercent(1))
            v1_firstDer = dtLinearAlgebra.normalize(v1.firstDerUPercent(0))

            angle = dtLinearAlgebra.angle(v0_firstDer, v1_firstDer)
            
            # condition for combining the curves:
            is_steady = (angle <= 2*(numpy.pi/180))
            
            # if the condition is fulfilled the curves will be combined
            # no splits will be made here
            if is_steady:
                current_curve = analyticCurve(
                    bSplineCurve_curveConnectConstructOCC(
                        current_curve.ptrDtCurve(),
                        v1.ptrDtCurve()
                    ).result()
                )
                del on_rad_zero[i]
                logging.info("Combining curves %i and %i to make one layer" % (i, i+1))
            # else the curves will be appended
            else:
                speCurve.append(current_curve)
                current_curve = v1

        # appending the last curve outside of the loop
        # speCurve will be used in the rest of this method
        speCurve.append(current_curve)
        logging.info("Creating %i Layers on %s" % (len(speCurve), lab))
        
        #
        # third loop
        # iterate over all curves and create the bounding curves
        #
        for i in range(len(speCurve)+1):
            
            # the first layer boundary is built from the first global boundary
            #  (the last interface in streamwise direction)
            if i == 0:
                # check if the direction of the interface is correct
                #  if its not correct, the curve will be reversed for trimming
                base = analyticCurve.MustDownCast(speCurve[i]).ptrDtCurve()  
                dc = analyticCurve.MustDownCast(boundsGlob[0]).ptrDtCurve()
                distance = dtLinearAlgebra.distance(
                    base.pointPercent(0.0), dc.pointPercent(0.0)
                )
                if not analyticGeometry.inXYZTolerance(distance):                  
                    logging.debug("Reverse direction. distance = %f" % (distance)) 
                    dc = geomCurve_curveReverseConstructOCC(dc, True).result()
                    direction = -1.0          
                
                # trimms the global boundary at the thickness of the layer
                layerStreamOrtho.append(
                    analyticCurve(
                        trimmedCurve_uBounds(
                            dc, dc.getUMin(), dc.u_l(thickness) 
                        ).result()
                    )
                )
                logging.info("Created orthogonal layer boundary no. %i" % i)
            
            # the last layer boundary is built from the second global boundary
            elif i == len(speCurve):
                base = analyticCurve.MustDownCast(speCurve[i-1]).ptrDtCurve()  
                dc = analyticCurve.MustDownCast(boundsGlob[-1]).ptrDtCurve()
                distance = dtLinearAlgebra.distance(
                    base.pointPercent(1.0), dc.pointPercent(0.0)
                )
                if not analyticGeometry.inXYZTolerance(distance):                  
                    logging.debug("Reverse direction. distance = %f" % (distance)) 
                    dc = geomCurve_curveReverseConstructOCC(dc, True).result()
                    direction = -1.0
                
                layerStreamOrtho.append(
                    analyticCurve(
                        trimmedCurve_uBounds(  
                            dc, dc.getUMin(), dc.u_l(thickness)          
                        ).result()
                    )
                )
                logging.info("Created orthogonal layer boundary no. %i" % i)
                # calculates the parallel curve to the channel curve
                v0 = analyticCurve.MustDownCast(speCurve[i-1])
                ext = self.layerCurve(
                        layerStreamOrtho, 
                        i,
                        v0, 
                        thickness, 
                        supports,
                        lab
                )   
                layerParallel.append(ext)
                logging.info("Created parallel layer boundary on curve no. %i" % (i-1))

                # appends the boundary curves for this boundary to the return list
                newLayer = [
                        v0.clone(), 
                        layerStreamOrtho[i].clone(), 
                        layerParallel[i-1].clone(), 
                        layerStreamOrtho[i-1].clone()
                    ]
                returnBounds.append(newLayer)

 
            # for the curves inbetween the global boundrries the layers curves othogonal to the 
            #  streamwise direction have to be calculated
            else:
                # get the two curves which are in sequence to each other
                v0 = analyticCurve.MustDownCast(speCurve[i-1])
                v1 = analyticCurve.MustDownCast(speCurve[i])
                
                # calculates the resulting vector of the gradients at the intersect point
                # WARNING: this will retun the wrong vector if both curves have the same gradient 
                #layerVec = dtLinearAlgebra.normalize(
                #    dtLinearAlgebra.normalize(v0.firstDerUPercent(1.0))
                #    + dtLinearAlgebra.normalize(v1.firstDerUPercent(0.0))*(-1)
                #)

                # calculates the direction of the layer curve as the sum of the normals 
                #  of the curves at the intersect point
                layerVec = dtLinearAlgebra.normalize(
                        dtLinearAlgebra.normalize(
                            dtLinearAlgebra.crossProduct(
                                v0.firstDerUPercent(1), self.normalAxis_
                            )
                        )
                        + dtLinearAlgebra.normalize(
                            dtLinearAlgebra.crossProduct(
                                v1.firstDerUPercent(0), self.normalAxis_
                            )
                        )
                    )

                # this vector connects the start point of the layer curve 
                #  with the center of the bounding box of the special channel
                # it will be used to check if the layer curve points inside the domain
                insideVec = dtVector3(
                    analyticCurve.MustDownCast(v0).getPointPercent(1) - self.speCenter_
                )
                
                # calculating a normalvector on the origin point of the layer 
                # this vector will have the length of the layer thickness
                normalVec = dtLinearAlgebra.normalize(
                    dtLinearAlgebra.crossProduct(
                        v0.firstDerUPercent(1), self.normalAxis_
                    )
                ) * thickness
                
                # comparing the vectors 
                # if the dot product is smaller than zero the normalvector 
                #  points out of the domain and has to be reversed (times -1)
                direction = 1
                if dtLinearAlgebra.dotProduct(normalVec, insideVec) < 0:
                    direction = -1
                    normalVec = normalVec*direction
                
                # extracting the length of the layer boundary 
                #  so the thickness of the layer is consistent with the normal vector
                angle = dtLinearAlgebra.angle(layerVec, normalVec)
                length = dtLinearAlgebra.length(normalVec)/numpy.cos(angle)
                
                # creating the layer boundary curve 
                # the lenth of the curve is set so the thickens is consistent at the 
                #  normal at 100 percent of the curve
                layerStreamOrtho.append(
                    analyticCurve(
                        bSplineCurve_pointConstructOCC(
                            vectorDtPoint3()
                            << analyticCurve.MustDownCast(v0).getPointPercent(1)
                            << analyticCurve.MustDownCast(v0).getPointPercent(1)+(layerVec * length),
                            1
                        ).result()
                    )
                )
                logging.info("Created orthogonal layer boundary no. %i" % i)

                # creating the layer curve parallel to the channel curve
                ext = self.layerCurve(layerStreamOrtho, i, v0, thickness, supports, lab)
                layerParallel.append(ext)
                logging.info("Created parallel layer boundary on curve no. %i" % i)

                # appending the curves for return
                # the curves of one layer are appended in clockwise direction
                newLayer = [
                        v0.clone(),                     # channel segment
                        layerStreamOrtho[i].clone(),    # ortho0
                        layerParallel[i-1].clone(),     # parallel to channel
                        layerStreamOrtho[i-1].clone()   # ortho1
                    ]
                returnBounds.append(newLayer)
                logging.info("Finished creating %s layer curves" % lab) 
            
        # appending the curve on radius 0 if it exists
        # the parallel boundaries and the radius 0 curve are needed as boundaries for the
        #  unstructured mesh
        if unstructOnRotAxis != None:
            layerParallel.append(unstructOnRotAxis)
        
        return returnBounds, on_rad_zero, layerParallel 

    # creates the layer curve parallel to the channel curves
    # the curve is created from the two points at 100 percent of the matching 
    #  curves orthogonally to the streamwise direction and offsets from the points 
    #  of the channel curve at the support points
    def layerCurve(self, layerStreamOrtho, i, curve, thickness, supports, lab):
        """Create boundary curve on the hub and shroud layer curves.

        This method:

            - Creates third layer bounding curve from the end points of the second and
              third bounding curves and support points.

        Parameters
        ----------
        layerStreamOrtho: List[analyticGeometry]
          boundary curves the layer orthogonal to the wall curve
        i: int
          iterator, iD of the wall curve
        curve: analyticGeometry
          wall curve
        thickness: float
          layer thickness
        supports: List[float]
          List with positions of support points in percent along wall curve
        lab: string
          label

        Returns
        -------
        ext: analyticGeometry
          created layer curve inside the layer region


        The inputs to this method are the list of second and third layer bounding curves
        ``layerStreamOrtho``, the wall curve ``curve`` for which the third bounding curve
        is created and the iterator ``i`` of this curve in the loop in 
        :meth:`createLayerBound`. The creation of the bounding curve is controled with 
        the layer thickness ``thickness`` and the number and position of support points 
        specified in ``supports``.

        The following figure shows the creation of the third layer bound.

        .. _layerCurve:
        .. figure:: meridionalFigs/layerCurve.png
           :width: 60%
           :align: center

           Creation of the third layer bound (blue) with the method :meth:`layerCurve`.

        The iterator is used to return the second and third bounding curve of the current 
        layer from ``layerStreamOrtho``. The curves are allocated to the variables 
        ``bound0`` and ``bound1``.

        With the method :meth:`calculateNormalAxis` the center of the bounding box 
        ``layerCenter`` of the two bounds and the curve is calculated. 
        This point is used to create the vector ``insideVec`` which points in the 
        direction from a point on 50% of the parameter range of ``curve`` inside the
        layer face.

        The vector ``normalVec`` is created to point in the normal axis of the wall curve
        at 50% of the its span. It is calculated from the cross product
        of the curve's tangent at this point and the global normal axis ``normaAxis_``.

        With the dot product of ``normalVec`` and ``insideVec`` the offset direction from 
        the wall curve can be set with the variable ``direction`` to ``1`` or ``-1``. 

        A container object of the `dtOO` class `vectorDtPoint3` is created. The end point
        of the fourth layer bounding curve ``bound0`` is appended to the container.

        By iterating over ``supports`` the support points :math:`P_s` are created. Each 
        entry of ``supports`` contains a float value. Based on the float value a base 
        point :math:`P_0` on the wall curve parameter range is created 
        ``curve0.pointPercent(support)``.

        The support point is calculated by offseting :math:`P_0` into the normal 
        direction of the wall curve at the :math:`P_0`.
        The normal direction is defined as the cross product between the tangent 
        direction :math:`\\mathbf{t}` in :math:`P_0` and the global normal axis 
        ``normalAxis_`` (:math:`\\mathbf{n_{global}}`).
        With ``layer_thickness`` (:math:`t_{Layer}`) the offset length is set. The value 
        of ``direction`` (:math:`k`) is used to ensure, that the support points are 
        inside the flow domain.

        The following formula shows the calculation of a support point.

        .. math::

           P_s = P_0 + \\frac{\\mathbf{t} \\times \\mathbf{n_{global}}}{\\|\\mathbf{t} \\times \\mathbf{n_{global}}\\|} * t_{Layer} * k
        
        The support points are pushed into the point container.

        After all the support points are calculated, the end point of the second layer
        bound ``bound1`` is pushed into the container.

        The third layer bound is created from as a `bSplineCurve_pointConstructOCC` 
        object from the points in ``container``.
        The method returns this curve.
        """
        logging.info("creating layer curve no. %i" % i) 
        # finding the correct curves for the layer
        # bound is the curve orthogonal to the streamwise direction
        bound0 = analyticCurve.MustDownCast(layerStreamOrtho[i-1]).ptrDtCurve()
        bound1 = analyticCurve.MustDownCast(layerStreamOrtho[i]).ptrDtCurve()
        curve0 = analyticCurve.MustDownCast(curve).ptrDtCurve()
        

        # setting the direction of the offset by checking the direction of the normalvector
        # as insideVector the centerpoint of the layer is used
        layer_list = [layerStreamOrtho[i-1], layerStreamOrtho[i], curve]
        layerBox, layerCenter, unusedBb = self.calculateNormalAxis(layer_list)
        insideVec = dtVector3(
            curve.getPointPercent(0.5) - layerCenter
        )
        normalVec = dtLinearAlgebra.crossProduct(
            curve0.firstDerPercent(0.5), self.normalAxis_
        )
        direction = 1
        if dtLinearAlgebra.dotProduct(normalVec, insideVec) < 0:
            logging.info("changing offset direction")
            direction = -1
                
        # Create point container;
        # first and last points are set as the 100 percent points of the orthogonal curves
        # the points inbetween are created from an offset of the support points of the channel curves
        container = vectorDtPoint3()
        # first point
        container = container << bound0.pointPercent(1.0)
        # support points
        for support in supports:                                    
            container = container << (
                curve0.pointPercent(support)   
                + dtLinearAlgebra.normalize(
                    dtLinearAlgebra.crossProduct(
                        curve0.firstDerPercent(support), self.normalAxis_
                    )
                )
                * thickness
                * direction
            )

            ## debug statement to plot the vectors from the channel wall to the parallels
            #self.appendAnalyticGeometry(
            #    analyticCurve(
            #        bSplineCurve_pointConstructOCC(
            #            vectorDtPoint3()
            #              << curve0.pointPercent(support)
            #              << (curve0.pointPercent(support)
            #                    + dtLinearAlgebra.normalize(
            #                        dtLinearAlgebra.crossProduct(
            #                            curve0.firstDerPercent(support), 
            #                            self.normalAxis_
            #                        )
            #                    )
            #                * thickness * direction
            #                ),
            #              1
            #        ).result()
            #    ),
            #    "debug_supportVector_"+lab+str(i)+"_"+str(support)
            #)
        # last point
        container = container << bound1.pointPercent(1.0)          

        # creating the curve from the point container
        ext = analyticCurve(
                bSplineCurve_pointConstructOCC(container, 2).result()
            )

        return ext

    def rz_xyz(self, pp: dtPoint3) -> dtPoint2:
        """Transform a point in cartesian coordinates into cylindirc coordinates.

        This method:

            - Transforms a point in the xyz-coordinates into cylindric coordinates.
            - Returns radius and z-position of the point.

        Parameters
        ----------
        pp: dtPoint3
          Point in xyz-coordinates.

        Returns
        -------
        : dtPoint2
          dtPoint2 object containing the radius and z-position of pp

        The point ``pp`` in carthesian coordinates are reparametrized in cylindirc 
        coordinate based on the origin point ``origin_`` and the rotational vector 
        ``rotVector_``. 

        Returns the radius ``rr`` and the z-coordinate ``zz`` in a `dtPoint2` 
        object.
        
        """
        vv = pp - self.origin_
        zz = dtLinearAlgebra.dotProduct(vv, self.rotVector_)
        rr = dtLinearAlgebra.length(vv - self.rotVector_ * zz)
        return dtPoint2(rr, zz)

    #
    # calculate the normal axis of the whole domain 
    #
    @staticmethod
    def calculateNormalAxis(curves):
        """Calculate the bounding box and normal axis of curves in one plane.
        
        This method:
            
            - Creates a bounding box around the curves.
            - Calculates the normal axis on the bounding box.
            - Calculates the center of the bounding box.
            - Returns normal axis, center and bounding box.

        Parameters
        ----------
        curves: List[analyticGeometry]
          List of curves.

        Returns
        -------
        normalAxis: dtVector3
          Normal axis of the bounding box.
        bbCenter: dtPoint3
          Center point of the bounding box.
        bb: pairDtPoint3
          Bounding box points.

        """

        vv = vectorDtPoint3()                 
        for curve in curves:
            vv = vv << curve.getPointPercent(0.0) << curve.getPointPercent(1.0) 
        # calculates a bounding box of all the start and endpoints of the curves
        bb = dtLinearAlgebra.boundingBox(vv)                                    
        dist = bb[0] - bb[1]
        logging.info("Create bounding box from %d points." % vv.size())
        logging.debug(
            "  bb[0] : %5.2f %5.2f %5.2f" % (bb[0][0], bb[0][1], bb[0][2])
        )
        logging.debug(
            "  bb[1] : %5.2f %5.2f %5.2f" % (bb[1][0], bb[1][1], bb[1][2])
        )
        logging.info(
            "  dist  : %5.2f %5.2f %5.2f" % (dist[0], dist[1], dist[2])
        )
        # calculates the normal axis
        normalAxis = dtVector3(
            float(analyticGeometry.inXYZTolerance(dist[0])),
            float(analyticGeometry.inXYZTolerance(dist[1])),
            float(analyticGeometry.inXYZTolerance(dist[2])),
        )
        logging.info(
            "  -> normalAxis = (%5.2f %5.2f %5.2f)"
            % (normalAxis[0], normalAxis[1], normalAxis[2])     # normal axis to bounding box
        )
        
        # calculationg center point of bounding box
        bbCenter = dtPoint3(
            (bb[0][0]+bb[1][0])*0.5,
            (bb[0][1]+bb[1][1])*0.5,
            (bb[0][2]+bb[1][2])*0.5,
        )
        logging.info("Centerpoint : [%f, %f, %f]" % (bbCenter[0], bbCenter[1],bbCenter[2]))
        return normalAxis, bbCenter, bb
    
    # builder function 
    # here is specified which debug geometries will be plotted
    def build(self) -> None:
        """Build part.

        Parameters
        ----------
        None

        Returns
        -------
        None

        """
        logging.info("Building %s ..." % (self.label_))

        if self.debug():
            try:        
                ii = 0
                for layer in self.hubLayers_:   
                    self.appendAnalyticGeometry(
                        layer.clone(),
                        "debug_hubLayer_" + str(ii) + "_" + self.label_,
                    )
                    ii = ii + 1
            except:
                pass
            try:
                ii = 0
                for layer in self.shroudLayers_:
                    self.appendAnalyticGeometry(
                        layer.clone(),
                        "debug_shroudLayer_" + str(ii) + "_" + self.label_,
                    )
                    ii = ii + 1
            except:
                pass
            try:
                ii = 0
                for aCurve in self.unstructVH_:
                    self.appendAnalyticGeometry(
                        aCurve.clone(),
                        "debug_unstructBound_" + str(ii) + "_" + self.label_,
                    )
                    ii = ii+1
            except:
                pass
            try:
                for k in range(len(self.hubLayerCurves_)):
                    for c in range(len(self.hubLayerCurves_[k])) :
                        self.appendAnalyticGeometry(
                        self.hubLayerCurves_[k][c].clone(),
                        "debug_hubLayerCurve_" + str(k) + str(c) + "_" + self.label_,
                    )
            except:
                pass
            try:
                for k in range(len(self.shroudLayerCurves_)):
                    for c in range(len(self.shroudLayerCurves_[k])) :
                        self.appendAnalyticGeometry(
                        self.shroudLayerCurves_[k][c].clone(),
                        "debug_shroudLayerCurve_" + str(k) + str(c) + "_" + self.label_,
                    )
            except:
                pass
            try:
                ii = 0
                # defines which channels will be rotatetd for the debug geometries
                for channel in (                
                    self.hubLayers_
                    + self.shroudLayers_
                ):
                    off = float(ii) * 0.10      # rotational offset of each channel
                    self.appendAnalyticGeometry(
                        partRotatingMap2dTo3d(
                            self.rotVector_,
                            channel,
                            off - 0.05,         # min segment in percent 
                            off + 0.05,         # max segment in percent
                        ).clone(),
                        "debug_layer_" + str(ii) + "_" + self.label_,
                    )
                    ii = ii + 1
            except:
                pass
        return
    
    # returns a list containing the hub and shroud layers as well as information 
    #  on what kind of layer it is
    def getLayerList(
            self, 
            nSlices: int
        ) -> List[
                List[
                    List[analyticGeometry] | List[bool]
                ]
            ]:      
        """Return function for the six sided Layers.

        Parameters
        ----------
        nSlices: int
          number of rotationally periodic slices

        Returns
        -------
        layerList: List[List[List[analyticGeometry] | List[bool]]]
          List containing the following data for the hub and shroud layers:
            
            - 3d Layer region
            
            - bool list with information of the radius
        
        """

        logging.info("Request Layer Volumes: %i Slices" % nSlices)

        
        # partRotatingMap2dTo3d takes the rotation angle in %
        # ofc
        hubLayer3d = []
        for layer in self.hubLayers_:
            hubLayer3d.append(
                    partRotatingMap2dTo3d(
                        self.rotVector_, 
                        layer,
                        0.00,
                        (1/nSlices),
                    ).clone()
                )
        shroudLayer3d = []
        for layer in self.shroudLayers_:
            shroudLayer3d.append(
                    partRotatingMap2dTo3d(
                        self.rotVector_, 
                        layer,
                        0.00,
                        (1/nSlices),
                    ).clone()
                )
        
        # returns layer data in the following nested list:
        # layers = [[hub layer lists],[shroud layer list]]
        # with:
        # [hub layer lists] = [[3d layer domain], [bool list radius zero]]
        layerList = [
                [hubLayer3d, self.hubRadZero_],
                [shroudLayer3d, self.shroudRadZero_]
                ]
   
        return layerList
    
    # returns a multiple bounded volume of the unstructured region
    def getUnstructuredRegion(self, nSlices: int) -> Tuple[analyticGeometry, List[analyticGeometry]]: 
        """Return function for the multiple bounded Volume.

        Parameters
        ----------
        nSlices: int
          number of rotationally periodic slices

        Returns
        -------
        multBoundedVol: multipleBoundedVolume
          unstructred region between hub and shroud layers
        boundSurf: vectorHandlingAnalyticGeometry
          List with boundary surfaces of the multiple bounded volume
        """


        logging.info("Request Volume of unstructured Region: %i Slices" % nSlices)

        # rotation angle defined by the number of slices
        rotAngle = 2*numpy.pi/nSlices
        
        vv = vectorDtPoint3()
        boundSurf = vectorHandlingAnalyticGeometry()
        # iterating over the boundary curves of the unstructured region
        #  extracts the start and end points of all curves for a bounding box
        #  creates surfaces of the boundary curves by rotationg them
        ib = 0
        for bound in self.unstructVH_:
            
            # rotating the boundary curves, pushing them in boundSurf
            bs = analyticSurface(
                    rectangularTrimmedSurface_curveRotateConstructOCC(
                        analyticCurve.MustDownCast(bound).ptrDtCurve(),
                        self.origin_,
                        self.rotVector_,
                        rotAngle
                    ).result()
                )
            # setting labels for the interface and the outlet surface
            if ib == 0:
                bs.setLabel("interface_unstruct")
            elif ib == (len(self.unstructVH_)-1):
                bs.setLabel("outlet_unstruct")
            else:
                bs.setLabel("para"+str(ib))

            if not bs.degenerated():
                boundSurf.push_back(bs.clone())
            else:
                logging.info("Degenerated MBV boundary: %s " % bs.getLabel())

            ib = ib + 1
        
        # building a bounding box surface for the 2d unstructured region
        # using the bounding box of the special hub and shroud curves
        unstructBB = self.speBb_
        BB_dist = unstructBB[0] - unstructBB[1]  

        # creating a rectangular surface which extends over the bounding box by 0.1
        P0 = dtPoint3(unstructBB[0][0] - 0.1, unstructBB[0][1], unstructBB[0][2] - 0.1)
        P1 = dtPoint3(P0[0] + BB_dist[0]+0.2, 0.00, P0[2])
        P2 = dtPoint3(P1[0], 0.00, P1[2] + BB_dist[2]+0.2)
        P3 = dtPoint3(P0[0], 0.00, P2[2])
        m2d = analyticSurface(
                bSplineSurface_bSplineCurveFillConstructOCC(
                bSplineCurve_pointConstructOCC(P0, P1).result(),
                bSplineCurve_pointConstructOCC(P1, P2 ).result(),
                bSplineCurve_pointConstructOCC(P2, P3).result(),
                bSplineCurve_pointConstructOCC(P3, P0).result()
            ).result()
        )
        
        # creating the first multi bounded surface  and pushing it into the VH
        mbs1 = multipleBoundedSurface(m2d, self.unstructVH_)
        mbs1.setLabel("periodicUnstruct_0")
        boundSurf.push_back( mbs1.clone())
        
        # initializing a rotation dtTransformer
        cfg = jsonPrimitive()
        cfg.appendDtPoint3("_origin", self.origin_)
        cfg.appendDtVector3("_rotVector", self.rotVector_)
        cfg.appendReal("_angle", rotAngle)
        
        # creating transformer object
        rot = rotate(cfg)
        
        # rotating the 2d bounding box
        m2d_rot = (rot.applyAnalyticGeometry(m2d))
        
        # rotating the boundary curves of the unstructured region
        unstructVH_rot = vectorHandlingAnalyticGeometry()
        for curve in self.unstructVH_:
            unstructVH_rot.push_back(rot.applyAnalyticGeometry(curve))
        
        # creating the rotated mbs and pushing it
        mbs2 = multipleBoundedSurface(m2d_rot, unstructVH_rot)
        mbs2.setLabel("periodicUnstruct_1")
        boundSurf.push_back( mbs2.clone())
 
        # generating the multiple bounded volume
        multBoundedVol = multipleBoundedVolume(infinityMap3dTo3d(), boundSurf)
        
        # adding the multi bounded surfaces to the vhs
        boundSurf.push_back(mbs1.clone())
        boundSurf.push_back(mbs2.clone())
       

        # returning the multi bounded volume and its boundary surfaces
        return multBoundedVol.clone(), boundSurf

