from oslo_concurrency import lockutils
import glob
import os
import tempfile
import numpy as np
import logging
import io
import json
import ast
from pyDtOO.dtGod import dtGod

class dtClusteredSingletonState:
  PREFIX = 'A1'
  CASE = ''
  LOCKDIR = dtGod().LockPath().FullName()
  PIDDIR = './runPid'
  DATADIR = './runData'
  NPROC = 1
  SIMSH = ''
  ADDDATA = []
  ADDDATADEF = []
  PROB = None

  @lockutils.synchronized('fileIO', external=True)
  def __init__(self, id=-1, defObj=None, defFit=None):
    #
    # get id from state
    #
    if isinstance(id, str):
      id = int( id.replace( dtClusteredSingletonState.PREFIX+'_', '') )

    #
    # create new id
    #
    if id < 0:
      #
      # create files
      #
      if not os.path.isdir( dtClusteredSingletonState.DATADIR ) or not os.path.isfile( dtClusteredSingletonState.DATADIR+'/id.0' ):
        if not os.path.isdir( dtClusteredSingletonState.DATADIR ):
          os.mkdir(dtClusteredSingletonState.DATADIR)
        io.open(dtClusteredSingletonState.DATADIR+'/id.0', mode='w', encoding='utf-8').close()
        io.open(dtClusteredSingletonState.DATADIR+'/state.0', mode='w', encoding='utf-8').close()
        io.open(dtClusteredSingletonState.DATADIR+'/objective.0', mode='w', encoding='utf-8').close()
        io.open(dtClusteredSingletonState.DATADIR+'/fitness.0', mode='w', encoding='utf-8').close()
        for i in range( np.size(dtClusteredSingletonState.ADDDATA) ):
          io.open(
            dtClusteredSingletonState.DATADIR+'/'+dtClusteredSingletonState.ADDDATA[i]+'.0', mode='w', encoding='utf-8'
          ).close()
        self.cur_id = 1
