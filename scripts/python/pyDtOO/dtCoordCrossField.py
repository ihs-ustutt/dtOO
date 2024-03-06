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
import numpy

class dtCoordCrossField(dtField):
  def __init__(self, fn):
    dtField.__init__(self, fn)
    
  def Read(self):
    data = dtField.Read(self)

    data_shape = numpy.shape(data)
   
    if data_shape[1] == 8:
      logging.info( 'Vectorize scalar field')
      # insert columns
      data = numpy.c_[ 
        data[:,0], 
        data[:,1], 
        data[:,2], 
        data[:,3], 
        numpy.zeros(data_shape[0], float), 
        numpy.zeros(data_shape[0], float), 
        data[:,4:] 
      ]
      # vectorize
      for i in range(numpy.shape(data)[0]):
        val = data[i,3]
        area = numpy.sqrt(data[i,6]**2.0 + data[i,7]**2.0 + data[i,8]**2.0)
        data[i,3] = val * data[i,6] / area
        data[i,4] = val * data[i,7] / area
        data[i,5] = val * data[i,8] / area
      # update data_shape
      data_shape = numpy.shape(data)
    
    if data_shape[1] == 10:
      logging.info( 'Multiplicate vector field')
      for i in range(numpy.shape(data)[0]):
        x_t = data[i,1] * data[i,5] - data[i,4] * data[i,2]
        y_t = data[i,2] * data[i,3] - data[i,5] * data[i,0]
        z_t = data[i,0] * data[i,4] - data[i,1] * data[i,3]
        data[i,3] = x_t
        data[i,4] = y_t
        data[i,5] = z_t
    else:
      raise ValueError('Bad shape of data array.')

    return data
