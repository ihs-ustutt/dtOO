import os
os.environ["OSLO_LOCK_PATH"] = "./runLock"
from pyDtOO import dtClusteredSingletonState

M = dtClusteredSingletonState.fullAddRead(
  ['eta', 'islandID'],
  [ dict, int]
)
