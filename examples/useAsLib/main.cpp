#include <stdio.h>

#include <logMe/logMe.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <interfaceHeaven/vectorHandling.h>
#include <analyticFunctionHeaven/scaFunction.h>
#include <constValueHeaven/constValue.h>


using namespace dtOO;

int main( void ) {
	baseContainer bC;
//	vectorContainer vC;
	aGPtrVec aG; 
	cVPtrVec cV; 
	aFPtrVec aF; 
	std::vector< std::string > label;    
	dtXmlParser parser;

	FILELog::ReportingLevel() = logINFO;
  Output2FILE::Stream().open( "machine.log", std::ofstream::out | std::ofstream::trunc );					
	
	parser.openFileAndParse( "machine.xml" );
	// constValues
	parser.getNames("constValue", &label); 
	for (int ii = 0;ii<label.size();ii++) {
		parser.createConstValue(label[ii], &cV);
	}
	label.clear();    
	// scaFunctions
	parser.getNames("function", &label);
	for (int ii = 0;ii<label.size();ii++) {
		parser.createAnalyticFunction(label[ii], &bC, &cV, &aF);
	}
	label.clear();
  
	// analyticGeometries
	parser.getNames("part", &label);
	for (int ii = 0;ii<label.size();ii++) {
		parser.createAnalyticGeometry(label[ii], &bC, &cV, &aF, &aG);
	}
	
  for (int ii=0; ii<aG.size(); ii++) {
		aG[ii]->dump();
	}
  cV.destroy();
	aF.destroy();
	aG.destroy();
	
	return 0;
}
