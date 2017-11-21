from oslo_concurrency import lockutils
import os
import tempfile
import numpy as np
import logging
import time
import shutil
import datetime

class dtSingletonState:
  PREFIX = ''
  CASE = ''
  PIDDIR = 'runPid'
  DATADIR = 'runData'
  ITERDIR = 'runIter'
  NPROC = 1
  SIMSH = ''
  ADDDATA = []

  @lockutils.synchronized('__init__', external=True, lock_path='./runLock/')
  def __init__(self, id=-1):
    #
    # get id from state
    #
    if isinstance(id, str):
      id = int( id.replace( dtSingletonState.PREFIX+'_', '') )

    #
    # create new id
    #
    if id < 0:
      if not os.path.isdir( dtSingletonState.DATADIR ) or not os.path.isfile( dtSingletonState.DATADIR+'/id' ):
        if not os.path.isdir( dtSingletonState.DATADIR ):
          os.mkdir(dtSingletonState.DATADIR)
        open(dtSingletonState.DATADIR+'/id', 'w').close()
        open(dtSingletonState.DATADIR+'/state', 'w').close()
        open(dtSingletonState.DATADIR+'/objective', 'w').close()
        open(dtSingletonState.DATADIR+'/fitness', 'w').close()
        open(dtSingletonState.DATADIR+'/id', "a").write( str(1)+'\n' )
        open(dtSingletonState.DATADIR+'/state', "a").write( dtSingletonState.PREFIX+'_1\n' )
        open(dtSingletonState.DATADIR+'/objective', "a").write( '__empty__\n' )
        open(dtSingletonState.DATADIR+'/fitness', "a").write( '__empty__\n' )
        for i in range( np.size(dtSingletonState.ADDDATA) ):
          open(
            dtSingletonState.DATADIR+'/'+dtSingletonState.ADDDATA[i], "a"
          ).write( '__empty__\n' )
        self.cur_id = 1
      else:
        lastId = -1
        lastId = sum(1 for line in open(dtSingletonState.DATADIR+'/id'))
        self.cur_id = lastId + 1
        open(dtSingletonState.DATADIR+'/id', "a").write( str(self.cur_id)+'\n' )
        open(dtSingletonState.DATADIR+'/state', "a").write( dtSingletonState.PREFIX+'_'+str(self.cur_id)+'\n' )
        open(dtSingletonState.DATADIR+'/objective', "a").write( '__empty__\n' )
        open(dtSingletonState.DATADIR+'/fitness', "a").write( '__empty__\n' )
        for i in range( np.size(dtSingletonState.ADDDATA) ):
          open(
            dtSingletonState.DATADIR+'/'+dtSingletonState.ADDDATA[i], "a"
          ).write( '__empty__\n' )        
    #
    # existing id
    #
    else:
      if id > dtSingletonState.currentMaxId():
        raise ValueError(
          'Create dtSingletonState with id = %d that does not exist. Current maxId = %d'
          % (id, dtSingletonState.currentMaxId())
        )
      self.cur_id = id

    self.cur_state = dtSingletonState.PREFIX+'_'+str(self.cur_id)  

  @staticmethod
  @lockutils.synchronized('backup', external=True, lock_path='./runLock/')
  def backup( stamp='' ):
    #
    # create time stamp
    #
    if stamp =='':
      stamp = datetime.datetime.fromtimestamp(
        time.time()
      ).strftime('%Y-%m-%d_%H-%M-%S')

    logging.info('create backup : %s', stamp)    

    #
    # copy
    #
    shutil.copy(
      dtSingletonState.DATADIR+'/id', dtSingletonState.DATADIR+'/'+stamp+'_id'
    )
    shutil.copy(
      dtSingletonState.DATADIR+'/state', dtSingletonState.DATADIR+'/'+stamp+'_state'
    )
    shutil.copy(
      dtSingletonState.DATADIR+'/objective', dtSingletonState.DATADIR+'/'+stamp+'_objective'
    )
    shutil.copy(
      dtSingletonState.DATADIR+'/fitness', dtSingletonState.DATADIR+'/'+stamp+'_fitness'
    )
    for i in range( np.size(dtSingletonState.ADDDATA) ):
      shutil.copy(
        dtSingletonState.DATADIR+'/'+dtSingletonState.ADDDATA[i],
        dtSingletonState.DATADIR+'/'+stamp+'_'+dtSingletonState.ADDDATA[i]
      )

  @staticmethod  
  def islandEvolutes( isl, evo, x, f ):
    #
    # create stamp
    #
    stamp = datetime.datetime.fromtimestamp(
      time.time()
    ).strftime('%Y-%m-%d_%H-%M-%S')

    if not os.path.isdir( dtSingletonState.ITERDIR ):
      os.mkdir(dtSingletonState.ITERDIR)   

    theHistFile = open( dtSingletonState.ITERDIR+'/history', 'a' )  
    theXFile = open( dtSingletonState.ITERDIR+'/x_island_'+str(isl)+'_'+str(evo), 'w' )
    theFFile = open( dtSingletonState.ITERDIR+'/f_island_'+str(isl)+'_'+str(evo), 'w' )
    xAsArr = np.array(x)
    fAsArr = np.array(f)

    for i in range( np.shape(xAsArr)[0] ):
      #
      # x
      #
      xAsStr = np.array2string(
        xAsArr[i,:], 
        formatter = {
          'float_kind':lambda xAsArr: "%32.16f" % xAsArr
        }
      )
      #
      # remove square brackets and line breaks
      #
      xAsStr = xAsStr.replace('[','').replace(']','').replace('\n','')
      theXFile.write( xAsStr+'\n' )
      #
      #f
      #
      fAsStr = np.array2string(
        fAsArr[i,:], 
        formatter = {
          'float_kind':lambda fAsArr: "%32.16f" % fAsArr
        }
      )
      #
      # remove square brackets and line breaks
      #
      fAsStr = fAsStr.replace('[','').replace(']','').replace('\n','')
      theFFile.write( fAsStr+'\n' )      

    #
    # write history
    #
    theHistFile.write( stamp+' '+theXFile.name+' '+theFFile.name+'\n' )

    #
    # close files
    #
    theXFile.close()
    theFFile.close()
    theHistFile.close()    

  @staticmethod  
  def islandFertilizes( isl, evo, x, f ):
    #
    # create stamp
    #
    stamp = datetime.datetime.fromtimestamp(
      time.time()
    ).strftime('%Y-%m-%d_%H-%M-%S')

    if not os.path.isdir( dtSingletonState.ITERDIR ):
      os.mkdir(dtSingletonState.ITERDIR)   

    theHistFile = open( dtSingletonState.ITERDIR+'/history', 'a' )  
    theXFile = open( dtSingletonState.ITERDIR+'/x_islandFertilize_'+str(isl)+'_'+str(evo), 'w' )
    theFFile = open( dtSingletonState.ITERDIR+'/f_islandFertilize_'+str(isl)+'_'+str(evo), 'w' )
    xAsArr = np.array(x)
    fAsArr = np.array(f)

    for i in range( np.shape(xAsArr)[0] ):
      #
      # x
      #
      xAsStr = np.array2string(
        xAsArr[i,:], 
        formatter = {
          'float_kind':lambda xAsArr: "%32.16f" % xAsArr
        }
      )
      #
      # remove square brackets and line breaks
      #
      xAsStr = xAsStr.replace('[','').replace(']','').replace('\n','')
      theXFile.write( xAsStr+'\n' )
      #
      #f
      #
      fAsStr = np.array2string(
        fAsArr[i,:], 
        formatter = {
          'float_kind':lambda fAsArr: "%32.16f" % fAsArr
        }
      )
      #
      # remove square brackets and line breaks
      #
      fAsStr = fAsStr.replace('[','').replace(']','').replace('\n','')
      theFFile.write( fAsStr+'\n' )      

    #
    # write history
    #
    theHistFile.write( stamp+' '+theXFile.name+' '+theFFile.name+'\n' )

    #
    # close files
    #
    theXFile.close()
    theFFile.close()
    theHistFile.close()   

  @staticmethod
  def currentMaxId():
    return sum(1 for line in open(dtSingletonState.DATADIR+'/id'))

  def id(self):
    if ( self.cur_id < 0 ):
      raise ValueError('Id is smaller than zero')
    return self.cur_id

  def state(self):
    if ( self.cur_id < 0 ):
      raise ValueError('Id is smaller than zero')
    return self.cur_state

  @lockutils.synchronized('update', external=True, lock_path='./runLock/')
  def update( self, fileName, value ):
    tmpF = tempfile.TemporaryFile()
    curF = open(dtSingletonState.DATADIR+'/'+fileName, "r")
    lineCounter = 0
    for line in curF:
      lineCounter = lineCounter + 1
      if lineCounter == self.cur_id:
        #replace
        if isinstance(value,float):
          tmpF.write( "{:s}\n".format( repr(value) ) )
        elif isinstance(value,str):
          tmpF.write( "{:s}\n".format(value) )          
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
          valueAsStr = valueAsStr.replace('[','').replace(']','').replace('\n','')
          tmpF.write( valueAsStr+'\n' )
        else:
          tmpF.write( '__unknownDataType__\n' )
      else:
        tmpF.write( line )
    curF.close()

    curF = open(dtSingletonState.DATADIR+'/'+fileName, "w")
    tmpF.seek(0)
    for line in tmpF:
      curF.write( line )
    curF.close()

  def read( self, fName ):
    curF = open(dtSingletonState.DATADIR+'/'+fName, "r")
    lineCounter = 0
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

  @staticmethod
  def readIdFromObjective( obj ):
    bestFit = float('inf')
    bestId = -1
    #ids = np.zeros( 0, int )
    for i in range( dtSingletonState.currentMaxId() ):
      thisObj = dtSingletonState(i+1).objective()
      if thisObj.size != obj.size:
        continue
      thisFit = abs( np.sum( thisObj - obj ) )
      if (thisFit < bestFit):
        bestId = i+1
        bestFit = thisFit

    if (bestFit > .1):
      logging.warning(
        'readIdFromObjective( %s ) -> bestId = %d, bestFit = %f ', 
        bestId, obj, bestFit
      )
    return bestId
  
  @staticmethod
  @lockutils.synchronized('fullRead', external=True, lock_path='./runLock/')  
  def fullRead():
    FIT = np.genfromtxt('runData/fitness')
    OBJ = np.genfromtxt('runData/objective')
    ID = np.genfromtxt('runData/id', dtype=int)

    if np.size( np.shape(FIT) ) == 1:
      FIT = np.resize(FIT, (np.size(FIT),1))

    return ID, OBJ, FIT

  def hasDirectory(self):
    return os.path.isdir( dtSingletonState.CASE+'_'+self.state() )