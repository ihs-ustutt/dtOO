import numpy as np
import logging
from pyDtOO.dtMigrateState import dtMigrateState
  
class dtInsertFittest(dtMigrateState):
  def __init__(self, fDim = 0):
    dtMigrateState.__init__(self)
    self.fDim_ = fDim
    
  def DetectFittest( self, I, F ):
    fertilizerId = -1
    fertilizerPos = -1
    fertilizerFit = float('inf')
    for i in range( np.size(I) ):
      fit = F[i,self.fDim_]#s.fitness()
      if fit < fertilizerFit:
        fertilizerId = I[i]#s.id()
        fertilizerPos = i
        fertilizerFit = fit
        
    logging.info(
      'Current community fertilizer id = %d, fitness = %f', 
      fertilizerId, fertilizerFit
    )      
    return fertilizerPos, fertilizerId
  
  def Migrate( self, I, O, F, popO, popF ):
    fertilizerPos, fertilizerId = self.DetectFittest(I, F)
    popWorstIndPos = np.argmax( popF[:, self.fDim_] )
    popWorstIndFit = popF[ popWorstIndPos, self.fDim_ ]
    popBestIndPos = np.argmin( popF[:, self.fDim_] )
    popBestIndFit = popF[ popBestIndPos, self.fDim_ ]

    logging.info(
      'Best individual id = %d, fitness = %f / worst individual id = %d, fitness = %f' 
      % (popBestIndPos, popBestIndFit, popWorstIndPos, popWorstIndFit)
    )
    
    if ( F[ fertilizerPos, self.fDim_ ] < popBestIndFit ):
      logging.info(
        'Community fertilizer id = %d, fitness = %f -> pop[ %d ] = %f' 
        % (fertilizerId, F[ fertilizerPos, self.fDim_ ], popWorstIndPos, popWorstIndFit)
      )
      #pop.set_xf(popWorstIndPos, O[fertilizerPos, :], F[fertilizerPos, :])
      popO[ popWorstIndPos, : ] = O[fertilizerPos, :]
      popF[ popWorstIndPos, self.fDim_ ] = F[fertilizerPos, self.fDim_]
  
    return popO, popF