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

from dtOOPythonSWIG import baseContainer
from dtOOPythonSWIG import labeledVectorHandlingConstValue
from dtOOPythonSWIG import labeledVectorHandlingAnalyticFunction
from dtOOPythonSWIG import labeledVectorHandlingAnalyticGeometry
from dtOOPythonSWIG import labeledVectorHandlingBoundedVolume
from dtOOPythonSWIG import labeledVectorHandlingDtCase

def checkDecorator(func):
  def inner(
    self,
    bC: baseContainer,
    cV: labeledVectorHandlingConstValue,
    aF: labeledVectorHandlingAnalyticFunction,
    aG: labeledVectorHandlingAnalyticGeometry,
    bV: labeledVectorHandlingBoundedVolume,
    dC: labeledVectorHandlingDtCase
      ):
      bCLab0 = bC.labels()
      cVLab0 = cV.labels()
      aFLab0 = aF.labels()
      aGLab0 = aG.labels()
      bVLab0 = bV.labels()
      dCLab0 = dC.labels()
      ret = func( self, bC, cV, aF, aG, bV, dC )

      retLab0 = []
      retLab1 = ret.labels()
      if isinstance(ret,baseContainer):
        retLab0 = bCLab0
      elif isinstance(ret,labeledVectorHandlingConstValue):
        retLab0 = cVLab0
      elif isinstance(ret,labeledVectorHandlingAnalyticFunction):
        retLab0 = aFLab0
      elif isinstance(ret,labeledVectorHandlingAnalyticGeometry):
        retLab0 = aGLab0
      elif isinstance(ret,labeledVectorHandlingBoundedVolume):
        retLab0 = bVLab0
      elif isinstance(ret,labeledVectorHandlingDtCase):
        retLab0 = dCLab0
      else:
        print("Unknown Type")
      retLab0 = list(retLab0)
      retLab1 = list(retLab1)
      diff = list(set(retLab1) - set(retLab0))

      return ret, diff
  return inner
