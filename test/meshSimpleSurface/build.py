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
bC = baseContainer()
cV = labeledVectorHandlingConstValue()
aF = labeledVectorHandlingAnalyticFunction()
aG = labeledVectorHandlingAnalyticGeometry()
bV = labeledVectorHandlingBoundedVolume()
dC = labeledVectorHandlingDtCase()
dP = labeledVectorHandlingDtPlugin()

lVHOstateHandler( jsonPrimitive(), cV ).thisown = False

vh = vectorHandlingConstDtCurve()

lab = "0"
pp = vectorDtPoint3()
pp.push_back( dtPoint3( +0.05, +0.25, +0.00 ) )
pp.push_back( dtPoint3( +0.05, +1.00, +0.00 ) )
pp.push_back( dtPoint3( +0.00, +1.00, +0.00 ) )
ref = analyticCurve( bSplineCurve_pointConstructOCC(pp, 2).result() )
ref.setLabel("s3_"+lab)
aG.set( ref.clone() )
vh.push_back( analyticCurve.DownCast( aG.get("s3_"+lab) ).ptrConstDtCurve() )

lab = "1"
pp = vectorDtPoint3()
pp.push_back( dtPoint3( +0.00, +1.00, +0.00 ) )
pp.push_back( dtPoint3( +0.00, +2.00, +0.00 ) )
ref = analyticCurve( bSplineCurve_pointConstructOCC(pp, 1).result() )
ref.setLabel("s3_"+lab)
aG.set( ref.clone() )
vh.push_back( analyticCurve.DownCast( aG.get("s3_"+lab) ).ptrConstDtCurve() )

lab = "2"
pp = vectorDtPoint3()
pp.push_back( dtPoint3( +0.00, +2.00, +0.00 ) )
pp.push_back( dtPoint3( +2.00, +2.00, +0.00 ) )
pp.push_back( dtPoint3( +2.00, +0.00, +0.00 ) )
ref = analyticCurve( bSplineCurve_pointConstructOCC(pp, 2).result() )
ref.setLabel("s3_"+lab)
aG.set( ref.clone() )
vh.push_back( analyticCurve.DownCast( aG.get("s3_"+lab) ).ptrConstDtCurve() )

lab = "3"
pp = vectorDtPoint3()
pp2 = vectorDtPoint3()
pp.push_back( dtPoint3( +2.00, +0.00, +0.00 ) )
pp.push_back( dtPoint3( +0.30, +0.00, +0.00 ) )
pp.push_back( dtPoint3( +0.30, +0.25, +0.00 ) )
pp.push_back( dtPoint3( +0.05, +0.25, +0.00 ) )

ref = analyticCurve(
  bSplineCurve_poleWeightKnotMultOrderConstructOCC(
    pp,
    vectorReal([1.0, 4.0, 1.0, 1.0]),
    vectorReal([0.0, 0.5, 1.0]),
    vectorInt([3,1,3]), # [k+1, ... , k+1] 
    2,
    False
  ).result()
)

ref.setLabel("s3_"+lab)
aG.set( ref.clone() )
vh.push_back( analyticCurve.DownCast( aG.get("s3_"+lab) ).ptrConstDtCurve() )

ref = analyticSurface(
 bSplineSurface_geomCurveFillConstructOCC(vh).result()
)
ref.setLabel("aS_0")
aG.set( ref.clone() )

tt = translate(jsonPrimitive('{"_v3" : { "dtVector3" : {"x" : 0.0, "y" : 2.1, "z" : 0.0} } }'))
ref = tt.applyAnalyticGeometry( ref.clone() )
ref.setLabel("aS_1")
aG.set( ref.clone() )

#for ii in ["alpha1", "alpha2", "beta1", "beta2", "beta3", ]:
#cV.set( sliderFloatParam("cV_tf_alpha", 5.00000, 0.1, 10.0).clone() )
#cV.set( sliderFloatParam("cV_tf_beta",  1.0000, 0.1, 10.0).clone() )

#fRef = scaMuParserOneD("(1.-xx) * exp(-("+str(cV["cV_tf_alpha"]())+")*xx)", "xx")
fRef = scaMuParserOneD("1.-xx", "xx")
#fRef = scaMuParserOneD("(1.-xx)*exp(-8.0*xx)", "xx")
fRef.setLabel("aF_tf_alpha1")
fRef.setMin(+0.0)
fRef.setMax(+1.0)
aF.set( fRef.clone() )

