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