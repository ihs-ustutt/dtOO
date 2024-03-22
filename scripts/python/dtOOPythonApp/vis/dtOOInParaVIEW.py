#------------------------------------------------------------------------------
#  dtOO < design tool Object-Oriented >
#    
#    Copyright (C) 2024 A. Tismer.
#------------------------------------------------------------------------------
#License
#    This file is part of dtOO.
#
#    dtOO is distributed in the hope that it will be useful, but WITHOUT
#    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
#    dtOO root directory for more details.
#
#    You should have received a copy of the License along with dtOO.
#
#------------------------------------------------------------------------------

from dtOOPythonSWIG import dtBundle
from dtOOPythonSWIG import baseContainer
from dtOOPythonSWIG import labeledVectorHandlingConstValue
from dtOOPythonSWIG import labeledVectorHandlingAnalyticFunction
from dtOOPythonSWIG import labeledVectorHandlingAnalyticGeometry
from dtOOPythonSWIG import labeledVectorHandlingBoundedVolume
from dtOOPythonSWIG import labeledVectorHandlingDtCase
from dtOOPythonSWIG import renderInterface

from .basicArray_renderInterface import basicArray_renderInterface

from vtkmodules.vtkCommonDataModel import vtkPolyData
import vtk
from paraview.simple import *

import numpy as np
from typing import List

