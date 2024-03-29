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

import numpy
import logging

class dtValueField:
  def __init__(self, data):
    self.data_ = data
    data_shape = numpy.shape(data)

    self.coord_ = 0
    self.value_ = 0
    self.q_ = 0
    self.sf_ = 0
    self.vDim_ = 0
    
    if data_shape[0] == 0:
      return
    
    self.coord_ = data[:,0:3]
    self.nValues_ = data_shape[0]
    
    if data_shape[1] == 8:
      logging.info( 'Create dtValueField of scalar')    
      self.value_ = data[:,3:4]
      self.sf_ = data[:,4:7]
      self.q_ = data[:,7:8]
      self.vDim_ = 1
    elif data_shape[1] == 10:
      logging.info( 'Create dtValueField of vector')
      self.value_ = data[:,3:6]
      self.sf_ = data[:,6:9]
      self.q_ = data[:,9:10]      
      self.vDim_ = 3
    else:
      raise ValueError('Bad shape of data array.')
  
    self.a_ = numpy.zeros( numpy.shape(self.sf_)[0], float )
    self.n_ = numpy.zeros( numpy.shape(self.sf_), float )
    for i in range( numpy.shape(self.sf_)[0] ):
      self.a_[i] = numpy.linalg.norm( self.sf_[i,:] )
      self.n_[i,:] = self.sf_[i,:] / self.a_[i]

  def Coord(self):
    return self.coord_

  def Value(self):
    return self.value_

  def Q(self):
    return self.q_

  def A(self):
    return self.a_
  
  def IntQ(self):
    return numpy.sum( self.q_ )

  def IntA(self):
    return numpy.sum( self.a_ )
  
  def IntValueQ(self):
    ret = numpy.zeros( self.vDim_ )
    logging.info('vDim = %d', self.vDim_)
    for i in range(0, self.vDim_):
      ret[i] = numpy.dot( self.value_[:,i], self.q_[:] )
    return ret

  def IntMagQ(self):
    return numpy.dot( 
      numpy.sqrt(numpy.sum( self.value_[:,:]**2., axis=1 )), self.q_[:] 
    )

  def IntMagSquareQ(self):
    return numpy.dot( numpy.sum( self.value_[:,:]**2., axis=1 ), self.q_[:] )
  
  def IntValueA(self):
    ret = numpy.zeros( self.vDim_ )
    logging.info('vDim = %d', self.vDim_)
    for i in range(0, self.vDim_):
      for j in range( len(self.q_) ):
        ret[i] = ret[i] + self.value_[j,i] * numpy.linalg.norm( self.sf_[j,:] )      
    return ret
    
  def SIntValueA(self):
    ret = numpy.zeros( (self.vDim_, 3) )
    logging.info('vDim = %d', self.vDim_)
    for i in range(0, self.vDim_):
      ret[i,:] = numpy.sum( self.value_[:,i:i+1] * self.sf_, axis=0 )
    return ret
  
  def VDim(self):
    return self.vDim_
  
  def NumValues(self):
    return self.nValues_
