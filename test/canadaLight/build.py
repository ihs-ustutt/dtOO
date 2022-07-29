from dtOOPythonSWIG import *

logMe.initLog('build.log')
dtXmlParser.init("machine.xml", "E1_12685.xml")
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
parser.loadStateToConst("E1_12685", cV)
parser.destroyAndCreate(bC, cV, aF, aG, bV, dC, dP)

dC.get("ingvrudtout_coupled_of").runCurrentState()