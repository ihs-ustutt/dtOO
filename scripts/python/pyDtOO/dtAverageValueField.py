import numpy
import logging
from pyDtOO.dtValueField import dtValueField

class dtAverageValueField(dtValueField):
  HEADER_ = {
    'r_ur_uphi_uz' 
    : 
    '[Data]\nR [ m ], Velocity Radial [ m s^-1 ], '
                     'Velocity Circumferential [ m s^-1 ], '
                     'Velocity Axial [ m s^-1 ]',
    'r_k'
    :
    '[Data]\nR [ m ], Turbulence Kinetic Energy [ m^2 s^-2 ]',
    'r_epsilon'
    :
    '[Data]\nR [ m ], Turbulence Eddy Dissipation [ m^2 s^-3 ]'
  }
  def __init__(self, data, nBinsOne, nBinsTwo, nBinsThree):
    dtValueField.__init__(self, data)
    
    #
    # binOne
    #
    orderingOne = numpy.argsort( self.Coord()[:,0] )
    perBinOne = self.NumValues() / nBinsOne
    binsOne = numpy.zeros(nBinsOne)
    for i in range(nBinsOne):
      binsOne[ i ] = self.Coord()[ orderingOne[ i * (perBinOne-1) ], 0 ]
    binsOne[nBinsOne-1] = max(self.Coord()[:,0])

    #
    # binTwo
    #
    orderingTwo = numpy.argsort( self.Coord()[:,1] )
    perBinTwo = self.NumValues() / nBinsTwo
    binsTwo = numpy.zeros(nBinsTwo)
    for i in range(nBinsTwo):
      binsTwo[ i ] = self.Coord()[ orderingTwo[ i * (perBinTwo-1) ], 1 ]
    binsTwo[nBinsTwo-1] = max(self.Coord()[:,1])

    #
    # binThree
    #
    orderingThree = numpy.argsort( self.Coord()[:,2] )
    perBinThree = self.NumValues() / nBinsThree
    binsThree = numpy.zeros(nBinsThree)
    for i in range(nBinsThree):
      binsThree[ i ] = self.Coord()[ orderingThree[ i * (perBinThree-1) ], 2 ]
    binsThree[nBinsThree-1] = max(self.Coord()[:,2])    
    
