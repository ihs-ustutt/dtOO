#import dtOOPythonSWIG as dtOO
from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import jsonPrimitive
from dtOOPythonSWIG import staticPropertiesHandler
from dtOOPythonSWIG import dtBundle
from dtOOPythonSWIG import lVHOstateHandler
from dtOOPythonSWIG import analyticSurface
from dtOOPythonSWIG import bVOMeshRule
from dtOOPythonSWIG import analyticGeometry
from dtOOPythonSWIG import map2dTo3d
from dtOOPythonSWIG import map3dTo3d
from dtOOPythonSWIG import map3dTo3dGmsh
from dtOOPythonSWIG import jsonPrimitive
from dtOOPythonSWIG import logMe
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

import logging
import numpy
from typing import List, Tuple, Union, Dict
import json

class map3dTo3dGmsh_gridFromLayers (dtBundleBuilder):

    def __init__(self,
                 layers: List,
                 nLayers: int,
                 firstElement: float,
                 elementSize):
        super(map3dTo3dGmsh_gridFromLayers, self).__init__()
        #self._m3dGmsh = None

        print("the class is working")
        self.layerList_ = layers
        self.nLayers_ = nLayers
        self.firstElement_ = firstElement
        self.elementSize_ = elementSize

        logMe.initLog('layerMesh.log')

        self.map3dTo3dGmshJson_ = jsonPrimitive(
            '{"label" : "meshLayer",'
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

        logging.info( "Building %s ..." % ("meshLayers") )
        #print("=== HARD RESET dtOO STATE ===")

        #try:
        #    print("dtOO.clear()")
        #    dtOO.clear()
        #except:
        #    print("except")
        #    pass
        #try:
        #    print("dtOO.lVHOstateHandler.clear()")
        #    dtOO.lVHOstateHandler.clear()
        #except:
        #    print("except")
        #    pass

        #try:
        #    print("dtOO.staticPropertiesHandler.clear()")
        #    dtOO.staticPropertiesHandler.clear()
        #except:
        #    
        #    print("except")
        #    pass

        #try:
        #    print("dtOO.map3dTo3dGmsh.clearGlobalModel()")
        #    dtOO.map3dTo3dGmsh.clearGlobalModel()
        #except:

        #    print("except")
        #    pass

        ##if 'm3dGmsh' in locals():
        ##    print("del m3dGmsh")
        ##    del m3dGmsh
        #if getattr(self, "_m3dGmsh", None):
        #    try:
        #        self._m3dGmsh.detachAllObservers()
        #        self._m3dGmsh.reset()
        #        print("resetting m3dGmsh")
        #    except:
        #        print("failed")
        #        pass
        #    self._m3dGmsh = None

        m3dGmsh = map3dTo3dGmsh()
        self._m3dGmsh = m3dGmsh

        #m3dGmsh = map3dTo3dGmsh()
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
        
        #
        #   start for loop here
        #
        print("hublayers")
        print(len(self.layerList_[0][0]))
        print(self.layerList_[0][0])
        print(len(self.layerList_[0][1]))
        print(self.layerList_[0][1])
        print("shroudlayers")
        print(len(self.layerList_[1][0]))
        print(self.layerList_[1][0])
        print(len(self.layerList_[1][1]))
        print(self.layerList_[1][1])
        
        regionIDs = []
        fiveSidedLayers = []
        for i_hs in range(len(self.layerList_)):

            if i_hs == 0:
                label = "hub"
            else:
                label = "shroud"
            print("###########   ",label) 
            print("self.layerList_[i_hs][1][i_l] = ", self.layerList_[i_hs][1])
            
            for i_l in range(len(self.layerList_[i_hs][0])):
                
                print("i_l = ", i_l)
                
                layer3d = self.layerList_[i_hs][0][i_l]
            
                # layers with 6 sides
                if self.layerList_[i_hs][1][i_l] == False:
                
                    ortho0, ortho1 = self.detectFirstAndSecond(layer3d, 2)
                    periodic0, periodic1 = self.detectFirstAndSecond(layer3d, 1)
                    channel, parallel = self.detectFirstAndSecond(layer3d, 3)
                    
                    ortho0.setLabel("ortho_"+label+str(i_l))
                    ortho1.setLabel("ortho_"+label+str(i_l+1))
                    periodic0.setLabel("periodic0_"+label+str(i_l))
                    periodic1.setLabel("periodic1_"+label+str(i_l))
                    channel.setLabel("channel_"+label+str(i_l))
                    parallel.setLabel("parallel_"+label+str(i_l))
                    
                    self.appendAnalyticGeometry(
                        analyticSurface(analyticSurface.MustDownCast(ortho0).ptrDtSurface()),
                        "debug_TEST_ortho0_"+label + str(i_l),
                    )
                    self.appendAnalyticGeometry(
                        analyticSurface(analyticSurface.MustDownCast(ortho1).ptrDtSurface()),
                        "debug_TEST_ortho1_"+label + str(i_l),
                    )

                    
                    print("ortho0.getLabel = ", ortho0.getLabel())
                    print("ortho1.getLabel = ", ortho1.getLabel())

                    #if i_l == 0:
                    aG.push_back(ortho0)
                    if i_l == len(self.layerList_[i_hs][0])-1 and self.layerList_[i_hs][1][i_l] == False:
                        aG.push_back(ortho1)
                    aG.push_back(periodic0)
                    aG.push_back(periodic1)
                    aG.push_back(channel)
                    aG.push_back(parallel)
             
                    rID = m3dGmsh.getModel().addIfRegionToGmshModel(layer3d)
                    m3dGmsh.getModel().getDtGmshRegionByTag( rID ).meshTransfiniteRecursive()
                    m3dGmsh.getModel().getDtGmshRegionByTag( rID ).meshRecombineRecursive()
                    regionIDs.append((rID, self.layerList_[i_hs][1][i_l]))
                
                # layers with 5 sides
                else:
                    fiveSidedLayers.append((label, i_l))
                    
                    ortho0, ortho1 = self.detectFirstAndSecond(layer3d, 2)
                    periodic0, periodic1 = self.detectFirstAndSecond(layer3d, 1)
                    channel, parallel = self.detectFirstAndSecond(layer3d, 3)
                 
                    ortho0.setLabel("ortho_"+label+str(i_l))
                    ortho1.setLabel("ortho1FiveSided_"+label+str(i_l))
                    periodic0.setLabel("periodic0FiveSided_"+label+str(i_l))
                    periodic1.setLabel("periodic1FiveSided_"+label+str(i_l))
                    channel.setLabel("channelFiveSided_"+label+str(i_l))
                    parallel.setLabel("parallelFiveSided_"+label+str(i_l))
                 
                    self.appendAnalyticGeometry(
                        analyticSurface(analyticSurface.MustDownCast(ortho0).ptrDtSurface()),
                        "debug_TEST_ortho0FiveSided_"+label + str(i_l),
                    )
                 
                    #print("ortho0.getLabel = ", ortho0.getLabel())
                    #if i_l == 0:
                    aG.push_back(ortho0)
                    #aG.push_back(ortho1)
                    aG.push_back(periodic0)
                    aG.push_back(periodic1)
                    aG.push_back(channel)
                    aG.push_back(parallel)
          
                    rID = m3dGmsh.getModel().addIfRegionToGmshModel(layer3d)
                    regionIDs.append((rID, self.layerList_[i_hs][1][i_l]))


                   ## m3dGmsh.getModel().getDtGmshRegionByTag( rID ).dtFaces() -> f.dtEdges()
                    #m3dGmsh.getModel().getDtGmshRegionByTag( rID ).meshTransfiniteRecursive()
                    #m3dGmsh.getModel().getDtGmshRegionByTag( rID ).meshRecombineRecursive()
        
        #
        # name regions
        #
        

        ob = bVONameRegions()
        ob.jInit( jsonPrimitive('{ "_regionLabel" : [] }'), m3dGmsh )
        ob.preUpdate()

                #m3dGmsh.getModel().addIfFaceToGmshModel(ortho0)
                #m3dGmsh.getModel().addIfFaceToGmshModel(ortho1)
                #m3dGmsh.getModel().addIfFaceToGmshModel(periodic0)
                #m3dGmsh.getModel().addIfFaceToGmshModel(periodic1)
                #m3dGmsh.getModel().addIfFaceToGmshModel(channel)
                #m3dGmsh.getModel().addIfFaceToGmshModel(parallel)

        analytic_geometry = [
            {"labels": "ortho_*"},
            #{"labels": "ortho1_*"},
            {"labels": "periodic0_*"},
            {"labels": "periodic1_*"},
            {"labels": "channel_*"},
            {"labels": "parallel_*"},
        ]
        print(analytic_geometry)

        if fiveSidedLayers:
            print("FIVESIDEDLAYER EXISTS")
            analytic_geometry.extend([
                #{"labels": "ortho0FiveSided_*"},
                #{"labels": "ortho1FiveSided_*"},
                {"labels": "periodic0FiveSided_*"},
                {"labels": "periodic1FiveSided_*"},
                {"labels": "channelFiveSided_*"},
                {"labels": "parallelFiveSided_*"},
            ])
        print(analytic_geometry)
        data = {
            "analyticGeometry": analytic_geometry,
            "_inc": 10.0,
            "_facesPerEntry": []
        }

        ob = bVOAnalyticGeometryToFace()
        ob.jInit(
            jsonPrimitive(json.dumps(data)),
            None, None, None, aG, None, m3dGmsh
        )
        ob.preUpdate() 
        
        channelToParallelLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("channel_*","parallel_*")
        for line in channelToParallelLines:
            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            theEdge.meshTransfiniteWNElements( 1, 1, self.nLayers_ )
            theEdge.setGrading(1.0, 3)



        lChannel_circ = [0, 0]
        for i_hs in range(len(self.layerList_)):

            if i_hs == 0:
                label = "hub"
            else:
                label = "shroud"
            print(label)
            print("")
            for i_l in range(len(self.layerList_[i_hs][0])):
                print("i_l = ", i_l)
                # layers with 6 sides
                if self.layerList_[i_hs][1][i_l] == False:
                    edges = self.getCommonEdgesByPhysicalFaces(
                            m3dGmsh,
                            "channel_"+label+str(i_l)+"*",
                            "periodic0_"+label+str(i_l)+"*"
                        )
                    lChannel_sw = edges[0].getMap1dTo3d().length()
                    print("lChannel_sw = ", lChannel_sw)
                    nE = int(numpy.ceil(lChannel_sw/self.elementSize_))                  

                    swLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                            "ortho_"+label+str(i_l)+"*",
                            "ortho_"+label+str(i_l+1)+"*")
                    
                    for line in swLines:
                    #for line in channelLines:

                        #print("line ID = ", line)
                        self.appendAnalyticGeometry(
                            m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                            "debug_TEST_swLine_"+label+str(i_l)+"_ID"+str(line)
                        )
                        logging.info( "meshing streamwise Line : %s%i, ID: %i, number of elements: %i" % (label, i_l, line, nE) )
                        m3dGmsh.getModel().getDtGmshEdgeByTag(line).meshTransfiniteWNElements(1,1,nE)

                # layers with 6 sides
                if self.layerList_[i_hs][1][i_l] == False:
                    
                
                    edges = self.getCommonEdgesByPhysicalFaces(
                        m3dGmsh,
                        "channel_"+label+str(i_l)+"*",
                        "ortho_"+label+str(i_l)+"*"
                    )
                    l_ortho = edges[0].getMap1dTo3d().length()
                    if l_ortho > lChannel_circ[i_hs]:
                        lChannel_circ[i_hs] = l_ortho
                    print("l_ortho = ", l_ortho)
                    
                    if i_l == len(self.layerList_[i_hs][0])-1 and self.layerList_[i_hs][1][i_l] == False:
                        edges = self.getCommonEdgesByPhysicalFaces(
                            m3dGmsh,
                            "channel_"+label+str(i_l)+"*",
                            "ortho_"+label+str(i_l+1)+"*"
                        )
                        l_ortho = edges[0].getMap1dTo3d().length()
                        if l_ortho > lChannel_circ[i_hs]:
                            lChannel_circ[i_hs] = l_ortho
                        print("l_ortho = ", l_ortho)
           
            nE = int(numpy.ceil(lChannel_circ[i_hs]/self.elementSize_))
            
            circLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical(
                            "periodic0_"+label+"*",
                            "periodic1_"+label+"*")
            print("circLines = ", circLines)
            for line in circLines:
                self.appendAnalyticGeometry(
                    m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                    "debug_TEST_circLine_"+label+"_ID"+str(line)
                )

                logging.info( "meshing circumferential Line : %s, ID: %i, number of elements: %i" % (label, line, nE) )
                m3dGmsh.getModel().getDtGmshEdgeByTag(line).meshTransfiniteWNElements(1,1,nE)

        #return
        """
        #print("lChannels = ", lChannels)
        channelToParallelLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("channel_*","parallel_*")
        channelLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("channel_*","channel_*")
        parallelLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("parallel_*","parallel_*")
        
        channel_faces = m3dGmsh.getModel().getDtGmshFaceListByPhysical("channel_*")
        i=0
        for face in channel_faces:
            self.appendAnalyticGeometry(
                face.getMap2dTo3d(),
                "debug_TEST_channelFaces_"+str(i)
            )
            i=i+1
        print("number of channel faces = ", i)
        parallel_faces = m3dGmsh.getModel().getDtGmshFaceListByPhysical("parallel_*")
        i=0
        for face in parallel_faces:
            self.appendAnalyticGeometry(
                face.getMap2dTo3d(),
                "debug_TEST_parallelFaces_"+str(i)
            )
            i=i+1
        print("number of parallel faces = ", i)
        

        print("type surf")
        print(channelToParallelLines)
        print(type(m3dGmsh.getModel().getDtGmshEdgeByTag(channelToParallelLines[0]).getMap1dTo3d()))

        for ii in channelToParallelLines:
            self.appendAnalyticGeometry(
                m3dGmsh.getModel().getDtGmshEdgeByTag( ii ).getMap1dTo3d(),
                "debug_TEST_channelToParallelLine_"+str(ii)
            )

        for line in channelToParallelLines:
            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            theEdge.meshTransfiniteWNElements( 1, 1, self.nLayers_ )
            theEdge.setGrading(1.0, 3)
        
        print("length of sized lines = ", len(channelLines + parallelLines))
        
        for line in channelLines + parallelLines:
        #for line in channelLines:
        
            print("line ID = ", line)
            self.appendAnalyticGeometry(
                m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                "debug_TEST_sizedLine_"+str(line)
            )

            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            eL = theEdge.getMap1dTo3d().length()
            nE = int(numpy.ceil(eL/self.elementSize_))
            logging.info( "Layer line %i with %i elements" % (line, nE) )
            theEdge.meshTransfiniteWNElements(1,1,nE)
        
        channelToParallelLines_5s = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("channelFiveSided_*","parallelFiveSided_*")
        channelLines_5s = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("channelFiveSided_*","channelFiveSided_*")
        parallelLines_5s = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("parallelFiveSided_*","parallelFiveSided_*")
        print("should all be 3")
        print(len(channelToParallelLines_5s))
        print(len(channelLines_5s))
        print(len(parallelLines_5s))

        for line in channelToParallelLines_5s:
            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            theEdge.meshTransfiniteWNElements( 1, 1, self.nLayers_ )
            #theEdge.setNElements( 1, 1, self.nLayers_ )
            theEdge.setGrading(1.0, 3)
        print("length of sized lines = ", len(channelLines + parallelLines))
        for line in channelLines_5s + parallelLines_5s:
        #for line in parallelLines:
            print("line ID = ", line)
            self.appendAnalyticGeometry(
                m3dGmsh.getModel().getDtGmshEdgeByTag(line).getMap1dTo3d(),
                "debug_TEST_sizedLine_"+str(line)
            )

            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            eL = theEdge.getMap1dTo3d().length()
            #nE = int(numpy.ceil(eL/self.elementSize_))
            nE = 5
            logging.info( "Layer line %i with %i elements" % (line, nE) )
            theEdge.meshTransfiniteWNElements(1,1,nE)
            #theEdge.setNElements(nE)

        self.appendAnalyticGeometry(
            m3dGmsh.getModel().getDtGmshFaceByPhysical("ortho1_hub1*").getMap2dTo3d(),
            "debug_TEST_otrho1_hub1"
        )

        # test line
        print("test line")
        
        m3dGmsh.getModel().getDtGmshFaceByPhysical("ortho1_hub1*").meshTransfinite()
        m3dGmsh.getModel().getDtGmshFaceByPhysical("ortho1_hub1*").meshRecombine()
        m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic0FiveSided_*").meshTransfinite()
        m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic0FiveSided_*").meshRecombine()
        m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic1FiveSided_*").meshTransfinite()
        m3dGmsh.getModel().getDtGmshFaceByPhysical("periodic1FiveSided_*").meshRecombine()
        """
        #return
        # f(x)=c[0]+c[1]*tanh(g*(c[2]+c[3]*x))/tanh(g)
        theRef = scaTanhGradingOneDCompound(
                    scaTanhGradingOneD(
                        vectorReal([1.0, -1.0, 1.0, -1.0]),
                                        1.0,
                                        0.1, 5.0
                        )
                    )

        theRef.setLabel( "aF_grading" )
        aF.push_back( theRef.clone() )

        ob = bVOSetPrescribedElementSize()
        ob.thisown = False
        ob.jInit(
            jsonPrimitive(
                '{'
                '"_type": 3,'
                '"_firstElementSize": '+str(self.firstElement_)+','
                #'"_grading": ["aF_grading"]'
                '"_grading" : {'
                    '"analyticFunction" : {"label" : "aF_grading"}'
                  '}'
                '}'
            ),
            None, None, aF, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)

        ob = bVOMeshRule()                  # mesh rules
        ob.thisown = False
        ob.jInit(
          jsonPrimitive(
            '{'
              '"option" : ['
                '{"name" : "debug", "value" : "true"}'
              '],'
              '"_rule1D" : ['
                #'"dtMeshGEdge(*)"'                         
                '"dtMeshFreeGradingGEdge(*)"'
              '],'
              '"_rule2D" : ['
                '"dtMeshGFace(*)"'
                #'"dtMeshGFace(surface1_0)",'
                #'"dtMeshGFaceWithTransfiniteLayer(*)"'
              '],'
              '"_rule3D" : ['
                '"dtMeshGRegion(*)"'
              '],'
              '"_only" : [],'
              '"dtMeshOperator" : ['
                #'{'
                #  '"name" : "dtMeshGEdge",'
                #  '"label" : "dtMeshGEdge"'
                #'},'
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
                #'{'
                #  '"name" : "dtMeshGFaceWithTransfiniteLayer",'         #setting for mesh face (corresponds to command in _rule2D)
                #  '"label" : "dtMeshGFaceWithTransfiniteLayer",'
                #  '"_nLayers" : [ 2, 0],'                              # number of boundary layers on surface ([surf0, surf1])
                #  '"_direction" : 0,'                                   # direction of layers
                #  '"_nSmooth" : 1000'                                   # smoothing iterations
                #'},'
                '{'
                  '"name" : "dtMeshGRegion",'                           # dtMeshGRegion -> not used here
                  '"label" : "dtMeshGRegion",'
                  '"_minQShapeMetric" : 0.0,'
                  '"_relax" : 0.1,'
                  '"_nPyramidOpenSteps" : 10,'                          # pyramid layer between structured and unstructured mesh
                  '"_nSmooths" : 3'
                '}'
                #'{'
                #  '"option" : ['
                #    '{"name" : "debug", "value" : "true"}'
                #  '],'
                #  '"name" : "dtMeshGRegionWithBoundaryLayer",'          # mesh settings for the volume 
                #  '"label" : "dtMeshGRegionWithBoundaryLayer",'
                #  '"_nSpacingSteps" : [-9],'                            # tied to _nLayers (= _nLayers-1)
                #  '"_nNormalSmoothingSteps" : 100,'
                #  '"_nGrowingSmoothingSteps" : 100,'
                #  '"_maxGrowingRatePerStep" : 2,'
                #  '"_maxDihedralAngle" : '+str(float(numpy.pi*360./180.))+','
                #  '"_faceLabel" : ['
                #    '"channel*"'                                      # face where layers are made
                #  '],"'
                #  '_slidableFaceLabel" : ['                             # these layers can be adapted for the benefit of _faceLabel
                #    '"ortho*", "periodic*"'
                #  '],'
                #  '"dtMesh3DOperator" : "dtMeshGRegion",'
                #  '"_fixedFaceLabel" : ["parallel*"]'                  # the other surface surface1_0 could also be written here
                #'}'

              ']'
            '}'
          ),
          None, None, aF, None, None, m3dGmsh
        )
        m3dGmsh.attachBVObserver(ob)
        
        bV.set( m3dGmsh )

        bV[0].makeGrid()                #generates the mesh

        ob = bVOWriteMSH()
        ob.jInit(
          jsonPrimitive('{"_filename" : "layer_mesh.msh", "_saveAll" : true}'),  # specifies how the mesh is saved
          None, None, None, None, None, m3dGmsh
        )
        ob.postUpdate()

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
        
        commonEdges = []
        edges0 = m3dGmsh.getModel().getDtGmshFaceByPhysical(face0).dtEdges()
        edges1 = m3dGmsh.getModel().getDtGmshFaceByPhysical(face1).dtEdges()

        for e0 in edges0:
            for e1 in edges1:
                if numpy.abs(e0.tag()) == numpy.abs(e1.tag()):
                    commonEdges.append(e0)

        return commonEdges