#fRef = scaMuParserOneD("(xx) * exp(-("+str(cV["cV_tf_alpha"]())+")*(1.-xx))", "xx")
fRef = scaMuParserOneD("xx", "xx")
#fRef = scaMuParserOneD("xx*exp(-8.0*(1.-xx))", "xx")
fRef.setLabel("aF_tf_alpha2")
fRef.setMin(+0.0)
fRef.setMax(+1.0)
aF.set( fRef.clone() )

#fRef = scaMuParserOneD(
#  '1. - ('
#    '(exp('+str(cV["cV_tf_beta"]())+'* xx)- 1 -('+str(cV["cV_tf_beta"]())+')* xx)'
#    '/'
#    '(exp('+str(cV["cV_tf_beta"]())+')- 1 - ('+str(cV["cV_tf_beta"]())+'))'
#  ')',
#  "xx"
#)
#fRef = scaMuParserOneD("1.-xx", "xx")
fRef = scaMuParserOneD("(1.-xx)*exp(-3.0*xx)", "xx")
fRef.setLabel("aF_tf_beta1")
fRef.setMin(+0.0)
fRef.setMax(+1.0)
aF.set( fRef.clone() )

#fRef = scaMuParserOneD(
#  'xx - ('
#    '(exp('+str(cV["cV_tf_beta"]())+'* xx)- 1 -('+str(cV["cV_tf_beta"]())+')* xx)'
#    '/'
#    '(exp('+str(cV["cV_tf_beta"]())+') - 1 - ('+str(cV["cV_tf_beta"]())+'))'
#  ')',
#  "xx"
#)
fRef = scaMuParserOneD("0.0 * xx", "xx")
fRef.setLabel("aF_tf_beta2")
fRef.setMin(+0.0)
fRef.setMax(+1.0)
aF.set( fRef.clone() )

#fRef = scaMuParserOneD(
#  '('
#    '(exp('+str(cV["cV_tf_beta"]())+'* xx)- 1 -('+str(cV["cV_tf_beta"]())+')* xx)'
#    '/'
#    '(exp('+str(cV["cV_tf_beta"]())+') - 1 - ('+str(cV["cV_tf_beta"]())+'))'
#  ')',
#  "xx"
#)
#fRef = scaMuParserOneD("xx", "xx")
fRef = scaMuParserOneD("xx*exp(-3.0*(1.-xx))", "xx")
fRef.setLabel("aF_tf_beta3")
fRef.setMin(+0.0)
fRef.setMax(+1.0)
aF.set( fRef.clone() )


ref = customGmsh()
ref.thisown = False
ref.jInit(
  jsonPrimitive(
    '{"label" : "cBoundVol",' 
      '"option" : ['
        #<option name="[gmsh]General.Terminal" value="1."/>
        '{"name" : "[gmsh]General.Terminal", "value" : "1."},'
        # <option name="[gmsh]General.Verbosity" value="100."/>
        '{"name" : "[gmsh]General.Verbosity", "value" : "100."},'
        #<option name="[gmsh]General.ExpertMode" value="1."/>
        '{"name" : "[gmsh]General.ExpertMode", "value" : "1."},'
        #<option name="[gmsh]Mesh.LcIntegrationPrecision" value="#cV_ru_lcIntPrec#"/>
#        '{"name" : "[gmsh]Mesh.LcIntegrationPrecision", "value" : "'+str(cV["cV_ru_lcIntPrec"]())+'"},'
        #<option name="[gmsh]Mesh.CharacteristicLengthMin" value="#cV_ru_charLMin#"/>
        '{"name" : "[gmsh]Mesh.CharacteristicLengthMin", "value" : "0.05"},'
        #<option name="[gmsh]Mesh.CharacteristicLengthMax" value="#cV_ru_charCLMax#*#cV_ru_charLMin#"/>
        '{"name" : "[gmsh]Mesh.CharacteristicLengthMax", "value" : "0.1"},'
        #<option name="[gmsh]Mesh.Algorithm" value="1"/>
        '{"name" : "[gmsh]Mesh.Algorithm", "value" : "1"},'
        '{"name" : "[gmsh]Mesh.MeshSizeExtendFromBoundary", "value" : "1"},'
        '{"name" : "[gmsh]Mesh.MeshSizeFromPoints", "value" : "1"}'
      ']'
  '}'),
  bC, cV, aF, aG, bV
)

