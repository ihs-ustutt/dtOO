#include "dtXmlParserBase.h"
#include "interfaceHeaven/ptrHandling.h"
#include "baseContainerHeaven/transformerContainer.h"
#include <functionHeaven/scaOneD.h>
#include <functionHeaven/vec2dOneD.h>
#include <functionHeaven/vec3dOneD.h>
#include <functionHeaven/vec3dTwoD.h>

#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <progHelper.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

#include <constValueHeaven/constValue.h>
#include <functionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <dtTransformerHeaven/doNothing.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/rotatingSpline.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/oneDimTrimmable.h>
#include <analyticGeometryHeaven/oneDimForwardExtendable.h>
#include <analyticGeometryHeaven/oneDimBackwardExtendable.h>
#include <analyticGeometryHeaven/oneDimRevertable.h>
#include <analyticGeometryHeaven/twoDimRevertable.h>
#include <analyticGeometryHeaven/oneDimCloseable.h>

namespace dtOO {
  dtXmlParserBase::dtXmlParserBase() {
  }

  dtXmlParserBase::~dtXmlParserBase() {
  }

  dtTransformer * dtXmlParserBase::createTransformer(
	  QDomElement const * toBuildP,
    baseContainer * const bC,                    
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP, 
		vectorHandling< analyticGeometry * > const * const depAGeoP
	) const {
		dtTransformer * dtTransformerP;

		//
		// get transformer element
		//
		QDomElement wElement;
		if ( hasSibling("transformer", *toBuildP) ) {
			wElement = getSibling("transformer", *toBuildP);				
		}
		else if ( is("transformer", *toBuildP) ) {
			wElement = *toBuildP;
		}

		//
		// check if there is a transformer
		//
		if ( !wElement.isNull() ) {	
			bool hasLabel = hasAttribute("label", wElement);
			bool hasName = hasAttribute("name", wElement);
			bool inContainer = false;
			std::string label;
			std::string name;
			if (hasLabel) {
				label = getAttributeStr("label", wElement);
				inContainer = bC->ptrTransformerContainer()->has(label);
			}
			if (hasName) {
				name = getAttributeStr("name", wElement);
			}			
			
			if ( !hasLabel ) {
				dtTransformerP 
				= 
				dtTransformerFactory::create( name );
				dtTransformerP->init(&wElement, bC, cValP, sFunP, depAGeoP);			
			}
			else if (hasLabel && inContainer && !hasName) {
				return bC->ptrTransformerContainer()->get(label)->clone();
			}
			else if (hasLabel && !inContainer && hasName) {
				dtTransformerP 
				= 
				dtTransformerFactory::create( name );
				dtTransformerP->init(&wElement, bC, cValP, sFunP, depAGeoP);			
        bC->ptrTransformerContainer()->add(dtTransformerP);
				delete dtTransformerP;
				dtTransformerP = dtTransformerFactory::create("doNothing");
			}
			else if (hasLabel && inContainer && hasName) {
				dtTransformerP = dtTransformerFactory::create("doNothing");
			}			
			else {
				dt__THROW(
					createTransformer(),
					<< DTLOGEVAL(hasLabel) << LOGDEL
					<< DTLOGEVAL(hasName) << LOGDEL
					<< DTLOGEVAL(inContainer) << LOGDEL
					<< DTLOGEVAL(label) << LOGDEL
					<< DTLOGEVAL(name) << LOGDEL
					<< "Problem creating transformer."
				);
			}
		}
		else {
			dtTransformerP = dtTransformerFactory::create("doNothing");
		}
		return dtTransformerP;
  }

