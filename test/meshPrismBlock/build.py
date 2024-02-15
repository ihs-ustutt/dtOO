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

import sys

from dtOOPythonSWIG import *
import numpy as np

logMe.initLog("build.log")
staticPropertiesHandler.getInstance().jInit(
  jsonPrimitive(
    '{'
      '"option" : ['
        '{"name" : "reparamOnFace_precision", "value" : "1.e-05"},'
        '{"name" : "reparamInVolume_precision","value" : "1.e-05"},'
        '{"name" : "reparam_internalRestarts", "value" : "10"},'
        '{"name" : "reparam_restarts", "value" : "10"},'
        '{"name" : "reparam_restartIncreasePrecision", "value" : "10."},'
        '{"name" : "reparam_internalRestartDecreasePrecision", "value" : "0.9"},'
        '{"name" : "reparamOnFace_minimizer", "value" : ":Minuit2::kMigrad:"},'		
        '{"name" : "reparamInVolume_minimizer", "value" : ":Minuit2::kMigrad:"},'				
        '{"name" : "invY_precision", "value" : "1.e-04"},'
        '{"name" : "xyz_resolution", "value" : "1.e-05"},'
        '{"name" : "XYZ_resolution", "value" : "1.e-04"},'
        '{"name" : "uvw_resolution", "value" : "1.e-04"},'
        '{"name" : "point_render_diameter", "value" : "0.0025"},'
        '{"name" : "vector_render_size", "value" : "0.0030"},'
        '{"name" : "function_render_resolution_u", "value" : "201"},'
        '{"name" : "function_render_resolution_v", "value" : "201"},'
        '{"name" : "function_render_resolution_w", "value" : "201"},'
        '{"name" : "geometry_render_resolution_u", "value" : "21"},'
        '{"name" : "geometry_render_resolution_v", "value" : "21"},'
        '{"name" : "geometry_render_resolution_w", "value" : "21"},'
        '{"name" : "root_printLevel", "value" : "0"},'		
        '{"name" : "root_maxIterations", "value" : "1000"},'
        '{"name" : "root_maxFunctionCalls", "value" : "1000000"},'
        '{"name" : "ompNumThreads", "value" : "2"},'
        '{"name" : "map1dTo3d_deltaPer", "value" : "0.01"},'
        '{"name" : "map2dTo3d_deltaPer", "value" : "0.01"},'
        '{"name" : "map3dTo3d_deltaPer", "value" : "0.01"},'
        '{"name" : "logLevel", "value" : "99"}'
      ']'
    '}'
  )
)
container = dtBundle()
bC = container.cptr_bC()
cV = container.cptr_cV()
aF = container.cptr_aF()
aG = container.cptr_aG()
bV = container.cptr_bV()
dC = container.cptr_dC()
dP = container.cptr_dP()

lVHOstateHandler( jsonPrimitive(), cV ).thisown = False

base_s = analyticSurface(#analyticRotatingMap1dTo3d(
  rectangularTrimmedSurface_curveRotateConstructOCC(    
    bSplineCurve_pointConstructOCC(
      dtPoint3( +0.00, +0.00, +0.00 ), dtPoint3( +0.00, +1.00, +0.00 ),
    ).result(),
    dtPoint3(0,0,0),
    dtVector3(0,0,1),
    0.50*np.pi                                                
  ).result()#,
  #dtPoint3(0,0,0),
  #dtVector3(0,0,1),
  #0.50*np.pi
)
base_s.setLabel("base")
aG.push_back( base_s.clone() )

mid0_s = analyticSurface(
  geomSurface_surfaceTranslateConstructOCC(
    base_s.ptrConstDtSurface(), 
    dtVector3(0.1,0.1,0.5)
  ).result()
)

trans_s = analyticSurface(
  geomSurface_surfaceTranslateConstructOCC(
    base_s.ptrConstDtSurface(), 
    dtVector3(0,0,1)
  ).result()
)
trans_s.setLabel("trans")
aG.push_back( trans_s.clone() )

vhs = vectorHandlingConstDtSurface()
vhs.push_back( base_s.ptrConstDtSurface() )
vhs.push_back( mid0_s.ptrConstDtSurface() )
vhs.push_back( trans_s.ptrConstDtSurface() )
vh = bSplineSurfaces_bSplineSurfaceSkinConstructOCC(vhs).result()
p = 0
for i in vh:
  print( "Add ", p )
  aa = analyticSurface( i )
  aa.setLabel("s_"+str(p))
  aG.push_back( aa.clone() )
  p = p + 1
  
