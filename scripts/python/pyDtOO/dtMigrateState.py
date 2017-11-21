from abc import ABCMeta, abstractmethod

class dtMigrateState:
  __metaclass__ = ABCMeta
  def __init__(self):
    pass
  
  @abstractmethod
  def Migrate( self, I, O, F, popO, popF ):
    pass