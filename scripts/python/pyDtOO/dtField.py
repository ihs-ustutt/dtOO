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
    logging.info( 'Read data with %s', str(numpy.shape(data)) )
    return data