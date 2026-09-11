*****************
API documentation
*****************

The documentation of the C++ code.

C++ core classes
================

The C++ core provides the building blocks used to define and process a dtOO
model. ``constValue`` stores named parameters, while ``analyticFunction``
describes mappings between parameter and result vectors. ``analyticGeometry``
is the common base for parameterized curves, surfaces, and volumes embedded in
three-dimensional space; its ``map1dTo3d``, ``map2dTo3d``, and ``map3dTo3d``
specializations provide the corresponding one-, two-, and three-dimensional
mapping interfaces.

``boundedVolume`` combines geometries into meshable regions, ``dtCase`` holds
the simulation setup, and ``dtPlugin`` provides extension points for
model-processing operations.

The following classes define named parameters, analytic functions and
geometries, and the bounded regions built from those geometries.

Geometry
--------

Analytic geometries map a parameter domain into three-dimensional Cartesian
space. ``analyticGeometry`` defines the common geometry contract. Its map
superclasses specialize that contract by parameter-space dimension:
``map1dTo3d`` represents curves parameterized by ``u``, ``map2dTo3d``
represents surfaces parameterized by ``u`` and ``v``, and ``map3dTo3d``
represents volumes parameterized by ``u``, ``v``, and ``w``.

All maps evaluate to three-dimensional points, irrespective of the dimension
of their parameter domain. Concrete and adapting geometries below use these
superclasses to provide specific representations and operations.

.. toctree::
   :maxdepth: 2

   analyticGeometry

.. toctree::
   :maxdepth: 2

   analyticGeometryBuilders

Function
--------

Analytic functions map an input vector ``aFX`` to an output vector ``aFY``.
The function family name specifies the output type: ``scaFunction`` has one
scalar output, while ``vec2dFunction`` and ``vec3dFunction`` have two- and
three-component vector outputs. The ``OneD``, ``TwoD``, and ``ThreeD`` suffixes
specify the dimension of the input parameter space, not the output dimension.

For example, ``scaThreeD`` maps three input coordinates to one scalar, whereas
``vec3dOneD`` maps one input coordinate to a three-component vector. Concrete
and adapting functions below use the shared superclasses to provide specific
representations and operations.

.. toctree::
   :maxdepth: 2

   analyticFunction

.. toctree::
   :maxdepth: 2

   analyticFunctionBuilders

Other core classes
------------------

.. toctree::
   :maxdepth: 1

   constValue
   boundedVolume

Execution and extension
-----------------------

These classes represent simulation cases and extensions that process a model.

.. toctree::
   :maxdepth: 1

   dtCase
   dtPlugin

Python pyDtOO classes
=====================

The documentation of the pyDtOO package.

.. automodule:: pyDtOO.dtClusteredSingletonState
    :members:

.. _dtOOPythonApp:

Python dtOOPythonApp classes
============================

The documentation of the dtOOPythonApp package.

The following classes are used in the demonstration case
of the :ref:`radial turbine <radial_turbine_demo>`.

.. _analyticGeometry_piecewiseMeridionalRotContour:

.. automodule:: dtOOPythonApp.builder.analyticGeometry_piecewiseMeridionalRotContour
  :members:
  :special-members: __init__
  :member-order: bysource

.. _vec3dThreeD_skinAndSplit:

.. automodule:: dtOOPythonApp.builder.vec3dThreeD_skinAndSplit
  :members:
  :member-order: bysource

.. _analyticSurface_inOutFeMeanplane:

.. automodule:: dtOOPythonApp.builder.analyticSurface_inOutFeMeanplane
  :members:
  :member-order: bysource

.. _multipleBoundedVolume_gridChannel:

.. automodule:: dtOOPythonApp.builder.multipleBoundedVolume_gridChannel
  :members:
  :member-order: bysource

.. _map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks:

.. automodule:: dtOOPythonApp.builder.map3dTo3dGmsh_gridFromMultipleBoundedVolumeAndBlocks
  :members:
  :member-order: bysource

.. _analyticGeometry_layerRegion:

.. automodule:: dtOOPythonApp.builder.analyticGeometry_layerRegion
  :members:
  :special-members: __init__
  :member-order: bysource

.. _map3dTo3dGmsh_gridFromLayers:

.. automodule:: dtOOPythonApp.builder.map3dTo3dGmsh_gridFromLayers
  :members:
  :member-order: bysource
