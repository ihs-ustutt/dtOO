from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder 

from dtOOPythonSWIG import (
  dtPoint2,
  scaOneD,
)

from typing import List
import logging

from dtOOPythonApp.builder import scaOneD_scaCurve2dOneDPointConstruct 

class draftTubeParameterDistribution(dtBundleBuilder):
  """This class holds the declaration of the surge Pipe Parameters.

  Attributes
  ----------
  label_: str
    Label.
  order_: int
    Splines' order for various parameter distribution splines.

  Examples
  --------
  >>> from dtOOPythonSWIG import (
  ...   dtPoint2,
  ...   scaOneD,
  ... )

  Create a simple parameter distribution for a simple surge pipe.
  There are given the heigth-distribution, the width-distribution and the
  radius-distribution for the surge pipe surface profile.

  >>> builder = surgePipeParameterDistribution(
  ...   "simple_distribution",
  ...   order = 1,
  ... )

  embedding the parameter-distirbution in the code:

  >>> builder.build()
  """
  def __init__(
    self,
    label: str,
    order: int,

  ) -> None:
    """Constructor.

    Parameters
    ----------
    label: str
      Label.
    order: int
      Splines' order for various distributions.

    Returns
    -------
    None
    """
    logging.info( "Initializing %s ..." % (label) )
    super(
      draftTubeParameterDistribution, self
    ).__init__()

    self.label_ = label
    self.order_ = order
    return

  def build(self) -> None:
    """Build part.

    Parameters
    ----------
    None

    Returns
    -------
    None
    """
    hh = scaOneD_scaCurve2dOneDPointConstruct(
      [
        #straight draft tube
        # dtPoint2(0.0000, 1.55),
        # dtPoint2(0.8085, 1.55),
        # dtPoint2(1.3965, 1.55),
        # dtPoint2(1.9845, 1.55),
        # dtPoint2(2.6607, 1.55),
        # dtPoint2(3.3075, 1.55),
        # dtPoint2(3.9837, 1.55),
        # dtPoint2(4.6305, 1.55),
        # dtPoint2(5.3067, 1.55),
        # dtPoint2(5.8065, 1.55),
        # dtPoint2(6.4827, 1.55),
        # dtPoint2(6.9825, 1.55),
        # dtPoint2(7.4235, 1.555),    #1.55),  #1.5699
        # dtPoint2(7.8645, 1.555),    #1.55),  #1.5882
        # dtPoint2(8.3937, 1.55),    #1.55),  #1.6391
        # dtPoint2(9.3345, 1.85),    #1.55),  #1.7320
        # dtPoint2(29.5745, 4.05),    #1.55),#27.5745

        # elbow draft tube
        dtPoint2(0.0000, 3.4000),
        dtPoint2(0.8085, 3.4382),
        dtPoint2(1.3965, 3.4081),
        dtPoint2(1.9845, 3.3126),
        dtPoint2(2.6607, 3.1430),
        dtPoint2(3.3075, 2.8900),
        dtPoint2(3.9837, 2.5720),
        dtPoint2(4.6305, 2.2706),
        dtPoint2(5.3067, 2.0144),
        dtPoint2(5.8065, 1.7545),
        dtPoint2(6.4827, 1.6366),
        dtPoint2(6.9825, 1.5500),
        dtPoint2(7.4235, 1.5699),    #1.55),  #1.5699
        dtPoint2(7.8645, 1.5882),    #1.55),  #1.5882
        dtPoint2(8.3937, 1.6391),    #1.55),  #1.6391
        dtPoint2(9.3345, 1.7320),    #1.55),  #1.7320
        dtPoint2(29.5745, 3.9047),    #1.55),#27.5745
      ],
      self.order_
    ).build()

    bb = scaOneD_scaCurve2dOneDPointConstruct(
      [
        #straight draft tube
        # dtPoint2(0.0000, 0.0001),
        # dtPoint2(0.8085, 0.0001),
        # dtPoint2(1.3965, 0.0001),
        # dtPoint2(1.9845, 0.0001),
        # dtPoint2(2.6607, 0.0001),
        # dtPoint2(3.3075, 0.0001),
        # dtPoint2(3.9837, 0.0001),
        # dtPoint2(4.6305, 0.0001),
        # dtPoint2(5.3067, 0.0001),
        # dtPoint2(5.8065, 0.0001),
        # dtPoint2(6.4827, 0.0001),
        # dtPoint2(6.9825, 0.0001),
        # dtPoint2(7.4235, 0.0001),     #8.54),  #7.3000
        # dtPoint2(7.8645, 0.0001),     #8.98),  #7.5000
        # dtPoint2(8.3937, 0.0001),     #9.55),  #7.7000
        # dtPoint2(9.3345, 0.0001),     #10.38),  #7.9800
        # dtPoint2(29.5745, 0.00001),    #21.92), #8.3000

        #elbow draft tube
        dtPoint2(0.0000, 0.0000),
        dtPoint2(0.8085, 0.1626),
        dtPoint2(1.3965, 0.4705),
        dtPoint2(1.9845, 0.8823),
        dtPoint2(2.6607, 1.5274),
        dtPoint2(3.3075, 2.4282),
        dtPoint2(3.9837, 3.6021),
        dtPoint2(4.6305, 4.8840),
        dtPoint2(5.3067, 5.7984),
        dtPoint2(5.8065, 6.6702),
        dtPoint2(6.4827, 7.0109),
        dtPoint2(6.9825, 7.1480),
        dtPoint2(7.4235, 7.3000),     #8.54),  #7.3000
        dtPoint2(7.8645, 7.5000),     #8.98),  #7.5000
        dtPoint2(8.3937, 7.7000),     #9.55),  #7.7000
        dtPoint2(9.3345, 7.9800),     #10.38),  #7.9800
        dtPoint2(29.5745, 8.3000),    #21.92), #8.3000
      ],
      self.order_
    ).build()

    rr = scaOneD_scaCurve2dOneDPointConstruct(
      [
        # straight draft tube
        # dtPoint2(7.0000, 1.000),
        # dtPoint2(7.4235, 0.755),
        # dtPoint2(7.8645, 0.755),
        # dtPoint2(8.3937, 0.75),
        # dtPoint2(9.3345, 0.90),
        # dtPoint2(29.5745, 2.0),

        # elbow draft tube
        dtPoint2(7.0000, 1.00),
        dtPoint2(7.4235, 0.70),
        dtPoint2(7.8645, 0.60),
        dtPoint2(8.3937, 0.50),
        dtPoint2(9.3345, 0.36),
        dtPoint2(29.5745, 0.20),
      ],
      self.order_
    ).build()


    theRefhh = scaOneD.DownCast(
      hh
    )
    theRefhh.setLabel("h-distribution")

    self.appendAnalyticFunction( theRefhh )


    theRefbb = scaOneD.DownCast(
      bb
    )
    theRefbb.setLabel("b-distribution")

    self.appendAnalyticFunction( theRefbb )


    theRefrr = scaOneD.DownCast(
      rr
    )
    theRefrr.setLabel("r-distribution")

    self.appendAnalyticFunction( theRefrr )

    return
