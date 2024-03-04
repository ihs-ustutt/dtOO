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

#from dtOOPythonSWIG import analyticFunction

from abc import ABC,abstractmethod

class generalStrategy(ABC):
  def creates(self) -> str:
    return type(self).__name__

  @staticmethod
  def depBaseContainer():
    return []

  @staticmethod
  def depConstValue():
    return []

  @staticmethod
  def depAnalyticFunction():
    return []

  @staticmethod
  def depAnalyticGeometry():
    return []

  @staticmethod
  def depBoundedVolume():
    return []

  def dep(self):
    return self.depBaseContainer()+self.depConstValue()\
      +self.depAnalyticFunction()+self.depAnalyticGeometry()\
      +self.depBoundedVolume()
