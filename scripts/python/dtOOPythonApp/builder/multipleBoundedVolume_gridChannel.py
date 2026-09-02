from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
    analyticGeometry,
    analyticSurface,
    rectangularTrimmedSurface_curveRotateConstructOCC,
    rotatingMap1dTo3d,
    partRotatingMap1dTo3d,
    dtPoint3,
    analyticCurve,
    map2dTo3d,
    map3dTo3d,
    dtVector3,
    dtLinearAlgebra,
    labeledVectorHandlingAnalyticGeometry,
    vectorHandlingAnalyticGeometry,
    infinityMap3dTo3d,
    multipleBoundedVolume,
    multipleBoundedSurface,
    partRotatingMap2dTo3d,
    rotate,
    jsonPrimitive
    #bSplineCurve_curveConnectConstructOCC,
)

from typing import List, Tuple
import logging

import numpy as np

class multipleBoundedVolume_gridChannel(dtBundleBuilder):
    """ Create the grid channel as a multiple bounded volume.
    
    This class:
        
        - Creates multiple bounded surfaces on the hub and shroud.
        - Creates bounding faces from the meanplane and coupling faces.
        - Creates a multiple bounded volume of the grid channel.

    Attributes
    ----------
    label_: str
      Label.
    channel_: analyticGeometry
      360° rotated channel domain
    meanplanes_: List[analyticGeometry]
      List of meanplane faces
    couplings_: List[analyticGeometry]
      List of coupling faces
    nBlades_: int
      Number of blades
    nInOutSurf_: int
      Number of meanplane faces extending from the mesh blocks
      to the inlet and outlet each
    rotVector_: dtVector3
      Rotation vector of the grid channel.
    orientation_: int
      Orientation of the blade in the channel.
      
        - 1 : Blade is oriented in u-direction of channel
        - -1 : Blade is oriented in negative u-direction of the channel

    boundSurf_: labeledVectorHandlingAnalyticGeometry
      Container for bounding surfaces.
    gridChannel_: analyticGeometry
      Volume of the grid channel.


    Examples
    --------

    >>> import dtOOPythonSWIG as dtOO
    
    Build channel geometry

    >>> channel = dtOO.rotatingMap2dTo3d(
    ...     dtOO.dtVector3(0,0,1),
    ...     dtOO.analyticSurface(
    ...         dtOO.bSplineSurface_bSplineCurveFillConstructOCC(
    ...             dtOO.bSplineCurve_pointConstructOCC(
    ...                 dtOO.vectorDtPoint3()
    ...                   << dtOO.dtPoint3(+1.00, +0.00, 0.50)
    ...                   << dtOO.dtPoint3(+0.50, +0.00, 0.50),
    ...                 1
    ...             ).result(),
    ...         dtOO.bSplineCurve_pointConstructOCC(
    ...              dtOO.vectorDtPoint3()
    ...                << dtOO.dtPoint3(+0.50, +0.00, 0.50)
    ...                << dtOO.dtPoint3(+0.50, +0.00, 0.00),
    ...                1
    ...            ).result(),
    ...         dtOO.bSplineCurve_pointConstructOCC(
    ...              dtOO.vectorDtPoint3()
    ...                << dtOO.dtPoint3(+0.50, +0.00, 0.00)
    ...                << dtOO.dtPoint3(+1.00, +0.00, 0.00),
    ...                1
    ...            ).result(),
    ...         dtOO.bSplineCurve_pointConstructOCC(
    ...                   dtOO.vectorDtPoint3()
    ...                     << dtOO.dtPoint3(+1.00, +0.00, 0.00)
    ...                     << dtOO.dtPoint3(+1.00, +0.00, 0.50),
    ...                     1
    ...             ).result(),
    ...         ).result()
    ...     )
    ... )

    Define meanplane curves

    >>> c_mp0 = dtOO.bSplineCurve_pointConstructOCC(
    ...     dtOO.vectorDtPoint3()
    ...       << dtOO.dtPoint3(+0.50, +0.00, 0.50)
    ...       << dtOO.dtPoint3(+0.50, +0.00, 0.00),
    ...     1
    ... ).result()
    >>> c_mp1 = dtOO.bSplineCurve_pointConstructOCC(
    ...     dtOO.vectorDtPoint3()
    ...       << dtOO.dtPoint3(+0.55, +0.10, 0.50)
    ...       << dtOO.dtPoint3(+0.60, +0.10, 0.00),
    ...     1
    ... ).result()
    >>> c_mp2 = dtOO.bSplineCurve_pointConstructOCC(
    ...     dtOO.vectorDtPoint3()
    ...       << dtOO.dtPoint3(+0.90, +0.10, 0.50)
    ...       << dtOO.dtPoint3(+0.95, +0.10, 0.00),
    ...     1
    ... ).result()
    >>> c_mp3 = dtOO.bSplineCurve_pointConstructOCC(
    ...     dtOO.vectorDtPoint3()
    ...       << dtOO.dtPoint3(+1.00, +0.00, 0.50)
    ...       << dtOO.dtPoint3(+1.00, +0.00, 0.00),
    ...     1
    ... ).result()
    
    Define coupling curve

    >>> c_coup = dtOO.bSplineCurve_pointConstructOCC(
    ...     dtOO.vectorDtPoint3()
    ...       << dtOO.dtPoint3(+0.85, +0.15, 0.50)
    ...       << dtOO.dtPoint3(+0.85, +0.25, 0.00),
    ...     1
    ... ).result()

    Create meanplane faces in the channel from the curves

    >>> mp0 = dtOO.analyticSurface(
    ...         dtOO.bSplineSurface_exchangeSurfaceConstructOCC(
    ...             dtOO.bSplineSurface_skinConstructOCC(
    ...                 c_mp1, c_mp0
    ...             ).result()
    ...         ).result()
    ...     )
    >>> mp1 = dtOO.analyticSurface(
    ...         dtOO.bSplineSurface_exchangeSurfaceConstructOCC(
    ...             dtOO.bSplineSurface_skinConstructOCC(
    ...                 c_mp1, c_mp2
    ...             ).result()
    ...         ).result()
    ...     )
    >>> mp2 = dtOO.analyticSurface(
    ...         dtOO.bSplineSurface_exchangeSurfaceConstructOCC(
    ...             dtOO.bSplineSurface_skinConstructOCC(
    ...                 c_mp2, c_mp3
    ...             ).result()
    ...         ).result()
    ...     )
    
    Create coupling faces from meanplane curves and coupling curve 

    >>> coup0 = dtOO.analyticSurface(
    ...             dtOO.bSplineSurface_skinConstructOCC(
    ...                     c_mp2, c_coup
    ...                 ).result()
    ...         )
    >>> coup1 = dtOO.analyticSurface(
    ...             dtOO.bSplineSurface_skinConstructOCC(
    ...                 c_coup, c_mp1
    ...             ).result()
    ...     )

    Make a list containing the meanplane and coupling faces

    >>> meanplaneFaces = [
    ...        dtOO.map2dTo3d.MustDownCast(mp0),
    ...        dtOO.map2dTo3d.MustDownCast(mp1),
    ...        dtOO.map2dTo3d.MustDownCast(mp2),
    ...    ]
    
    >>> couplingFaces = [
    ...         dtOO.map2dTo3d.MustDownCast(coup0),
    ...         dtOO.map2dTo3d.MustDownCast(coup1),
    ...     ]

    Create the grid channel

    >>> from dtOOPythonApp.builder import multipleBoundedVolume_gridChannel
    >>> gridChannel = multipleBoundedVolume_gridChannel(
    ...     label = "test",
    ...     channel = channel,
    ...     meanplanes = meanplaneFaces,
    ...     couplings = couplingFaces,
    ...     nBlades = 12,
    ...     nInOutSurfSuction = 1
    ... )
    >>> gridChannel.build()
    
    Return the multiple bounded volume and a list with the bounding faces

    >>> gc, gcFaces = gridChannel.getGridChannel()
    
    Check the class of the returned volume

    >>> gc.virtualClassName()
    'multipleBoundedVolume'
    
    The main method of this class is :meth:`build`.

    The grid channel is built as a multiple bounded volume. It forms the part
    of the bladed channel mesh that is not occupied by the mesh blocks. 
    The construction of the multiple bounded volume requires a set
    of bounding surfaces.

    The bounding surfaces ``boundSurfs_`` are based on the meanplane and
    coupling faces provided to the class through the lists
    ``meanplanes_`` and ``couplings_``.
    The following figure shows the surfaces in ``meanplanes_`` and
    ``couplings_``.

    .. _meanplanesAndCouplings:
    .. figure:: bladeFigs/gidChannel_meanplanesAndCouplings.png
       :width: 100%
       :align: center

       Meanplane faces (yellow and green) and coupling faces (cyan) provided
       to this class. The blade (grey) is shown for reference.

    The grid channel is formed on the blade side from the coupling faces of
    the mesh blocks (cyan) and the FE-Meanplane faces 
    (:numref:`meanplanesAndCouplings` yellow).
    
    The bounding faces on the opposing side of the grid channel are formed
    through a rotational translation of the faces in ``meanplanes_`` 
    (:numref:`meanplanesAndCouplings` yellow and green). These rotated faces form 
    the periodic pressure faces of the bladed channel.
    
    The inlet and outlet faces (:numref:`bounds` red) of the grid channel are created from
    the rotation of the interface curves of the FE-Meanplane faces.
    The FE-Meanplane face from which the inlet interface is created is
    the last face in the meanplane list ``meanplanes_[-1]``. The face
    from which the outlet is created is the first face in the meanplane 
    list ``meanplanes_[0]``.

    The rotation angle is defined through the number of blades
    ``nBlades_`` in the full 360° channel. The rotation vector is provided
    through ``rotVector_``.

    The bounding surfaces on the hub and shroud are created as multiple
    bounded surfaces. The required bounding curves (:numref:`bounds`, magenta)
    are generated from the edges of the other bounding faces of the grid channel.
    The multiple bounded surfaces need bounding faces on the hub and shroud of 
    the channel geometry ``channel_`` in which the bounding curves are located.

    The input ``orientation_`` encodes the orientation of the blade within
    the channel. The following values are supported:

        - ``1``: Blade is oriented in the positive u-direction of the channel
        - ``-1``: Blade is oriented in the negative u-direction of the channel

    The method :meth:`calcRotParams` is used to ensure that the bounding
    faces of the multiple bounded surfaces extend over the correct hub and
    shroud regions.
    
    The bounding surfaces are appended to the list ``boundSurf_``

    The boundary faces of the grid channel and the bounding curves of
    the hub boundary face, are shown in the following figure.

    .. _bounds:
    .. figure:: bladeFigs/gidChannel_bound.png
       :width: 100%
       :align: center

       Bounding faces of the grid channel with periodic faces (yellow / green),
       inlet and outlet (red), coupling faces (cyan) and bounding curves of
       the hub boundary (magenta). The blade (grey) is shown for
       reference.
    
    The following face labels are used for the bounding faces:

        - ``"inlet"``:
          Inlet boundary (:numref:`bounds` red)

        - ``"outlet"``:
          Outlet boundary (:numref:`bounds` red)

        - ``"suction_tri_" + str(i)``:
          Suction boundary built from meanplane faces
          (:numref:`bounds` and :numref:`meanplanesAndCouplings` yellow)

        - ``"coupling_" + str(i)``:
          Boundary connecting to the blade mesh block
          (:numref:`bounds` cyan)

        - ``"pressure_tri_" + str(i)``:
          Pressure boundary built from rotated meanplane faces
          (:numref:`bounds` yellow)

        - ``"pressure_quad_" + str(i)``:
          Pressure boundary built from rotated meanplane faces
          (:numref:`bounds` green)

        - ``"hub"``:
          Multiple bounded surface on the hub

        - ``"shroud"``:
          Multiple bounded surface on the shroud

    The strings ``"pressure"`` and ``"suction"`` correspond to the periodic
    faces associated with the pressure and suction sides of a turbine
    channel created with this class.

    The markers ``"tri"`` and ``"quad"`` in the naming of the pressure and
    suction boundaries are used to distinguish between faces meshed
    unstructured with prism elements and faces meshed transfinite with
    hexahedral elements.

    The grid channel is created from the faces in ``boundSurf_`` and stored
    in ``gridChannel_``.

    The method :meth:`getGridChannel` returns the grid channel
    ``gridChannel_`` together with a list of the bounding faces
    ``boundSurf_``

    If debug mode is enabled, the bounding faces can be plotted using the
    following naming convention:

    ::

        "debug_gridChannelFace_" + label_ + "_" + face.getLabel()

    """
    def __init__(
        self,
        label: str,
        channel: analyticGeometry,
        meanplanes: List[analyticGeometry],
        couplings: List[analyticGeometry],
        nBlades: int,
        nInOutSurfSuction: int = 2,
        rotVector: dtVector3 = dtVector3(0, 0, 1),
        origin: dtPoint3 = dtPoint3(0, 0, 0),
        orientation: int = 1,
        tERounded: bool = False,

      ) -> None:
        """
        Parameters
        ----------
        label: str
          Label.
        channel: analyticGeometry
          360° rotated channel domain
        meanplanes: List[analyticGeometry]
          List of meanplane faces
        couplings: List[analyticGeometry]
          List of coupling faces
        nBlades: int
          Number of blades
        nInOutSurfSuction: int
          Number of meanplane faces extending from the mesh blocks
          to the inlet and outlet each
        rotVector: dtVector3
          Rotation vector of the grid channel
        orientation: int
          Orientation of the blade in the channel.
          
            - 1 : Blade is oriented in u-direction of channel
            - -1 : Blade is oriented in negative u-direction of the channel
        """
        super(
          multipleBoundedVolume_gridChannel, self
        ).__init__()

        logging.debug("Initializing %s ..." % (label))
        self.label_ = label
        self.channel_ = map3dTo3d.MustDownCast( channel )
        self.meanplanes_ = meanplanes
        self.couplings_ = couplings
        self.nBlades_ = nBlades
        self.nInOutSurf_ = nInOutSurfSuction
        self.rotVector_ = dtLinearAlgebra.normalize(rotVector)
        self.origin_ = origin
        self.orientation_ = orientation
        self.tE_ = tERounded 

    def build(self) -> None:
        """Build part.

        This method:

            - Creates multiple bounded surfaces on the hub and shroud.
            - Creates bounding faces from the meanplane and coupling faces.
            - Creates a multiple bounded volume representing the grid channel.

        Parameters
        ----------
        None

        Returns
        -------
        None


        This method creates the grid channel from the faces in ``meanplanes_``
        and ``couplings_``. The following figure illustrates the operations
        performed.

        .. _mbvActivity:
        .. figure:: bladeFigs/multipleBoundedVolume_gridChannel.png
           :width: 100%
           :align: center

           Activity diagram of class `multipleBoundedVolume_gridChannel`.
           Colors correspond to the creation of geometries shown in
           :numref:`meanplanesAndCouplings` and :numref:`bounds`.
        
        **Prepare Containers**

            Three vector-handling containers are created. The container
            ``boundSurf_`` stores the bounding surfaces of the multiple bounded
            volume. The containers ``hubCurves`` and ``shroudCurves`` store the
            bounding curves of the multiple bounded surfaces that define the hub
            and shroud boundaries of the grid channel.

        **Create Bounding faces for the Multiple Bounded Surfaces on Hub and Shroud**

            To create the multiple bounded surfaces, bounding faces on the hub and
            shroud are required. To ensure that these surfaces extend over the full
            grid channel domain, the hub and shroud points ``p0h`` and ``p0s`` at
            the inlet or outlet, depending on ``orientation_``, are extracted from
            ``meanplanes_``.

            By passing these points to the method :meth:`calcRotParams`, their
            u-coordinate within the channel, including a tolerance, is calculated.
            The hub and shroud bounding faces ``m2d_hub`` and ``m2d_shr`` are then
            created by rotating a segment of the channel ``channel_`` at this
            u-coordinate on the hub or shroud around ``rotVector_``.

        **Iterate over meanplane faces**

            ::

                for i, face in enumerate(meanplanes_):

            The periodic faces as well as the inlet and outlet boundaries of
            the grid channel are created by iterating over ``meanplanes_``.

            A volume ``vol`` is created by rotating the current meanplane face 
            ``face``. The bounding surfaces of the grid channel are extracted 
            from ``vol`` as segments of constant parameter coordinates.

            Initially, the string ``lab`` is set to ``"quad"``.
            
            **Interface Meanplane**

                ::

                    i == 0 or i == len(meanplanes_) - 1

                If the iteration processes the first or last meanplane face,
                an inlet or outlet interface boundary is created. By definition of
                ``meanplanes_``, the first entry ``i == 0`` contains the outlet
                meanplane surface, while the last entry ``i == len(meanplanes_) - 1``
                contains the inlet surface.

                The inlet and outlet boundaries are added to ``boundSurf_`` with
                the labels ``"inlet"`` and ``"outlet"``, respectively.

                The corresponding bounding curves on the hub and shroud are appended
                to ``hubCurves`` and ``shroudCurves``.
            
            **Periodic FE-Meanplane faces**

                ::

                    i < nInOutSurf_ or i >= len(meanplanes_) - nInOutSurf_

                The first and last ``n`` faces in ``meanplanes_`` are part of the
                boundary surfaces (compare
                :numref:`meanplanesAndCouplings` and :numref:`bounds`). The number
                of these faces is specified by ``nInOutSurf_``.

                If the iteration is processing one of these faces,
                the value of ``lab`` is changed to ``"tri"``. These faces are added
                to ``boundSurf_`` with the label

                ::

                    "suction_" + lab + "_" + str(i)

                The corresponding bounding curves on the hub and shroud are 
                added to the respective containers.
            
            **Create Periodic Pressure Surfaces**

                In every iteration, the rotated meanplane face is added to
                ``boundSurf_`` with the following label:
                
                ::

                    "pressure_" + lab + "_" + str(i)

                The associated bounding curves are appended to ``hubCurves`` and
                ``shroudCurves``.

        **Iterate over coupling faces**

            ::

                for i, face in enumerate(couplings_):

            All coupling faces are part of the grid channel boundary surfaces. The last two
            faces in ``couplings_`` correspond to the faces downstream of the
            trailing edge and are oriented orthogonally to the flow direction.
            These faces are oriented differently from the remaining coupling
            faces.

            The condition ``i >= len(couplings_) - 2``
            identifies these faces and ensures that the correct bounding curves
            are added to ``hubCurves`` and ``shroudCurves``.
        
        **Create the Multiple Bounded Surfaces on the Hub and Shroud**

            The multiple bounded surfaces on the hub and shroud,
            ``mbs_hub`` and ``mbs_shroud``, are created from ``m2d_hub`` and
            ``m2d_shr`` together with the lists of bounding curves
            ``hubCurves`` and ``shroudCurves``.

        **Create the Grid Channel**

            The grid channel volume is created using the ``dtOO`` class
            ``multipleBoundedVolume`` from ``boundSurf_``. The resulting object is
            stored in ``gridChannel_``.
        """
        #
        # prepare container objects for geometires
        #
        # vector handler for boundary surfaces
        self.boundSurf_ = labeledVectorHandlingAnalyticGeometry()
        
        # vector handlers for bounding curves of multiple bounded surfaces of hub and shroud
        hubCurves = vectorHandlingAnalyticGeometry()
        shroudCurves = vectorHandlingAnalyticGeometry()
        
        # get the hub and shroud points from the meanplane at the inlet or outlet 
        if self.orientation_ > 0: 
            # at the inlet
            p0h = self.meanplanes_[-1].getPointPercent(1,0)
            p0s = self.meanplanes_[-1].getPointPercent(1,1)
        else:
            # at the outlet
            p0h = self.meanplanes_[0].getPointPercent(1,0)
            p0s = self.meanplanes_[0].getPointPercent(1,1)
        
        # get the u coordinates of these points in the channel
        uHub = self.calcRotParams(p0h)
        uShr = self.calcRotParams(p0s)
        
        #
        # generate bounding faces for multiple bounded surfaces
        # the face is a circular segment and not the whole rotating face
        #  (rotation of 2*pi would be detected as degenerated face
        #  (starting and ending curve would be at the same point))
        #
        angle = 2*np.pi * 0.95

        # curve segment at hub at calculated u coordinate
        m1d_hub = self.channel_.segmentConstWPercent(0).segmentConstUPercent(uHub)
        # create the bounding face
        m2d_hub = analyticSurface(
                    rectangularTrimmedSurface_curveRotateConstructOCC(
                        analyticCurve.MustDownCast(m1d_hub).ptrDtCurve(),
                        dtPoint3(0, 0, 0),
                        self.rotVector_,
                        angle
                    ).result()
                ) 
        # do the same with the shroud
        m1d_shr = self.channel_.segmentConstWPercent(1).segmentConstUPercent(uShr)
        m2d_shr = analyticSurface(
                    rectangularTrimmedSurface_curveRotateConstructOCC(
                        analyticCurve.MustDownCast(m1d_shr).ptrDtCurve(),
                        dtPoint3(0, 0, 0),
                        self.rotVector_,
                        angle
                    ).result()
                )
        
        ## Debug statement to plot the hub and shroud faces
        #self.appendAnalyticGeometry(
        #        m2d_hub,
        #        "TEST_m2d_hub_"+self.label_
        #    )
        #self.appendAnalyticGeometry(
        #        m2d_shr,
        #        "TEST_m2d_shr_"+self.label_
        #    )
        
        # initialize a rotation dtTransformer
        cfg = jsonPrimitive()
        cfg.appendDtPoint3("_origin", self.origin_)
        cfg.appendDtVector3("_rotVector", self.rotVector_)
        cfg.appendReal("_angle", 2*np.pi/self.nBlades_)
        
        # create transformer object
        rot = rotate(cfg) 
        
        #
        # Iterate over meanplane faces 
        # Store the bounding faces of the MBV in a list
        # Store the bounding edges for the MBS in a list
        #
        for i, face in enumerate(self.meanplanes_):
            
            # Hub and Shroud edges of the meanplanes for the multiple bounded surface
            hub = face.segmentConstVPercent(0)
            shr = face.segmentConstVPercent(1)
            
            # Rotational transformation of the meanplane faces and hub / shroud edges 
            rotFace = rot.applyAnalyticGeometry(face)
            rotHub = rot.applyAnalyticGeometry(hub)
            rotShr = rot.applyAnalyticGeometry(shr)
            
            # label "quad" for faces which are meshed with hexagonal faces
            #  (faces with rotational periodicity to mesh blocks)
            lab = "quad"
            
            #
            # outlet or inlet boundary of region (first or last meanplane face)
            #
            if i == 0 or i == len(self.meanplanes_)-1:

                # boundary label
                let = "outlet" if i == 0 else "inlet"
                
                # rotate the interface edge to create the interface surface
                rM = rotatingMap1dTo3d(
                        self.rotVector_,
                        face.segmentConstUPercent(1)
                    )
                inOut = partRotatingMap1dTo3d(
                        self.rotVector_,
                        face.segmentConstUPercent(1),
                        rM.getMin(0), 
                        rM.getMin(0) + 1/self.nBlades_ * (rM.getMax(0) - rM.getMin(0))
                    )

                # boundary curves for multiple bounded surfaces at hub and shroud
                letHub = inOut.segmentConstVPercent(0)
                hubCurves.push_back(letHub)
                letShr = inOut.segmentConstVPercent(1)
                shroudCurves.push_back(letShr)

                self.boundSurf_.push_back(inOut << let)
            
            #
            # Periodic meanplane faces
            #
            # special treatment for suction boundaries
            #  only the first two and last two faces are taken from the meanplane
            #  the other faces are the coupling faces
            # self.nInOutSurf_ specifies how many extention faces are defined between 
            #  the mesh blocks and inlet or outlet
            if i < self.nInOutSurf_ or i >= len(self.meanplanes_)-self.nInOutSurf_:
                
                # label "tri" for faces which are meshed with prisms
                #  (faces with rotational periodicity to suction and pressure)
                lab = "tri"

                # suction boundary (second periodic)
                self.boundSurf_.push_back(face << "suction_"+lab+"_"+str(i))
                
                # boundary curves for multiple bounded surfaces at hub and shroud
                hubCurves.push_back(hub)
                shroudCurves.push_back(shr)
            
            # pressure boundary (first periodic)
            #  here always the meanpalne is used
            #  lab changes from "quad" to "tri" depending on the periodicity
            self.boundSurf_.push_back(rotFace << "pressure_"+lab+"_"+str(i))
            
            # boundary curves for multiple bounded surfaces at hub and shroud
            hubCurves.push_back(rotHub)
            shroudCurves.push_back(rotShr)
        
        #
        # Iterate over coupling faces
        #
        for i, face in enumerate(self.couplings_):
            self.boundSurf_.push_back(face << "coupling_"+str(i))
            
            # boundary curves for multiple bounded surfaces at hub and shroud
            if i >= len(self.couplings_)-2 and self.tE_ == False:
                # trailing edge faces orthogonal to the flow direction (oriented differently)
                hubCurves.push_back(face.segmentConstUPercent(0))
                shroudCurves.push_back(face.segmentConstUPercent(1))
            else:
                # coupling faces surrounding blade
                hubCurves.push_back(face.segmentConstVPercent(0))
                shroudCurves.push_back(face.segmentConstVPercent(1))
        
        # create multiple bounded surfaces for hub and shroud
        # push them into the vector handler for the multiple bounded volume
        mbs_hub = multipleBoundedSurface(m2d_hub, hubCurves)
        mbs_shroud = multipleBoundedSurface(m2d_shr, shroudCurves)
         
        self.boundSurf_.push_back(mbs_hub.clone() << "hub")
        self.boundSurf_.push_back(mbs_shroud.clone() << "shroud")

        # create grid channel as multi bounded volume
        self.gridChannel_ = multipleBoundedVolume(infinityMap3dTo3d(), self.boundSurf_)
         
        # append boundaries if debug is enabeled
        if self.debug():
            for face in self.boundSurf_:
                self.appendAnalyticGeometry(
                        face,
                        "debug_gridChannelFace_"+self.label_+"_"+face.getLabel()
                    )
            for i, curve in enumerate(hubCurves):
                self.appendAnalyticGeometry(
                        curve,
                        "debug_"+self.label_+"_hubCurve_"+str(i)
                    )
            for i, curve in enumerate(shroudCurves):
                self.appendAnalyticGeometry(
                        curve,
                        "debug_"+self.label_+"_shroudCurve_"+str(i)
                    )
        
    def calcRotParams(self, p0) -> float:
        """Calculate the u-parameter of a point within the channel.

        This method:

            - Reparametrizes a point in channel coordinates.
            - Offsets the u-parameter by ``0.01``.
            - Corrects the value if the parameter becomes negative.
            - Returns the resulting u-parameter.

        Parameters
        ----------
        p0 : dtPoint3
            Cartesian point within the channel.

        Returns
        -------
        uvwP0 : float
            u-parameter of the point in the channel reduced by the tolerance.
        """
        # reparametrize the point in uwv-parameters of the channel
        # get the u coordinate in percent and adding 0.01 tolerance
        uvwP0 = self.channel_.percent_u(
                self.channel_.reparamInVolume(p0).x()
            ) - 0.01
        
        # correcting if parameter is negative
        if uvwP0 < 0.0:
            uvwP0 = 1 + uvwP0
         
        return uvwP0

    #
    # return method for grid channel and its faces
    #
    def getGridChannel(self) -> Tuple[analyticGeometry, List[analyticGeometry]]:
        """Return the grid channel volume ``gridChannel_`` and its bounding surfaces ``boundSurf_``.

        Parameters
        ----------
        None

        Returns
        -------
        gridChannel_: analyticGeometry
          Multiple bounded volume of the grid channel
        boundSurf_: labeledVectorHandlingAnalyticGeometry
          List of bounding faces
        """
        return self.gridChannel_, self.boundSurf_ 
