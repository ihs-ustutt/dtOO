#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <interfaceHeaven/vectorHandling.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
//#include <dtXmlParserDecorator/sca3PointMeanlineFunctionDecorator.h>
#include <sca3PointMeanlineFunctionDecoratorXY.h>
#include <logMe/logMe.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <getBaseline.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <dtTransformerHeaven/thicknessIncreasing.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <interfaceHeaven/ptrHandling.h>

#include "functionHeaven/scaFunction.h"


using namespace dtOO;

double ffMeanline(const std::vector<double> &_ind) {	//_ind = variables in one genotypes (vgl. GenerationalGenericAlgorithm.cpp (General)
    pointContainer pC;
    vectorContainer vC;
    vectorHandling< constValue * > cV1, cV2;			
    vectorHandling< analyticFunction * > sF1, sF2, sF3, sF4;
    std::vector<std::string > label;
    dtXmlParser parser1, parser2;
    float max;
    float min;
    std::vector<dtPoint2>CtrlPnts2;					//CtrlPnts2 = 3 Punkte der 3PMeanline
    dtTransformer * thick1;
    dtTransformer * thick2;
    float res = 0;							//Rücksetzung des Rückgabewertes
    int n = 10;								//Anzahl an Auswertungsstellen

/////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//!! Meanline !!//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
/////////////////////////////////////////////

    
    
    parser1.openFileAndParse("surface.xml");		//surface.xml mit den cV und functions für die Ermittlung der Meanline
    
    /////////////////////////////////////////////
    // constValues erzeugen //
    /////////////////////////////////////////////
    
    
    parser1.getNames("constValue", &label);
    for (int ii = 0;ii<label.size();ii++) {
        parser1.createConstValue(label[ii], &cV1);
	}
	
    label.clear();
    
    for (int ii = 0; ii < 3 ; ii++){						//_ind immer nur zwischen 0 und 1, daher multiplikation mit den Grenzen
      float min = cV1[ii]->getMin();
      float max = cV1[ii]->getMax();
      cV1[ii]->setValue(min + ((max-min)* (static_cast<float> (_ind[ii]))));
      }
    
    parser1.getNames("Point_2", &label);
    for (int ii = 0;ii<label.size();ii++) {
	QDomElement p2El = parser1.getElement("Point_2", label[ii]);
	std::vector< dtPoint2 * > p2Vec;
	parser1.createBasic(&p2El, &cV1, &sF1, &p2Vec);
	p2Vec = getBaseline().apply(&p2Vec);
	std::vector< dtPoint2 > p2Twin;
	dt__FORALL( p2Vec, jj, p2Twin.push_back(*(p2Vec[jj]) ); );
			
//	sF1.push_back( new scaSISLSplineTwoD(&p2Twin, 3) );
//	sF1.back()->setLabel("f_pointsFile");


		//
		// create scaCurve2dOneD
		//
		sF1.push_back( 
		  new vec2dCurve2dOneD( 
				ptrHandling<dtCurve2d>( 
					bSplineCurve2d_pointConstructOCC(p2Twin, 2).result() 
				).get()
		  ) 
		);		
  	sF1.back()->setLabel("f_pointsFile");	
		
	max = p2Vec.back()->x();
	min = p2Vec.front()->x();
	}
    
    label.clear();

    /////////////////////////////////////////////
    // scaFunctions//
    /////////////////////////////////////////////
		
     parser1.getNames("function", &label);
     for (int ii = 0;ii<label.size();ii++) {
     
	if(label[ii]=="f_3pMeanLine") {
		QDomElement * functionElementP = new QDomElement(parser1.getElement("function", "f_3pMeanLine") );
		QDomElement builder1 = parser1.getChild("builder", *functionElementP);
//		QDomElement *builderptr1 = &builder1;
		sca3PointMeanlineFunctionDecoratorXY qdomEl1;
		qdomEl1.buildPart( builder1, &cV1, NULL, &sF1);
		sF1.back()->setLabel("f_3pMeanLine");
	}
	else {
		std::cerr<< "ERROR: " << label[ii] << "is no supportet function!" << std::endl; //Verbesserungswürdig, da das Problem der builder und nicht die function ist!
	}
    }
    label.clear();
    
    
//    CtrlPnts2 = (dynamic_cast<vec2dCurve2dOneD*>(sF1.get("f_3pMeanLine")))->ptrDtCurve2d()->controlPoint();
//    
//    for(int aa = 0 ; aa<CtrlPnts2.size() ; aa++){
//    //int aa = 0;
//    std::cout<< "2ControllPoints " << aa << ":\t" << CtrlPnts2[aa] << std::endl;
//    }	
    
    
    vec2dOneD * orig = vec2dOneD::DownCast(sF1.get("f_pointsFile"));
    vec2dOneD * interp = vec2dOneD::DownCast(sF1.get("f_3pMeanLine"));
    
    /*
		 * atismer: 
		 * hier bin ich mir nicht sicher, ob die auswertung noch genau das gleich macht wie vorher
		 * 
		 */
    for(int kk=1 ; kk<=n ; kk++){										//Residuum berechnet akutell nur die Güte der Meanline
//      res = res + (1+((kk/n)-1)*(kk/n))*fabs((orig->Y(kk*max/n))-(interp->Y(kk*max/n)));
			dtVector2 dist = orig->YdtPoint2(kk*max/n)-interp->YdtPoint2(kk*max/n);
		  res 
			= 
			res 
			+ 
			(1+((kk/n)-1)*(kk/n))*fabs( sqrt(dist.squared_length()) );
    }
    
    std::cout << "Residuum: " << res << std::endl;

    

/////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//!! Aufdickung !!//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
/////////////////////////////////////////////
    

    
    
    parser2.openFileAndParse("thickness.xml");
  
    /////////////////////////////////////////////
    // constValues erzeugen //
    /////////////////////////////////////////////
  
  
    parser2.getNames("constValue", &label);
    for (int ii = 0;ii<label.size();ii++) {
        parser2.createConstValue(label[ii], &cV2);
    }
    label.clear();
    
    parser2.getNames("function", &label);
    for (int ii = 0;ii<label.size();ii++) {
        parser2.createAnalyticFunction(label[ii], &cV2, &sF2);
    }
    label.clear();

    /////////////////////////////////////////////
    // Aufdickung druckseitig//
    /////////////////////////////////////////////
    

	QDomElement qdtransfPr = parser2.getElement("transformer", "thicknessIncreasingPr");
	
	thick1 = new thicknessIncreasing();
	thick1->init(&qdtransfPr, &cV2 , &sF2);
	
	thick1->apply(sF1[1]);
	
	//sF3 = *thick1;
	
        
    /////////////////////////////////////////////
    // Aufdickung saugseitig//
    /////////////////////////////////////////////

	
  
	QDomElement qdtransfSuct = parser2.getElement("transformer", "thicknessIncreasingSuct");
	
	thick2 = new thicknessIncreasing();
	thick2->init(&qdtransfSuct, &cV2 , &sF2);
	
	thick2->apply(sF1[1]);
	
	//sF4 = *thick2;
   
	delete thick1, thick2;
	
 
    cV1.destroy();
    cV2.destroy();
    sF1.destroy();
    sF2.destroy();
    sF3.destroy();
    sF4.destroy();
    return res;


    
}