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
from dtOOPythonSWIG import bool_map1dTo3dInMap2dTo3d
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
from dtOOPythonSWIG import sca3PPointsBSplineOneD
from dtOOPythonSWIG import sca3PPointsBSplineOneDCompound
from dtOOPythonSWIG import bVOSetPrescribedElementSize
from dtOOPythonSWIG import bVOSetRotationalPeriodicity
from dtOOPythonSWIG import xYz_rPhiZ
from dtOOPythonSWIG import baseContainer

import numpy as np
from typing import List, Tuple, Union, Dict
import logging
import math

class map3dTo3dGmsh_gridFromChannelAndBlocks(dtBundleBuilder):
  """Create mesh's topology as map3dTo3dGmsh.

  The topology of the mesh is build from the channel and all mesh blocks that
  surround the blade. All mesh blocks' surfaces that lie not at the blade, 
  hub, shroud or at any other mesh block are treated as coupling surfaces. 
  These surfaces connect the channel with the mesh blocks. On the hub and
  shroud a prismatic boundary layer with nBoundaryLayers_ is created.

  The numbering of the mesh regions is as follows:

    - R_0    : channel
    
    - R_1     : First mesh block

    - ...
    
    - R_(N-1) : (N-1)^th mesh block
    
    - R_(N) : (N)^th mesh block

  All mesh blocks are meshed as transfinite region in addition with 
  recombining recursively. Topologies' faces are labelled as they are defined.
  This means within the topology the following faces are available:

    - hub_0

    - shroud_0

    - inlet_0

    - outlet_0

    - suction_0

    - pressure_0

    - blade_0 ... blade_(M-1)

    - coupling_0 ... coupling_(L-1)

  If only one mesh face lies on the analyticGeometry, only one face ending
  with "_0" is available. If multiple ("M" blade or "L" coupling ) faces lie 
  on the corresponding analyticGeometry, also multiple faces are available.

  In order to set the number of nodes and to apply a specific grading to an
  edge, the following groups of edges are extracted:

    - hubToShroudLines: edges from hub to shroud

    - bladeLines: edges that belong to the blade

    - hubLines: edges that belong to the hub 

    - shroudLines: edges that belong to the shroud

    - inletLines: edges that belong to the inlet

    - outletLines: edges that belong to the outlet

    - suctionLines: edges that belong to the suction side

    - pressureLines: edges that belong to the pressure side

    - bladeToHubLines: edges that share one vertex with blade and one with hub

    - bladeToShroudLines: edges that share one vertex with blade and one with
      shroud

    - bladeHubLines: edges that share both vertices with blade and hub

    - bladeShroudLines: edges that share both vertices with blade and shroud

    - inletHubToShroudLines: edges that share one vertex with hub and one
      with shroud, and additionaly lie in the inlet

    - outletHubToShroudLines: edges that share one vertex with hub and one
      with shroud, and additionaly lie in the outlet

    - pressureHubToShroudLines: edges that share one vertex with hub and one
      with shroud, and additionaly lie in the pressure side

    - suctionHubToShroudLines: edges that share one vertex with hub and one
      with shroud, and additionaly lie in the suction side

  If debug is enabled all edges are attached with prefix "debug_".

  For the meshing process the two compound gradings "hubToShroud" and
  "normalBlade" are used. Additionally, each mesh block contains a grading
  in tangential direction called "tangentialBlade_*". For "hubToShroud" and
  "normalBlade" the attribute firstElementSizeHubToShroud_ and 
  firstElementSizeNormalBlade_ is used, respectively. By providing a
  function either the function bladeShroudElementSize_ or 
  bladeHubElementSize_ the minimum number of elements for each edge is 
  determined at start and end vertex. The blending factor 
  bladeShroudElementScale_ and bladeHubElementScale_ are then used to 
  interpolate between number of elements at both ends and, thus, define the 
  number of elements for this edge. In order to have a smooth transition in 
  element sizes, between consecutive edges the "angentialBlade_*" grading is 
  adjusted for start and end vertex.

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
  blocks_: List[ map3dTo3d ]
    List of mesh blocks surrounding the blade.
  blade_: map2dTo3d
    Blade.
  hub_: map2dTo3d
    Hub.
  shroud_: map2dTo3d
    Shroud.
  inlet_: map2dTo3d
    Inlet.
  outlet_: map2dTo3d
    Outlet.
  suction_: map2dTo3d
    Suction side.
  pressure_: map2dTo3d
    Pressure side.
  couplingFaces_: List[ map2dTo3d ]
    Coupling faces between mesh blocks an channel.
  nBoundaryLayers_: int
    Number of boundary layers.
  nElementsSpanwise_: int
    Number of elements in spanwise direction.
  nElementsNormal_: int
    Number of elements in normal to the blade direction.
  firstElementSizeHubToShroud_: float
    Size of first element on hub and shroud.
  firstElementSizeNormalBlade_: float
    Size of first element at the blade in normal to the blade direction.
  bladeHubElementSize_: scaOneD
    Function describing the element size versus the standardized unwrapped 
    length of the blade at the hub.
  bladeHubElementScale_: float
    Factor defining the number of elements at the hub for each mesh block.
  bladeShroudElementSize_: scaOneD
    Function describing the element size versus the standardized unwrapped 
    length of the blade at the shroud.
  bladeShroudElementScale_: float
    Factor defining the number of elements at the shroud for each mesh block.
  channelHubShroudDir_: int
    Parameter direction in channel from hub to shroud.
  channelInletOutletDir_: int
    Parameter direction in channel from inlet to outlet.
  channelSuctionPressureDir_: int
    Parameter direction in channel from suction to pressure side.
  map3dTo3dGmshJson_: jsonPrimitive
    JSON structure for map3dTo3dGmsh.

  Examples
  --------
  
  """
  def __init__( 
    self, 
    label: str, 
    channel: analyticGeometry,
    blocks: List[analyticGeometry],
    blade: analyticGeometry,
    couplingFaces: List[analyticGeometry],
    nBoundaryLayers: int,
    nElementsSpanwise: int,
    nElementsNormal: int,
    firstElementSizeHubToShroud: float,
    firstElementSizeNormalBlade: float,
    bladeHubElementSize: scaOneD = None,
    bladeHubElementScale: int = None,
    bladeShroudElementSize: scaOneD = None,
    bladeShroudElementScale: int = None,
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
    blocks: List[ map3dTo3d ]
      List of mesh blocks surrounding the blade.
    blade: map2dTo3d
      Blade.
    couplingFaces: List[ map2dTo3d ]
      Coupling faces between mesh blocks an channel.
    nBoundaryLayers: int
      Number of boundary layers.
    nElementsSpanwise: int
      Number of elements in spanwise direction.
    nElementsNormal: int
      Number of elements in normal to the blade direction.
    firstElementSizeHubToShroud: float
      Size of first element on hub and shroud.
    firstElementSizeNormalBlade: float
      Size of first element at the blade in normal to the blade direction.
    bladeHubElementSize: scaOneD
      Function describing the element size versus the standardized unwrapped 
      length of the blade at the hub.
    bladeHubElementScale: float
      Factor defining the number of elements at the hub for each mesh block.
    bladeShroudElementSize: scaOneD
      Function describing the element size versus the standardized unwrapped 
      length of the blade at the shroud.
    bladeShroudElementScale: float
      Factor defining the number of elements at the shroud for each mesh block.
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
    super(map3dTo3dGmsh_gridFromChannelAndBlocks, self).__init__()
    self.label_ = label
    self.channel_ = map3dTo3d.MustDownCast( channel.clone() ) 
    self.blocks_ = []
    for block in blocks:
      self.blocks_.append( map3dTo3d.MustDownCast(block.clone()) )
    self.blade_ = map2dTo3d.MustDownCast( blade.clone() )
    self.couplingFaces_ = []
    for couplingFace in couplingFaces:
      self.couplingFaces_.append( 
        map2dTo3d.MustDownCast( couplingFace.clone()  ) 
      )
    self.nBoundaryLayers_ = nBoundaryLayers
    self.nElementsSpanwise_ = nElementsSpanwise
    self.nElementsNormal_ = nElementsNormal
    self.firstElementSizeHubToShroud_ = firstElementSizeHubToShroud
    self.firstElementSizeNormalBlade_ = firstElementSizeNormalBlade
    self.bladeHubElementSize_ = bladeHubElementSize
    self.bladeHubElementScale_ = bladeHubElementScale
    self.bladeShroudElementSize_ = bladeShroudElementSize
    self.bladeShroudElementScale_ = bladeShroudElementScale
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
    self.blade_.setLabel( "blade" )
    cc = 0
    for couplingFace in self.couplingFaces_:
      couplingFace.setLabel("coupling_"+str(cc))
      cc = cc + 1

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
    # add hub, shroud, channel and coupling faces
    #
    hubId = m3dGmsh.getModel().addIfFaceToGmshModel( self.hub_ )
    shroudId = m3dGmsh.getModel().addIfFaceToGmshModel( self.shroud_ )
    channelId = m3dGmsh.getModel().addIfRegionToGmshModel( self.channel_ )
    for couplingFace in self.couplingFaces_:
      fid = m3dGmsh.getModel().addIfFaceToGmshModel( couplingFace )
      m3dGmsh.getModel().getDtGmshRegionByTag( channelId ).addFace( fid, 1 )
  
    #
    # detect edges that lie in hub and shroud
    #
    hubEdges, shroudEdges = self.extractEdgesInFirstAndSecond(
      m3dGmsh.getModel(), self.couplingFaces_, self.hub_, self.shroud_
    )

    #
    # add hub and shroud edges as internal line loop to hub and shroud
    #
    m3dGmsh.getModel().getDtGmshFaceByTag( hubId ).addEdgeLoop( hubEdges )
    m3dGmsh.getModel().getDtGmshFaceByTag( shroudId ).addEdgeLoop( shroudEdges )

    #
    # add mesh blocks
    #
    for block in self.blocks_:
      rid = m3dGmsh.getModel().addIfRegionToGmshModel(block)
      m3dGmsh.getModel().getDtGmshRegionByTag( rid ).meshTransfiniteRecursive()
      m3dGmsh.getModel().getDtGmshRegionByTag( rid ).meshWNElements(1,1,1)
      m3dGmsh.getModel().getDtGmshRegionByTag( rid ).meshRecombineRecursive()
   
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
    aG.push_back( self.blade_ )
    for couplingFace in self.couplingFaces_:
      aG.push_back( couplingFace )

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
            '{"label" : "pressure"},'
            '{"label" : "blade"},'
            '{"labels" : "coupling_*"}'
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
    bladeLines = \
      m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical( 
        "*blade*", "*blade*" 
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
    bladeToHubLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical( 
      "*blade*", "*hub*" 
    )
    bladeToShroudLines = \
      m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical( 
        "*blade*", "*shroud*" 
      )

    bladeHubLines = set(bladeLines)&set(hubLines)
    bladeShroudLines = set(bladeLines)&set(shroudLines)
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
    bladeToHubLines = \
      set(bladeToHubLines)-set(-np.array(bladeLines))-set(bladeLines)
    bladeToShroudLines = \
      set(bladeToShroudLines)-set(-np.array(bladeLines))-set(bladeLines)

    #
    # add debug faces and lines
    #
    if self.debug():
      for face in [
        self.hub_, self.shroud_, 
        self.inlet_, self.outlet_, 
        self.suction_, self.pressure_,
      ] + self.couplingFaces_:
        self.appendAnalyticGeometry( 
          face, "debug_"+self.label_+"_"+face.getLabel() 
        )

      for lab, lines in zip(
        [
          "bladeHubLines", 
          "bladeShroudLines", 
          "inletHubToShroudLines", 
          "outletHubToShroudLines", 
          "pressureHubToShroudLines", 
          "suctionHubToShroudLines", 
          "bladeToHubLines", 
          "bladeToShroudLines", 
        ], 
        [
          bladeHubLines, 
          bladeShroudLines, 
          inletHubToShroudLines, 
          outletHubToShroudLines, 
          pressureHubToShroudLines, 
          suctionHubToShroudLines, 
          bladeToHubLines, 
          bladeToShroudLines, 
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
    # blade normal
    gradings = self.addGrading(
      gradings,
      scaTanhGradingOneDCompound(
        scaTanhGradingOneD(
          vectorReal([1.0, -1.0, 1.0, -1.0]),
          1.0,
          0.1, 5.0
        )
      ),
      "normalBlade",
      m3dGmsh,
      self.firstElementSizeNormalBlade_
    )

    bladeHubShroudDirection = self.boundaryEdgeDirection(
      m3dGmsh.getModel(), 
      [
        [self.blade_, bladeHubLines],
        [self.blade_, bladeShroudLines],
      ]
    )
    logging.info("bladeHubShroudDirection = %d" % bladeHubShroudDirection)

    #
    # specify number of elements
    #
    for lines in [ 
      hubToShroudLines, inletHubToShroudLines, outletHubToShroudLines, 
    ]:
      for line in lines:
        theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
        theEdge.setNElements( self.nElementsSpanwise_ )
        theEdge.setGrading( 1.0, gradings["hubToShroud"][0] )
    
    for lines, elementSize, elementScale, outStr in zip(
      [bladeShroudLines, bladeHubLines,],
      [self.bladeShroudElementSize_, self.bladeHubElementSize_,],
      [self.bladeShroudElementScale_, self.bladeHubElementScale_,],
      ["BladeShroud", "BladeHub",],
    ):
      direction = bladeHubShroudDirection
      for line in lines:
        theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
        if elementSize!=None:
          eL = theEdge.getMap1dTo3d().length()
          v0 = theEdge.getMap1dTo3d().getPointPercent(0.0)
          v1 = theEdge.getMap1dTo3d().getPointPercent(1.0)
          p_0_uv = self.blade_.reparamPercentOnFace( v0 )
          p_1_uv = self.blade_.reparamPercentOnFace( v1 )
          ms_0 = elementSize( p_0_uv[direction] )[0]
          ms_1 = elementSize( p_1_uv[direction] )[0]
          nE_0 = math.ceil(eL/ms_0) 
          nE_1 = math.ceil(eL/ms_1) 
          nE = math.ceil( 
            min(nE_0,nE_1) + elementScale * abs(nE_1 - nE_0) 
          )
       
          logging.info(
              "\n  %s Edge %d / length %f "
              "\n    p_uv: (%f, %f) -> (%f, %f) "
              "\n    meshLength: (%f) -> (%f) "
              "\n    nElements (%f -> %d) -> (%f -> %d) => %d" 
            % 
            (
              outStr,
              line,
              eL,
              p_0_uv[0], p_0_uv[1], 
              p_1_uv[0], p_1_uv[1], 
              ms_0, ms_1,
              eL/ms_0, nE_0, eL/ms_1, nE_1, nE
            )
          )
       
          theEdge.meshTransfiniteWNElements( 1, 1.0, nE )
          gradings = self.addGrading(
            gradings,
            sca3PPointsBSplineOneDCompound(
              sca3PPointsBSplineOneD(
                1./nE, ms_0 / eL,
                1.-1./nE, (eL-ms_1) / eL,
              )
            ),
            "tangentialBlade_"+str(line),
            m3dGmsh,
            ms_0,
            ms_1
          ) 
          theEdge.setGrading( 1.0, gradings["tangentialBlade_"+str(line)][0] )
        else:
          theEdge.meshTransfiniteWNElements( 1, 1.0, 5 )
   
    for lines in [ bladeToHubLines, bladeToShroudLines, ]:
      for line in lines:
        theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
        theEdge.setNElements( self.nElementsNormal_ )
        theEdge.setGrading( 1.0, gradings["normalBlade"][0] )

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

    #
    # check boundary layer direction
    #
    boundaryLayerDir = self.detectBoundaryLayerDirection( 
      m3dGmsh.getModel(), 
      [ 
        [self.inlet_, inletHubToShroudLines], 
        [self.outlet_, outletHubToShroudLines],
        [self.pressure_, pressureHubToShroudLines],
        [self.suction_, suctionHubToShroudLines],
      ]
    )

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
            '"dtMeshGFaceWithTransfiniteLayer(*inlet*)",'
            '"dtMeshGFaceWithTransfiniteLayer(*outlet*)",'
            '"dtMeshGFaceWithTransfiniteLayer(*suction*)",'
            '"dtMeshGFaceWithTransfiniteLayer(*pressure*)",'
            '"dtMeshTransfiniteGFace(*)",'
            '"dtMeshGFace(*)"'
          '],'
          '"_rule3D" : ['
            '"dtMeshGRegionWithBoundaryLayer(R_0)",'
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
            '},'
            '{'
              '"option" : ['
                '{"name" : "debug", "value" : "true"}'
              '],'
              '"name" : "dtMeshGRegionWithBoundaryLayer",'
              '"label" : "dtMeshGRegionWithBoundaryLayer",'
#              '"_nSpacingSteps" : ['
#                +str(self.nBoundaryLayers_-1)+', '
#                +str(self.nBoundaryLayers_-1)+
#              '],'
              '"_nSpacingSteps" '
                ': '+str(
                  [-(self.nBoundaryLayers_-1), -(self.nBoundaryLayers_-1)]
                )+','
              '"_nNormalSmoothingSteps" : 1000,'
              '"_nGrowingSmoothingSteps" : 1000,'
              '"_maxGrowingRatePerStep" : 1.10,'
              '"_maxDihedralAngle" : '+str(float(np.pi*360./180.))+','
              '"_faceLabel" : ['
                '"R_0->F_0", "R_0->F_1"'
              '],"'
              '_slidableFaceLabel" : ['
                '"*inlet*", "*outlet*", "*pressure*", "*suction*", "*coupling*"'
              '],'
              '"dtMesh3DOperator" : "dtMeshGRegion",'
              '"_fixedFaceLabel" : ['
              ']'
          '},'
            +jsonPrimitive()\
              .appendStr("name", "dtMeshGFaceWithTransfiniteLayer")\
              .appendStr("label", "dtMeshGFaceWithTransfiniteLayer")\
              .appendVectorInt("_nLayers", 
                vectorInt(
                  [
                    self.nBoundaryLayers_,
                    self.nBoundaryLayers_
                  ]
                )
              )\
              .appendInt("_direction", boundaryLayerDir)\
              .appendInt("_nSmooth", 3)\
              .toStdString()+
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
            '":*blade*::'+self.label_+'_blade:",'
            '":*inlet*::'+self.label_+'_inlet:",'
            '":*outlet*::'+self.label_+'_outlet:",'
            '":*suction*::'+self.label_+'_suction:",'
            '":*pressure*::'+self.label_+'_pressure:",'
            '":*coupling*:::"'
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

  @staticmethod
  def extractEdgesInFirstAndSecond(
    theModel: dtGmshModel, 
    faces: List[map2dTo3d], 
    first: map2dTo3d, 
    second: map2dTo3d
  ) -> Tuple[List[int], List[int]]:
    firstEdges = []
    secondEdges = []
    for face in faces:
      for edge in [
        face.segmentConstUPercent(0.0),
        face.segmentConstVPercent(1.0),
        face.segmentConstUPercent(1.0),
        face.segmentConstVPercent(0.0),
      ]:
        if bool_map1dTo3dInMap2dTo3d(edge, first).result():
          firstEdges.append( 
            theModel.addIfEdgeToGmshModel( edge )
          ) 
          logging.debug("Detect first edge tag = %d" % firstEdges[-1] )
        elif bool_map1dTo3dInMap2dTo3d(edge, second).result():
          secondEdges.append( 
            theModel.addIfEdgeToGmshModel( edge )
          )
          logging.debug("Detect second edge tag = %d" % secondEdges[-1])
    logging.info(
      "firstEdges = %s, secondEdges = %s" 
      % 
      (
        str(firstEdges), str(secondEdges)
      ) 
    )
    return firstEdges, secondEdges

  @staticmethod
  def boundaryEdgeDirection( 
    theModel: dtGmshModel, 
    boundaryLayerDirCheck: List[ List[ Union[ map2dTo3d, List[int] ] ] ]
  ) -> int:
    boundaryLayerDir = []
    for faceLines in boundaryLayerDirCheck:
      for line in faceLines[1]:
        theEdge = theModel.getDtGmshEdgeByTag( line )
        p0_uv = faceLines[0].reparamPercentOnFace( 
          theEdge.getMap1dTo3d().getPointPercent(0.0)
        )
        p1_uv = faceLines[0].reparamPercentOnFace(
          theEdge.getMap1dTo3d().getPointPercent(1.0)
        )
        boundaryLayerDirT = 0
        if ( not analyticGeometry.inUVWTolerance( p0_uv.x(), p1_uv.x() ) ):
          boundaryLayerDirT = boundaryLayerDirT + 1
        if ( not analyticGeometry.inUVWTolerance( p0_uv.y(), p1_uv.y() ) ):
          boundaryLayerDirT = boundaryLayerDirT + 2
        logging.debug("p0_uv = (%f, %f)" % (p0_uv.x(), p0_uv.y()))
        logging.debug("p1_uv = (%f, %f)" % (p1_uv.x(), p1_uv.y()))
        logging.debug("boundaryLayerDirT = %d" % (boundaryLayerDirT))
        boundaryLayerDir.append( boundaryLayerDirT )
      boundaryLayerDir = list(dict.fromkeys(boundaryLayerDir))
    if len(boundaryLayerDir)!=1:
      raise ValueError("BoundaryLayerDirection is not equal in all faces.")
    boundaryLayerDir = boundaryLayerDir[0]
    logging.info("boundaryLayerDir = %d" % boundaryLayerDir)

    if boundaryLayerDir==1:
      return 0
    elif boundaryLayerDir ==2:
      return 1
    else:
      raise ValueError("Direction should be 1 or 2.")

  @staticmethod
  def detectBoundaryLayerDirection( 
    theModel: dtGmshModel, 
    boundaryLayerDirCheck: List[ List[ Union[ map2dTo3d, List[int] ] ] ]
  ) -> int:
    boundaryLayerDir = \
      map3dTo3dGmsh_gridFromChannelAndBlocks.boundaryEdgeDirection(
        theModel, boundaryLayerDirCheck
      )
    # (1,0)[boundaryLayerDir==1])
    if boundaryLayerDir==0:
      return 1
    elif boundaryLayerDir ==1:
      return 0
    else:
      return -1

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
