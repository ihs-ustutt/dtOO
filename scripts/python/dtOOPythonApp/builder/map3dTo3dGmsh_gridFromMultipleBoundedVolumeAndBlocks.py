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
from dtOOPythonSWIG import reverse
from dtOOPythonSWIG import multipleBoundedSurface 
from dtOOPythonSWIG import bVODumpModel

import numpy as np
from typing import List, Tuple, Union, Dict
import logging
import math

class map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks(dtBundleBuilder):
    """Create mesh's topology as map3dTo3dGmsh.

    The topology of the mesh is build from the channel as a multiple bounded 
    volume (MBV) and all mesh blocks that surround the blade. 
    The mesh block surfaces that connect the mesh blocks with the channel are
    treated as coupling surfaces. The mesh block surfaces which dont connect to the
    channel, the blade or hub and shroud are part of the suction boundary and are 
    treated as block faces.
    On the hub and shroud a prismatic boundary layer with nBoundaryLayers_ is created.

    The numbering of the mesh regions is as follows:

        - R_0    : channel

        - R_1     : First mesh block

        - ...

        - R_(N-1) : (N-1)^th mesh block

        - R_(N) : (N)^th mesh block

    All mesh blocks are meshed as transfinite region in addition with 
    recombining recursively. Topologies' faces are labelled as they are defined.
    The periodic faces are built by rotating the suction boundary faces, which
    are in turn defined by the number of mesh blocks nMeanplaneBlocks used for 
    the generation in the meanplane, plus two faces extending from the mesh blocks 
    towards the mesh channel inlet and outlet.
    The resulting faces extending to the inlet and outlet and their periodic 
    counterparts are meshed unstructured and labeled with "*_tri_*". The blockfaces 
    are meshed transfinite and labeled with "block_*" and their periodic counterparts 
    with "*_quad_*".
    This means within the topology the following faces are available:

        - hub_0 ... hub_(N)

        - shroud_0 ... shroud_(N)

        - inlet_0

        - outlet_0

        - suction_tri_0, suction_tri_1, suction_tri_(nMeanplaneBlocks+3), suction_tri_(nMeanplaneBlocks+4)

        - block_0 ... block_(nMeanplaneBlocks)

        - pressure_tri_0, pressure_tri_1, pressure_tri_(nMeanplaneBlocks+3), pressure_tri_(nMeanplaneBlocks+4)
        
        - pressure_quad_2 ... pressure_quad_(nMeanplaneBlocks+2)

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

        - bladeToBlockLines: edges that share one vertex with blade and one with 
          block or coupling faces

        - bladeHubLines: edges that share both vertices with blade and hub

        - bladeShroudLines: edges that share both vertices with blade and shroud

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
    channel_: multipleBoundedVolume
      Channel.
    channelFaces_: List[ map2dTo3d ]
      List of bounding faces surrounding the channel
    blocks_: List[ map3dTo3d ]
      List of mesh blocks surrounding the blade.
    nMeanplaneBlocks_: int 
      number of block faces which are part of the meanplane
    blade_: map2dTo3d
      Blade.
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
    meshTEBlocks_: Bool
      marker if trailing edge mesh blocks should be meshed
    map3dTo3dGmshJson_: jsonPrimitive
      JSON structure for map3dTo3dGmsh.

    Examples
    --------

    """
    def __init__( 
        self, 
        label: str, 
        channel: analyticGeometry,
        channelFaces: List[analyticGeometry],
        blocks: List[analyticGeometry],
        nMeanplaneBlocks: int,
        blade: analyticGeometry,
        nBoundaryLayers: int,
        nElementsSpanwise: int,
        nElementsNormal: int,
        firstElementSizeHubToShroud: float,
        firstElementSizeNormalBlade: float,
        bladeHubElementSize: scaOneD = None,
        bladeHubElementScale: float = None,
        bladeShroudElementSize: scaOneD = None,
        bladeShroudElementScale: float = None,
        charLengthMin: float = 0.05,
        charLengthMax: float = 0.10,
        meshTEBlocks: bool = False
    ) -> None:

        """
        Parameters 
        ----------
        label: str
          Label.
        channel: multipleBoundedVolume
          Channel.
        channelFaces: List[ map2dTo3d ]
          List of bounding faces surrounding the channel
        blocks: List[ map3dTo3d ]
          List of mesh blocks surrounding the blade.
        nMeanplaneBlocks: int
          number of block faces which are part of the meanplane
        blade: map2dTo3d
          Blade.
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
        charLengthMin: float
          Sets gmsh attribute `Mesh.CharacteristicLengthMin`.
        charLengthMax: float
          Sets gmsh attribute `Mesh.CharacteristicLengthMax`.
        meshTEBlocks: Bool
          marker if trailing edge mesh blocks should be meshed
        """
        logging.info( "Initializing %s ..." % (label) )
        super(map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks, self).__init__()
        
        self.label_ = label
        self.channel_ =  channel.clone() 
        self.channelFaces_ = channelFaces
        
        self.blocks_ = []
        for block in blocks:
            self.blocks_.append( map3dTo3d.MustDownCast(block.clone()) )
        
        self.nMeanplaneBlocks_ = nMeanplaneBlocks
        self.blade_ = map2dTo3d.MustDownCast( blade.clone() )

        self.nBoundaryLayers_ = nBoundaryLayers
        self.nElementsSpanwise_ = nElementsSpanwise
        self.nElementsNormal_ = nElementsNormal
        self.firstElementSizeHubToShroud_ = firstElementSizeHubToShroud
        self.firstElementSizeNormalBlade_ = firstElementSizeNormalBlade
        self.bladeHubElementSize_ = bladeHubElementSize
        self.bladeHubElementScale_ = bladeHubElementScale
        self.bladeShroudElementSize_ = bladeShroudElementSize
        self.bladeShroudElementScale_ = bladeShroudElementScale
        
        self.meshTEBlocks_ = meshTEBlocks

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
        # create lvh
        #
        aG = labeledVectorHandlingAnalyticGeometry()   
        
        #
        # add channel (multiple bounded volume (MBV))
        #
        channelId = m3dGmsh.getModel().addIfToGmshModel( self.channel_ )
        
        #
        # add bounding faces of the multiple bounded volume
        #
        for face in self.channelFaces_:
            ## coupling faces were addes as blockFaces in previous loop
            #if not face.getLabel().startswith("coupling_"):
                
            # not multi bounded faces
            #  those are: inlet, outlet suction, pressure and coupling faces
            if multipleBoundedSurface.ConstDownCast(face) == None:
                aG.push_back(face.clone())
            # multi bounded faces
            #  those are: hub and shroud faces
            else:
                # surfaceConstPtr returns the rectangular bounding box in which the mbs was created
                scp = multipleBoundedSurface.MustDownCast(face).surfaceConstPtr()
                scp.setLabel(face.getLabel())
                aG.push_back(scp.clone())
        
        #
        # add mesh blocks
        #
        for i, block in enumerate(self.blocks_):
            
            # find face on blade and block (parallel to blade)
            bladeFace, blockFace = self.detectFirstAndSecond(block, 3)
            
            if self.meshTEBlocks_ == True: 
                # bladeFace at i == 1 and i == len(self.blocks_)-1 are the same
                #  when trailing edge blocks were build.
                # this face can only be added once
                if i != 0: #and i < len(self.blocks_)-1:
                    aG.push_back(bladeFace << "blade_"+str(i))
            else:
                aG.push_back(bladeFace << "blade_"+str(i))

            # only the block faces which are part of the meanplane are pushed
            if i <= self.nMeanplaneBlocks_:
                aG.push_back(blockFace << "block_"+str(i))
            
            # add block volumes, those are 6-sided and meshed transfinite
            rid = m3dGmsh.getModel().addIfRegionToGmshModel(block)
            m3dGmsh.getModel().getDtGmshRegionByTag( rid ).meshTransfiniteRecursive()
            m3dGmsh.getModel().getDtGmshRegionByTag( rid ).meshWNElements(1,1,1)
            m3dGmsh.getModel().getDtGmshRegionByTag( rid ).meshRecombineRecursive()
        
        #
        # name regions
        #
        #  - R_0    : channel
        #  - R_1     : First mesh block
        #  - ...
        #  - R_(N-1) : (N-1)^th mesh block
        #  - R_(N) : (N)^th mesh block
        ob = bVONameRegions()
        ob.jInit( jsonPrimitive('{ "_regionLabel" : [] }'), m3dGmsh )
        ob.preUpdate()

        #
        # trailing block edges extending in flow direction 
        #  from the blade, the first and the last block
        #
        if self.meshTEBlocks_ == True:
            # faces from hub to shroud
            #   first block
            tEBlade, tEBlock0 = self.detectFirstAndSecond(
              self.blocks_[0], 3
            )
            #   last block
            tEBlade, tEBlock1 = self.detectFirstAndSecond(
              self.blocks_[-1], 3
            )
            # edges on hub and shroud
            #   trailing edge
            tEHub, tEShroud = self.extractEdgesInFirstAndSecond(
              m3dGmsh.getModel(), 
              [tEBlade], 
              map2dTo3d.MustDownCast(aG["hub"]), 
              map2dTo3d.MustDownCast(aG["shroud"])
            )
            #   first block
            tEBlock0_Hub, tEBlock0_Shroud = self.extractEdgesInFirstAndSecond(
              m3dGmsh.getModel(), 
              [tEBlock0], 
              map2dTo3d.MustDownCast(aG["hub"]), 
              map2dTo3d.MustDownCast(aG["shroud"])
            )
            #   last block
            tEBlock1_Hub, tEBlock1_Shroud = self.extractEdgesInFirstAndSecond(
              m3dGmsh.getModel(), 
              [tEBlock1], 
              map2dTo3d.MustDownCast(aG["hub"]), 
              map2dTo3d.MustDownCast(aG["shroud"])
            )
            # create a mesh list containing the edges and a float 
            #  >/<0 for grading directions
            tEMeshList = [
                    [[tEHub, tEShroud], -1.0],
                    [[tEBlock0_Hub, tEBlock0_Shroud], -1.0],
                    [[tEBlock1_Hub, tEBlock1_Shroud], 1.0]
                ]  
        
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
                '{"labels" : "suction_*"},'
                '{"labels" : "pressure_*"},'
                '{"labels" : "blade_*"},'
                '{"labels" : "block_*"},'
                '{"labels" : "coupling_*"}'
              '],'
              '"_inc" : 10.0,'
              '"_facesPerEntry" : []'
            '}'
          ), 
          None, None, None, aG, None, m3dGmsh 
        )
        ob.preUpdate()
         
        #
        # Managing Faces and Edges
        #
        
        # untagging the face between the first and last TE block from the blade faces
        #  this face was needed to get all the bladeToBlockLines 
        bladeFaces = m3dGmsh.getModel().getDtGmshFaceListByPhysical("*blade*")
        l = len(bladeFaces)
        m3dGmsh.getModel().untagPhysical(m3dGmsh.getModel().getDtGmshFaceByPhysical("*blade_"+str(l)+"*"))
        
        # organizing periodic faces
        #  the order is set by adding the faces seqentially and the naming
        periodic_pressure = m3dGmsh.getModel().getDtGmshFaceListByPhysical("*pressure_*")
        
        # suction faces contain the inlet and outlet faces and the block faces
        #  there are two suction faces ant inlet and outlet each
        periodic_suction = []
        periodic_suction.append(m3dGmsh.getModel().getDtGmshFaceByPhysical("*suction_tri_0*"))
        periodic_suction.append(m3dGmsh.getModel().getDtGmshFaceByPhysical("*suction_tri_1*"))
        periodic_block = m3dGmsh.getModel().getDtGmshFaceListByPhysical("*block_*")
        for face in periodic_block:
            periodic_suction.append(face)
        l = len(periodic_suction)
        periodic_suction.append(m3dGmsh.getModel().getDtGmshFaceByPhysical("*suction_tri_"+str(l)+"*"))
        periodic_suction.append(m3dGmsh.getModel().getDtGmshFaceByPhysical("*suction_tri_"+str(l+1)+"*"))
        
        # list with periodic faces
        periodics = list(zip(periodic_suction, periodic_pressure))
        
        # all hub to shroud lines
        hubToShroudLines = \
            m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*hub*", "*shroud*"
            )
        
        # unstructured faces and their lines, they will later get a boundary layer
        # structure of the resulting list:
        #  unstrFacesAndh2sLines = List[List[face0, List[edges0]]]
        unstrFacesAndh2sLines = []
        for i, face in enumerate(aG):
            # faces with boundary layer
            if face.getLabel().startswith(
                    ("inlet", "outlet", "suction_tri_", "pressure_tri_")
                ):
                
                f = map2dTo3d.MustDownCast(face)
                # getting all edges on the face
                faceLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                    "*"+face.getLabel()+"*", "*"+face.getLabel()+"*"
                )
                # only keeping the ones extending from hub to shroud
                hsLines = (
                    set(faceLines)&set(hubToShroudLines)
                  ).union(
                    set(faceLines)&set(-np.array(hubToShroudLines))
                  )

                unstrFacesAndh2sLines.append([f, hsLines])
        
        # getting other relevant edges
        hubLines = \
            m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*hub*", "*hub*"
            )
        shroudLines = \
            m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*shroud*", "*shroud*"
            )
        hAndSLines = set(hubLines).union(set(shroudLines))
        
        bladeToBlockLines_0 = \
            m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*blade_*", "*block_*"
            )
        bladeToBlockLines_1 = \
            m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*blade_*", "*coupling_*"
            )
        bladeToBlockLines = set(bladeToBlockLines_0).union(set(bladeToBlockLines_1))
        bladeToBlockLines = set(bladeToBlockLines)&set(hAndSLines)

        bladeLines = \
            m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                "*blade_*", "*blade_*"
            )

        bladeHubLines = set(bladeLines)&set(hubLines)
        bladeShroudLines = set(bladeLines)&set(shroudLines)
        
        # trailing edge hub and shroud edges have to be removed here if
        #  trailing edge mesh blocks exist
        if self.meshTEBlocks_ == True:
            bladeHubLines = \
              set(bladeHubLines)-set(-np.array(tEHub))-set(tEHub)
            bladeShroudLines = \
              set(bladeShroudLines)-set(-np.array(tEShroud))-set(tEShroud)
            
            ## untagging the face between the first and last TE block from the blade faces
            ##  this face was needed to get all the bladeToBlockLines 
            #bladeFaces = m3dGmsh.getModel().getDtGmshFaceListByPhysical("*blade*")
            #l = len(bladeFaces)
            #m3dGmsh.getModel().untagPhysical(m3dGmsh.getModel().getDtGmshFaceByPhysical("*blade_"+str(l)+"*"))
        
        #
        # add debug faces and lines
        #
        
        if self.debug():
            
            # curves for visualisation purposes only
            #tEList = [tEHub[0], tEShroud[0], tEBlock0_Hub[0], tEBlock0_Shroud[0], tEBlock1_Hub[0], tEBlock1_Shroud[0]]

            #otherHubLines = set(hubLines) \
            #    - set([-x for x in bladeHubLines]) \
            #    - set(bladeHubLines)
            #otherHubLines = set(otherHubLines) \
            #    - set([-x for x in bladeToBlockLines]) \
            #    - set(bladeToBlockLines)
            #otherHubLines = set(otherHubLines) \
            #    - set([-x for x in tEList]) \
            #    - set(tEList)
            #
            #otherShroudLines = set(shroudLines) \
            #    - set([-x for x in bladeShroudLines]) \
            #    - set(bladeShroudLines)
            #otherShroudLines = set(otherShroudLines) \
            #    - set([-x for x in bladeToBlockLines]) \
            #    - set(bladeToBlockLines)
            #otherShroudLines = set(otherShroudLines) \
            #    - set([-x for x in tEList]) \
            #    - set(tEList)
            
            # all faces
            for faceLabel in [
                  "*hub*", "*shroud*", 
                  "*inlet*", "*outlet*", 
                  "*suction_*", "*pressure_*", 
                  "*blade_*", "*block_*",
                  "*coupling_*"
                ]:
                
                faces = m3dGmsh.getModel().getDtGmshFaceListByPhysical(faceLabel)
                for face in faces:
                    self.appendAnalyticGeometry(
                        face.getMap2dTo3d(), 
                        "debug_faceAll_"+self.label_+"_"+face.getPhysicalString()
                    )
            # slidable faces for boundary layer meshing of MBV
            for faceLabel in [
                "*inlet*", "*outlet*", "*pressure_*", "*suction_*", "*coupling_*"
                ]:
                
                faces = m3dGmsh.getModel().getDtGmshFaceListByPhysical(faceLabel)
                for face in faces:
                    self.appendAnalyticGeometry(
                        face.getMap2dTo3d(), 
                        "debug_faceMBV_"+self.label_+"_"+face.getPhysicalString()
                    )
            # lines to which element numbers and gradings are assigned 
            for lab, lines in zip(
                [
                  "hubToShroudLines",
                  "bladeToBlockLines",
                  "bladeHubLines",
                  #"TrailingEdgeLines",
                  "bladeShroudLines",
                  #"otherHubLines",
                  #"otherShroudLines"
                ],
                [ 
                  hubToShroudLines,
                  bladeToBlockLines,
                  bladeHubLines,
                  #tEList,
                  bladeShroudLines,
                  #otherHubLines,
                  #otherShroudLines
                ]
            ):
                for ii in lines:
                  self.appendAnalyticGeometry(
                    m3dGmsh.getModel().getDtGmshEdgeByTag( ii ).getMap1dTo3d(),
                    "debug_lineNE_"+self.label_+"_"+lab+"_"+str(ii)
                  )
            # faces and lines which will be meshed unstructured with boundary layer
            for faceAndLines in unstrFacesAndh2sLines:

                self.appendAnalyticGeometry(
                        faceAndLines[0],
                        "debug_faceUnstr_"+self.label_+"_"+faceAndLines[0].getLabel()
                    )
                for i, line in enumerate(faceAndLines[1]):
                    self.appendAnalyticGeometry(
                        m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                        "debug_lineUnstr_"+self.label_+"_"+faceAndLines[0].getLabel()+"_"+str(i)
                    )
            # periodic boundary faces
            for i, periodic in enumerate(periodics):
                self.appendAnalyticGeometry(
                    periodic[0].getMap2dTo3d(),
                    "debug_periodic0_"+str(i)+"_"+periodic[0].getPhysicalString()
                )
                self.appendAnalyticGeometry(
                    periodic[1].getMap2dTo3d(),
                    "debug_periodic1_"+str(i)+"_"+periodic[1].getPhysicalString()
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
        
        # getting blade direction for grading along the blade
        bladeHubShroudDirection = self.boundaryEdgeDirection(
          m3dGmsh.getModel(),
          [
            [self.blade_, bladeHubLines],
            [self.blade_, bladeShroudLines],
          ]
        )
        logging.info("bladeHubShroudDirection = %d" % bladeHubShroudDirection)

        #
        # mesh settings
        #
        
        # hub to shroud
        for line in hubToShroudLines:
            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            theEdge.setNElements( self.nElementsSpanwise_ )
            theEdge.setGrading( 1.0, gradings["hubToShroud"][0] )
        
        # along blade
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
        
        # from blade to block
        for line in bladeToBlockLines:
            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            theEdge.setNElements( self.nElementsNormal_ )
            theEdge.setGrading( 1.0, gradings["normalBlade"][0] )
        
        # makes mesh settings for Trailing edge mesh blocks
        if self.meshTEBlocks_ == True:

            # meshing trailing edge lines, tEMeshList has the following format:
            #   tEMeshList = [
            #        [[[tEHub], [tEShroud]], -1.0],
            #        ...
            #    ]
            for meshList in tEMeshList:
                for edgeTag in meshList[0]:
                    edge = m3dGmsh.getModel().getDtGmshEdgeByTag(edgeTag[0])
                    edge.setNElements( self.nElementsNormal_ )
                    #edge.setGrading( meshList[1], gradings["normalBlade"][0] )
        
        #
        # add observers
        #

        # read mesh
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
        
        # odding observers for all periodic faces
        for periodic in periodics:
            ob = bVOSetRotationalPeriodicity()
            ob.jInit(
              jsonPrimitive()\
                .appendStr("_faceMaster", periodic[0].getPhysicalString())\
                .appendStr("_faceSlave", periodic[1].getPhysicalString())\
                .appendDtTransformer( "_dtT", theT ), 
              bC, None, None, None, None, m3dGmsh 
            )
            ob.preUpdate()
        
        # dump model information
        ob = bVODumpModel()
        ob.thisown = False
        ob.jInit(
          jsonPrimitive(),
          None, None, None, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
        
        # getting boundary layer direction for unstructured faces
        #  with transfinite layers
        boundaryLayerDir = self.detectBoundaryLayerDirection(
          m3dGmsh.getModel(),
          unstrFacesAndh2sLines
        )
        #
        # setting mesh rules
        #
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
                '"dtMeshGFaceWithTransfiniteLayer(*suction_tri_*)",'
                '"dtMeshGFaceWithTransfiniteLayer(*pressure_tri_*)",'
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
                  #'"_nSpacingSteps" : ['
                  #  +str(self.nBoundaryLayers_-1)+', '
                  #  +str(self.nBoundaryLayers_-1)+
                  #'],'
                  '"_nSpacingSteps" '
                    ': '+str(
                      [-(self.nBoundaryLayers_-1), -(self.nBoundaryLayers_-1)]
                    )+','
                  '"_nNormalSmoothingSteps" : 1000,'
                  '"_nGrowingSmoothingSteps" : 1000,'
                  '"_maxGrowingRatePerStep" : 1.10,'
                  '"_maxDihedralAngle" : '+str(float(np.pi*360./180.))+','
                  '"_faceLabel" : ['
                    '"hub_0", "shroud_0"'
                  '],"'
                  '_slidableFaceLabel" : ['
                    '"*inlet*", "*outlet*", "*pressure_*", "*suction_*", "*coupling_*"'
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
        
        # face to patch rule
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
                '":*block*::'+self.label_+'_suction:",'
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

        # write mesh
        ob = bVOWriteMSH()
        ob.thisown = False
        ob.jInit(
          jsonPrimitive('{"_filename" : "", "_saveAll" : true}'),
          None, None, None, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
        #ob.postUpdate()
        
        # orient volumes
        ob = bVOOrientCellVolumes()
        ob.thisown = False
        ob.jInit(
            jsonPrimitive('{"_positive" : true}'), m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)

        m3dGmsh.thisown = False

        self.appendBoundedVolume(m3dGmsh)

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
            logging.debug("face: %s" % faceLines[0].getLabel())
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
          map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks.boundaryEdgeDirection(
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
