import numpy
import logging
from pyDtOO.dtValueField import dtValueField

class dtAverageValueField(dtValueField):
  def __init__(self, data, nBinsOne, nBinsTwo, nBinsThree):
    dtValueField.__init__(self, data)
    binsOne = numpy.linspace(min(self.Coord()[:,0]), max(self.Coord()[:,0]), nBinsOne)
    binsTwo = numpy.linspace(min(self.Coord()[:,1]), max(self.Coord()[:,1]), nBinsTwo)    
    binsThree = numpy.linspace(min(self.Coord()[:,2]), max(self.Coord()[:,2]), nBinsThree)

    indsOne = numpy.digitize(self.Coord()[:,0], binsOne)
    indsTwo = numpy.digitize(self.Coord()[:,1], binsTwo)
    indsThree = numpy.digitize(self.Coord()[:,2], binsThree)

    self.valueAvQOne_ = numpy.zeros( (nBinsOne, self.VDim()) ) 
    self.valueAvQTwo_ = numpy.zeros( (nBinsTwo, self.VDim()) ) 
    self.valueAvQThree_ = numpy.zeros( (nBinsThree, self.VDim()) ) 
    self.valueAvAOne_ = numpy.zeros( (nBinsOne, self.VDim()) ) 
    self.valueAvATwo_ = numpy.zeros( (nBinsTwo, self.VDim()) ) 
    self.valueAvAThree_ = numpy.zeros( (nBinsThree, self.VDim()) ) 
    self.QOne_ = numpy.zeros( nBinsOne ) 
    self.QTwo_ = numpy.zeros( nBinsTwo ) 
    self.QThree_ = numpy.zeros( nBinsThree ) 
    self.AOne_ = numpy.zeros( nBinsOne ) 
    self.ATwo_ = numpy.zeros( nBinsTwo ) 
    self.AThree_ = numpy.zeros( nBinsThree )     
    self.coordOne_ = numpy.zeros( (nBinsOne, 3) ) 
    self.coordTwo_ = numpy.zeros( (nBinsTwo, 3) ) 
    self.coordThree_ = numpy.zeros( (nBinsThree, 3) ) 
    self.nPointsOne_ = numpy.zeros( nBinsOne ) 
    self.nPointsTwo_ = numpy.zeros( nBinsTwo ) 
    self.nPointsThree_ = numpy.zeros( nBinsThree ) 
    
    for i in range(len(indsOne)):
      self.nPointsOne_[ indsOne[i]-1 ] = self.nPointsOne_[ indsOne[i]-1 ] + 1
      self.nPointsTwo_[ indsTwo[i]-1 ] = self.nPointsTwo_[ indsTwo[i]-1 ] + 1
      self.nPointsThree_[ indsThree[i]-1 ] = self.nPointsThree_[ indsThree[i]-1 ] + 1
      
      self.coordOne_[ indsOne[i]-1,: ] = self.coordOne_[ indsOne[i]-1,: ] + self.Coord()[i,:]
      self.coordTwo_[ indsTwo[i]-1,: ] = self.coordTwo_[ indsTwo[i]-1,: ] + self.Coord()[i,:]
      self.coordThree_[ indsThree[i]-1,: ] = self.coordThree_[ indsThree[i]-1,: ] + self.Coord()[i,:]
      
      self.valueAvQOne_[ indsOne[i]-1,: ] = self.valueAvQOne_[ indsOne[i]-1,: ] + self.Value()[i,:] * self.Q()[i]
      self.valueAvAOne_[ indsOne[i]-1,: ] = self.valueAvAOne_[ indsOne[i]-1,: ] + self.Value()[i,:] * self.A()[i]
      self.valueAvQTwo_[ indsTwo[i]-1,: ] = self.valueAvQTwo_[ indsTwo[i]-1,: ] + self.Value()[i,:] * self.Q()[i]
      self.valueAvATwo_[ indsTwo[i]-1,: ] = self.valueAvATwo_[ indsTwo[i]-1,: ] + self.Value()[i,:] * self.A()[i]
      self.valueAvQThree_[ indsThree[i]-1,: ] = self.valueAvQThree_[ indsThree[i]-1,: ] + self.Value()[i,:] * self.Q()[i]
      self.valueAvAThree_[ indsThree[i]-1,: ] = self.valueAvAThree_[ indsThree[i]-1,: ] + self.Value()[i,:] * self.A()[i]
          
      self.QOne_[indsOne[i]-1] = self.QOne_[indsOne[i]-1] + self.Q()[i]
      self.AOne_[indsOne[i]-1] = self.AOne_[indsOne[i]-1] + self.A()[i]
      self.QTwo_[indsTwo[i]-1] = self.QTwo_[indsTwo[i]-1] + self.Q()[i]
      self.ATwo_[indsTwo[i]-1] = self.ATwo_[indsTwo[i]-1] + self.A()[i]
      self.QThree_[indsThree[i]-1] = self.QThree_[indsThree[i]-1] + self.Q()[i]
      self.AThree_[indsThree[i]-1] = self.AThree_[indsThree[i]-1] + self.A()[i]
      
    for i in range(nBinsOne):
      self.coordOne_[i,:] = self.coordOne_[i,:] / self.nPointsOne_[i]
      self.valueAvQOne_[i,:] = self.valueAvQOne_[i,:] / self.QOne_[i]
      self.valueAvAOne_[i,:] = self.valueAvAOne_[i,:] / self.AOne_[i]
      self.coordTwo_[i,:] = self.coordTwo_[i,:] / self.nPointsTwo_[i]
      self.valueAvQTwo_[i,:] = self.valueAvQTwo_[i,:] / self.QTwo_[i]
      self.valueAvATwo_[i,:] = self.valueAvATwo_[i,:] / self.ATwo_[i]
      self.coordThree_[i,:] = self.coordThree_[i,:] / self.nPointsThree_[i]
      self.valueAvQThree_[i,:] = self.valueAvQThree_[i,:] / self.QThree_[i]
      self.valueAvAThree_[i,:] = self.valueAvAThree_[i,:] / self.AThree_[i]      
  
  def ValueAvQOne(self):
    return self.valueAvQOne_

  def ValueAvAOne(self):
    return self.valueAvAOne_  
  
  def CoordOne(self):
    return self.coordOne_  
  
  def ValueAvQTwo(self):
    return self.valueAvQTwo_

  def ValueAvATwo(self):
    return self.valueAvATwo_  
  
  def CoordTwo(self):
    return self.coordTwo_    
  
  def ValueAvQThree(self):
    return self.valueAvQThree_

  def ValueAvAThree(self):
    return self.valueAvAThree_  
  
  def CoordThree(self):
    return self.coordThree_