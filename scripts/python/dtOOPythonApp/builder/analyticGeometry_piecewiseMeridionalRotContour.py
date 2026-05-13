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
        
        - Splits the hub and shroud curves at the intersect points with the interface.
        
        - Creates regular channels between the interfaces.
        
        - Creates lists with boundary curves for a layered flow channel downstream of the last interface.
    

    Attributes
    ----------
    label_: str
      Label.
    rotVector_: dtVector3
      Rotation vector.
    normalAxis_: dtVector3
      Vector normal to 2d domain.
    interfaces_: List[analyticGeometry]
      List of interface curves.
      With interfaces_[i] being the i-th interface curve. 
    hubSplits_: List[List[Tuple[float,int]]]
      List of splits on hub curves.
      Each entry represents:

        - ``hubSplits_[i][n]`` : n-th split on the i-th hub curve.
        - ``hubSplits_[i][n][0]`` : Percentage where the split is applied.
        - ``hubSplits_[i][n][1]`` : ID of the interface which creates the split.
        
    shroudSplits_: List[List[Tuple[float,int]]]
      List of splits on hub curves.
      Each entry represents:

        - ``shroudSplits_[i][n]`` : n-th split on the i-th shroud curve.
        - ``shroudSplits_[i][n][0]`` : Percentage where the split is applied.
        - ``shroudSplits_[i][n][1]`` : ID of the interface which creates the split.
        
    hubCurves_: List[analyticGeometry]
      Split hub curves.
    shroudCurves_: List[analyticGeometry]
      split shroud curves.
    inOutCurves_: List[analyticGeometry]
      List of inlet and outlet curves of the whole domain.
    regChannels_: List[analyticGeometry]
      List of channel surfaces.
    speHub_: List[analyticGeometry]
      List of Hub curves of the layer region.
    speShroud_: List[analyticGeometry]
      List of Shroud curves of the layer region.
    inOutLayerReg_: List[analyticGeometry]
      List of inlet and outlet curves of the layer region.
    
    Returns
    -------
    None

    Examples
    --------

    >>> import dtOOPythonSWIG as dtOO
    
    Define list of hub curves

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
    
    Define list of shroud curves

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
   
     
    The main method of this class is the class constructor.
    The inputs are the hub and shroud curves as well as the lists 
    setting the interface parameters.

    The normal axis of the crosssection of the flow channel is returned
    by the method `calculateNormalAxis`. It is assigned th the variable
    ``normalAxis_`` to be later used to determine the direction of the 
    interface curves.

    Here the interface curves are created with the method `createInterfaces`.
    The method returns the in the list ``interfaces_``.
    with the method `detectIntersect` it is detected if the interface curves
    intersect with the hub or shroud curves which throws an exception.

    Special inlet and outlet curves are created as straight lines 
    on the inlet and the outlet of the meridional contour. The list 
    ``inOutCurves_`` contains them.
    
    The hub and shroud curves are split at their intersects at the start or
    end points of the interface curves with `createSplits`. 
    With the lists ``hubSplits_`` and ``shroudSplits_`` it is tracked, which 
    curves are split by which interface at what percentage.
    The method `createSplits` returns the split curves in the lists 
    ``hubCurves_`` and ``shroudCurves_``
    
    The newly split curves are managed with lists. These lists are first 
    returned by `createSplits` and track which interface curve is located
    at which hub and shroud curves. These lists are edited by 
    `propagate_interface_ids_next` to show which hub and shroud curve is part
    of which regular channel.
    
    The regular channels are created between two neighbouring interfaces in the 
    flow direction of the channel contour.
    The first interface is created between the inlet and the first interface.
    The following regular channels are created between the interfaces.

    The surfaces of the regular channels are created with the interface curves 
    and the hub and shroud curves extending between them. These curves are 
    found with the method `findChannelCurves`.
    The surfaces are created in the constructor and saved in the list 
    ``regChannels_``.
    
    The hub and shroud curves which are not part of the regular channels are
    saved as special curves in the lists ``speHub_`` and ``speShroud_``.
    These will later form the basis of a layered flow channel. The inlet and 
    outlet curves for this layered flow channel are saved in the list 
    ``inOutLayerReg_`` and are formed by the last interface curve and the 
    outlet of the flow domain.

    The `build` method is used for debugging of the class. If the debug option
    is enabeled in the class call the created geometries are returned to 
    `paraview`, where they can be plotted with their name assigned in the 
    `build` method and the label ``label_``.

    The method `getRegChannel` is used to return the volume of a specified 
    regular channel. The volume is created by rotating the face of a regular 
    channel around the rotational axis ``rotVector_`` with a specified angle.

    With the getter method `getLayerRegionCurves` the lists ``speHub_`` and 
    ``speShroud_`` as well as ``inOutLayerReg_`` are returned.

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

        
        The constructor of this class is used to partition the hub and shroud curves
        into the regular channels and special hub and shroud curves.
         
        The input lists named ``hubCurves`` and ``shroudCurves`` define the 2 dimensional
        crosssection of the channel. 
        The curves have to be created so that their direction is the same as the 
        downstream direction of the flow machine. Furthermore, the sequence of the curves
        in the lists must also correspond to the downstream direction of the machine.
        
        The following figure shows the hub and shroud curves.

        .. _hsCurves0:
        .. figure:: meridionalFigs/hsCurve_noInterface.png
           :width: 50%
           :align: center

           Hub and shroud curves of the meridional channel.
           Numbering corresponds to the indices in the
           ``hubCurves`` and ``shroudCurves`` lists.
        
        With the method `calculateNormalAxis` the normal axis of the crosssection 
        ``normalAxis_`` is computed. 
        
        By handing the inputs ``hubCurves``, ``shroudCurves``, ``interface_hub``, 
        ``interface_shroud`` and ``interface_curvature`` as well as the mewly created
        normal axis to the method `createInterface`, the interface curves are created.
        The method returns a list with the interface curves which is saved with the 
        variable name ``interfaces_``. The position of the interfaces in this list
        doubles as the interface ID.
        
        The hub and shroud curves are split at their intersect points with the interface 
        curves. For this the lists ``hubSplits_`` and ``shroudSplits_`` are created, in 
        which every entry is an empty list. The number of entries corresponds to the 
        number of hub and shroud curves.
        By looping over the interfaces, the split position and interface ID, which 
        intersects at this position, is inserted as a Tuple into the list entrie of the 
        curve, where the split is applied.
        The resulting ``hubSplits_`` and ``shroudSplits_`` are of the type 
        List[List[Tuple[float, int]]].
        The values in this nested list represent the following inputs:

            - ``hubSplits_[i][n]`` : n-th split on the i-th hub curve.
            - ``hubSplits_[i][n][0]`` : Percentage where the split is applied.
            - ``hubSplits_[i][n][1]`` : ID of the interface which creates the split.

        The splits for the hub and the shroud curves each are applied with the method 
        `createSplits`. The hub and shroud curves are split independently from each other
        by calling the method once for the hub and once for the shroud curves.

        The method takes the list with the split data ``hubSplits_`` or
        respectively ``shroudSplits_`` and the lists with curves ``hubCurves`` or 
        respectively ``shroudCurves`` as input. Furthermore a label is handed over.

        The method returns lists of the split hub and shroud curves as well as `cti` 
        lists. 
        If no split is done on a curve or the split position on the curve is at 0 or 100
        percent of the curve's span, no split is created and the curve is appended as a 
        copy of the original curve.
        The returned curve lists are instantiated with the names ``hubCurves_`` and 
        ``shroudCurves_``.
        
        The following figure shows the meridional contour with interface and inlet and 
        outlet curves. The curves ``hubCurves[1]`` and ``shroudCurves[2]`` 
        (see :numref:`hsCurves0`) are split into two curves by the interface curve 
        ``interfaces_[1]``. The new curves are saved as ``hubCurves_[1]`` and 
        ``hubCurves_[2]`` as well as ``shroudCurves_[2]`` and ``shroudCurves_[3]`` in
        the instantiated curve lists.

        .. _hsCurves01:
        .. figure:: meridionalFigs/hsCurves.png
           :width: 50%
           :align: center

           Meridional channel with interface (red) and inlet and outlet curves (orange).
           Numbering  of the hub and shroud curves (black) corresponds to the indices in 
           the ``hubCurves_`` and ``shroudCurves_`` lists.
        
        The `cti` lists (short for curve to interface) track which hub and shroud curves
        are upsteram to which interface. The curve to interface lists have a 
        number of entries according to the number of hub or shroud curves. The curves
        whose end points are on an interface curve get the interface's ID in the `cti`.
        The curves for which this does not apply get a ``None`` entry.
        The resulting `cti` lists in the example of :numref:`hsCurves01` is:

        ``hub_cti = [0, 1, None, None, None]`` 
        
        ``shroud_cti = [0, None, 1, None, None]`` 
        
        The `cti` lists are handed to the method `propagate_interface_ids_next` where 
        the entries are matched to the regular channels, which the curves are part of.
        The return of this method overwrites the `cti` list.
        The resulting ``shroud_cti`` list is:

        ``shroud_cti = [0, 1, 1, None, None]`` 
        
        In ``inOutCurves_`` inlet and outlet curves of the meridional contour are saved.
        The inlet curve is created as an straight line between the start points of the 
        first curves in the hub and shroud curve lists. The outlet curve is created
        in the same way, but with the end points of the last hub and shroud curve lists.

        The number of regular channels corresponds to the number of defined interfaces.
        The first regular channel is created between the inlet curve and the first 
        interface. The following regular channels are created between the in flow 
        direction adjacent interfaces.
        
        The regular channel surfaces are created by iterating over the number of 
        interfaces. 

        For each regular channel the hub and shroud curves, which are part
        of the channel, are found with the method `findChannelCurves`. This method is 
        used once for the hub and once for the shroud curves.
        The inputs are the instantiated curve lists ``hubCurves_`` or ``shroud_curves``,
        the `cti` lists ``hub_cti`` and ``shroud_cti`` and the ID of the regular channel
        as well as a label. The curves which are part of the regular channel are found 
        by comparing the channel ID to the `cti`-list and combined into one curve.

        The upstream interface or, in case of the first regular channel, the inlet of the
        meridional channel form the inlet curves of the regular channels. The outlet of 
        the regular channel is fromed by the downstream interface.

        The surfaces are built with a `bSplineSurface_bSplineCurveFillConstructOCC` 
        object from the inlet outlet and the combined hub and shroud curves of the 
        regular channel. 
        In order to keep consistent directions in each channel the curves are handed
        to the `construct` object in the following order:

            1. combined hub curve
            2. inlet of the regular channel
            3. combined shroud curve
            4. outlet of the regular channel

        This results in a regular channel whose uvw-directions after the rotation are as
        follows:

            - u : circumferential direction
            - v : meridional direction
            - w : hub to shroud direction

        The regular channel surfaces are saved in the list ``regChannels_``. The 
        following figures show the surfaces of the regular channels.

        .. _regChannels:
        .. figure:: meridionalFigs/regChannels.png
           :width: 50%
           :align: center

           Regular channels (yellow, green) between the interfaces.
           Special hub and shroud curves are not part of the regular channels.

        Curves which are not part of the regular channels have a value of ``None`` in
        their `cti` list. By iterating over the `cti` list and checking for ``None`` 
        vaules the special hub and shroud curves are fuound and written into the the 
        lists ``speHub_`` and ``speShroud_``.
        These special curves are used for a layered flow channel.
        The inlet of this layered flow channel is formed by the last interface curve 
        and the outlet by the outlet of the meridional channel thei are saved in the 
        list ``inOutLayerReg_``. 

        The following figure shows the special curves and the inlet and outlet of the
        layered flow channel contour.

        .. _speCurvesClass:
        .. figure:: meridionalFigs/speCurvesClass.png
           :width: 55%
           :align: center

           Special hub and shroud curves with numbering according to positions in
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

        
        The interfaces are created between the points on the hub and shroud curves
        (``pointHub`` and ``pointShroud``) specified with the input lists 
        ``interface_hub`` and ``interface_shroud``.

        The lists are unpacked as follows:

            ``interface_hub[i]``: Start point of the i-th interface curve on the hub.

            ``interface_shroud[i]``: End point of the i-th interface curve on the shroud.

        The lower-level lists specify the hub and shroud curves and the percentages of 
        those curves at which the start and end points of the interfaces are located:

            ``interface_hub[i][0]``: Index of the hub curve on which the start point is located.

            ``interface_hub[i][1]``: Percentage along the hub curve where the start point is located.
        
        By iterating over the interface lists the interfaces are created.

        Between the points the line ``MP_linear`` is created spanning from the point on
        the hub to the point of the shroud.
        Based on this line and the input list ``curve`` the curvature of the interface is
        calculated.

        The list ``curve`` is unpacked as follows.
        The highest level of the list corresponds to the index of the interface curve:

            ``curve[i]``: Index of the interface.

        The lower level of the list defines the curvature of the interface as follows:

            ``curve[i][0]``: Control point offset as a percentage of the length of ``MP_linear``.

            ``curve[i][1]``: Control point base position as a percentage along ``MP_linear``.

            ``curve[i][2]``: Direction of the control point offset.
        
        
        The following figure shows the creation of the interface curves, 
        with a focus on the curvature of the second interface curve.

        .. _interfaceCalc0:
        .. figure:: meridionalFigs/interfaceCalcMethod.png
           :width: 80%
           :align: center

           Creation of the interface curves (red) between the hub and shroud curves 
           (black), with the linear mean-plane curve ``MP_linear`` (green).

        :numref:`map-fig-var_interface0` gives the mapping between the mathematical symbols in
        :numref:`interfaceCalc0` and the naming of the variables this method.

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

        The curvature is defined through one control point ``pointCurve`` 
        (:math:`\mathbf{P_{C}}`). This control point is calculated by making an offset 
        from a point (:math:`\mathbf{P_{MP}}`) on the span of ``MP_linear`` specified by 
        the spanwise percentage in ``curve[i][1]`` (:math:`b`).

        The offset is made in the normal direction of ``MP_linear`` (:math:`\mathbf{v}`). 
        It is calculated as the normalized cross product of the tangential direction of 
        ``MP_linear`` (:math:`\mathbf{t}`) and the normal axis of the meridional contour 
        ``normalAxis`` (:math:`\mathbf{n}`).
        The following formula shows the calculation of the offset direction.

        .. math::
       
           \\mathbf{v} = \\frac{\\mathbf{t} \\times \\mathbf{n}}
           {\\|\\mathbf{t} \\times \\mathbf{n}\\|}
        
        The offset distance is calculated as the percentage of the length of 
        ``MP_linear`` (:math:`l_{MP,lin}`) specified in ``curve[i][0]`` (:math:`a`). The 
        direction of the curvature of the interface is specifed with ``curve[i][2]`` 
        (:math:`c`) by handing over the value ``1`` or ``-1``. 

        The control point ``pointCurve`` is calculated as follows.
        
        .. math::

           \\mathbf{P_{C}} = \\mathbf{P_{MP}} + \\mathbf{v} * {l_{MP,lin}} * {a} * {c}

        The interface curve is created from the start and end points on hub and shroud 
        ``pointHub`` and ``pointShroud`` and the control point ``pointCurve``.
        It is saved in the list ``interfaces`` with the other interface curves created in
        this loop.

        The method `detectIntersect` is used to detect intersections between the
        interface curves and the hub and shroud curves. The inputs to this method is the
        list of interface curves and a list of curves for wihich it is checked if they 
        intersect with the interface curve. The method is called twice, once to check for 
        the intersection of the interfaces with the hub curves and once with the shroud 
        curves.
        
        The returned lists ``intersects_hub`` and ``intersects_shroud`` contain the
        information of which interface intersects with which wall curve. If no intersects
        are detected the list is empty.
        If intersects are detected the intersecting curves will be written out in 
        logging statements.

        The list of interfaces is returned by the method.
        
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
        """Detect intersects between curves from two lists.

        This method:
            
            - iterates over interface list.
            - cuts the interface at 5 and 95 percent.
            - iterates over curve list.
            - checks for intersections between the cut interface and curve list.
            - creates list managing intersecting curves.

        Parameters
        ----------
        interfaces: List[analyticGeometry]
          List of interfaces
        curves: List[analyticGeometry]
          List of curves
          
        Returns
        -------
        intersectList: List[Tuple[int, int, bool]]
          List of intersects.
          Each entry represents:

            - ``intersectlist[i]``:  curve combination
            - ``intersectlist[i][0]``: ID of interface curve
            - ``intersectlist[i][1]``: ID of curve
            - ``intersectlist[i][2]``: True if intersect occurs
        

        By comparing the curves in two lists, possible intersections between these curves
        can be found and managed with the list ``intersect_list``.

        The method iterates over the interfaces in the list ``interfaces``.
        Every interface curve is cut at 5 and 95 percent its span in order to
        prevent the detection of the intersections at the interfaces' start and end 
        points on the hub or shroud curves. The cut interface curve is saved in 
        ``cut_interface``.

        By iterating over the list of curves a check for an itersection between 
        ``cut_interface`` and the curves an be performed with the object ``gmf`` of the 
        `dtOO` class `gslMinFloatAttr`.
        The call ``gmf.perform()`` returns ``True`` if an intersect is detected.
        The variable ``interbool`` saves this value.

        If ``interbool`` is ``True`` for an iteration of the inner loop, the intersect
        list is appended with the IDs of the interface and the curve in which the 
        intersect occurs as well as ``interbool``.
        
        The resulting intersect list is unpacked as follows:

            ``intersect_list[i]``: Detected intersect between two curves.
            ``intersect_list[i][0]``: ID of curve in first list (interface).
            ``intersect_list[i][1]``: ID of curve in second list (hub or shroud curve).
            ``intersect_list[i][2]``: ``True`` value for the intersect.

        The intersect list is returned by the method.
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
    
    # edits the cti / curve_to_interface list
    # the list is first returned by self.cerateSplits()
    # it originally only tracks which curve is directly upstream of an interface
    # here it is propagated so the curve Ids match the number of the regular channel they will be part of
    # As example:
    # self.CreateSplits() would return the cti like this:
    #   shroud_cti =  [0, None, 1, None, None, None, None]
    # this function edits the code so the cti will look like this:
    #   shroud_cti =  [0, 1, 1, None, None, None, None]
    # entries 1 and 2 can now easily identified to belong to regular channel 1
    def propagate_interface_ids_next(self, curve_to_interface,lab):
        """Map curves to IDs of regular channels.

        This method:
            
            - takes the cti lists created in `createSplits`.
            - propagates the interface IDs to the upsteam curves.
            - returns the propagated lists.

        Parameters
        ----------
        curve_to_interface: List
          Curve to interface list containing interface IDs or ``None``.
        lab: string
          Label.
          
        Returns
        -------
        result: List
          Propagated curve to regular channel list.


        This method takes the ``curve_to_interface`` list and propagtes the ``int`` 
        values to the curves which are located upstream and for which a ``None`` value
        is written in ``curve_to_interface``.

        The interface IDs in ``curve_to_interface`` are saved in the list 
        ``iface_indices``. If no interface IDs are in the cti list the ``result`` list is
        returned empty.

        The ID of the first interface is added with ``first_idx``.
        The following interface IDs are written to the curve entries which are between
        the previous and the next IDs, this is tracked in a loop with the variables 
        ``start`` and ``end``.
        
        The resulting list has entries, in which for every curve an ID of a regular 
        channel or ``None`` is saved. The list is returned at the end of the method.
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
    
    
    # will iterate trough the curves and find out which curves belong to which regular channel
    # this will be done with the help of the cti
    # the hub or shroud curves which build the channel curves of one channel will be combined 
    def findChannelCurves(self,
                          curves,
                          cti,
                          ii,
                          lab
                          ):
        """Find and combine curves belonging to a regular channel.
        
        This method:
            
            - iterates over list of curves.
            - finds curves which are part of the regular channel.
            - combines the curves.
            - returns the combnined curve in a list.

        Parameters
        ----------
        curves List[analyticGeometry]
          List of curves
        cti: List
          Curve to interface list matching split curves to the regular channels.
          Entries can be:
            
            - int:   ID of the regular channel the curve is part of.
            - None:  Curve is part of layered region.
        
        ii: int
          Id of regular channel.
        lab: str
          Label.
          
        Returns
        -------
        curve_list: List[analyticGeometry]
          List containing the combined curve.


        The ID of the regular channel is handed to the method with the input ``ii``. The 
        list ``curves`` contains the curves which form either the hub or the shroud 
        curves of the channel surface.
        The list ``cti`` contains for every curve in ``curves`` the IDs of the regular 
        channels, they are part of (or ``None`` if they are not part of the regular 
        channels).

        By iterating over ``curves`` and checking if their corresponding value in ``cti``
        matches ``ii`` the correct curves are found.

        The first curve which is part of the regular channel is assigned to ``regCurve``.
        In the following iterations of the loop, the curves which are part of the regular
        channel are combined with ``regCurve`` with the `dtOO` class 
        `bSplineCurve_curveConnectConstructOCC`.

        The combined curve of the regular channel is returned in the list ``curve_list``.
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

            - iterates over splits.
            - preforms the splits on the specifed curves and positions.
            - creates a curve to interface list.

        Parameters
        ----------
        splits: List[List[Tuple[float, int]]]
          List with splits on every curve.
          Each entry represents:

            - ``splits[i][n]`` : n-th split on the i-th curve.
            - ``splits[i][n][0]`` : Percentage where the split is applied.
            - ``splits[i][n][1]`` : ID of the interface which creates the split.
        
        inCurves: List[analyticGeometry]
        lab: str
          Label

        Returns
        -------
        outCurves: List[analyticGeometry]
          split curves
        curve_to_interface: List
          List containing the interface number for every curve
          Entries can be:
            
            - int:   ID of the interface which is at the endpoint of the curve.
            - None:  No interface is located at the endpoint of the curve.
        

        The curves which are split in this method are in the input list ``inCurves``.
        With the nested list ``splits`` the positions of the the splits on the curves
        are handed to the method. 
        In this list, the first level ``splits[i]`` points to the curves in the 
        correlating entries of ``inCurves[i]`` on which splits are applied. The second 
        level list corresponds to the number of splits which are made on the i-th curve.
        So ``splits[i][n]`` corresponds to the n-th split on the i-th curve in 
        ``inCurves``.

        Every split position and the ID of the interface which performs the split is 
        located in the second level list as a `Tuple`. The resulting ``splits`` list has 
        the format ``List[List[Tuple[float, int]]]``. Where ``splits[i][n][0]`` is the 
        percentage of the curve span where the split is performed and ``splits[i][n][1]``
        is the ID of the interface which is located on this split.
        
        The following figure shows an activity diagramm of the method.
        
        .. _createSplits:
        .. figure:: meridionalFigs/createSplits.png
           :width: 110%
           :align: center

           Activity diagram of the `createSplits` method.

        At the start of the method empty lists are created for the split curves 
        ``outCurves`` and the cti ``curve_to_interface``. The cti will be used to track,
        which of the curves in ``outCurves`` belongs to which interface.

        The method creates the splits on the curves by iterating over the first level of
        of the list ``splits``. The curve which is split in this iteration is saved as 
        ``curve``.
        If a split occurs on this curve the lists ``split_pos`` and ``interface_ids`` are
        created. These lists unify the values in the second level list, in order to 
        create multiple splits on the curve. 
        ``split_pos`` contains the percentages of the curve spans where splits 
        are created, the values ``0.0`` and ``1.0`` are appended to the start or 
        respectively the end of the list.
        ``interface_ids`` contains the IDs of the interfaces which create the splits on 
        the curve. 

        The curve is split by looping over ``split_pos``. Here three cases can be applied.

        In the first case the split is on 0 or 100 percent of the curve.
        This case applies, when a value of ``0.0`` or ``1.0`` is in 
        ``splits[i][n][0]``. The resulting ``split_pos`` list will have two 
        neighbouring entries with the same value and 
        ``split_pos[n] == split_pos[n+1]`` applies.
        In this case the interface ID is inserted in the cti list but no split is
        created.

        In the second case the only split on the curve is at 100 percent of the span.
        This case applies, when neighbouring entries in ``split_pos`` are ``0.0``
        and ``1.0``.
        In this case the whole curve is appended to ``outCurves``. The editing of
        ``curve_to_interface`` is performed in the following iteration where case 1. 
        will apply.

        In the last case the cases 1. or 2. do not apply and a regular split is created. 
        The curve is split and a new curve extending between the two neighbouring 
        percentages in ``split_pos`` is created with the `dtOO` class 
        `trimmedCurve_uBounds`.
        The interface ID is appended if the resulting curve is upstream of the 
        interface. If the split curve is downstream of the interface ``None`` is
        appended.

        If no split is performed on the curve, the curve is appended to ``outCurves``
        without any operation performed on it. The value ``None`` is appended to 
        ``curve_to_interface``.

        The lists ``outCurves`` and ``curve_to_interface`` are returned.
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
            
            - creates a bounding box around the curves.
            - calculates the normal axis on the bounding box.
            - calculates the center of the bounding box.
            - returns normal axis, center and bounding box.

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


        This method allows the instantiated geometries to be plotted in paraview, by
        adding them to the analytic geometry container.
        If the code is run in paraview, the geoemtries can be found with the 
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

            - returns a rotated segment of a regular channel surface. 

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


        This getter method is used to return a regular channel specified by th channel id
        with ``pos``. The rotation angle can be set ``nSlices`` which 
        determines how many partitions the flow domain will have.

        If the number of slices is set to one, a whole 360 degree volume of the flow 
        channel is returned. It is created with the `dtOO` class `rotatingMap2dT03d`.

        If a number greater than one is handed to the method the class 
        `partRotatingMap2dTo3d`, where angle is calculated as :math:`360°/n_{Slices}`.

        The following figure shows the regular channel volumes resulting from the
        rotation of the regular channel faces.
 
        .. _regChannels3d:
        .. figure:: meridionalFigs/regChannels3d.png
           :width: 80%
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
        inOutLayerReg: List[analyticGeometry]
          inlet and outlet curves of the layered region
        """
        return self.speHub_, self.speShroud_, self.inOutLayerReg_



