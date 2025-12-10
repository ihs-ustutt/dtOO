from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder 

from dtOOPythonSWIG import (
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
  trimmedCurve_twoPointsConnectConstructOCC,
)

from typing import List
import logging

class draftTubeDiffusor(dtBundleBuilder):

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
    super(draftTubeDiffusor, self).__init__()
    self.label_ = label
    self.hcurve_ = hDistCurve
    self.bcurve_ = bDistCurve
    self.rcurve_ = rDistCurve
    self.curve_ = surgeCurve
    self.resolution_ = resolution
    self.halfDivide_ = halfDivide

  def build(self) -> None:

    #Bezugsgrößen & Gesamtvektor definieren
    curve = analyticCurve.MustConstDownCast( self.curve_ ).ptrDtCurve()
    sectionsLength = curve.length() / ( self.resolution_ - 1 )
    vh = vectorHandlingConstDtCurve()
    elbowLength = 7.0123

    #Schüsse in der Schleife erstellen, Anzahl: resolution-1
    for i in range(self.resolution_):
      logging.debug( "Create diffusor section number %d ..." % (i) )
      l_section = i * sectionsLength
      u_section = curve.u_l(l_section)

      #Einbinden der h und b Verteilung
      h = self.hcurve_(l_section + elbowLength)[0] / 2
      b = self.bcurve_(l_section + elbowLength)[0] / 2
      r = self.rcurve_(l_section + elbowLength)[0]

      #Abfangen des ersten Querschnitts
      if i == 0:
        r = h

      #Grundkreis erstellen
      geom = circle_radiusCoordinateSystemConstructOCC(
        curve.point(u_section), #point
        dtVector3(1.00, 0.00, 0.00), #vector of rotation
        dtVector3(0.00, 1.00, 0.00), #solid orientation of the CS
        r # radius of the circle
      ).result()

      #Grundkreis splitten
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

      all_geom_split_trans = []

      #Viertelkreise in die Breite verschieben
      for k in range(0, 4):
        if k == 0 or k == 3:
          breite = b
        else:
          breite = -b

        geom_split_trans_b_k = geomCurve_curveTranslateConstructOCC(
          all_geom_splits[k],
          dtVector3(0.00, breite, 0.00)
        ).result()

        all_geom_split_trans.append(geom_split_trans_b_k)

      #Viertelkreise in die Höhe verschieben - nicht für ersten Querschnitt
      if i != 0:
        for l in range(0, 4):
          if l == 0 or l == 1:
            hoehe = h - r
          else:
            hoehe = -h + r

          geom_split_trans_b_h_l = geomCurve_curveTranslateConstructOCC(
            all_geom_split_trans[l],
            dtVector3(0.00, 0.00, hoehe)
          ).result()

          all_geom_split_trans[l] = geom_split_trans_b_h_l

      #Verbindungsstück oben bauen
      uu = all_geom_split_trans[0].getUMax()
      end_1 = all_geom_split_trans[0].point(uu)

      uu = all_geom_split_trans[1].getUMin()
      beginn_2 = all_geom_split_trans[1].point(uu)

      connection_oben = trimmedCurve_twoPointsConnectConstructOCC(
        end_1,
        beginn_2
      ).result()

      all_connection_splits = []

      for m in range(0, 2):
        u_start = (connection_oben.length() / 2.00 )* (m)
        u_end = (connection_oben.length() / 2.00 )* (m + 1.00)

        geom_connection_split_m = bSplineCurve_bSplineCurveSplitConstructOCC(
          bSplineCurve_convertOCC(connection_oben).result(),
          connection_oben.u_l(u_start),
          connection_oben.u_l(u_end)
        ).result()

        all_connection_splits.append(geom_connection_split_m)

      #Verbindungsstück unten bauen
      uu = all_geom_split_trans[2].getUMax()
      end_3 = all_geom_split_trans[2].point(uu)

      uu = all_geom_split_trans[3].getUMin()
      beginn_4 = all_geom_split_trans[3].point(uu)

      connection_unten = trimmedCurve_twoPointsConnectConstructOCC(
        end_3,
        beginn_4
      ).result()

      for n in range(0, 2):
        u_start = (connection_unten.length() / 2.00 )* (n)
        u_end = (connection_unten.length() / 2.00 )* (n + 1.00)

        geom_connection_split_n = bSplineCurve_bSplineCurveSplitConstructOCC(
          bSplineCurve_convertOCC(connection_unten).result(),
          connection_unten.u_l(u_start),
          connection_unten.u_l(u_end)
        ).result()

        all_connection_splits.append(geom_connection_split_n)

      #Verbindungsstück rechts und links bauen - nicht für ersten Querschnitt
      if i != 0:
        #links
        uu = all_geom_split_trans[3].getUMax()
        end_3 = all_geom_split_trans[3].point(uu)

        uu = all_geom_split_trans[0].getUMin()
        beginn_4 = all_geom_split_trans[0].point(uu)

        connection_links = trimmedCurve_twoPointsConnectConstructOCC(
          end_3,
          beginn_4
        ).result()
        #rechts
        uu = all_geom_split_trans[1].getUMax()
        end_3 = all_geom_split_trans[1].point(uu)

        uu = all_geom_split_trans[2].getUMin()
        beginn_4 = all_geom_split_trans[2].point(uu)

        connection_rechts = trimmedCurve_twoPointsConnectConstructOCC(
          end_3,
          beginn_4
        ).result()

      #Einzelne Splines in richtiger Reihnvolge in Vektor hinzufügen
      vs = vectorHandlingConstDtCurve()

      if i == 0:
        vs.push_back(all_geom_split_trans[1])
        vs.push_back(all_geom_split_trans[2])
        vs.push_back(all_connection_splits[2])
        if self.halfDivide_ == 0:
          vs.push_back(all_connection_splits[3])
          vs.push_back(all_geom_split_trans[3])
          vs.push_back(all_geom_split_trans[0])
          vs.push_back(all_connection_splits[0])
        vs.push_back(all_connection_splits[1])
      else:
        vs.push_back(all_geom_split_trans[1])
        vs.push_back(connection_rechts)
        vs.push_back(all_geom_split_trans[2])
        vs.push_back(all_connection_splits[2])
        if self.halfDivide_ == 0:
          vs.push_back(all_connection_splits[3])
          vs.push_back(all_geom_split_trans[3])
          vs.push_back(connection_links)
          vs.push_back(all_geom_split_trans[0])
          vs.push_back(all_connection_splits[0])
        vs.push_back(all_connection_splits[1])

      #geschlossenes Profil zusammen setzen
      geom_connect = bSplineCurve_curveConnectConstructOCC(
        vs,
        1.0E-08, #tol
        20, #param
        5 #minM
      ).result()

      #Kurven darstellen
      # bb = analyticCurve(geom_connect)
      # bb.setLabel(self.label_+"_connected_" + str(i))
      # self.appendAnalyticGeometry( bb )

      vh.push_back(geom_connect)
      #ENDE FOR-SCHLEIFE

    ref = analyticSurface(
      bSplineSurface_skinConstructOCC(
        vh,
        1, #minDeg
        4, #maxDeg
        0  #nIter
      ).result()
    )

    ref.setLabel(self.label_)

    self.appendAnalyticGeometry( ref )

    return
