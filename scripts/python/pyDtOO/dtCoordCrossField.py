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
      logging.info( 'Multiplicate scalar field')    
      data[:,3] = data[:,3] * data[:,0]
    elif data_shape[1] == 10:
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