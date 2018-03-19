import numpy as np
import logging
from pyDtOO.dtMigrateState import dtMigrateState
from sklearn.cluster import KMeans
from pyDtOO.dtInsertFittest import dtInsertFittest

class dtCreateMultiCulti(dtMigrateState):
  def __init__(self, nCultures, mode, randState=None, fDim=0):
    dtMigrateState.__init__(self)
    self.nCultures_ = nCultures
    self.mode_ = mode
    self.randState_ = randState
    self.fDim_ = fDim
    
  def CreateCultures( self, O ):
    theCluster = KMeans(n_clusters=self.nCultures_, random_state=self.randState_)
    clusterId = theCluster.fit_predict( O )
    return clusterId

  def Mixture(self, I, O, F, popO, popF, clusterId):
    nInd = np.shape( popF )[0]
    
    chosenCultures = np.random.choice( 
      range(self.nCultures_), nInd, replace=False 
    )
    for i in range(nInd):
      thisCultureI = I[ clusterId == chosenCultures[i] ]
      thisCultureO = O[ clusterId == chosenCultures[i], : ]
      thisCultureF = F[ clusterId == chosenCultures[i], : ]
      fP, fI = dtInsertFittest(self.fDim_).DetectFittest(
        thisCultureI, thisCultureF
      )
      popO[i, :] = thisCultureO[fP, :]
      popF[i, :] = thisCultureF[fP, :]
    
    return popO, popF
  
  def Elite(self, I, O, F, popO, popF, clusterId):
    nInd = np.shape( popF )[0]
    
    chosenCulture = np.random.randint(self.nCultures_)

    thisCultureI = I[ clusterId == chosenCulture ]
    thisCultureO = O[ clusterId == chosenCulture, : ]
    thisCultureF = F[ clusterId == chosenCulture, : ]
    replace = False
    if np.size(thisCultureI) < nInd:
      logging.info(
        'Create elite culture with replacement. culture size = %d, nInd = %d' 
        % ( np.size(thisCultureI), nInd )
      )         
      replace = True
    chosenIndividuals = np.random.choice( 
      np.size(thisCultureI), nInd, replace=replace
    )
    for i in range(nInd):
      popO[i, :] = thisCultureO[chosenIndividuals[i], :]
      popF[i, :] = thisCultureF[chosenIndividuals[i], :]
    
    return popO, popF
  
  def Migrate( self, poolI, poolO, poolF, popO, popF, islandIndex=-1 ):
    nInd = np.shape( popF )[0]
    if nInd > self.nCultures_:
      raise ValueError(
        'More individuals in poulation ( %d ) than number of cultures ( %d ).'
        % (nInd, self.nCultures_)
      )
    clusterId = self.CreateCultures( poolO )

    logging.info(
      'Create multi culti population with nCultures = %d, mode = %s, fDim = %d' 
      % ( self.nCultures_, self.mode_, self.fDim_ )
    )    
    if self.mode_ == 'mix':
      return self.Mixture(poolI, poolO, poolF, popO, popF, clusterId)
    elif self.mode_ == 'elite':
      return self.Elite(poolI, poolO, poolF, popO, popF, clusterId)
    else:
      raise ValueError( 'mode = > %s < not supported' )
      