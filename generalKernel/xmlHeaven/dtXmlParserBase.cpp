#include "dtXmlParserBase.h"
#include "interfaceHeaven/ptrHandling.h"
#include <baseContainerHeaven/transformerContainer.h>
#include <baseContainerHeaven/baseBuilder/dtPoint3_readIBL.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticFunctionHeaven/vec2dOneD.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>

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
#include <analyticFunctionHeaven/analyticFunction.h>
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

namespace dtOO {
  dtXmlParserBase::dtXmlParserBase() {
  }

  dtXmlParserBase::~dtXmlParserBase() {
  }

  dtTransformer * dtXmlParserBase::createTransformer(
	  QDomElement const * toBuildP,
    baseContainer * const bC,                    
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF, 
		vectorHandling< analyticGeometry * > const * const aG
	) {
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
				if (aG) {
				  dtTransformerP->init(&wElement, bC, cV, aF, aG);			
				}
				else {
					dtTransformerP->init(&wElement, bC, cV, aF);			
				}
			}
			else if (hasLabel && inContainer && !hasName) {
				return bC->ptrTransformerContainer()->get(label)->clone();
			}
			else if (hasLabel && !inContainer && hasName) {
				dtTransformerP 
				= 
				dtTransformerFactory::create( name );
				if (aG) {
				  dtTransformerP->init(&wElement, bC, cV, aF, aG);			
				}
				else {
					dtTransformerP->init(&wElement, bC, cV, aF);
				}
        bC->ptrTransformerContainer()->add(dtTransformerP);
				delete dtTransformerP;
				dtTransformerP = dtTransformerFactory::create("doNothing");
			}
			else if (hasLabel && inContainer && hasName) {
				dtTransformerP = dtTransformerFactory::create("doNothing");
			}			
			else {
				dt__throw(
					createTransformer(),
					<< dt__eval(hasLabel) << std::endl
					<< dt__eval(hasName) << std::endl
					<< dt__eval(inContainer) << std::endl
					<< dt__eval(label) << std::endl
					<< dt__eval(name) << std::endl
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
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF
	) {
		return createTransformer(toBuildP, bC, cV, aF, NULL);
  }

  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP,
    baseContainer * const bC,                  
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF, 
		vectorHandling< analyticGeometry * > const * const aG,
		std::vector< dtPoint3 > * basicP
	) {
    //
    // create transformer if necessary
    //
		QDomElement wEl = *toBuildP;
    ptrHandling<dtTransformer> dtTransformerP(   
      createTransformer(toBuildP, bC, cV, aF, aG)
	  );
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }    
    //
    //create points
    //
		vectorHandling< dtPoint3 * > pp;		
    createBasic(
			&wEl, bC, cV, aF, aG, dtTransformerP.get(), &pp
		);		
		dt__forAllIndex(pp, ii) basicP->push_back( *(pp[ii]) );
  }
	
  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP,
    baseContainer * const bC,		
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF, 
		vectorHandling< analyticGeometry * > const * const aG,
		dtTransformer const * const dtTransformerP, 
		std::vector< dtPoint3* > * basicP
	) {
    if ( !is("Point_3", *toBuildP) ) {
      dt__throw(createBasic(), << dt__eval( getTagName(*toBuildP) ) );
    }

		//
		// check if point has a label and point is already created
		// return point and that's it
		//
		if ( hasAttribute("label", *toBuildP) ) {
			std::string label = getAttributeStr("label", *toBuildP);
			//
			// return multiple points
			//        
			if ( hasAttribute("number_points", *toBuildP) ) {
				int nPoints = getAttributeInt("number_points", *toBuildP);
				for (int ii=0;ii<nPoints;ii++) {
					std::string pointName;
					pointName = label;
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
					pointName = label;
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
			else if ( stringContains("[", label) && stringContains("]", label) ) {
				std::vector< std::string > forStr
			  = 
				convertToStringVector(
					":", ":", getStringBetweenAndRemove("[", "]", &label)
				);
				int const from = stringToInt(forStr[0]);
				int const spacing = stringToInt(forStr[1]);
				int const to = stringToInt(forStr[2]);

				for (int ii=from; ii<=to; ii=ii+spacing) {
					std::string pointName;
					pointName = label;
					pointName += "_";
					pointName += intToString(ii);
					if ( bC->ptrPointContainer()->has( pointName ) ) {
						basicP->push_back( 
						  new dtPoint3(bC->ptrPointContainer()->get(pointName)) 
						);
					}
				}
				return;				
			}
			//
			// return a single point
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
										 getAttributeStr("x", *toBuildP), cV,  aF
									 )
								 );
			float cY = muParseString(
									 replaceUsedFunctions(
										 getAttributeStr("y", *toBuildP), cV, aF
									 )
								 );
			float cZ = muParseString(
									 replaceUsedFunctions(
										 getAttributeStr("z", *toBuildP), cV, aF
									 )
								 );        
			basicP->push_back( new dtPoint3(cX, cY, cZ) );      
		}
		/* ---------------------------------------------------------------------- */
		/* pick point from a part */
		/* ---------------------------------------------------------------------- */          
		else if (    hasAttribute("attribute", *toBuildP) 
						  && hasAttribute("part_label", *toBuildP) ) {
			analyticGeometry const * anAG 
			= 
			aG->get( getAttributeStr("part_label", *toBuildP) );
			map1dTo3d const * m1d = map1dTo3d::ConstDownCast(anAG);
			map2dTo3d const * m2d = map2dTo3d::ConstDownCast(anAG);
			map3dTo3d const * m3d = map3dTo3d::ConstDownCast(anAG);

			if (!m1d && !m2d && !m3d) {
				dt__throw(
				  createBasic(),
					<< dt__eval( getAttributeStr("part_label", *toBuildP) ) << std::endl
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
													 getAttributeStr("phi", *toBuildP), cV,  aF
												 )
											 );
						float cY = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("z", *toBuildP), cV, aF
												 )
											 );                      
						basicP->push_back( new dtPoint3( rS->xyz_phiZ(cX, cY) ) );
					}
					else if (    hasAttribute("parameter_one_percent", *toBuildP)
										&& hasAttribute("parameter_two_percent", *toBuildP) ) {
						float cX = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_one_percent", *toBuildP),
													 cV, 
													 aF
												 )
											 );
						float cY = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_two_percent", *toBuildP),
													 cV, 
													 aF
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
												 cV, 
												 aF
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
													 cV, 
													 aF
												 )
											 );
						float cY = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_two_percent", *toBuildP),
													 cV, 
													 aF
												 )
											 );
						float cZ = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_three_percent", *toBuildP),
													 cV, 
													 aF
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
													 cV, 
													 aF
												 )
											 );
						float cY = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_two", *toBuildP),
													 cV, 
													 aF
												 )
											 );
						float cZ = muParseString(
												 replaceUsedFunctions(
													 getAttributeStr("parameter_three", *toBuildP),
													 cV, 
													 aF
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
				// 1 map1dTo3d
				//        
				if ( m1d && hasAttribute("number_points_one", *toBuildP) ) {
					int nPointsOne = getAttributeInt("number_points_one", *toBuildP);					
				  float nPointsOneF = static_cast<float>(nPointsOne);              					
					if ( hasAttribute("parameter_one_percent_function", *toBuildP) ) {
						dt__ptrAss(
							scaOneD const * sF,
							scaOneD::ConstDownCast(
								aF->get( 
									getAttributeStr( "parameter_one_percent_function", *toBuildP )
								)
							)
						);
						for (int ii=0;ii<nPointsOne;ii++) {
							float iiF = static_cast<float>(ii);
							float uu = sF->YFloat( iiF / (nPointsOneF-1.) ) ;
							basicP->push_back( 
								new dtPoint3( m1d->getPointPercent(uu) )
							);              
						}
					}
					else if (hasAttribute("length_one_percent_function", *toBuildP)) {
						dt__ptrAss(
							scaOneD const * sF,
							scaOneD::ConstDownCast(
								aF->get( 
									getAttributeStr( "length_one_percent_function", *toBuildP )
								)
							)
						);
						float length = m1d->length();
						float uP;// = m1d->u_l(0.*length);
						for (int ii=0; ii<nPointsOne; ii++) {
							float iiF = static_cast<float>(ii);
							float uu = sF->YFloat( iiF / (nPointsOneF-1.) );
							uP = m1d->u_l(uu*length);
              basicP->push_back( new dtPoint3(m1d->getPoint(uP)) );              					
						}
					}
				}				
				//
				// 1 map2dTo3d
				//        
				else if ( m2d
									&& hasAttribute("parameter_one_percent_function", *toBuildP)
									&& hasAttribute("parameter_two_percent_function", *toBuildP)
									&& hasAttribute("number_points_one", *toBuildP)
									&& hasAttribute("number_points_two", *toBuildP) 
				) {
					int nPointsOne = getAttributeInt("number_points_one", *toBuildP);
					int nPointsTwo = getAttributeInt("number_points_two", *toBuildP);
					//
					// search functions
					//
					dt__ptrAss(
						scaOneD const * fOne, 
						scaOneD::ConstDownCast(
							aF->get( 
								getAttributeStr("parameter_one_percent_function", *toBuildP)
							)
						)
					);
					dt__ptrAss(
						scaOneD const * fTwo,
						scaOneD::ConstDownCast(
							aF->get( 
								getAttributeStr("parameter_two_percent_function", *toBuildP)
							)
						)
					);
					for (int ii=0;ii<nPointsOne;ii++) {
						for (int jj=0;jj<nPointsTwo;jj++) {
							dt__toFloat(float iiF, ii);
							dt__toFloat(float jjF, jj);
							float paraOne = fOne->YFloat( iiF / nPointsOne ) ;
							float paraTwo = fTwo->YFloat( jjF / nPointsTwo ) ;
							basicP->push_back( 
							  new dtPoint3( m2d->getPointPercent( paraOne, paraTwo ) )
							);              
						}
					}
				}                    
			}
		}
		/* ---------------------------------------------------------------------- */
		/* pick point from a function */
		/* ---------------------------------------------------------------------- */          
		else if (    hasAttribute("attribute", *toBuildP) 
						  && hasAttribute("function_label", *toBuildP) ) {
			analyticFunction const * anAF
			= 
			aF->get( getAttributeStr("function_label", *toBuildP) );
			vec3dOneD const * v1D = vec3dOneD::ConstDownCast(anAF);
			vec3dTwoD const * v2D = vec3dTwoD::ConstDownCast(anAF);

			dt__throwIfWithMessage(
				!v1D&&!v2D,
				createBasic(),
				<< dt__eval( getAttributeStr("function_label", *toBuildP) ) << std::endl
				<< "No such analyticFunction or analyticFunction has wrong type."
			);     

			if ( getAttributeStr("attribute", *toBuildP) == "pick_from_function" ) {
				if ( v1D && hasAttribute("x_one", *toBuildP) ) {
					float cX 
					= 
					muParseString(
						replaceUsedFunctions(
							getAttributeStr("x_one", *toBuildP), cV,  aF
						)
					);
					basicP->push_back( new dtPoint3( v1D->YdtPoint3(cX) ) );
				}
				else if ( v1D && hasAttribute("x_one_percent", *toBuildP) ) {
					float cX
					= 
					muParseString(
						replaceUsedFunctions(
							getAttributeStr("x_one_percent", *toBuildP), cV,  aF
						)
					);
					basicP->push_back( new dtPoint3( v1D->YdtPoint3Percent(cX) ) );
				}				
				else if ( 
					v2D 
					&& hasAttribute("x_one", *toBuildP) 
					&& hasAttribute("x_two", *toBuildP) 
				) {
					float cX 
					= 
					muParseString(
						replaceUsedFunctions(
							getAttributeStr("x_one", *toBuildP), cV,  aF
						)
					);
					float cY 
					= 
					muParseString(
						replaceUsedFunctions(
							getAttributeStr("x_two", *toBuildP), cV, aF
						)
					);                      
					basicP->push_back( 
					  new dtPoint3( v2D->YdtPoint3( analyticFunction::aFXTwoD(cX, cY) ) )
					);
				}	
				else if ( 
				  v2D 
					&& hasAttribute("x_one_percent", *toBuildP) 
					&& hasAttribute("x_two_percent", *toBuildP) 
				) {
					float cX
					= 
					muParseString(
						replaceUsedFunctions(
							getAttributeStr("x_one_percent", *toBuildP), cV,  aF
						)
					);
					float cY
					= 
					muParseString(
						replaceUsedFunctions(
							getAttributeStr("x_two_percent", *toBuildP), cV, aF
						)
					);                      
					basicP->push_back( new dtPoint3( v2D->YdtPoint3Percent(cX, cY) ) );
				}				
			}			
		}
		else if ( toBuildP->attribute("attribute") == "pick_order_from_file") {
			std::string filename = getAttributeStr("file_name", *toBuildP);
			if ( filename != "" ) {
				std::vector< dtPoint3 > p3;
				if (filename.find(".ibl") != std::string::npos) {
					p3 = dtPoint3_readIBL(filename).result();
					dt__forAllConstIter(std::vector< dtPoint3 >, p3, it) {
						basicP->push_back( new dtPoint3(*it) );
					}
				}
				else dt__throw(createBasic(), << "Unknown file extension.");
			}
			else dt__throw(createBasic(), << "attribute file_name is empty.");
		}
		
		//
		// check if a point was created
		//
		dt__throwIfWithMessage(
			basicP->size()==0,
			createBasic(), 
			<< "No point could be created." << std::endl
			<< convertToString(*toBuildP) 
		);	
		
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
		std::vector< float > itVal;
		for (int ii=0;ii<basicP->size();ii++) {
			itVal.push_back( basicP->at(ii)->x() );
			itVal.push_back( basicP->at(ii)->y() );
			itVal.push_back( basicP->at(ii)->z() );
		}    
		std::vector< std::string > header;
		header.push_back("p_x"); header.push_back("p_y"); header.push_back("p_z");
		dt__debug(
  	  createBasic(), 
			<< "created points" << std::endl
			<< logMe::floatVecToTable(header, itVal)
		);

  }

	/**
	 * 
   * @todo Function does not store two dimensional vector in container.
   */
  dtVector2 dtXmlParserBase::createDtVector2(
	  QDomElement const * toBuildP,
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cV,  
		vectorHandling< analyticFunction * > const * const aF, 
		vectorHandling< analyticGeometry * > const * const aG
	) {  
		dtVector2 vv;

    if ( !is("Vector_2", *toBuildP) && !is("dtVector2", *toBuildP) ) {
      dt__throw(createBasic(), << dt__eval( getTagName(*toBuildP) ) );
    }
    else {
      //
      // check if vector has a label and point is already created
      // return point and that's it
      //
//      if ( toBuildP->hasAttribute("label") ) {
//        if ( bC->ptrVectorContainer()->has(toBuildP->attribute("label").toStdString()) ) {
////          *basicP = new dtVector3(0,0,0);
//          vv = bC->ptrVectorContainer()->get( toBuildP->attribute("label").toStdString() );
//          return vv;
//        }
//      }
      //
      // check for attribute x, y and z
      //
      if ( toBuildP->hasAttribute("x") && toBuildP->hasAttribute("y") ) { 
        //
        // create coordinates
        //
        float cX = muParseString(
                     replaceUsedFunctions(
                       getAttributeStr("x", *toBuildP),
                       cV, 
                       aF
                     )
                   );  
        float cY = muParseString(
                     replaceUsedFunctions(
                       getAttributeStr("y", *toBuildP),
                       cV, 
                       aF
                     )
                   );            
        //
        // create vector
        //
        vv = dtVector2(cX, cY);
      }
      else if ( toBuildP->hasAttribute("u") && toBuildP->hasAttribute("v") ) { 
        //
        // create coordinates
        //
        float cX = muParseString(
                     replaceUsedFunctions(
                       getAttributeStr("u", *toBuildP),
                       cV, 
                       aF
                     )
                   );  
        float cY = muParseString(
                     replaceUsedFunctions(
                       getAttributeStr("v", *toBuildP),
                       cV, 
                       aF
                     )
                   );            
        //
        // create vector
        //
        vv = dtVector2(cX, cY);
      }			
//      //
//      //check if point has a label and add it to the container
//      //
//      if ( toBuildP->hasAttribute("label") ) {
//        bC->ptrVectorContainer()->add( vv, toBuildP->attribute("label").toStdString() );
//      }
      //
      //output
      //
      dt__debug(
				createDtVector2(),
        << "created vector: (" << vv.x() << " / " << vv.y() << ")"
			);
			
			return vv;
    }
  }
	
  dtVector3 dtXmlParserBase::createDtVector3(
	  QDomElement const * toBuildP,
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cV,  
		vectorHandling< analyticFunction * > const * const aF, 
		vectorHandling< analyticGeometry * > const * const aG
	) {
		dtVector3 vv;

    if ( !is("Vector_3", *toBuildP) ) {
      dt__throw(createBasic(),
              << dt__eval(qPrintable( toBuildP->tagName() ) ) );
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
                       cV, 
                       aF
                     )
                   );  
        float cY = muParseString(
                     replaceUsedFunctions(
                       getAttributeStr("y", *toBuildP),
                       cV, 
                       aF
                     )
                   );
        float cZ = muParseString(
                     replaceUsedFunctions(
                       getAttributeStr("z", *toBuildP),
                       cV, 
                       aF
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
      dt__debug(createBasic(),
              << "created vector: (" 
              << vv.x() << " / "
              << vv.y() << " / "
              << vv.z() << ")");
			
			return vv;
    }
  }

  dtVector3 dtXmlParserBase::createDtVector3(
	  QDomElement const * toBuildP,
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cV,  
		vectorHandling< analyticFunction * > const * const aF
	) {  
    return createDtVector3(toBuildP, bC, cV, aF, NULL); 
  }	

