"""
Create a cuboid by translating and skinning a base surface. 
Generate a  mesh of the geometry with a grading on a single edge.

.. _gradingCuboid:
.. figure:: img/gradingCuboid.png
   :width: 600
   :align: center

   The final mesh generated in this test. Visible is the graded edge as well as
   the surfaces "top" (orange), "skin_1" (yellow) and "skin_5" (green).


Import ``dtOO`` and ``numpy``:

>>> import dtOOPythonSWIG as dtOO
>>> import numpy as np

Define ``dtOO``'s options:

>>> dtOO.staticPropertiesHandler.getInstance().jInit(
...   dtOO.jsonPrimitive(
...     '{'
...       '"option" : ['
...         '{"name" : "reparamOnFace_precision", "value" : "1.e-05"},'
...         '{"name" : "reparamInVolume_precision","value" : "1.e-05"},'
...         '{"name" : "invY_precision", "value" : "1.e-04"},'
...         '{"name" : "xyz_resolution", "value" : "1.e-05"},'
...         '{"name" : "XYZ_resolution", "value" : "1.e-04"},'
...         '{"name" : "uvw_resolution", "value" : "1.e-04"},'
...         '{"name" : "point_render_diameter", "value" : "0.005"},'
...         '{"name" : "vector_render_size", "value" : "0.05"},'
...         '{"name" : "function_render_resolution_u", "value" : "21"},'
...         '{"name" : "function_render_resolution_v", "value" : "21"},'
...         '{"name" : "function_render_resolution_w", "value" : "21"},'
...         '{"name" : "geometry_render_resolution_u", "value" : "21"},'
...         '{"name" : "geometry_render_resolution_v", "value" : "21"},'
...         '{"name" : "geometry_render_resolution_w", "value" : "21"},'
...         '{"name" : "ompNumThreads", "value" : "2"},'
...         '{"name" : "map1dTo3d_deltaPer", "value" : "0.01"},'
...         '{"name" : "map2dTo3d_deltaPer", "value" : "0.01"},'
...         '{"name" : "map3dTo3d_deltaPer", "value" : "0.01"},'
...         '{"name" : "logLevel", "value" : "0"},'
...         '{"name" : "isEqualExtendCheck", "value" : "false"}'
...       ']'
...     '}'
...   )
... )

Create a log file:

>>> dtOO.logMe.initLog('gradingCuboid.log')
'gradingCuboid.log'

Create containers for object handling:

>>> container = dtOO.dtBundle()
>>> cV = container.cptr_cV()

>>> dtOO.lVHOstateHandler.clear()
>>> dtOO.lVHOstateHandler( dtOO.jsonPrimitive(), cV ).thisown = False

>>> aF = container.cptr_aF()
>>> aG = container.cptr_aG()
>>> bV = container.cptr_bV()
>>> dP = container.cptr_dP()

Create an array for the corners of the base surface:

>>> corner = [[0.00, 0.00, 0.00,],
...           [2.00, 0.50, 0.50,],
...           [2.00, 0.25, 1.25,],
...           [0.10, 0.00, 1.25,],]

Create 4 B-Splines connecting the corners:

>>> curve01 = dtOO.bSplineCurve_pointConstructOCC(
...     dtOO.vectorDtPoint3()
...       << dtOO.dtPoint3(corner[0][0],corner[0][1],corner[0][2])
...       << dtOO.dtPoint3((corner[0][0]+corner[1][0])/2*0.9,
...                        (corner[0][1]+corner[1][1])/2*0.9,
...                        (corner[0][2]+corner[1][2])/2*0.9)
...       << dtOO.dtPoint3(corner[1][0],corner[1][1],corner[1][2]),
...     1
... ).result()

>>> curve12 = dtOO.bSplineCurve_pointConstructOCC(
...     dtOO.vectorDtPoint3()
...       << dtOO.dtPoint3(corner[1][0],corner[1][1],corner[1][2])
...       << dtOO.dtPoint3((corner[1][0]+corner[2][0])/2*0.9,
...                        (corner[1][1]+corner[2][1])/2*0.9,
...                        (corner[1][2]+corner[2][2])/2*0.9)
...       << dtOO.dtPoint3(corner[2][0],corner[2][1],corner[2][2]),
...     1
... ).result()

>>> curve23 = dtOO.bSplineCurve_pointConstructOCC(
...     dtOO.vectorDtPoint3()
...       << dtOO.dtPoint3(corner[2][0],corner[2][1],corner[2][2])
...       << dtOO.dtPoint3((corner[2][0]+corner[3][0])/2*0.9,
...                        (corner[2][1]+corner[3][1])/2*0.9,
...                        (corner[2][2]+corner[3][2])/2*0.9)
...       << dtOO.dtPoint3(corner[3][0],corner[3][1],corner[3][2]),
...      1
... ).result()

>>> curve30 = dtOO.bSplineCurve_pointConstructOCC(
...     dtOO.vectorDtPoint3()
...       << dtOO.dtPoint3(corner[3][0],corner[3][1],corner[3][2])
...       << dtOO.dtPoint3((corner[3][0]+corner[0][0])/2*0.9,
...                        (corner[3][1]+corner[0][1])/2*0.9,
...                        (corner[3][2]+corner[0][2])/2*0.9)
...       << dtOO.dtPoint3(corner[0][0],corner[0][1],corner[0][2]),
...      1
... ).result()

Create the base surface as a B-Spline surface from the four curves:

>>> surf0 = dtOO.bSplineSurface_bSplineCurveFillConstructOCC(
...         curve01,
...         curve12,
...         curve23,
...         curve30,
...         ).result()

Create an analytic surface and label it with the string "base":
    
>>> base = dtOO.analyticSurface(surf0)
>>> base.setLabel("base")

Deposit a clone of the surface in ``aG``:

>>> aG.push_back( base.clone() )

Create the top surface by translating the base surface.
The command ``geomSurface_surfaceTranslateConstructOCC`` takes a surface and a translation vector as input:

>>> surf1 = dtOO.geomSurface_surfaceTranslateConstructOCC(
...          base.ptrConstDtSurface(),
...          dtOO.dtVector3(0,1,0)
...         ).result()

Create an analytic surface, label it with the string "top" and deposit it in ``aG``:

>>> top = dtOO.analyticSurface(surf1)
>>> top.setLabel("top")
>>> aG.push_back( top.clone() )

Create a vector and fill it with the handles of the "base" and "top" surfaces:

>>> vhs = dtOO.vectorHandlingConstDtSurface()
>>> vhs.push_back( base.ptrConstDtSurface() )
>>> vhs.push_back( top.ptrConstDtSurface() )

Create surfaces of the cuboid by skinning the base and the top surface.
The builder ``bSplineSurfaces_bSplineSurfaceSkinConstructOCC`` takes the vector vhs as input:

>>> vh = dtOO.bSplineSurfaces_bSplineSurfaceSkinConstructOCC(vhs).result()

Iterate over the skinned surfaces in order to create analytic surfaces. Label and deposit them in the ``aG`` container.
The skinned surfaces are labeled with the string ``skin_`` plus their iterator:

>>> p = 0
>>> for i in vh:
...   aa = dtOO.analyticSurface( i )
...   aa.setLabel("skin_"+str(p))
...   aG.push_back( aa.clone() )      
...   p = p + 1

Create and initialize the volume for meshing:

>>> ref = dtOO.map3dTo3dGmsh()
>>> ref.thisown = False
>>> ref.jInit(
...   dtOO.jsonPrimitive(
...     '{"label" : "cBoundVol",'
...         '"option" : ['
...         '{"name" : "[gmsh]General.Terminal", "value" : "1."},'
...         '{"name" : "[gmsh]General.Verbosity", "value" : "100."},'
...         '{"name" : "[gmsh]General.ExpertMode", "value" : "1."},'
...         '{"name" : "[gmsh]Mesh.CharacteristicLengthMin", "value" : "0.05"},'   
...         '{"name" : "[gmsh]Mesh.CharacteristicLengthMax", "value" : "0.1"},'
...         '{"name" : "[gmsh]Mesh.Algorithm", "value" : "1"},'
...         '{"name" : "[gmsh]Mesh.MeshSizeExtendFromBoundary", "value" : "1"},'
...         '{"name" : "[gmsh]Mesh.MeshSizeFromPoints", "value" : "1"}'
...       '],'
...       '"analyticGeometry" : []'
...     '}'),
...     None, None, None, None, None
... )

Add the analytic geometries to the Gmsh model.
Iterate over all analytic geometries and add them to the model with ``addIfFaceToGmshModel``.
The sucessfully added geometries are saved in the vector fid:

>>> fid = dtOO.vectorInt()
>>> for a_i in aG:
...   tid = ref.getModel().addIfFaceToGmshModel( dtOO.map2dTo3d.MustConstDownCast( a_i ) ) 
...   if tid!=0:                    
...     fid.push_back(tid)         

Add a volume to the Gmsh model with ``addIfRegionToGmshModel``. 
The volume is defined by the geometries in vector fid:

>>> ref.getModel().addIfRegionToGmshModel(dtOO.infinityMap3dTo3d(), fid)
1

Define and initialize the mesh boundaries by handing the surface labels to Gmsh.
"skin_0" and "skin_2" are the same geometries as "base" and "top" and are not handed over.
The specifier ``_facesPerEntry`` defines how many faces each geometry has. 
According to the number of faces a indice is added (for example: "base_0" for the first face of "base").:

>>> ob = dtOO.bVOAnalyticGeometryToFace() 
>>> ob.jInit(
...   dtOO.jsonPrimitive(
...     '{'
...       '"analyticGeometry" : ['
...         '{"label" : "base"},'    
...         '{"label" : "top"},'
...         '{"label" : "skin_1"},'  
...         '{"label" : "skin_3"},'
...         '{"label" : "skin_4"},'
...         '{"label" : "skin_5"}'
...       '],'
...       '"_inc" : 10.0,'                     
...       '"_facesPerEntry" : [1,1,1,1,1,1]'  
...     '}'                                  
...   ),
...   None, None, None, aG, None, ref
... )
>>> ob.preUpdate()

Set the number of elements to 10 on all edges by iterating over them:

>>> for e in ref.getModel().dtEdges():                                                          
...   e.meshTransfiniteWNElements(1,1,10)                                                       

Set the number of elments to 20 on the edges connecting the top and the bottom surface.
The method ``getDtGmshEdgeTagListByFromToPhysical`` takes two surfaces and returns the tag list of the edges between them.
By iterating through the tag list the number of elements is set on each edge.
Set the surface meshes on the sides to transfinite:

>>> eID = ref.getModel().getDtGmshEdgeTagListByFromToPhysical("base*","top*")         
>>> for e in eID:
...   ref.getModel().getDtGmshEdgeByTag( e ).meshTransfiniteWNElements(1,1.0,20)                
...   for f in ref.getModel().getDtGmshEdgeByTag( e ).dtFaces():
...     f.meshTransfinite()

Make the "base" surface transfinite with ``meshTransfinite`` and recombine it with ``meshRecombine``.
This will generate hex elements in the transfinite layer.

>>> ref.getModel().getDtGmshFaceByPhysical("base_0").meshTransfinite()  
>>> ref.getModel().getDtGmshFaceByPhysical("base_0").meshRecombine() 

Set a grading on the edge between "skin_1" and "skin_5". The Edge is found by comparing the tags of the edges of both surfaces.
The common edge of both surfaces will have the same absolute value of the tag.
The grading is set with the command ``setGrading`` the second entry in this command is an identifier:

>>> edges_1 = ref.getModel().getDtGmshFaceByPhysical("skin_1_0").dtEdges()
>>> edges_2 = ref.getModel().getDtGmshFaceByPhysical("skin_5_0").dtEdges()
>>> for e1 in edges_1:
...     for e2 in edges_2:
...         if np.abs(e1.tag()) == np.abs(e2.tag()):
...             e1.setGrading(1.0, 3)
 
Define the grading function. The method ``scaTanhGradingOneD(c, g, gMin, gMax)`` builds a tanh-function.
The function is defined by the following formula:

.. math::

    f(x)=c[0]+c[1]*tanh(g*(c[2]+c[3]*x))/tanh(g)


>>> gradingFun = dtOO.scaTanhGradingOneDCompound(
...                 dtOO.scaTanhGradingOneD(
...                     dtOO.vectorReal([0.5, 0.5, -1.0, 2.0]),
...                                     1.0,
...                                     1.0, 5.0
...                     )
...                 )

Label the grading function and push it into ``aF``:

>>> gradingFun.setLabel( "aF_grading" )
>>> aF.push_back( gradingFun.clone() )

Define the grading. The ``_type`` is the identifier specified in ``setGrading``.
``_firstElementSize`` sets the size of the first element in the grading. The grading function is specified with ``_grading``:

>>> ob = dtOO.bVOSetPrescribedElementSize()
>>> ob.thisown = False
>>> ob.jInit( 
...     dtOO.jsonPrimitive(
...         '{'
...         '"_type": 3,'
...         '"_firstElementSize": 0.01,'
...         '"_grading" : {'
...             '"analyticFunction" : {"label" : "aF_grading"}'
...           '}'
...         '}'
...     ),  
...     None, None, aF, None, None, ref
... )         
>>> ref.attachBVObserver(ob)

Define the mesh rules.
The following rules are set:

    ``dtMeshFreeGradingGEdge``:
        Connects the specified grading numbers and functions.
        
        ``typeTransfinite`` contains an array of all the grading identifiers
        
        ``gradingFunctions`` contains an array with all the analytic functions of the gradings

    ``dtMeshGFace``:
        Creates the mesh of the "base" and the "top" surfaces.
        
    ``dtMeshGFaceWithTransfiniteLayer``:
        Creates the mesh of the other surfaces with transfinite layers.
        
        ``_nLayers`` specifies the number of layers on the opposing ends of the face.
        
        ``_direction`` specifies the direction of the layers (parametric u or v).
        
        ``_nSmooth`` specifies the number of smoothing iterations.

    ``dtMeshGRegionWithBoundaryLayer``:
       Creates the mesh of the volume between the surfaces.
       
       ``_nSpacingSteps`` specifies the layers in the Volume (has to be -1*(``_nLayers``-1)).
       
       ``_faceLabel`` specifies the face where the layers are.
       
       ``_slidableFaceLabel`` specifies the faces where the mesh is built adaptively.
       
       ``dtMesh3DOperator`` specifies the mesh operator used for the transition of the layers to the tetraedral mesh.
       
       ``_fixedFaceLabel`` specifies the face on the opposite side to ``_faceLabel``.
    
    ``dtMeshGRegion``:
        Creates the volume mesh, 
        specifically handles the pyramid elements between the hex-layer and the tet-elements.


>>> ob = dtOO.bVOMeshRule()                 
>>> ob.thisown = False
>>> ob.jInit(
...   dtOO.jsonPrimitive(
...     '{'
...       '"option" : ['
...         '{"name" : "debug", "value" : "true"}'
...       '],'
...       '"_rule1D" : ['
...         '"dtMeshFreeGradingGEdge(*)"'
...       '],'
...       '"_rule2D" : ['
...         '"dtMeshGFace(base_0)",'
...         '"dtMeshGFace(top_0)",'
...         '"dtMeshGFaceWithTransfiniteLayer(*)"'
...       '],'
...       '"_rule3D" : ['
...         '"dtMeshGRegionWithBoundaryLayer(*)"'
...       '],'
...       '"_only" : [],'
...       '"dtMeshOperator" : ['
...         '{'
...           '"name" : "dtMeshFreeGradingGEdge",'
...           '"label" : "dtMeshFreeGradingGEdge",'
...           '"typeTransfinite" : [3],'
...           '"gradingFunctions" : { "analyticFunction" : ['
...             '{"label" : "aF_grading"}'
...           ']}'
...         '},'
...         '{'
...           '"name" : "dtMeshGFace",'
...           '"label" : "dtMeshGFace"'
...         '},'
...         '{'
...           '"name" : "dtMeshGFaceWithTransfiniteLayer",'
...           '"label" : "dtMeshGFaceWithTransfiniteLayer",'
...           '"_nLayers" : [ 10, 0],'  
...           '"_direction" : 0,'   
...           '"_nSmooth" : 1000'   
...         '},'
...         '{'
...           '"name" : "dtMeshGRegion",'
...           '"label" : "dtMeshGRegion",'
...           '"_minQShapeMetric" : 0.0,'
...           '"_relax" : 0.1,'
...           '"_nPyramidOpenSteps" : 10,'
...           '"_nSmooths" : 3'
...         '},'
...         '{'
...           '"option" : ['
...             '{"name" : "debug", "value" : "true"}'
...           '],'
...           '"name" : "dtMeshGRegionWithBoundaryLayer",'
...           '"label" : "dtMeshGRegionWithBoundaryLayer",'
...           '"_nSpacingSteps" : [-9],'
...           '"_nNormalSmoothingSteps" : 100,'
...           '"_nGrowingSmoothingSteps" : 100,'
...           '"_maxGrowingRatePerStep" : 2,'
...           '"_maxDihedralAngle" : '+str(float(np.pi*360./180.))+','
...           '"_faceLabel" : ["base_0"],"'
...           '_slidableFaceLabel" : ["*skin_*"],'
...           '"dtMesh3DOperator" : "dtMeshGRegion",'
...           '"_fixedFaceLabel" : ["top_0"]'
...         '}'
... 
...       ']'
...     '}'
...   ),
...   None, None, aF, None, None, ref
... )
>>> ref.attachBVObserver(ob)

Add the Gmsh mapping to ``bV`` and create the mesh:

>>> bV.set( ref )
>>> bV[0].makeGrid()

Create and initialize an observer to wite the mesh file:

>>> ob = dtOO.bVOWriteMSH()
>>> ob.jInit(
...   dtOO.jsonPrimitive('{"_filename" : "mesh_cuboid.msh", "_saveAll" : true}'),
...   None, None, None, None, None, ref
... )
>>> ob.postUpdate()

Clear the vector handling object:

>>> dtOO.lVHOstateHandler.clear()
"""
