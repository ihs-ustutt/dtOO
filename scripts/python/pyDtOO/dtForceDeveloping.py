import numpy
import logging

class dtForceDeveloping:
  def __init__(self, data):
    if numpy.shape(data)[1]!=13:
      raise ValueError('Bad shape of data array.')    
    self.time_ = data[:,0]
    self.forcePressure_ = data[:,1:4]
    self.forceViscous_ = data[:,4:7]
    self.momentPressure_ = data[:,7:10]
    self.momentViscous_ = data[:,10:13]
    
  def LastTime(self):
    return max( self.time_[:] )

  def Moment(self):
    return self.momentPressure_ + self.momentViscous_
  
  def MomentMeanLast(self, nAverageSteps):
    return numpy.mean( 
      self.momentPressure_[-nAverageSteps:,:] + self.momentViscous_[-nAverageSteps:,:], 
      axis=0 
    )
    
  def MomentMinLast(self, nAverageSteps):
    return numpy.min( 
      self.momentPressure_[-nAverageSteps:,:] + self.momentViscous_[-nAverageSteps:,:], 
      axis=0 
    )

  def MomentMaxLast(self, nAverageSteps):
    return numpy.max( 
      self.momentPressure_[-nAverageSteps:,:] + self.momentViscous_[-nAverageSteps:,:], 
      axis=0 
    )    

  def MomentMean(self, step, nAverageSteps):
    return numpy.mean( 
      self.momentPressure_[step-nAverageSteps:step,:] + self.momentViscous_[step-nAverageSteps:step,:], 
      axis=0 
    )
    
  def MomentChangeLast(self, nAverageSteps ):
    return (self.MomentMaxLast(nAverageSteps) - self.MomentMinLast(nAverageSteps))/self.MomentMeanLast(nAverageSteps)
  
  def Force(self):
    return self.forcePressure_ + self.forceViscous_
  
  def ForceMeanLast(self, nAverageSteps):
    return numpy.mean( 
      self.forcePressure_[-nAverageSteps:,:] + self.forceViscous_[-nAverageSteps:,:], 
      axis=0 
    )
    
  def ForceMinLast(self, nAverageSteps):
    return numpy.min( 
      self.forcePressure_[-nAverageSteps:,:] + self.forceViscous_[-nAverageSteps:,:], 
      axis=0 
    )

  def ForceMaxLast(self, nAverageSteps):
    return numpy.max( 
      self.forcePressure_[-nAverageSteps:,:] + self.forceViscous_[-nAverageSteps:,:], 
      axis=0 
    )    

  def ForceMean(self, step, nAverageSteps):
    return numpy.mean( 
      self.forcePressure_[step-nAverageSteps:step,:] + self.forceViscous_[step-nAverageSteps:step,:], 
      axis=0 
    )
    
  def ForceChangeLast(self, nAverageSteps ):
    return (self.ForceMaxLast(nAverageSteps) - self.ForceMinLast(nAverageSteps))/self.ForceMeanLast(nAverageSteps)
 