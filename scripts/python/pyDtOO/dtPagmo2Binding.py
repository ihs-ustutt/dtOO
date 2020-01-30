import logging
import xml.etree.ElementTree as ET
import subprocess
#from threading import Thread, current_thread
import os
import time
import numpy
from oslo_concurrency import lockutils

class dtPagmo2Binding:
  STATECOUNTER = None
  PROB = None
  
  def __init__(self, dim=0):
    self.cVDict = dtPagmo2Binding.PROB.DoF()
    
    self.cVStr_ = []
    self.lowB_ = []
    self.upB_ = []
    for i in range(numpy.size(self.cVDict)):
      self.cVStr_.append( self.cVDict[i]['label'] )
      self.lowB_.append( self.cVDict[i]['min'] )
      self.upB_.append( self.cVDict[i]['max'] )
    self.dim = numpy.size( self.upB_ )
    self.xmlStateFileStr = 'templateState.xml'

  def get_nobj( self ):
    return numpy.size( dtPagmo2Binding.PROB.FailedFitness() )
  
  def stateLabel(self, stateNumber):
    return dtPagmo2Binding.STATECOUNTER.PREFIX+'_'+str(stateNumber)

  def findStates(self, C):
    import glob
    files = glob.glob( dtPagmo2Binding.STATECOUNTER.PREFIX+'_*.xml' )
    
    X = numpy.zeros( (numpy.shape(files)[0], numpy.shape(self.cVStr_)[0]) )
    fileCount = 0
    for aFile in files:
      #
      # parse
      #
      xmlStateFile = ET.parse( aFile )
       
      #
      # find state label and replace
      #
      stateC = xmlStateFile.find('state')
      
      if ( stateC.attrib['label'] !=  aFile.replace('.xml', '') ):
        logging.warning(
          'naming not consistent: state %s in file %s' % (stateC.attrib['label'], aFile)
        )
    
      #
      # find constValues and replace
      #
      cVCount = 0
      for aStr in self.cVStr_:
        X[fileCount, cVCount] = float( 
          stateC.find("constValue[@label='"+str(aStr)+"']").attrib['value'] 
        )
        cVCount = cVCount + 1
      
      #
      # increment file counter
      #
      fileCount = fileCount + 1
    
    
    #xmlStateFile.write(self.stateLabel(stateNumber)+'.xml')
    
    
    fStates = []
    for c in C:
      absDiffPara = abs( X - c )
      statePos = numpy.argmin(  absDiffPara.sum( axis=1) )
      if (numpy.sum(absDiffPara[statePos,:]) > 0.1):
        self.logging.warning(
          'sum( absDiffPara[ %d ] ) = %f -> %s ' % ( 
            statePos, numpy.sum(absDiffPara[statePos,:]), files[statePos]
          )
        )
      fStates.append( files[ statePos ].replace('.xml', '') )
     
    return fStates
  
  def findCoeffs(self, S):
    files = []
    for s in S:
      files.append(str(s)+'.xml')
    
    X = numpy.zeros( (numpy.shape(files)[0],numpy.shape(self.cVStr_)[0]) )
    fileCount = 0
    for aFile in files:
      #
      # parse
      #
      xmlStateFile = ET.parse( aFile )
       
      #
      # find state label and replace
      #
      stateC = xmlStateFile.find('state')
      
      if ( stateC.attrib['label'] !=  aFile.replace('.xml', '') ):
        logging.error(
          'naming not consistent: state %s in file %s' % (stateC.attrib['label'], aFile)
        )

      #
      # find constValues and replace
      #
      cVCount = 0
      for aStr in self.cVStr_:
        X[fileCount, cVCount] = float( 
          stateC.find("constValue[@label='"+str(aStr)+"']").attrib['value'] 
        )
        cVCount = cVCount + 1
      
      #
      # increment file counter
      #
      fileCount = fileCount + 1
    
    return X
  
  def pre(self, x, stateNumber=-1):
    #
    # create unique state label
    #
    if stateNumber == -1:
      s = dtPagmo2Binding.STATECOUNTER( defObj=x, defFit=dtPagmo2Binding.PROB.FailedFitness() )
    else:
      s = dtPagmo2Binding.STATECOUNTER(stateNumber)
    
    stateNumber = s.id()
    
    x, cVStr = dtPagmo2Binding.PROB.TransformPre( 
      self.stateLabel(stateNumber), x, self.cVStr_ 
    )
    #
    # parse
    #
    xmlStateFile = ET.parse( self.xmlStateFileStr )

    #
    # find state label and replace
    #
    stateC = xmlStateFile.find('state')
    stateC.attrib['label'] = self.stateLabel(stateNumber)
    #
    # find constValues and replace
    #
    aCount = 0
    for aStr in cVStr:
      print( aStr )
      stateC.find("constValue[@label='"+str(aStr)+"']").attrib['value'] = str( x[aCount] )
      aCount = aCount + 1
    
    if os.path.isfile( self.stateLabel(stateNumber)+'.xml' ):
      os.remove( self.stateLabel(stateNumber)+'.xml' ) 
    xmlStateFile.write(self.stateLabel(stateNumber)+'.xml')
    
    return stateNumber
  
  @lockutils.synchronized('sim', external=True, lock_path='./runLock/')  
  def sim(self, stateNumber):
    cmd = [
      '/bin/bash' , 
      dtPagmo2Binding.STATECOUNTER.SIMSH, 
      str(os.getcwd()), 
      dtPagmo2Binding.STATECOUNTER.CASE, 
      self.stateLabel(stateNumber),
      dtPagmo2Binding.STATECOUNTER.PIDDIR
    ]
    logging.info( 
      'Simulate : %s : %s : %s : %s', 
      str(os.getcwd()), 
      dtPagmo2Binding.STATECOUNTER.CASE, 
      self.stateLabel(stateNumber),
      dtPagmo2Binding.STATECOUNTER.PIDDIR 
    )
    
    while self.maxNumEvolsReached():
      time.sleep(1)
      
    p = subprocess.Popen(cmd)
    return p
  
  def post(self, stateNumber, x):
    aProb = dtPagmo2Binding.PROB( 
      case=dtPagmo2Binding.STATECOUNTER.CASE, 
      prefix=dtPagmo2Binding.STATECOUNTER.PREFIX, 
      stateNumber=stateNumber, coeff=x 
    )
    aProb.ReadResults()
    
    if (aProb.isOk_):
      logging.info("stateLabel = %s -> OK", self.stateLabel(stateNumber)) 
    else:
      logging.warning("stateLabel = %s -> FAIL", self.stateLabel(stateNumber)) 

    ret = aProb.GiveFitness()
    
    return ret
  
  def fitness(self, x):
    ret = dtPagmo2Binding.PROB.FailedFitness()
    
    if not os.path.isfile('./prob_noFitness'):
      #
      # preprocess
      #
      stateNumber = self.pre(x)
      
      #
      # sim
      #
      p = self.sim( stateNumber )   
      if os.path.isfile('./prob_noWait'):
        logging.info( 'Do not wait for pid = %d', p.pid )
      else:
        p.wait()

      #
      # do post
      #
      ret = self.post(stateNumber,x)
        
      #
      # insert in database
      #
      s = dtPagmo2Binding.STATECOUNTER(stateNumber)
      s.update('fitness', ret)

    #
    # return
    #    
    return ret
    
  def get_bounds(self):
    return (self.lowB_, self.upB_)
  
  def inEvolution(self):
    if os.listdir(dtPagmo2Binding.STATECOUNTER.PIDDIR) == []:
      return False
    return True
  
  def maxNumEvolsReached(self):
    if os.path.isdir( dtPagmo2Binding.STATECOUNTER.PIDDIR ):
      if numpy.shape( os.listdir(dtPagmo2Binding.STATECOUNTER.PIDDIR) )[0] > dtPagmo2Binding.STATECOUNTER.NPROC:
        return True
    return False
  
  def scaleObjective( self, obj ):
    B = numpy.array( self.get_bounds() )

    return ( obj - B[0,:] ) / ( B[1,:]-B[0,:] )

  def inBounds( self, obj ):
    scaObj = self.scaleObjective(obj)
    if ( (numpy.sum(scaObj<0.)==0) and (numpy.sum(scaObj>1.)==0) ):
      return True
    return False
  
  def giveAllSuccessfull( self ):
    I, O, F = dtPagmo2Binding.STATECOUNTER.fullRead()
    OK = numpy.zeros( numpy.size(I) )
    for i in range( numpy.size(I) ):
      if not dtPagmo2Binding.PROB.IsFailedFitness( F[i,:] ) and not numpy.isnan( F[i,0] ):
        OK[i] = 1
    
    return I[ OK==1 ], O[ OK==1, : ], F[ OK==1, : ]
  
  def giveAllFailed( self ):
    I, O, F = dtPagmo2Binding.STATECOUNTER.fullRead()
    FAILED = numpy.zeros( numpy.size(I) )
    for i in range( numpy.size(I) ):
      if dtPagmo2Binding.PROB.IsFailedFitness( F[i,:] ):
        FAILED[i] = 1
    
    return I[ FAILED==1 ], O[ FAILED==1, : ]
