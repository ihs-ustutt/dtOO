#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <interfaceHeaven/vectorHandling.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>

using namespace dtOO;

int main() {
  
    vectorHandling< constValue * > cV1;			
    vectorHandling< analyticFunction * > aF1;
    std::vector<std::string > label;
    dtXmlParser *parser1;
        
  for(int kk=0 ; kk<100; kk++){
    parser1 = new dtXmlParser();
    std::cout<<"Durchlauf "<< kk << std::endl;
    
    parser1->openFileAndParse("surface.xml");
    
    parser1->getNames("constValue", &label);
    for (int ii = 0;ii<label.size();ii++) {
        parser1->createConstValue(label[ii], &cV1);
	}
    label.clear();
        
    parser1->getNames("Point_2", &label);
    std::cout<<"label.size() "<< label.size() << std::endl;
    for (int ii = 0;ii<label.size();ii++) {
	QDomElement p2El = parser1->getElement("Point_2", label[ii]);
	std::vector< dtPoint2 * > p2Vec;
	parser1->createBasic(&p2El, &cV1, &aF1, &p2Vec);
	std::vector< dtPoint2 > p2Twin;
	dt__FORALL( p2Vec, jj, p2Twin.push_back(*(p2Vec[jj]) ); );

		aF1.push_back( 
		  new vec2dCurve2dOneD( 
				ptrHandling<dtCurve2d>( 
					bSplineCurve2d_pointConstructOCC(p2Twin, 2).result() 
				).get()
		  ) 
		);		
  	aF1.back()->setLabel("f_pointsFile");	
	}
    std::cout<<aF1.get("f_pointsFile")<<std::endl;
    
    label.clear();
    aF1.destroy();
    cV1.destroy();
    delete parser1;
    }
  
    
  return 0;


    
}
