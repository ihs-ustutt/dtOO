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

import logging
from pyDtOO.dtField import dtField
from pyDtOO.dtCylTrf import dtCylTrf

class dtCylField(dtField):
  def __init__(self, fn):
    dtField.__init__(self, fn)
    
  def Read(self):
    data = dtField.Read(self)
    logging.info('Convert to cylindrical coordinates')
    trf = dtCylTrf(data[:,0], data[:,1], data[:,2])
    data[:,0] = trf.R()
    data[:,1] = trf.Phi()
    data[:,2] = trf.Z()
    return data
