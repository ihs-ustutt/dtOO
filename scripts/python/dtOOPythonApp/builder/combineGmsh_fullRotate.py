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

from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import (
  boundedVolume,
  combineGmsh,
  #map2dTo3d,
  #partRotatingMap2dTo3d,
  dtVector3,
  dtPoint3,
  jsonPrimitive,
  labeledVectorHandlingBoundedVolume,
  #vectorBoundedVolume,
  rotate,
  baseContainer,
  bVOTransformMeshPoints,
  bVOOrientCellVolumes,
  bVOWriteMSH,
  vectorStr,
  bVOFaceToPatchRule,
)

from typing import List
import numpy as np
import logging

class combineGmsh_fullRotate(dtBundleBuilder):
  def __init__( 
    self, 
    label: str, 
    section: boundedVolume,
    numberOfSections: int,
    comPhys: List[str] = ['inlet', 'outlet', 'hub', 'shroud', 'blade',],
    rmPhys: List[str] = ['suction', 'pressure', ],
    rotVector: dtVector3 = dtVector3(0,0,1),
    origin: dtPoint3 = dtPoint3(0,0,0),
    absTol: float = 1.e-4,
    relTol: float = 1.e-4
  ) -> None:
    """Constructor.
  
    Parameters
    ----------
    label: str
      Label.
    section: boundedVolume
      One section of the full rotated mesh.
    numberOfSections: int
      Number of periodic sections.
    rotVector: dtVector3
      Rotation vector.
 
    Returns
    -------
    None
  
    """
    logging.info( "Initializing %s ..." % (label) )
    super(combineGmsh_fullRotate, self).__init__()
    self.label_ = label
    self.section_ = section
    self.numberOfSections_ = numberOfSections
    self.rotVector_ = rotVector
    self.origin_ = origin
    self.absTol_ = absTol
    self.relTol_ = relTol
    self.comPhys_ = comPhys
    self.rmPhys_ = rmPhys

  def build(self) -> None:
    """Build part.

    Parameters
    ----------
    None

    Returns
    -------
    None

    """
    logging.info( "Building %s ..." % (self.label_) )

    bVs = labeledVectorHandlingBoundedVolume()
    bVs.push_back(self.section_)
    comGmsh = combineGmsh()
    comGmsh.jInit(
      jsonPrimitive()\
        .appendStr("label", self.label_)\
        .appendVectorBoundedVolume("", bVs)\
        .appendReal("_relTol", self.relTol_)\
        .appendReal("_absTol", self.absTol_),
      None, None, None, None, bVs
    )
   
    bC = baseContainer()
    tLabels = vectorStr()
    for ii in range(1,self.numberOfSections_):
        ref = rotate(
                jsonPrimitive()\
                  .appendStr("label", self.label_+"_rot_"+str(ii))\
                  .appendDtPoint3("_origin", self.origin_)\
                  .appendDtVector3("_rotVector", self.rotVector_)\
                  .appendReal("_angle", ii*(1.0/self.numberOfSections_)*2.0*np.pi)\
        )
        tLabels.append(ref.getLabel())
        ref.thisown = False
        bC.ptrTransformerContainer().add(ref)
    
    ob = bVOTransformMeshPoints()
    ob.jInit(
      jsonPrimitive()\
        .appendBool("_copy", True)\
        .appendVectorStr("transformer", tLabels)\
        .appendReal("_relTol", self.relTol_)\
        .appendReal("_absTol", self.absTol_),
      bC, 
      None, 
      None, 
      None, 
      None, 
      comGmsh
    )
    ob.thisown = False
    comGmsh.attachBVObserver(ob)

    ob = bVOWriteMSH()
    ob.thisown = False
    ob.jInit(
      jsonPrimitive( 
        '{'
          '"_filename" : "",'
          '"_saveAll" : true'
        '}' 
      ), 
      comGmsh 
    )
    comGmsh.attachBVObserver(ob)

    ob = bVOOrientCellVolumes()
    ob.thisown = False
    ob.jInit( jsonPrimitive('{"_positive" : true}'), comGmsh )
    comGmsh.attachBVObserver(ob)
    comGmsh.thisown = False
   
    if (np.size(self.comPhys_) + np.size(self.rmPhys_)) != 0:
      # patch rule
      pr = vectorStr()
      for comPhy in self.comPhys_:
          pr.append(":*"+comPhy+"*::"+self.label_+"_"+comPhy+":")
      for rmPhy in self.rmPhys_:
          pr.append(":*"+rmPhy+"*:::")
      # reg rule
      rr = vectorStr()
      rr.append(":*::"+self.label_+":")

      ob = bVOFaceToPatchRule()
      ob.thisown = False
      ob.jInit(
        jsonPrimitive()\
          .appendVectorStr("_patchRule", pr)\
          .appendVectorStr("_regRule", rr), 
        comGmsh 
      )
      comGmsh.attachBVObserver(ob)

    self.appendBoundedVolume( comGmsh )

    return
