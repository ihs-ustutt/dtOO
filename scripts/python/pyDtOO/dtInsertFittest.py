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

import numpy as np
import logging
from pyDtOO.dtMigrateState import dtMigrateState
  
class dtInsertFittest(dtMigrateState):
  def __init__(self, fDim = 0):
    dtMigrateState.__init__(self)
    self.fDim_ = fDim
    
  def DetectFittest( self, I, F ):
    fertilizerPos = dtMigrateState.FittestPosition( F, self.fDim_)
    fertilizerId = I[fertilizerPos]
    fertilizerFit = F[fertilizerPos]
        
    logging.info(
      'Current community fertilizer id = %d, fitness = %f', 
      fertilizerId, fertilizerFit
    )      
    return fertilizerPos, fertilizerId
  
  def Migrate( self, poolI, poolO, poolF, popO, popF, islandIndex=-1 ):
    fertilizerPos, fertilizerId = self.DetectFittest(poolI, poolF)
    popWorstIndPos = np.argmax( popF[:, self.fDim_] )
    popWorstIndFit = popF[ popWorstIndPos, self.fDim_ ]
    popBestIndPos = np.argmin( popF[:, self.fDim_] )
    popBestIndFit = popF[ popBestIndPos, self.fDim_ ]

    logging.info(
      'Best individual id = %d, fitness = %f / worst individual id = %d, fitness = %f' 
      % (popBestIndPos, popBestIndFit, popWorstIndPos, popWorstIndFit)
    )
    
    if ( poolF[ fertilizerPos, self.fDim_ ] < popBestIndFit ):
      logging.info(
        'Community fertilizer id = %d, fitness = %f -> pop[ %d ] = %f' 
        % (fertilizerId, poolF[ fertilizerPos, self.fDim_ ], popWorstIndPos, popWorstIndFit)
      )
      #pop.set_xf(popWorstIndPos, O[fertilizerPos, :], F[fertilizerPos, :])
      popO[ popWorstIndPos, : ] = poolO[fertilizerPos, :]
      popF[ popWorstIndPos, self.fDim_ ] = poolF[fertilizerPos, self.fDim_]
  
    return popO, popF