#      else:
      #
      # write ti files
      #
      lastFileIndex = -1
      for aFile in glob.glob(dtClusteredSingletonState.DATADIR+'/id.*'):
        thisFileIndex = int(
          aFile.replace(
            dtClusteredSingletonState.DATADIR+'/id.',
            ''
          )
        )
        if lastFileIndex < thisFileIndex:
          lastFileIndex = thisFileIndex
      lastId = -1
      lastId = sum(
        1 for line in io.open(dtClusteredSingletonState.DATADIR+'/id.'+str(lastFileIndex), encoding='utf-8')
      )
      self.cur_id = lastId + lastFileIndex*1000 + 1

      if ( self.cur_id > (lastFileIndex+1)*1000 ):
        lastFileIndex = lastFileIndex + 1
        io.open(dtClusteredSingletonState.DATADIR+'/id.'+str(lastFileIndex), mode='w', encoding='utf-8').close()
        io.open(dtClusteredSingletonState.DATADIR+'/state.'+str(lastFileIndex), mode='w', encoding='utf-8').close()
        io.open(dtClusteredSingletonState.DATADIR+'/objective.'+str(lastFileIndex), mode='w', encoding='utf-8').close()
        io.open(dtClusteredSingletonState.DATADIR+'/fitness.'+str(lastFileIndex), mode='w', encoding='utf-8').close()

      io.open(dtClusteredSingletonState.DATADIR+'/id.'+str(lastFileIndex), mode='a', encoding='utf-8').write( str(self.cur_id)+'\n' )
      io.open(dtClusteredSingletonState.DATADIR+'/state.'+str(lastFileIndex), mode='a', encoding='utf-8').write( dtClusteredSingletonState.PREFIX+'_'+str(self.cur_id)+'\n' )
      if defObj is not None:
        io.open(dtClusteredSingletonState.DATADIR+'/objective.'+str(lastFileIndex), mode='a', encoding='utf-8').write( self.formatToWrite(defObj)+'\n' )
      else:
        io.open(dtClusteredSingletonState.DATADIR+'/objective.'+str(lastFileIndex), mode='a', encoding='utf-8').write( '__empty__\n' )
      if defFit is not None:
        io.open(dtClusteredSingletonState.DATADIR+'/fitness.'+str(lastFileIndex), mode='a', encoding='utf-8').write( self.formatToWrite(defFit)+'\n'  )
      else:
        io.open(dtClusteredSingletonState.DATADIR+'/fitness.'+str(lastFileIndex), mode='a', encoding='utf-8').write( '__empty__\n' )

      if np.size(dtClusteredSingletonState.ADDDATA) != np.shape(dtClusteredSingletonState.ADDDATADEF)[0]:
        raise ValueError(
          'Size of ADDDATA is not equal to size of ADDDATADEF  ' + str(dtClusteredSingletonState.ADDDATA) + ' ' + str(dtClusteredSingletonState.ADDDATADEF)
          )
      for i in range( np.size(dtClusteredSingletonState.ADDDATA) ):
        if np.size( dtClusteredSingletonState.ADDDATADEF[i] ) == 0 or np.size(dtClusteredSingletonState.ADDDATA) != np.shape(dtClusteredSingletonState.ADDDATADEF)[0]:
          io.open(
            dtClusteredSingletonState.DATADIR+'/'+dtClusteredSingletonState.ADDDATA[i]+'.'+str(lastFileIndex), mode='a', encoding='utf-8'
          ).write( '__empty__\n' )
        else:
          io.open(
            dtClusteredSingletonState.DATADIR+'/'+dtClusteredSingletonState.ADDDATA[i]+'.'+str(lastFileIndex), mode='a', encoding='utf-8'
          ).write( self.formatToWrite(dtClusteredSingletonState.ADDDATADEF[i])+'\n' )
    #
    # existing id
    #
    else:
      if id > dtClusteredSingletonState.currentMaxId():
        raise ValueError(
          'Create dtClusteredSingletonState with id = %d that does not exist. Current maxId = %d'
          % (id, dtClusteredSingletonState.currentMaxId())
        )
      self.cur_id = id

    self.cur_state = dtClusteredSingletonState.PREFIX+'_'+str(self.cur_id)

  @staticmethod
  def backup( stamp='' ):
    pass

  @staticmethod
  def islandEvolutes( isl, evo, x, f ):
    pass

  @staticmethod
  def islandFertilizes( isl, evo, x, f ):
    pass

  @staticmethod
  def fileIndex( id ):
    return int( (id  - 1) / 1000 )

  @staticmethod
  def currentMaxId():
    lastFileIndex = -1
    for aFile in glob.glob(dtClusteredSingletonState.DATADIR+'/id.*'):
      thisFileIndex = int(
        aFile.replace(
          dtClusteredSingletonState.DATADIR+'/id.',
          ''
        )
      )
      if lastFileIndex < thisFileIndex:
        lastFileIndex = thisFileIndex
    if (lastFileIndex<0):
      raise ValueError('Detected FileIndex is smaller than zero.')
    lastId = -1
    lastId = sum(
      1 for line in io.open(dtClusteredSingletonState.DATADIR+'/id.'+str(lastFileIndex), encoding='utf-8')
    )
    return (lastId + lastFileIndex*1000)

  def id(self):
    if ( self.cur_id < 0 ):
      raise ValueError('Id is smaller than zero')
    return self.cur_id

  def state(self):
    if ( self.cur_id < 0 ):
      raise ValueError('Id is smaller than zero')
    return self.cur_state

  def formatToWrite( self, value ):
    rStr = '__unknownDataType__'
    if isinstance(value,float):
      rStr = "{:s}".format(repr(value))
    elif isinstance(value,int):
      rStr = "{:s}".format(repr(value))
    elif isinstance(value,str):
      rStr = "{:s}".format(value)
    elif isinstance( np.array(value), np.ndarray):
      valueAsArr = np.array(value)

      valueAsStr = np.array2string(
        valueAsArr,
        formatter = {
          'float_kind':lambda valueAsArr: "%s" % repr(valueAsArr)
        }
      )
      #
      # remove square brackets and line breaks
      #
      rStr = valueAsStr.replace('[','').replace(']','').replace('\n','')
    elif isinstance(value, dict):
      rStr = json.dumps(value)
    else:
      logging.warning('Try to write unknown datatype.')

    return rStr

  @lockutils.synchronized('fileIO', external=True)
  def update( self, fileName, value ):
    fileIndex = dtClusteredSingletonState.fileIndex( self.cur_id )
    tmpF = tempfile.TemporaryFile(mode='r+', encoding='utf-8')
    fullFileName = dtClusteredSingletonState.DATADIR+'/'+fileName+'.'+str(fileIndex)
    curF = io.open( fullFileName, mode='r', encoding='utf-8' )
    lineCounter = 0 + fileIndex*1000
    for line in curF:
      lineCounter = lineCounter + 1
      if lineCounter == self.cur_id:
        tmpF.write(self.formatToWrite(value)+'\n')
      else:
        tmpF.write( line )
    curF.close()

    curF = io.open(fullFileName, mode='w', encoding='utf-8')
    tmpF.seek(0)
    for line in tmpF:
      curF.write( line )
    curF.close()

  def read( self, fName ):
    fileIndex = dtClusteredSingletonState.fileIndex( self.cur_id )
    fullFileName = dtClusteredSingletonState.DATADIR+'/'+fName+'.'+str(fileIndex)
    curF = io.open( fullFileName, mode='r', encoding='utf-8' )
    lineCounter = 0 + fileIndex*1000
    for line in curF:
      lineCounter = lineCounter + 1
      if lineCounter == self.cur_id:
        return line.replace('\n','')

  def readFloatArray( self, fName ):
    ret = np.zeros(0, float)
    try:
      ret = np.fromstring( self.read(fName), dtype=float, sep=' ' )
    except:
      logging.warning('exception in numpy.fromstring : %s', self.read(fName))

    if ( ret.size == 0):
      logging.warning('invalid float array : %s', self.read(fName))

    return ret

  def readInt( self, fName ):
    ret = float('NaN')
    try:
      ret = int( self.read(fName) )
    except:
      logging.warning('invalid integer : %s', self.read(fName))
    return ret

  def objective( self ):
    return self.readFloatArray( 'objective' )

  def fitness( self ):
    return self.readFloatArray( 'fitness' )

  def readDict( self, fName ):
    ret = {}
    try:
      ret = dict( ast.literal_eval( self.read(fName) ) )
    except:
      logging.warning('invalid dict : %s', self.read(fName))
    return ret

  @staticmethod
  def readIdFromObjective( obj ):
    bestFit = float('inf')
    bestId = -1
    #ids = np.zeros( 0, int )
    for i in range( 1,dtClusteredSingletonState.currentMaxId() ):
      thisObj = dtClusteredSingletonState(i).objective()
      if thisObj.size != obj.size:
        continue
      thisFit = abs( np.sum( thisObj - obj ) )
      if (thisFit < bestFit):
        bestId = i
        bestFit = thisFit

    if (bestFit > .1):
      logging.warning(
        'readIdFromObjective( %s ) -> bestId = %d, bestFit = %f ',
        bestId, bestId, bestFit
      )
    return bestId

  @staticmethod
  def oneD( arr ):
    if arr.ndim == 0:
      arr = arr.reshape((1,))
    return arr

  @staticmethod
  def twoD( arr ):
    if arr.ndim == 1:
      arr = arr.reshape((1,np.size(arr)))
    return arr

  @staticmethod
  def fullRead( addFile=None, addDtype=float ):
    if \
      not os.path.isfile(dtClusteredSingletonState.DATADIR+'/fitness.0') and \
      not os.path.isfile(dtClusteredSingletonState.DATADIR+'/objective.0') and \
      not os.path.isfile(dtClusteredSingletonState.DATADIR+'/id.0'):
      return \
        np.zeros(0, int), \
        np.zeros((0,0), int), \
        np.zeros((0,0), int)

    maxFileIndex = dtClusteredSingletonState.fileIndex(
      dtClusteredSingletonState.currentMaxId()
    )

    FIT = np.genfromtxt(dtClusteredSingletonState.DATADIR+'/fitness.0')
    OBJ = np.genfromtxt(dtClusteredSingletonState.DATADIR+'/objective.0')
    ID = np.genfromtxt(dtClusteredSingletonState.DATADIR+'/id.0', dtype=int)
    ADD = None
    for thisIndex in range(maxFileIndex):
      logging.info('fullRead file index %d', thisIndex)
      f = np.genfromtxt(dtClusteredSingletonState.DATADIR+'/fitness.'+str(thisIndex+1))
      o = np.genfromtxt(dtClusteredSingletonState.DATADIR+'/objective.'+str(thisIndex+1))
      i = np.genfromtxt(dtClusteredSingletonState.DATADIR+'/id.'+str(thisIndex+1), dtype=int)
      FIT = np.concatenate( (FIT, dtClusteredSingletonState.oneD(f)) )
      OBJ = np.concatenate( (OBJ, dtClusteredSingletonState.twoD(o)) )
      ID = np.concatenate( (ID, dtClusteredSingletonState.oneD(i)) )
    if addFile is not None:
      ADD = np.genfromtxt(dtClusteredSingletonState.DATADIR+'/'+addFile+'.0', dtype=addDtype)
      for thisIndex in range(maxFileIndex):
        add = np.genfromtxt(dtClusteredSingletonState.DATADIR+'/'+addFile+'.'+str(thisIndex+1), dtype=addDtype)
        ADD = np.concatenate( (ADD, add) )

    if np.size( np.shape(FIT) ) == 1:
      FIT = np.resize(FIT, (np.size(FIT),1))

    if addFile is not None:
      return ID, OBJ, FIT, ADD
    else:
      return ID, OBJ, FIT

  @staticmethod
  def fullAddRead( addFileV, addDtypeV ):
    retMap = {}

    if \
      not os.path.isfile(dtClusteredSingletonState.DATADIR+'/fitness.0') and \
      not os.path.isfile(dtClusteredSingletonState.DATADIR+'/objective.0') and \
      not os.path.isfile(dtClusteredSingletonState.DATADIR+'/id.0'):
      pass

    else:
      maxFileIndex = dtClusteredSingletonState.fileIndex(
        dtClusteredSingletonState.currentMaxId()
      )

      for addFile, addDtype in zip(addFileV, addDtypeV):
        ADD = []
        if addDtype == dict:
          ADD = dtClusteredSingletonState.fullAddReadDict(addFile, maxFileIndex)
        else:
          ADD = np.genfromtxt(dtClusteredSingletonState.DATADIR+'/'+addFile+'.0', dtype=addDtype)
          for thisIndex in range(maxFileIndex):
            add = np.genfromtxt(dtClusteredSingletonState.DATADIR+'/'+addFile+'.'+str(thisIndex+1), dtype=addDtype)
            ADD = np.concatenate( (ADD, add) )
        retMap[addFile] = ADD

    return retMap

  @staticmethod
  def fullAddReadDict( addFile, maxFileIndex ):
    ADD = np.full(dtClusteredSingletonState.currentMaxId(),dict())
    count = 0
    for thisIndex in range(maxFileIndex+1):
      thisFile = open(
        dtClusteredSingletonState.DATADIR+'/'+addFile+'.'+str(thisIndex),
        'r'
      )
      theLines = thisFile.readlines()
      for aLine in theLines:
        #print("%05d : %s" % (count, line.strip()))
        ADD[count] = dict( ast.literal_eval( aLine.strip() ) )
        count = count + 1
    return ADD

  def hasDirectory(self):
    return os.path.isdir( dtClusteredSingletonState.CASE+'_'+self.state() )
