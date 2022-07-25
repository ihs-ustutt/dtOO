import sys

from dtOOPythonSWIG import *

dtXmlParser.init("","")
cV = labeledVectorHandlingConstValue()

#
# intParam
#
iP = intParam()
iP.setLabel("cV_iP")
iP.setValue( 4.0 )
cV.set( iP.clone() )

#
# sliderFloatParam
#
sFP = sliderFloatParam()
sFP.setLabel("cV_sFP")
sFP.setValue( 10.5 )
cV.set( sFP.clone() )

#
# constrainedFloatParam
#
cFP = constrainedFloatParam("10.*#cV_sFP#")
cFP.setLabel("cV_cFP")
cFP.setValue( 10.0 )
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