class dtOOInParaVIEW:
  """Visualization of dtOO objects in ParaVIEW.

  Attributes
  ----------
  bC_ : baseContainer
    baseContainer's objects.
  aF_ : labeledVectorHandlingAnalyticFunction
    labeledVectorHandlingAnalyticFunction's objects.
  aG_ : labeledVectorHandlingAnalyticGeometry
    labeledVectorHandlingAnalyticGeometry's objects
  bV_ : labeledVectorHandlingBoundedVolume
    labeledVectorHandlingBoundedVolume's objects.
   
  Examples
  --------
  
  Create an object:

  > rr = dtOOInParaVIEW(ma)

  Search for a label within the object:
  
  > rr.Find("aS_ru_blade$")
    analyticFunction:  []
    analyticGeometry:  ['aS_ru_blade']
    boundedVolume:     []
    []

  Show the object in ParaVIEW

  > rr.Show( rr.Find("aS_ru_blade$", True) )
    analyticFunction:  []
    analyticGeometry:  ['aS_ru_blade']
    boundedVolume:     []
    Unsupported type discrete3dVector 
    Create 441 points and 400 cells

  """

  def __init__( self, dtCon: dtBundle ) -> None:
    """Constructor.

    Parameters
    ----------
    dtCon : dtBundle
      Bundle containing all objects for rendering

    Returns
    -------
    None
    """
    self.init(
      dtCon.cptr_bC(), dtCon.cptr_aF(), dtCon.cptr_aG(), dtCon.cptr_bV()
    )

  def init(
    self,
    bC: baseContainer,
    aF: labeledVectorHandlingAnalyticFunction,
    aG: labeledVectorHandlingAnalyticGeometry,
    bV: labeledVectorHandlingBoundedVolume,
  ) -> None:
    """Init.

    Parameters
    ----------
    bC : baseContainer
      baseContainer's objects.
    aF : labeledVectorHandlingAnalyticFunction
      labeledVectorHandlingAnalyticFunction's objects.
    aG : labeledVectorHandlingAnalyticGeometry
      labeledVectorHandlingAnalyticGeometry's objects
    bV : labeledVectorHandlingBoundedVolume
      labeledVectorHandlingBoundedVolume's objects.

    Returns
    -------
    None
    """
    self.bC_ = bC
    self.aF_ = aF
    self.aG_ = aG
    self.bV_ = bV

  def WhatAF(self) -> List[str]:
    """Labels within labeledVectorHandlingAnalyticFunction.

    Parameters
    ----------
    None

    Returns
    -------
    List[str]
      Labels.
    """
    ret = []
    for ii in self.aF_.labels():
      ret.append(ii)
    return ret

  def WhatAG(self) -> str:
    """Labels within labeledVectorHandlingAnalyticGeometry.

    Parameters
    ----------
    None

    Returns
    -------
    List[str]
      Labels.
    """
    ret = []
    for ii in self.aG_.labels():
      ret.append(ii)  
    return ret

  def WhatBV(self) -> str:
    """Labels within labeledVectorHandlingBoundedVolume.

    Parameters
    ----------
    None

    Returns
    -------
    List[str]
      Labels.
    """
    ret = []
    for ii in self.bV_.labels():
      ret.append(ii)  
    return ret


  def Find( 
    self, pattern: str, returnRenderInterface: bool = False 
  ) -> List[renderInterface]:
    """Find objects with a desired label.

    Within all stored objects the label is searched. The label can be a RegExp
    expression.

    Parameters
    ----------
    pattern : str
      Label to search for.
    returnRenderInterface : bool
      Flag if the List[renderInterface] is returned containing all objects that
      match the label.

    Returns
    -------
    List[renderInterface]
      List containing all renderIntefaces.
    """
    import re

    ret = []

    r = re.compile(pattern)
    #find_aF = filter( lambda a: pattern in a, self.whatAF() )
    find_aF = list( filter(r.match, self.WhatAF()) )
    print( "analyticFunction: ", find_aF )
    for ii in find_aF:
      ret.extend( self.aF_[ii].getRender() )
    #find_aG = filter( lambda a: pattern in a, self.whatAG() )
    find_aG = list( filter(r.match, self.WhatAG()) )
    print( "analyticGeometry: ", find_aG )
    for ii in find_aG:
      ret.extend( self.aG_[ii].getRender() )
    #find_bV = filter( lambda a: pattern in a, self.whatBV() )
    find_bV = list( filter(r.match, self.WhatBV()) )
    print( "boundedVolume:    ", find_bV )
    for ii in find_bV:
      ret.extend( self.bV_[ii].getRender() )
    
    if not returnRenderInterface:
      ret = []

    return ret

  def Show(
      self, parts: List[renderInterface], guiName: str =''
  ) -> TrivialProducer:
    """Render object in ParaVIEW.

    Parameters
    ----------
    parts: List[renderInterface]
      List of objects to be shown in ParaVIEW.
    guiName : str
      Source's name in dtOOInParaVIEW.

    Returns
    -------
    TrivialProducer
      ParaVIEW source containing the objects.
    """
    output = vtkPolyData()
    nCells = 0
    nPoints = 0
    mm = basicArray_renderInterface.defReturn()
    pL = mm['points']
    tL = mm['triangles']
    lL = mm['lines']
    qL = mm['quadrangles']

    mm = basicArray_renderInterface( parts )(mm)
    pL = mm['points']
    tL = mm['triangles']
    lL = mm['lines']
    qL = mm['quadrangles']
    nPoints = nPoints + np.shape(pL[-1])[0]
    nCells = nCells \
    + \
    np.shape(tL[-1])[0] + np.shape(lL[-1])[0] + np.shape(qL[-1])[0] 
 
    print("Create %d points and %d cells" % (nPoints, nCells) )
    output.Allocate(nCells)
    pp = vtk.vtkPoints()
    pp.Allocate(nPoints)
    pOff = 0 
    for jj in range( len(pL) ): 
      for ii in range( np.shape(pL[jj])[0] ):
        pp.InsertNextPoint(pL[jj][ii][0], pL[jj][ii][1], pL[jj][ii][2] )
      output.SetPoints( pp )
      for ii in range( np.shape(lL[jj])[0] ):
        output.InsertNextCell( 
          vtk.VTK_LINE, 
          2, 
          (
            lL[jj][ii][0] + pOff,
            lL[jj][ii][1] + pOff,
          ) 
        )
      for ii in range( np.shape(tL[jj])[0] ):
        output.InsertNextCell( 
          vtk.VTK_TRIANGLE, 
          3, 
          (
            tL[jj][ii][0] + pOff,
            tL[jj][ii][1] + pOff,
            tL[jj][ii][2] + pOff
          ) 
        )
      for ii in range( np.shape(qL[jj])[0] ):
        output.InsertNextCell( 
          vtk.VTK_QUAD, 
          4, 
          (
            qL[jj][ii][0] + pOff,
            qL[jj][ii][1] + pOff,
            qL[jj][ii][2] + pOff,
            qL[jj][ii][3] + pOff,
          ) 
        )
        
      pOff = output.GetNumberOfPoints()
    if guiName == '':
      import time
      guiName = 'TP-'+time.ctime()
    tp = TrivialProducer(guiName=guiName).GetClientSideObject().SetOutput(output)
    Show( tp )
    return tp
 
  def FindAndShow( self, pattern: str ) -> TrivialProducer:
    return self.Show( self.Find( pattern, True ) ) 
