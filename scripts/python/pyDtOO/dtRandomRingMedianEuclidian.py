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
  
class dtRandomRingMedianEuclidian(dtMigrateState):
  class dtMigrationDestination:
    def __init__( self, dest, obj, fit ):
      self.__pos_ = np.array( dest )
      self.__destSize_ = np.size(dest)
      self.__state_ = np.zeros(self.__destSize_, int)
      self.__O_ = np.array(obj)
      self.__F_ = np.array(fit)
      
    def GetObjAndFitForIsland(self, islI):
      for i in range( self.__destSize_ ):
        if self.__pos_[i] == islI:
          if self.__state_[i] == 0:
            self.__state_[i] = 1
            return self.__O_, self.__F_
      return None, None

    def Done(self):
      for i in range( self.__destSize_ ):
        if self.__state_[i] == 0:
          return False
      return True
  
  DESTINATION = []
    
  def __init__(self, numIslands, fDim = 0, topR = -1, invert = False):
    dtMigrateState.__init__(self)
    self.numIslands_ = numIslands
    self.fDim_ = fDim
    self.topR_ = topR
    if topR==-1:
      self.topR_ = numIslands-1
    self.invert_ = invert
    

  def __CreatePartnersOfIsland( self, islIndex ):
    r = np.random.randint( 1, self.topR_ )
    right = np.mod( islIndex + r, self.numIslands_ )
    left = np.mod( islIndex + self.numIslands_ - r, self.numIslands_ )    
    
    logging.info(
      'Create %d <- ( %d ) -> %d', 
      left, islIndex, right
    )

    return [left, right]

  def Migrate( self, I, O, F, popO, popF, islandIndex ):
    ED = dtMigrateState.MedianEuclidianDistance( popO )
    
    indId = -1
    if ( not self.invert_ ):
      indId = np.argmin( ED )
    else:
      indId = np.argmax( ED )
    logging.info( 
      'Choose migrant on position %d in island %d.', indId, islandIndex 
    )        
      
    dtRandomRingMedianEuclidian.DESTINATION.append( 
      self.dtMigrationDestination( 
        self.__CreatePartnersOfIsland(islandIndex), popO[indId,:], popF[indId,:]
      )
    )

    toRemove = []
    for aDest in dtRandomRingMedianEuclidian.DESTINATION:
      obj, fit = aDest.GetObjAndFitForIsland( islandIndex )
      if (obj is None) and (fit is None):
        if ( aDest.Done() ):
          toRemove.append( aDest )
      else:
        logging.info( 
          'Insert individual at position %d in island %d.', indId, islandIndex 
        )        
        popO[indId,:] = obj
        popF[indId,:] = fit
        break
    
    for aRemove in toRemove:
      dtRandomRingMedianEuclidian.DESTINATION.remove( aRemove )
        
    return popO, popF
