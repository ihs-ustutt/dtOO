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
    map3dTo3d,
    analyticCurve,
    bSplineSurface_skinConstructOCC,
    bSplineSurface_geomCurveFillConstructOCC,
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
    curveCurveDist,
    dtCurve,
    gslMinFloatAttr,
    vectorReal,
    vectorHandlingConstDtCurve,
    bSplineSurface_bSplineCurveFillConstructOCC,
    bSplineCurve_curveConnectConstructOCC
)

from typing import List, Union, Tuple
from collections import defaultdict
import logging
import numpy


class analyticGeometryLayers_piecewiseMeridionalRotContour(dtBundleBuilder):
    """Create a grid channel.

    The grid channel is created by rotating the channelSide in the range:

        $[ -0.5*2*pi/numberOfSections, 0.5*2*pi/numberOfSections ]$

    The grid channel is then represented as a rotatingMap2dTo3d within the
    predefined angle.
    """

    def __init__(
        self,
        label: str,
        hubCurves: List[analyticGeometry],
        shroudCurves: List[analyticGeometry],
        hub_splits: List[List[float]] = [],
        #hub_combine: List[List[bool]] = [],
        shroud_splits: List[List[float]] = [],
        #shroud_combine: List[List[bool]] = [],
        layer_thickness: float = 0,
        layer_supports: List[float] = [],
        interface_hub: List[float] = [],             # [curve, percent]
        interface_shroud: List[float] = [],
        interface_curvature: List[float] = [],         # [curvature offset, percent]
        internals: List[analyticGeometry] = [],
        rotVector: dtVector3 = dtVector3(0, 0, 1),
        origin: dtPoint3 = dtPoint3(0, 0, 0),
    ) -> None:
        """Constructor.

        Parameters
        ----------
        label: str
          Label.
        hubCurves: List[analyticGeometry]
          Channel side of the grid channel.
        shroudCurves: List[analyticGeometry]
          Number of periodic sections.
        internals: List[analyticGeometry]
          Internal objects.
        rotVector: dtVector3
          Rotation vector.
        origin: dtPoint3
          Origin.

        Returns
        -------
        None

        """
        logging.info("Initializing %s ..." % (label))
        super(analyticGeometryLayers_piecewiseMeridionalRotContour, self).__init__()
        self.label_ = label
        
        self.origin_ = origin
        self.rotVector_ = dtLinearAlgebra.normalize(rotVector)
        
        #
        # Extract a bounding box from all start and end points of hub and
        # shroud curves; necessary for calculating the normal direction
        #
        self.normalAxis_, center = self.calculateNormalAxis(            # returns normal direction of entire "2D domain"
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
        # Mapping the list of splits
        # Every split entrie gets a None descriptor 
        # This will tell the code that it isnt a interface split
        self.hubSplits_ = []
        for i in range(len(hub_splits)):
            splits = []
            for l in range(len(hub_splits[i])):
                splits.append((hub_splits[i][l], None))
            self.hubSplits_.append(splits)
        
        self.shroudSplits_ = []
        for i in range(len(shroud_splits)):
            splits = []
            for l in range(len(shroud_splits[i])):
                splits.append((shroud_splits[i][l], None))
            self.shroudSplits_.append(splits)
        
        # The hub and shroud points of the interfaces are added to the lists of splits
        # they get the ID of the interface i as a descriptor
        # trough this the correct interfaces can be tracked later
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
        # _ctst tracks if a split is made by an interface, different curve segments or a split
       
        # the cti lists will be updated to map all the curves to the regular channels they will be part of
        # every cut curve will be appointed the ID of its respective reg channel defined by its interface
        # None means that the chrve is part of a layer channel
        self.hubCurves_, hub_cti, hub_ctst = self.createSplits(self.hubSplits_, hubCurves, "Hub")
        hub_cti = self.propagate_interface_ids_next(hub_cti, 'Hub')
        
        logging.info("###   Creating Shroud Splits")
        self.shroudCurves_, shroud_cti, shroud_ctst = self.createSplits(self.shroudSplits_, shroudCurves, "Shroud")
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
        
        logging.info("###   Creating Regular Channels")        
        # Generating regular channels from the interfaces
        # here all the channels are defined trough the hub and shroud curves and the interface
        # the last interface builds the last regular channel
        # the regChannels are generated by crating a surface from 4 curves
        #  these curves are the upsteam and downstream boundaries 
        #  and the combined hub and shroud curves of the channel
        self.regChannels_ = []
        # interating trough the interfaces (interface_hub and ..._shroud have to be the same size)
        for ii in range(len(interface_hub)):
            logging.info("# Regular Channel[%i]" % ii)

            vhc = vectorHandlingConstDtCurve()
            # the current interface is always a boundary
            vhc.push_back(analyticCurve.MustDownCast(self.interfaces_[ii]).ptrDtCurve())
            
            if ii == 0:
                # the first regChannel gets the inlet as a boundary curve
                vhc.push_back(analyticCurve.MustDownCast(self.inOutCurves_[0]).ptrDtCurve())
            else:
                # the other regChannels get the previous interface as boundary
                vhc.push_back(analyticCurve.MustDownCast(self.interfaces_[ii-1]).ptrDtCurve())
            
            # finding the hub and shroud curves which belong to the interface
            regCurve = self.findChannelCurves(self.hubCurves_, hub_cti, ii, 'Hub')            
            vhc.push_back(analyticCurve.MustDownCast(regCurve[0]).ptrDtCurve())
            regCurve = self.findChannelCurves(self.shroudCurves_, shroud_cti, ii, 'Shroud')            
            vhc.push_back(analyticCurve.MustDownCast(regCurve[0]).ptrDtCurve())

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
        # speHubCtst is appended too, it will be used to check if the curves were
        #  created by a split or not
        speHub = []
        speHubCtst = []
        for i in range(len(hub_cti)): 
            # it the cti for the curve is none a layer will be created
            if hub_cti[i] == None:
                speHub.append(self.hubCurves_[i])
                speHubCtst.append(hub_ctst[i])

        speShroud = []
        speShroudCtst = []
        for i in range(len(shroud_cti)): 
            if shroud_cti[i] == None:
                speShroud.append(self.shroudCurves_[i])
                speShroudCtst.append(shroud_ctst[i])

        # calculating the centerpoint of the special curves
        # will be later used in order to make sure the layer boundaries are pointing inside the domain
        axis, self.speCenter_ = self.calculateNormalAxis(speHub + speShroud)
        
        logging.info("### Creating Hub Layer Curves")
        # Creating the boundary curves for the layers
        # each layer id defined by four curves those returned through self.hubLayerCurves_
        # the hubLayerCurves_ list has the following format:
        # self.hubLayerCurve = [
        #                        [channel curve, 
        #                        streamwise boundary 1, 
        #                        parallel to channel curve, 
        #                        streamwise boundary 0],
        #                        ...
        #                      ]
        # the curves are placed in the list so the rectangular domain in meridional view is build in clock wise direction
        self.hubLayerCurves_ = self.createLayerBounds(
                speHub,  
                speHubCtst, 
                layer_thickness, 
                layer_supports,
                'Hub'
            )
        logging.info("### Creating Shroud Layer Curves")
        self.shroudLayerCurves_ = self.createLayerBounds(
                speShroud,  
                speShroudCtst, 
                layer_thickness,
                layer_supports,
                'Shroud'
            )

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
        
        logging.info("##############  EOF  ##############")
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
    
    # creates the boundary curves for the layers in the suction area of the hydraulic machine
    # roughly the following tasks are done:
    #   - finds the channel curves which need a boundary (the ones where the cti is None)
    #      and combines them if neccecary
    #   - generates the boundaries which will be normal to the flow direction at the intersect points
    #      of two channel curves (either from a global boundary or a mean normal of the curves)
    #   - calculates the layer boundaries which will be parallel to the channel curves. This is done
    #      trough the end points of the boundaries in streamwise direction and support points on the channel curves
    # the curves are returned in a list
    def createLayerBounds(self, layerCurve, layerCtst, thickness, supports, lab):
        
        # global layer boundaries 
        # the layers will expand between these two boundaries in streamwise direction
        # here the last interface curve and the outlet curve is used 
        boundsGlob = []
        boundsGlob.append(self.interfaces_[-1])
        boundsGlob.append(self.inOutCurves_[1]) 
        
        # layer boundaries between the special curves
        # will extend from the channel curves inside th domain orthogonally to streamwise direction
        layerStreamOrtho = []
        # layerboundaries which will be parallel to channel curves
        layerParallel = []
        # list for return of the boundaries (will be explained later)
        returnBounds = []

        # iterating over all curves and deciding which curves get Layers
        for i in range(len(layerCurve)):
            # checking if the curve has a radius of zero
            rz_0 = self.rz_xyz(layerCurve[i].getPointPercent(0.0))
            rz_1 = self.rz_xyz(layerCurve[i].getPointPercent(1.0))
            onRotAxis_0 = analyticGeometry.inXYZTolerance(rz_0[0])
            onRotAxis_1 = analyticGeometry.inXYZTolerance(rz_1[0])
            logging.debug(
                "speHub: |<- %5.2f --- %5.2f ->|" % (rz_0[0], rz_1[0])
            )
            # if the radius is zero the curve will be assigned as the second global boundary
            # the layers will end here in streamwise direction
            if onRotAxis_0 and onRotAxis_1:                    
                # overwrites the outlet curve as global boundary
                boundsGlob[-1] = layerCurve[i]
                logging.info("Set %s curve no. %i as the global layer boundary" % (lab, i))
                del layerCurve[i]
                break
        
        # iterating over curves and deciding between which curves a layerStreamOrtho curve 
        #  should be created and which curves should be combined
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

            # conditions are an angle less or equal than two degrees 
            #  and both curves are not defined as splits
            # if these conditions are fulfilled the curves will be combined and create one layer
            # this is useful for example if two curves were defined with a steady transition
            # the definition of splits in the ctst list is made in self.CreateSplits()
            is_steady = (
                angle <= 2*(numpy.pi/180) and
                (layerCtst[i] != "split" or
                layerCtst[i + 1] != "split")
            )
            
            # if the condition is fulfilled the curves will be combined
            # no splits will be made here
            if is_steady:
                current_curve = analyticCurve(
                    bSplineCurve_curveConnectConstructOCC(
                        current_curve.ptrDtCurve(),
                        v1.ptrDtCurve()
                    ).result()
                )
                logging.info("Combining curves %i and %i to make one layer" % (i, i+1))
            # else the curves will be appended
            else:
                speCurve.append(current_curve)
                current_curve = v1

        # appending the last curve outside of the loop
        # speCurve will be used in the rest of this method
        speCurve.append(current_curve)
        logging.info("Creating %i Layers on %s" % (len(speCurve), lab))

        # iterating over all curves
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
                        supports
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
                ext = self.layerCurve(layerStreamOrtho, i, v0, thickness, supports)
                layerParallel.append(ext)
                logging.info("Created parallel layer boundary on curve no. %i" % i)

                # appending the curves for return
                # the curves of one layer are appended in clockwise direction
                newLayer = [
                        v0.clone(), 
                        layerStreamOrtho[i].clone(), 
                        layerParallel[i-1].clone(), 
                        layerStreamOrtho[i-1].clone()
                    ]
                returnBounds.append(newLayer)
                logging.info("Finished creating %s layer curves" % lab) 

        return returnBounds 

    # creates the layer curve parallel to the channel curves
    # the curve is created from the two points at 100 percent of the matching 
    #  curves orthogonally to the streamwise direction and offsets from the points 
    #  of the channel curve at the support points
    def layerCurve(self, layerStreamOrtho, i, curve, thickness, supports):
        logging.info("cerating layer curve no. %i" % i) 
        # finding the correct curves for the layer
        # bound is the curve orthogonal to the streamwise direction
        bound0 = analyticCurve.MustDownCast(layerStreamOrtho[i-1]).ptrDtCurve()
        bound1 = analyticCurve.MustDownCast(layerStreamOrtho[i]).ptrDtCurve()
        curve0 = analyticCurve.MustDownCast(curve).ptrDtCurve()
        

        # setting the direction of the offset by checking the direction of the normalvector
        # as insideVector the centerpoint of the layer is used
        layer_list = [layerStreamOrtho[i-1], layerStreamOrtho[i], curve]
        layerBox, layerCenter = self.calculateNormalAxis(layer_list)
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
        # last point
        container = container << bound1.pointPercent(1.0)          

        # creating the curve from the point container
        ext = analyticCurve(
                bSplineCurve_pointConstructOCC(container, 2).result()
            )

        return ext

    def rz_xyz(self, pp: dtPoint3) -> dtPoint2:
        vv = pp - self.origin_
        zz = dtLinearAlgebra.dotProduct(vv, self.rotVector_)
        rr = dtLinearAlgebra.length(vv - self.rotVector_ * zz)
        return dtPoint2(rr, zz)

    def xyz_rz(self, pp: dtPoint3) -> dtPoint3:
        vv = (
            self.origin_
            + self.rotVector_ * pp[1]
            + dtLinearAlgebra.crossProduct(self.normalAxis_, self.rotVector_)
            * pp[0]
        )
        return vv
    
    # creates the splits of the channel curve
    # splits can be made by in the class inputs trough the hub_splits and shroud_splits commands
    # furthermore splits are made by interfaces with interface_hub and interface_shroud
    # this function splits the specified curves at the specified percentages
    # it is possible to make multiple splits on one curve
    # furthermore it tracks which of the split curves are part of an interface and if the 
    #  split is made with a split or an interface or if the curve is an unsplit curve segment 
    @staticmethod
    def createSplits(splits, inCurves, lab): 
        # list with the split curves
        outCurves = []
        # mapping of all the curves and their corresponding interface IDs
        curve_to_interface = []
        # mapping all the curves and what kind of split is made
        curve_to_splitType = []
        
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
                # s[1] will contain the interface number if the split is made
                #  by an interface or None if it is made by a split
                interface_ids = [s[1] for s in splits[cc]]

                # iterating over the splits on this curve
                for n in range(len(split_pos)-1):

                    # exception for when a interface is located on 0 or 100 percent of a curve
                    # doesnt make a split just corrects the cti
                    if split_pos[n] == split_pos[n+1]:
                        # sets the id of this domain
                        iface_id = interface_ids[n] if n < len(interface_ids) else interface_ids[-1]
                        curve_to_interface[-1] = iface_id
                        # sets a identifier string if the split is made by an interface or a split
                        if type(iface_id) == int:
                            curve_to_splitType[-1] = "interf"
                        else:
                            curve_to_splitType[-1] = "split"

                    # exception for if the whole curve has a split or interface at 100 percent    
                    # doesnt make a split but appends outCurves and curve_to_interface
                    elif (split_pos[n] == 0 and split_pos[n+1] == 1):
                        outCurves.append(inCurves[cc].clone())                
                        curve_to_interface.append(None)
                        if type(iface_id) == int:
                            curve_to_splitType.append("interf")
                        else:
                            curve_to_splitType.append("split")

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
                        if type(iface_id) == int:
                            curve_to_splitType.append("interf")
                        else:
                            curve_to_splitType.append("split")
                    
                    if iface_id == None:
                        logging.debug(
                            "Split %s[%d] [%5f, %5f]" % (lab, cc, split_pos[n], split_pos[n+1])
                        )
                    else:
                        logging.debug(
                            "Interface No. %d, %s[%d] [%5f, %5f]" % (iface_id, lab, cc, split_pos[n], split_pos[n+1])
                        )
            # if the curve is not split the whole curve will be copied        
            else:
                outCurves.append(inCurves[cc].clone())                
                logging.debug("Copy %s[%d]" % (lab, cc))
                # adds none to curve to interface
                curve_to_interface.append(None)
                # as string identifier "segm" will be set
                curve_to_splitType.append('segm')

        logging.info("Finished splitting %s curves. %i curves created" % (lab, len(outCurves)))
        # returns the cut curves and the cti    
        return outCurves, curve_to_interface, curve_to_splitType    
    
    #
    # calculate the normal axis of the whole domain 
    #
    @staticmethod
    def calculateNormalAxis(curves):
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
        return normalAxis, bbCenter
    
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
            ii = 0
            for regChannel in self.regChannels_:
                self.appendAnalyticGeometry(
                    regChannel.clone(),
                    "debug_regChannel_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
            ii = 0
            for interface in self.interfaces_:
                self.appendAnalyticGeometry(
                    interface.clone(),
                    "debug_interface_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
            ii = 0
            for layer in self.hubLayers_:   
                self.appendAnalyticGeometry(
                    layer.clone(),
                    "debug_hubLayer_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
            ii = 0
            for layer in self.shroudLayers_:
                self.appendAnalyticGeometry(
                    layer.clone(),
                    "debug_shroudLayer_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
            ii = 0
            for aCurve in self.shroudCurves_:
                self.appendAnalyticGeometry(
                    aCurve.clone(),
                    "debug_shroudCurve_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
            ii = 0
            for aCurve in self.hubCurves_:
                self.appendAnalyticGeometry(
                    aCurve.clone(),
                    "debug_hubCurve_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
            ii = 0
            for k in range(len(self.hubLayerCurves_)):
                for c in range(len(self.hubLayerCurves_[k])) :
                    self.appendAnalyticGeometry(
                    self.hubLayerCurves_[k][c].clone(),
                    "debug_hubLayerCurve_" + str(k) + str(c) + "_" + self.label_,
                )

            for k in range(len(self.shroudLayerCurves_)):
                for c in range(len(self.shroudLayerCurves_[k])) :
                    self.appendAnalyticGeometry(
                    self.shroudLayerCurves_[k][c].clone(),
                    "debug_shroudLayerCurve_" + str(k) + str(c) + "_" + self.label_,
                )
            for aCurve in self.inOutCurves_:
                self.appendAnalyticGeometry(
                    aCurve.clone(),
                    "debug_inOutCurve_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
            ii = 0
            # defines which channels will be rotatetd for the debug geometries
            for channel in (                
                self.regChannels_
                + self.hubLayers_
                + self.shroudLayers_
            ):
                off = float(ii) * 0.10      # rotational offset of each channel
                self.appendAnalyticGeometry(
                    partRotatingMap2dTo3d(
                        self.rotVector_,
                        channel,
                        off - 0.05,         # min segment in radian 
                        off + 0.05,         # max segment in radian
                    ).clone(),
                    "debug_channel_" + str(ii) + "_" + self.label_,
                )
                ii = ii + 1
        return
    
    #
    # returns the rotated segment of the channel number
    #
    def regChannel(self, pos: int) -> analyticGeometry:
        logging.info("Request regChannel[ %d ]" % pos)
        return rotatingMap2dTo3d(
            self.rotVector_, self.regChannels_[pos]
        ).clone()
        # rm.thisown = False
        # logging.info("%s" % rm)
        # return rm

    def addInternals(
        self,
        internals: List[analyticGeometry],
    ) -> None:
        logging.info("Adding %d internals" % len(internals))
        for internal in internals:
            self.internals_.append(map3dTo3d.MustDownCast(internal))
