from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
  map3dTo3d,
  map1dTo3d,
  dtPoint3 ,
  bSplineCurve_pointConstructOCC,
  vectorDtPoint3,
  labeledVectorHandlingAnalyticGeometry,
  vec3dCurveOneD,
  vec3dOneDInMap3dTo3d,
  analyticGeometry,
  vec3dOneD,
  trans4SidedFace
)
import logging

class analyticSurface_inOutFeMeanplane(dtBundleBuilder):
    """Create the meanplane faces connecting to the interfaces of the regular channel.

    This class:

        - Gets the offset meanplane curves from ``aG_``.
        - Creates curves on the hub and shroud extending from the offset curves to the 
          respective interfaces along the hub and shroud contours.
        - Creates curves on the interfaces connecting the hub and shroud curves.
        - Creates meanplane faces extending to the inlet and outlet from the generated curves.

    Attributes
    ----------
    prefix_: str
      Prefix of label
    label_: str
      Label.
    channel_: map3dTo3d
      Channel.
    aG_: labeledVectorHandlingAnalyticGeometry
      Mesh block curves.
    Returns
    -------
    None

    Examples
    --------

    >>> import dtOOPythonSWIG as dtOO
    
    Create three dimensional channel domain.

    >>> c0 = dtOO.bSplineCurve_pointConstructOCC(
    ...       dtOO.vectorDtPoint3()
    ...         << dtOO.dtPoint3(+1.00, +0.00, 0.50)
    ...         << dtOO.dtPoint3(+0.50, +0.00, 0.50),
    ...         1
    ...     ).result()
    >>> c1 = dtOO.bSplineCurve_pointConstructOCC(
    ...       dtOO.vectorDtPoint3()
    ...         << dtOO.dtPoint3(+0.50, +0.00, 0.50)
    ...         << dtOO.dtPoint3(+0.30, +0.00, 0.25)
    ...         << dtOO.dtPoint3(+0.50, +0.00, 0.00),
    ...         2
    ...     ).result()
    >>> c2 = dtOO.bSplineCurve_pointConstructOCC(
    ...       dtOO.vectorDtPoint3()
    ...         << dtOO.dtPoint3(+0.50, +0.00, 0.00)
    ...         << dtOO.dtPoint3(+1.00, +0.00, 0.00),
    ...         1
    ...     ).result()
    >>> c3 = dtOO.bSplineCurve_pointConstructOCC(
    ...       dtOO.vectorDtPoint3()
    ...         << dtOO.dtPoint3(+1.00, +0.00, 0.00)
    ...         << dtOO.dtPoint3(+1.00, +0.00, 0.50),
    ...         1
    ...     ).result()
    >>> channel2d = dtOO.analyticSurface(
    ...         dtOO.bSplineSurface_bSplineCurveFillConstructOCC(
    ...             c0, c1, c2, c3
    ...         ).result()
    ...     )
    >>> channel = dtOO.rotatingMap2dTo3d(
    ...             dtOO.dtVector3(0,0,1),
    ...             channel2d,
    ...         )
    
    Create the mesh block curves.
    
    >>> c_in1 = dtOO.bSplineCurve_pointConstructOCC(
    ...           dtOO.vectorDtPoint3()
    ...             << dtOO.dtPoint3(+0.90, +0.20, 0.00)
    ...             << dtOO.dtPoint3(+0.90, +0.20, 0.50),
    ...             1
    ...         ).result()
    >>> c_out1 = dtOO.bSplineCurve_pointConstructOCC(
    ...           dtOO.vectorDtPoint3()
    ...             << dtOO.dtPoint3(+0.60, +0.05, 0.00)
    ...             << dtOO.dtPoint3(+0.65, -0.05, 0.50),
    ...             1
    ...         ).result()
    
    Push the mesh block curves into a vector handler.
    
    >>> meshBlockCurves = dtOO.labeledVectorHandlingAnalyticGeometry()
    >>> label = "test"    
    >>> meshBlockCurves.push_back(
    ...     dtOO.analyticCurve( c_in1 ) << "xyz_"+label+"_meshBlockCurve_in1"
    ... )
    >>> meshBlockCurves.push_back(
    ...     dtOO.analyticCurve( c_out1 ) << "xyz_"+label+"_meshBlockCurve_out1"
    ... )
    
    Create the meanplane faces between the interfaces and the mesh block curves.
    
    >>> from dtOOPythonApp.builder import analyticSurface_inOutFeMeanplane 
    >>> feMeanplane = analyticSurface_inOutFeMeanplane(
    ...     prefix = "xyz",
    ...     label = label,
    ...     channel = channel,
    ...     curves = meshBlockCurves
    ... )
    >>> feMeanplane.build()

    Check the label of the last generated geometry.
    
    >>> feMeanplane.lVH_aG().labels()[-1]
    'xyz_test_fe_meanplane_out1'
   

    The main method of this class is :meth:`build`, where all operations of this 
    class are performed.

    The meanplane faces are created as objects of the ``dtOO`` class 
    ``trans4SidedFace``. They extend between the interfaces and the corresponding 
    offset mesh block curves, which are created in the class 
    ``vec3dThreeD_skinAndSplit``.

    The channel is converted to the type ``map3dTo3d`` and instantiated as 
    ``channel_``. It has the following parametric directions:

        - u : circumferential direction
        - v : meridional direction
        - w : hub-to-shroud direction

    The interfaces represent the inlet and outlet of the channel domain.
    The inlet is located at 0% and the outlet at 100% of the v-coordinate 
    of the channel.
    The hub is located at 0% and the shroud at 100% of the w-coordinate.

    The string values of ``prefix_`` and ``label_`` are used to manage the 
    names of the geometry objects in this class.

    The offset meanplane curves are passed to the class through ``curves``. 
    This container, of type ``labeledVectorHandlingAnalyticGeometry``, is 
    instantiated as ``aG_``. The curves extend from the hub to the shroud of 
    the channel and follow the naming convention below:

        - Meanplane curve offset toward the inlet:
          ``prefix_+"_"+label_+"_meshBlockCurve_in1"``
        - Meanplane curve offset toward the outlet:
          ``prefix_+"_"+label_+"_meshBlockCurve_out1"``

    The offset meanplane curves are shown in the following figure.

    .. _offsetCurves:
    .. figure:: bladeFigs/inOutFEMeanplane_offsetCurves.png
       :width: 100%
       :align: center

       Offset meanplane curves (blue). Labels correspond to the established
       naming convention.

    The class ``trans4SidedFace`` requires a closed loop of four bounding curves 
    with consistent directions.
    The following curves are used for the interfaces:

        - Edge extending between the offset meanplane curve and the interface on the hub
        - Offset meanplane curve
        - Edge extending between the meanplane curve and the interface on the shroud
        - Edge extending between the hub and shroud edges on the interface

    The curves have the following locations, directions, and names:

    .. _boundCurveTable:
    .. csv-table:: Boundary curves of the ``trans4SidedFace``
       :header: "Location", "From -> To", "Name in :meth:`build`"
       :escape: #
       :align: center

       hub              , interface -> meanplane curve  , ``hsCurve_u0_<in/out>``
       meanplane curve  , hub -> shroud                 , see above
       shroud           , meanplane curve -> interface  , ``hsCurve_u1_<in/out>``
       interface        , shroud -> hub                 , ``interfCurve_<in/out>``

    By passing the curves to the constructor of ``trans4SidedFace`` in the order 
    established in :numref:`boundCurveTable`, the resulting parameter directions 
    are defined as follows:

        - u : direction of the hub curve
        - v : direction of the meanplane curve

    The following figure shows the ``trans4SidedFace`` objects and their bounding 
    curves.

    .. _trans4SidedFace:
    .. figure:: bladeFigs/inOutFEMeanplane_trans4SidedFace.png
       :width: 100%
       :align: center

       Boundary curves (blue) and resulting ``trans4SidedFace`` (yellow). Arrows 
       correspond to the curve directions, and labels correspond to the locations 
       listed in :numref:`boundCurveTable`.

    The created geometries are returned to the analytic geometry container of the 
    calling class.
    """
    
    def __init__(
        self,
        prefix: str,
        label: str,
        channel: analyticGeometry,
        curves: labeledVectorHandlingAnalyticGeometry
      ) -> None:
        
        """ Constructor

        Parameters 
        ----------
        prefix: str
          Prefix of label
        label: str
          Label.
        channel: analyyticGeometry
          Channel.
        curves: labeledVectorHandlingAnalyticGeometry
          Mesh block curves.
        Returns
        -------
        None
         
        """
        logging.info( "Initializing %s ..." % (label) )
        super(
          analyticSurface_inOutFeMeanplane, self
        ).__init__()
        self.prefix_ = prefix
        self.label_ = label
        self.channel_ = map3dTo3d.MustDownCast(channel)
        self.aG_ = curves
    
    def build(self) -> None:
        """Build part.

        This method:

            - Gets the offset meanplane curves from ``aG_``.
            - Creates curves on the hub and shroud extending from the offset curves to the 
              respective interfaces along the hub and shroud contours.
            - Creates curves on the interfaces connecting the hub and shroud curves.
            - Creates meanplane faces from the generated curves.

        Parameters
        ----------
        None

        Returns
        -------
        None


        A meanplane face is created at the inlet and outlet interfaces in a loop that 
        iterates over ``mpCurveList``. This list has the format 
        ``List[Tuple[str, int]]``.
        Two entries are defined in the list:

            - ``mpCurveList[0]`` : inlet data
            - ``mpCurveList[1]`` : outlet data

        The tuple entries encode an identifier string 
        ``mpCurveList[oc][0]`` and the normalized parameter coordinate 
        ``mpCurveList[oc][1]`` of the interface.

        The following diagram shows the activities performed in this method.

        .. _inOutFEMeanplane:
        .. figure:: bladeFigs/analyticSurface_inOutFEMeanplane.png
           :width: 100%
           :align: center

           Activities during the creation of the meanplane face.
        
        **Iterate over Inlet and Outlet**

            The offset meanplane curve of the current interface is allocated to ``offC``. 
            The v-parameter of the channel interface is allocated to ``vChannel``.
            A point container ``interfPoints`` is initialized as a 
            ``vectorDtPoint3`` object.
        
            **Iterate over Hub and Shroud Positions**

                The bounding curves extending along the hub and shroud are created in a loop 
                over the parameter coordinates ``uu in [1, 0]``.
            
                **Create Points on the current Position**

                    The hub and shroud curves are created from the points ``pCurve_uvw`` and 
                    ``pChannel_uvw``. ``pCurve_uvw`` is the point on ``offC`` at the current 
                    value ``uu``. It is reparameterized in the parametric space of the channel 
                    ``channel_``.

                    The point ``pChannel_uvw`` is created with the same u-coordinate as ``pCurve_uvw``, while 
                    the v- and w-coordinates correspond to the coordinates of the current interface 
                    on the channel's hub or shroud position.

                    The following figure shows the points created in the iterations over the two 
                    loops.

                    .. _points:
                    .. figure:: bladeFigs/inOutFEMeanplane_points.png
                       :width: 100%
                       :align: center

                       Points which are created (blue). The labeled points 
                       ``pCurve_uvw`` and ``pChannel_uvw`` correspond to the points created 
                       at the outlet (``oc == 1``) on the hub contour (``uu == 0``).

                    The point ``pChannel_uvw`` is appended to ``interfPoints`` in each iteration.
                    Through the iteration over ``uu in [1, 0]``, the locations of the points in 
                    this container are as follows:

                        - ``interfPoints[0]`` : shroud
                        - ``interfPoints[1]`` : hub
                
                **Create Hub or Shroud Curve**

                    From the points ``pCurve_uvw`` and ``pChannel_uvw``, the hub or shroud curve 
                    ``hsCurve`` is created.
                    Depending on whether the current iteration creates the hub or the shroud curve 
                    (``uu == 0`` or ``uu == 1``), the direction of the curve is reversed.
                    This results in the hub and shroud curve directions specified in 
                    :numref:`boundCurveTable`.
                    The curves are reparameterized in xyz-coordinates and pushed into ``aG_`` with 
                    the following naming convention:

                    ::

                        "hsCurve_"+"u"+str(uu)+"_"+str(mpCurveList[oc][0])

            **Check if the Interface Curve Extends over u = 100% of the Channel**

                The bounding curve on the interface is created from the points in 
                ``interfPoints`` on the hub and shroud walls.
                To enable the extension of the interface meanplane surface across 0% of the channel 
                u-coordinate, a check is implemented that detects jumps in the u-coordinates 
                ``u1`` and ``u2`` of ``interfPoints[0]`` and ``interfPoints[1]``.

                If the normalized u-parameter range of these two points in the channel is 
                greater than 50% (``abs(u1-u2) > 0.5``), the larger of the two values is 
                subtracted by one. This shifts the value into the negative parameter range.
                The shifted parameter is reassigned to ``interfPoints``.
            
            **Create the Interface Curve**

                The interface curve ``interfCurve`` is created from the points in 
                ``interfPoints`` and mapped into ``channel_`` as ``interfCurveInChannel``.
                Due to the definition of ``interfPoints``, the resulting curve extends from 
                the shroud to the hub walls of the channel.
                It is pushed into ``aG_`` with the following naming convention:

                ::

                    "interfCurve_"+str(mpCurveList[oc][0])
            
            **Create the Meanplane Face**

                The meanplane faces are constructed as ``trans4SidedFace`` objects (see 
                :numref:`trans4SidedFace`) using the curve sequence established in 
                :numref:`boundCurveTable`. The bounding curves are retrieved from 
                ``aG_`` by their names.

                The face is returned to the geometry container of the calling class with the 
                following name:

                ::

                    prefix_+"_"+label_+"_fe_meanplane_"+mpCurveList[oc][0]+str(1)
        """

        #
        # create list encoding the inlet and outlet regions
        # iterate over the list
        #
        mpCurveList = [["in", 0], ["out", 1]]
        for oc in range(len(mpCurveList)):
            
            # get current offset curve
            offC = map1dTo3d.MustDownCast(
                    self.aG_[self.prefix_+"_"+self.label_+"_meshBlockCurve_"+mpCurveList[oc][0]+"1"]
                )
            
            # v coordinate at outlet or inlet
            vChannel = self.channel_.v_percent(mpCurveList[oc][1])
            
            # prepare point container
            interfPoints = vectorDtPoint3()
            
            #
            # calculate points and curves at hub and shroud
            # shroud: uu = 1
            # hub: uu = 0 
            #
            # the shroud is computed first and the hub second so interfPoints is 
            # filled so the resulting interfCurve extends from shroud to hub
            #
            for uu in [1, 0]:
                logging.info( "Meanplane extention curve at: %slet , v = %d" %(mpCurveList[oc][0], uu)  )
                 
                # get the point on the offset curve and reparamtrize it in the channel
                # returns the uvw coordinates of the point in the channel
                pCurve_uvw = self.channel_.reparamInVolume(offC.getPointPercent(uu)) 
                
                # shift the point on the offset curve to the current interface
                pChannel_uvw = dtPoint3(pCurve_uvw.x(), vChannel, self.channel_.w_percent(uu))
                interfPoints.append( pChannel_uvw )
                
                #
                # create hub and shroud curves
                # directions have to be reversed at hub and shroud
                #
                # at hub
                if uu == 0:
                    hsCurve = bSplineCurve_pointConstructOCC(
                            vectorDtPoint3()
                              << pChannel_uvw
                              << pCurve_uvw,
                            1
                        ).result()
                # at shroud
                elif uu == 1:
                    hsCurve = bSplineCurve_pointConstructOCC(
                            vectorDtPoint3()
                              << pCurve_uvw
                              << pChannel_uvw,
                            1
                        ).result()
                # parametrize uvw hub/shroud curves in the xyz channel 
                hsCurveInChannel = vec3dOneDInMap3dTo3d(
                        vec3dOneD.MustConstDownCast(
                            vec3dCurveOneD(
                                hsCurve
                            ).clone()
                        ), 
                        self.channel_
                    )

                # push it into aG_
                # naming convention hsCurve_u0_in
                # hs -> hub or shroud
                # u0 -> u coordinate on curve 0 -> hub, 1 -> shroud
                # in / out at inlet or outlet
                self.aG_.push_back(
                        hsCurveInChannel << "hsCurve_"+"u"+str(uu)+"_"+str(mpCurveList[oc][0])
                    )
            
            #
            # exception if the interface curve extends over the u coordinate of 100% (or 0%)
            # checking the percentage of the u coordinate
            #
            u1 = self.channel_.percent_u(interfPoints[0].x())
            u2 = self.channel_.percent_u(interfPoints[1].x())
            
            # if the u coordinates of the start and end points of the interface curve are
            #  far apart (blade stretches over u = 100% of the channel) 
            if abs(u1 - u2) > 0.50:
                logging.info("WARNING: Blade %s extends over 0.5 of channel u" % self.label_)
                logging.info("uShroud = %f, uHub = %f" %(u1, u2))
                # shroud point is at a greater u coordinate
                if u1 > u2:
                    logging.info("Changing shroud point to extend over u = 1 of channel")
                    # calculate the negative u coordinate
                    u1 =  u1 - 1
                    logging.info("uShroud = %f" % u1)
                    # change the point to contain negative u coordinate
                    interfPoints[0] = dtPoint3(
                            self.channel_.u_percent(u1), 
                            interfPoints[0].y(), 
                            interfPoints[0].z()
                        )
                else:
                    logging.info("Changing hub point to extend over u = 100% of channel")
                    # other way round if hub point has the greater u value
                    u2 = u2 - 1
                    logging.info("uHub = %f" % u2)
                    interfPoints[1] = dtPoint3(
                            self.channel_.u_percent(u2), 
                            interfPoints[1].y(), 
                            interfPoints[1].z()
                        )
                
            # create the interface curve
            interfCurve = bSplineCurve_pointConstructOCC(
                    interfPoints,
                    1
                ).result()
            
            # mapp it to the channel and pushing it into aG_
            interfCurveInChannel = vec3dOneDInMap3dTo3d(
                    vec3dOneD.MustConstDownCast(
                        vec3dCurveOneD(
                            interfCurve
                        ).clone()
                    ),
                    self.channel_
                )
            self.aG_.push_back(
                    interfCurveInChannel << "interfCurve_"+str(mpCurveList[oc][0])
                ) 
            
            # create the meanplane face at the outlet as a trans4SidedFace
            self.aG_.set(
              trans4SidedFace(
                map1dTo3d.MustDownCast( self.aG_["hsCurve_u0_"+mpCurveList[oc][0]] ),
                map1dTo3d.MustDownCast( self.aG_[self.prefix_+"_"+self.label_+"_meshBlockCurve_"+mpCurveList[oc][0]+"1"] ),
                map1dTo3d.MustDownCast( self.aG_["hsCurve_u1_"+mpCurveList[oc][0]] ),
                map1dTo3d.MustDownCast( self.aG_["interfCurve_"+mpCurveList[oc][0]] )
              ) << self.label_+"_fe_meanplane_"+mpCurveList[oc][0]+str(1)
            )
            # append it to the geometry
            self.appendAnalyticGeometry(
                    self.aG_[self.label_+"_fe_meanplane_"+mpCurveList[oc][0]+str(1)],
                    self.prefix_+"_"+self.label_+"_fe_meanplane_"+mpCurveList[oc][0]+str(1)
                )
            
        #
        # add debug faces and lines
        #
        if self.debug():
            for jj in ["interfCurve_*", "hsCurve_*"]:
            #for jj in ["hsCurve_*"]:
                for iNum in self.aG_.getIndices(jj):
                    ii = self.aG_.getLabel( iNum )
                    self.appendAnalyticGeometry(
                        self.aG_[ii],
                        "debug_"+ii
                    )

        return

