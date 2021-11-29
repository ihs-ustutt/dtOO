import sys

from libdtOOPython import *

logMe.initLog("build.log")
dtXmlParser.init("machine.xml", "machineSave.xml")
parser = dtXmlParser.reference()
parser.parse()
bC = baseContainer()
cV = labeledVectorHandlingConstValue()
aF = labeledVectorHandlingAnalyticFunction()
aG = labeledVectorHandlingAnalyticGeometry()
bV = labeledVectorHandlingBoundedVolume()
dC = labeledVectorHandlingDtCase()
dP = labeledVectorHandlingDtPlugin()
parser.createConstValue(cV)
parser.load()
parser.loadStateToConst("myState", cV)
parser.destroyAndCreate(bC, cV, aF, aG, bV, dC, dP)

rStr = "5,7,-3"
tStr = parser.replaceDependencies("@s3_line[%](0.5)@", bC, cV, aF, aG)

print("Check > %s < should be > %s <" % (tStr, rStr) )
if  tStr == rStr:
  print("OK")
else:
  print("FAIL")
  sys.exit(-1)

rStr = "54"
tStr = parser.replaceDependencies("$aF_muparser(1.)$", bC, cV, aF, aG)

print("Check > %s < should be > %s <" % (tStr, rStr) )
if  tStr == rStr:
  print("OK")
else:
  print("FAIL")
  sys.exit(-1)
