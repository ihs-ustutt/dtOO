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

from dtOOPythonSWIG import jsonPrimitive
from dtOOPythonSWIG import analyticGeometry
from dtOOPythonSWIG import map2dTo3d
from dtOOPythonSWIG import map3dTo3d
from dtOOPythonSWIG import map3dTo3dGmsh
from dtOOPythonSWIG import bVOMeshRule
from dtOOPythonSWIG import bVOWriteMSH
from dtOOPythonSWIG import bVOReadMSH
from dtOOPythonSWIG import bVONameRegions
from dtOOPythonSWIG import bVOOrientCellVolumes
from dtOOPythonSWIG import dtPoint3
from dtOOPythonSWIG import dtVector3
from dtOOPythonSWIG import bVOAnalyticGeometryToFace
from dtOOPythonSWIG import labeledVectorHandlingAnalyticGeometry
from dtOOPythonSWIG import scaOneD
from dtOOPythonSWIG import bVOFaceToPatchRule
from dtOOPythonSWIG import vectorInt
from dtOOPythonSWIG import dtGmshModel
from dtOOPythonSWIG import vectorReal
from dtOOPythonSWIG import scaTanhGradingOneD
from dtOOPythonSWIG import scaTanhGradingOneDCompound
from dtOOPythonSWIG import bVOSetPrescribedElementSize
from dtOOPythonSWIG import bVOSetRotationalPeriodicity
from dtOOPythonSWIG import xYz_rPhiZ
from dtOOPythonSWIG import baseContainer

import numpy as np
from typing import List, Tuple, Union, Dict
import logging
import math

