#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>
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
#include <geometryEngine/geoBuilder/bSplineCurve2d_curveConnectConstructOCC.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <interfaceHeaven/ptrHandling.h>

#include "functionHeaven/scaFunction.h"


using namespace dtOO;

double ffMeanline(const std::vector<double> &_ind) {	//_ind = variables in one genotypes (vgl. GenerationalGenericAlgorithm.cpp (General)
    pointContainer pC;
    vectorContainer vC;
    vectorHandling< constValue * > cV1, cV2;			
    vectorHandling< analyticFunction * > aF1, aF2;
   	vectorHandling< dtCurve2d const * > cc;
    std::vector<std::string > label;
    dtXmlParser parser1, parser2;
    float maxcV, mincV;
    float maxTh, minTh;
    float maxML;
    float maxSurf;
    std::vector<dtPoint2>CtrlPnts2;					//CtrlPnts2 = 3 Punkte der 3PMeanline
    dtTransformer * thick1;
    dtTransformer * thick2;
    float res = 0.;
    float res1 = 0.;
    float res2 = 0.;//Rücksetzung des Rückgabewertes
    int n = 100;								//Anzahl an Auswertungsstellen
    
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
      float mincV = cV1[ii]->getMin();
      float maxcV = cV1[ii]->getMax();
      cV1[ii]->setValue(mincV + ((maxcV-mincV)* (static_cast<float> (_ind[ii]))));
      }
    
    parser1.getNames("Point_2", &label);
    for (int ll = 0;ll<label.size();ll++) {
	QDomElement p2El = parser1.getElement("Point_2", label[ll]);
	std::vector< dtPoint2 * > p2Vec1, p2Vec2;
	parser1.createBasic(&p2El, &cV1, &aF1, &p2Vec1);
	p2Vec2 = getBaseline().apply(&p2Vec1);
	std::vector< dtPoint2 > p2Twin1, p2Twin2;

/*originale Meanline*/
	
	dt__FORALL( p2Vec2, jj, p2Twin1.push_back(*(p2Vec2[jj]) ); );
			
	//
	// create scaCurve2dOneD
	//
	aF1.push_back( 
	  new vec2dCurve2dOneD( 
	    ptrHandling<dtCurve2d>( 
	      bSplineCurve2d_pointConstructOCC(p2Twin1, 2).result() 
	    ).get()
	  ) 
	);		
  	aF1.back()->setLabel("f_pointsFileML");	
		
	maxML = p2Vec2.back()->x();
	
/*originale Blattform*/
	
	dt__FORALL( p2Vec1, jj, p2Twin2.push_back(*(p2Vec1[jj]) ); );
			
	//
	// create scaCurve2dOneD
	//
	aF1.push_back( 
	  new vec2dCurve2dOneD( 
	    ptrHandling<dtCurve2d>( 
	      bSplineCurve2d_pointConstructOCC(p2Twin2, 2).result() 
	    ).get()
	  ) 
	);		
  	aF1.back()->setLabel("f_pointsFileShape");	
		
	maxSurf = p2Vec1.back()->x();
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
		qdomEl1.buildPart( builder1, &cV1, NULL, &aF1);
		aF1.back()->setLabel("f_3pMeanLine");
	}
	else {
		std::cerr<< "ERROR: " << label[ii] << "is no supportet function!" << std::endl; //Verbesserungswürdig, da das Problem der builder und nicht die function ist!
	}
    }
    label.clear();

