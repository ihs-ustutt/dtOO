from abc import ABCMeta, abstractmethod
import numpy as np

class dtMigrateState:
  __metaclass__ = ABCMeta
  def __init__(self):
    pass
  
  @staticmethod
  def FittestPosition( popF, fDim=0 ):
    return np.argmin( popF[:,fDim] )

  @staticmethod
  def HeterogeneityToPosition( popO, pos ):
    #return np.argmin( popF[:,fDim] )
    m = np.shape( popO )[0]
    n = np.shape( popO )[1]
    sum_i = 0.
    for i in range(m):
      sum_j = 0.
      for j in range(n):
        sum_j = sum_j + (popO[pos, j] - popO[i, j])**2.
      sum_i = sum_i + np.sqrt( sum_j )
    
    return 1./(m-1.) * sum_i

  @staticmethod
  def HeterogeneityToFittest( popO, popF, fDim=0 ):
    return dtMigrateState.HeterogenityToPosition(
      popO, dtMigrateState.FittestPosition( popF, fDim)
    )

  @staticmethod
  def MedianEuclidianDistanceToPosition( popO, pos ):
    m = np.shape( popO )[0]
    ED = np.zeros(m-1, float)
    c = 0
    for i in range(m):
      if i==pos:
        continue
      ED[c] = np.linalg.norm( popO[i, :] - popO[pos,:] )
      c = c+1
    return np.median( ED )
  
  @staticmethod
  def MedianEuclidianDistance( popO ):
    m = np.shape( popO )[0]
    ED = np.zeros(m, float)
    for i in range(m):
      ED[i] = dtMigrateState.MedianEuclidianDistanceToPosition(popO, i)
    return ED
  
  @abstractmethod
  def Migrate( self, poolI, poolO, poolF, popO, popF, islandIndex ):
    pass