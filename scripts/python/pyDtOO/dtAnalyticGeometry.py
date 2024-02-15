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
from pyDtOO.dtFile import dtFile

class dtAnalyticGeometry(dtFile):
  def __init__(self, fn):
    dtFile.__init__(self, fn)
    self.dim_ = 0
    self.nP_ = 0
    self.data_ = 0
    # read header   
    self.ReadHeader()
    
  def Log(self):
    logging.info( 
      'Create dtAnalyticGeometry > %s > %s', self.DirName(), self.FileName() 
    )

  def Read(self):
    self.data_ = numpy.genfromtxt( self.FullName(), comments='#')
    logging.info( 'Read data with %s', str(numpy.shape(self.data_)) )
  
  def ReadHeader(self):
    f = open( self.FullName(), mode='r', encoding='utf-8' )
    # first line contains dimension
    dim = f.readline()
    # second line contains number of points
    np = f.readline()
    f.close()
    
    self.dim_ = int( dim.replace('# dim','') )
    self.nP_ = numpy.fromstring( np.replace('# nP',''), int, count=3, sep=" ")

  def Dim(self):
    return self.dim_
  
  def NumPoints(self, dir):
    return self.nP_[dir]  
  
  def Point(self, ii, jj=0, kk=0):
    return self.data_[ ii + self.nP_[0] * (jj + self.nP_[1] * kk) ]

  def LineI(self, jj=0, kk=0):
    rD = numpy.zeros( (self.nP_[0], 3), float )
    for ii in range(self.nP_[0]):
      rD[ii,:] = self.data_[ ii + self.nP_[0] * (jj + self.nP_[1] * kk) ]  
    return rD
