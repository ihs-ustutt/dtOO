#include <stdio.h>

#include <dtXmlParserDecorator/dtXmlParser.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <interfaceHeaven/vectorHandling.h>
#include <functionHeaven/scaFunction.h>
#include <constValueHeaven/constValue.h>


using namespace dtOO;

int main( void ) {
    pointContainer pC;
    vectorContainer vC;
    vectorHandling< analyticGeometry * > aG; 
//    vectorHandling< analyticGeometry * > aGIn; 
    vectorHandling< constValue * > cV; 
    vectorHandling< scaFunction * > sF; 
    std::vector< std::string > label;    
    dtXmlParser parser;

    parser.openFileAndParse( "test.xml" );    

    // constValues
    parser.getNames("constValue", &label); 
    for (int ii = 0;ii<label.size();ii++) {
      parser.createConstValue(label[ii], &cV);
    }
    label.clear();    

    // scaFunctions
    parser.getNames("scaFunction", &label);
    for (int ii = 0;ii<label.size();ii++) {
      parser.createFunction(label[ii], &cV, &sF);
    }
    label.clear();
    
    // analyticGeometries
    parser.getNames("part", &label);
    for (int ii = 0;ii<label.size();ii++) {
      parser.createMachinePart(label[ii], &pC, &vC, &cV, &sF, &aG);
    }
    for (int ii=0; ii<aG.size(); ii++) {
      aG[ii]->dump();
    }

    return 0;

}

