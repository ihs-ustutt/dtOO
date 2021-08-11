import numpy
import logging
from pyDtOO.dtFile import dtFile
import io
import os
import fnmatch

class dtDeveloping:
  def __init__(self, fn):
    self.f_ = dtFile(fn)
    
  def Log(self):
    logging.info( 'Create dtDeveloping > %s > %s', self.f_.DirName(), self.f_.FileName() )

  @staticmethod
  def parseSlice(sliceStr):
    return tuple(
      (slice(*(int(i) if i else None for i in part.strip().split(':'))) if ':' in part else int(part.strip())) for part in sliceStr.split(',')
    )

  def Read(self, pattern=None):
    data = []
    rare_data = {}
    for p in self.__GetFilepaths( self.f_.FullName() ):
      base = os.path.basename(p)
      
      # create default pattern
      if pattern == None:
        pattern = {base : ':,1:'}
      logging.info( 'Read data from %s', p )
      txt = io.open(p, mode='r', encoding='utf-8').read() 
      txt = io.StringIO(txt.replace('(', '').replace(')', ''))
      tmp = numpy.genfromtxt( txt, delimiter='', comments='#')
      
      # validate that pattern exist for every input file
      thisPattern = None
      thisFile    = ''
      for file in pattern:
        if fnmatch.fnmatch(base, file):
          thisPattern = pattern[file]
          thisFile = file
          break
      if thisPattern == None:
        raise ValueError('No Rule for file %s' % base)    

      if thisFile not in rare_data.keys():
        rare_data[thisFile] = tmp
      else:
        rare_data[thisFile] = numpy.append(rare_data[thisFile], tmp, axis=0)
         
    for aKey in pattern:
      thisPattern = pattern[aKey]
      if thisPattern != '':
        logging.info('Apply pattern > %s < to > %s <' % (thisPattern, aKey))
        if len(data)==0:
          data = rare_data[aKey][:, 0:1]  
        data = numpy.concatenate((data, rare_data[aKey][self.parseSlice(thisPattern)],), axis=1) 

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