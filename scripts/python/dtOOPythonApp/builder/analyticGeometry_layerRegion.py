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
    """Create layer channel as six sided regions and a multiple bounded
    Volume.
    
    On the hub and shroud curves of the channel six sided layers are 
    created. A multiple bounded volume is created connecting the hub and
    the shroud layers inside the flow channel.
    The layers are defined by the layer thickness and the support points
    on procentual positions along the hub and shroud curves.
    The multiple bounded volume is defined trough the inner boundaries
    of the layers and the inlet and outlet curves of the region.
    
    The inputs are the hub and shroud wall curves as well as the inelt and
    the outlet of the layer region. The inlet of this region is defined in 
    the creation of the meridional contour trough the last interface curve.

    The multi bounded volume and its boundary faces are returned by: 
    getUnstructuredRegion(nSlices) -> analyticGeometry, List[analyticGeometry]

    The wall layers are returned by the methods:
    getLayerList(nSlices)  -> List[List[List[analyticGeometry] | List[bool]]]
    
    The List returned by getLayerList has the following format:
    layers = [[hub layer lists],[shroud layer list]]
    with:
    [layer lists] = [[3d layer domain], [bool list radius zero]]
    
    The inputs nSlices signifies the number of slices in which the whole 360° domain 
    will be segmented. The resulting angle of the returned segment is as follows:
    $[ rotAngle = 2*numpy.pi/nSlices ]$

    Attributes
    ----------
    label_: str
      Label.
    rotVector_: dtVector3
      Rotation vector.
    origin_: dtPoint3
      Origin.
    normalAxis_: dtVector3
      Normal direction of the 2d region curves
    speCenter_: dtPoint3
      Center point of the bounding box of the region
    speBb_: pairDtPoint3
      Bounding box of the whol region
    hubLayerCurves_: List[analyticGeometry]
      Boundary curves of the hub Layer regions
    hubRadZero_: List[Bool]
      List containing information about which hub curves are on a radius of zero
    hubUnstructBounds_: List[List[analyticGeometry]]
      Boundary curves of the multiple bounded volume at the hub
    shroudLayerCurves_: List[List[analyticGeometry]]
      Boundary curves of the shroud Layer regions
    shroudRadZero_: List[Bool]
      List containing information about which shroud curves are on a radius of zero
    shroudUnstructBounds_: List[analyticGeometry]
      Boundary curves of the multiple bounded volume at the shroud
    interfaceUnstructBound_: analyticGeometry
      Inlet boundary curve of the multiple bounded volume
    unstructVH_: vectorHandlerAnalyticGeometry
      vector handler containing all boundary curves of the multiple bounded volume.
    hubLayers_: List[analyticGeometry]
      List containing the faces of the six sided hub layers
    shroudLayers_: List[analyticGeometry]
      List containing the faces of the six sided shroud layers

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

        """
        logging.info("Initializing %s ..." % (label))
        super(analyticGeometry_layerRegion, self).__init__()
        self.label_ = label
        
        self.rotVector_ = dtLinearAlgebra.normalize(rotVector)
        self.origin_ = origin

        # calculating the centerpoint of the special curves
        # will be later used in order to make sure the layer boundaries are pointing inside the domain
        # the bounding box of the special curves self.speBb_ will be used to generate 
        #  the multi bounded volume of the unstructured region
        self.normalAxis_, self.speCenter_, self.speBb_ = self.calculateNormalAxis(speHub + speShroud)
        
        logging.info("### Creating Hub Layer Curves")
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
        # the curves are placed in the list so the rectangular domain in meridional view is build in clock wise direction
        # 
        # self.hubRadZero_ contains booleans which will show true for layers which have a radius of zero
        #  this is used for meshing later
        #
        # self.hubUnstructBounds_ contains all the hub  boundaries of the unstructured region 
        #  these will be the curves parallel to the channel curves or the channel curves if they are on
        #  a radius of zero
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
        
        # trimming the last interface curve between the layerthicknesses
        #  this curve will be used as a boundary for the unstructured region
        interf = analyticCurve.MustDownCast(inOutCurves[0]).ptrDtCurve()
        self.interfaceUnstructBound_ = analyticCurve(
                trimmedCurve_uBounds(
                    interf, 
                    interf.u_l(layer_thickness), 
                    interf.u_l(interf.length() - layer_thickness)
                ).result()
            )
        
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
         
    # creates the boundary curves for the layers in the suction area of the hydraulic machine
    # roughly the following tasks are done:
    #   - finds the channel curves which need a boundary (the ones where the cti is None)
    #      and combines them if neccecary
    #   - generates the boundaries which will be normal to the flow direction at the intersect points
    #      of two channel curves (either from a global boundary or a mean normal of the curves)
    #   - calculates the layer boundaries which will be parallel to the channel curves. This is done
    #      trough the end points of the boundaries in streamwise direction and support points on the channel curves
    # the curves are returned in a list
    def createLayerBounds(self, layerCurve, inOutCurves, thickness, supports, lab):
        """Create boundary curve on the hub and shroud layer curves.

        Parameters
        ----------
        layerCurve: List[analyticGeometry]
          List of wall curves
        inOurCurves: List[analyticGeometry]
          list with inlet and outlet curve
        thickness: float
          layer thickness
        supports: List[float]
          List with positions of support points in percent along wall curve
        lab: string
          label
        
        Returns
        -------
        returnBounds: List[List[analyticGeometry]]
          boundary curves of each layer on the wall curves
        on_rad_zero: List[Bool]
          information for each wall curve if the radius is on Zero
        layerParallel: List[analyticGeometry]
          Inner boundary curves of the Layers, used for the multiple bounded volume
        """
        
        boundsGlob = inOutCurves.copy()
        # layer boundaries between the special curves
        # will extend from the channel curves inside the domain, orthogonally to streamwise direction
        layerStreamOrtho = []
        # layerboundaries which will be parallel to channel curves
        layerParallel = []
        # list for return of the boundaries (will be explained later)
        returnBounds = []
        # list which tracks if the layer will endo on a radius of zero 
        # will be used for mesh generation later
        on_rad_zero = []

        unstructOnRotAxis = None
        #unstructBounds = []
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
            else:
                on_rad_zero.append(False)
        
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
        vv = pp - self.origin_
        zz = dtLinearAlgebra.dotProduct(vv, self.rotVector_)
        rr = dtLinearAlgebra.length(vv - self.rotVector_ * zz)
        return dtPoint2(rr, zz)

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

