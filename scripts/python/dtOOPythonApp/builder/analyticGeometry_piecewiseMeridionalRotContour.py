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


class analyticGeometry_piecewiseMeridionalRotContour(dtBundleBuilder):
    """Create regular channels and special hub and shroud curves.

    This class:

        - Creates interface curves in the meridional contour.

        - Splits the hub and shroud curves at the intersection points
          with the interfaces.

        - Creates regular channels between the interfaces.

        - Creates lists of boundary curves for a layered flow channel
          downstream of the last interface.

        - Returns the volumes of the regular channels and the boundary curve lists.


    Attributes
    ----------
    label_ : str
        Label.

    rotVector_ : dtVector3
        Rotation vector.

    normalAxis_ : dtVector3
        Vector normal to the 2D domain.

    interfaces_ : List[analyticGeometry]
        List of interface curves.

    hubSplits_ : List[List[Tuple[float, int]]]
        List of splits on hub curves.
        Each entry represents:

        - ``hubSplits_[i][n]``: n-th split on the i-th hub curve.
        - ``hubSplits_[i][n][0]``: Relative position where the split is applied.
        - ``hubSplits_[i][n][1]``: ID of the interface creating the split.

    shroudSplits_ : List[List[Tuple[float, int]]]
        List of splits on shroud curves.
        Each entry represents:

        - ``shroudSplits_[i][n]``: n-th split on the i-th shroud curve.
        - ``shroudSplits_[i][n][0]``: Relative position where the split is applied.
        - ``shroudSplits_[i][n][1]``: ID of the interface creating the split.

    hubCurves_ : List[analyticGeometry]
        Split hub curves.

    shroudCurves_ : List[analyticGeometry]
        Split shroud curves.

    inOutCurves_ : List[analyticGeometry]
        List of inlet and outlet curves of the complete domain.

    regChannels_ : List[analyticGeometry]
        List of channel surfaces.

    speHub_ : List[analyticGeometry]
        List of hub curves of the layered region.

    speShroud_ : List[analyticGeometry]
        List of shroud curves of the layered region.

    inOutLayerReg_ : List[analyticGeometry]
        List of inlet and outlet curves of the layered region.


    Returns
    -------
    None

    Examples
    --------

    >>> import dtOOPythonSWIG as dtOO
    
    Define list of hub curves:

    >>> hubCurves = [
    ...      dtOO.analyticCurve(
    ...        dtOO.bSplineCurve_pointConstructOCC(
    ...          dtOO.vectorDtPoint3()
    ...            << dtOO.dtPoint3(+0.50, +0.00, 1.00)
    ...            << dtOO.dtPoint3(+0.00, +0.00, 0.50),
    ...            1
    ...        ).result()
    ...      ),
    ...      dtOO.analyticCurve(
    ...        dtOO.bSplineCurve_pointConstructOCC(
    ...          dtOO.vectorDtPoint3()
    ...            << dtOO.dtPoint3(+0.00, +0.00, 0.50)
    ...            << dtOO.dtPoint3(+0.00, +0.00, 0.00),
    ...            1
    ...        ).result()
    ...      )
    ...    ]
    
    Define list of shroud curves:

    >>> shroudCurves = [
    ...      dtOO.analyticCurve(
    ...        dtOO.bSplineCurve_pointConstructOCC(
    ...          dtOO.vectorDtPoint3()
    ...            << dtOO.dtPoint3(+1.00, +0.00, 1.00)
    ...            << dtOO.dtPoint3(+1.00, +0.00, 0.00),
    ...            1
    ...        ).result()
    ...      )
    ...    ]
    
    Initialize builder:

    >>> builder = analyticGeometry_piecewiseMeridionalRotContour(
    ...  label = "channel",
    ...  hubCurves = hubCurves, 
    ...  shroudCurves = shroudCurves,
    ...  layer_thickness = 0.2,
    ...  layer_supports = [0.33, 0.66],
    ...  interface_hub = [[0, 0.7]],
    ...  interface_shroud = [[0, 0.3]],
    ...  interface_curvature = [[0.5, 0.2, -1]],
    ... )
    
    Build volume:

    >>> builder.build()

    Return the six sided domain and check type:

    >>> builder.getRegChannel(0, 12).virtualClassName()
    'partRotatingMap2dTo3d'
   
     
    The main method of this class is the constructor.
    The inputs are the hub and shroud curves, as well as the lists
    defining the interface parameters.

    The normal axis ``normalAxis_`` of the flow channel's cross-section 
    is returned by the method :meth:`calculateNormalAxis`.
    It is used to determine the direction of the interface curves.

    The interface curves are created with the method
    :meth:`createInterface`. The method returns the curves in the
    list ``interfaces_``.
    With the method :meth:`detectIntersect`, it is checked whether the
    interface curves intersect the hub or shroud curves; if so, an
    exception is raised.

    Special inlet and outlet curves are created as straight lines at
    the inlet and outlet of the meridional contour.
    The list ``inOutCurves_`` contains these curves.

    The hub and shroud curves are split at the intersection points
    corresponding to the start or end points of the interface curves
    using :meth:`createSplits`.
    The lists ``hubSplits_`` and ``shroudSplits_`` track which curves
    are split by which interface and at which percentual position along
    their span.
    The method :meth:`createSplits` returns the split curves in the
    lists ``hubCurves_`` and ``shroudCurves_``.

    The newly split curves are managed with ``cti`` (curve-to-interface)
    lists.
    These lists are initially returned by :meth:`createSplits` and track
    which interface curve is associated with which hub and shroud curve.
    The lists are modified by
    :meth:`propagate_interface_ids_next` to indicate which hub and
    shroud curves belong to which regular channel.

    The regular channels are created between two neighbouring
    interfaces in the flow direction of the channel contour.
    The first regular channel is created between the inlet and the
    first interface.
    The following regular channels are created between neighbouring
    interfaces.

    The surfaces of the regular channels are created from the interface
    curves and the hub and shroud curves extending between them.
    These curves are identified with the method
    :meth:`findChannelCurves`.
    The surfaces are created in the constructor of this class and stored
    in the list ``regChannels_``.

    The hub and shroud curves that are not part of the regular channels
    are stored as special curves in the lists ``speHub_`` and
    ``speShroud_``.
    These curves later form the basis of a layered flow channel.
    The inlet and outlet curves for this layered flow channel are stored
    in the list ``inOutLayerReg_`` and are formed by the last interface
    curve and the outlet of the flow domain.

    The :meth:`build` method is used for debugging the class.
    If the debug option is enabled in the class constructor, the created
    geometries are returned to `ParaView`, where they can be plotted
    with the names assigned in the :meth:`build` method and the label
    ``label_``.

    The method :meth:`getRegChannel` returns the volume of a specified
    regular channel.
    The volume is created by rotating the face of a regular channel
    around the rotational axis ``rotVector_`` with a specified angle.

    The getter method :meth:`getLayerRegionCurves` returns the lists
    ``speHub_``, ``speShroud_``, and ``inOutLayerReg_``.
    """

    def __init__(
        self,
        label: str,
        hubCurves: List[analyticGeometry],
        shroudCurves: List[analyticGeometry],
        interface_hub: List[Tuple[int, float]] = [],             # [curve, percent]
        interface_shroud: List[Tuple[int, float]] = [],
        interface_curvature: List[Tuple[float, float, int]] = [],   # [curvature offset, percent, direction]
        rotVector: dtVector3 = dtVector3(0, 0, 1),
    ) -> None:
        """Constructor.
        
        This method:

            - Creates the interface curves.
            - Splits the hub and shroud curves at the interfaces.
            - Creates inlet and outlet lines.
            - Creates regular channel surfaces.
            - Creates lists with curves for layered region.

        Parameters
        ----------
        label : str
          Label.
        hubCurves : List[analyticGeometry]
          List of hub curves.
        shroudCurves : List[analyticGeometry]
          List of shroud curves.
        interface_hub : List[Tuple[int, float]]             
          Positions of the interfaces on the hub curves.
          Each entry represents:

            - ``interface_hub[i]``: Interface number
            - ``interface_hub[i][0]``: Curve number where the interface is located
            - ``interface_hub[i][1]``: Percentage along the curve (0 to 1)

        interface_shroud : List[Tuple[int, float]]             
          Positions of the interfaces on the shroud curves.
          Each entry represents:

            - ``interface_shroud[i]``: Interface number
            - ``interface_shroud[i][0]``: Curve number where the interface is located
            - ``interface_shroud[i][1]``: Percentage along the curve (0 to 1)
        
        interface_curvature : List[Tuple[float, float, int]]
          Curvature of the interface curve from hub to shroud.
          Each entry represents:

            - ``interface_curvature[i]``: Interface number
            - ``interface_curvature[i][0]``: Curvature offset point [%] from hub to shroud
            - ``interface_curvature[i][1]``: Curvature as a percentage of the connection line length
            - ``interface_curvature[i][2]``: Curvature direction

        rotVector: dtVector3
          Rotation vector.
        
        Returns
        -------
        None
        

        The constructor of this class is used to partition the hub and shroud curves into regular
        channel segments and special hub and shroud curves.

        The input lists named ``hubCurves`` and ``shroudCurves`` define the two-dimensional
        cross-section of the channel. The curves must be created such that their direction is
        consistent with the downstream direction of the flow machine. Furthermore, the ordering
        of the curves in the lists must also follow the downstream direction of the machine.

        The following figure shows the hub and shroud curves:

        .. _hsCurves0:
        .. figure:: meridionalFigs/hsCurve_noInterface.png
           :width: 50%
           :align: center

           Hub and shroud curves of the meridional channel.
           Numbering corresponds to the indices in the ``hubCurves`` and ``shroudCurves`` lists.

        The method :meth:`calculateNormalAxis` is used to compute the normal axis of the cross-section,
        stored in ``normalAxis_``.

        By passing ``hubCurves``, ``shroudCurves``, ``interface_hub``, ``interface_shroud``, and
        ``interface_curvature``, together with the computed normal axis, to :meth:`createInterface`,
        the interface curves are created. The method returns a list of interface curves, which is
        stored in ``interfaces_``. The index of each interface in this list also serves as its
        interface ID.

        The hub and shroud curves are split at their intersection points with the interface curves.
        For this purpose, the lists ``hubSplits_`` and ``shroudSplits_`` are created, where each entry
        is initialized as an empty list. The number of entries corresponds to the number of hub and
        shroud curves.

        By iterating over the interfaces, each split position together with the corresponding interface
        ID is stored as a tuple in the respective curve entry at the location where the split occurs.

        The resulting ``hubSplits_`` and ``shroudSplits_`` are of type ``List[List[Tuple[float, int]]]``.

        The values in this nested list represent:

        - ``hubSplits_[i][n]``: n-th split on the i-th hub curve.
        - ``hubSplits_[i][n][0]``: Percentual position along the curve span where the split occurs.
        - ``hubSplits_[i][n][1]``: ID of the interface that creates the split.

        The splits for the hub and the shroud curves are applied using the method
        :meth:`createSplits`. The hub and shroud curves are split independently by calling
        the method once for the hub curves and once for the shroud curves.

        The method takes the split data lists ``hubSplits_`` or ``shroudSplits_`` and the
        corresponding curve lists ``hubCurves`` or ``shroudCurves`` as input. Additionally,
        a label is passed to the method.

        The method returns the split hub and shroud curve lists as well as ``cti`` (curve-to-interface)
        lists. If no split is performed on a curve, or if the split position lies at 0 or 100 percent
        of the curve span, no new split is created and the original curve is appended as a copy.

        The resulting curve lists are stored as ``hubCurves_`` and ``shroudCurves_``.

        The following figure shows the meridional contour with interface, inlet, and outlet curves.
        The curves ``hubCurves[1]`` and ``shroudCurves[2]`` (see :numref:`hsCurves0`) are split into
        two curves each by the interface curve ``interfaces_[1]``. The resulting curves are stored as
        ``hubCurves_[1]`` and ``hubCurves_[2]`` as well as ``shroudCurves_[2]`` and ``shroudCurves_[3]``
        in the instantiated curve lists.

        .. _hsCurves01:
        .. figure:: meridionalFigs/hsCurves.png
           :width: 50%
           :align: center

           Meridional channel with interface (red) and inlet and outlet curves (orange).
           Numbering of the hub and shroud curves (black) corresponds to the indices in the
           ``hubCurves_`` and ``shroudCurves_`` lists.

        The ``cti`` lists (curve-to-interface) track which hub and shroud curves are upstream of which
        interface. The lists contain one entry per hub or shroud curve. If a curve endpoint lies on an
        interface curve, the corresponding entry stores the interface ID; otherwise the entry is ``None``.

        For the example shown in :numref:`hsCurves01`, the resulting ``cti`` lists are:

        ``hub_cti = [0, 1, None, None, None]``

        ``shroud_cti = [0, None, 1, None, None]``

        The ``cti`` lists are passed to the method :meth:`propagate_interface_ids_next`, where the entries
        are matched to the regular channels to which the curves belong. The returned values overwrite the
        original ``cti`` lists.

        The resulting ``shroud_cti`` list is:

        ``shroud_cti = [0, 1, 1, None, None]``
        
        In ``inOutCurves_``, the inlet and outlet curves of the meridional contour are stored.
        The inlet curve is created as a straight line between the start points of the first curves
        in the hub and shroud curve lists. The outlet curve is created in the same way, using the
        end points of the last hub and shroud curves.

        The number of regular channels corresponds to the number of defined interfaces. The first
        regular channel is created between the inlet curve and the first interface. Subsequent
        regular channels are created between adjacent interfaces in flow direction.

        The regular channel surfaces are created by iterating over the number of interfaces.

        For each regular channel, the hub and shroud curves belonging to that channel are determined
        using the method :meth:`findChannelCurves`. This method is called once for the hub curves
        and once for the shroud curves. The inputs are the instantiated curve lists ``hubCurves_``
        or ``shroudCurves_``, the ``cti`` lists ``hub_cti`` and ``shroud_cti``, and the ID of the
        regular channel, as well as a label. The curves belonging to a channel are identified by
        matching the channel ID against the ``cti`` list entries and are combined into a single curve.

        The upstream interface, or in the case of the first regular channel the inlet of the meridional
        channel, forms the inlet boundary of each regular channel. The downstream interface forms the
        outlet boundary.

        The surfaces are constructed using a ``bSplineSurface_bSplineCurveFillConstructOCC`` object
        from the inlet, outlet, and the combined hub and shroud curves of the regular channel.

        To ensure consistent orientation, the curves are passed to the construct object in the following
        order:

            1. combined hub curve
            2. inlet of the regular channel
            3. combined shroud curve
            4. outlet of the regular channel

        This results in a regular channel whose uvw-directions after rotation are:

            - u : circumferential direction
            - v : meridional direction
            - w : hub-to-shroud direction

        The regular channel surfaces are stored in the list ``regChannels_``. The following figure
        shows the surfaces of the regular channels.

        .. _regChannels:
        .. figure:: meridionalFigs/regChannels.png
           :width: 50%
           :align: center

           Regular channels (yellow, green) between the interfaces.
           Special hub and shroud curves are not part of the regular channels.

        Curves that are not part of the regular channels have a value of ``None`` in their ``cti`` list.
        By iterating over the ``cti`` list and checking for ``None`` values, the special hub and shroud
        curves are identified and stored in the lists ``speHub_`` and ``speShroud_``. These special curves
        are used for the layered flow channel.

        The inlet of the layered flow channel is formed by the last interface curve, and the outlet is
        formed by the outlet of the meridional channel. These curves are stored in the list ``inOutLayerReg_``.

        The following figure shows the special curves and the inlet and outlet of the layered flow channel
        contour.

        .. _speCurvesClass:
        .. figure:: meridionalFigs/speCurvesClass.png
           :width: 55%
           :align: center

           Special hub and shroud curves with numbering according to their positions in the
           ``speHub_`` and ``speShroud_`` lists.
           Inlet and outlet of the layered flow channel.
 
        """
        
        logging.info("Initializing %s ..." % (label))
        super(analyticGeometry_piecewiseMeridionalRotContour, self).__init__()
        self.label_ = label
        
        self.rotVector_ = dtLinearAlgebra.normalize(rotVector)
        
        #
        # Extract a bounding box from all start and end points of hub and
        # shroud curves; necessary for calculating the normal direction
        #
        self.normalAxis_, center, bb = self.calculateNormalAxis(            
            hubCurves + shroudCurves
        )
       
        logging.info("###   Creating Interfaces")
        
        #
        # Generate interface curves
        #
        self.interfaces_ = self.createInterface(hubCurves,
                                               shroudCurves,
                                               interface_hub,
                                               interface_shroud,
                                               interface_curvature,
                                               self.normalAxis_)
        
        logging.info("###   Preparing Splits")
        
        #
        # makes split lists with number of empty lists 
        #equal to the number of curves in hub and shroud
        #
        self.hubSplits_ = [[] for _ in range(len(hubCurves))] 
        self.shroudSplits_ = [[] for _ in range(len(shroudCurves))]
        
        #
        # The hub and shroud points of the interfaces are added to the lists of splits
        # they get the ID of the interface i as a descriptor
        # trough this the correct interfaces can be tracked later
        # the .sort function sorts the splits by their percentage along the curve
        # Format:
        #   self.hubSplits_ = List[List[Tuple[percentage, ID]]]
        #
        for i in range(len(interface_hub)):
            self.hubSplits_[interface_hub[i][0]].append((interface_hub[i][1],i))
            self.hubSplits_[i].sort()
            self.shroudSplits_[interface_shroud[i][0]].append((interface_shroud[i][1],i))
            self.shroudSplits_[i].sort()

        logging.info("###   Creating Hub Splits")
        #
        # Create splits of hub and shroud curves
        # hub_cti and shroud_cti (curve_to_interface) contains a datastructure list
        # Here is mapped which of the newly spilt curves are located upstream of an interface
        # a curve located upstream of an interface is marked with the interface ID the others with None
        #
        # the cti lists will be updated to map all the curves to the regular channels
        # every cut curve will be appointed the ID of its respective reg channel defined by its interface
        # None means that the curve is part of a layer channel
        #
        self.hubCurves_, hub_cti = self.createSplits(self.hubSplits_, hubCurves, "Hub")
        hub_cti = self.propagate_interface_ids_next(hub_cti, 'Hub')
        
        logging.info("###   Creating Shroud Splits")
        self.shroudCurves_, shroud_cti = self.createSplits(self.shroudSplits_, shroudCurves, "Shroud")
        shroud_cti = self.propagate_interface_ids_next(shroud_cti, 'Shroud')
        
        #
        # Add inlet and outlet curves
        #
        self.inOutCurves_ = []
        # The inlet curve is needed as a boundary for the first regular channel
        self.inOutCurves_.append(                                    
            analyticCurve(
                bSplineCurve_pointConstructOCC(
                    self.hubCurves_[0].getPointPercent(0),
                    self.shroudCurves_[0].getPointPercent(0),
                ).result()
            )
        )
        # The outlet curve will be needed for layer generation
        self.inOutCurves_.append(                                    
            analyticCurve(
                bSplineCurve_pointConstructOCC(
                    self.hubCurves_[-1].getPointPercent(1),
                    self.shroudCurves_[-1].getPointPercent(1),
                ).result()
            )
        )
        
        # Returning if no interfaces are defined
        if not self.interfaces_:
            logging.error("NO INTERFACES DEFINED")
            return

        logging.info("###   Creating Regular Channels") 
        #
        # Generating regular channels from the interfaces
        # here all the channels are defined trough the hub and shroud curves and the interface
        # the last interface builds the last regular channel
        # the regChannels are generated by creating a surface from 4 curves
        # these curves are the upsteam and downstream boundaries 
        # and the combined hub and shroud curves of the channel
        #
        self.regChannels_ = []
        #
        # interating through the interfaces 
        # (interface_hub and ..._shroud have to be the same size)
        #
        for ii in range(len(interface_hub)):
            logging.info("# Regular Channel[%i]" % ii)

            vhc = vectorHandlingConstDtCurve()
            #
            # adding the boundary curves in the succession:
            # hub -> inlet -> shroud -> outlet
            # this succession is important so the coordinate system will be as follows:
            # u -   circumferential
            # v --  meridional
            # w --- hub to shroud
            #

            # finding the hub curves which belong to the interface
            regCurve = self.findChannelCurves(self.hubCurves_, hub_cti, ii, 'Hub')            
            vhc.push_back(analyticCurve.MustDownCast(regCurve[0]).ptrDtCurve())
            
            # setting the inlet boundary for the regular channel
            if ii == 0:
                # the first regChannel gets the inlet as a boundary curve
                vhc.push_back(analyticCurve.MustDownCast(self.inOutCurves_[0]).ptrDtCurve())
            else:
                # the other regChannels get the previous interface as boundary
                vhc.push_back(analyticCurve.MustDownCast(self.interfaces_[ii-1]).ptrDtCurve())
            
            # finding the shroud curves which belong to the interface
            regCurve = self.findChannelCurves(self.shroudCurves_, shroud_cti, ii, 'Shroud')            
            vhc.push_back(analyticCurve.MustDownCast(regCurve[0]).ptrDtCurve())
            
            # the outlet boundary is always the current interface
            vhc.push_back(analyticCurve.MustDownCast(self.interfaces_[ii]).ptrDtCurve())
            
            #
            # generates the regular Channel by filling the four curves in vhc    
            #
            self.regChannels_.append(                                        
                analyticSurface(                                            
                    bSplineSurface_bSplineCurveFillConstructOCC(vhc).result()
                )
            )  
            del vhc
        logging.info("%d regular channels created." % len(self.regChannels_))
        
        logging.info("###   Preparing Layer Curves")
        #
        # checking in the cti which curves will get a layer and appending them to speHub, 
        #
        self.speHub_ = []
        for i in range(len(hub_cti)): 
            # it the cti for the curve is none a layer will be created
            if hub_cti[i] == None:
                self.speHub_.append(self.hubCurves_[i])

        self.speShroud_ = []
        for i in range(len(shroud_cti)): 
            if shroud_cti[i] == None:
                self.speShroud_.append(self.shroudCurves_[i])
        
        #
        # inlet and outlet curves of the layer region
        #
        self.inOutLayerReg_ = [self.interfaces_[-1],
                              self.inOutCurves_[-1]]
    #
    # creates an interface curve between two specified points on the hub and the shroud curve
    #
    def createInterface(self, 
                        hubCurves,
                        shroudCurves,
                        interface_hub,
                        interface_shroud, 
                        curve, 
                        normalAxis):
        """Create interface curve between hub and shroud.
        
        This method:

            - Creates a linear mean line between the hub and shroud point of the interface.
            - Creates the interface curve based on the mean line.
            - Checks if the interface curve intersects with any hub or shroud curves.

        Parameters
        ----------
        hubCurves : List[analyticGeometry]
          List of hub curves.
        shroudCurves : List[analyticGeometry]
          List of shroud curves.
        interface_hub : List[Tuple[int, float]]             
          Positions of the interfaces on the hub curves.
          Each entry represents:

            - ``interface_hub[i]``: Interface number.
            - ``interface_hub[i][0]``: Curve number where the interface is located.
            - ``interface_hub[i][1]``: Percentage along the curve (0 to 1).

        interface_shroud : List[Tuple[int, float]]             
          Positions of the interfaces on the shroud curves.
          Each entry represents:

            - ``interface_shroud[i]``: Interface number.
            - ``interface_shroud[i][0]``: Curve number where the interface is located.
            - ``interface_shroud[i][1]``: Percentage along the curve (0 to 1).
        
        curve : List[Tuple[float, float, int]]
          Curvature of the interface curve from hub to shroud.
          Each entry represents:

            - ``curve[i]``: Interface number.
            - ``curve[i][0]``: Curvature offset point [%] from hub to shroud.
            - ``curve[i][1]``: Curvature as a percentage of the connection line length.
            - ``curve[i][2]``: Curvature direction.

        normalAxis: dtVector3
          Normal axis
          
        Returns
        -------
        interfaces: List[analyticGeometry]
          List of interface curves.


        
        The interfaces are created between points on the hub and shroud curves (``pointHub``
        and ``pointShroud``), which are specified using the input lists ``interface_hub`` and
        ``interface_shroud``.

        The lists are unpacked as follows:

            ``interface_hub[i]``: Start point of the i-th interface curve on the hub.

            ``interface_shroud[i]``: End point of the i-th interface curve on the shroud.

        The lower-level list entries define the hub and shroud curves and the procentual positions
        along those curves at which the interface start and end points are located:

            ``interface_hub[i][0]``: Index of the hub curve on which the start point lies.

            ``interface_hub[i][1]``: Percentage along the hub curve where the start point is located.

        By iterating over the interface lists, the interface curves are constructed.

        Between the hub and shroud points, a straight line ``MP_linear`` is created, spanning from
        the hub point to the shroud point. Based on this line and the input list ``curve``, the
        curvature of the interface is defined.

        The list ``curve`` is structured as follows. The highest level corresponds to the interface
        index:

            ``curve[i]``: Parameters of the i-th interface.

        The lower level defines the curvature properties of the interface:

            ``curve[i][0]``: Control point offset as a percentage of the length of ``MP_linear``.

            ``curve[i][1]``: Control point base position as a percentage along ``MP_linear``.

            ``curve[i][2]``: Direction of the control point offset.

        The following figure illustrates the creation of the interface curves, with emphasis on
        the curvature definition of the second interface curve.

        .. _interfaceCalc0:
        .. figure:: meridionalFigs/interfaceCalcMethod.png
           :width: 80%
           :align: center

           Creation of the interface curves (red) between the hub and shroud curves (black),
           using the linear mean-plane curve ``MP_linear`` (green).

        :numref:`map-fig-var_interface0` gives the mapping between the mathematical symbols in
        :numref:`interfaceCalc0` and the naming of the variables in this method.

        .. _map-fig-var_interface0:
        .. csv-table:: Mapping between mathematical symbols and variable names.
           :header: "Symbol", "Label"
           :widths: 10, 20
           :escape: #
           :align: center

           :math:`a`, ``curve[i][0]``
           :math:`b`, ``curve[i][1]``
           :math:`c`, ``curve[i][2]``
           ":math:`MP,lin`", ``MP_linear``

        The curvature is defined using a single control point ``pointCurve`` (:math:`\\mathbf{P_{C}}`).
        This control point is computed by applying an offset from a point (:math:`\\mathbf{P_{MP}}`)
        located along the span of ``MP_linear``. The position along the span is given as a percentage
        in ``curve[i][1]`` (:math:`b`).

        The offset is applied in the normal direction of ``MP_linear`` (:math:`\\mathbf{v}`) within the
        two-dimensional plane of the meridional contour. This direction is computed as the normalized
        cross product of the tangential direction of ``MP_linear`` (:math:`\\mathbf{t}`) and the normal
        axis of the meridional contour ``normalAxis`` (:math:`\\mathbf{n_{global}}`):

        .. math::

           \\mathbf{v} = \\frac{\\mathbf{t} \\times \\mathbf{n_{global}}}{\\|\\mathbf{t} \\times \\mathbf{n_{global}}\\|}

        The offset magnitude is defined as a percentage of the length of ``MP_linear``
        (:math:`l_{MP,lin}`), specified by ``curve[i][0]`` (:math:`a`). The direction of curvature is
        controlled by ``curve[i][2]`` (:math:`c`), which takes the values ``1`` or ``-1``.

        The control point ``pointCurve`` is then defined as:

        .. math::

           \\mathbf{P_{C}} = \\mathbf{P_{MP}} + \\mathbf{v} \\cdot l_{MP,lin} \\cdot a \\cdot c

        The interface curve is created from the start and end points on the hub and shroud
        (``pointHub`` and ``pointShroud``) together with the control point ``pointCurve``.
        It is stored in the list ``interfaces``, along with the other interface curves created
        within this loop.

        The method :meth:`detectIntersect` is used to detect intersections between the interface
        curves and the hub and shroud curves. The inputs to this method are the list of interface
        curves and a list of curves against which intersections are checked.

        The returned lists ``intersects_hub`` and ``intersects_shroud`` contain information about
        which interface curves intersect which hub or shroud curves. If no intersections are detected,
        the lists are empty.

        If intersections are detected, the corresponding intersecting curves are reported via logging
        statements.

        The list of interface curves is returned by the method. 
        """
        
        #
        # check that the interface arreays have the same amount of entries
        #
        if (len(interface_hub) != len(interface_shroud)) or (len(interface_hub) != len(curve)):
            raise ValueError("interface arrays need the same no. of entries.")

        interfaces = []
        
        #
        # iterate through all the interfaces that are created
        #
        for ii in range(len(interface_hub)):
            
            #
            # find the points at the specified percentage 
            # of the specified hub and shroud curves
            #
            pointHub = analyticCurve.MustDownCast(
                    hubCurves[interface_hub[ii][0]]).getPointPercent(
                            interface_hub[ii][1])
            pointShroud = analyticCurve.MustDownCast(
                    shroudCurves[interface_shroud[ii][0]]).getPointPercent(
                            interface_shroud[ii][1])
            
            logging.debug("Interface [%d] at hub[%d] [%f]" %  (
                ii, interface_hub[ii][0], interface_hub[ii][1])
            )
            logging.debug("Interface [%d] at shroud[%d] [%f]" %  (
                ii, interface_shroud[ii][0], interface_shroud[ii][1])
            )
            
            #
            # create a straigth line between the two points
            #
            MP_linear = analyticCurve(
                bSplineCurve_pointConstructOCC(
                    vectorDtPoint3()
                    << pointHub
                    << pointShroud,
                    1
                ).result()
            )
            
            direction = curve[ii][2]
            
            #
            # calculate the curvature point by making an offset in normal 
            # direction of the specified percentage from MP_linear
            # 
            # the normal direction is calculated from the crossproduct 
            # of the direction of MP_linear and the normalAxis
            # 
            # the offset is a specified percentage of the length of MP_linear
            # times +-1 for direction
            #
            pointCurve = (analyticCurve.MustDownCast(MP_linear).getPointPercent(curve[ii][1])
                + dtLinearAlgebra.normalize(
                    dtLinearAlgebra.crossProduct(
                        MP_linear.firstDerUPercent(curve[ii][1]), normalAxis
                    )
                ) 
                * MP_linear.length() * curve[ii][0] 
                * direction)
            
            #
            # create the interface as a bSpline curve from the three points
            #
            interface = analyticCurve(
                bSplineCurve_pointConstructOCC(
                    vectorDtPoint3()
                    << pointHub
                    << pointCurve
                    << pointShroud,
                    2
                ).result()
            )
            logging.debug("Interface [%d] : curve offset[%f], percent[%f], direction[%d]" %  (
                ii, curve[ii][0], curve[ii][1], curve[ii][2])
            )


            interfaces.append(interface.clone())
            logging.debug("Copy interface [%d]" % (ii)) 
        
        #
        # check if the generated interfaces are intersecting with any hub or shroud curves 
        #
        intersects_hub = self.detectIntersect(interfaces, hubCurves)
        intersects_shroud = self.detectIntersect(interfaces, shroudCurves)
        
        #
        # logging or warning if intersections occur
        #
        if (not intersects_hub and not intersects_shroud):
            logging.debug("Interfaces created without any illegal intersections")
        else:
            for i_hub in range(len(intersects_hub)):
                logging.warning(
                    "Illegal intersect between interface no. %i and hub curve no. %i",
                    intersects_hub[i_hub][0],
                    intersects_hub[i_hub][1]
                )

            for i_shroud in range(len(intersects_shroud)):
                logging.warning(
                    "Illegal intersect between interface no. %i and shroud curve no. %i",
                    intersects_shroud[i_shroud][0], 
                    intersects_shroud[i_shroud][1]
                )
        
        return interfaces
    
    #
    # searches for intersections between two lists of curves
    #
    def detectIntersect(self, interfaces, curves):
        """Detect intersections between curves from two lists.

        This method:

            - Iterates over the interface list.
            - Cuts each interface curve at 5% and 95% of its span.
            - Iterates over the second curve list.
            - Checks for intersections between the cut interface and each curve.
            - Creates and returns a list managing intersecting curve pairs.

        Parameters
        ----------
        interfaces : List[analyticGeometry]
            List of interface curves.

        curves : List[analyticGeometry]
            List of curves to be checked for intersections.

        Returns
        -------
        intersectList : List[Tuple[int, int, bool]]
            List of detected intersections. Each entry represents:

                - ``intersectList[i]``: curve pair entry
                - ``intersectList[i][0]``: ID of interface curve
                - ``intersectList[i][1]``: ID of curve from second list
                - ``intersectList[i][2]``: True if intersection occurs


        By comparing curves in the two input lists, possible intersections are detected and stored
        in ``intersectList``.

        The method iterates over all interface curves in ``interfaces``. Each interface is trimmed
        at 5% and 95% of its parameter range to avoid detecting artificial intersections at the
        interface endpoints where they meet the hub or shroud curves. The resulting trimmed curve
        is stored as ``cut_interface``.

        For each ``cut_interface``, all curves in the input list ``curves`` are checked for
        intersections. This is performed using the object ``gmf`` of type ``dtOO.gslMinFloatAttr``.
        The call ``gmf.perform()`` returns ``True`` if an intersection is detected, which is stored
        in the variable ``interbool``.

        If ``interbool`` is ``True``, the IDs of the interface and curve, together with the boolean
        flag, are appended to the intersection list.

        The resulting ``intersectList`` is unpacked as follows:

            ``intersectList[i]``: detected intersection between two curves
            ``intersectList[i][0]``: ID of interface curve
            ``intersectList[i][1]``: ID of curve in second list
            ``intersectList[i][2]``: ``True`` if intersection is detected

        The intersection list is returned by the method.
        """
        
        intersect_list = []
        
        #
        # iterate over first curve list
        #
        for ii in range(len(interfaces)):
         
            #
            # trimm the interface curve at 5 and 95 percent. 
            # this is done so the intersect ath the start and end points are not detected. 
            # (this would only occur at very high angles at the start and end points) 
            interface = analyticCurve.MustDownCast(interfaces[ii]).ptrDtCurve()
            cut_interface = analyticCurve(
                trimmedCurve_uBounds(
                    interface,
                    interface.u_uPercent(0.05),
                    interface.u_uPercent(0.95),  
                ).result()
            ) 
            
            #
            # iterate over the list of second curves
            #
            for ic in range(len(curves)):
                #
                # create a minimization object to search for the intersect point between the two curves
                # implemented from the dtOO documentation: Test\Transfinite Mesh
                #
                gmf = gslMinFloatAttr(
                    curveCurveDist(cut_interface.ptrConstDtCurve(), curves[ic].ptrConstDtCurve()),
                    vectorReal([0.5, 0.5,]),
                    vectorReal([0.001, 0.001,]),
                    1.e-8,
                    100
                )
                # returns True if a intersect is detected
                interbool = gmf.perform()

                del gmf
                
                # cache the curves which intersect for return
                if interbool == True:
                    intersect_list.append([ii, ic, interbool])
        
        #
        # return the intersect list
        #
        return intersect_list
    
    def propagate_interface_ids_next(self, curve_to_interface,lab):
        """Map curves to IDs of regular channels.
        
        This method:

            - takes the ``cti`` (curve-to-interface) lists created in :meth:`createSplits`
            - propagates interface IDs to upstream curve segments
            - returns the updated (propagated) lists

        Parameters
        ----------
        curve_to_interface : List
            Curve-to-interface list containing interface IDs or ``None``.

        lab : str
            Label.

        Returns
        -------
        result : List
            Propagated curve-to-regular-channel list.


        This method modifies the ``cti`` (`curve_to_interface`) list. The list is initially
        returned by :meth:`createSplits`. In its original form, it only marks curves that are
        directly upstream of an interface.

        In this method, the interface IDs are propagated upstream so that all curve segments
        belonging to the same regular channel carry the same interface ID. This allows the curves
        to be directly associated with their corresponding regular channels.

        As an example, :meth:`createSplits` may return a ``cti`` list such as:

            ``shroud_cti = [0, None, 1, None, None, None, None]``

        After propagation, this method modifies it to:

            ``shroud_cti = [0, 1, 1, None, None, None, None]``

        In this case, entries 1 and 2 can now be identified as belonging to regular channel 1.

        The method takes the ``curve_to_interface`` list and propagates the integer interface IDs
        to all upstream curves that currently contain ``None`` values.

        All interface IDs present in ``curve_to_interface`` are first collected in the list
        ``iface_indices``. If no interface IDs are found, an empty ``result`` list is returned.

        The index of the first interface is stored in ``first_idx``. Subsequent interface IDs are
        propagated forward to all curve entries between consecutive interface indices. This process
        is controlled using the variables ``start`` and ``end`` within a loop.

        The resulting list contains, for each curve, either the corresponding regular channel ID or
        ``None``. The updated list is returned at the end of the method.
        """

        logging.info("propagating %s_cti" % (lab))
        result = curve_to_interface.copy()
        n = len(result)
        
        # find indices of segments with interface IDs
        iface_indices = [i for i, x in enumerate(result) if x is not None]
        
        if not iface_indices:
            return result  # nothing to propagate
        
        # assign first interface to segments before first one
        first_idx = iface_indices[0]
        for i in range(0, first_idx):
            result[i] = result[first_idx]
        
        # assign next interface to segments between interface IDs
        for idx in range(len(iface_indices) - 1):
            start = iface_indices[idx]
            end = iface_indices[idx + 1]
            for i in range(start + 1, end):
                result[i] = result[end]
        
        groups = defaultdict(list)
        logging.info("Interface and Layers on %s curves" % lab)
        logging.info("Interface/Layer : Curves")
        for i, val in enumerate(result):
            groups[val].append(i)
        for val, indices in groups.items():
            logging.info(f"{val} : {' '.join(map(str, indices))}")
        
        # segments after the last interface remain None
        return result
    
    
    def findChannelCurves(self,
                          curves,
                          cti,
                          ii,
                          lab
                          ):
        """Find and combine curves belonging to a regular channel.
        
        This method:

            - Iterates over a list of curves.
            - Identifies curves that belong to the regular channel.
            - Combines the curves.
            - Returns the combined curve in a list.

        Parameters
        ----------
        curves : List[analyticGeometry]
            List of curves.

        cti : List
            Curve-to-interface list mapping split curves to regular channels.
            Entries can be:

              - int: ID of the regular channel the curve belongs to
              - None: curve belongs to a layered region

        ii : int
            ID of the regular channel.

        lab : str
            Label.

        Returns
        -------
        curve_list : List[analyticGeometry]
            List containing the combined curve.


        The ID of the regular channel is passed to the method via the input ``ii``.
        The list ``curves`` contains the curves that form either the hub or the shroud
        of the channel surface.

        The list ``cti`` contains, for each curve in ``curves``, the ID of the regular
        channel it belongs to (or ``None`` if it is not part of a regular channel).

        By iterating over ``curves`` and checking whether the corresponding entry in
        ``cti`` matches ``ii``, the relevant curves are identified.

        The first curve belonging to the regular channel is assigned to ``regCurve``.
        In subsequent iterations, all additional matching curves are merged into
        ``regCurve`` using the `dtOO` class `bSplineCurve_curveConnectConstructOCC`.

        The combined curve of the regular channel is returned in ``curve_list``.
        """
 
        curve_list = []
        
        first = 0
        #
        # iterate over the curves
        #
        for jj in range(len(curves)):
            #
            # Check if the regular channel ID cti[jj] of the current curve 
            # matches the current regular channel ii.
            # If it matches the curve will be combined with the other matching curves.
            #
            if cti[jj] == ii:
                
                if first == 0:
                    # the first curve is allocated to regCurve
                    regCurve = curves[jj]
                     
                    first = 1
                else:
                    # the subsequent curves are be combined with regCurve
                    regCurve = analyticCurve(
                        bSplineCurve_curveConnectConstructOCC(
                            analyticCurve.MustDownCast(
                                regCurve
                            ).ptrDtCurve(),
                            analyticCurve.MustDownCast(
                                curves[jj]
                            ).ptrDtCurve()
                        ).result()
                    )
        #
        # The combined channel curve is returned in a list with a single entrie.
        #
        curve_list.append(regCurve.clone())           
        logging.info("Combined regular channel curves of %s" % lab)
        return curve_list
     
    # creates the splits of the channel curve 
    # splits are made by interfaces with interface_hub and interface_shroud
    # this function splits the specified curves at the specified percentages
    # it is possible to make multiple splits on one curve
    @staticmethod
    def createSplits(splits, inCurves, lab): 
        """Split the curves at the defined positions.

        This method:

            - Iterates over split definitions.
            - Performs splits on the specified curves at given positions.
            - Creates a curve-to-interface list.

        Parameters
        ----------
        splits : List[List[Tuple[float, int]]]
            List defining splits for each curve.
            Each entry represents:

              - ``splits[i][n]`` : n-th split on the i-th curve
              - ``splits[i][n][0]`` : parameter (percentage) where the split is applied
              - ``splits[i][n][1]`` : ID of the interface that defines the split

        inCurves : List[analyticGeometry]
            Input curves to be split.

        lab : str
            Label.

        Returns
        -------
        outCurves : List[analyticGeometry]
            List of split curves.
        curve_to_interface : List
            List containing the interface ID for each curve endpoint.
            Entries can be:

              - int: ID of the interface located at the curve endpoint
              - None: no interface is located at the curve endpoint


        The curves that are split by this method are provided in the input list ``inCurves``.
        The nested list ``splits`` defines the split positions on these curves.

        The first-level index ``splits[i]`` corresponds to the i-th curve in ``inCurves``.
        The second-level index ``splits[i][n]`` corresponds to the n-th split applied to that curve.

        Each split is defined by a tuple containing:

            - the position along the curve (as a percentage of its parameter range),
            - the ID of the interface that defines the split.

        The full structure of ``splits`` is therefore ``List[List[Tuple[float, int]]]``,
        where ``splits[i][n][0]`` defines the split position and ``splits[i][n][1]``
        defines the corresponding interface ID.

        The following figure shows an activity diagram of this method.

        .. _createSplits:
        .. figure:: meridionalFigs/createSplits.png
           :width: 110%
           :align: center

           Activity diagram of the ``createSplits`` method.

        At the start of the method, empty lists are created for the split curves
        ``outCurves`` and the curve-to-interface list ``curve_to_interface``.
        The latter is used to track which interface each curve in ``outCurves`` belongs to.

        The method generates the split curves by iterating over the first level of the
        list ``splits``. The curve processed in each iteration is referred to as ``curve``.

        If splits occur on this curve, the lists ``split_pos`` and ``interface_ids`` are created.
        These lists aggregate the values from the second level of ``splits`` in order to
        support multiple splits per curve.

        ``split_pos`` contains the percentual split positions along the curve span.
        The values ``0.0`` and ``1.0`` are prepended or appended respectively to ensure
        the full curve range is covered.

        ``interface_ids`` stores the IDs of the interfaces associated with each split.

        The curve splitting is performed by iterating over ``split_pos``.
        During this loop, three cases can occur:

        1. **Boundary split at 0 or 100 percent**

           This case applies when a split position is exactly ``0.0`` or ``1.0``.
           In this situation, ``split_pos`` contains two adjacent identical values
           (i.e. ``split_pos[n] == split_pos[n+1]``).

           The corresponding interface ID is appended to ``curve_to_interface``,
           but no geometric split is performed.

        2. **Single split at full curve length**

           This case occurs when the only effective split is at ``1.0``, resulting in
           neighbouring entries ``0.0`` and ``1.0`` in ``split_pos``.

           The full curve is appended to ``outCurves`` without modification.
           The corresponding entry in ``curve_to_interface`` is handled in the
           subsequent iteration, where Case 1 applies.

        3. **Regular split case**

           If neither Case 1 nor Case 2 applies, a standard split is performed.

           The curve is split using the `dtOO` class
           `trimmedCurve_uBounds`, creating a new curve segment between two
           neighbouring values in ``split_pos``.

           The interface ID is appended to ``curve_to_interface`` if the resulting
           segment lies upstream of the interface; otherwise, ``None`` is appended
           for downstream segments.

        If no split is defined for a curve, it is appended unchanged to ``outCurves``,
        and ``None`` is appended to ``curve_to_interface``.

        Finally, the lists ``outCurves`` and ``curve_to_interface`` are returned.
        """

        # list with the split curves
        outCurves = []
        # mapping of all the curves and their corresponding interface IDs
        curve_to_interface = []
        
        #
        # iterating over the curves 
        # (the length of splits has to be the same as the number or curves)
        #
        for cc in range(len(splits)):
            curve = analyticCurve.MustDownCast(inCurves[cc]).ptrDtCurve()
            #    
            # if a split is made on this curve (splits[cc] contains an entrie)
            #
            if splits[cc]:
                #
                # makes a list of all the occuring splits in this curve
                # appends 0 and 1 at the start and end for the segments
                #
                split_pos = [0.0] + [s[0] for s in splits[cc]] + [1.0]
                #
                # makes a list of the IDs the new curves will have
                # s[1] will contain the interface number ot the interface or None
                #
                interface_ids = [s[1] for s in splits[cc]]
                #
                # iterating over the splits on this curve
                #
                for n in range(len(split_pos)-1):
                    
                    #
                    # exception for when a interface is located on 0 or 100 percent of a curve
                    # doesnt make a split just corrects the cti
                    #
                    if split_pos[n] == split_pos[n+1]:
                        # sets the id of this domain
                        iface_id = interface_ids[n] if n < len(interface_ids) else interface_ids[-1]
                        curve_to_interface[-1] = iface_id
                    
                    #
                    # exception for if the whole curve has a split or interface at 100 percent    
                    # doesnt make a split but appends outCurves and curve_to_interface
                    #
                    elif (split_pos[n] == 0 and split_pos[n+1] == 1):
                        outCurves.append(inCurves[cc].clone())                
                        curve_to_interface.append(None)
                    
                    #
                    # else the splits are created and the interface IDs added
                    #
                    else:
                        # trimms the curves at the split positions
                        ac = analyticCurve(
                            trimmedCurve_uBounds(
                                curve,
                                curve.u_uPercent(split_pos[n]),
                                curve.u_uPercent(split_pos[n+1]),  
                            ).result()
                        )
                        outCurves.append(ac.clone())               
                        
                        # Determines interface ownership
                        # assigns the interface ID of the *upper bound* of the segment
                        iface_id = interface_ids[n] if n < len(interface_ids) else None
                        curve_to_interface.append(iface_id)
                    logging.debug(
                        "Split %s[%d] [%5f, %5f]" % (lab, cc, split_pos[n], split_pos[n+1])
                    )
            
            #
            # if the curve is not split the whole curve will be copied        
            #
            else:
                outCurves.append(inCurves[cc].clone())                
                logging.debug("Copy %s[%d]" % (lab, cc))
                # adds none to curve to interface
                curve_to_interface.append(None)
                # as string identifier "segm" will be set

        logging.info("Finished splitting %s curves. %i curves created" % (lab, len(outCurves)))
        # returns the cut curves and the cti    
        return outCurves, curve_to_interface    
    
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
        """Plot the instantiated geometries in paraview if debug is enabeled.

        Parameters
        ----------
        None

        Returns
        -------
        None


        This method allows the instantiated geometries to be plotted in `ParaView`, by
        adding them to the analytic geometry container.
        If the code is run in paraview, the geometries can be found with the 
        `FindAndShow` method.
        """
        logging.info("Building %s ..." % (self.label_))

        if self.debug():
            try:
                ii = 0
                for interface in self.interfaces_:
                    self.appendAnalyticGeometry(
                        interface.clone(),
                        "debug_interface_" + str(ii) + "_" + self.label_,
                    )
                    ii = ii + 1
            except:
                pass
            try:
                ii = 0
                for aCurve in self.shroudCurves_:
                    self.appendAnalyticGeometry(
                        aCurve.clone(),
                        "debug_shroudCurve_" + str(ii) + "_" + self.label_,
                    )
                    ii = ii + 1
            except:
                pass
            try:
                ii = 0
                for aCurve in self.hubCurves_:
                    self.appendAnalyticGeometry(
                        aCurve.clone(),
                        "debug_hubCurve_" + str(ii) + "_" + self.label_,
                    )
                    ii = ii + 1            
            except:
                pass
            try:
                ii = 0
                for aCurve in self.speHub_:
                    self.appendAnalyticGeometry(
                        aCurve.clone(),
                        "debug_speHub_" + str(ii) + "_" + self.label_,
                    )
                    ii = ii + 1
            except:
                pass
            try:
                ii = 0
                for aCurve in self.speShroud_:
                    self.appendAnalyticGeometry(
                        aCurve.clone(),
                        "debug_speShroud_" + str(ii) + "_" + self.label_,
                    )
                    ii = ii + 1
            except:
                pass
            try:
                ii = 0
                for aCurve in self.inOutCurves_:
                    self.appendAnalyticGeometry(
                        aCurve.clone(),
                        "debug_inOutCurve_" + str(ii) + "_" + self.label_,
                    )
                    ii = ii + 1
            except:
                pass
            try:
                ii = 0
                # defines which channels will be rotatetd for the debug geometries
                for channel in (                
                    self.regChannels_
                ):
                    #off = float(ii) * 0.10      # rotational offset of each channel
                    off = 0.05      # rotational offset of each channel
                    self.appendAnalyticGeometry(
                        partRotatingMap2dTo3d(
                            self.rotVector_,
                            channel,
                            off - 0.05,         # min segment in percent 
                            off + 0.05,         # max segment in percent
                        ).clone(),
                        "debug_channel_" + str(ii) + "_" + self.label_,
                    )
                    ii = ii + 1
            except:
                pass
        return
    
    #
    # returns the rotated segment of the channel number
    #
    def getRegChannel(self, 
                   pos: int,
                   nSlices: int
                   ) -> analyticGeometry:
        """Return a regular channel.
        
        This method:

            - Returns a rotated segment of a regular channel surface. 

        Parameters
        ----------
        pos: int
          Channel Id.
        nSlices: int
          Number of periodic slices.

        Returns
        -------
        : analyticGeometry
          Regular channel volume.


        This getter method is used to return a regular channel specified by the channel ID
        with ``pos``. The rotation angle can be set with``nSlices`` which 
        determines how many partitions the flow domain will have.

        If the number of slices is set to one, a whole 360 degree volume of the flow 
        channel is returned. It is created with the `dtOO` class `rotatingMap2dT03d`.

        If a number greater than one is handed to the method, the class 
        `partRotatingMap2dTo3d` is used, where the angle is calculated as 
        :math:`360°/n_{Slices}`.

        The following figure shows the regular channel volumes resulting from the
        rotation of the regular channel faces.
 
        .. _regChannels3d:
        .. figure:: meridionalFigs/regChannels3d.png
           :width: 60%
           :align: center

           Regular channel volumes created by rotating the first (yellow) and second 
           (green) regular channel faces.
           
        """

        logging.info("Request regChannel[ %d ]" % pos)
        
        # whole rotational domain
        if nSlices == 1:
            return rotatingMap2dTo3d(
                        self.rotVector_,
                        self.regChannels_[pos],
                    ).clone()
        # segment
        else:
            return partRotatingMap2dTo3d(
                        self.rotVector_, 
                        self.regChannels_[pos],
                        0.00,
                        (1/nSlices),
                    ).clone()

    def getLayerRegionCurves(self) -> Tuple[List[analyticGeometry]]:
        """Returns the bounding curves for the layered region.

        Parameters
        ----------
        None

        Returns
        -------
        speHub_: List[analyticGeometry]
          Hub curves of the layered region
        speShroud_: List[analyticGeometry]
          Shroud curves of the layered region
        inOutLayerReg_: List[analyticGeometry]
          inlet and outlet curves of the layered region
        

        Returns the bounding curves of the layered region shown in 
        :numref:`speCurvesClass`.
        """
        return self.speHub_, self.speShroud_, self.inOutLayerReg_



