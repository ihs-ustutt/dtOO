import numpy
import logging

class dtValueSpace:
  def __init__(self, data):
    self.data_ = data
    data_shape = numpy.shape(data)

    self.coord_ = 0
    self.value_ = 0
    self.v_ = 0
    self.vDim_ = 0
    
    if data_shape[0] == 0:
      return
    
    self.coord_ = data[:,0:3]
    
    if data_shape[1] == 5:
      logging.info( 'Create dtValueSpace of scalar')    
      self.value_ = data[:,3:4]
      self.v_ = data[:,4:5]
      self.vDim_ = 1
    elif data_shape[1] == 7:
      logging.info( 'Create dtValueSpace of vector')
      self.value_ = data[:,3:6]
      self.v_ = data[:,6:7]
      self.vDim_ = 3
    else:
      raise ValueError('Bad shape of data array.')
  
  def Coord(self):
    return self.coord_

  def Value(self):
    return self.value_

  def V(self):
    return self.v_
 
  def IntV(self):
    return numpy.sum( self.v_ )
  
  def VDim(self):
    return self.vDim_