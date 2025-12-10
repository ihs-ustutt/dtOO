from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder 

from dtOOPythonSWIG import (
  dtPoint3,
  dtVector3,
  vectorHandlingConstDtCurve,
  analyticSurface,
  analyticCurve,
  analyticGeometry,
  analyticFunction,
  bSplineCurve_pointConstructOCC,
  bSplineSurface_skinConstructOCC,
  circle_radiusCoordinateSystemConstructOCC,
  bSplineCurve_bSplineCurveSplitConstructOCC,
  bSplineCurve_convertOCC,
  geomCurve_curveTranslateConstructOCC,
  bSplineCurve_curveConnectConstructOCC,
  trimmedCurve_twoPointsConnectConstructOCC
)

from typing import List
import logging

class draftTubeElbow(dtBundleBuilder):

  def __init__(
    self,
    label: str,
    hDistCurve: analyticFunction,
    bDistCurve: analyticFunction,
    rDistCurve: analyticFunction,
    surgeCurve: analyticGeometry,
    resolution: int,
    halfDivide: int,
  ) -> None:

    logging.info("Initializing %s ..." % (label))
    super(draftTubeElbow, self).__init__()
    self.label_ = label
    self.hcurve_ = hDistCurve
    self.bcurve_ = bDistCurve
    self.rcurve_ = rDistCurve
    self.curve_ = surgeCurve
    self.resolution_ = resolution
    self.halfDivide_ = halfDivide

  def build(self) -> None:

    # Reference values
    curve = analyticCurve.MustConstDownCast( self.curve_ ).ptrDtCurve()
    sectionsLength = curve.length() / ( self.resolution_ - 1 )
    vh = vectorHandlingConstDtCurve() #total vector

    # Create sections in the loop
    for i in range(self.resolution_):
      logging.debug( "Create elbow section number %d ..." % (i) )
      l_section = i * sectionsLength # length
      u_section = curve.u_l(l_section) # running parameter
      all_connection_splits = [] # vector of connection pieces

      # Inclusion h and b distribution
      h = self.hcurve_(l_section)[0] / 2
      b = self.bcurve_(l_section)[0] / 2

      # Define tangent, special at start and end
      tangent_section = curve.firstDer(u_section)
      if i == 0:
        tangent_section = dtVector3(0.00, 0.00, -1.00)
      elif ( i + 1 ) == (self.resolution_):
        tangent_section = dtVector3(1.00, 0.00, 0.00)

      # Create basic circle
      geom = self.create_circle_geometry(curve, u_section, tangent_section, h)
      # Split basic circle
      geom_split = self.split_circle(geom)
      # Move quarter circles
      geom_split_trans = self.translate_quarter_circles(geom_split, b)
      # Create connecting piece at the top
      self.build_top_connection(geom_split_trans, all_connection_splits, i)
      # Create connecting piece at the bottom
      self.build_bottom_connection(geom_split_trans, all_connection_splits, i)
      # Fill single splines in right order into vector
      geom_split_trans_connect = self.assemble_profile(geom_split_trans,
                                                       all_connection_splits,
                                                       self.halfDivide_,
                                                       i
                                                       )
      # Assemble splines to closed profile
      geom_closed_profile = self.close_profile(geom_split_trans_connect)
      # Put closed section profil into total vector
      vh.push_back(geom_closed_profile)

    # Create surface
    geom_surface = self.build_surface(vh)
    # Set a lable
    geom_surface.setLabel(self.label_)
    # Append it to aG
    self.appendAnalyticGeometry( geom_surface )

    return


  def create_circle_geometry(self, curve, u_section, tangent_section, h):
    return circle_radiusCoordinateSystemConstructOCC(
        curve.point(u_section), #point
        tangent_section, #vector of rotation
        dtVector3(0.00, 1.00, 0.00), #solid orientation of the CS
        h # radius of the circle
    ).result()

  def split_circle(self, geom):
      all_geom_splits = []
      for j in range(0, 4):
        u_start = (geom.length() / 4.00 )* (j)
        u_end = (geom.length() / 4.00 )* (j + 1.00)
        geom_split_j = bSplineCurve_bSplineCurveSplitConstructOCC(
          bSplineCurve_convertOCC(geom).result(),
          geom.u_l(u_start),
          geom.u_l(u_end)
        ).result()
        all_geom_splits.append(geom_split_j)

      return all_geom_splits

  def translate_quarter_circles(self, geom_split, b):
      all_geom_split_trans = []
      for k in range(0, 4):
        if k == 0 or k == 3:
          breite = b
        else:
          breite = -b
        geom_split_trans_k = geomCurve_curveTranslateConstructOCC(
          geom_split[k],
          dtVector3(0.00, breite, 0.00)
        ).result()
        all_geom_split_trans.append(geom_split_trans_k)

      return all_geom_split_trans

  def build_top_connection(self, geom_split_trans, all_connection_splits, i):
      if i != 0 :
        uu = geom_split_trans[0].getUMax()
        end_1 = geom_split_trans[0].point(uu)

        uu = geom_split_trans[1].getUMin()
        beginn_2 = geom_split_trans[1].point(uu)

        connection_oben = trimmedCurve_twoPointsConnectConstructOCC(
          end_1,
          beginn_2
        ).result()

        #all_connection_splits = []

        for m in range(0, 2):
          u_start = (connection_oben.length() / 2.00 )* (m)
          u_end = (connection_oben.length() / 2.00 )* (m + 1.00)

          geom_connection_split_m = bSplineCurve_bSplineCurveSplitConstructOCC(
            bSplineCurve_convertOCC(connection_oben).result(),
            connection_oben.u_l(u_start),
            connection_oben.u_l(u_end)
          ).result()

          all_connection_splits.append(geom_connection_split_m)

        #return all_connection_splits

  def build_bottom_connection(self, geom_split_trans, all_connection_splits, i):
      if i !=0 :
        uu = geom_split_trans[2].getUMax()
        end_3 = geom_split_trans[2].point(uu)

        uu = geom_split_trans[3].getUMin()
        beginn_4 = geom_split_trans[3].point(uu)

        connection_unten = trimmedCurve_twoPointsConnectConstructOCC(
          end_3,
          beginn_4
        ).result()

        for n in range(0, 2):  # Beachten Sie, dass range(0, 2) von 0 bis 1 geht.
          u_start = (connection_unten.length() / 2.00 )* (n)
          u_end = (connection_unten.length() / 2.00 )* (n + 1.00)

          geom_connection_split_n = bSplineCurve_bSplineCurveSplitConstructOCC(
            bSplineCurve_convertOCC(connection_unten).result(),
            connection_unten.u_l(u_start),
            connection_unten.u_l(u_end)
          ).result()

          all_connection_splits.append(geom_connection_split_n)

  def assemble_profile(self, geom_split_trans, all_connection_splits, halfDivide_, i):

      vs = vectorHandlingConstDtCurve()

      if i == 0:
        vs.push_back(geom_split_trans[2])
        if self.halfDivide_ == 0:
          vs.push_back(geom_split_trans[3])
          vs.push_back(geom_split_trans[0])
        vs.push_back(geom_split_trans[1])
      else:
        vs.push_back(geom_split_trans[1])
        vs.push_back(geom_split_trans[2])
        vs.push_back(all_connection_splits[2])
        if self.halfDivide_ == 0:
          vs.push_back(all_connection_splits[3])
          vs.push_back(geom_split_trans[3])
          vs.push_back(geom_split_trans[0])
          vs.push_back(all_connection_splits[0])
        vs.push_back(all_connection_splits[1])

      return vs

  def close_profile(self, geom_split_trans_connect):
      return bSplineCurve_curveConnectConstructOCC(
        geom_split_trans_connect,
        1.0E-08, #tol
        0, #param
        0 #minM
      ).result()

  def build_surface(self, vh):
      return analyticSurface(
      bSplineSurface_skinConstructOCC(
        vh,
        2, #minDeg
        6, #maxDeg
        0  #nIter
      ).result()
    )