ob = bVOSetPrescribedMeshSizeAtPoints()
ob.thisown = False
ob.jInit( 
  jsonPrimitive( 
    '{'
      '"_meshSize" : [ 0.05 ]'
    '}' 
  ), 
  bC, cV, aF, aG, bV, ref 
)
ref.attachBVObserver(ob)

ob = bVOAnalyticGeometryToFace()
ob.thisown = False
ob.jInit( 
  jsonPrimitive(
    '{'
      '"analyticGeometry" : ['
        '{"label" : "aS_0"},'
        '{"label" : "aS_1"}'
      '],'
      '"_inc" : 10.0'
    '}'
  ), 
  bC, cV, aF, aG, bV, ref
)
ref.attachBVObserver(ob)

ob = bVOTransfiniteFaces()
ob.thisown = False
ob.jInit( 
  jsonPrimitive( 
    '{'
      '"_faceLabel" : ['
        '"aS_0_0", "aS_1_0"'
      '],'
      '"_nE" : ['
        '20,41'
      ']'
    '}' 
  ), 
  bC, cV, aF, aG, bV, ref 
)
ref.attachBVObserver(ob)

ob = bVOSetGrading()
ob.thisown = False
ob.jInit( 
  jsonPrimitive( 
    '{'
      '"_edgeLabel" : [],'
      '"_regionLabel" : [],'
      '"_faceLabel" : ['
        '"aS_0_0", "aS_1_0"'
      '],'
      '"_grading" : ['
        '0.8,1.05,1.0,1.1'
      '],'
       '"_type" : ['
        '1.0,1.0,1.0,1.0'
      ']'
    '}' 
  ), 
  bC, cV, aF, aG, bV, ref 
)
ref.attachBVObserver(ob)

ob = bVORecombine()
ob.thisown = False
ob.jInit( 
  jsonPrimitive( 
    '{'
      '"_faceLabel" : ['
        '"aS_0_0", "aS_1_0"'
      ']'
    '}' 
  ), 
  bC, cV, aF, aG, bV, ref 
)
ref.attachBVObserver(ob)

ob = bVODumpModel()
ob.thisown = False
ob.jInit(
  jsonPrimitive(), bC, cV, aF, aG, bV, ref 
)
ref.attachBVObserver(ob)

ob = bVOMeshRule()
ob.thisown = False
ob.jInit(
  jsonPrimitive( 
    '{'
      '"option" : ['
        '{"name" : "debug", "value" : "true"}'
      '],'
      '"_rule1D" : ['
        '"dtMeshGEdge(aS_0_0->*)",'
        '"dtMeshGEdge(aS_1_0->*)"'
      '],'
      '"_rule2D" : ['
        '"dtMeshCustom2x3TransfiniteGFace(aS_1_0)",'
        '"dtMeshTransfiniteGFace(aS_0_0)"'
      '],'
      '"_rule3D" : ['
        '"dtMeshGRegion(*)"'
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
          '"name" : "dtMeshTransfiniteGFace",'
          '"label" : "dtMeshTransfiniteGFace"'
        '},'
        '{'
          '"name" : "dtMeshCustom2x3TransfiniteGFace",'
          '"label" : "dtMeshCustom2x3TransfiniteGFace",'
          '"_alpha_1" : {"analyticFunction" : {"label" : "aF_tf_alpha1"}},'
          '"_alpha_2" : {"analyticFunction" : {"label" : "aF_tf_alpha2"}},'
          '"_alpha_3" : {"analyticFunction" : {"label" : "aF_tf_alpha3"}},'
          '"_beta_1" : {"analyticFunction" : {"label" : "aF_tf_beta1"}},'
          '"_beta_2" : {"analyticFunction" : {"label" : "aF_tf_beta2"}},'
          '"_beta_3" : {"analyticFunction" : {"label" : "aF_tf_beta3"}}'
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
  bC, cV, aF, aG, bV, ref
)
ref.attachBVObserver(ob)

ob = bVOWriteMSH()
ob.thisown = False
ob.jInit(
  jsonPrimitive( 
    '{'
      '"_filename" : "mesh.msh",'
      '"_saveAll" : true'
    '}' 
  ), 
  bC, cV, aF, aG, bV, ref 
)
ref.attachBVObserver(ob)

ref.getModel().addIfFaceToGmshModel( map2dTo3d.ConstDownCast( aG["aS_0"] ) )
ref.getModel().addIfFaceToGmshModel( map2dTo3d.ConstDownCast( aG["aS_1"] ) )

bV.set( ref )

bV[0].makeGrid()
