Abstract
========

These classes define the general behavior shared by all analytic geometries,
including labels, rendering, bounds, normalized percent coordinates, and
compound geometry handling.

.. doxygenclass:: dtOO::analyticGeometry
   :members:

.. doxygenclass:: dtOO::analyticGeometryCompound
   :members:

.. doxygenclass:: dtOO::analyticGeometryGroup
   :members:

These classes define the curve, surface, and volume interfaces. They provide
the common evaluation, derivative, parameter-bound, and reparameterization
operations for their respective parameter-space dimensions.

.. doxygenclass:: dtOO::map1dTo3d
   :members:

.. doxygenclass:: dtOO::map2dTo3d
   :members:

.. doxygenclass:: dtOO::map3dTo3d
   :members:

Concrete
========

These concrete and adapting classes use ``analyticGeometry`` or one of the
map superclasses to represent curves, surfaces, volumes, transformations, and
function-based geometries.

.. doxygenclass:: dtOO::analyticCurve
   :members:

.. doxygenclass:: dtOO::analyticRotatingMap1dTo3d
   :members:

.. doxygenclass:: dtOO::analyticSurface
   :members:

.. doxygenclass:: dtOO::infinityMap3dTo3d
   :members:

.. doxygenclass:: dtOO::map1dTo3dTransformed
   :members:

.. doxygenclass:: dtOO::map2dTo3dTransformed
   :members:

.. doxygenclass:: dtOO::map3dTo3dTransformed
   :members:

.. doxygenclass:: dtOO::multipleBoundedSurface
   :members:

.. doxygenclass:: dtOO::multipleBoundedVolume
   :members:

.. doxygenclass:: dtOO::partRotatingMap1dTo3d
   :members:

.. doxygenclass:: dtOO::partRotatingMap2dTo3d
   :members:

.. doxygenclass:: dtOO::radialTranslatingMap2dTo3d
   :members:

.. doxygenclass:: dtOO::rotatingMap1dTo3d
   :members:

.. doxygenclass:: dtOO::rotatingMap2dTo3d
   :members:

.. doxygenclass:: dtOO::scaOneDInMap1dTo3d
   :members:

.. doxygenclass:: dtOO::trans4SidedFace
   :members:

.. doxygenclass:: dtOO::trans6SidedCube
   :members:

.. doxygenclass:: dtOO::translatingMap2dTo3d
   :members:

.. doxygenclass:: dtOO::vec2dOneDInMap2dTo3d
   :members:

.. doxygenclass:: dtOO::vec2dTwoDInMap2dTo3d
   :members:

.. doxygenclass:: dtOO::vec3dOneDInMap3dTo3d
   :members:

.. doxygenclass:: dtOO::vec3dThreeDInMap3dTo3d
   :members:

.. doxygenclass:: dtOO::vec3dTwoDInMap3dTo3d
   :members:
