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

from dtOOPythonSWIG import vectorHandlingRenderInterface
from dtOOPythonSWIG import solid3dSurface
from dtOOPythonSWIG import solid3dLine

import numpy as np
from typing import Dict, List, Tuple

class basicArray_renderInterface:
  """Convert instances of renderInterface to basic arrays.

  The renderInterface is converted to basic arrays, e.g. points, triangles,
  lines and quadrangles.

  Attributes
  ----------
  vHrI_ : vectorHandlingRenderInterface
    renderInterface to be converted.
  
  Examples
  --------

  """
  
  def __init__( self, vHrI: vectorHandlingRenderInterface ) -> None:
    """Constructor

    Store vectorHandlingRenderInterface as attribute.

    Parameters
    ----------
    vHrI : vectorHandlingRenderInterface
      renderInterface to be converted.

    Returns
    -------
    None

    """
    self.vHrI_ = vHrI

  @staticmethod
  def defReturn() -> Dict[str, List]:
    """Create structure of return argument for the __call__ method.

    Parameters
    ----------
    None

    Returns
    -------
    Dict[str, List[]]
      Empty dictionary containing points, triangles, lines and quadrangles.

    """
    return  {'points': [], 'triangles': [],'lines': [], 'quadrangles': []} 
  
  @staticmethod
  def correctReturn( mm: Dict[str, List] ) -> Dict[str, List]:
    """Correct size of return argument.

    The return argument is corrected to contain the same number of lists as
    the point list.

    Parameters
    ----------
    mm : Dict[str, List]
      Return argument with wrong size.

    Returns
    -------
    Dict[str, List]
      Return argument with correct size.

    """
    maxLen = len(mm['points'])
    for key in mm.keys():
      if key == 'points':
        continue
      toAdd = maxLen - len( mm[key] ) 
      for ii in range(toAdd):
        mm[key].append( np.zeros((0,0), int) )
    return mm  

  def __call__( self, mm: Dict[str, List] ) -> Dict[str, List]:
    """Creates the basic arrays for the stored vectorHandlingRenderInterface.

    Parameters
    ----------
    mm : Dict[str, List]
      Return argument to be extended.

    Returns
    -------
    Dict[str, List]
      Return argument.
    
    """
    for rI in self.vHrI_:
      if solid3dSurface.Is( rI ):
        sS = solid3dSurface.DownCast( rI )
        points, quads = basicArray_renderInterface.solid3dSurface2Quadrangles(sS)
        mm['points'].append( points )
        mm['quadrangles'].append( quads ) 
        mm = basicArray_renderInterface.correctReturn(mm)
      elif solid3dLine.Is( rI ):
        sL = solid3dLine.DownCast( rI )
        points, lines = basicArray_renderInterface.solid3dLine2Lines(sL)
        mm['points'].append( points )
        mm['lines'].append( lines )
        mm = basicArray_renderInterface.correctReturn(mm)
      else:
        print( "Unsupported type %s " % (rI.virtualClassName()) )

    return mm

  @staticmethod
  def solid3dSurface2Triangles( 
    sS : solid3dSurface 
  ) -> Tuple[np.ndarray, np.ndarray]:
    """Convert solid3dSurface to triangles.
    
    Parameters
    ----------
    sS : solid3dSurface
      Surface to be converted.

    Returns
    -------
    Tuple[np.ndarray, np.ndarray]
      Tuple containing [points, triangles].
    """
    pp = sS.refP3()
   
    dx = pp.size(0)
    dy = pp.size(1)
    nP = dx * dy
    nE = (dx-1) * (dy-1) * 2
    points = np.zeros((nP,3),float)
    counter = 0
    for jj in range(dy):
      for ii in range(dx):
        xyz = pp[ii][jj]
        points[counter][0] = xyz.x()
        points[counter][1] = xyz.y()
        points[counter][2] = xyz.z()
        counter = counter + 1
    
    counter = 0
    tris = np.zeros((nE,3),int)
    for jj in range(dy-1):
      for ii in range(dx-1):
        pId = jj*dx + ii
        tris[counter][0] = pId + 0#ii*dx
        tris[counter][1] = pId + 1#ii*dx+1
        tris[counter][2] = pId + 1 + dx#ii*dx+jj*dy
        tris[counter+1][0] = pId + 0#ii*dx
        tris[counter+1][1] = pId + 1 + dx#ii*dx+1
        tris[counter+1][2] = pId + dx#ii*dx+jj*dy
        counter = counter + 2
    
    return points, tris

  @staticmethod
  def solid3dSurface2Quadrangles( 
    sS : solid3dSurface 
  ) -> Tuple[np.ndarray, np.ndarray]:
    """Convert solid3dSurface to quadrangles.
    
    Parameters
    ----------
    sS : solid3dSurface
      Surface to be converted.

    Returns
    -------
    Tuple[np.ndarray, np.ndarray]
      Tuple containing [points, quadrangles].
    """
    pp = sS.refP3()
   
    dx = pp.size(0)
    dy = pp.size(1)
    nP = dx * dy
    nE = (dx-1) * (dy-1)
    points = np.zeros((nP,3),float)
    counter = 0
    for jj in range(dy):
      for ii in range(dx):
        xyz = pp[ii][jj]
        points[counter][0] = xyz.x()
        points[counter][1] = xyz.y()
        points[counter][2] = xyz.z()
        counter = counter + 1
    
    counter = 0
    quads = np.zeros((nE,4),int)
    for jj in range(dy-1):
      for ii in range(dx-1):
        pId = jj*dx + ii
        quads[counter][0] = pId
        quads[counter][1] = pId      + 1
        quads[counter][2] = pId + dx + 1
        quads[counter][3] = pId + dx
        counter = counter + 1
    
    return points, quads

  @staticmethod
  def solid3dLine2Lines( 
    sL : solid3dLine 
  ) -> Tuple[np.ndarray, np.ndarray]:
    """Convert solid3dLine to lines.
    
    Parameters
    ----------
    sS : solid3dLine
      Line to be converted.

    Returns
    -------
    Tuple[np.ndarray, np.ndarray]
      Tuple containing [points, lines].
    """
    pp = sL.refP3()
    nP = pp.size()
    nE = nP - 1
    points = np.zeros((nP,3),float)
    
    for ii in range(nP):
      xyz = pp[ii]
      points[ii][0] = xyz.x()
      points[ii][1] = xyz.y()
      points[ii][2] = xyz.z()
    pId = 0
    lines = np.zeros((nE,2),int)
    
    for ii in range(nE):
      lines[ii][0] = pId + 0
      lines[ii][1] = pId + 1
      pId = pId + 1
    
    return points, lines 
