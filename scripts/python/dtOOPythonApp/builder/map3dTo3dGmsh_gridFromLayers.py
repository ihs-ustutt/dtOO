import dtOOPythonSWIG as dtOO
from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder
import logging
import numpy
from typing import List, Tuple, Union, Dict

class map3dTo3dGmsh_gridFromLayers (dtBundleBuilder):

    def __init__(self,
                 layers: List,
                 nLayers: int,
                 firstElement: float,
                 elementSize):
        super(map3dTo3dGmsh_gridFromLayers, self).__init__()

        print("the class is working")
        self.layerList_ = layers
        self.nLayers_ = nLayers
        self.firstElement_ = firstElement
        self.elementSize_ = elementSize
        print(len(self.layerList_))

        dtOO.logMe.initLog('layerMesh.log')

        self.map3dTo3dGmshJson_ = dtOO.jsonPrimitive(
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

        m3dGmsh = dtOO.map3dTo3dGmsh()
        m3dGmsh.jInit(
          self.map3dTo3dGmshJson_, None, None, None, None, None
        )

        dtOO.staticPropertiesHandler.getInstance().jInit(
          dtOO.jsonPrimitive(
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

        container = dtOO.dtBundle()
        cV = container.cptr_cV()

        dtOO.lVHOstateHandler.clear()
        dtOO.lVHOstateHandler( dtOO.jsonPrimitive(), cV ).thisown = False

        aF = container.cptr_aF()
        #aG = container.cptr_aG()
        aG = dtOO.labeledVectorHandlingAnalyticGeometry()

        bV = container.cptr_bV()
        dP = container.cptr_dP()
        
        #
        #   start for loop here
        #
        
        for i_hs in range(len(self.layerList_)):

            if i_hs == 0:
                label = "hub"
            else:
                label = "shroud"
            
            print("self.layerList_[i_hs][1][i_l] = ", self.layerList_[i_hs][1])
            
            for i_l in range(len(self.layerList_[i_hs])):
                
                    print("i_l = ", i_l)
                    
                    layer3d = self.layerList_[i_hs][0][i_l]
                
                if self.layerList_[i_hs][1][i_l] == False:
                    
                    ortho0, ortho1 = self.detectFirstAndSecond(layer3d, 2)
                    periodic0, periodic1 = self.detectFirstAndSecond(layer3d, 1)
                    channel, parallel = self.detectFirstAndSecond(layer3d, 3)
                    
                    ortho0.setLabel("ortho0_"+label+str(i_l))
                    ortho1.setLabel("ortho1_"+label+str(i_l))
                    periodic0.setLabel("periodic0_"+label+str(i_l))
                    periodic1.setLabel("periodic1_"+label+str(i_l))
                    channel.setLabel("channel_"+label+str(i_l))
                    parallel.setLabel("parallel_"+label+str(i_l))
                    
                    self.appendAnalyticGeometry(
                        dtOO.analyticSurface(dtOO.analyticSurface.MustDownCast(ortho0).ptrDtSurface()),
                        "debug_TEST_ortho0_"+label + str(i_l),
                    )
                    self.appendAnalyticGeometry(
                        dtOO.analyticSurface(dtOO.analyticSurface.MustDownCast(ortho1).ptrDtSurface()),
                        "debug_TEST_ortho1_"+label + str(i_l),
                    )

                    
                    print("ortho0.getLabel = ", ortho0.getLabel())
                    if i_l == 0:
                        aG.push_back(ortho0)
                    if self.layerList_[i_hs][1][i_l] == False:
                        aG.push_back(ortho1)
                    aG.push_back(periodic0)
                    aG.push_back(periodic1)
                    aG.push_back(channel)
                    aG.push_back(parallel)
             
                    rID = m3dGmsh.getModel().addIfRegionToGmshModel(layer3d)
                    m3dGmsh.getModel().getDtGmshRegionByTag( rID ).meshTransfiniteRecursive()
                    m3dGmsh.getModel().getDtGmshRegionByTag( rID ).meshRecombineRecursive()

                else:

        #
        # name regions
        #
        ob = dtOO.bVONameRegions()
        ob.jInit( dtOO.jsonPrimitive('{ "_regionLabel" : [] }'), m3dGmsh )
        ob.preUpdate()

                #m3dGmsh.getModel().addIfFaceToGmshModel(ortho0)
                #m3dGmsh.getModel().addIfFaceToGmshModel(ortho1)
                #m3dGmsh.getModel().addIfFaceToGmshModel(periodic0)
                #m3dGmsh.getModel().addIfFaceToGmshModel(periodic1)
                #m3dGmsh.getModel().addIfFaceToGmshModel(channel)
                #m3dGmsh.getModel().addIfFaceToGmshModel(parallel)

        ob = dtOO.bVOAnalyticGeometryToFace()
        ob.jInit(
          dtOO.jsonPrimitive(
            '{'
              '"analyticGeometry" : ['
                '{"labels" : "ortho0_*"},'
                '{"labels" : "ortho1_*"},'
                '{"labels" : "periodic0_*"},'
                '{"labels" : "periodic1_*"},'
                '{"labels" : "channel_*"},'
                '{"labels" : "parallel_*"}'
              '],'
              '"_inc" : 10.0,'
              '"_facesPerEntry" : []'
            '}'
          ),
          None, None, None, aG, None, m3dGmsh
        )
        ob.preUpdate()
        
        #for e in m3dGmsh.getModel().dtEdges():
        #    e.meshTransfiniteWNElements(1,1,10)
        #    print("type : ", type(e))
        #    print("edge length = ", e.length())

        channelToParallelLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("channel_*","parallel_*")
        channelLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("channel_*","channel_*")
        parallelLines = m3dGmsh.getModel().getDtGmshEdgeTagListByFromToPhysical("parallel_*","parallel_*")

        #surfaceLines = (
        #        set(channelLines)&set(parallelLines)
        #    ).union(
        #        set(channelLines)&set(-numpy.array(parallelLines)))

        #channelLines = m3dGmsh.getModel().getDtGmshFaceByPhysical("channel_*").dtEdges()
        #channelLineTags = []
        #for edge in channelLines:
        #    channelLineTags.append(edge.tag())
        #    print("edge.tag() = ", edge.tag()) 
        #print("channelLineTags = ", channelLineTags)
        #parallelLines = m3dGmsh.getModel().getDtGmshFaceByPhysical("parallel_*").dtEdges()
        #parallelLineTags = []
        #for edge in parallelLines:
        #    parallelLineTags.append(edge.tag())

        #surfaceLines = (
        #        set(channelLineTags)&set(parallelLineTags)
        #    ).union(
        #        set(channelLineTags)&set(-numpy.array(parallelLineTags)))
        print("type surf")
        print(channelToParallelLines)
        print(type(m3dGmsh.getModel().getDtGmshEdgeByTag(channelToParallelLines[0]).getMap1dTo3d()))

        for ii in channelToParallelLines:
            self.appendAnalyticGeometry(
                m3dGmsh.getModel().getDtGmshEdgeByTag( ii ).getMap1dTo3d(),
                "debug_TEST_channelToParallelLine_"+str(ii)
            )

        for ii in channelLines:
            self.appendAnalyticGeometry(
                m3dGmsh.getModel().getDtGmshEdgeByTag( ii ).getMap1dTo3d(),
                "debug_TEST_channelLine_"+str(ii)
            )
        for ii in parallelLines:
            self.appendAnalyticGeometry(
                m3dGmsh.getModel().getDtGmshEdgeByTag( ii ).getMap1dTo3d(),
                "debug_TEST_parallelLine_"+str(ii)
            )

        for line in channelToParallelLines:
            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            theEdge.meshTransfiniteWNElements( 1, 1, self.nLayers_ )
            theEdge.setGrading(1.0, 3)

        for line in channelLines + parallelLines:
            theEdge = m3dGmsh.getModel().getDtGmshEdgeByTag( line )
            eL = theEdge.getMap1dTo3d().length()
            nE = int(numpy.ceil(eL/self.elementSize_))
            logging.info( "Layer line %i with %i elements" % (line, nE) )
            theEdge.meshTransfiniteWNElements(1,1,nE)


        # f(x)=c[0]+c[1]*tanh(g*(c[2]+c[3]*x))/tanh(g)
        theRef = dtOO.scaTanhGradingOneDCompound(
                    dtOO.scaTanhGradingOneD(
                        dtOO.vectorReal([1.0, -1.0, 1.0, -1.0]),
                                        1.0,
                                        0.1, 5.0
                        )
                    )

        theRef.setLabel( "aF_grading" )
        aF.push_back( theRef.clone() )

        ob = dtOO.bVOSetPrescribedElementSize()
        ob.thisown = False
        ob.jInit(
            dtOO.jsonPrimitive(
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

        ob = dtOO.bVOMeshRule()                  # mesh rules
        ob.thisown = False
        ob.jInit(
          dtOO.jsonPrimitive(
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

        ob = dtOO.bVOWriteMSH()
        ob.jInit(
          dtOO.jsonPrimitive('{"_filename" : "layer_mesh.msh", "_saveAll" : true}'),  # specifies how the mesh is saved
          None, None, None, None, None, m3dGmsh
        )
        ob.postUpdate()

        dtOO.lVHOstateHandler.clear()
       
        return

    def detectFirstAndSecond(self,
                             channel: dtOO.map3dTo3d, 
                             direction: int
        ) -> Tuple[dtOO.map2dTo3d, dtOO.map2dTo3d]:
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

