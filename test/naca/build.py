import sys
sys.path.append("/home/ati/sandbox/leap~15.2~x86_64/tools")

from libdtOOPython import *

#from mpi4py import MPI

#comm = MPI.COMM_WORLD
#rank = comm.Get_rank()
#print('My rank is ',rank)

logMe.initLog('build.log')
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
#parser.loadStateToConst("K1_1", cV) 
parser.destroyAndCreate(bC, cV, aF, aG, bV, dC, dP)
bV.get("gridGmsh").makeGrid()
#bV.get("ingvrudtout_blkGridMesh").makeGrid()
#dC.get("ingvrudtout_coupled_of").runCurrentState()
