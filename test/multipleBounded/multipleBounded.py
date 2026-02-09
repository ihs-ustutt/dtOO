"""
Import ``dtOOPythonSWIG``:

>>> from dtOOPythonSWIG import *

Create a ``staticPropertiesHandler`` object and initialize:

>>> staticPropertiesHandler.getInstance().jInit(jsonPrimitive())

Create a log file:

>>> logMe.initLog('multipleBounded.log')
'multipleBounded.log'

Create a container, a vector of ``constValue`` objects and initialize a 
``lVHOstateHandler`` object:

>>> container = dtBundle()
>>> cV = container.cptr_cV()
>>> lVHOstateHandler.clear()
>>> lVHOstateHandler( jsonPrimitive(), cV ).thisown = False

Function that creates edges and faces at z-position ``zz``; the face is
rectangular with the vertices ``P0``, ``P1``, ``P2`` and ``P3``; the face is
stored in the variable ``m2d``; the five edges lie inside of this face, but 
are _not_ parameterized in the face; the vector ```m1ds`` contains all 
edges:

>>> def CreateFaceAtZ( zz ):
...   P0 = dtPoint3(-0.5,-0.5,zz)
...   P1 = dtPoint3(+1.5,-0.5,zz)
...   P2 = dtPoint3(+1.5,+1.5,zz)
...   P3 = dtPoint3(-0.5,+1.5,zz)
...
...   m2d = analyticSurface(
...     bSplineSurface_bSplineCurveFillConstructOCC(
...       bSplineCurve_pointConstructOCC(P0, P1).result(),
...       bSplineCurve_pointConstructOCC(P1, P2).result(),
...       bSplineCurve_pointConstructOCC(P2, P3).result(),
...       bSplineCurve_pointConstructOCC(P3, P0).result()
...     ).result()
...   )
...
...   p0 = dtPoint3(0.0,0.0,zz)
...   p1 = dtPoint3(1.0,0.0,zz)
...   p2 = dtPoint3(1.0,0.5,zz)
...   p3 = dtPoint3(0.5,1.0,zz)
...   p4 = dtPoint3(0.0,1.0,zz)
...   
...   bc0 = bSplineCurve_pointConstructOCC(p0, p1).result()
...   bc1 = bSplineCurve_pointConstructOCC(p1, p2).result()
...   bc2 = bSplineCurve_pointConstructOCC(p2, p3).result()
...   bc3 = bSplineCurve_pointConstructOCC(p3, p4).result()
...   bc4 = bSplineCurve_pointConstructOCC(p4, p0).result()
...   
...   m1ds = vectorHandlingAnalyticGeometry()
...   m1ds.push_back(analyticCurve(bc0).clone())
...   m1ds.push_back(analyticCurve(bc1).clone())
...   m1ds.push_back(analyticCurve(bc2).clone())
...   m1ds.push_back(analyticCurve(bc3).clone())
...   m1ds.push_back(analyticCurve(bc4).clone())
... 
...   return m1ds, m2d

Call the function twice at two different positions of ``zz``:

>>> m1ds0, m2d0 = CreateFaceAtZ(0.0)
>>> m1ds1, m2d1 = CreateFaceAtZ(1.0)

Create an empty vector of ``analyticGeometry`` objects:
    
>>> m2ds = vectorHandlingAnalyticGeometry()

Create two objects of ``multipleBoundedSurface``; the surfaces are bounded by
five edges; the first argument holds the surface information; it is important 
to keep in mind, that the edge and face is not coupled outside of the 
``multipleBoundedSurface``-object:

>>> m2ds.push_back( multipleBoundedSurface(m2d0, m1ds0).clone() )
>>> m2ds.push_back( multipleBoundedSurface(m2d1, m1ds1).clone() )

Loop through all edges of a face and create a skin surface with the two edges;
these faces are the *side* faces of the whole volume:

>>> ii = 0
>>> for m1d in m1ds0:
...     m2ds.push_back(
...       analyticSurface(
...         bSplineSurface_skinConstructOCC(
...           analyticCurve.ConstSecureCast(m1ds0[ii]).ptrDtCurve(),
...           analyticCurve.ConstSecureCast(m1ds1[ii]).ptrDtCurve()
...         ).result()
...       ).clone()
...     )
...     ii = ii+1

Create the ``multipleBoundedVolume`` object with all bounding faces and an
``infinityMap3dTo3d``; the map is a *dummy* object that servers as a function
inside of the volume; this function is not used in ``gmsh``:

>>> mv = multipleBoundedVolume(infinityMap3dTo3d(), m2ds)

Create a meshable region, set some options and initialize:

>>> ref = map3dTo3dGmsh()
>>> ref.thisown = False
>>> ref.jInit(
...   jsonPrimitive(
...     '{"label" : "multipleBounded",'
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

Get the model and add the ``multipleBoundedVolume`` object to it:

>>> gm = ref.getModel()
>>> mv_id = gm.addIfToGmshModel(mv)

Store the region in the variable ``theRegion``:

>>> theRegion = gm.getDtGmshRegionByTag(mv_id)

Set a physical tag to the region:

>>> gm.tagPhysical( theRegion, "mbv" )

#for aFace in gm.getDtGmshRegionByPhysical("mbv").dtFaces():
Iterate over all faces of the region and tag all faces with an ascending 
number; the ``dtGmshModel``'s function ``getDtGmshFaceListByPhysical`` 
searches for physical tags and returns a list; the operator ``->`` in the 
input string argument separates the face from the volume; the wildcard matches
all possible face labels:

>>> ii = 0
>>> for aFace in gm.getDtGmshFaceListByPhysical("mbv->*"):
...   gm.tagPhysical( aFace, "f_"+str(ii) )
...   ii = ii + 1

Another example of setting tags to edges:

>>> jj = 0
>>> for anEdge in gm.getDtGmshEdgeListByPhysical("mbv->*->*"):
...     gm.tagPhysical( anEdge, "e_"+str(jj) )
...     jj = jj + 1

Generate an output of the model by using ``bVODumpModel``; the output can be 
found in the generated log file:

>>> ob = bVODumpModel()
>>> ob.jInit( jsonPrimitive(), None, None, None, None, None, ref )
>>> ob.preUpdate()

Create a mesh rule; give different operators for meshing vertices, edges, and 
faces:

>>> ob = bVOMeshRule()                 
>>> ob.thisown = False
>>> ob.jInit(
...   jsonPrimitive(
...     '{'
...       '"option" : ['
...         '{"name" : "debug", "value" : "true"}'
...       '],'
...       '"_rule1D" : ['
...         '"dtMeshGEdge(*)"'
...       '],'
...       '"_rule2D" : ['
...         '"dtMeshGFace(*)"'
...       '],'
...       '"_rule3D" : ['
...         '"dtMeshGRegion(*)"'
...       '],'
...       '"_only" : [],'
...       '"dtMeshOperator" : ['
...         '{'
...           '"name" : "dtMeshGEdge",'
...           '"label" : "dtMeshGEdge"'
...         '},'
...         '{'
...           '"name" : "dtMeshGFace",'
...           '"label" : "dtMeshGFace"'
...         '},'
...         '{'
...           '"name" : "dtMeshGRegion",'
...           '"label" : "dtMeshGRegion",'
...           '"_minQShapeMetric" : 0.0,'
...           '"_relax" : 0.1,'
...           '"_nPyramidOpenSteps" : 10,'
...           '"_nSmooths" : 3'
...         '}'
...       ']'
...     '}'
...   ),
...   None, None, None, None, None, ref
... )
>>> ref.attachBVObserver(ob)

Mesh the volume:

>>> ref.makeGrid()

Create an output in ``gmsh``'s format:

>>> ob = bVOWriteMSH()
>>> ob.jInit(
...   jsonPrimitive(
...     '{"_filename" : "multipleBounded.msh", "_saveAll" : true}'
...   ),
...   None, None, None, None, None, ref
... )
>>> ob.postUpdate()

Clear the ``lVHOstateHandler`` object:

>>> lVHOstateHandler.clear()
"""
