import numpy
import logging

class dtScalarDeveloping:
  def __init__(self, data):
    if numpy.shape(data)[1]!=2:
      raise ValueError('Bad shape of data array.')    
    self.time_ = data[:,0]
    self.scalar_ = data[:,1]
    
  def LastTime(self):
    return max( self.time_[:] )
  
  def MeanLast(self, nAverageSteps):
    return numpy.mean( self.scalar_[-nAverageSteps:] )
  
  def MinLast(self, nAverageSteps):
    return min( self.scalar_[-nAverageSteps:] )

  def MaxLast(self, nAverageSteps):
    return max( self.scalar_[-nAverageSteps:] )  
  
  def ChangeLast(self, nAverageSteps ):
    return (self.MaxLast(nAverageSteps) - self.MinLast(nAverageSteps))/self.MeanLast(nAverageSteps)
  def __add__(self, other):
    if not numpy.array_equal(self.time_,other.time_):
      raise ValueError('Times are not equal.')
    
    data = numpy.zeros( (numpy.shape(self.time_)[0],2) )
    data[:,0] = self.time_
    data[:,1] = self.scalar_ + other.scalar_
    
    return dtScalarDeveloping(data)
  
  def __sub__(self, other):
    if not numpy.array_equal(self.time_,other.time_):
      raise ValueError('Times are not equal.')
    
    data = numpy.zeros( (numpy.shape(self.time_)[0],2) )
    data[:,0] = self.time_
    data[:,1] = self.scalar_ - other.scalar_
    
    return dtScalarDeveloping(data)  