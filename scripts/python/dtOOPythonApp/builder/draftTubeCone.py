from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder 

from dtOOPythonSWIG import (
  dtPoint3,
  vectorDtPoint3,
  vectorDtVector3,
  dtVector3,
  dtCurve,
  dtSurface,
  vectorHandlingConstDtCurve,
  analyticSurface,
  analyticCurve,
  bSplineSurface_skinConstructOCC,
  circle_radiusCoordinateSystemConstructOCC,
  bSplineCurve_convertOCC,
  bSplineCurve_bSplineCurveSplitConstructOCC,
  bSplineCurve_curveConnectConstructOCC,
  trimmedCurve_twoPointsConnectConstructOCC,
)

from typing import List
import logging

class draftTubeCone(dtBundleBuilder):
  """Create a channel by skinning hub and shroud.

  Attributes
  ----------


  Examples
  --------

  """

  def __init__(
    self,
    label: str,
    coordPoints: List[dtPoint3],
    rotationVectors: List[dtVector3],
    radiusList: [],
    halfDivide: int,
  ) -> None:

    logging.info("Initializing %s ..." % (label))
    super(draftTubeCone, self).__init__()
    self.label_ = label
    self.coordPoints_ = vectorDtPoint3()
    self.rotationVectors_ = vectorDtVector3()
    self.radiusList_ = []
    for pp in coordPoints:
      self.coordPoints_.append( pp )
    for ff in rotationVectors:
      self.rotationVectors_.append( ff )
    for gg in radiusList:
      self.radiusList_.append( gg )
    self.halfDivide_ = halfDivide

  def build(self) -> None:

    logging.info("Building %s ..." % (self.label_))
    vh = vectorHandlingConstDtCurve()

    for i in range(0, 2):
      circle = circle_radiusCoordinateSystemConstructOCC(
          self.coordPoints_[i],
          self.rotationVectors_[i],
          dtVector3(0.00, 1.00, 0.00), #solid orientation of the CS
          self.radiusList_[i]
        ).result()

      all_circle_splits = []
      for j in range(0, 4):
        u_start = (circle.length() / 4.00 )* (j)
        u_end = (circle.length() / 4.00 )* (j + 1.00)

        circle_split_j = bSplineCurve_bSplineCurveSplitConstructOCC(
          bSplineCurve_convertOCC(circle).result(),
          circle.u_l(u_start),
          circle.u_l(u_end)
        ).result()

        all_circle_splits.append(circle_split_j)

      vs = vectorHandlingConstDtCurve()
      vs.push_back(all_circle_splits[1])
      vs.push_back(all_circle_splits[2])
      if self.halfDivide_ == 0 :
        vs.push_back(all_circle_splits[3])
        vs.push_back(all_circle_splits[0])

      circle_connect = bSplineCurve_curveConnectConstructOCC(
        vs,
        1.0E-08, #tol
        0, #param
        0 #minM
      ).result()

      #Verschiedene Tests
      #1
      # uu = circle_connect.getUMin()
      # start_1 = circle_connect.point(uu)
      #
      # uu = circle_connect.length() * (1/2)
      # zz = circle_connect.u_l(uu)
      # end_1 = circle_connect.point(zz)
      #
      # test_ = trimmedCurve_twoPointsConnectConstructOCC(
      #   start_1,
      #   end_1
      # ).result()
      #
      # x = analyticCurve(test_)
      # x.setLabel(self.label_+"_test_" + str(i))
      # self.appendAnalyticGeometry( x )

      vh.append(circle_connect)

    ref = analyticSurface(
      bSplineSurface_skinConstructOCC(
        vh,
        2, #minDeg
        4, #maxDeg
        0  #nIter
      ).result()
    )

    ref.setLabel(self.label_)

    self.appendAnalyticGeometry( ref )

    return
