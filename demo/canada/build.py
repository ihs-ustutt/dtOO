from dtOOPythonSWIG import *

logMe.initLog("build.log")
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

from PyFoam.Applications.Decomposer import Decomposer
from PyFoam.Applications.WriteDictionary import WriteDictionary
from PyFoam.Applications.Runner import Runner
from PyFoam.Applications.ClearCase import ClearCase
from PyFoam.Applications.PackCase import PackCase

caseName = "ingvrudtout_coupled_of"
stateName = "E1_12685"
Decomposer(args=[caseName+"_"+stateName,"4","--method=scotch","--clear","--silent"])

WriteDictionary(args=[caseName+"_"+stateName+"/system/controlDict", "endTime", "100"])
WriteDictionary(args=[caseName+"_"+stateName+"/system/controlDict", "writeInterval", "100"])
WriteDictionary(args=[caseName+"_"+stateName+"/constant/turbulenceProperties", "RAS['turbulence']", "off"])
#!sed -e "s/cellL/faceL/g" ${caseName}_${stateName}/system/fvSchemes
Runner(args=["--silent", "--autosense-parallel", "simpleFoam", "-case", caseName+"_"+stateName])

#!sed -e "s/faceL/cellL/g" ${caseName}_${stateName}/system/fvSchemes
WriteDictionary(args=[caseName+"_"+stateName+"/system/controlDict", "endTime", "1000"])
WriteDictionary(args=[caseName+"_"+stateName+"/system/controlDict", "writeInterval", "1000"])
WriteDictionary(args=[caseName+"_"+stateName+"/constant/turbulenceProperties", "RAS['turbulence']", "on"])
Runner(args=["--silent", "--autosense-parallel", "simpleFoam", "-case", caseName+"_"+stateName]) #, "--remove-processor-dirs"])

Runner(args=['--silent', 'reconstructPar', '-latestTime', "-case", caseName+"_"+stateName])

ClearCase(['--keep-postprocessing', '--processors-remove', '--remove-analyzed', '--keep-postprocessing', '--clear-history', '--keep-last', caseName+"_"+stateName])

PackCase([caseName+"_"+stateName, '--last'])
