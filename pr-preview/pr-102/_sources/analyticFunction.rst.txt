Abstract
========

These types define the general function contract and common support for input
and output vectors, compound functions, and transformed functions.

.. doxygenclass:: dtOO::analyticFunction
   :members:

.. doxygenclass:: dtOO::aFX
   :members:

.. doxygenclass:: dtOO::aFY
   :members:

.. doxygenclass:: dtOO::analyticFunctionCombination
   :members:

.. doxygenclass:: dtOO::analyticFunctionCompound
   :members:

.. doxygenclass:: dtOO::analyticFunctionCompoundTrojanHorse
   :members:

.. doxygenclass:: dtOO::analyticFunctionTransformed
   :members:

These classes define scalar, two-component vector, and three-component vector
output interfaces. The dimension-specific classes define the one-, two-, and
three-dimensional input interfaces from which concrete functions derive.

.. doxygenclass:: dtOO::scaFunction
   :members:

.. doxygenclass:: dtOO::scaOneD
   :members:

.. doxygenclass:: dtOO::scaOneDPolyInterface
   :members:

.. doxygenclass:: dtOO::scaTwoD
   :members:

.. doxygenclass:: dtOO::scaThreeD
   :members:

.. doxygenclass:: dtOO::vec2dFunction
   :members:

.. doxygenclass:: dtOO::vec2dOneD
   :members:

.. doxygenclass:: dtOO::vec2dTwoD
   :members:

.. doxygenclass:: dtOO::vec3dFunction
   :members:

.. doxygenclass:: dtOO::vec3dOneD
   :members:

.. doxygenclass:: dtOO::vec3dTwoD
   :members:

.. doxygenclass:: dtOO::vec3dThreeD
   :members:

Concrete
========

These concrete and adapting classes use ``analyticFunction`` or one of the
function superclasses to implement scalar and vector mappings.

.. doxygenclass:: dtOO::sca3PPointsBSplineOneD
   :members:

.. doxygenclass:: dtOO::scaCurve2dOneD
   :members:

.. doxygenclass:: dtOO::scaLinearOneD
   :members:

.. doxygenclass:: dtOO::scaMuParserOneD
   :members:

.. doxygenclass:: dtOO::scaMuParserThreeD
   :members:

.. doxygenclass:: dtOO::scaMuParserTwoD
   :members:

.. doxygenclass:: dtOO::scaMultiOneD
   :members:

.. doxygenclass:: dtOO::scaTanhGradingOneD
   :members:

.. doxygenclass:: dtOO::scaTanhUnitGradingOneD
   :members:

.. doxygenclass:: dtOO::transIntCube
   :members:

.. doxygenclass:: dtOO::vec2dBiLinearTwoD
   :members:

.. doxygenclass:: dtOO::vec2dCurve2dOneD
   :members:

.. doxygenclass:: dtOO::vec2dMuParserTwoD
   :members:

.. doxygenclass:: dtOO::vec2dMultiBiLinearTwoD
   :members:

.. doxygenclass:: dtOO::vec2dSurface2dTwoD
   :members:

.. doxygenclass:: dtOO::vec3dBiLinearTwoD
   :members:

.. doxygenclass:: dtOO::vec3dBoxThreeD
   :members:

.. doxygenclass:: dtOO::vec3dCurve2dInSurfaceOneD
   :members:

.. doxygenclass:: dtOO::vec3dCurveOneD
   :members:

.. doxygenclass:: dtOO::vec3dMuParserOneD
   :members:

.. doxygenclass:: dtOO::vec3dMuParserThreeD
   :members:

.. doxygenclass:: dtOO::vec3dMuParserTwoD
   :members:

.. doxygenclass:: dtOO::vec3dMultiThreeD
   :members:

.. doxygenclass:: dtOO::vec3dSurfaceTwoD
   :members:

.. doxygenclass:: dtOO::vec3dThickedTwoD
   :members:

.. doxygenclass:: dtOO::vec3dTransVolThreeD
   :members:

.. doxygenclass:: dtOO::vec3dTriLinearThreeD
   :members:
