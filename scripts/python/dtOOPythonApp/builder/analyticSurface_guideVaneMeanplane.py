from dtOOPythonApp.tools import dtBundleBuilder

from dtOOPythonSWIG import (
    analyticGeometry,
    dtPoint3,
    map3dTo3d,
    analyticCurve,
    bSplineCurve_pointConstructOCC,
    vectorDtPoint3,
    vec3dSurfaceTwoD,
    bSplineSurface_skinConstructOCC,
)

from typing import List, Union
import logging
import numpy as np

class analyticSurface_guideVaneMeanplane(dtBundleBuilder):
    """Create a straight meanplane surface inside a radially extending channel.

    This class:

        - Calculates the position of the spanwise roation axis of a guide vane.
        - Calculates the position of the leading edge and trailing edge.
        - Creates a guide vane meanplane as a straight surface inside the regular channel.

    Attributes
    ----------
    label_: str
      Label.
    channel_: analyticGeometry
      Channel.
    offRad_: float
      Radius of the guide vane rotation axis in the channel
    offAng_: float
      Angular position of the guide vane rotation axis in the regular channel
    alpha_: float
      Angle of the guide vane in relation to offAng_
    l0_: float
      Length portion of the blade extending from the rotation axis to the leading edge
    l1_: float
      Length portion of the blade extending from the rotation axis to the trailing edge
    Returns
    -------
    None

    Examples
    --------
    
    >>> import dtOOPythonSWIG as dtOO
    
    Create a channel extending in radial direction.

    >>> c0 = dtOO.bSplineCurve_pointConstructOCC(
    ...       dtOO.vectorDtPoint3()
    ...         << dtOO.dtPoint3(+1.56, +0.00, 0.36)
    ...         << dtOO.dtPoint3(+1.29, +0.00, 0.36),
    ...         1
    ...     ).result()
    >>> c1 = dtOO.bSplineCurve_pointConstructOCC(
    ...       dtOO.vectorDtPoint3()
    ...         << dtOO.dtPoint3(+1.29, +0.00, 0.36)
    ...         << dtOO.dtPoint3(+1.29, +0.00, 0.00),
    ...         1
    ...     ).result()
    >>> c2 = dtOO.bSplineCurve_pointConstructOCC(
    ...       dtOO.vectorDtPoint3()
    ...         << dtOO.dtPoint3(+1.29, +0.00, 0.00)
    ...         << dtOO.dtPoint3(+1.56, +0.00, 0.00),
    ...         1
    ...     ).result()
    >>> c3 = dtOO.bSplineCurve_pointConstructOCC(
    ...       dtOO.vectorDtPoint3()
    ...         << dtOO.dtPoint3(+1.56, +0.00, 0.00)
    ...         << dtOO.dtPoint3(+1.56, +0.00, 0.36),
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
    
    Create the guide vane meanplane.
    
    >>> from dtOOPythonApp.builder import analyticSurface_guideVaneMeanplane 
    >>> gvMeanplane = analyticSurface_guideVaneMeanplane(
    ...     label = "gv_meanplane",
    ...     channel = channel,
    ...     offRad = 1.425,
    ...     offAng = -0.1745,
    ...     alpha = -1.1868, 
    ...     l_tot = 0.4,
    ...     ratioL = 0.3
    ... )
    >>> gvMeanplane.build()

    Check the label of the last generated geometry.
    
    >>> gvMeanplane.lVH_aF().labels()[-1]
    'gv_meanplane'
    
    The position of the guide vane meanplane is defined by the guide vane blade's
    spanwise rotational axis, using the radius ``offRad_`` and the angle ``offAng_``
    measured from the global z-axis.
    The rotational axis can be visualized using the debug curve
    ``debug_guideVaneRotAxis``.
    The guide vane opening angle is specified by ``alpha_``, which defines the angle
    between the radial line extending from the global z-axis to the guide vane's
    rotational axis and the guide vane meanplane.
    The radial line can be visualized using the debug line ``debug_offVec``.

    The total length of the meanplane is specified by ``l_tot_``. The portion of this
    length extending from the rotational axis to the leading edge is specified by
    ``l0_``, while the portion extending from the rotational axis to the trailing edge
    is specified by ``l1_``. The ratio of ``l0_`` to ``l_tot_`` is specified by
    ``ratioL_``.

    The meanplane surface is skinned from two meanplane curves, one on the hub and one
    on the shroud, each extending from the leading edge to the trailing edge. The
    skinning direction is from the hub to the shroud.
    """

    def __init__(
        self, 
        label: str,  
        channel: analyticGeometry,
        offRad: float,
        offAng: float,
        alpha: float,
        l_tot: float,
        ratioL: float = 0.5
        
    ) -> None:
        """ Constructor

        Parameters 
        ----------
        label: str
          Label.
        channel: analyticGeometry
          Channel.
        offRad: float
          Radius of the guide vane rotation axis in the channel
        offAng: float
          Angular position of the guide vane rotation axis in the regular channel
        alpha: float
          Angle of the guide vane in relation to offAng_
        l_tot: float
          Tatal blade length
        ratioL: float, default = 0.5
          Ratio of the total blade length which will form portion from leading edge to rotational axis
        Returns
        -------
        None
         
        """

        logging.info( "Initializing %s ..." % (label) )
        super(analyticSurface_guideVaneMeanplane, self).__init__()
        self.label_ = label
        self.channel_ = map3dTo3d.MustDownCast(channel)

        self.offRad_ = offRad
        self.offAng_ = offAng
        self.alpha_ = alpha
        self.l0_ = l_tot * ratioL
        self.l1_ = l_tot - self.l0_


    def build(self) -> None:
        """Build part.

        Parameters
        ----------
        None

        Returns
        -------
        None
        """
        logging.info( "Building %s ..." % (self.label_) )
        
        # get z coordinates of the hub and shroud 
        z_hub = self.channel_.getPointPercent(0.5, 0.5, 0).z()
        z_shr = self.channel_.getPointPercent(0.5, 0.5, 1).z()
        
        # calculate the x and y coordinates of the guide vane's spanwise rotation axis
        x = self.offRad_*np.cos(self.offAng_)
        y = self.offRad_*np.sin(self.offAng_)
        
        # calculate the x and y coordinates of the leading edge
        ang0 = self.offAng_ + self.alpha_
        x0 = x + self.l0_*np.cos(ang0)
        y0 = y + self.l0_*np.sin(ang0)
        
        # calculate the x and y coordinates of the trailing edge
        ang1 = self.offAng_ + self.alpha_-np.pi
        x1 = x + self.l1_*np.cos(ang1)
        y1 = y + self.l1_*np.sin(ang1)
        
        # create the meanplane curve on the hub as a straight line
        mpCurveHub = analyticCurve(
              bSplineCurve_pointConstructOCC(
                vectorDtPoint3()
                  << dtPoint3(x0,y0,z_hub)
                  << dtPoint3(x1,y1,z_hub),
                1
              ).result()
            )
        # create the meanplane curves on the shroud as a straight line
        mpCurveShr = analyticCurve(
              bSplineCurve_pointConstructOCC(
                vectorDtPoint3()
                  << dtPoint3(x0,y0,z_shr)
                  << dtPoint3(x1,y1,z_shr),
                1
              ).result()
            )
        # skin meanplane surface from meanplane curves    
        res = vec3dSurfaceTwoD( 
            bSplineSurface_skinConstructOCC(
                mpCurveHub.ptrConstDtCurve(), 
                mpCurveShr.ptrConstDtCurve()
            ).result() 
        )
        res.setLabel( self.label_ )
        self.appendAnalyticFunction( res )
                 
        #
        # debug geometries
        #
        if self.debug():
            # spanwise rotation axis
            rotAxis = analyticCurve(
                  bSplineCurve_pointConstructOCC(
                    vectorDtPoint3()
                      << dtPoint3(x,y,z_hub)
                      << dtPoint3(x,y,z_shr),
                    1
                  ).result()
                )
            self.appendAnalyticGeometry(
                rotAxis,
                "debug_guideVaneRotAxis"
            )
            # vector pointing from global z-axis to spanwise rotation axis
            offVec = analyticCurve(
                  bSplineCurve_pointConstructOCC(
                    vectorDtPoint3()
                      << dtPoint3(0,0,z_hub)
                      << dtPoint3(x,y,z_hub),
                    1
                  ).result()
                )
            self.appendAnalyticGeometry(
                offVec,
                "debug_offVec"
            )
            # hub and shroud meanplane curves
            self.appendAnalyticGeometry(
                mpCurveHub,
                "debug_mpCurveHub"
            )
            self.appendAnalyticGeometry(
                mpCurveShr,
                "debug_mpCurveShr"
            )

        return

