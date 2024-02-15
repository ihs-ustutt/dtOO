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

class dtField:
  def __init__(self, fn):
    self.f_ = dtFile(fn)
    
  def Log(self):
    logging.info( 'Create dtField > %s > %s', self.f_.DirName(), self.f_.FileName() )

  def Read(self):
    data = numpy.genfromtxt( self.f_.FullName(), delimiter=',', comments='#')
    if numpy.size( numpy.shape( data ) )==1 and numpy.size( data )>0 :
      data = data.reshape( (1,numpy.size(data)) ) 
    logging.info( 'Read data with %s', str(numpy.shape(data)) )      
    return data