#    binsTwo = numpy.linspace(min(self.Coord()[:,1]), max(self.Coord()[:,1]), nBinsTwo)    
#    binsThree = numpy.linspace(min(self.Coord()[:,2]), max(self.Coord()[:,2]), nBinsThree)

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
      self.coordOne_[ indsOne[i]-1,: ] = self.coordOne_[ indsOne[i]-1,: ] + self.Coord()[i,:]
      self.valueAvQOne_[ indsOne[i]-1,: ] = self.valueAvQOne_[ indsOne[i]-1,: ] + self.Value()[i,:] * self.Q()[i]
      self.valueAvAOne_[ indsOne[i]-1,: ] = self.valueAvAOne_[ indsOne[i]-1,: ] + self.Value()[i,:] * self.A()[i]
      self.QOne_[indsOne[i]-1] = self.QOne_[indsOne[i]-1] + self.Q()[i]
      self.AOne_[indsOne[i]-1] = self.AOne_[indsOne[i]-1] + self.A()[i]
    
    for i in range(len(indsTwo)):      
      self.nPointsTwo_[ indsTwo[i]-1 ] = self.nPointsTwo_[ indsTwo[i]-1 ] + 1
      self.coordTwo_[ indsTwo[i]-1,: ] = self.coordTwo_[ indsTwo[i]-1,: ] + self.Coord()[i,:]
      self.valueAvQTwo_[ indsTwo[i]-1,: ] = self.valueAvQTwo_[ indsTwo[i]-1,: ] + self.Value()[i,:] * self.Q()[i]
      self.valueAvATwo_[ indsTwo[i]-1,: ] = self.valueAvATwo_[ indsTwo[i]-1,: ] + self.Value()[i,:] * self.A()[i]
      self.QTwo_[indsTwo[i]-1] = self.QTwo_[indsTwo[i]-1] + self.Q()[i]
      self.ATwo_[indsTwo[i]-1] = self.ATwo_[indsTwo[i]-1] + self.A()[i]
      
    for i in range(len(indsThree)):      
      self.nPointsThree_[ indsThree[i]-1 ] = self.nPointsThree_[ indsThree[i]-1 ] + 1      
      self.coordThree_[ indsThree[i]-1,: ] = self.coordThree_[ indsThree[i]-1,: ] + self.Coord()[i,:]
      self.valueAvQThree_[ indsThree[i]-1,: ] = self.valueAvQThree_[ indsThree[i]-1,: ] + self.Value()[i,:] * self.Q()[i]
      self.valueAvAThree_[ indsThree[i]-1,: ] = self.valueAvAThree_[ indsThree[i]-1,: ] + self.Value()[i,:] * self.A()[i]
      self.QThree_[indsThree[i]-1] = self.QThree_[indsThree[i]-1] + self.Q()[i]
      self.AThree_[indsThree[i]-1] = self.AThree_[indsThree[i]-1] + self.A()[i]
      
    for i in range(nBinsOne):
      self.coordOne_[i,:] = self.coordOne_[i,:] / self.nPointsOne_[i]
      self.valueAvQOne_[i,:] = self.valueAvQOne_[i,:] / self.QOne_[i]
      self.valueAvAOne_[i,:] = self.valueAvAOne_[i,:] / self.AOne_[i]
    for i in range(nBinsTwo):      
      self.coordTwo_[i,:] = self.coordTwo_[i,:] / self.nPointsTwo_[i]
      self.valueAvQTwo_[i,:] = self.valueAvQTwo_[i,:] / self.QTwo_[i]
      self.valueAvATwo_[i,:] = self.valueAvATwo_[i,:] / self.ATwo_[i]
    for i in range(nBinsThree):      
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
  
  def WriteProfile1DFixedValueCSV(self, prefix, header):
    fileOut1 = open(prefix+'_Q1.csv', 'w')
    fileOut1.write( "%s\n" % (dtAverageValueField.HEADER_[header]) )
    fileOut2 = open(prefix+'_Q2.csv', 'w')
    fileOut2.write( "%s\n" % (dtAverageValueField.HEADER_[header]) )
    fileOut3 = open(prefix+'_Q3.csv', 'w')
    fileOut3.write( "%s\n" % (dtAverageValueField.HEADER_[header]) )
    if self.vDim_ == 3:
      fileOut1.write(
        "%16.8e, %16.8e, %16.8e, %16.8e\n" 
        % 
        (
          0.9 * self.CoordOne()[0,0],
          self.ValueAvQOne()[0,0],
          self.ValueAvQOne()[0,1],
          self.ValueAvQOne()[0,2]
        ) 
      )
      fileOut2.write(
        "%16.8e, %16.8e, %16.8e, %16.8e\n" 
        % 
        (
          0.9 * self.CoordTwo()[0,0],
          self.ValueAvQTwo()[0,0],
          self.ValueAvQTwo()[0,1],
          self.ValueAvQTwo()[0,2]
        ) 
      )
      fileOut3.write(
        "%16.8e, %16.8e, %16.8e, %16.8e\n" 
        % 
        (
          0.9 * self.CoordThree()[0,0],
          self.ValueAvQThree()[0,0],
          self.ValueAvQThree()[0,1],
          self.ValueAvQThree()[0,2]
        ) 
      )
      
      for i in range( numpy.shape(self.ValueAvQOne())[0] ):
        fileOut1.write(
          "%16.8e, %16.8e, %16.8e, %16.8e\n" 
          % 
          (
            self.CoordOne()[i,0],
            self.ValueAvQOne()[i,0],
            self.ValueAvQOne()[i,1],
            self.ValueAvQOne()[i,2]
          ) 
        )  
        fileOut2.write(
          "%16.8e, %16.8e, %16.8e, %16.8e\n" 
          % 
          (
            self.CoordTwo()[i,0],
            self.ValueAvQTwo()[i,0],
            self.ValueAvQTwo()[i,1],
            self.ValueAvQTwo()[i,2]
          ) 
        )  
        fileOut3.write(
          "%16.8e, %16.8e, %16.8e, %16.8e\n" 
          % 
          (
            self.CoordThree()[i,0],
            self.ValueAvQThree()[i,0],
            self.ValueAvQThree()[i,1],
            self.ValueAvQThree()[i,2]
          ) 
        )  
        
      fileOut1.write(
        "%16.8e, %16.8e, %16.8e, %16.8e\n" 
        % 
        (
          1.1 * self.CoordOne()[-1,0],
          self.ValueAvQOne()[-1,0],
          self.ValueAvQOne()[-1,1],
          self.ValueAvQOne()[-1,2]
        ) 
      )  
      fileOut2.write(
        "%16.8e, %16.8e, %16.8e, %16.8e\n" 
        % 
        (
          1.1 * self.CoordTwo()[-1,0],
          self.ValueAvQTwo()[-1,0],
          self.ValueAvQTwo()[-1,1],
          self.ValueAvQTwo()[-1,2]
        ) 
      )  
      fileOut3.write(
        "%16.8e, %16.8e, %16.8e, %16.8e\n" 
        % 
        (
          1.1 * self.CoordThree()[-1,0],
          self.ValueAvQThree()[-1,0],
          self.ValueAvQThree()[-1,1],
          self.ValueAvQThree()[-1,2]
        ) 
      )
    else:
      fileOut1.write(
        "%16.8e, %16.8e\n" 
        % 
        (
          0.9 * self.CoordOne()[0,0], self.ValueAvQOne()[0,0]
        ) 
      )
      fileOut2.write(
        "%16.8e, %16.8e\n" 
        % 
        (
          0.9 * self.CoordTwo()[0,0], self.ValueAvQTwo()[0,0]
        ) 
      )
      fileOut3.write(
        "%16.8e, %16.8e\n" 
        % 
        (
          0.9 * self.CoordThree()[0,0], self.ValueAvQThree()[0,0]
        )
      )
      
      for i in range( numpy.shape(self.ValueAvQOne())[0] ):
        fileOut1.write(
          "%16.8e, %16.8e\n" 
          % 
          (
            self.CoordOne()[i,0], self.ValueAvQOne()[i,0]
          ) 
        )  
        fileOut2.write(
          "%16.8e, %16.8e\n" 
          % 
          (
            self.CoordTwo()[i,0], self.ValueAvQTwo()[i,0]
          ) 
        )  
        fileOut3.write(
          "%16.8e, %16.8e\n" 
          % 
          (
            self.CoordThree()[i,0], self.ValueAvQThree()[i,0]
          ) 
        )  
      fileOut1.write(
        "%16.8e, %16.8e\n" 
        % 
        (
          1.1 * self.CoordOne()[-1,0], self.ValueAvQOne()[-1,0]
        ) 
      )  
      fileOut2.write(
        "%16.8e, %16.8e\n" 
        % 
        (
          1.1 * self.CoordTwo()[-1,0], self.ValueAvQTwo()[-1,0]
        ) 
      )  
      fileOut3.write(
        "%16.8e, %16.8e\n" 
        % 
        (
          1.1 * self.CoordThree()[-1,0], self.ValueAvQThree()[-1,0]
        ) 
      )