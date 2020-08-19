import numpy
import logging
import io
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
    '[Data]\nR [ m ], Turbulence Eddy Dissipation [ m^2 s^-3 ]',
    'r_p'
    :
    '[Data]\nR [ m ], Pressure [ Pa ]'
  }
  def __init__(self, data, nBinsOne, nBinsTwo, nBinsThree):
    dtValueField.__init__(self, data)
    #
    # determine characteristic length by bounding box
    #
    bbMin = numpy.zeros(3, float)
    bbMax = numpy.zeros(3, float)
    for i in range(3):
      bbMin[i] = min( self.Coord()[:, i] )
      bbMax[i] = max( self.Coord()[:, i] )
    
    cDist = numpy.linalg.norm(bbMax-bbMin)
    
    self.nBinsOne_ = nBinsOne
    self.nBinsTwo_ = nBinsTwo
    self.nBinsThree_ = nBinsThree
    
    #
    # binOne
    #
    orderingOne = numpy.argsort( self.Coord()[:,0] )
    perBinOne = int(self.NumValues() / nBinsOne)
    self.binsOne = numpy.zeros(nBinsOne+1)
    for i in range(nBinsOne):
      self.binsOne[ i ] = self.Coord()[ orderingOne[ i * (perBinOne) ], 0 ]
    self.binsOne[nBinsOne] = max(self.Coord()[:,0])
    self.binsOne[0] = self.binsOne[0] - 0.01*cDist
    self.binsOne[-1] = self.binsOne[-1] + 0.01*cDist

    #
    # binTwo
    #
    orderingTwo = numpy.argsort( self.Coord()[:,1] )
    perBinTwo = int(self.NumValues() / nBinsTwo)
    self.binsTwo = numpy.zeros(nBinsTwo+1)
    for i in range(nBinsTwo):
      self.binsTwo[ i ] = self.Coord()[ orderingTwo[ i * (perBinTwo) ], 1 ]
    self.binsTwo[nBinsTwo] = max(self.Coord()[:,1])
    self.binsTwo[0] = self.binsTwo[0] - 0.01*cDist
    self.binsTwo[-1] = self.binsTwo[-1] + 0.01*cDist

    #
    # binThree
    #
    orderingThree = numpy.argsort( self.Coord()[:,2] )
    perBinThree = int(self.NumValues() / nBinsThree)
    self.binsThree = numpy.zeros(nBinsThree+1)
    for i in range(nBinsThree):
      self.binsThree[ i ] = self.Coord()[ orderingThree[ i * (perBinThree) ], 2 ]
    self.binsThree[nBinsThree] = max(self.Coord()[:,2])    
    self.binsThree[0] = self.binsThree[0] - 0.01*cDist
    self.binsThree[-1] = self.binsThree[-1] + 0.01*cDist

    self.indsOne = numpy.digitize(self.Coord()[:,0], self.binsOne)
    self.indsTwo = numpy.digitize(self.Coord()[:,1], self.binsTwo)
    self.indsThree = numpy.digitize(self.Coord()[:,2], self.binsThree)

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
    
    for i in range(len(self.indsOne)):
      self.nPointsOne_[ self.indsOne[i]-1 ] = self.nPointsOne_[ self.indsOne[i]-1 ] + 1      
      self.coordOne_[ self.indsOne[i]-1,: ] = self.coordOne_[ self.indsOne[i]-1,: ] + self.Coord()[i,:]
      self.valueAvQOne_[ self.indsOne[i]-1,: ] = self.valueAvQOne_[ self.indsOne[i]-1,: ] + self.Value()[i,:] * self.Q()[i]
      self.valueAvAOne_[ self.indsOne[i]-1,: ] = self.valueAvAOne_[ self.indsOne[i]-1,: ] + self.Value()[i,:] * self.A()[i]
      self.QOne_[self.indsOne[i]-1] = self.QOne_[self.indsOne[i]-1] + self.Q()[i]
      self.AOne_[self.indsOne[i]-1] = self.AOne_[self.indsOne[i]-1] + self.A()[i]
    
    for i in range(len(self.indsTwo)):      
      self.nPointsTwo_[ self.indsTwo[i]-1 ] = self.nPointsTwo_[ self.indsTwo[i]-1 ] + 1
      self.coordTwo_[ self.indsTwo[i]-1,: ] = self.coordTwo_[ self.indsTwo[i]-1,: ] + self.Coord()[i,:]
      self.valueAvQTwo_[ self.indsTwo[i]-1,: ] = self.valueAvQTwo_[ self.indsTwo[i]-1,: ] + self.Value()[i,:] * self.Q()[i]
      self.valueAvATwo_[ self.indsTwo[i]-1,: ] = self.valueAvATwo_[ self.indsTwo[i]-1,: ] + self.Value()[i,:] * self.A()[i]
      self.QTwo_[self.indsTwo[i]-1] = self.QTwo_[self.indsTwo[i]-1] + self.Q()[i]
      self.ATwo_[self.indsTwo[i]-1] = self.ATwo_[self.indsTwo[i]-1] + self.A()[i]
      
    for i in range(len(self.indsThree)):      
      self.nPointsThree_[ self.indsThree[i]-1 ] = self.nPointsThree_[ self.indsThree[i]-1 ] + 1      
      self.coordThree_[ self.indsThree[i]-1,: ] = self.coordThree_[ self.indsThree[i]-1,: ] + self.Coord()[i,:]
      self.valueAvQThree_[ self.indsThree[i]-1,: ] = self.valueAvQThree_[ self.indsThree[i]-1,: ] + self.Value()[i,:] * self.Q()[i]
      self.valueAvAThree_[ self.indsThree[i]-1,: ] = self.valueAvAThree_[ self.indsThree[i]-1,: ] + self.Value()[i,:] * self.A()[i]
      self.QThree_[self.indsThree[i]-1] = self.QThree_[self.indsThree[i]-1] + self.Q()[i]
      self.AThree_[self.indsThree[i]-1] = self.AThree_[self.indsThree[i]-1] + self.A()[i]
      
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
  
  def InterValueAvQOne( self, interCoord ):
    interValueAvQ = numpy.zeros((numpy.size(interCoord),3),float)
    for i in range(3):
      interValueAvQ[:,i] = numpy.interp( 
        interCoord, 
        self.coordOne_[:,0], self.valueAvQOne_[:,i] 
      )
    return interValueAvQ

  def InterValueAvQTwo( self, interCoord ):
    interValueAvQ = numpy.zeros((numpy.size(interCoord),3),float)
    for i in range(3):
      interValueAvQ[:,i] = numpy.interp( 
        interCoord, 
        self.coordTwo_[:,1], self.valueAvQTwo_[:,i] 
      )
    return interValueAvQ
  
  def InterValueAvQThree( self, interCoord ):
    interValueAvQ = numpy.zeros((numpy.size(interCoord),3),float)
    for i in range(3):
      interValueAvQ[:,i] = numpy.interp( 
        interCoord, 
        self.coordThree_[:,2], self.valueAvQThree_[:,i] 
      )
    return interValueAvQ
  
  def IntValueAOne(self):
    ret = numpy.zeros( (self.nBinsOne_, self.vDim_), float )
    for i in range(0, self.vDim_):
      for j in range( len(self.indsOne) ):
        ret[self.indsOne[j]-1,i] \
        = \
        ret[self.indsOne[j]-1, i] \
        + \
        self.value_[j,i] * numpy.linalg.norm( self.sf_[j,:] )      
    return ret

  def IntValueATwo(self):
    ret = numpy.zeros( (self.nBinsTwo_, self.vDim_), float )
    for i in range(0, self.vDim_):
      for j in range( len(self.indsTwo) ):
        ret[self.indsTwo[j]-1,i] \
        = \
        ret[self.indsTwo[j]-1, i] \
        + \
        self.value_[j,i] * numpy.linalg.norm( self.sf_[j,:] )      
    return ret

  def IntValueAThree(self):
    ret = numpy.zeros( (self.nBinsThree_, self.vDim_), float )
    for i in range(0, self.vDim_):
      for j in range( len(self.indsThree) ):
        ret[self.indsThree[j]-1,i] \
        = \
        ret[self.indsThree[j]-1, i] \
        + \
        self.value_[j,i] * numpy.linalg.norm( self.sf_[j,:] )      
    return ret

  def SIntValueOne(self):
    ret = numpy.zeros( (self.nBinsOne_, self.vDim_, 3), float )
    for j in range(self.nBinsOne_):
      for i in range(0, self.vDim_):
        ret[j, i,:] = numpy.sum( 
          self.value_[self.indsOne==j+1,i:i+1] * self.sf_[self.indsOne==j+1,:], 
          axis=0 
        )
    return ret

  def SIntValueTwo(self):
    ret = numpy.zeros( (self.nBinsTwo_, self.vDim_, 3), float )
    for j in range(self.nBinsTwo_):
      for i in range(0, self.vDim_):
        ret[j, i,:] = numpy.sum( 
          self.value_[self.indsTwo==j+1,i:i+1] * self.sf_[self.indsTwo==j+1,:], 
          axis=0 
        )
    return ret

  def SIntValueThree(self):
    ret = numpy.zeros( (self.nBinsThree_, self.vDim_, 3), float )
    for j in range(self.nBinsThree_):
      for i in range(0, self.vDim_):
        ret[j, i,:] = numpy.sum( 
          self.value_[self.indsThree==j+1,i:i+1] * self.sf_[self.indsThree==j+1,:], 
          axis=0 
        )
    return ret

  def WriteProfile1DFixedValueCSV(self, prefix, header):
    dtAverageValueField.WriteOneProfile1DFixedValueCSV( 
      prefix+'_Q1', header, self.CoordOne(), self.ValueAvQOne() 
    )
    dtAverageValueField.WriteOneProfile1DFixedValueCSV( 
      prefix+'_Q2', header, self.CoordTwo(), self.ValueAvQTwo() 
    )    
    dtAverageValueField.WriteOneProfile1DFixedValueCSV( 
      prefix+'_Q3', header, self.CoordThree(), self.ValueAvQThree() 
    )    
      
  @staticmethod    
  def WriteOneProfile1DFixedValueCSV(prefix, header, coord, value):
    fileOut = io.open(prefix+'.csv', mode='w', encoding='utf-8')
    fileOut.write( "%s\n" % (dtAverageValueField.HEADER_[header]) )
    ij = numpy.shape( value )
    
    if ij[1] == 3:
      fileOut.write(
        "%16.8e, %16.8e, %16.8e, %16.8e\n" 
        % 
        (
          0.9 * coord[0,0], value[0,0], value[0,1], value[0,2]
        ) 
      )     
      for i in range( ij[0] ):
        fileOut.write(
          "%16.8e, %16.8e, %16.8e, %16.8e\n" 
          % 
          (
            coord[i,0], value[i,0], value[i,1], value[i,2]
          ) 
        )         
      fileOut.write(
        "%16.8e, %16.8e, %16.8e, %16.8e\n" 
        % 
        (
          1.1 * coord[-1,0], value[-1,0], value[-1,1], value[-1,2]
        ) 
      )  
    else:
      fileOut.write(
        "%16.8e, %16.8e\n" 
        % 
        (
          0.9 * coord[0,0], value[0,0]
        ) 
      )
      
      for i in range( ij[0] ):
        fileOut.write(
          "%16.8e, %16.8e\n" 
          % 
          (
            0.9 * coord[i,0], value[i,0]
          ) 
        )

      fileOut.write(
        "%16.8e, %16.8e\n" 
        % 
        (
          1.1 * coord[-1,0], value[-1,0]
        ) 
      ) 
