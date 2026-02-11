#import dtOOPythonSWIG as dtOO
from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import jsonPrimitive
from dtOOPythonSWIG import staticPropertiesHandler
from dtOOPythonSWIG import dtBundle
from dtOOPythonSWIG import lVHOstateHandler
from dtOOPythonSWIG import bVOMeshRule
from dtOOPythonSWIG import map2dTo3d
from dtOOPythonSWIG import map3dTo3d
from dtOOPythonSWIG import map3dTo3dGmsh
from dtOOPythonSWIG import jsonPrimitive
from dtOOPythonSWIG import bVOWriteMSH
from dtOOPythonSWIG import bVONameRegions
from dtOOPythonSWIG import bVOAnalyticGeometryToFace
from dtOOPythonSWIG import labeledVectorHandlingAnalyticGeometry
from dtOOPythonSWIG import vectorReal
from dtOOPythonSWIG import scaTanhGradingOneD
from dtOOPythonSWIG import scaTanhGradingOneDCompound
from dtOOPythonSWIG import bVOSetPrescribedElementSize

import logging
import numpy
from typing import List, Tuple, Union, Dict

class map3dTo3dGmsh_gridFromLayers (dtBundleBuilder):

    def __init__(self,
                 label,
                 layers: List,
                 nLayers: int,
                 firstElement: float,
                 elementSize_sw,
                 elementSize_circ,
                 mv):

        super(map3dTo3dGmsh_gridFromLayers, self).__init__()

        logging.info( "Building %s ..." % ("meshLayers") )
        
        # setting global params
        self.label_ = label
        self.layerList_ = layers
        self.nLayers_ = nLayers
        self.firstElement_ = firstElement
        self.elementSizeSW_ = elementSize_sw
        self.elementSizeCIRC_ = elementSize_circ
        self.unstructured_ = mv

        #logMe.initLog('layerMesh.log')
        
        # setting up volume
        self.map3dTo3dGmshJson_ = jsonPrimitive(
            '{"label" : "'+self.label_+'",'
                '"option" : ['
                '{"name" : "[gmsh]General.Terminal", "value" : "1."},'
                '{"name" : "[gmsh]General.Verbosity", "value" : "100."},'
                '{"name" : "[gmsh]General.ExpertMode", "value" : "1."},'
                '{"name" : "[gmsh]Mesh.CharacteristicLengthMin", "value" : "0.05"},'
                '{"name" : "[gmsh]Mesh.CharacteristicLengthMax", "value" : "0.1"},'
                '{"name" : "[gmsh]Mesh.Algorithm", "value" : "1"},'
                '{"name" : "[gmsh]Mesh.MeshSizeExtendFromBoundary", "value" : "1"},'
                '{"name" : "[gmsh]Mesh.MeshSizeFromPoints", "value" : "1"}'
              '],'
              '"analyticGeometry" : []'
            '}'
        )

    def buildLayerMesh(self) -> None:

        m3dGmsh = map3dTo3dGmsh()
        m3dGmsh.jInit(
          self.map3dTo3dGmshJson_, None, None, None, None, None
        )

        staticPropertiesHandler.getInstance().jInit(
          jsonPrimitive(
            '{'
              '"option" : ['
                '{"name" : "reparamOnFace_precision", "value" : "1.e-05"},'
                '{"name" : "reparamInVolume_precision","value" : "1.e-05"},'
                '{"name" : "invY_precision", "value" : "1.e-04"},'
                '{"name" : "xyz_resolution", "value" : "1.e-05"},'
                '{"name" : "XYZ_resolution", "value" : "1.e-04"},'
                '{"name" : "uvw_resolution", "value" : "1.e-04"},'
                '{"name" : "point_render_diameter", "value" : "0.005"},'
                '{"name" : "vector_render_size", "value" : "0.05"},'
                '{"name" : "function_render_resolution_u", "value" : "21"},'
                '{"name" : "function_render_resolution_v", "value" : "21"},'
                '{"name" : "function_render_resolution_w", "value" : "21"},'
                '{"name" : "geometry_render_resolution_u", "value" : "21"},'
                '{"name" : "geometry_render_resolution_v", "value" : "21"},'
                '{"name" : "geometry_render_resolution_w", "value" : "21"},'
                '{"name" : "ompNumThreads", "value" : "2"},'
                '{"name" : "map1dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "map2dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "map3dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "logLevel", "value" : "0"},'
                '{"name" : "isEqualExtendCheck", "value" : "false"}'
              ']'
            '}'
          )
        )

        container = dtBundle()
        cV = container.cptr_cV()

        lVHOstateHandler.clear()
        lVHOstateHandler( jsonPrimitive(), cV ).thisown = False

        aF = container.cptr_aF()
        #aG = container.cptr_aG()
        aG = labeledVectorHandlingAnalyticGeometry()

        bV = container.cptr_bV()
        dP = container.cptr_dP()
        
        logging.info("Creating Layer Mesh on hub and shroud")
        
        # adding the unstructured region to the model
        m3dGmsh.getModel().addIfToGmshModel(self.unstructured_)
        
        # self.layerList_ has the following format:
        # self.layerList_ = [[hub layer lists],[shroud layer list]]
        # with:
        # [hub layer lists] = [[3d layer domain], [bool list radius zero]]
        # [bool list radius zero] tracks which layer se3gment has a radius of zero
        #  those layers have to be meshed five sided
        #  the entrie for those layers will be True 

        # iterating over the layers and pushing their respective surfaces
        # six and five sided layers are treated differently
        for i_hs in range(len(self.layerList_)):

            if i_hs == 0:
                label = "hub"
            else:
                label = "shroud"
            logging.info("Adding %s Layers. Number of Layers: %i" % (label, len(self.layerList_[i_hs][0])))
            
            # iterating over the hub and shroud layers
            for i_l in range(len(self.layerList_[i_hs][0])):
                
                # current 3d region
                layer3d = self.layerList_[i_hs][0][i_l]
                
                self.appendAnalyticGeometry(
                    layer3d.clone(),
                    "debug_layer_"+label+str(i_l)+"_"+self.label_
                )
 
                # finding the correct surfaces on the region by using the uvw direction
                # naming of faces:
                #   ortho    -> faces orthogonal to streamwise direction (connectsing the layers)
                #   periodic -> periodic faces (extend from channel curve radially inside the channel)
                #   channel  -> faces on channel side
                #   parallel -> faces parallel to channel
                ortho0, ortho1 = self.detectFirstAndSecond(layer3d, 2)
                periodic0, periodic1 = self.detectFirstAndSecond(layer3d, 1)
                channel, parallel = self.detectFirstAndSecond(layer3d, 3)
                
                # setting labels
                ortho0.setLabel("ortho_"+label+str(i_l))
                ortho1.setLabel("ortho_"+label+str(i_l+1))
                # labelling is different for six and five sided regions
                #  they have to be treated differently during meshing
                if self.layerList_[i_hs][1][i_l] == False:
                    logging.info("%s layer no. %i is six sided" % (label, i_l))
                    periodic0.setLabel("periodic0_"+label+str(i_l))
                    periodic1.setLabel("periodic1_"+label+str(i_l))
                    channel.setLabel("channel_"+label+str(i_l))
                    parallel.setLabel("parallel_"+label+str(i_l))
                else:
                    logging.info("%s layer no. %i is five sided" % (label, i_l))
                    periodic0.setLabel("periodic05s_"+label+str(i_l))
                    periodic1.setLabel("periodic15s_"+label+str(i_l))
                    channel.setLabel("channel5s_"+label+str(i_l))
                    parallel.setLabel("parallel5s_"+label+str(i_l))
                
                # pushing back the geometries
                aG.push_back(ortho0)
                # ortho1 is only pushed in the last region if it is six sided
                #  (ortho1 in five sided region is rotated surface with radius of zero)
                if i_l == len(self.layerList_[i_hs][0])-1 and self.layerList_[i_hs][1][i_l] == False:
                    aG.push_back(ortho1)
                aG.push_back(periodic0)
                aG.push_back(periodic1)
                aG.push_back(channel)
                aG.push_back(parallel)
                
                # adding the 3d layer as a region
                rID = m3dGmsh.getModel().addIfRegionToGmshModel(layer3d)
                # only six sided refions can be meshed completely transfinite
                if self.layerList_[i_hs][1][i_l] == False:
                    # Recursive means that all the edges and faces of the region are also tramsfinite
                    m3dGmsh.getModel().getDtGmshRegionByTag( rID ).meshTransfiniteRecursive()
                    m3dGmsh.getModel().getDtGmshRegionByTag( rID ).meshRecombineRecursive()
                 
        # initializing region labels
        #  later it will be iterated over the list in self (region labels are not used here)
        ob = bVONameRegions()
        ob.jInit( jsonPrimitive('{ "_regionLabel" : [] }'), m3dGmsh )
        ob.preUpdate()
        
        # initializing surface labels in gmsh
        ob = bVOAnalyticGeometryToFace()
        ob.jInit(
          jsonPrimitive(
            '{'
              '"analyticGeometry" : ['
                '{"labels" : "ortho_*"},'
                '{"labels" : "periodic0*"},'
                '{"labels" : "periodic1*"},'
                '{"labels" : "channel*"},'
                '{"labels" : "parallel*"}'
              '],'
              '"_inc" : 10.0,'
              '"_facesPerEntry" : []'
            '}'
          ),
          None, None, None, aG, None, m3dGmsh
        )
        ob.preUpdate()
        
        # meshing the lines orthogonal to the channel lines
        # these lines connect the faces channel and parallel
        channelToParallelLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("channel*","parallel*")
        for line in channelToParallelLines:
            logging.info( "meshing graded Line : ID: %i, number of elements: %i" % (line, self.nLayers_) )
            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            # meshing the lines with the in self specified number of elements and sets the grading
            theEdge.meshTransfiniteWNElements( 1, 1, self.nLayers_ )
            theEdge.setGrading(1.0, 3)

            self.appendAnalyticGeometry(
                m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                "debug_layerEdge_channelToParallelLine_"+"_ID"+str(line)+"_"+self.label_
            )

        # list containing the lengths of the mesh lines in circunferential direction
        lChannel_circ = [0, 0]

        # iterating over the hub and shroud layers
        # here the mesh settings for the edges in streamwise and circumferential directions are set
        # regions with five sides are specially trated in this loop
        for i_hs in range(len(self.layerList_)):

            if i_hs == 0:
                label = "hub"
            else:
                label = "shroud"
            
            # iterating over the individual layers in the hub and shroud layer list
            for i_l in range(len(self.layerList_[i_hs][0])):
                
                # finds an edge on the channel in streamwise direction
                edges = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "channel*"+label+str(i_l)+"*",
                        "periodic0*"+label+str(i_l)+"*"
                    )
                # calculates the length and number of elements on this line
                # this number of elements is used for all streamwise lines in this layer region
                lChannel_sw = edges[0].getMap1dTo3d().length()
                nE = int(numpy.ceil(lChannel_sw/self.elementSizeSW_))                  
                logging.info("layer %s%i has a channel length of %.4f, meshing with %i elements" % (
                    label, i_l, lChannel_sw, nE))
                
                # finding all streamwise layer curves for this six sided regions
                if self.layerList_[i_hs][1][i_l] == False:
                    # found by searching the connecting lines of the orthogonal faces
                    swLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                            "ortho_"+label+str(i_l)+"*",
                            "ortho_"+label+str(i_l+1)+"*")
                
                # finding streamwise layercurves for five sided regions
                #  needs special treatment because this region doesnt have a second ortho face
                elif self.layerList_[i_hs][1][i_l] == True:
                    # lines have to be found manually as the common lines 
                    #  between channel/parallel face and the periodic faces
                    line0 = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "channel5s_"+label+str(i_l)+"*",
                        "periodic05s_"+label+str(i_l)+"*"
                    )
                    line1 = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "channel5s_"+label+str(i_l)+"*",
                        "periodic15s_"+label+str(i_l)+"*"
                    )
                    line2 = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "parallel5s_"+label+str(i_l)+"*",
                        "periodic05s_"+label+str(i_l)+"*"
                    )
                    line3 = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "parallel5s_"+label+str(i_l)+"*",
                        "periodic15s_"+label+str(i_l)+"*"
                    )
                    swLines = [line0[0].tag(), line1[0].tag(), line2[0].tag(), line3[0].tag()]
                    
                    # setting the orthogonal and the periodic faces of the five sided region as transfinite
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("ortho_"+label+str(i_l)+"*").meshTransfinite()
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("ortho_"+label+str(i_l)+"*").meshRecombine()
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic05s_"+label+str(i_l)+"*").meshTransfinite()
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic05s_"+label+str(i_l)+"*").meshRecombine()
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic15s_"+label+str(i_l)+"*").meshTransfinite()
                    m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic15s_"+label+str(i_l)+"*").meshRecombine()
                
                # setting the mesh size of the streamwise layer lines of the region
                for line in swLines:
                    self.appendAnalyticGeometry(
                        m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                        "debug_layerEdge_swLine_"+label+str(i_l)+"_ID"+str(line) +"_"+self.label_
                    )
                    logging.info( "meshing streamwise Line : %s%i, ID: %i, number of elements: %i" % (label, i_l, line, nE) )
                    m3dGmsh.getModel().getDtGmshEdgeByTag(line).meshTransfiniteWNElements(1,1,nE)
                   
                # finding the layer curve in circumferential direction 
                #  as the common edge between the channel and the ortho face
                edges = self.getCommonEdgesByPhysicalFaces(
                    m3dGmsh,
                    "channel*"+label+str(i_l)+"*",
                    "ortho_"+label+str(i_l)+"*"
                )
                # length of circumferential curve
                l_ortho = numpy.abs(edges[0].getMap1dTo3d().length())
                logging.info("Circumferential edge : %s%i, ID: %i, length: %.4f" % (label, i_l, edges[0].tag(), l_ortho))
                # finding the longest circumferential edge in the hub and shroud layers
                #  based on this length the number of elements in circ direction will be calculated
                if l_ortho > lChannel_circ[i_hs]:
                    lChannel_circ[i_hs] = l_ortho
                
                # finding the last layer curve in circumferential direction of the last region
                #  this is only done for six sided regions 
                #  (the radius of the last circ line of five sided regions is zero)
                if i_l == len(self.layerList_[i_hs][0])-1 and self.layerList_[i_hs][1][i_l] == False:
                    edges = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "channel_"+label+str(i_l)+"*",
                        "ortho_"+label+str(i_l+1)+"*"
                    )
                    l_ortho = numpy.abs(edges[0].getMap1dTo3d().length())
                    logging.info("Circumferential edge : %s%i, ID: %i, length: %.4f" % (label, i_l+1, edges[0].tag(), l_ortho))
                    if l_ortho > lChannel_circ[i_hs]:
                        lChannel_circ[i_hs] = l_ortho                
                    
            # calculating the number of elements for the edges in circ direction from the longest edge length
            nE = int(numpy.ceil(lChannel_circ[i_hs]/self.elementSizeCIRC_))
            
            # finding all circ edges on hub or shroud
            circLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                            "periodic0_"+label+"*",
                            "periodic1_"+label+"*"
                        ) 

            # setting the number of elements on the circ edges for hub or shroud
            # number of elements have to be constant for these edges for transfinte meshing
            for line in circLines:
                self.appendAnalyticGeometry(
                    m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                    "debug_layerEdge_circLine_"+label+"_ID"+str(line) +"_"+self.label_
                )

                logging.info( "meshing circumferential Line : %s, ID: %i, number of elements: %i" % (label, line, nE) )
                m3dGmsh.getModel().getDtGmshEdgeByTag(line).meshTransfiniteWNElements(1,1,nE)

        # initializing grading 
        # scaTanhGradingOneD(c, g, gMin, gMax) -> f(x)=c[0]+c[1]*tanh(g*(c[2]+c[3]*x))/tanh(g)
        theRef = scaTanhGradingOneDCompound(
                    scaTanhGradingOneD(
                        vectorReal([1.0, -1.0, 1.0, -1.0]),
                                        1.0,
                                        0.1, 5.0
                        )
                    )
        theRef.setLabel( "aF_grading" )
        aF.push_back( theRef.clone() )
        
        # setting the first element size specified as a class variable 
        ob = bVOSetPrescribedElementSize()
        ob.thisown = False
        ob.jInit(
            jsonPrimitive(
                '{'
                '"_type": 3,'
                '"_firstElementSize": '+str(self.firstElement_)+','
                '"_grading" : {'
                    '"analyticFunction" : {"label" : "aF_grading"}'
                  '}'
                '}'
            ),
            None, None, aF, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
        
        # setting mesh rules
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
                  '"typeTransfinite" : [3],'
                  '"gradingFunctions" : { "analyticFunction" : ['
                    '{"label" : "aF_grading"}'
                  ']}'
                '},'
                '{'
                  '"name" : "dtMeshGFace",'
                  '"label" : "dtMeshGFace"'
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
          None, None, aF, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
        
        # generating the mesh
        logging.info("Generating the mesh: %s" % self.label_)
        bV.set( m3dGmsh )
        bV[0].makeGrid()    

        ob = bVOWriteMSH()
        ob.jInit(
          jsonPrimitive('{"_filename" : "'+self.label_+'.msh", "_saveAll" : true}'),
          None, None, None, None, None, m3dGmsh
        )
        ob.postUpdate()
        logging.info("Mesh generated and Saved in %s.msh" % self.label_)
        bV.set(None)
        del m3dGmsh
        lVHOstateHandler.clear()
        
        return

    def detectFirstAndSecond(self,
                             channel: map3dTo3d, 
                             direction: int
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
    
    def getCommonEdgesByPhysicalFaces(self,
                                      m3dGmsh,
                                      face0: str,
                                      face1: str):
        """Returns the common edge between two faces
        """
        commonEdges = []
        edges0 = m3dGmsh.getModel().getDtGmshFaceByPhysical(face0).dtEdges()
        edges1 = m3dGmsh.getModel().getDtGmshFaceByPhysical(face1).dtEdges()

        for e0 in edges0:
            for e1 in edges1:
                if numpy.abs(e0.tag()) == numpy.abs(e1.tag()):
                    commonEdges.append(e0)

        return commonEdges