/*/////////////////////////////////////////////////
sF beinhaltet jetzt auf:
[0] ->	Skelettlinie aus den transformierten Originalpunkten	label:	f_pointsFileML
[1] ->	Gesamtspline aus den Originalpunkten			label:	f_pointsFileShape
[2] ->	Skelettlinie auf Parameterbasis				label:	f_3pMeanLine
*//////////////////////////////////////////////////  
    
    vec2dOneD * orig = vec2dOneD::DownCast(aF1.get("f_pointsFileML"));
    vec2dOneD * interp = vec2dOneD::DownCast(aF1.get("f_3pMeanLine"));
    
    vec2dCurve2dOneD * origCurve = vec2dCurve2dOneD::DownCast(aF1.get("f_pointsFileML"));
    vec2dCurve2dOneD * interpCurve = vec2dCurve2dOneD::DownCast(aF1.get("f_3pMeanLine"));
    
    dtCurve2d const * ptrOrig =	origCurve->ptrDtCurve2d () ;
    dtCurve2d const * ptrInterp = interpCurve->ptrDtCurve2d () ;
    
    /*
		 * atismer: 
		 * hier bin ich mir nicht sicher, ob die auswertung noch genau das gleich macht wie vorher
		 * 
		 */
    
    

    for(int kk=1 ; kk<=n ; kk++){										//Residuum berechnet akutell nur die Güte der Meanline
//      res = res + (1+((kk/n)-1)*(kk/n))*fabs((orig->Y(kk*max/n))-(interp->Y(kk*max/n)));
			dtVector2 dist1 = orig->YdtPoint2(ptrOrig->u_lPercent(100*kk/n))-interp->YdtPoint2(ptrInterp->u_lPercent(100*kk/n));
		  res1 
			= 
			res1 
			+ 
			kk * ( sqrt(dist1.squared_length()) );
    }


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
    
      minTh = cV2[1]->getMin();
      maxTh = cV2[1]->getMax();
      cV2[1]->setValue(mincV + ((maxcV-mincV)* (static_cast<float> (_ind[3]))));
      cV2[2]->setValue(maxSurf);
      
    
    parser2.getNames("function", &label);
    for (int ii = 0;ii<label.size();ii++) {
        parser2.createAnalyticFunction(label[ii], &cV2, &aF2);
    }
    label.clear();

    /////////////////////////////////////////////
    // Aufdickung druckseitig//
    /////////////////////////////////////////////
    

	QDomElement qdtransfPr = parser2.getElement("transformer", "thicknessIncreasingPr");
	
	thick1 = new thicknessIncreasing();
	thick1->init(&qdtransfPr, &cV2 , &aF2);
			
	analyticFunction * aF3 = thick1->apply(aF1[2]);
  vec2dCurve2dOneD * press = vec2dCurve2dOneD::DownCast(aF3);
	
  cc.push_back(press->ptrDtCurve2d());	
        
    /////////////////////////////////////////////
    // Aufdickung saugseitig//
    /////////////////////////////////////////////

	
  
	QDomElement qdtransfSuct = parser2.getElement("transformer", "thicknessIncreasingSuct");
	
	thick2 = new thicknessIncreasing();
	thick2->init(&qdtransfSuct, &cV2 , &aF2);
	
	//cc[1]= thick2->apply(aF1[1]);
	analyticFunction * aF4 = thick2->apply(aF1[2]);
  
  vec2dCurve2dOneD * suct = vec2dCurve2dOneD::DownCast(aF4);
	
  cc.push_back(suct->ptrDtCurve2d());
  ptrHandling<dtCurve2d> dtC2d(
  bSplineCurve2d_curveConnectConstructOCC(cc).result()
  );
  
  vec2dCurve2dOneD * optShape = new vec2dCurve2dOneD(dtC2d.get());  //ermitteltes Profil ist in optBlade gespeichert
  
  //jetzt optBlade mit f_pointsFileShape vergleichen
  
  
  vec2dCurve2dOneD * origShape = vec2dCurve2dOneD::DownCast(aF1.get("f_pointsFileShape"));
      
    dtCurve2d const * ptrOrigShape = origShape->ptrDtCurve2d () ;
    dtCurve2d const * ptrInterpShape = optShape->ptrDtCurve2d () ;
  
  //float const & xx = 0.5;
  
  //dtPoint2 P2_1 = origShape->YdtPoint2Percent(xx);	
  
/*    for(int kk=1 ; kk<=n ; kk++){										//Residuum berechnet akutell nur die Güte der Meanline
    for(int kk=1 ; kk<=n ; kk++){										//Residuum berechnet akutell nur die Güte der Meanline
			dtVector2 dist = origShape->YdtPoint2Percent(kk)-optBlade->YdtPoint2Percent(kk);
		  res2 
			= 
			res2
			+ 
			(1+((kk/n)-1)*(kk/n))*fabs( sqrt(dist.squared_length()) );
    }
    
    
//      res = res + (1+((kk/n)-1)*(kk/n))*fabs((orig->Y(kk*max/n))-(interp->Y(kk*max/n)));
			dtVector2 dist = orig->YdtPoint2(kk*maxML/n)-interp->YdtPoint2(kk*maxML/n);
		  res 
			= 
			res 
			+ 
			(1+((kk/n)-1)*(kk/n))*fabs( sqrt(dist.squared_length()) );
    }
*/    
    for(int kk=1 ; kk<=n ; kk++){										//Residuum berechnet akutell nur die Güte der Meanline
//      res = res + (1+((kk/n)-1)*(kk/n))*fabs((orig->Y(kk*max/n))-(interp->Y(kk*max/n)));
			dtVector2 dist2 = origShape->YdtPoint2(ptrOrigShape->u_lPercent(kk/n))-optShape->YdtPoint2(ptrInterpShape->u_lPercent(kk/n));
		  res2 
			= 
			res2 
			+ 
			(1+((kk/n)-1)*(kk/n))*fabs( sqrt(dist2.squared_length()) );
    }
    res=res1*res2;
    std::cout << "Residuum1: " << res1 << std::endl;
//    std::cout << "Residuum2: " << res2 << std::endl;
//    std::cout << "Residuum: " << res << std::endl;

    //Destruktoren
  
    delete thick1;
    delete thick2;
    delete aF3;
    delete aF4;
    
    p2Vec1.destroy();
    p2Vec2.destroy();
    cc.destroy();
    cV1.destroy();
    cV2.destroy();
    aF1.destroy();
    aF2.destroy();

    return res1;


    
}
