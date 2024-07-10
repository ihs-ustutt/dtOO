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

import sys
import os
os.environ["OSLO_LOCK_PATH"] = "./runLock"
import numpy as np
from pyDtOO import dtClusteredSingletonState

defObj = [1.0, 2.0]
defFit = 1.0

s = dtClusteredSingletonState(defObj=defObj, defFit=defFit)

print("defObj")
if np.array_equal( s.objective(), defObj):
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)

print("defFit")
if s.fitness() == defFit:
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)