#
ref = map3dTo3dGmsh()
ref.thisown = False
ref.jInit(
  jsonPrimitive(
    '{"label" : "cBoundVol",' 
      '"option" : ['
        '{"name" : "[gmsh]General.Terminal", "value" : "1."},'
        '{"name" : "[gmsh]General.Verbosity", "value" : "100."},'
        '{"name" : "[gmsh]General.ExpertMode", "value" : "1."},'
        #'{"name" : "[gmsh]Mesh.LcIntegrationPrecision", "value" : "1.e-4"},'
        '{"name" : "[gmsh]Mesh.CharacteristicLengthMin", "value" : "0.05"},'
        '{"name" : "[gmsh]Mesh.CharacteristicLengthMax", "value" : "0.1"},'
        '{"name" : "[gmsh]Mesh.Algorithm", "value" : "1"},'
        '{"name" : "[gmsh]Mesh.MeshSizeExtendFromBoundary", "value" : "1"},'
        '{"name" : "[gmsh]Mesh.MeshSizeFromPoints", "value" : "1"}'
      '],'
      '"analyticGeometry" : []'
  '}'),
  None, None, None, None, None
)
fid = vectorInt()
for a_i in aG:
  tid = ref.getModel().addIfFaceToGmshModel( map2dTo3d.MustConstDownCast( a_i ) )
  if tid!=0:
    fid.push_back(tid)

ob = bVOAnalyticGeometryToFace()
ob.jInit( 
  jsonPrimitive(
    '{'
      '"analyticGeometry" : ['
        '{"label" : "base"},'
        '{"label" : "trans"},'
#        '{"label" : "s_0"}'
        '{"label" : "s_1"},'
#        '{"label" : "s_2"},'
        '{"label" : "s_3"},'
        '{"label" : "s_4"},'
        '{"label" : "s_5"}'
      '],'
      '"_inc" : 10.0'
    '}'
  ), 
  None, None, None, aG, None, ref
)
ob.preUpdate()

ref.getModel().addIfRegionToGmshModel(infinityMap3dTo3d(), fid)
for e in ref.getModel().dtEdges():
  e.meshTransfiniteWNElements(1,1,10)
#for f in ref.getModel().dtFaces():
#  f.meshTransfinite()
 
eID = ref.getModel().getDtGmshEdgeTagListByFromToPhysical("base*", "trans*")
for e in eID:
  print("e => ", e)
  ref.getModel().getDtGmshEdgeByTag( e ).meshTransfiniteWNElements(1,1.0,30)
  for f in ref.getModel().getDtGmshEdgeByTag( e ).dtFaces():#meshTransfiniteWNElements(1,1,10)
    print("f = ", f.tag())
    f.meshTransfinite()
    #f.meshRecombine()
  
ob = bVOMeshRule()
ob.thisown = False
ob.jInit(
  jsonPrimitive( 
    '{'
      '"option" : ['
        '{"name" : "debug", "value" : "true"}'
      '],'
      '"_rule1D" : ['
        '"dtMeshGEdge(*)"'
      '],'
      '"_rule2D" : ['
        '"dtMeshGFace(base*)",'
        '"dtMeshGFace(trans*)",'
        '"dtMeshGFaceWithTransfiniteLayer(*)"'
      '],'
      '"_rule3D" : ['
        '"dtMeshGRegionWithBoundaryLayer(*)"'
      '],'
      '"_only" : [],'
      '"dtMeshOperator" : ['
        '{'
          '"name" : "dtMeshGEdge",'
          '"label" : "dtMeshGEdge"'
        '},'
        '{'
          '"name" : "dtMeshGFace",'
          '"label" : "dtMeshGFace"'
        '},'
        '{'
          '"name" : "dtMeshGFaceWithTransfiniteLayer",'
          '"label" : "dtMeshGFaceWithTransfiniteLayer",'
          '"_nLayers" : [ 11, 0],'
          '"_direction" : 0,'
          '"_nSmooth" : 1000'
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
          '"_nSpacingSteps" : [-10],'
          '"_nNormalSmoothingSteps" : 100,'
          '"_nGrowingSmoothingSteps" : 100,'
          '"_maxGrowingRatePerStep" : 1.10,'
          '"_maxDihedralAngle" : '+str(float(np.pi*360./180.))+','
          '"_faceLabel" : ['
            '"base_0"'
          '],"'
          '_slidableFaceLabel" : ['  
            '"*s_*"'
          '],'
          '"dtMesh3DOperator" : "dtMeshGRegion",'
          '"_fixedFaceLabel" : []'
        '}' 
      ']'
    '}'
  ),
  None, None, None, None, None, ref
)
ref.attachBVObserver(ob)

bV.set( ref )

bV[0].makeGrid()

ob = bVOWriteMSH()
ob.jInit(
  jsonPrimitive('{"_filename" : "mesh.msh", "_saveAll" : true}'), 
  None, None, None, None, None, ref 
)
ob.postUpdate()

lVHOstateHandler.clear()
