#include <interfaceHeaven/vectorHandling.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>

using namespace dtOO;

int main() {
  
    cVPtrVec cV1;			
    aFPtrVec aF1;
    std::vector<std::string > label;
    dtXmlParser *parser1;
        
  for(int kk=0 ; kk<1000; kk++){
    parser1 = new dtXmlParser();
    std::cout<<"Durchlauf "<< kk << std::endl;
    
    parser1->openFileAndParse("surface.xml");
    
		parser1->createConstValue(&cV1);
//		label.clear();
//    parser1->getNames("constValue", &label);
//    for (int ii = 0;ii<label.size();ii++) {
//			std::cout << "label[" << ii << "] = " << label[ii] << std::endl;
//        parser1->createConstValue(label[ii], &cV1);
//		}
//    label.clear();
        
    parser1->getNames("Point_2", &label);
    std::cout<<"label.size() "<< label.size() << std::endl;
    for (int ii = 0;ii<label.size();ii++) {
			aF1.push_back( new vec2dCurve2dOneD() );		
			aF1.back()->setLabel("f_pointsFile");
			std::cout << "aF1.size() = " << aF1.size() << std::endl;
		}
    std::cout<<aF1.get("f_pointsFile")<<std::endl;
    
    label.clear();
    aF1.destroy();
//    cV1.destroy();
    delete parser1;
  }
  
    
  return 0;


    
}
