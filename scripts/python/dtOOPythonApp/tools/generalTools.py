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
