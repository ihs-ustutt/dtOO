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
from pyDtOO.dtCylField import dtCylField
from pyDtOO.dtCylTrf import dtCylTrf
import numpy

class dtRScaleCylField(dtCylField):
  def __init__(self, fn):
    dtCylField.__init__(self, fn)
    
  def Read(self):
    data = dtCylField.Read(self)

    data_shape = numpy.shape(data)
   
    if data_shape[1] == 8:
      logging.info( 'Multiplicate scalar field')    
      data[:,3] = data[:,3] * data[:,0]
    elif data_shape[1] == 10:
      logging.info( 'Multiplicate vector field')
      data[:,3] = data[:,3] * data[:,0]
      data[:,4] = data[:,4] * data[:,0]
      data[:,5] = data[:,5] * data[:,0]
    else:
      raise ValueError('Bad shape of data array.')

    return data
