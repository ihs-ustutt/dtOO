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

from oslo_concurrency import lockutils
import glob
import os
import tempfile
import numpy as np
import logging
import io
import json
import ast
from pyDtOO.dtDirectory import dtDirectory
from pyDtOO.dtGod import dtGod
from typing import List, Union, Any, Tuple

class dtClusteredSingletonState:
  """Manage database.

  Connection to the database that holds at least fitness and objective values.
  Objectives (inputs, parameters, degrees of freedom, ...) are the values that
  are changed during optimization to improve the fitness (result value, ...) of
  the problem. Every individual has an ID and a state as integer and str,
  respectively. This class also stores optional (additional) data for each
  individual. A data value is in the simplest case a scalar value as integer
  or float. But it can be also a float array or a dict.

  The class stores all data in the directory `DATADIR`. Fitness value,
  objective value, ID and state are stored in the files `fitness.*`,
  `objective.*`, `id.*` and `state.*`, respectively. If additional data is
  used the files are named according to the `ADDDATA` array. The data in
  `DATADIR` are divided in chunks of 1000 lines per file.

  Attributes
  ----------
  PREFIX : str
    Prefix for the state label. The default is 'A1'
  CASE : str
    Label of the simulation case. The default is ''
  LOCKDIR : dtDirectory
    Directory of the lock directory. The default is dtGod().LockPath()
  PIDDIR : str
    Directory of the PIDDIR. The default is './runPid'
  DATADIR : str
    Directory to store the data. The default is './runData'
  NPROC : int
    Number of processes. The default is 1
  SIMSH : str
    Name of the simulation script. The default is ''
  ADDDATA : List[str]
    Array of additional (optional) data. The default is []
  ADDDATADEF : List[ Union[ List[float], int, dict] ]
    Default values for additional data. The default is []
  PROB : Any
    Problem class. The default is None
  id_ : float
    ID of individual
  state_ : str
    State of individual

  Examples
  --------
  Add additional values for each individual

  >>> dtClusteredSingletonState.ADDDATA = [                                           
  ...   'dict',                                                                       
  ...   'float',                                                                      
  ...   'int',                                                                        
  ...   'np-float-arr',                                                               
  ...   'np-int-arr',                                                                 
  ...   'str',                                                                        
  ... ]                                                                               
  >>> dtClusteredSingletonState.ADDDATADEF = [                                        
  ...   {'key' : 0.0},                                                                
  ...   1.0,                                                                          
  ...   2,                                                                            
  ...   np.full(3, 5.0),                                                              
  ...   np.full(3, -5),                                                               
  ...   'test',                                                                       
  ... ]

  Create first individual

  >>> dtClusteredSingletonState(
  ...   defObj=[0.0,0.0,], 
  ...   defFit=[0.0,0.0,]
  ... ) # doctest: +ELLIPSIS
  <dtClusteredSingletonState.dtClusteredSingletonState object at ...>

  Access individual by id

  >>> cSS = dtClusteredSingletonState(1)

  Check if data exactly matches the ADDDATADEF list

  >>> for i in dtClusteredSingletonState.ADDDATA:                                     
  ...   print( 
  ...     cSS(i) 
  ...     == 
  ...     dtClusteredSingletonState.ADDDATADEF[
  ...       dtClusteredSingletonState.ADDDATA.index(i)
  ...     ] 
  ...   )
  True
  True
  True
  [ True  True  True]
  [ True  True  True]
  True
  
  Write new data

  >>> cSS = dtClusteredSingletonState(1)
  >>> cSS.update('fitness', 1.0)
  >>> cSS.update('objective', [1.0,2.0])
  >>> cSS.update('dict', {'float' : 1.0, 'str' : 'myStr'})

  Read default data from individual

  >>> cSS.id()
  1
  >>> cSS.state()
  'A1_1'
  >>> cSS.objective()
  array([1., 2.])
  >>> cSS.fitness()
  array([1.])

  Read additional data as str

  >>> cSS.read('float')
  '1.e+00'
  >>> cSS.read('dict')
  '{"float": 1.0, "str": "myStr"}'

  Read additional data with specified type

  >>> cSS.readFloatArray('float')
  array([1.])
  >>> cSS.readFloat('float')
  1.0
  >>> cSS.readArray('np-int-arr', int)
  array([-5, -5, -5])
  >>> cSS.read('np-int-arr')
  '-5 -5 -5'
  >>> cSS.readDict('dict')
  {'float': 1.0, 'str': 'myStr'}

  Check data again; some checks have to fail, because the data was updated
  
  >>> for i in dtClusteredSingletonState.ADDDATA:                                     
  ...   print("=> %s" % i)                                                            
  ...   print(cSS(i))                                                                   
  ...   print( 
  ...     cSS(i) 
  ...     == 
  ...     dtClusteredSingletonState.ADDDATADEF[
  ...       dtClusteredSingletonState.ADDDATA.index(i)
  ...     ] 
  ...   )
  => dict
  {'float': 1.0, 'str': 'myStr'}
  False
  => float
  1.0
  True
  => int
  2
  True
  => np-float-arr
  [5. 5. 5.]
  [ True  True  True]
  => np-int-arr
  [-5 -5 -5]
  [ True  True  True]
  => str
  test
  True
  """

  PREFIX: str = 'A1'
  CASE: str = ''
  LOCKDIR: dtDirectory = dtGod().LockPath()
  DATADIR: str = './runData'
  ADDDATA: List[str] = []
  ADDDATADEF: List[ Union[ float, int, List[float], List[int], dict] ] = []
  PIDDIR: str = './runPid'
  NPROC: int = 1
  SIMSH: str = ''
  PROB: Any = None

  @staticmethod
  @lockutils.synchronized('fileIO', external=True)
  def clear( remove: bool = False ) -> None:
    """Clear database.

    Parameters
    ----------
    remove : bool
      Flag to clean directory, too.

    Returns
    -------
    None

    """
    if remove:
      logging.info("Delete database in %s" % dtClusteredSingletonState.DATADIR)
      os.system('rm -rf '+dtClusteredSingletonState.DATADIR)

    dtClusteredSingletonState.PREFIX = 'A1'
    dtClusteredSingletonState.LOCKDIR = dtGod().LockPath()
    dtClusteredSingletonState.DATADIR = './runData'
    dtClusteredSingletonState.ADDDATA = []
    dtClusteredSingletonState.ADDDATADEF = []
    dtClusteredSingletonState.PIDDIR = './runPid'
    dtClusteredSingletonState.NPROC = 1
    dtClusteredSingletonState.SIMSH = ''
    dtClusteredSingletonState.CASE = ''
    dtClusteredSingletonState.PROB = None

  @lockutils.synchronized('fileIO', external=True)
  def __init__(
    self,
    id: Union[int,str] = -1,
    defObj: Union[ None, List[float] ] = None,
    defFit: Union[ None, List[float] ] = None
  ) -> None:
    """Constructor

    Create new individual and initially write data, if default arguments are
    given.

    Access an existing individual and its data, if id is given.

    Parameters
    ----------
    id : Union(int,str), optional
      ID of individual. The default is -1.
    defObj : Union[ None, List[float] ], optional
      Default objective values. The default is None.
    defFit : Union[ None, List[float] ], optional
      Default fitness values. The default is None.

    Returns
    -------
    None

    Examples
    --------

    >>> dtClusteredSingletonState.clear(True)
    >>> cSS = dtClusteredSingletonState()
    >>> cSS.id()
    1
    """
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
      if \
        not os.path.isdir( dtClusteredSingletonState.DATADIR ) \
        or \
        not os.path.isfile( dtClusteredSingletonState.DATADIR+'/id.0' ):
        if not os.path.isdir( dtClusteredSingletonState.DATADIR ):
          os.mkdir(dtClusteredSingletonState.DATADIR)
        io.open(
          dtClusteredSingletonState.DATADIR+'/id.0', 
          mode='w', 
          encoding='utf-8'
        ).close()
        io.open(
          dtClusteredSingletonState.DATADIR+'/state.0', 
          mode='w', 
          encoding='utf-8'
        ).close()
        io.open(
          dtClusteredSingletonState.DATADIR+'/objective.0', 
          mode='w', 
          encoding='utf-8'
        ).close()
        io.open(
          dtClusteredSingletonState.DATADIR+'/fitness.0', 
          mode='w', 
          encoding='utf-8'
        ).close()
        for i in range( np.size(dtClusteredSingletonState.ADDDATA) ):
          io.open(
            dtClusteredSingletonState.DATADIR\
              +'/'\
              +dtClusteredSingletonState.ADDDATA[i]\
              +'.0', 
            mode='w', 
            encoding='utf-8'
          ).close()
        self.id_ = 1
      #
      # write files
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
      self.id_ = lastId + lastFileIndex*1000 + 1

      if ( self.id_ > (lastFileIndex+1)*1000 ):
        lastFileIndex = lastFileIndex + 1
        io.open(dtClusteredSingletonState.DATADIR+'/id.'+str(lastFileIndex), mode='w', encoding='utf-8').close()
        io.open(dtClusteredSingletonState.DATADIR+'/state.'+str(lastFileIndex), mode='w', encoding='utf-8').close()
        io.open(dtClusteredSingletonState.DATADIR+'/objective.'+str(lastFileIndex), mode='w', encoding='utf-8').close()
        io.open(dtClusteredSingletonState.DATADIR+'/fitness.'+str(lastFileIndex), mode='w', encoding='utf-8').close()

      io.open(dtClusteredSingletonState.DATADIR+'/id.'+str(lastFileIndex), mode='a', encoding='utf-8').write( str(self.id_)+'\n' )
      io.open(dtClusteredSingletonState.DATADIR+'/state.'+str(lastFileIndex), mode='a', encoding='utf-8').write( dtClusteredSingletonState.PREFIX+'_'+str(self.id_)+'\n' )
      if defObj is not None:
        io.open(dtClusteredSingletonState.DATADIR+'/objective.'+str(lastFileIndex), mode='a', encoding='utf-8').write( self.formatToWrite(defObj)+'\n' )
      else:
        io.open(dtClusteredSingletonState.DATADIR+'/objective.'+str(lastFileIndex), mode='a', encoding='utf-8').write( '__empty__\n' )
      if defFit is not None:
        io.open(dtClusteredSingletonState.DATADIR+'/fitness.'+str(lastFileIndex), mode='a', encoding='utf-8').write( self.formatToWrite(defFit)+'\n'  )
      else:
        io.open(dtClusteredSingletonState.DATADIR+'/fitness.'+str(lastFileIndex), mode='a', encoding='utf-8').write( '__empty__\n' )

      if len(dtClusteredSingletonState.ADDDATA) != len(dtClusteredSingletonState.ADDDATADEF):
        raise ValueError(
          'Size of ADDDATA is not equal to size of ADDDATADEF  ' + str(dtClusteredSingletonState.ADDDATA) + ' ' + str(dtClusteredSingletonState.ADDDATADEF)
          )
      for i in range( np.size(dtClusteredSingletonState.ADDDATA) ):
        if \
          np.size( dtClusteredSingletonState.ADDDATADEF[i] ) == 0 \
          or \
          ( \
            np.size(dtClusteredSingletonState.ADDDATA) \
            != \
            len(dtClusteredSingletonState.ADDDATADEF) \
          ):
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
      self.id_ = id

    self.state_ = dtClusteredSingletonState.PREFIX+'_'+str(self.id_)

  @staticmethod
  def fileIndex( id: int ) -> int:
    """Get file index of individual's data.

    Parameters
    ----------
    id : int
      ID of individual.

    Returns
    -------
    int
      File index.

    """
    return int( (id  - 1) / 1000 )

  @staticmethod
  def currentMaxId() -> int:
    """Get current maximum ID stored in database.

    Raises
    ------
    ValueError
      If file index < 0.

    Returns
    -------
    int
      Maximum ID.

    Examples
    --------
    >>> dtClusteredSingletonState.clear(True)
    >>> a = dtClusteredSingletonState()
    >>> b = dtClusteredSingletonState()
    >>> dtClusteredSingletonState.currentMaxId()
    2
    """
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

  def id(self) -> int:
    """Get individual's ID.

    Raises
    ------
    ValueError
      If ID < 0.

    Returns
    -------
    int
      ID.

    """
    if ( self.id_ < 0 ):
      raise ValueError('Id is smaller than zero')
    return self.id_

  def state(self) -> str:
    """Get individual's state.

    Raises
    ------
    ValueError
      If ID < 0.

    Returns
    -------
    str
      State.

    """
    if ( self.id_ < 0 ):
      raise ValueError('Id is smaller than zero')
    return self.state_

  @staticmethod
  def formatToWrite( value: Union[float, int, dict, np.array] ) -> str:
    """Converts `value` to str.

    Parameters
    ----------
    value : Union[float, int, dict, np.array]
      Value to convert.

    Returns
    -------
    str
      Converted value.

    Examples
    --------
    >>> dtClusteredSingletonState.formatToWrite(1.0)
    '1.e+00'
    >>> dtClusteredSingletonState.formatToWrite(1)
    '1'
    >>> dtClusteredSingletonState.formatToWrite('word')
    'word'
    >>> dtClusteredSingletonState.formatToWrite(
    ...   {'str' : 'word', 'float' : 1.0}
    ... )
    '{"str": "word", "float": 1.0}'
    >>> dtClusteredSingletonState.formatToWrite(np.full(2,1.0))
    '1.e+00 1.e+00'
    >>> try:
    ...   dtClusteredSingletonState.formatToWrite(['a', 'b',])
    ... except ValueError as e:
    ...   print(e)
    Unknown datatype.
    >>> class testclass:
    ...   pass
    >>> try:
    ...   dtClusteredSingletonState.formatToWrite(testclass)
    ... except ValueError as e:
    ...   print(e)
    Unknown datatype.
    """

    rStr = '__unknownDataType__'
    if isinstance(value,float):
      rStr = np.format_float_scientific(value)
    elif isinstance(value,int):
      rStr = "{:s}".format(repr(value))
    elif isinstance(value,str):
      rStr = "{:s}".format(value)
    elif isinstance(value, dict):
      rStr = json.dumps(value)
    elif isinstance( np.array(value), np.ndarray):
      valueAsArr = np.array(value)
      if (valueAsArr.dtype == int) or (valueAsArr.dtype == float):
        valueAsStr = np.array2string(
          valueAsArr,
          formatter = {
            'float_kind': np.format_float_scientific
          }
        )
        #
        # remove square brackets and line breaks
        #
        rStr = valueAsStr.replace('[','').replace(']','').replace('\n','')

    if rStr == '__unknownDataType__':
      raise ValueError('Unknown datatype.')

    return rStr

  @lockutils.synchronized('fileIO', external=True)
  def update(
    self, fileName: str, value: Union[float, int, dict, np.array]
  ) -> None:
    """Write data of `fName` for current individual.

    Parameters
    ----------
    fileName : str
      Label of data.
    value : Union[float, int, dict, np.array]
      Value.

    Returns
    -------
    None

    """
    fileIndex = dtClusteredSingletonState.fileIndex( self.id_ )
    tmpF = tempfile.TemporaryFile(mode='r+', encoding='utf-8')
    fullFileName = \
      dtClusteredSingletonState.DATADIR+'/'+fileName+'.'+str(fileIndex)
    curF = io.open( fullFileName, mode='r', encoding='utf-8' )
    lineCounter = 0 + fileIndex*1000
    for line in curF:
      lineCounter = lineCounter + 1
      if lineCounter == self.id_:
        tmpF.write(self.formatToWrite(value)+'\n')
      else:
        tmpF.write( line )
    curF.close()

    curF = io.open(fullFileName, mode='w', encoding='utf-8')
    tmpF.seek(0)
    for line in tmpF:
      curF.write( line )
    curF.close()

  def read( self, fName: str ) -> str:
    """Read data of `fName` for current individual.

    Parameters
    ----------
    fName : str
      Label of data.

    Returns
    -------
    str
      Value.

    """
    fileIndex = dtClusteredSingletonState.fileIndex( self.id_ )
    fullFileName = \
      dtClusteredSingletonState.DATADIR+'/'+fName+'.'+str(fileIndex)
    curF = io.open( fullFileName, mode='r', encoding='utf-8' )
    lineCounter = 0 + fileIndex*1000
    for line in curF:
      lineCounter = lineCounter + 1
      if lineCounter == self.id_:
        return line.replace('\n','')

  def readArray( 
    self, fName: str, dtype: Union[ float, int, str ]  
  ) -> np.ndarray:
    """Read data of `fName` for current individual and return as array.

    Parameters
    ----------
    fName : str
      Label of data.

    Returns
    -------
    numpy.ndarray
      Converted value.

    """
    ret = np.zeros(0, dtype)
    try:
      ret = np.fromstring( self.read(fName), dtype=dtype, sep=' ' )
    except:
      logging.warning('exception in numpy.fromstring : %s', self.read(fName))

    if ( ret.size == 0):
      logging.warning('invalid array : %s', self.read(fName))

    return ret

  def readFloatArray( self, fName: str ) -> np.ndarray:
    """Read data of `fName` for current individual and return as float array.

    Parameters
    ----------
    fName : str
      Label of data.

    Returns
    -------
    numpy.ndarray
      Converted value.

    """
    return self.readArray(fName, float)

  def readIntArray( self, fName: str ) -> np.ndarray:
    """Read data of `fName` for current individual and return as int array.

    Parameters
    ----------
    fName : str
      Label of data.

    Returns
    -------
    numpy.ndarray
      Converted value.

    """
    return self.readArray(fName, int)

  def readInt( self, fName: str ) -> int:
    """Read data of `fName` for current individual and return as integer.

    Parameters
    ----------
    fName : str
      Label of data.

    Returns
    -------
    int
      Converted value.

    """
    try:
      ret = int( self.read(fName) )
    except:
      logging.warning('invalid integer : %s', self.read(fName))
    return ret

  def readFloat( self, fName: str ) -> float:
    """Read data of `fName` for current individual and return as float.

    Parameters
    ----------
    fName : str
      Label of data.

    Returns
    -------
    float
      Converted value.

    """
    try:
      ret = float( self.read(fName) )
    except:
      logging.warning('invalid integer : %s', self.read(fName))
    return ret

  def objective( self ) -> np.ndarray:
    """Read objective for current individual.

    Returns
    -------
    numpy.ndarray
      Objective.

    """
    return self.readFloatArray( 'objective' )

  def fitness( self ) -> np.ndarray:
    """Read fitness for current individual.

    Returns
    -------
    numpy.ndarray
      Fitness.

    """
    return self.readFloatArray( 'fitness' )

  def readDict( self, fName: str ) -> dict:
    """Read data of `fName` for current individual and return as dict.

    Parameters
    ----------
    fName : str
      Label of data.

    Returns
    -------
    dict
      Converted value.

    """
    ret = {}
    try:
      ret = dict( ast.literal_eval( self.read(fName) ) )
    except:
      logging.warning('invalid dict : %s', self.read(fName))
    return ret

  @staticmethod
  def readIdFromObjective( obj: np.ndarray ) -> int:
    """Returns ID of individual with objective `obj`.

    Parameters
    ----------
    obj : numpy.ndarray
      Objective.

    Returns
    -------
    int
      ID.

    Warns
    -----
    Warning if difference in objective values >0.1.

    Examples
    --------
    >>> dtClusteredSingletonState.clear(True)
    >>> a = dtClusteredSingletonState(defObj=[1.0, 1.0],defFit=[2.0])
    >>> a.objective()
    array([1., 1.])
    >>> b = dtClusteredSingletonState(defObj=[2.0, 2.0],defFit=[2.0])
    >>> b.objective()
    array([2., 2.])
    >>> dtClusteredSingletonState.readIdFromObjective(np.array(a.objective()))
    1
    >>> dtClusteredSingletonState.readIdFromObjective(np.array(b.objective()))
    2
    """

    bestFit = float('inf')
    bestId = -1
    #ids = np.zeros( 0, int )
    for i in range( 1, dtClusteredSingletonState.currentMaxId()+1 ):
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
  def oneD( arr: np.ndarray ) -> np.ndarray:
    """Convert array's shape to one-dimensional numpy.ndarray.

    Parameters
    ----------
    arr : numpy.ndarray
      Array.

    Returns
    -------
    numpy.ndarray
      Converted array.

    """
    if arr.ndim == 0:
      arr = arr.reshape((1,))
    return arr

  @staticmethod
  def twoD( arr: np.ndarray ) -> np.ndarray:
    """Convert array's shape to two-dimensional numpy.ndarray.

    Parameters
    ----------
    arr : numpy.ndarray
      Array.

    Returns
    -------
    numpy.ndarray
      Converted array.

    """
    if arr.ndim == 1:
      arr = arr.reshape((1,np.size(arr)))
    return arr

  @staticmethod
  @lockutils.synchronized('fileIO', external=True)
  def fullRead() -> Tuple[np.ndarray, np.ndarray, np.ndarray]:
    """Read IDs, objective values and fitness values of all individuals.

    Parameters
    ----------
    addFile : Union[ None, List[str] ], optional
      List of additional files to read. The default is None.
    addDtype : Union[float, int, dict], optional
      `dtype` of additional values. The default is float.

    Returns
    -------
    Union[
      Tuple[np.ndarray, np.ndarray, np.ndarray],
      Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray]
    ]
      Tuple of IDs, objective and fitness values. If `addFile` is not None,
      then the return tuple is extended by additional array.

    Examples
    --------
    >>> dtClusteredSingletonState.clear(True)
    >>> a = dtClusteredSingletonState(defObj=[1.0, 1.0],defFit=[1.5,1.5])
    >>> b = dtClusteredSingletonState(defObj=[2.0, 2.0],defFit=[2.5, 2.5])
    >>> I,O,F = dtClusteredSingletonState.fullRead()
    >>> for i,o,f in zip(I,O,F):
    ...   i
    ...   o
    ...   f
    np.int64(1)
    array([1., 1.])
    array([1.5, 1.5])
    np.int64(2)
    array([2., 2.])
    array([2.5, 2.5])
    """
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
    for thisIndex in range(maxFileIndex):
      logging.info('fullRead file index %d', thisIndex)
      f = np.genfromtxt(
        dtClusteredSingletonState.DATADIR+'/fitness.'+str(thisIndex+1)
      )
      o = np.genfromtxt(
        dtClusteredSingletonState.DATADIR+'/objective.'+str(thisIndex+1)
      )
      i = np.genfromtxt(
        dtClusteredSingletonState.DATADIR+'/id.'+str(thisIndex+1), dtype=int
      )
      if FIT.ndim == 1:
        FIT = np.concatenate( (FIT, dtClusteredSingletonState.oneD(f)) )
      else:
        FIT = np.concatenate( (FIT, dtClusteredSingletonState.twoD(f)) )
      OBJ = np.concatenate( (OBJ, dtClusteredSingletonState.twoD(o)) )
      ID = np.concatenate( (ID, dtClusteredSingletonState.oneD(i)) )

    if np.size( np.shape(FIT) ) == 1:
      FIT = np.resize(FIT, (np.size(FIT),1))

    return ID, OBJ, FIT

  @staticmethod
  @lockutils.synchronized('fileIO', external=True)
  def fullAddRead(
    addFileV: List, addDtypeV: List = []
  ) -> List[np.ndarray]:
    """Read additional data of all individuals.

    Parameters
    ----------
    addFileV : np.ndarray
      List of additional data to read.
    addDtypeV : np.ndarray
      List of additional `dtype`.

    Returns
    -------
    List

    Examples
    --------
    >>> dtClusteredSingletonState.clear(True)
    >>> dtClusteredSingletonState.ADDDATA = ['float', 'dict']
    >>> dtClusteredSingletonState.ADDDATADEF = [1.0, {'test' : 2.0},]
    >>> a = dtClusteredSingletonState()
    >>> b = dtClusteredSingletonState()
    >>> A = dtClusteredSingletonState.fullAddRead(['float', 'dict'])
    >>> A['float']
    array([1., 1.])
    >>> A['dict']
    array([{'test': 2.0}, {'test': 2.0}], dtype=object)
    >>> A['dict'][0]['test']
    2.0
    """
    retMap = {}

    if addDtypeV==[]:
      for addFile in addFileV:
        addDtypeV.append(
          type( 
            dtClusteredSingletonState.ADDDATADEF[
              dtClusteredSingletonState.ADDDATA.index(addFile)
            ]
          )
        )
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
            if np.size(add.shape) != np.size(ADD.shape):
              add = np.array([add])
            ADD = np.concatenate( (ADD, add) )
        retMap[addFile] = ADD

    return retMap

  @staticmethod
  def fullAddReadDict( addFile: str, maxFileIndex: int ) -> np.ndarray:
    """Read additional dicts of all individuals.

    Parameters
    ----------
    addFile : str
      File name of additional dict.
    maxFileIndex : int
      Maximum file index.

    Returns
    -------
    np.ndarray
      Additional dicts for all individuals.

    """
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

  def hasDirectory(self) -> bool:
    """Check if individual's case directory exist

    Returns
    -------
    bool

    """
    return os.path.isdir( dtClusteredSingletonState.CASE+'_'+self.state() )

  def __call__(self, idStr: str) -> Union[ str, dict, int, np.ndarray ]:
    """Read data of `fStr` for current individual.

    Parameters
    ----------
    fStr : str
      Label of data.

    Returns
    -------
    Union[ str, dict, int, np.ndarray ]
      Converted value.

    """
    match self.ADDDATADEF[self.ADDDATA.index(idStr)]:
      case np.ndarray():
        return self.readArray(
          idStr, 
          self.ADDDATADEF[self.ADDDATA.index(idStr)].dtype
        )
      case int():
        return self.readInt(idStr)
      case dict():
        return self.readDict(idStr)
      case float():
        return self.readFloat(idStr)
      case str():
        return self.read(idStr)
      case _:
       logging.warning(
         "> %s < is not handled. Return as str > %s <." 
         % (
             type(self.ADDDATADEF[self.ADDDATA.index(idStr)]),
             self.read(idStr)
           )
       )
    return self.read(idStr)
