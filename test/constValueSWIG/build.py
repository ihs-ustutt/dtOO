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

dtXmlParser.init("","")
cV = labeledVectorHandlingConstValue()
lVHOstateHandler( jsonPrimitive(), cV ).thisown = False

#
# intParam
#
iP = intParam("cV_iP", 4 )
cV.set( iP.clone() )

#
# sliderFloatParam
#
sFP = sliderFloatParam( "cV_sFP", 10.5 )
cV.set( sFP.clone() )

#
# constrainedFloatParam
#
cFP = constrainedFloatParam("cV_cFP", "10.*#cV_sFP#")
cFP.resolveConstraint(cV)
cV.set( cFP.clone() )

print("intParam")
if cV.get("cV_iP").getValue()==4:
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)

print("sliderFloatParam")
if cV.get("cV_sFP").getValue()==10.5:
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)

print("constrainedFloatParam (1/2)")
if cV.get("cV_cFP").getValue()==105.0:
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)

cV.get("cV_sFP").setValue( 5.25 )
print("constrainedFloatParam (2/2)")
if cV.get("cV_cFP").getValue()==52.5:
  print("-> OK")
else:
  print("-> FAIL")
  sys.exit(-1)
