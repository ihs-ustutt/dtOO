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
    """Create grid channels from hub and shroud curves.
    
    The hub and shroud curves which were input are organized into 6 sided channel
    regions and a set of curves building a layer region. 
    Interface curves on the meridional channel are defined trough their positions 
    on the hub and shroud curves, their curvature and the direction of the curvature.
    The first six sided region is defined by the inlet and first interface and the
    hub and shroud curves between them. The subseqent six sided channels are between 
    the interfaces.
    The curves for the layered region will be the curves after the last interface.
    
    The six sided channel is returned by the following method: 
        
        getRegChannel(pos, nSlices) -> analyticGeoemetry
          -The input pos is the number of the channel to be returned starting with 0.

    The curves for the layered region are returned by: 
        
        getLayerRegionCurves -> Tuple[List[analyticGeometry]]
          The outputs are lists containing the hub, the shroud and the inlet and outlet
          curves of the layerde region
 
    The input nSlices signifies the number of slices in which the whole 360° domain 
    will be segmented. The resulting angle of the returned segment is as follows:
        $[ rotAngle = 2*numpy.pi/nSlices ]$

    Attributes
    ----------
    label_: str
      Label.
    rotVector_: dtVector3
      Rotation vector.
    normalAxis_: dtVector3
      Vector normal to 2d domain.
    interfaces_: List[analyticGeometry]
      list of interface curves.
    hubSplits_: List[List[Tuple[percentage, ID]]]
      list of splits on hub curves.
    shroudSplits_: List[List[Tuple[percentage, ID]]]
      list of splits on splits curves.
    hubCurves_: List[analyticGeometry]
      Channel side of the grid channel.
    shroudCurves_: List[analyticGeometry]
      Channel side of the grid channel.
    inOutCurves_: List[analyticGeometry]
      List of inlet and outlet curves of the whole domain
    regChannels_: List[analyticGeometry]
      List of channel surfaces.
    speHub_: List[analyticGeometry]
      List of Hub curves of the layer region
    speShroud_: List[analyticGeometry]
      List of Shroud curves of the layer region
    inOutLayerReg_: List[analyticGeometry]
      List of inlet and outlet curves of the layer region
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
        label: str
          Label.
        hubCurves: List[analyticGeometry]
          Channel side of the grid channel.
        shroudCurves: List[analyticGeometry]
          Channel side of the grid channel.
        interface_hub: List[Tuple[int, float]]             
          Interface position on hub curves.
          interface_hub[i] = number of interface curve
          interface_hub[i][0] = curveNo, int (starts with 0)
          interface_hub[i][1] = percentage on curve (0 to 1)
        interface_shroud: List[Tuple[int, float]]
          Interface position on hub curves.
          Same input logic as interface_hub.
        interface_curvature: List[Tuple[float, float, int]]
          Curvature of the interface curve from hub to shroud.
          interface_curvature[i] = number of interface curve
          interface_curvature[i][0] = curvature offset point [%] from hub to shroud
          interface_curvature[i][1] = curvature in % of length of connection line
          interface_curvature[i][2] = curvature direction
        rotVector: dtVector3
          Rotation vector.
        Returns
        -------
        None

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
        # Generates interface curves
        self.interfaces_ = self.createInterface(hubCurves,
                                               shroudCurves,
                                               interface_hub,
                                               interface_shroud,
                                               interface_curvature,
                                               self.normalAxis_)
        
        logging.info("###   Preparing Splits")
        # makes split lists with n number of empty listes equal to the number of curves in hub and shroud
        self.hubSplits_ = [[] for _ in range(len(hubCurves))] 
        self.shroudSplits_ = [[] for _ in range(len(shroudCurves))]
        
        # The hub and shroud points of the interfaces are added to the lists of splits
        # they get the ID of the interface i as a descriptor
        # trough this the correct interfaces can be tracked later
        # the .sort function sorts the splits by their percentage along the curve
        # Format:
        #   self.hubSplits = List[List[Tuple[percentage, ID]]]
        for i in range(len(interface_hub)):
            self.hubSplits_[interface_hub[i][0]].append((interface_hub[i][1],i))
            self.hubSplits_[i].sort()
            self.shroudSplits_[interface_shroud[i][0]].append((interface_shroud[i][1],i))
            self.shroudSplits_[i].sort()

        logging.info("###   Creating Hub Splits")
        # Create splits of hub and shroud curves
        # hub_cti and shroud_cti (curve_to_interface) contains a datastructure list
        # Here is mapped which of the newly spilt curves are located upstream of an interface
        # a curve located upstream of an interface is marked with the interface ID the others with None
       
        # the cti lists will be updated to map all the curves to the regular channels they will be part of
        # every cut curve will be appointed the ID of its respective reg channel defined by its interface
        # None means that the chrve is part of a layer channel
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
        # Generating regular channels from the interfaces
        # here all the channels are defined trough the hub and shroud curves and the interface
        # the last interface builds the last regular channel
        # the regChannels are generated by creating a surface from 4 curves
        #  these curves are the upsteam and downstream boundaries 
        #  and the combined hub and shroud curves of the channel
        self.regChannels_ = []
        # interating trough the interfaces (interface_hub and ..._shroud have to be the same size)
        for ii in range(len(interface_hub)):
            logging.info("# Regular Channel[%i]" % ii)

            vhc = vectorHandlingConstDtCurve()
            # adding the boundary curves in the sucession:
            # hub -> inlet -> shroud -> outlet
            # this succession is important so the coordinate system will be as follows:
            # u -   circumferential
            # v --  meridional
            # w --- hub to shroud

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
            
            

            # generates the regular Channel by filling the four curves in vhc    
            self.regChannels_.append(                                        
                analyticSurface(                                            
                    bSplineSurface_bSplineCurveFillConstructOCC(vhc).result()
                )
            )  
            del vhc
        logging.info("%d regular channels created." % len(self.regChannels_))
        
        logging.info("###   Preparing Layer Curves")
        # checking in the cti which curves will get a layer and appending them to speHub, 
        self.speHub_ = []
        for i in range(len(hub_cti)): 
            # it the cti for the curve is none a layer will be created
            if hub_cti[i] == None:
                self.speHub_.append(self.hubCurves_[i])

        self.speShroud_ = []
        for i in range(len(shroud_cti)): 
            if shroud_cti[i] == None:
                self.speShroud_.append(self.shroudCurves_[i])
        
        # inlet and outlet curves of the layerde Regions
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
        """Create interface Curve between hub and shroud.

        Parameters
        ----------
        hubCurves: List[analyticGeometry]
          List of hub Curves
        shroudCurves: List[analyticGeometry]
          List of shroud Curves
        interface_hub: List[Tuple[int, float]]
          Curve Id and procentual position on curve for the interface at hub
        interface_shroud: List[Tuple[int, float]]
          Curve Id and procentual position on curve for the interface at shroud
        curve: List[Tuple[float, float, int]]
          Curvature of the interface curve from hub to shroud.
          curve[i] = number of interface curve
          curve[i][0] = curvature offset point [%] from hub to shroud
          curve[i][1] = curvature in % of length of connection line
          curve[i][2] = curvature direction
        normalAxis: dtVector3
          Normal axis
          
        Returns
        -------
        interfaces: List[analyticGeometry]
          List of interface curves
        """

        if (len(interface_hub) != len(interface_shroud)) or (len(interface_hub) != len(curve)):
            raise ValueError("interface arrays need the same no. of entries.")

        interfaces = []
        
        # iterating through all the interfaces that will be created
        for ii in range(len(interface_hub)):
            
            # finds the points at the specified percentage of the specified hub and shroud curves
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

            # creates a straigth line between the two points
            MP_linear = analyticCurve(
                bSplineCurve_pointConstructOCC(
                    vectorDtPoint3()
                    << pointHub
                    << pointShroud,
                    1
                ).result()
            )
            
            direction = curve[ii][2]

            # calculates the curvature point by making an offset in normal 
            #  direction of the specified percentage from MP_linear
            # the normal direction is calculated from the crossproduct 
            #  of the direction of MP_linear and the normalAxis
            # the offset is a specified percentage of the length of MP_linear
            #  times +-1 for direction
            pointCurve = (analyticCurve.MustDownCast(MP_linear).getPointPercent(curve[ii][1])
                + dtLinearAlgebra.normalize(
                    dtLinearAlgebra.crossProduct(
                        MP_linear.firstDerUPercent(curve[ii][1]), normalAxis
                    )
                ) 
                * MP_linear.length() * curve[ii][0] 
                * direction)
       
            # interface as a bSpline curve from the three points
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

        # checking if the generated interfaces are intersecting with any hub or shroud curves 
        intersects_hub = self.detectIntersect(interfaces, hubCurves)
        intersects_shroud = self.detectIntersect(interfaces, shroudCurves)

        # logging or warning if intersections occur
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

        Parameters
        ----------
        interfaces: List[analyticGeometry]
          List of interfaces
        curves: List[analyticGeometry]
          List of curves
          
        Returns
        -------
        intersectList: List[Tuple[int, int, bool]]
          List of intersects
          intersectlist[i] = curve combination
          intersectlist[i][0] = id of interface curve
          intersectlist[i][1] = id of curve
          intersectlist[i][2] = True if intersect occurs
        """
    
        intersect_list = []

        for ii in range(len(interfaces)):
            
            # trimms the interface curve at 5 and 95 percent 
            # this is done so the planned intersect is not detected 
            #  (this would only occur at very high angles) 
            interface = analyticCurve.MustDownCast(interfaces[ii]).ptrDtCurve()
            cut_interface = analyticCurve(
                trimmedCurve_uBounds(
                    interface,
                    interface.u_uPercent(0.05),
                    interface.u_uPercent(0.95),  
                ).result()
            ) 

            for ic in range(len(curves)):
                # creates a minimization object to search for the intersect point between the two curves
                # implemented form the dtOO documentation: Transfinite Mesh
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
                
                # caches the curves which intersect for return
                if interbool == True:
                    intersect_list.append([ii, ic, interbool])

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
        """Finds the curves belonging to the regular channel with the id ii.

        Parameters
        ----------
        curves List[analyticGeometry]
          List of curves
        cti: List
          Curve to interface list matching split curves to the interfaces
          Entries can be:
            - int:   number of the channel the curve belongs to
            - None:  Curve belongs to layered region
        ii: int
          Id of current regular channel
        lab: str
          Label
          
        Returns
        -------
        curve_list: List[analyticGeometry]
          List of curves belonging to the channel
        """
 
        # ii is the channel that is searched
        curve_list = []
        
        first = 0
        # iterating over the hubCurves
        for jj in range(len(curves)):
            # checking if the interface ID of the current curve matches the current interface
            if cti[jj] == ii:
                
                # if it matches the curve will be combined with the other matching curves
                if first == 0:
                    # the first curve will be saved
                    regCurve = curves[jj]
                     
                    first = 1
                else:
                    # the subsequent curves will be combined to the previous curves
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
        # the combined channel curve is returned in a list with a single entrie
        curve_list.append(regCurve.clone())           
        logging.info("Combined regular channel curves of %s" % lab)
        return curve_list
     
    # creates the splits of the channel curve 
    # splits are made by interfaces with interface_hub and interface_shroud
    # this function splits the specified curves at the specified percentages
    # it is possible to make multiple splits on one curve
    @staticmethod
    def createSplits(splits, inCurves, lab): 
        """splits the curves at the defined positions.

        Parameters
        ----------
        splits: List[List[Tuple[percentage, ID]]]
          List with splits on every curve
        inCurves: List[analyticGeometry]
        lab: str
          Label

        Returns
        -------
        outCurves: List[analyticGeometry]
          split curves
        curve_to_interface: List
          List containing the interface number for every curve
        """

        # list with the split curves
        outCurves = []
        # mapping of all the curves and their corresponding interface IDs
        curve_to_interface = []
        
        # iterating over the curves 
        # (the length of splits has to be the same as the number or curves)
        for cc in range(len(splits)):
            curve = analyticCurve.MustDownCast(inCurves[cc]).ptrDtCurve()

            # if a split is made on this curve (splits[cc] contains an entrie)
            if splits[cc]:
                # makes a list of all the occuring splits in this curve
                # appends 0 and 1 at the start and end for the segments
                split_pos = [0.0] + [s[0] for s in splits[cc]] + [1.0]
                # makes a list of the IDs the new curves will have
                # s[1] will contain the interface number ot the interface or None
                interface_ids = [s[1] for s in splits[cc]]

                # iterating over the splits on this curve
                for n in range(len(split_pos)-1):

                    # exception for when a interface is located on 0 or 100 percent of a curve
                    # doesnt make a split just corrects the cti
                    if split_pos[n] == split_pos[n+1]:
                        # sets the id of this domain
                        iface_id = interface_ids[n] if n < len(interface_ids) else interface_ids[-1]
                        curve_to_interface[-1] = iface_id

                    # exception for if the whole curve has a split or interface at 100 percent    
                    # doesnt make a split but appends outCurves and curve_to_interface
                    elif (split_pos[n] == 0 and split_pos[n+1] == 1):
                        outCurves.append(inCurves[cc].clone())                
                        curve_to_interface.append(None)

                    # else the splits are created and the interface IDs added
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
            
            # if the curve is not split the whole curve will be copied        
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
        """calculates the bounding box and normal axis of curves in one plane.

        Parameters
        ----------
        curves: List[analyticGeometry]
          List of curves.

        Returns
        -------
        normalAxis: dtVector3
          Normal axis of the bounding box
        bbCenter: dtPoint3
          Center point of the bounding box
        bb: pairDtPoint3
          Bounding box points
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
                    off = float(ii) * 0.10      # rotational offset of each channel
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
        """Returns regular channel.

        Parameters
        ----------
        pos: int
          Channel Id.
        nSlices: int
          Number of periodic slices

        Returns
        -------
        : analyticGeometry
          Channel
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



