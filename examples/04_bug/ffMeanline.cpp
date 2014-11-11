#include <analyticGeometryHeaven/analyticGeometry.h>
#include <interfaceHeaven/vectorHandling.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <logMe/logMe.h>
#include <stdio.h>
#include <iostream>
#include <interfaceHeaven/ptrHandling.h>


using namespace dtOO;

int main() {
  vectorHandling< constValue * > cV1;			
	vectorHandling< analyticFunction * > aF1;
	std::vector< std::string > label;
	dtXmlParser parser1;
        
  for(int kk=0 ; kk<100; kk++){
//    parser1 = new dtXmlParser();
    std::cout<<"Durchlauf "<< kk << std::endl;
    
    parser1.openFileAndParse("surface.xml");
    parser1.getNames("constValue", &label);
		for (int ii = 0;ii<label.size();ii++) {
      parser1.createConstValue(label[ii], &cV1);
	  }
    label.clear();
    
    std::cout << cV1.get("cV_alpha_two") << std::endl;
    
    parser1.getNames("Point_2", &label);
    label.clear();
		std::cout << "label.size() = " << label.size() << std::endl;
//    delete parser1;
    cV1.destroy();		
  }
  return 0;    
}
