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

from dtOOPythonSWIG import dtBundle
from dtOOPythonSWIG import analyticFunction
from dtOOPythonSWIG import labeledVectorHandlingAnalyticFunction
from dtOOPythonSWIG import analyticGeometry
from dtOOPythonSWIG import labeledVectorHandlingAnalyticGeometry
from dtOOPythonSWIG import boundedVolume
from dtOOPythonSWIG import labeledVectorHandlingBoundedVolume
from dtOOPythonSWIG import dtCase
from dtOOPythonSWIG import labeledVectorHandlingDtCase

from abc import ABC,abstractmethod

import logging

class dtBundleBuilder:
  def __init__( self ) -> None:
    logging.info("Initializing ...")
    self.dB_ = dtBundle()
    self.debug_ = False

  def enableDebug( self ) -> 'dtBundleBuilder':
    logging.info("Enable Debug ...")
    self.debug_ = True
    return self

  def debug( self ) -> bool:
    return self.debug_

  def lVH_aF(self) -> labeledVectorHandlingAnalyticFunction:
    return self.dB_.cptr_aF()

  def lVH_aG(self) -> labeledVectorHandlingAnalyticGeometry:
    return self.dB_.cptr_aG()

  def appendAnalyticFunction(self, aF: analyticFunction) -> None:
    logging.info("Add analyticFunction %s" % aF.getLabel())
    self.dB_.cptr_aF().append( aF.clone() )
    return

  def appendAnalyticGeometry(self, aG: analyticGeometry, label: str = "") -> None:
    logging.info("Add analyticGeometry %s" % aG.getLabel())
    theClone = aG.clone()
    if label!="":
      theClone.setLabel( label )
    logging.info( "  --> New label %s" % theClone.getLabel() )
    self.dB_.cptr_aG().append( theClone )
    return

  def appendBoundedVolume(self, bV: boundedVolume) -> None:
    logging.info("Add boundedVolume %s" % bV.getLabel())
    self.dB_.cptr_bV().append( bV )
    return

  def appendDtCase(self, dC: dtCase) -> None:
    logging.info("Add dtCase %s" % dC.getLabel())
    self.dB_.cptr_dC().append( dC )
    return

  def extract( self, dB: dtBundle ) -> dtBundle:
    for aF in self.dB_.cptr_aF():
      dB.cptr_aF().append( aF.clone() )
    for aG in self.dB_.cptr_aG():
      dB.cptr_aG().append( aG.clone() )
    for bV in self.dB_.cptr_bV():
      dB.cptr_bV().append( bV )
    for dC in self.dB_.cptr_dC():
      dB.cptr_dC().append( dC )
    
    return dB

  @abstractmethod
  def build(self) -> None:
    pass
  
  def buildExtract( self, dB: dtBundle ) -> dtBundle:
    self.build()
    return self.extract( dB )
