import numpy
import logging
from pyDtOO.dtFile import dtFile
import cStringIO
import os

class dtDeveloping:
  def __init__(self, fn):
    self.f_ = dtFile(fn)
    
  def Log(self):
    logging.info( 'Create dtDeveloping > %s > %s', self.f_.DirName(), self.f_.FileName() )

  def Read(self):
    data = []
    for p in self.__GetFilepaths( self.f_.FullName() ):
      logging.info( 'Read data from %s', p )
      txt = open(p, 'r').read() 
      txt = cStringIO.StringIO(txt.replace('(', '').replace(')', ''))
      tmp = numpy.genfromtxt( txt, delimiter='', comments='#')
      if len(data)==0:
        data = tmp
      else:
        data = numpy.append(data, tmp, axis=0)
  
    timeSort = numpy.argsort( data[:,0] )

    return data[timeSort, :]
  
  def __GetFilepaths(self, directory):
    file_paths = []  # List which will store all of the full filepaths.

    # Walk the tree.
    for root, directories, files in os.walk(directory):
      for filename in files:
        # Join the two strings in order to form the full filepath.
        filepath = os.path.join(root, filename)
        file_paths.append(filepath)  # Add it to the list.

    return file_paths  # Self-explanatory.