//	dtPoint3 dtXmlParserBase::getDtPoint3(
//		QDomElement const * toBuildP,
//		baseContainer const * const bC,
//		vectorHandling< constValue * > const * const cV,
//		vectorHandling< analyticFunction * > const * const aF, 
//		vectorHandling< analyticGeometry * > const * const aG
//	) {
//		return createDtPoint3(toBuildP, NULL, cV, aF, aG);		
//	}
	
	dtVector2 dtXmlParserBase::getDtVector2(
		QDomElement const * toBuildP,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,  
		vectorHandling< analyticFunction * > const * const aF, 
		vectorHandling< analyticGeometry * > const * const aG
	) {
		return createDtVector2(toBuildP, NULL, cV, aF, aG);
	} 
	
	dtPoint3 dtXmlParserBase::getDtPoint3(
		QDomElement const * toBuildP,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,  
		vectorHandling< analyticFunction * > const * const aF, 
		vectorHandling< analyticGeometry * > const * const aG
	) {
		if ( hasAttribute("label", *toBuildP) ) {
			return bC->constPtrPointContainer()->get(getAttributeStr("label", *toBuildP));
		}
		else {
			return createDtPoint3(toBuildP, NULL, cV, aF, aG);
		}
	} 
	
	dtPoint3 dtXmlParserBase::getDtPoint3(
		QDomElement const * toBuildP,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,  
		vectorHandling< analyticFunction * > const * const aF
	) {
		if ( hasAttribute("label", *toBuildP) ) {
			return bC->constPtrPointContainer()->get(getAttributeStr("label", *toBuildP));
		}
		else {
			return createDtPoint3(toBuildP, NULL, cV, aF);
		}		
	}
		
	dtVector3 dtXmlParserBase::getDtVector3(
		QDomElement const * toBuildP,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,  
		vectorHandling< analyticFunction * > const * const aF, 
		vectorHandling< analyticGeometry * > const * const aG
	) {
		if ( hasAttribute("label", *toBuildP) ) {
			return bC->constPtrVectorContainer()->get(getAttributeStr("label", *toBuildP));
		}
		else {
			return createDtVector3(toBuildP, NULL, cV, aF, aG);
		}
	} 
	
	dtVector3 dtXmlParserBase::getDtVector3(
		QDomElement const * toBuildP,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,  
		vectorHandling< analyticFunction * > const * const aF
	) {
		if ( hasAttribute("label", *toBuildP) ) {
			return bC->constPtrVectorContainer()->get(getAttributeStr("label", *toBuildP));
		}
		else {
			return createDtVector3(toBuildP, NULL, cV, aF);
		}		
	}
	
  dtVector3 dtXmlParserBase::getDtVector3(
	  std::string const & str, baseContainer const * const bC
	) {
		return bC->constPtrVectorContainer()->get(str);
	}

  dtVector3 dtXmlParserBase::getDtVector3(
	  QDomElement const * toBuildP, baseContainer const * const bC
	) {
		return getDtVector3(getAttributeStr("label", *toBuildP), bC);
	}
	
  dtPoint3 dtXmlParserBase::getDtPoint3(
	  std::string const & str, baseContainer const * const bC
	) {
		return bC->constPtrPointContainer()->get(str);
	}

  dtPoint3 dtXmlParserBase::getDtPoint3(
	  QDomElement const * toBuildP, baseContainer const * const bC
	) {
		return getDtPoint3(getAttributeStr("label", *toBuildP), bC);
	}	
	
  std::string dtXmlParserBase::createString(
	  QDomElement const * toBuildP, 
		vectorHandling< constValue * > const * const cV, 
		vectorHandling< analyticFunction * > const * const aF
	) {
    if ( getTagName(*toBuildP) != "string" ) {
      dt__throw(createBasic(), << dt__eval( getTagName(*toBuildP) ) );
    }
		//
		// check for attribute value
		//
		if ( hasAttribute("value", *toBuildP)  ) {
			// create point
			return replaceUsedFunctions( 
							 getAttributeStr("value", *toBuildP), cV, aF
						 );
		}  
  }

  dtPoint3 dtXmlParserBase::createDtPoint3(
	  QDomElement const * toBuildP,
    baseContainer * const bC,                   
    vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF, 
		vectorHandling< analyticGeometry * > const * const aG) {
//		createBasic(toBuildP, pointContainerP, vectorContainerP, cV, aF, aG, &basicP);
    std::vector< dtPoint3 > basicVec;

    createBasic(toBuildP,
                bC,                    
                cV,
                aF, 
                aG,
                &basicVec);
    if (basicVec.size() != 1) {
      dt__throw(createBasic(),
              << "Try to create one point, but function "
              << "createBasic() returns more than one point.");
    }
		dtPoint3 ret = basicVec[0];	
		return ret;
	}
	
  dtPoint3 dtXmlParserBase::createDtPoint3(
	  QDomElement const * toBuildP,
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cV,  
		vectorHandling< analyticFunction * > const * const aF
	) {  
    return createDtPoint3(toBuildP, bC, cV, aF, NULL); 
  }	
	
  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP, 
    baseContainer * const bC,      					
		vectorHandling< constValue * > const * const cV, 
		vectorHandling< analyticFunction * > const * const aF, 
		std::vector< dtPoint2 > * basicP
	) {
    ptrHandling< dtTransformer > dtT(
		  createTransformer(toBuildP, bC, cV, aF)
		);		
		QDomElement wEl = *toBuildP;
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }		
		std::vector< dtPoint2 * > pp;
    createBasic(&wEl, NULL, cV, aF, NULL, dtT.get(), &pp);      		
		dt__forAllIndex(pp, ii) basicP->push_back( *(pp[ii]) );
  }

  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP,
    baseContainer * const bC,					
		vectorHandling< constValue * > const * const cV, 
		vectorHandling< analyticFunction * > const * const aF, 
		std::vector< dtPoint3 > * basicP
	) {
		QDomElement wEl = *toBuildP;
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }		
    createBasic(&wEl, bC, cV, aF, NULL, basicP);      		
  }	

  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP,
    baseContainer * const bC,            
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF, 
		vectorHandling< analyticGeometry * > const * const aG,
		dtTransformer const * const dtTransformerP, 
		std::vector< dtPoint2 * > * basicP
	) {

    /* ------------------------------------------------------------------------ */
    /* error handling */
    /* ------------------------------------------------------------------------ */
    bool isdtPoint2 = is("Point_2", *toBuildP);
    if ( !isdtPoint2  ) {
      dt__throw(createBasic(), << dt__eval( getTagName(*toBuildP) ) );
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
                     cV, 
                     aF
                   )
                 );  
      float cY = muParseString(
                   replaceUsedFunctions(
                     getAttributeStr("y", *toBuildP),
                     cV, 
                     aF
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
      analyticGeometry const * anAG = aG->get( getAttributeStr("part_label", *toBuildP) );
      analyticSurface_partToPickFromP = dynamic_cast< analyticSurface const * >( anAG );
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
                           cV, 
                           aF
                         )
                       );  
            float pY = muParseString(
                         replaceUsedFunctions(
                           getAttributeStr("parameter_two_percent", *toBuildP),
                           cV, 
                           aF
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
            dt__throw(createBasic(),
                    << dt__eval(toBuildP->hasAttribute("phi") ) << std::endl
                    << dt__eval(toBuildP->hasAttribute("r") ) << std::endl
                    << dt__eval(toBuildP->hasAttribute("z") ) << std::endl
                    << dt__eval(toBuildP->hasAttribute("parameter_one_percent") ) << std::endl
                    << dt__eval(toBuildP->hasAttribute("parameter_two_percent") ) << std::endl
                    << dt__eval( qPrintable(toBuildP->attribute("phi")) ) << std::endl
                    << dt__eval( qPrintable(toBuildP->attribute("r")) ) << std::endl
                    << dt__eval( qPrintable(toBuildP->attribute("z")) ) << std::endl
                    << dt__eval( qPrintable(toBuildP->attribute("parameter_one_percent")) ) << std::endl
                    << dt__eval( qPrintable(toBuildP->attribute("parameter_two_percent")) ) << std::endl
                    << dt__eval( analyticSurface_partToPickFromP ) );
            return;
          }          
        }
        else {
          dt__throw(createBasic(), 
                  << dt__eval( toBuildP->hasAttribute("parameter_one_percent") ) << std::endl
                  << dt__eval( qPrintable(toBuildP->attribute("attribute")) ) << std::endl
                  << dt__eval( qPrintable(toBuildP->attribute("part_label")) ) );                 
        }        
      }    
    }
    else if ( toBuildP->hasAttribute("attribute") && toBuildP->hasAttribute("function_label") ) {
      //
      //look for function to pick point from
      //
			dt__ptrAss(
				vec2dOneD const * const theF,
				vec2dOneD::ConstDownCast(
					aF->get( getAttributeStr("function_label", *toBuildP) )
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
            "number_points", *toBuildP, cV, aF
					);
          //
          // search functions
          //
					dt__ptrAss(
						scaOneD const * const thePara,
						scaOneD::ConstDownCast(
							aF->get(
								getAttributeStr( "parameter_percent_function", *toBuildP ) 
							)
						)
					);

          //get parameter range of function
          for (int ii=0;ii<nPoints;ii++) {
            dt__toFloat(float iiF, ii);
						dt__toFloat(float nPointsF, nPoints);
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
//                      << "Function is not a 2-dim function." << std::endl
//                      << dt__eval(vecValue.size()) );
//            }
            basicP->push_back( new dtPoint2(YY) );
          }
        }
        else {
          dt__warning(createBasic(), 
                  << dt__eval( hasAttribute("parameter_percent_function", *toBuildP) ) << std::endl
                  << dt__eval( hasAttribute("number_points", *toBuildP) ) << std::endl
                  << dt__eval( getAttributeStr("parameter_percent_function", *toBuildP) ) << std::endl
                  << dt__eval( getAttributeStr("number_points", *toBuildP) ) );
//                  << dt__eval( sF ) );
          return;        
        }
      }      
    }
		else if ( toBuildP->attribute("attribute") == "pick_order_from_file") {       
			if ( getAttributeStr("file_name", *toBuildP) != "" ) {
				std::ifstream in( getAttributeStr("file_name", *toBuildP).c_str() );
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
						dt__throw(createBasic(),
										<< "field[" << ii << "] of read point has not a size of 2." << std::endl
										<< "file_name = " << getAttributeStr("file_name", *toBuildP));
					}
					basicP->push_back( new dtPoint2(fields[ii][0], fields[ii][1]) );          
				}
				//
				// close file
				//
				in.close();
			}
			else {
				dt__throw(createBasic(), << "Attribute file_name is empty");
			}
		}  		
    //
    // 4 unknown attribute
    //
    else {
      dt__throw(
				createBasic(),
        << dt__eval(getAttributeStr("attribute", *toBuildP)) << std::endl
        << "Error creating point."
			);
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
//        DTBUFFER(<< dt__point2dP(basicPTwin[ii]) << "=>" << dt__point2dP(basicP->at(ii)) << std::endl );          
//      );
//      dt__debug_BUFFER(createBase());
//      for (int ii=0;ii<basicPTwin.size();ii++) {
//        delete basicPTwin[ii];
//      }
      basicPTwin.destroy();
    }

    //
    // point output
    //
    for (int ii=0;ii<basicP->size();ii++) {
      dt__debug(
			  createBasic(),
        << "created point:" << dt__point2d( (*(basicP->at(ii))) )
			);
    }    

  }

  void dtXmlParserBase::createBasic(
	  QDomElement const * toBuildP,
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF, 
		vectorHandling< analyticGeometry * > const * const aG,
		std::vector< dtPoint2 > * basicP
	) {
    //
    // create transformer if necessary
    //
    ptrHandling< dtTransformer > dtT(
		  createTransformer(toBuildP, bC, cV, aF, aG)
		);
		QDomElement wEl = *toBuildP;
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }
		vectorHandling< dtPoint2 * > pp;
    createBasic(&wEl, bC, cV, aF, aG, dtT.get(), &pp);
		dt__forAllIndex(pp, ii) basicP->push_back( *(pp[ii]) );		
  }

    void dtXmlParserBase::createAdvanced( 
		  QDomElement const * toBuildP,
			baseContainer * const bC,
			vectorHandling< constValue * > const * const cV,
			vectorHandling< analyticFunction * > const * const aF, 
			vectorHandling< analyticGeometry * > const * const aG,            
			vectorHandling< analyticGeometry * > * advancedP 
		) {

      //
      // create transformer if necessary
      //
      ptrHandling< dtTransformer > dtT(
			  createTransformer(toBuildP, bC, cV, aF, aG)
			);
			QDomElement wEl = *toBuildP;
      if ( is("transformer", *toBuildP) ) {
        wEl = toBuildP->nextSiblingElement();
      }
      createAdvanced(&wEl, bC, cV, aF, aG, dtT.get(), advancedP);      
    }

		/**
		 * 
     * @todo Maybe this could be done with builder on analyticGeometry-level.
		 *       This prevents the definition of many classes.
     */
    void dtXmlParserBase::createAdvanced( 
		  QDomElement const * toBuildP,
			baseContainer * const bC,
			vectorHandling< constValue * > const * const cV,
			vectorHandling< analyticFunction * > const * const aF, 
			vectorHandling< analyticGeometry * > const * const aG, 
			dtTransformer const * const dtTransformerP,                          
			vectorHandling< analyticGeometry * > * advancedP 
		) {
			//
			// get label
			//
			std::string label = getAttributeStr("label", *toBuildP);
			
      if ( stringContains("*", label) ) {
				//
				// string contains "*" --> return set of analyticGeometries
				//				
				std::string pattern 
				= 
				getStringBetween("*", "*", label );
				for (int ii=0;ii<aG->size();ii++) { 
          if ( stringContains(pattern, aG->at(ii)->getLabel()) ) {
            advancedP->push_back( aG->at(ii)->clone() );
          }					
				}
			}
			else if ( stringContains("~", label) ) {
				//
				// string contains "~" --> transform analyticGeometry
				//
				
				std::vector< std::string > transLabels;
				std::string tmpString = getStringBetweenAndRemove("~", "(", &label);
				while (tmpString != "") {
          transLabels.push_back( tmpString );
					tmpString = getStringBetweenAndRemove("~", "(", &label);
				}
				
				std::string aGLabel = getStringBetweenAndRemove("", ")", &label);
				dt__pH(analyticGeometry) toTrans(aG->get(aGLabel)->clone());
				for (int ii=transLabels.size()-1; ii>=0; ii--) {
					dt__info(
						createAdvanced(),
						<< "Applying " << dt__eval(transLabels[ii]) << " to " 
						<< dt__eval(aGLabel) << "."
					);
					
					dtTransformer const * const dtT 
					= 
					bC->ptrTransformerContainer()->get(transLabels[ii]);
					toTrans.reset(dtT->apply( toTrans.get() ));
				}
				advancedP->push_back( toTrans->clone() );
			}
			else {
				//
				// normal case
				//				
				advancedP->push_back(
				  aG->get( label )->clone()
				);
			}
      
      if ( advancedP->size() == 0) {
        dt__throw(createAdvanced(),
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
			vectorHandling< constValue * > const * const cV,
			vectorHandling< analyticFunction * > const * const aF, 
			vectorHandling< analyticGeometry * > const * const aG
		) {
      vectorHandling< analyticGeometry * > advancedVec;
      createAdvanced(toBuildP, bC, cV, aF, aG, &advancedVec);
      if (advancedVec.size() != 1) {
        dt__throw(createAdvanced(), << dt__eval( advancedVec.size() ) );
      }
      return advancedVec[0];
    }
		
    analyticGeometry * dtXmlParserBase::createAnalyticGeometry( 
		  QDomElement const * toBuildP,
			vectorHandling< constValue * > const * const cV,
			vectorHandling< analyticFunction * > const * const aF, 
			vectorHandling< analyticGeometry * > const * const aG
		) {
      return createAnalyticGeometry(toBuildP, NULL, cV, aF, aG);			
		}		
  
    void dtXmlParserBase::createAdvanced( 
		  QDomElement const * toBuildP,
			baseContainer * const bC,
			vectorHandling< constValue * > const * const cV,
			vectorHandling< analyticFunction * > const * const aF, 
			dtTransformer const * const dtTransformerP,                          
			vectorHandling< analyticFunction * > * advancedP 
		) {
			//
			// get label
			//
			std::string label = getAttributeStr("label", *toBuildP);
		  dt__info(createAdvanced(), << dt__eval(label));
      
			if ( stringContains("*", label) ) {
				//
				// string contains "*" --> return set of analyticGeometries
				//				
				std::string pattern 
				= 
				stringRemoveSingle("*", label );
				for (int ii=0;ii<aF->size();ii++) { 
          if ( stringContains(pattern, aF->at(ii)->getLabel()) ) {
            advancedP->push_back( aF->at(ii)->clone() );
          }					
				}
			}
			else if ( stringContains("~", label) ) {
				//
				// string contains "~" --> transform analyticFunction
				//
				
				std::vector< std::string > transLabels;
				std::string tmpString = getStringBetweenAndRemove("~", "(", &label);
				while (tmpString != "") {
          transLabels.push_back( tmpString );
					tmpString = getStringBetweenAndRemove("~", "(", &label);
				}
				
				std::string aFLabel = getStringBetweenAndRemove("", ")", &label);
				dt__pH(analyticFunction) toTrans(aF->get(aFLabel)->clone());
				for (int ii=transLabels.size()-1; ii>=0; ii--) {
					dt__info(
						createAdvanced(),
						<< "Applying " << dt__eval(transLabels[ii]) << " to " 
						<< dt__eval(aFLabel) << "."
					);
					
					dtTransformer const * const dtT 
					= 
					bC->ptrTransformerContainer()->get(transLabels[ii]);
					toTrans.reset(dtT->apply( toTrans.get() ));
				}
				advancedP->push_back( toTrans->clone() );
			}
			else {
				//
				// normal case
				//				
				advancedP->push_back(aF->get( label )->clone());
			}
			
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
			baseContainer * const bC,
			vectorHandling< constValue * > const * const cV,
			vectorHandling< analyticFunction * > const * const aF, 
			vectorHandling< analyticFunction * > * advancedP 
		) {

      //
      // create transformer if necessary
      //
      ptrHandling< dtTransformer > dtTransformerP(
			  createTransformer(toBuildP, bC, cV, aF)
			);
			QDomElement wEl = *toBuildP;
      if ( is("transformer", *toBuildP) ) {
        wEl = toBuildP->nextSiblingElement();
      }
      createAdvanced(&wEl, bC, cV, aF, dtTransformerP.get(), advancedP);      
    }
    
    analyticFunction * dtXmlParserBase::createAnalyticFunction( 
			QDomElement const * toBuildP,
			baseContainer * const bC,
			vectorHandling< constValue * > const * const cV,
			vectorHandling< analyticFunction * > const * const aF 
		) {

      vectorHandling< analyticFunction * > advancedVec;
      createAdvanced(toBuildP, bC, cV, aF, &advancedVec);
      if (advancedVec.size() != 1) {
        dt__throw(createAdvanced(), << dt__eval( advancedVec.size() ) );
      }
      return advancedVec[0];
    }
}