class map3dTo3dGmsh_gridFromChannel(dtBundleBuilder):
  """Create mesh's topology as map3dTo3dGmsh.

  The topology of the mesh is build from the channel. The channel consists of
  only one mesh block.
  The numbering of the mesh regions is as follows:

    - R_0    : channel
    
  The mesh block is meshed as transfinite region in addition with recombining 
  recursively. Topologies' faces are labelled as they are defined.
  This means within the topology the following faces are available:

    - hub_0

    - shroud_0

    - inlet_0

    - outlet_0

    - suction_0

    - pressure_0

  In order to set the number of nodes and to apply a specific grading to an
  edge, the following groups of edges are extracted:

    - hubToShroudLines: edges from hub to shroud

    - hubLines: edges that belong to the hub 

    - shroudLines: edges that belong to the shroud

    - inletLines: edges that belong to the inlet

    - outletLines: edges that belong to the outlet

    - suctionLines: edges that belong to the suction side

    - pressureLines: edges that belong to the pressure side

    - inletToOutletLines: edges from hub to shroud

  If debug is enabled all edges are attached with prefix "debug_".

  For the meshing process the two compound gradings "hubToShroud" and
  "inletToOutlet" are used. For "hubToShroud" and  "inletToOutlet" the 
  attribute firstElementSizeHubToShroud_ and firstElementSizeInlet_ is used, 
  respectively. 
  
  Added observers:

    - bVOReadMSH

    - bVOSetRotationalPeriodicity

    - bVOFaceToPatchRule

    - bVOWriteMSH

    - bVOOrientCellVolumes

  Attributes
  ----------
  label_: str
    Label.
  channel_: map3dTo3d
    Channel.
  nBoundaryLayers_: int
    Number of boundary layers.
  nElementsRadial_: int
    Number of elements in radial direction.
  nElementsCircumferential_: int
    Number of elements in circumferential direction.
  nElementsMeridional_: int
    Number of elements in meridional direction.
  firstElementSizeHubToShroud_: float
    Size of first element on hub and shroud.
  firstElementSizeInlet_: float
    Size of first element on inlet.
  channelHubShroudDir_: int
    Parameter direction in channel from hub to shroud.
  channelInletOutletDir_: int
    Parameter direction in channel from inlet to outlet.
  channelSuctionPressureDir_: int
    Parameter direction in channel from suction to pressure side.

  Examples
  --------
  
  """
 
  def __init__( 
    self, 
    label: str, 
    channel: analyticGeometry,
    nBoundaryLayers: int,
    nElementsRadial: int,
    nElementsCircumferential: int,
    nElementsMeridional: int,
    firstElementSizeHubToShroud: float,
    firstElementSizeInlet: float,
    channelHubShroudDir: int = 3,
    channelInletOutletDir: int = 2,
    channelSuctionPressureDir: int = 1,
    charLengthMin: float = 0.05,
    charLengthMax: float = 0.10
  ) -> None:
    """Constructor.
  
    Parameters
    ----------
    label: str
      Label.
    channel: map3dTo3d
      Channel.
    nBoundaryLayers: int
      Number of boundary layers.
    nElementsRadial: int
      Number of elements in radial direction.
    nElementsCircumferential: int
      Number of elements in circumferential direction.
    nElementsMeridional: int
      Number of elements in meridional direction.
    firstElementSizeHubToShroud: float
      Size of first element on hub and shroud.
    firstElementSizeInlet: float
      Size of first element on inlet.
    channelHubShroudDir: int
      Parameter direction in channel from hub to shroud.
    channelInletOutletDir: int
      Parameter direction in channel from inlet to outlet.
    channelSuctionPressureDir: int
      Parameter direction in channel from suction to pressure side.
    charLengthMin: float
      Sets gmsh attribute `Mesh.CharacteristicLengthMin`.
    charLengthMax: float
      Sets gmsh attribute `Mesh.CharacteristicLengthMax`.

    Returns
    -------
    None
  
    """
    logging.info( "Initializing %s ..." % (label) )
    super(map3dTo3dGmsh_gridFromChannel, self).__init__()
    self.label_ = label
    self.channel_ = map3dTo3d.MustDownCast( channel.clone() ) 
    self.nBoundaryLayers_ = nBoundaryLayers
    self.nElementsRadial_ = nElementsRadial
    self.nElementsCircumferential_ = nElementsCircumferential
    self.nElementsMeridional_ = nElementsMeridional
    self.firstElementSizeHubToShroud_ = firstElementSizeHubToShroud
    self.firstElementSizeInlet_ = firstElementSizeInlet
    self.channelHubShroudDir_ = channelHubShroudDir
    self.channelInletOutletDir_ = channelInletOutletDir
    self.channelSuctionPressureDir_ = channelSuctionPressureDir

    self.hub_, self.shroud_ = self.detectFirstAndSecond( 
      self.channel_, self.channelHubShroudDir_ 
    )
    self.inlet_, self.outlet_ = self.detectFirstAndSecond( 
      self.channel_, self.channelInletOutletDir_ 
    )
    self.suction_, self.pressure_ = self.detectFirstAndSecond( 
      self.channel_, self.channelSuctionPressureDir_ 
    )

    #
    # label faces
    #
    self.hub_.setLabel( "hub" )
    self.shroud_.setLabel( "shroud" )
    self.inlet_.setLabel( "inlet" )
    self.outlet_.setLabel( "outlet" )
    self.suction_.setLabel( "suction" )
    self.pressure_.setLabel( "pressure" )

    self.map3dTo3dGmshJson_ = jsonPrimitive(
      '{'
        '"label" : "'+self.label_+'", '
        '"option" : ['
          '{"name" : "[gmsh]General.Terminal", "value" : "1."},'
          '{"name" : "[gmsh]General.Verbosity", "value" : "100."},'
          '{"name" : "[gmsh]General.ExpertMode", "value" : "1."},'
          '{'
            '"name" : "[gmsh]Mesh.LcIntegrationPrecision", '
            '"value" : "1.0E-04"'
          '},'
          '{'
            '"name" : "[gmsh]Mesh.CharacteristicLengthMin", '
            '"value" : "'+str(charLengthMin)+'"'
          '},'
          '{'
            '"name" : "[gmsh]Mesh.CharacteristicLengthMax", '
            '"value" : "'+str(charLengthMax)+'"'
          '},'
          '{"name" : "[gmsh]Mesh.Algorithm", "value" : "1"},'
          '{'
            '"name" : "[gmsh]Mesh.MeshSizeExtendFromBoundary", '
            '"value" : "1"'
          '},'
          '{"name" : "[gmsh]Mesh.MeshSizeFromPoints", "value" : "1"}'
        '],'
        '"analyticGeometry" : []'
      '}'
    )

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

    m3dGmsh = map3dTo3dGmsh()
    m3dGmsh.jInit(
      self.map3dTo3dGmshJson_, None, None, None, None, None
    )

    #
    # add hub, shroud and channel faces
    #
    channelId = m3dGmsh.getModel().addIfRegionToGmshModel( self.channel_ )
    m3dGmsh.getModel().getDtGmshRegionByTag( 
      channelId 
    ).meshTransfiniteRecursive()
    nElements = np.zeros(3, int)
    nElements[self.channelInletOutletDir_-1] = self.nElementsMeridional_
    nElements[self.channelHubShroudDir_-1] = self.nElementsRadial_
    nElements[self.channelSuctionPressureDir_-1] = self.nElementsCircumferential_
    logging.info(
      "Mesh with nElements = (%d, %d, %d)" 
      % 
      (nElements[0], nElements[1], nElements[2])
    )
    m3dGmsh.getModel().getDtGmshRegionByTag( 
      channelId 
    ).meshWNElements(int(nElements[0]), int(nElements[1]), int(nElements[2]))
    m3dGmsh.getModel().getDtGmshRegionByTag( 
      channelId 
    ).meshRecombineRecursive()
   
    #
    # name regions
    #
    ob = bVONameRegions()
    ob.jInit( jsonPrimitive('{ "_regionLabel" : [] }'), m3dGmsh )
    ob.preUpdate()

    #
    # create lvh
    #
    aG = labeledVectorHandlingAnalyticGeometry()  
    aG.push_back( self.hub_ )
    aG.push_back( self.shroud_ )
    aG.push_back( self.inlet_ )
    aG.push_back( self.outlet_ )
    aG.push_back( self.suction_ )
    aG.push_back( self.pressure_ )
    #
    # name faces
    #
    ob = bVOAnalyticGeometryToFace()
    ob.jInit( 
      jsonPrimitive(
        '{'
          '"analyticGeometry" : ['
            '{"label" : "hub"},'
            '{"label" : "shroud"},'
            '{"label" : "inlet"},'
            '{"label" : "outlet"},'
            '{"label" : "suction"},'
            '{"label" : "pressure"}'
          '],'
          '"_inc" : 10.0'
        '}'
      ), 
      None, None, None, aG, None, m3dGmsh 
    )
    ob.preUpdate()

    #
    # extract edges for specification of number of elements and gradings
    #
    hubToShroudLines = \
      m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical( 
        "*hub*", "*shroud*" 
      )
    hubLines = \
      m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical( 
        "*hub*", "*hub*" 
      )
    shroudLines = \
      m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical( 
        "*shroud*", "*shroud*" 
      )
    inletLines = \
      m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical( 
        "*inlet*", "*inlet*" 
      )
    outletLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical( 
      "*outlet*", "*outlet*" 
    )
    suctionLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical( 
      "*suction*", "*suction*" 
    )
    pressureLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical( 
      "*pressure*", "*pressure*" 
    )
    inletToOutletLines = \
      m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical( 
        "*inlet*", "*outlet*" 
      )

    inletHubToShroudLines = (
        set(inletLines)&set(hubToShroudLines)
      ).union(
        set(inletLines)&set(-np.array(hubToShroudLines))
      )
    outletHubToShroudLines = (
        set(outletLines)&set(hubToShroudLines)
      ).union(
        set(outletLines)&set(-np.array(hubToShroudLines))
      )
    pressureHubToShroudLines = (
        set(pressureLines)&set(hubToShroudLines)
      ).union(
        set(pressureLines)&set(-np.array(hubToShroudLines))
      )
    suctionHubToShroudLines = (
         set(suctionLines)&set(hubToShroudLines)
      ).union(
        set(suctionLines)&set(-np.array(hubToShroudLines))
      )

    #
    # add debug faces and lines
    #
    if self.debug():
      for face in [
        self.hub_, self.shroud_, 
        self.inlet_, self.outlet_, 
        self.suction_, self.pressure_,
      ]:
        self.appendAnalyticGeometry( 
          face, "debug_"+self.label_+"_"+face.getLabel() 
        )

      for lab, lines in zip(
        [
          "inletHubToShroudLines", 
          "outletHubToShroudLines", 
          "pressureHubToShroudLines", 
          "suctionHubToShroudLines", 
        ], 
        [
          inletHubToShroudLines, 
          outletHubToShroudLines, 
          pressureHubToShroudLines, 
          suctionHubToShroudLines, 
        ]
      ):
        for ii in lines:
          self.appendAnalyticGeometry( 
            m3dGmsh.getModel().getDtGmshEdgeByTag( ii ).getMap1dTo3d(),
            "debug_"+self.label_+"_"+lab+"_"+str(ii)
          )

    #
    # gradings
    #
    gradings = {}
    # hub to shroud
    gradings = self.addGrading( 
      gradings,
      scaTanhGradingOneDCompound(
        scaTanhGradingOneD(
          vectorReal([0.5, 0.5, -1.0, 2.0]),
          1.0,
          0.1, 5.0
        )
      ),
      "hubToShroud",
      m3dGmsh,
      self.firstElementSizeHubToShroud_
    )
    # inletToOutlet
    gradings = self.addGrading(
      gradings,
      scaTanhGradingOneDCompound(
        scaTanhGradingOneD(
          vectorReal([1.0, -1.0, 1.0, -1.0]),
          1.0,
          0.1, 5.0
        )
      ),
      "inletToOutlet",
      m3dGmsh,
      self.firstElementSizeInlet_
    )

    #
    # specify number of elements
    #
    for lines in [ 
      hubToShroudLines, inletHubToShroudLines, outletHubToShroudLines, 
    ]:
      for line in lines:
        theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
        theEdge.setNElements( self.nElementsRadial_ )
        theEdge.setGrading( 1.0, gradings["hubToShroud"][0] )
    for lines in [inletToOutletLines,]:
      for line in lines:
        theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
        theEdge.setNElements( self.nElementsMeridional_ )
        theEdge.setGrading( 1.0, gradings["inletToOutlet"][0] )
 
    ob = bVOReadMSH()
    ob.thisown = False
    ob.jInit(
      jsonPrimitive( 
        '{'
          '"_filename" : "",'
          '"_mustRead" : false'
        '}' 
      ), 
      None, None, None, None, None, m3dGmsh 
    )
    m3dGmsh.attachBVObserver(ob)

    #
    # rotational periodicity
    #
    ob = bVOSetRotationalPeriodicity()
    theT = xYz_rPhiZ()
    theT.jInit( 
      jsonPrimitive()\
        .appendStr("label", "xYz_rPhiZ")\
        .appendDtPoint3("_origin", dtPoint3(0,0,0))\
        .appendDtVector3("_rotAxis", dtVector3(0,0,-1))\
        .appendDtVector3("_refAxis", dtVector3(1,0,0)), 
      None, 
      None, 
      None, 
      None 
    )
    bC = baseContainer()
    bC.ptrTransformerContainer().add( theT )
    ob.jInit(
      jsonPrimitive( 
        '{'
          '"_faceMaster" : "suction_0",'
          '"_faceSlave" : "pressure_0"'
        '}' 
      )\
        .appendDtTransformer( "_dtT", theT ), 
      bC, None, None, None, None, m3dGmsh 
    )
    ob.preUpdate()

    ob = bVOMeshRule()
    ob.thisown = False
    ob.jInit(
      jsonPrimitive( 
        '{'
          '"option" : ['
            '{"name" : "debug", "value" : "true"}'
          '],'
          '"_rule1D" : ['
            '"dtMeshFreeGradingGEdge(*)"'
          '],'
          '"_rule2D" : ['
            '"dtMeshTransfiniteGFace(*)",'
            '"dtMeshGFace(*)"'
          '],'
          '"_rule3D" : ['
            '"dtMeshGRegion(*)"'
          '],'
          '"_only" : [],'
          '"dtMeshOperator" : ['
            '{'
              '"name" : "dtMeshFreeGradingGEdge",'
              '"label" : "dtMeshFreeGradingGEdge",'
              '"typeTransfinite" : '+self.gradingsTypeTransfinite(gradings)+','
              '"gradingFunctions" : {'
                '"analyticFunction" '
                  ': '+self.gradingsGradingFunctions(gradings)+' '
              '}'
            '},'
           '{'
              '"name" : "dtMeshGEdge",'
              '"label" : "dtMeshGEdge"'
            '},'
            '{'
              '"name" : "dtMeshGFace",'
              '"label" : "dtMeshGFace"'
            '},'
            '{'
              '"name" : "dtMeshTransfiniteGFace",'
              '"label" : "dtMeshTransfiniteGFace"'
            '},'
            '{'
              '"name" : "dtMeshGRegion",'
              '"label" : "dtMeshGRegion",'
              '"_minQShapeMetric" : 0.0,' 
              '"_relax" : 0.1,'
              '"_nPyramidOpenSteps" : 10,'
              '"_nSmooths" : 3'
            '}'
          ']'
        '}'
      ),
      None, None, self.lVH_aF(), None, None, m3dGmsh 
    )
    m3dGmsh.attachBVObserver(ob)

    ob = bVOFaceToPatchRule()
    ob.thisown = False
    ob.jInit(
      jsonPrimitive( 
        '{'
          '"_patchRule" : ['
            '":*hub*::'+self.label_+'_hub:",'
            '":*shroud*::'+self.label_+'_shroud:",'
            '":*inlet*::'+self.label_+'_inlet:",'
            '":*outlet*::'+self.label_+'_outlet:",'
            '":*suction*::'+self.label_+'_suction:",'
            '":*pressure*::'+self.label_+'_pressure:"'
          '],'
          '"_regRule" : ['
            '":*::'+self.label_+':"'
          ']'
        '}' 
      ), 
      m3dGmsh 
    )
    m3dGmsh.attachBVObserver(ob)

    ob = bVOWriteMSH()
    ob.thisown = False
    ob.jInit(
      jsonPrimitive( 
        '{'
          '"_filename" : "",'
          '"_saveAll" : true'
        '}' 
      ), 
      m3dGmsh 
    )
    m3dGmsh.attachBVObserver(ob)

    ob = bVOOrientCellVolumes()
    ob.thisown = False
    ob.jInit(
      jsonPrimitive('{"_positive" : true}'), m3dGmsh 
    )
    m3dGmsh.attachBVObserver(ob)
    m3dGmsh.thisown = False
    
    self.appendBoundedVolume( m3dGmsh )

    return

  @staticmethod
  def detectFirstAndSecond(
    channel: map3dTo3d, direction: int
  ) -> Tuple[map2dTo3d, map2dTo3d]:
    """Detect first and second faces in a volume's direction.

    Parameters
    ----------
    channel: map3dTo3d
      Volume.
    
    """
    firstPar = 0.0
    secondPar = 1.0
    if direction<0:
      firstPar = 1.0
      secondPar = 0.0

    if direction == 1:
      first = channel.segmentConstUPercent(firstPar)
      second = channel.segmentConstUPercent(secondPar)
    elif direction == 2:
      first = channel.segmentConstVPercent(firstPar)
      second = channel.segmentConstVPercent(secondPar)
    elif direction == 3:
      first = channel.segmentConstWPercent(firstPar)
      second = channel.segmentConstWPercent(secondPar)
    else:
      raise ValueError("Wrong direction.")

    return first, second

  def addGrading(
    self, 
    gradings: Dict, 
    theRef: scaOneD, 
    gradingLabel: str, 
    m3dGmsh: map3dTo3dGmsh = None, 
    firstElementSize: float = 0.0,
    lastElementSize: float = 0.0
  ) -> Dict:
    gradingNumber = 2
    for ii in gradings:
      gradingNumber = max( gradingNumber, gradings[ii][0] )
    gradingNumber = gradingNumber + 1

    gradings[gradingLabel] = [
      gradingNumber, 
      self.label_+"_gradings_"+str(gradingNumber)+"_"+gradingLabel
    ]
    logging.info(
      "%d -> %s" % (
        gradings[gradingLabel][0], gradings[gradingLabel][1]
      )
    )
    theRef.setLabel( gradings[gradingLabel][1] )
    self.appendAnalyticFunction( theRef.clone() )
    
    if ( (firstElementSize>0.0) and (lastElementSize==0.0) ):
      ob = bVOSetPrescribedElementSize()
      ob.thisown = False
      ob.jInit( 
        jsonPrimitive()\
          .appendInt("_type", gradings[gradingLabel][0])\
          .appendReal("_firstElementSize", firstElementSize)\
          .appendAnalyticFunction(
            "_grading", self.lVH_aF()[ gradings[gradingLabel][1] ]
          ), 
        None, None, self.lVH_aF(), None, None, m3dGmsh 
      )
      m3dGmsh.attachBVObserver(ob)
    elif ( (firstElementSize>0.0) and (lastElementSize>0.0) ):
      ob = bVOSetPrescribedElementSize()
      ob.thisown = False
      ob.jInit( 
        jsonPrimitive()\
          .appendInt("_type", gradings[gradingLabel][0])\
          .appendReal("_firstElementSize", firstElementSize)\
          .appendReal("_lastElementSize", lastElementSize)\
          .appendAnalyticFunction(
            "_grading", self.lVH_aF()[ gradings[gradingLabel][1] ]
          ), 
        None, None, self.lVH_aF(), None, None, m3dGmsh 
      )
      m3dGmsh.attachBVObserver(ob)
     
    return gradings
  
  @staticmethod
  def gradingsTypeTransfinite(gradings: Dict) -> str:
    retList = list()
    for ii in gradings:
      retList.append( gradings[ii][0] ) 
    logging.debug("Convert to %s" % str(retList))
    return str(retList)

  @staticmethod
  def gradingsGradingFunctions(gradings: Dict) -> str:
    retStr = '['
    first = True
    for ii in gradings:
      if first:
        retStr = retStr + '{ "label" : "'+gradings[ii][1]+'" }'
        first = False
      else:
        retStr = retStr + ', { "label" : "'+gradings[ii][1]+'" }'
    retStr = retStr + ']'
    logging.debug("Convert to %s" % retStr)
    return retStr 