  dtTransformer * dtXmlParserBase::createTransformer(
	  QDomElement const * toBuildP,                   
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP
	) const {

      dtTransformerFactory dtTransFac;
      dtTransformer * dtTransformerP;
        //
      // check if there is a transformer
      //
      if ( hasSibling("transformer", *toBuildP) ) {
        //get transformer element
        QDomElement wElement = getSibling("transformer", *toBuildP);
        //
        //create and initialize transformer
        //
        dtTransformerP = dtTransFac.create( wElement.attribute("name").toStdString() );
        dtTransformerP->init(&wElement, cValP, sFunP);
      }
      else {
        dtTransformerP = dtTransFac.create("doNothing");
      }
      return dtTransformerP;
  }

  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP,
    baseContainer * const bC,                  
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP, 
		vectorHandling< analyticGeometry * > const * const depAGeoP,
		std::vector< dtPoint3 > * basicP
	) const {
    //
    // create transformer if necessary
    //
		QDomElement wEl = *toBuildP;
    ptrHandling<dtTransformer> dtTransformerP(   
      createTransformer(toBuildP, bC, cValP, sFunP, depAGeoP)
	  );
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }    
    //
    //create points
    //
		vectorHandling< dtPoint3 * > pp;		
    createBasic(
			&wEl, bC, cValP, sFunP, depAGeoP, dtTransformerP.get(), &pp
		);		
		dt__FORALL(pp, ii,
		  basicP->push_back( *(pp[ii]) );
		);		
  }
	
  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP,
    baseContainer * const bC,		
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP, 
		vectorHandling< analyticGeometry * > const * const depAGeoP,
		dtTransformer const * const dtTransformerP, 
		std::vector< dtPoint3* > * basicP
	) const {
    if ( !is("Point_3", *toBuildP) ) {
      dt__THROW(createBasic(), << DTLOGEVAL( getTagName(*toBuildP) ) );
    }

		//
		// check if point has a label and point is already created
		// return point and that's it
		//
		if ( hasAttribute("label", *toBuildP) ) {
			//
			// return multiple points
			//        
			if ( hasAttribute("number_points", *toBuildP) ) {
				int nPoints = getAttributeInt("number_points", *toBuildP);
				for (int ii=0;ii<nPoints;ii++) {
					std::string pointName;
					pointName = getAttributeStr("label", *toBuildP);
					pointName += "_";
					pointName += intToString(ii+1);
					if ( bC->ptrPointContainer()->has( pointName ) ) {
						basicP->push_back( new dtPoint3(0,0,0) );
						*(basicP->back()) = bC->ptrPointContainer()->get( pointName );
					}
				}
				return;
			}
			else if ( hasAttribute("start_number", *toBuildP) 
								 && hasAttribute("end_number", *toBuildP) ) {
				int startPoint = getAttributeInt("start_number", *toBuildP);
				int endPoint = getAttributeInt("end_number", *toBuildP);
				int nPoints = endPoint - startPoint + 1;
				for (int ii=0;ii<nPoints;ii++) {
					std::string pointName;
					pointName = getAttributeStr("label", *toBuildP);
					pointName += "_";
					pointName += intToString(startPoint);
					if ( bC->ptrPointContainer()->has( pointName ) ) {
						basicP->push_back( new dtPoint3(0,0,0) );
						*(basicP->back()) = bC->ptrPointContainer()->get( pointName );
					}
					startPoint++;
				}
				return;
			}        
			//
			//return a single point
			//
			else {
				if ( bC->ptrPointContainer()->has(toBuildP->attribute("label").toStdString()) ) {
					basicP->push_back( new dtPoint3(0,0,0) );
					*(basicP->back()) = bC->ptrPointContainer()->get( toBuildP->attribute("label").toStdString() );
				return;
				}
			}
		}

		if (    hasAttribute("x", *toBuildP) 
				 && hasAttribute("y", *toBuildP) 
				 && hasAttribute("z", *toBuildP) ) { 
			float cX = muParseString(
									 replaceUsedFunctions(
										 getAttributeStr("x", *toBuildP), cValP,  sFunP
									 )
								 );
			float cY = muParseString(
									 replaceUsedFunctions(
										 getAttributeStr("y", *toBuildP), cValP, sFunP
									 )
								 );
			float cZ = muParseString(
									 replaceUsedFunctions(
										 getAttributeStr("z", *toBuildP), cValP, sFunP
									 )
								 );        
			basicP->push_back( new dtPoint3(cX, cY, cZ) );      
		}
		/* ---------------------------------------------------------------------- */
		/* pick point from a part */
		/* ---------------------------------------------------------------------- */          
		else if (    hasAttribute("attribute", *toBuildP) 
						  && hasAttribute("part_label", *toBuildP) ) {
			analyticGeometry const * aG 
			= 
			depAGeoP->get( getAttributeStr("part_label", *toBuildP) );
			map1dTo3d const * m1d = map1dTo3d::ConstDownCast(aG);
			map2dTo3d const * m2d = map2dTo3d::ConstDownCast(aG);
			map3dTo3d const * m3d = map3dTo3d::ConstDownCast(aG);

			if (!m1d && !m2d && !m3d) {
				dt__THROW(
				  createBasic(),
					<< DTLOGEVAL( getAttributeStr("part_label", *toBuildP) ) << LOGDEL
					<< "No such part or part is not of correct type");     
			}
			if ( getAttributeStr("attribute", *toBuildP) == "pick_from_part" ) {
        // 
        // 1 map2dTo3d
        // 				
				if (m2d) {
					rotatingSpline const * rS = rotatingSpline::ConstDownCast(m2d);
					//
					//rotatingSpline
					//
					if (rS && hasAttribute("phi", *toBuildP) && hasAttribute("z", *toBuildP) ) {
						float cX = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("phi", *toBuildP), cValP,  sFunP
												 )
											 );
						float cY = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("z", *toBuildP), cValP, sFunP
												 )
											 );                      
						basicP->push_back( new dtPoint3( rS->xyz_phiZ(cX, cY) ) );
					}
					else if (    hasAttribute("parameter_one_percent", *toBuildP)
										&& hasAttribute("parameter_two_percent", *toBuildP) ) {
						float cX = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_one_percent", *toBuildP),
													 cValP, 
													 sFunP
												 )
											 );
						float cY = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_two_percent", *toBuildP),
													 cValP, 
													 sFunP
												 )
											 );              
						basicP->push_back( new dtPoint3( m2d->getPointPercent(cX, cY) ) );
					}
				}
		// 
		// 2 map1dTo3d
		//         
				else if (m1d && hasAttribute("parameter_one_percent", *toBuildP) ) {
					float cX = muParseString(
											 replaceUsedFunctions(
												 getAttributeStr("parameter_one_percent", *toBuildP),
												 cValP, 
												 sFunP
											 )
										 );
					basicP->push_back( new dtPoint3( m1d->getPointPercent(cX) ) );
				}
		// 
		// 3 map3dTo3d
		//         
				else if ( m3d ) {
					if (    hasAttribute("parameter_one_percent", *toBuildP) 
							 && hasAttribute("parameter_two_percent", *toBuildP) 
							 && hasAttribute("parameter_three_percent", *toBuildP) ) {
						float cX = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_one_percent", *toBuildP),
													 cValP, 
													 sFunP
												 )
											 );
						float cY = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_two_percent", *toBuildP),
													 cValP, 
													 sFunP
												 )
											 );
						float cZ = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_three_percent", *toBuildP),
													 cValP, 
													 sFunP
												 )
											 );                
						basicP->push_back( new dtPoint3( m3d->getPointPercent(cX, cY, cZ) ) );
					}        
					else if (    hasAttribute("parameter_one", *toBuildP) 
							 && hasAttribute("parameter_two", *toBuildP) 
							 && hasAttribute("parameter_three", *toBuildP) ) {
						float cX = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_one", *toBuildP),
													 cValP, 
													 sFunP
												 )
											 );
						float cY = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_two", *toBuildP),
													 cValP, 
													 sFunP
												 )
											 );
						float cZ = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_three", *toBuildP),
													 cValP, 
													 sFunP
												 )
											 );                
						basicP->push_back( new dtPoint3( m3d->getPoint(cX, cY, cZ) ) );
					} 					
				}      
			}
	//
	// attribute: pick_order_from_part
	//      
			else if ( getAttributeStr("attribute", *toBuildP) == "pick_order_from_part" ) {
				//
				// 1 map2dTo3d
				//        
				if (   m2d
						&& hasAttribute("parameter_one_percent_function", *toBuildP)
						&& hasAttribute("parameter_two_percent_function", *toBuildP)
						&& hasAttribute("number_points_one", *toBuildP)
						&& hasAttribute("number_points_two", *toBuildP) ) {
					int nPointsOne = getAttributeInt("number_points_one", *toBuildP);
					int nPointsTwo = getAttributeInt("number_points_two", *toBuildP);
					//
					// search functions
					//
					dt__PTRASS(
						scaOneD const * functionOneP, 
						scaOneD::ConstDownCast(
							sFunP->get( 
								getAttributeStr("parameter_one_percent_function", *toBuildP)
							)
						)
					);
					dt__PTRASS(
						scaOneD const * functionTwoP,
						scaOneD::ConstDownCast(
							sFunP->get( 
								getAttributeStr("parameter_two_percent_function", *toBuildP)
							)
						)
					);
					for (int ii=0;ii<nPointsOne;ii++) {
						for (int jj=0;jj<nPointsTwo;jj++) {
							float paraOne = functionOneP->YFloat( ((float) ii) / nPointsOne ) ;
							float paraTwo = functionTwoP->YFloat( ((float) jj) / nPointsTwo ) ;
							basicP->push_back( 
							  new dtPoint3( m2d->getPointPercent( paraOne, paraTwo ) )
							);              
						}
					}
				}
				//
				// 1 map1dTo3d
				//        
				else if (   m1d
								 && hasAttribute("parameter_one_percent_function", *toBuildP)
								 && hasAttribute("number_points_one", *toBuildP) ) {
					dt__PTRASS(
						scaOneD const * sF,
						scaOneD::ConstDownCast(
							sFunP->get( 
								getAttributeStr( "parameter_one_percent_function", *toBuildP )
							)
						)
					);
					int nPointsOne = getAttributeInt("number_points_one", *toBuildP);
					float nPointsOneF = static_cast<float>(nPointsOne);              
					for (int ii=0;ii<nPointsOne;ii++) {
						float iiF = static_cast<float>(ii);
						float uu = sF->YFloat( iiF / nPointsOneF ) ;
						basicP->push_back( 
							new dtPoint3( m1d->getPointPercent(uu) )
						);              
					}
				}                    
			}
		}
		/* ---------------------------------------------------------------------- */
		/* pick point from a function */
		/* ---------------------------------------------------------------------- */          
		else if (    hasAttribute("attribute", *toBuildP) 
						  && hasAttribute("function_label", *toBuildP) ) {
			analyticFunction const * aF
			= 
			sFunP->get( getAttributeStr("function_label", *toBuildP) );
			vec3dOneD const * v1D = vec3dOneD::ConstDownCast(aF);
			vec3dTwoD const * v2D = vec3dTwoD::ConstDownCast(aF);

			if (!v1D && !v2D) {
				dt__THROW(
				  createBasic(),
					<< DTLOGEVAL( getAttributeStr("function_label", *toBuildP) ) << LOGDEL
					<< "No such analyticFunction or analyticFunction has wrong type.");     
			}
			if ( getAttributeStr("attribute", *toBuildP) == "pick_from_function" ) {
				if ( v1D && hasAttribute("x_one", *toBuildP) ) {
					float cX = muParseString(
											 replaceUsedFunctions(
												 getAttributeStr("x_one", *toBuildP), cValP,  sFunP
											 )
										 );
					basicP->push_back( new dtPoint3( v1D->YdtPoint3(cX) ) );
				}
				else if ( v1D && hasAttribute("x_one_percent", *toBuildP) ) {
					float cX = muParseString(
											 replaceUsedFunctions(
												 getAttributeStr("x_one_percent", *toBuildP), cValP,  sFunP
											 )
										 );
					basicP->push_back( new dtPoint3( v1D->YdtPoint3Percent(cX) ) );
				}				
				else if ( v2D && hasAttribute("x_one", *toBuildP) && hasAttribute("x_two", *toBuildP) ) {
					float cX = muParseString(
											 replaceUsedFunctions(
												 getAttributeStr("x_one", *toBuildP), cValP,  sFunP
											 )
										 );
					float cY = muParseString(
											 replaceUsedFunctions(
												 getAttributeStr("x_two", *toBuildP), cValP, sFunP
											 )
										 );                      
					basicP->push_back( new dtPoint3( v2D->YdtPoint3(cX, cY) ) );
				}	
				else if ( v2D && hasAttribute("x_one_percent", *toBuildP) && hasAttribute("x_two_percent", *toBuildP) ) {
					float cX = muParseString(
											 replaceUsedFunctions(
												 getAttributeStr("x_one_percent", *toBuildP), cValP,  sFunP
											 )
										 );
					float cY = muParseString(
											 replaceUsedFunctions(
												 getAttributeStr("x_two_percent", *toBuildP), cValP, sFunP
											 )
										 );                      
					basicP->push_back( new dtPoint3( v2D->YdtPoint3Percent(cX, cY) ) );
				}				
			}			
			
		}
		
		//
		// check if a point was created
		//
		if (basicP->size() == 0 ) {
			dt__THROW(
		    createBasic(), 
				<< "No point could be created." << LOGDEL
				<< convertToString(*toBuildP) 
			);	
		}
		
		//
		//transform points
		//
		if ( dtTransformerP->isNecessary() ) {
			std::vector< dtPoint3 * > basicPTwin;
			for (int ii=0;ii<basicP->size();ii++) {
				basicPTwin.push_back( basicP->at(ii) );
			}
			basicP->clear();
			*basicP = dtTransformerP->apply( &basicPTwin );
			for (int ii=0;ii<basicPTwin.size();ii++) {
				delete basicPTwin[ii];
			}
			basicPTwin.clear();
		}
