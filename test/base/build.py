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

#
# tolerance
#
tolXYZ = 1.0E-08

#
# Get point on s3_line
#
rX = +5.0
rY = +7.0
rZ = -3.0
tX = float( parser.replaceDependencies("@s3_line[%x](0.5)@", bC, cV, aF, aG) )
tY = float( parser.replaceDependencies("@s3_line[%y](0.5)@", bC, cV, aF, aG) )
tZ = float( parser.replaceDependencies("@s3_line[%z](0.5)@", bC, cV, aF, aG) )
dXYZ = ( (rX-tX)**2.0 + (rY-tY)**2.0 + (rZ-tZ)**2.0 )**0.5
print("Check: %e < %e" % (dXYZ, tolXYZ) )
if  dXYZ < tolXYZ:
  print("OK")
else:
  print("FAIL")
  sys.exit(-1)

#
# Get point on $aF_muparser
#
rX = +54.00
tXStr = parser.replaceDependencies("$aF_muparser(1.)$", bC, cV, aF, aG)
tX = float( parser.replaceDependencies("$aF_muparser(1.)$", bC, cV, aF, aG) )
print("%s -> %e" % (tXStr, tX) )
dXYZ = ( (rX-tX)**2.0 )**0.5
print("Check: %e < %e" % (dXYZ, tolXYZ) )
if  dXYZ < tolXYZ:
  print("OK")
else:
  print("FAIL")
  sys.exit(-1)