//		} 
		if ( basicP->size() > 1 ) {
			for (int ii=0;ii<basicP->size();ii++) {
				if ( toBuildP->hasAttribute("label") ) {
					std::string pointName;
					pointName = toBuildP->attribute("label").toStdString();
					pointName += "_";
					pointName += intToString(ii+1);
					bC->ptrPointContainer()->add( basicP->at(ii) , pointName );
				}
			}
		}
		else {
			if ( toBuildP->hasAttribute("label") ) {
				bC->ptrPointContainer()->add( basicP->at(0) , toBuildP->attribute("label").toStdString() );
			}
		}
		//
		//output
		//
		for (int ii=0;ii<basicP->size();ii++) {
			DTDEBUGWF(
			  createBasic(), 
				<< "created point: " << DTLOGPOI3DP(basicP->at(ii)) 
			);
		}    
  }

  dtVector3 dtXmlParserBase::createDtVector3(
	  QDomElement const * toBuildP,
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP,  
		vectorHandling< analyticFunction * > const * const sFunP, 
		vectorHandling< analyticGeometry * > const * const depAGeoP
	) const {  
		dtVector3 vv;

    if ( !is("Vector_3", *toBuildP) ) {
      dt__THROW(createBasic(),
              << DTLOGEVAL(qPrintable( toBuildP->tagName() ) ) );
    }
    else {
      //
      // check if vector has a label and point is already created
      // return point and that's it
      //
      if ( toBuildP->hasAttribute("label") ) {
        if ( bC->ptrVectorContainer()->has(toBuildP->attribute("label").toStdString()) ) {
//          *basicP = new dtVector3(0,0,0);
          vv = bC->ptrVectorContainer()->get( toBuildP->attribute("label").toStdString() );
          return vv;
        }
      }
      //
      // check for attribute x, y and z
      //
      if ( toBuildP->hasAttribute("x") && toBuildP->hasAttribute("y") && toBuildP->hasAttribute("z") ) { 
        //
        // create coordinates
        //
        float cX = muParseString(
                     replaceUsedFunctions(
                       getAttributeStr("x", *toBuildP),
                       cValP, 
                       sFunP
                     )
                   );  
        float cY = muParseString(
                     replaceUsedFunctions(
                       getAttributeStr("y", *toBuildP),
                       cValP, 
                       sFunP
                     )
                   );
        float cZ = muParseString(
                     replaceUsedFunctions(
                       getAttributeStr("z", *toBuildP),
                       cValP, 
                       sFunP
                     )
                   );              
        //
        // create vector
        //
        vv = dtVector3(cX, cY, cZ);      
      }
      //
      //check if point has a label and add it to the container
      //
      if ( toBuildP->hasAttribute("label") ) {
        bC->ptrVectorContainer()->add( vv, toBuildP->attribute("label").toStdString() );
      }
      //
      //output
      //
      DTDEBUGWF(createBasic(),
              << "created vector: (" 
              << vv.x() << " / "
              << vv.y() << " / "
              << vv.z() << ")");
			
			return vv;
    }
  }

  std::string dtXmlParserBase::createString(
	  QDomElement const * toBuildP, 
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const sFunP
	) const {

    if ( getTagName(*toBuildP) != "string" ) {
      dt__THROW(createBasic(), << DTLOGEVAL( getTagName(*toBuildP) ) );
    }
		//
		// check for attribute value
		//
		if ( hasAttribute("value", *toBuildP)  ) {
			// create point
			return replaceUsedFunctions( 
							 getAttributeStr("value", *toBuildP), cValP, sFunP
						 );
		}  
  }

  dtPoint3 dtXmlParserBase::createDtPoint3(
	  QDomElement const * toBuildP,
    baseContainer * const bC,                   
    vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP, 
		vectorHandling< analyticGeometry * > const * const depAGeoP) const {
//		createBasic(toBuildP, pointContainerP, vectorContainerP, cValP, sFunP, depAGeoP, &basicP);
    std::vector< dtPoint3 > basicVec;

    createBasic(toBuildP,
                bC,                    
                cValP,
                sFunP, 
                depAGeoP,
                &basicVec);
    if (basicVec.size() != 1) {
      dt__THROW(createBasic(),
              << "Try to create one point, but function "
              << "createBasic() returns more than one point.");
    }
		dtPoint3 ret = basicVec[0];	
		return ret;
	}
	
  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP, 
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const sFunP, 
		std::vector< dtPoint2 * > * basicP
	) const {
		QDomElement wEl = *toBuildP;
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }		
    createBasic(&wEl, NULL, cValP, sFunP, NULL, basicP);      		
  }

  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP, 
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const sFunP, 
		std::vector< dtPoint3 > * basicP
	) const {
		QDomElement wEl = *toBuildP;
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }		
    createBasic(&wEl, NULL, cValP, sFunP, NULL, basicP);      		
  }	

  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP,
    baseContainer * const bC,            
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP, 
		vectorHandling< analyticGeometry * > const * const depAGeoP,
		dtTransformer const * const dtTransformerP, 
		std::vector< dtPoint2* > * basicP
	) const {

    /* ------------------------------------------------------------------------ */
    /* error handling */
    /* ------------------------------------------------------------------------ */
    bool isdtPoint2 = is("Point_2", *toBuildP);
    if ( !isdtPoint2  ) {
      dt__THROW(createBasic(), << DTLOGEVAL( getTagName(*toBuildP) ) );
      return;
    }

    /* ---------------------------------------------------------------------- */
    /* create normal x-y-z-point */
    /* ---------------------------------------------------------------------- */
    if ( toBuildP->hasAttribute("x") && toBuildP->hasAttribute("y") ) { 
      //
      // create coordinates
      //
      float cX = muParseString(
                   replaceUsedFunctions(
                     getAttributeStr("x", *toBuildP),
                     cValP, 
                     sFunP
                   )
                 );  
      float cY = muParseString(
                   replaceUsedFunctions(
                     getAttributeStr("y", *toBuildP),
                     cValP, 
                     sFunP
                   )
                 );              
      //
      // create point
      //
      basicP->push_back( new dtPoint2(cX, cY) );
    }
    /* ---------------------------------------------------------------------- */
    /* pick point from a part */
    /* ---------------------------------------------------------------------- */          
    else if ( toBuildP->hasAttribute("attribute") && toBuildP->hasAttribute("part_label") ) {
      //
      //look for part to pick point from
      //
      analyticSurface const * analyticSurface_partToPickFromP = NULL;
      analyticGeometry const * aG = depAGeoP->get( getAttributeStr("part_label", *toBuildP) );
      analyticSurface_partToPickFromP = dynamic_cast< analyticSurface const * >( aG );
      //
      // attribute: pick_from_part
      //      
      if ( toBuildP->attribute("attribute") == "pick_from_part" ) {
  //   
  //   1 analyticSurface
  //   
        if (analyticSurface_partToPickFromP) {
          //
          // analyticSurface
          //
          if (analyticSurface_partToPickFromP 
              && toBuildP->hasAttribute("parameter_one_percent")
              && toBuildP->hasAttribute("parameter_two_percent") ) {
            //
            // create coordinates
            //
            float pX = muParseString(
                         replaceUsedFunctions(
                           getAttributeStr("parameter_one_percent", *toBuildP),
                           cValP, 
                           sFunP
                         )
                       );  
            float pY = muParseString(
                         replaceUsedFunctions(
                           getAttributeStr("parameter_two_percent", *toBuildP),
                           cValP, 
                           sFunP
                         )
                       );              
            //
            // ask analyticSurface for a point
            //
            basicP->push_back( 
              new dtPoint2(
                analyticSurface_partToPickFromP->uv_percent( dtPoint2(pX, pY)
                  
                )
              )
            );
          }
          else {
            //
            // error handling
            //
            dt__THROW(createBasic(),
                    << DTLOGEVAL(toBuildP->hasAttribute("phi") ) << LOGDEL
                    << DTLOGEVAL(toBuildP->hasAttribute("r") ) << LOGDEL
                    << DTLOGEVAL(toBuildP->hasAttribute("z") ) << LOGDEL
                    << DTLOGEVAL(toBuildP->hasAttribute("parameter_one_percent") ) << LOGDEL
                    << DTLOGEVAL(toBuildP->hasAttribute("parameter_two_percent") ) << LOGDEL
                    << DTLOGEVAL( qPrintable(toBuildP->attribute("phi")) ) << LOGDEL
                    << DTLOGEVAL( qPrintable(toBuildP->attribute("r")) ) << LOGDEL
                    << DTLOGEVAL( qPrintable(toBuildP->attribute("z")) ) << LOGDEL
                    << DTLOGEVAL( qPrintable(toBuildP->attribute("parameter_one_percent")) ) << LOGDEL
                    << DTLOGEVAL( qPrintable(toBuildP->attribute("parameter_two_percent")) ) << LOGDEL
                    << DTLOGEVAL( analyticSurface_partToPickFromP ) );
            return;
          }          
        }
        else {
          dt__THROW(createBasic(), 
                  << DTLOGEVAL( toBuildP->hasAttribute("parameter_one_percent") ) << LOGDEL
                  << DTLOGEVAL( qPrintable(toBuildP->attribute("attribute")) ) << LOGDEL
                  << DTLOGEVAL( qPrintable(toBuildP->attribute("part_label")) ) );                 
        }        
      }    
    }
    else if ( toBuildP->hasAttribute("attribute") && toBuildP->hasAttribute("function_label") ) {
      //
      //look for function to pick point from
      //
			dt__PTRASS(
				vec2dOneD const * const theF,
				vec2dOneD::ConstDownCast(
					sFunP->get( getAttributeStr("function_label", *toBuildP) )
				)
			);

      if ( getAttributeStr("attribute", *toBuildP) == "pick_order_from_function" ) {
        //
        // 1 analyticFunction
        //        
        if (theF && hasAttribute("parameter_percent_function", *toBuildP)
            && hasAttribute("number_points", *toBuildP) ) {
          int nPoints
				  = 
					getAttributeIntMuParse(
            "number_points", *toBuildP, cValP, sFunP
					);
          //
          // search functions
          //
					dt__PTRASS(
						scaOneD const * const thePara,
						scaOneD::ConstDownCast(
							sFunP->get(
								getAttributeStr( "parameter_percent_function", *toBuildP ) 
							)
						)
					);

          //get parameter range of function
          for (int ii=0;ii<nPoints;ii++) {
            dt__TOFLOAT(float iiF, ii);
						dt__TOFLOAT(float nPointsF, nPoints);
//            float para = thePara->YFloat( iiF / (nPointsF-1.) );
//            dtPoint2 YY;            
            //get value of scaFunction
            //pick points out of range
            dtPoint2 YY 
						=
						theF->YdtPoint2Percent(
						  thePara->YFloat( iiF / (nPointsF-1.) )
						);
//            if (vecValue.size() != 2) {
//              dt__THROW(createBasic(),
//                      << "Function is not a 2-dim function." << LOGDEL
//                      << DTLOGEVAL(vecValue.size()) );
//            }
            basicP->push_back( new dtPoint2(YY) );
          }
        }
        else {
          DTWARNINGWF(createBasic(), 
                  << DTLOGEVAL( hasAttribute("parameter_percent_function", *toBuildP) ) << LOGDEL
                  << DTLOGEVAL( hasAttribute("number_points", *toBuildP) ) << LOGDEL
                  << DTLOGEVAL( getAttributeStr("parameter_percent_function", *toBuildP) ) << LOGDEL
                  << DTLOGEVAL( getAttributeStr("number_points", *toBuildP) ) );
//                  << DTLOGEVAL( sF ) );
          return;        
        }
      }      
    }
		else if ( toBuildP->attribute("attribute") == "pick_order_from_file") {       
			if ( getAttributeStr("file_name", *toBuildP) != "" ) {
				ifstream in( getAttributeStr("file_name", *toBuildP).c_str() );
				std::vector< std::vector < double > > fields;
				if (in) {
					std::string line;
					while (getline(in, line)) {
						std::stringstream sep(line);
						std::string field;
						fields.push_back( std::vector< double >() );
						while (getline(sep, field, ',')) {
							fields.back().push_back( stringToFloat( field ) );
						}
					}
				}

				for ( int ii=0; ii<fields.size(); ii++ ) {
					if ( fields[ii].size() != 2 ) {
						dt__THROW(createBasic(),
										<< "field[" << ii << "] of read point has not a size of 2." << LOGDEL
										<< "file_name = " << getAttributeStr("file_name", *toBuildP));
					}
					basicP->push_back( new dtPoint2(fields[ii][0], fields[ii][1]) );          
				}
			}
			else {
				dt__THROW(createBasic(), << "Attribute file_name is empty");
			}
		}  		
    //
    // 4 unknown attribute
    //
    else {
      dt__THROW(createBasic(),
              << DTLOGEVAL(qPrintable(toBuildP->attribute("attribute"))) << LOGDEL
              << "Error creating point.");
    }
    //
    //transform points
    //
    if ( dtTransformerP->isNecessary() ) {
      vectorHandling< dtPoint2 * > basicPTwin;
      for (int ii=0;ii<basicP->size();ii++) {
        basicPTwin.push_back( basicP->at(ii) );
      }
      basicP->clear();
      *basicP = dtTransformerP->apply( &basicPTwin );
      //
      // output
      //
//      DTBUFFERINIT();
//      dt__FORALL(basicPTwin, ii,
//        DTBUFFER(<< DTLOGPOI2DP(basicPTwin[ii]) << "=>" << DTLOGPOI2DP(basicP->at(ii)) << LOGDEL );          
//      );
//      DTDEBUGWF_BUFFER(createBase());
//      for (int ii=0;ii<basicPTwin.size();ii++) {
//        delete basicPTwin[ii];
//      }
      basicPTwin.destroy();
    }

    //
    // point output
    //
    for (int ii=0;ii<basicP->size();ii++) {
      DTDEBUGWF(createBasic(),
              << "created point:" << DTLOGPOI2DP( (*basicP)[ii] ) );
    }    

  }

  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP,
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP, 
		vectorHandling< analyticGeometry * > const * const depAGeoP,
		std::vector< dtPoint2* > * basicP
	) const {
    //
    // create transformer if necessary
    //
    ptrHandling< dtTransformer > dtT(
		  createTransformer(toBuildP, bC, cValP, sFunP, depAGeoP)
		);
		QDomElement wEl = *toBuildP;
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }
    createBasic(&wEl, 
                bC, 
                cValP, 
                sFunP, 
                depAGeoP, 
                dtT.get(), 
                basicP);
  }

    void dtXmlParserBase::createAdvanced( 
		  QDomElement const * toBuildP,
			baseContainer * const bC,
			vectorHandling< constValue * > const * const cValP,
			vectorHandling< analyticFunction * > const * const sFunP, 
			vectorHandling< analyticGeometry * > const * const depAGeoP,            
			vectorHandling< analyticGeometry * > * advancedP 
		) const {

      //
      // create transformer if necessary
      //
      ptrHandling< dtTransformer > dtT(
			  createTransformer(toBuildP, bC, cValP, sFunP, depAGeoP)
			);
			QDomElement wEl = *toBuildP;
      if ( is("transformer", *toBuildP) ) {
        wEl = toBuildP->nextSiblingElement();
      }
      createAdvanced(&wEl,
                     bC,                    
                     cValP,
                     sFunP, 
                     depAGeoP, 
                     dtT.get(),                          
                     advancedP);      
    }

		/**
		 * 
     * @todo Maybe this could be done with builder on analyticGeometry-level.
		 *       This prevents the definition of many classes.
     */
    void dtXmlParserBase::createAdvanced( 
		  QDomElement const * toBuildP,
			baseContainer * const bC,
			vectorHandling< constValue * > const * const cValP,
			vectorHandling< analyticFunction * > const * const sFunP, 
			vectorHandling< analyticGeometry * > const * const depAGeoP, 
			dtTransformer const * const dtTransformerP,                          
			vectorHandling< analyticGeometry * > * advancedP ) const {

      for (int ii=0;ii<depAGeoP->size();ii++) {
        if ( (depAGeoP->at(ii))->getLabel() == getAttributeStr("label", *toBuildP) ) {
          advancedP->push_back( depAGeoP->at(ii)->clone() );
          break;
        }
        else if ( stringContains("*", getAttributeStr("label", *toBuildP)) ) {
          std::string pattern = stringRemoveSingle("*", getAttributeStr("label", *toBuildP) );
          if ( stringContains(pattern, depAGeoP->at(ii)->getLabel()) ) {
            advancedP->push_back( depAGeoP->at(ii)->clone() );
          }          
        }
      }
      
      if ( advancedP->size() == 0) {
        dt__THROW(createAdvanced(),
                << "Cannot find part " << getAttributeStr("label", *toBuildP) );
      }

      //
      // copy
      //
      vectorHandling< analyticGeometry * > advancedTwin;
      for (int ii=0;ii<advancedP->size();ii++) {
        advancedTwin.push_back( advancedP->at(ii) );
      }
      advancedP->clear();

      //
      // check for attributes
      //
      if ( toBuildP->hasAttribute("attribute") ) {
  //------------------------------------------------------------------------------
  // trim
  //------------------------------------------------------------------------------
        if ( getAttributeStr("attribute", *toBuildP) == "trim" ) {
          for (int ii=0;ii<advancedTwin.size();ii++) {
            oneDimTrimmable * toTrim = dynamic_cast< oneDimTrimmable * >(advancedTwin[ii]);
            if (toTrim) {
              float uuStart = muParseString( 
                                replaceUsedFunctions( 
                                  getAttributeStr(
                                    "parameter_one_percent_start", 
                                    *toBuildP
                                  ), 
                                  cValP, 
                                  sFunP
                                ) 
                              );
              float uuEnd = muParseString( 
                                replaceUsedFunctions( 
                                  getAttributeStr(
                                    "parameter_one_percent_end", 
                                    *toBuildP
                                  ), 
                                  cValP, 
                                  sFunP
                                ) 
                              );
              toTrim->trim(uuStart,uuEnd);
            }          
            else {
              DTWARNINGWF(createAdvanced(),
                      << DTLOGEVAL( getAttributeStr("attribute", *toBuildP) ) << LOGDEL
                      << DTLOGEVAL( advancedTwin[ii]->getLabel() ) << LOGDEL
                      << "function not defined on " << advancedTwin[ii]->ClassName() );     
            }
          }                
        }
  //------------------------------------------------------------------------------
  // forward_extend
  //------------------------------------------------------------------------------
        else if ( getAttributeStr("attribute", *toBuildP) == "forward_extend" ) {
          for (int ii=0;ii<advancedTwin.size();ii++) {
            oneDimForwardExtendable * toExt = dynamic_cast< oneDimForwardExtendable * >(advancedTwin[ii]);
            if (toExt) {
              DTINFOWF(createAdvanced(),
                      << "do a forward extension on new class");            
              toExt->forwardExtend();
            }          
            else {
              DTWARNINGWF(createAdvanced(),
                      << DTLOGEVAL( getAttributeStr("attribute", *toBuildP) ) << LOGDEL
                      << DTLOGEVAL( advancedTwin[ii]->getLabel() ) << LOGDEL
                      << "function not not defined on " << advancedTwin[ii]->ClassName() );     
            }
          }                
        }
  //------------------------------------------------------------------------------
  // backward_extend
  //------------------------------------------------------------------------------
        else if ( getAttributeStr("attribute", *toBuildP) == "backward_extend" ) {
          for (int ii=0;ii<advancedTwin.size();ii++) {
            oneDimBackwardExtendable * toExt = dynamic_cast< oneDimBackwardExtendable * >(advancedTwin[ii]);
            if (toExt) {
              DTINFOWF(createAdvanced(),
                      << "do a backward extension on new class");              
              toExt->backwardExtend();          
            }          
            else {
              DTWARNINGWF(createAdvanced(),
                      << DTLOGEVAL( getAttributeStr("attribute", *toBuildP) ) << LOGDEL
                      << DTLOGEVAL( advancedTwin[ii]->getLabel() ) << LOGDEL
                      << "function not not defined on " << advancedTwin[ii]->ClassName() );     
            }
          }                
        }
  //------------------------------------------------------------------------------
  // revert
  //------------------------------------------------------------------------------
        else if ( getAttributeStr("attribute", *toBuildP) == "revert" ) {
          for (int ii=0;ii<advancedTwin.size();ii++) {
            oneDimRevertable * oneDToRev = dynamic_cast< oneDimRevertable * >(advancedTwin[ii]);
            twoDimRevertable * twoDToRev = dynamic_cast< twoDimRevertable * >(advancedTwin[ii]);
            if (oneDToRev) {
              DTINFOWF(createAdvanced(),
                      << "revert geometry");            
              oneDToRev->revert();
            }          
            else if (twoDToRev) {
              DTINFOWF(createAdvanced(),
                      << "revert geometry");            
              twoDToRev->revert();
            }                
            else {
              DTWARNINGWF(createAdvanced(),
                      << DTLOGEVAL( getAttributeStr("attribute", *toBuildP) ) << LOGDEL
                      << DTLOGEVAL( advancedTwin[ii]->getLabel() ) << LOGDEL
                      << "function not defined on " << advancedTwin[ii]->ClassName() );     
            }
          }                
        }      
  //------------------------------------------------------------------------------
  // close
  //------------------------------------------------------------------------------
        else if ( getAttributeStr("attribute", *toBuildP) == "closeArithmetic" ) {
          for (int ii=0;ii<advancedTwin.size();ii++) {
            oneDimCloseable * toClose = dynamic_cast< oneDimCloseable * >(advancedTwin[ii]);
            if (toClose) {
              toClose->closeArithmetic();
            }
            else {
              DTWARNINGWF(createAdvanced(),
                      << DTLOGEVAL( getAttributeStr("attribute", *toBuildP) ) << LOGDEL
                      << DTLOGEVAL( advancedTwin[ii]->getLabel() ) << LOGDEL
                      << "function not not defined on " << advancedTwin[ii]->ClassName() );     
            }
          }                
        }        
        else {
          DTWARNINGWF(createAdvanced(),
                  << DTLOGEVAL( getAttributeStr("attribute", *toBuildP) ) << LOGDEL
                  << "Unknown attribute");
        }
      }

      //
      // transform
      //
      if ( dtTransformerP->isNecessary() ) {
        *advancedP = dtTransformerP->apply( &advancedTwin );
      }
      else {
        for (int ii=0;ii<advancedTwin.size();ii++) {
          advancedP->push_back( advancedTwin[ii]->clone() );
        }   
      }
			advancedTwin.destroy();
    }

    analyticGeometry * dtXmlParserBase::createAnalyticGeometry( 
		  QDomElement const * toBuildP,
			baseContainer * const bC,
			vectorHandling< constValue * > const * const cValP,
			vectorHandling< analyticFunction * > const * const sFunP, 
			vectorHandling< analyticGeometry * > const * const depAGeoP
		) const {

      vectorHandling< analyticGeometry * > advancedVec;
      createAdvanced(toBuildP, bC, cValP, sFunP, depAGeoP, &advancedVec);
      if (advancedVec.size() != 1) {
        dt__THROW(createAdvanced(), << DTLOGEVAL( advancedVec.size() ) );
      }
      return advancedVec[0];
    }
  
    void dtXmlParserBase::createAdvanced( 
		  QDomElement const * toBuildP,
			vectorHandling< constValue * > const * const cValP,
			vectorHandling< analyticFunction * > const * const sFunP, 
			dtTransformer const * const dtTransformerP,                          
			vectorHandling< analyticFunction * > * advancedP 
		) const {

//			dt__PTRASS(
//			  scaOneD const * const s1d, 
//				scaOneD::ConstDownCast( sFunP->get(getAttributeStr("label", *toBuildP)) )				
//			);
			analyticFunction const * aF = sFunP->get(getAttributeStr("label", *toBuildP));
			advancedP->push_back( aF->clone() );
			
      //
      // copy
      //
      vectorHandling< analyticFunction * > advancedTwin;
      for (int ii=0;ii<advancedP->size();ii++) {
        advancedTwin.push_back( advancedP->at(ii) );
      }
      advancedP->clear();

      //
      // transform
      //
      if ( dtTransformerP->isNecessary() ) {
        *advancedP = dtTransformerP->apply( &advancedTwin );
      }
      else {
        for (int ii=0;ii<advancedTwin.size();ii++) {
          advancedP->push_back( advancedTwin[ii] );
        }   
      }
    }  
		
    void dtXmlParserBase::createAdvanced( 
		  QDomElement const * toBuildP,
			vectorHandling< constValue * > const * const cValP,
			vectorHandling< analyticFunction * > const * const sFunP, 
			vectorHandling< analyticFunction * > * advancedP 
		) const {

      //
      // create transformer if necessary
      //
      ptrHandling< dtTransformer > dtTransformerP(
			  createTransformer(toBuildP, cValP, sFunP)
			);
			QDomElement wEl = *toBuildP;
      if ( is("transformer", *toBuildP) ) {
        wEl = toBuildP->nextSiblingElement();
      }
      createAdvanced(&wEl,                 
                     cValP,
                     sFunP, 
                     dtTransformerP.get(),                          
                     advancedP);      
    }
    
    analyticFunction * dtXmlParserBase::createAnalyticFunction( 
			QDomElement const * toBuildP,           
			vectorHandling< constValue * > const * const cValP,
			vectorHandling< analyticFunction * > const * const sFunP 
		) const {

      vectorHandling< analyticFunction * > advancedVec;
      createAdvanced(
        toBuildP,
        cValP,
        sFunP,
        &advancedVec
      );
      if (advancedVec.size() != 1) {
        dt__THROW(createAdvanced(), << DTLOGEVAL( advancedVec.size() ) );
      }
      return advancedVec[0];
    }
}