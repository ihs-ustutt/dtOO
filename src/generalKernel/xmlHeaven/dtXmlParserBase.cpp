#include "dtXmlParserBase.h"

#include <baseContainerHeaven/baseBuilder/dtPoint3_readIBL.h>
#include <baseContainerHeaven/baseBuilder/dtPoint3_readCSV.h>
#include <baseContainerHeaven/baseBuilder/dtPointD_readCSV.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticFunctionHeaven/aFBuilder/float_scaOneDPoint.h>
#include <analyticFunctionHeaven/vec2dOneD.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>
#include <baseContainerHeaven/transformerContainer.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <dtTransformerHeaven/doNothing.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticRotatingMap1dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>

#include <parseHeaven/dtParser.h>

namespace dtOO {
  dtXmlParserBase::dtXmlParserBase() {
  }

  dtXmlParserBase::~dtXmlParserBase() {
  }

  dtTransformer * dtXmlParserBase::createTransformer(
	  ::QDomElement const * toBuildP,
    baseContainer * const bC,                    
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG
	) {
		dtTransformer * dtTransformerP = NULL;
		
		//
		// get transformer element
		//
		::QDomElement wElement;
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
			
			if (hasLabel && inContainer && !hasName) {
				return bC->ptrTransformerContainer()->get(label)->clone();
			}
			if (hasLabel && inContainer && hasName) {
				return dtTransformerFactory::create("doNothing");
			}			
      
      if ( hasAttribute("transformerLibrary", wElement) ) {
				dtTransformerP 
				= 
        dtTransformerFactory::createFromPlugin( 
          getAttributeStr("name", wElement) ,
          getAttributeStr("transformerLibrary", wElement),
          getAttributeStr("transformerDriver", wElement)
        );
      }
      else {
				dtTransformerP 
				= 
        dtTransformerFactory::create( getAttributeStr("name", wElement) );
      }
			dtTransformerP->init(&wElement, bC, cV, aF, aG);	      
			
      if ( !inContainer && hasLabel) {	
        bC->ptrTransformerContainer()->add(dtTransformerP);
				delete dtTransformerP;
				dtTransformerP = dtTransformerFactory::create("doNothing");
			}
		}
		else dtTransformerP = dtTransformerFactory::create("doNothing");

    dt__throwIf( dtTransformerP==NULL, createTransformer() );
   
		return dtTransformerP;
  }

  dtTransformer * dtXmlParserBase::createTransformer(
	  ::QDomElement const * toBuildP,    
		baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF
	) {
		return createTransformer(toBuildP, bC, cV, aF, NULL);
  }
 
  dtReal dtXmlParserBase::createFloat(
    ::QDomElement const & toBuildP,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    lvH_analyticGeometry const * const aG
  ) {
    std::vector< dtReal > vec;
    createBasic(&toBuildP, bC, cV, aF, aG, &vec);
    dt__throwIf( vec.size()!=1, createAdvanced() );

    return vec[0];
  }
    
  void dtXmlParserBase::createBasic(
	  ::QDomElement const * toBuildP,
    baseContainer * const bC,                  
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG,
		std::vector< dtPoint3 > * basicP
	) {
    //
    // create transformer if necessary
    //
		::QDomElement wEl = *toBuildP;      
    ptrHandling<dtTransformer> dtTransformerP(
      createTransformer(toBuildP, bC, cV, aF, aG)
	  );
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }
    //
    //create points
    //
		vectorHandling< dtPoint3 > pp;		
    createBasic(
			&wEl, bC, cV, aF, aG, dtTransformerP.get(), &pp
		);		
		dt__forAllIndex(pp, ii) basicP->push_back( pp[ii] );
  }
	
  void dtXmlParserBase::createBasic(
	  ::QDomElement const * toBuildP,
    baseContainer * const bC,		
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG,
		dtTransformer const * const dtTransformerP, 
		std::vector< dtPointD > * basicP
	) {
    dt__throwIf(!is("Point_D", *toBuildP), createBasic());

		if ( hasAttribute("coordinates", *toBuildP) ) {
      std::vector< dtReal > cXYZ 
      = 
      muParseCSString(
        replaceDependencies( 
          getAttributeStr("coordinates", *toBuildP), bC, cV, aF, aG 
        )
      );
			basicP->push_back( dtPointD(cXYZ.size(), cXYZ.begin(), cXYZ.end()) );      
    }
		else if ( hasAttribute("attribute", *toBuildP) ) {
      std::string attribute = getAttributeStr("attribute", *toBuildP);      
      if ( attribute == "pick_order_from_file" ) {
        std::string filename = getAttributeStr("file_name", *toBuildP);
        std::vector< dtPointD > pD;
        if ( stringContains(".csv", filename) ) {
          pD = dtPointD_readCSV(filename).result();
          dt__forAllConstIter(std::vector< dtPointD >, pD, it) {
            basicP->push_back( *it );
          }
        }          
        else dt__throw(createBasic(), << "Unknown file extension.");
      }
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
    if (dtTransformerP != NULL) {
      if ( dtTransformerP->isNecessary() ) {
        dt__throw(
          createBasic(), << "Transformation of Point_D is not supported."
        );
      }
    }
  }
  
  void dtXmlParserBase::createBasic(
	  ::QDomElement const * toBuildP,
    baseContainer * const bC,		
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG,
		dtTransformer const * const dtTransformerP, 
		std::vector< dtPoint3 > * basicP
	) {
    dt__throwIf(!is("Point_3", *toBuildP), createBasic());

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
          pointName = label+"_"+intToString(ii+1);
					if ( bC->ptrPointContainer()->has( pointName ) ) {
						basicP->push_back( bC->ptrPointContainer()->get(pointName) );
					}
				}
				return;
			}
			else if ( hasAttribute("start_number|start_number", *toBuildP) ) {
				int startPoint = getAttributeInt("start_number", *toBuildP);
				int endPoint = getAttributeInt("end_number", *toBuildP);
				int nPoints = endPoint - startPoint + 1;
				for (int ii=0;ii<nPoints;ii++) {
					std::string pointName;
					pointName = label+"_"+intToString(startPoint);
					if ( bC->ptrPointContainer()->has( pointName ) ) {
						basicP->push_back( bC->ptrPointContainer()->get(pointName) );
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
          pointName = label+"_"+intToString(ii);
					if ( bC->ptrPointContainer()->has( pointName ) ) {
						basicP->push_back( 
						  dtPoint3(bC->ptrPointContainer()->get(pointName)) 
						);
					}
				}
				return;				
			}
			//
			// return a single point
			//
			else {
				if ( bC->ptrPointContainer()->has(label) ) {
					basicP->push_back( bC->ptrPointContainer()->get(label) );
				  return;
				}
			}
		}

    //
    // create simple xyz point with given coordinates
    //
		if ( hasAttribute("x|y|z", *toBuildP) ) {
      dtReal cX 
      = 
      muParseString(
        replaceDependencies( getAttributeStr("x", *toBuildP), bC, cV, aF, aG )
      );
      dtReal cY 
      = 
      muParseString(
        replaceDependencies( getAttributeStr("y", *toBuildP), bC, cV, aF, aG )
      );
      
      dtReal cZ 
      = 
      muParseString(
        replaceDependencies( getAttributeStr("z", *toBuildP), bC, cV, aF, aG )
      );
			basicP->push_back( dtPoint3(cX, cY, cZ) );      
    }      
		else if ( hasAttribute("xyz", *toBuildP) ) { 
      std::vector< dtReal > cXYZ 
      = 
      muParseCSString(
        replaceDependencies( getAttributeStr("xyz", *toBuildP), bC, cV, aF, aG )
      );
			basicP->push_back( dtPoint3(cXYZ[0], cXYZ[1], cXYZ[2]) );      
    }       
    else if ( hasAttribute("attribute|part_label", *toBuildP) ) {
      std::string attribute = getAttributeStr("attribute", *toBuildP);
      std::string partLabel = getAttributeStr("part_label", *toBuildP);
      
			analyticGeometry const * anAG 
			= 
			aG->get( partLabel );
			map1dTo3d const * m1d = map1dTo3d::ConstDownCast(anAG);
			map2dTo3d const * m2d = map2dTo3d::ConstDownCast(anAG);
			map3dTo3d const * m3d = map3dTo3d::ConstDownCast(anAG);

  		dt__throwIfWithMessage(
        !m1d&&!m2d&&!m3d,
				createBasic(),
				<< dt__eval( partLabel ) << std::endl
			  << "No such part or part is not of correct type"
      );     
	//
	// attribute: pick_from_part
	//            
			if ( attribute == "pick_from_part" ) {
        // 
        // 1 map2dTo3d
        // 				
				if (m2d) {
					analyticRotatingMap1dTo3d const * rS 
          = 
          analyticRotatingMap1dTo3d::ConstDownCast(m2d);
					//
					//analyticRotatingMap1dTo3d
					//
					if ( rS && hasAttribute("phi|z", *toBuildP) ) {
			      dtReal cX = getAttributeFloatMuParse("phi", *toBuildP, cV,  aF);
			      dtReal cY = getAttributeFloatMuParse("z", *toBuildP, cV,  aF);                 
						basicP->push_back( dtPoint3( rS->xyz_phiZ(cX, cY) ) );
					}
					else if (    
            hasAttribute(
              "parameter_one_percent|parameter_two_percent", *toBuildP
            )
          ) {
			      dtReal cX 
            = 
            getAttributeFloatMuParse(
              "parameter_one_percent", *toBuildP, cV,  aF
            );
			      dtReal cY 
            = 
            getAttributeFloatMuParse(
              "parameter_two_percent", *toBuildP, cV,  aF
            );       
						basicP->push_back( m2d->getPointPercent(cX, cY) );
					}
				}
		// 
		// 2 map1dTo3d
		//         
				else if (m1d && hasAttribute("parameter_one_percent", *toBuildP) ) {
          dtReal cX 
          = 
          getAttributeFloatMuParse(
            "parameter_one_percent", *toBuildP, cV,  aF
          );
					basicP->push_back( m1d->getPointPercent(cX) );
				}
		// 
		// 3 map3dTo3d
		//         
				else if ( m3d ) {
					if (    
            hasAttribute(
              "parameter_one_percent|parameter_two_percent"
              "|parameter_three_percent", 
              *toBuildP
            ) 
          ) {
			      dtReal cX 
            = 
            getAttributeFloatMuParse(
              "parameter_one_percent", *toBuildP, cV,  aF
            );
			      dtReal cY 
            = 
            getAttributeFloatMuParse(
              "parameter_two_percent", *toBuildP, cV,  aF
            );       
			      dtReal cZ 
            = 
            getAttributeFloatMuParse(
              "parameter_three_percent", *toBuildP, cV,  aF
            );            
						basicP->push_back( m3d->getPointPercent(cX, cY, cZ) );
					}        
					else if (    
            hasAttribute(
              "parameter_one|parameter_two|parameter_three", *toBuildP
            ) 
          ) {
			      dtReal cX 
            = 
            getAttributeFloatMuParse("parameter_one", *toBuildP, cV,  aF);
			      dtReal cY 
            = 
            getAttributeFloatMuParse("parameter_two", *toBuildP, cV,  aF);       
			      dtReal cZ 
            = 
            getAttributeFloatMuParse("parameter_three", *toBuildP, cV,  aF);       
						basicP->push_back( m3d->getPoint(cX, cY, cZ) );
					} 					
				}      
			}
	//
	// attribute: pick_order_from_part
	//      
			else if ( attribute == "pick_order_from_part" ) {
				//
				// 1 map1dTo3d
				//        
				if ( m1d && hasAttribute("number_points_one", *toBuildP) ) {
					int nPointsOne 
          = 
          getAttributeIntMuParse("number_points_one", *toBuildP, cV);					       					
					if ( hasAttribute("parameter_one_percent_function", *toBuildP) ) {
						dt__ptrAss(
							scaOneD const * sF,
							scaOneD::ConstDownCast(
								aF->get( 
									getAttributeStr("parameter_one_percent_function", *toBuildP)
								)
							)
						);
            std::vector< dtReal > ff 
            = 
            float_scaOneDPoint(sF, nPointsOne).result();
            dt__forAllConstIter(std::vector< dtReal >, ff, it) {
							basicP->push_back(m1d->getPointPercent(*it));              
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
						dtReal length = m1d->length();
						dtReal uP;
            std::vector< dtReal > ff 
            = 
            float_scaOneDPoint(sF, nPointsOne).result();
            dt__forAllConstIter(std::vector< dtReal >, ff, it) {            
							uP = m1d->u_l((*it)*length);
              basicP->push_back( m1d->getPoint(uP) );              					
						}
					}
				}				
				//
				// 1 map2dTo3d
				//        
				else if ( 
          m2d
					&& 
          hasAttribute(
            "parameter_one_percent_function|parameter_two_percent_function"
            "|number_points_one|number_points_two", 
            *toBuildP
          ) 
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
							dt__toFloat(dtReal iiF, ii);
							dt__toFloat(dtReal jjF, jj);
							dtReal paraOne = fOne->YFloat( iiF / nPointsOne ) ;
							dtReal paraTwo = fTwo->YFloat( jjF / nPointsTwo ) ;
							basicP->push_back( 
							  dtPoint3( m2d->getPointPercent( paraOne, paraTwo ) )
							);              
						}
					}
				}                    
			}
		}
		/* ---------------------------------------------------------------------- */
		/* pick point from a function */
		/* ---------------------------------------------------------------------- */          
		else if ( hasAttribute("attribute|function_label", *toBuildP) ) {
      std::string attribute = getAttributeStr("attribute", *toBuildP);
      std::string functionLabel = getAttributeStr("function_label", *toBuildP);      
			analyticFunction const * anAF
			= 
			aF->get( functionLabel );
			vec3dOneD const * v1D = vec3dOneD::ConstDownCast(anAF);
			vec3dTwoD const * v2D = vec3dTwoD::ConstDownCast(anAF);

			dt__throwIfWithMessage(
				!v1D&&!v2D,
				createBasic(),
				<< dt__eval(functionLabel) << std::endl
				<< "No such analyticFunction or analyticFunction has wrong type."
			);

			if ( attribute == "pick_from_function" ) {
				if ( v1D && hasAttribute("x_one", *toBuildP) ) {
			    dtReal cX = getAttributeFloatMuParse("x_one", *toBuildP, cV,  aF);
					basicP->push_back( v1D->YdtPoint3(cX) );
				}
				else if ( v1D && hasAttribute("x_one_percent", *toBuildP) ) {
			    dtReal cX 
          = 
          getAttributeFloatMuParse("x_one_percent", *toBuildP, cV,  aF);
					basicP->push_back( v1D->YdtPoint3Percent(cX) );
				}				
				else if ( v2D && hasAttribute("x_one|x_two", *toBuildP) ) {
			    dtReal cX = getAttributeFloatMuParse("x_one", *toBuildP, cV,  aF);
			    dtReal cY = getAttributeFloatMuParse("x_two", *toBuildP, cV,  aF);          
					basicP->push_back( v2D->YdtPoint3(cX, cY) );
				}	
				else if (
				  v2D && hasAttribute("x_one_percent|x_two_percent", *toBuildP)
				) {
			    dtReal cX 
          = 
          getAttributeFloatMuParse("x_one_percent", *toBuildP, cV,  aF);
			    dtReal cY 
          = 
          getAttributeFloatMuParse("x_two_percent", *toBuildP, cV,  aF);          
					basicP->push_back( v2D->YdtPoint3Percent(cX, cY) );
				}				
			}			
		}
		else if ( hasAttribute("attribute", *toBuildP) ) {
      std::string attribute = getAttributeStr("attribute", *toBuildP);      
      if ( attribute == "pick_order_from_file" ) {
        std::string filename = getAttributeStr("file_name", *toBuildP);
        if ( filename != "" ) {
          std::vector< dtPoint3 > p3;
          if ( stringContains(".ibl", filename) ) {
            p3 = dtPoint3_readIBL(filename).result();
            dt__forAllConstIter(std::vector< dtPoint3 >, p3, it) {
              basicP->push_back( *it );
            }
          }
          else if ( stringContains(".csv", filename) ) {
            p3 = dtPoint3_readCSV(filename).result();
            dt__forAllConstIter(std::vector< dtPoint3 >, p3, it) {
              basicP->push_back( *it );
            }
          }          
          else dt__throw(createBasic(), << "Unknown file extension.");
        }
        else dt__throw(createBasic(), << "attribute file_name is empty.");
      }
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
    if (dtTransformerP != NULL) {
      if ( dtTransformerP->isNecessary() ) {
        std::vector< dtPoint3 > basicPTwin;
        for (int ii=0;ii<basicP->size();ii++) {
          basicPTwin.push_back( basicP->at(ii) );
        }
        basicP->clear();
        *basicP = dtTransformerP->apply( &basicPTwin );
        basicPTwin.clear();
      }
    }
    
    //
    // store labeled points
    //
		if ( hasAttribute("label", *toBuildP) ) {
			std::string label = getAttributeStr("label", *toBuildP);    
      if ( basicP->size() > 1 ) {
        dt__forAllIndex(*basicP, ii) {
          bC->ptrPointContainer()->add( 
            basicP->at(ii) , label+"_"+intToString(ii+1) 
          );
        }
      }
      else bC->ptrPointContainer()->add(basicP->at(0), label);
    }
  }

	/**
	 * 
   * @todo Function does not store two dimensional vector in container.
   */
  dtVector2 dtXmlParserBase::createDtVector2(
	  ::QDomElement const * toBuildP,
		baseContainer * const bC,
		lvH_constValue const * const cV,  
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG
	) {  
		dtVector2 vv;

    if ( !is("Vector_2", *toBuildP) && !is("dtVector2", *toBuildP) ) {
      dt__throw(createBasic(), << dt__eval( getTagName(*toBuildP) ) );
    }
    else {
      //
      // check for attribute x, y and z
      //
      if ( toBuildP->hasAttribute("x") && toBuildP->hasAttribute("y") ) { 
        //
        // create coordinates
        //
        dtReal cX 
        = 
        muParseString(
          replaceDependencies(
            getAttributeStr("x", *toBuildP),
            cV, 
            aF
          )
        );  
        dtReal cY 
        = 
        muParseString(
          replaceDependencies(
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
        dtReal cX 
        =
        muParseString(
          replaceDependencies(
            getAttributeStr("u", *toBuildP),
            cV, 
            aF
          )
        );  
        dtReal cY 
        = 
        muParseString(
          replaceDependencies(
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
	  ::QDomElement const * toBuildP,
		baseContainer * const bC,
		lvH_constValue const * const cV,  
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG
	) {
		dtVector3 vv;

    if ( !is("Vector_3", *toBuildP) ) {
      dt__throw(
        createDtVector3(),
        << dt__eval(qPrintable( toBuildP->tagName() ) ) 
      );
    }
    else {
      //
      // check if vector has a label and point is already created
      // return point and that's it
      //
      if ( toBuildP->hasAttribute("label") ) {
        if ( 
          bC->ptrVectorContainer()->has(getAttributeStr("label", *toBuildP)) 
        ) {
          vv 
          = 
          bC->ptrVectorContainer()->get( getAttributeStr("label", *toBuildP) );
          return vv;
        }
      }
      //
      // check for attribute x, y and z
      //
      if ( 
           toBuildP->hasAttribute("x") 
        && toBuildP->hasAttribute("y") 
        && toBuildP->hasAttribute("z") 
      ) {
        //
        // create coordinates
        //
        dtReal cX 
        = 
        muParseString(
          replaceDependencies(
            getAttributeStr("x", *toBuildP),
            cV, 
            aF
          )
        );  
        dtReal cY 
        = 
        muParseString(
          replaceDependencies(
            getAttributeStr("y", *toBuildP),
            cV, 
            aF
          )
        );
        dtReal cZ 
        = 
        muParseString(
          replaceDependencies(
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
      else if ( hasAttribute("xyz", *toBuildP) ) { 
        std::vector< dtReal > cXYZ 
        = 
        muParseCSString(
          replaceDependencies( 
            getAttributeStr("xyz", *toBuildP), bC, cV, aF, aG 
          )
        );
        vv = dtVector3(cXYZ[0], cXYZ[1], cXYZ[2]);      
      }      
      //
      //check if point has a label and add it to the container
      //
      if ( hasAttribute("label", *toBuildP) ) {
        bC->ptrVectorContainer()->add( 
          vv, getAttributeStr("label", *toBuildP) 
        );
      }
      //
      //output
      //
      dt__debug(
        createDtVector3(),
        << "created vector: (" 
        << vv.x() << " / "
        << vv.y() << " / "
        << vv.z() << ")"
      );
			
			return vv;
    }
  }

  dtVector3 dtXmlParserBase::createDtVector3(
	  ::QDomElement const * toBuildP,
		baseContainer * const bC,
		lvH_constValue const * const cV,  
		lvH_analyticFunction const * const aF
	) {  
    return createDtVector3(toBuildP, bC, cV, aF, NULL); 
  }	

	dtVector2 dtXmlParserBase::getDtVector2(
		::QDomElement const * toBuildP,
		baseContainer const * const bC,
		lvH_constValue const * const cV,  
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG
	) {
		return createDtVector2(toBuildP, NULL, cV, aF, aG);
	} 
	
	dtPoint3 dtXmlParserBase::getDtPoint3(
		::QDomElement const * toBuildP,
		baseContainer const * const bC,
		lvH_constValue const * const cV,  
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG
	) {
		if ( hasAttribute("label", *toBuildP) ) {
			return bC->constPtrPointContainer()->get(
        getAttributeStr("label", *toBuildP)
      );
		}
		else return createDtPoint3(toBuildP, NULL, cV, aF, aG);
	} 
	
	dtPoint3 dtXmlParserBase::getDtPoint3(
		::QDomElement const * toBuildP,
		baseContainer const * const bC,
		lvH_constValue const * const cV,  
		lvH_analyticFunction const * const aF
	) {
		if ( hasAttribute("label", *toBuildP) ) {
			return bC->constPtrPointContainer()->get(
        getAttributeStr("label", *toBuildP)
      );
		}
		else return createDtPoint3(toBuildP, NULL, cV, aF);
	}
		
	dtVector3 dtXmlParserBase::getDtVector3(
		::QDomElement const * toBuildP,
		baseContainer const * const bC,
		lvH_constValue const * const cV,  
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG
	) {
		if ( hasAttribute("label", *toBuildP) ) {
			return bC->constPtrVectorContainer()->get(
        getAttributeStr("label", *toBuildP)
      );
		}
		else return createDtVector3(toBuildP, NULL, cV, aF, aG);
	} 
	
	dtVector3 dtXmlParserBase::getDtVector3(
		::QDomElement const * toBuildP,
		baseContainer const * const bC,
		lvH_constValue const * const cV,  
		lvH_analyticFunction const * const aF
	) {
		if ( hasAttribute("label", *toBuildP) ) {
			return bC->constPtrVectorContainer()->get(
        getAttributeStr("label", *toBuildP)
      );
		}
		else return createDtVector3(toBuildP, NULL, cV, aF);
	}
	
  dtVector3 dtXmlParserBase::getDtVector3(
	  std::string const & str, baseContainer const * const bC
	) {
		return bC->constPtrVectorContainer()->get(str);
	}

  dtVector3 dtXmlParserBase::getDtVector3(
	  ::QDomElement const & toBuildP, baseContainer const * const bC
	) {
		return getDtVector3(getAttributeStr("label", toBuildP), bC);
	}
	
  dtPoint3 dtXmlParserBase::getDtPoint3(
	  std::string const & str, baseContainer const * const bC
	) {
		return bC->constPtrPointContainer()->get(str);
	}

  dtPoint3 dtXmlParserBase::getDtPoint3(
	  ::QDomElement const & toBuildP, baseContainer const * const bC
	) {
		return getDtPoint3(getAttributeStr("label", toBuildP), bC);
	}	

  std::string dtXmlParserBase::replaceDependencies( 
	  std::string const expression, 
		lvH_constValue const * const cV
	) {
    return dtParser(NULL, cV, NULL, NULL, NULL, NULL)[ expression ];
  }
  
  std::string dtXmlParserBase::replaceDependencies( 
	  std::string const expression, 
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF
	) {
    return dtParser(NULL, cV, aF, NULL, NULL, NULL)[ expression ];
  }
  
  std::string dtXmlParserBase::replaceDependencies( 
	  std::string const expression, 
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG
	) {
    return dtParser(NULL, cV, aF, aG, NULL, NULL)[ expression ];
  }
  
  std::string dtXmlParserBase::replaceDependencies( 
	  std::string const expression, 
    baseContainer const * const bC,       
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG
	) {
    return dtParser(bC, cV, aF, aG, NULL, NULL)[ expression ];
  } 
  
  std::string dtXmlParserBase::replaceDependenciesRef(
    std::string const expression, 
    baseContainer const & bC,      
    lvH_constValue const & cV,
    lvH_analyticFunction const & aF,
    lvH_analyticGeometry const & aG
  ) {
    return dtXmlParserBase::replaceDependencies( 
      expression, &bC, &cV, &aF, &aG 
    );
  }

  dtReal dtXmlParserBase::getAttributeFloatMuParse( 
    std::string const attName, 
    ::QDomElement const element, 
    lvH_constValue const * const cV
  ) {
    return muParseString( 
      replaceDependencies( getAttributeStr(attName, element), cV )
    );
  }
  
  dtReal dtXmlParserBase::getAttributeFloatMuParse(
    std::string const attName, 
    ::QDomElement const element, 
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF
  ) {
    return muParseString( 
      replaceDependencies( getAttributeStr(attName, element), cV, aF )
    );
  }
  
  dtReal dtXmlParserBase::getAttributeFloatMuParse(
    std::string const attName, 
    ::QDomElement const element, 
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    dtReal const & def
  ) {
    if ( hasAttribute(attName, element) ) {
      return getAttributeFloatMuParse( attName, element, cV, aF );
    }
    else return def;
  }    

  dtInt dtXmlParserBase::getAttributeIntMuParse(
    std::string const attName, 
    ::QDomElement const element, 
    lvH_constValue const * const cV
  ) {
    return muParseStringInt( 
      replaceDependencies( getAttributeStr(attName, element), cV )
    );
  }  
  
  dtInt dtXmlParserBase::getAttributeIntMuParse(
    std::string const attName, 
    ::QDomElement const element, 
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF 
  ) {
    return muParseStringInt( 
      replaceDependencies( getAttributeStr(attName, element), cV, aF )
    );
  }

  dtInt dtXmlParserBase::getAttributeIntMuParse(
    std::string const attName, 
    ::QDomElement const element, 
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG 
  ) {
    return muParseStringInt( 
      replaceDependencies( getAttributeStr(attName, element), cV, aF, aG )
    );
  }
  
  dtInt dtXmlParserBase::getAttributeIntMuParse(
    std::string const attName, 
    ::QDomElement const element, 
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    dtInt const & def
  ) {
    if ( hasAttribute(attName, element) ) {
      return getAttributeIntMuParse( attName, element, cV, aF );
    }
    else return def;
  }  
  
	std::vector< dtReal > dtXmlParserBase::getAttributeFloatVectorMuParse( 
		std::string const attName, 
		::QDomElement const element, 
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF 
	) {
		std::string att = getAttributeStr(attName, element);
		std::vector< std::string > attVec = convertToStringVector("{", "}", att);
		std::vector< dtReal > floatVec(attVec.size(), 0.);
		int counter = 0;
		for ( auto &el : attVec ) {		
			floatVec[counter] = muParseString( replaceDependencies(el, cV, aF) );
			counter++;
		}		
		return floatVec;
	}

	std::vector< double > dtXmlParserBase::getAttributeDoubleVectorMuParse( 
		std::string const attName, 
		::QDomElement const element, 
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF 
	) {
		std::vector< dtReal > floatVec
    =
    getAttributeFloatVectorMuParse(attName, element, cV, aF);
    
    return std::vector< double >( floatVec.begin(), floatVec.end() );
	}  

	std::vector< dtInt > dtXmlParserBase::getAttributeIntVectorMuParse( 
		std::string const attName, 
		::QDomElement const element, 
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF 
	) {
		std::string att = getAttributeStr(attName, element);
		std::vector< std::string > attVec = convertToStringVector("{", "}", att);
		std::vector< dtInt > intVec(attVec.size(), 0.);
		int counter = 0;
		for ( auto &el : attVec ) {		
			intVec[counter] = muParseStringInt( replaceDependencies(el, cV, aF) );
			counter++;
		}		
		return intVec;
	}
  
	std::vector< dtInt > dtXmlParserBase::getAttributeIntVectorMuParse( 
		std::string const attName, 
		::QDomElement const element, 
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG      
	) {
		std::string att = getAttributeStr(attName, element);
		std::vector< std::string > attVec = convertToStringVector("{", "}", att);
		std::vector< dtInt > intVec(attVec.size(), 0.);
		int counter = 0;
		for ( auto &el : attVec ) {		
			intVec[counter] = muParseStringInt( replaceDependencies(el, cV, aF, aG) );
			counter++;
		}		
		return intVec;
	}
  
  dtReal dtXmlParserBase::getAttributeFloatMuParse( 
    std::string const attName, 
    ::QDomElement const element, 
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG  
  ) {
    return muParseString( 
      replaceDependencies( getAttributeStr(attName, element), cV, aF, aG )
    );
  } 
  
  dtPoint3 dtXmlParserBase::createDtPoint3(
	  ::QDomElement const * toBuildP,
    baseContainer * const bC,                   
    lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG
  ) {
    std::vector< dtPoint3 > basicVec;

    createBasic(toBuildP, bC, cV, aF, aG,&basicVec);
    
    dt__throwIf(basicVec.size()!=1, createBasic());
	
    return basicVec[0];
	}
  
  dtPoint3 dtXmlParserBase::createDtPoint3(
	  ::QDomElement const & toBuild,
    baseContainer * const bC,                   
    lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG
  ) {
    return createDtPoint3(&toBuild, bC, cV, aF, aG);
	}  
	
  dtPoint3 dtXmlParserBase::createDtPoint3(
	  ::QDomElement const * toBuildP,
		baseContainer * const bC,
		lvH_constValue const * const cV,  
		lvH_analyticFunction const * const aF
	) {  
    return createDtPoint3(toBuildP, bC, cV, aF, NULL); 
  }	
	
  dtPoint2 dtXmlParserBase::createDtPoint2(
	  ::QDomElement const * toBuildP,
    baseContainer * const bC,                   
    lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG
  ) {
    std::vector< dtPoint2 > basicVec;

    createBasic(toBuildP, bC, cV, aF, aG,&basicVec);
    
    dt__throwIf(basicVec.size()!=1, createBasic());
	
    return basicVec[0];
	}
	
  dtPoint2 dtXmlParserBase::createDtPoint2(
	  ::QDomElement const * toBuildP,
		baseContainer * const bC,
		lvH_constValue const * const cV,  
		lvH_analyticFunction const * const aF
	) {  
    return createDtPoint2(toBuildP, bC, cV, aF, NULL); 
  }	
  
  void dtXmlParserBase::createBasic(
	  ::QDomElement const * toBuildP, 
    baseContainer * const bC,      					
		lvH_constValue const * const cV, 
		lvH_analyticFunction const * const aF, 
		std::vector< dtPoint2 > * basicP
	) {
    ptrHandling< dtTransformer > dtT(
		  createTransformer(toBuildP, bC, cV, aF)
		);		
		::QDomElement wEl = *toBuildP;
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }		
		std::vector< dtPoint2 > pp;
    createBasic(&wEl, NULL, cV, aF, NULL, dtT.get(), &pp);      		
		dt__forAllIndex(pp, ii) basicP->push_back( pp[ii] );
  }

  void dtXmlParserBase::createBasic(
	  ::QDomElement const * toBuildP,
    baseContainer * const bC,					
		lvH_constValue const * const cV, 
		lvH_analyticFunction const * const aF, 
		std::vector< dtPointD > * basicP
	) {
    ptrHandling< dtTransformer > dtT(
		  createTransformer(toBuildP, bC, cV, aF)
		);		
		::QDomElement wEl = *toBuildP;
    if ( is("transformer", *toBuildP) ) wEl = toBuildP->nextSiblingElement();
    
    createBasic(&wEl, bC, cV, aF, NULL, dtT.get(), basicP);      		
  }	
  
  void dtXmlParserBase::createBasic(
	  ::QDomElement const * toBuildP,
    baseContainer * const bC,					
		lvH_constValue const * const cV, 
		lvH_analyticFunction const * const aF, 
		std::vector< dtPoint3 > * basicP
	) {
		::QDomElement wEl = *toBuildP;
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }		
    createBasic(&wEl, bC, cV, aF, NULL, basicP);      		
  }	

  void dtXmlParserBase::createBasic(
	  ::QDomElement const * toBuildP,
    baseContainer * const bC,            
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG,
		dtTransformer const * const dtTransformerP, 
		std::vector< dtPoint2 > * basicP
	) {
    dt__throwIf(!is("Point_2", *toBuildP), createBasic());

    /* ---------------------------------------------------------------------- */
    /* create normal x-y-z-point */
    /* ---------------------------------------------------------------------- */
    if ( hasAttribute("x|y", *toBuildP) ) {
      //
      // create coordinates
      //
      dtReal cX = getAttributeFloatMuParse("x", *toBuildP, cV, aF, aG);
      dtReal cY = getAttributeFloatMuParse("y", *toBuildP, cV, aF, aG);
      //
      // create point
      //
      basicP->push_back( dtPoint2(cX, cY) );
    }
		else if ( hasAttribute("xy", *toBuildP) ) { 
      std::vector< dtReal > cXY 
      = 
      muParseCSString(
        replaceDependencies( getAttributeStr("xy", *toBuildP), bC, cV, aF, aG )
      );
			basicP->push_back( dtPoint2(cXY[0], cXY[1]) );      
    }     
    /* ---------------------------------------------------------------------- */
    /* pick point from a part */
    /* ---------------------------------------------------------------------- */          
    else if ( 
         hasAttribute("attribute", *toBuildP) 
      && hasAttribute("part_label", *toBuildP) 
    ) {
      //
      //look for part to pick point from
      //
      analyticSurface const * analyticSurface_partToPickFromP = NULL;
      analyticGeometry const * anAG 
      = 
      aG->get( getAttributeStr("part_label", *toBuildP) );
      analyticSurface_partToPickFromP 
      = 
      analyticSurface::ConstDownCast(anAG);
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
          if (
               analyticSurface_partToPickFromP 
            && toBuildP->hasAttribute("parameter_one_percent")
            && toBuildP->hasAttribute("parameter_two_percent") 
          ) {
            //
            // create coordinates
            //
            dtReal pX 
            = 
            muParseString(
              replaceDependencies(
                getAttributeStr("parameter_one_percent", *toBuildP),
                cV, 
                aF
              )
            );  
            dtReal pY 
            = 
            muParseString(
              replaceDependencies(
                getAttributeStr("parameter_two_percent", *toBuildP),
                cV, 
                aF
              )
            );              
            //
            // ask analyticSurface for a point
            //
            basicP->push_back( 
              analyticSurface_partToPickFromP->uv_percent(dtPoint2(pX, pY))
            );
          }
          else dt__throwUnexpected(createBasic());      
        }
        else dt__throwUnexpected(createBasic());
      }    
    }
    else if ( 
         hasAttribute("attribute", *toBuildP) 
      && hasAttribute("function_label", *toBuildP) 
    ) {
      //
      // look for function to pick point from
      //
			dt__ptrAss(
				vec2dOneD const * const theF,
				vec2dOneD::ConstDownCast(
					aF->get( getAttributeStr("function_label", *toBuildP) )
				)
			);

      if ( 
        getAttributeStr("attribute", *toBuildP) == "pick_order_from_function" 
      ) {
        //
        // 1 analyticFunction
        //        
        if (
             theF 
          && hasAttribute("parameter_percent_function", *toBuildP)
          && hasAttribute("number_points", *toBuildP) 
        ) {
          dtInt nPoints
				  = 
					getAttributeIntMuParse("number_points", *toBuildP, cV, aF);
          
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
            dt__toFloat(dtReal iiF, ii);
						dt__toFloat(dtReal nPointsF, nPoints);
            // get value of scaFunction
            // pick points out of range
            basicP->push_back( 
              theF->YdtPoint2Percent(
                thePara->YFloat( iiF / (nPointsF-1.) )
              )            
            );
          }
        }
        else dt__throwUnexpected(createBasic());
      }
    }
		else if ( 
      hasAttribute("attribute", *toBuildP) 
      &&
      (getAttributeStr("attribute", *toBuildP) == "pick_order_from_file") 
    ) {       
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

				for ( dtInt ii=0; ii<fields.size(); ii++ ) {
					if ( fields[ii].size() != 2 ) {
						dt__throw(
              createBasic(),
							<< "field[" << ii << "] of read point has not a size of 2." 
              << std::endl
							<< "file_name = " << getAttributeStr("file_name", *toBuildP)
            );
					}
					basicP->push_back( dtPoint2(fields[ii][0], fields[ii][1]) );          
				}
				//
				// close file
				//
				in.close();
			}
			else dt__throw(createBasic(), << "Attribute file_name is empty");
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
      vectorHandling< dtPoint2 > basicPTwin;
      for (int ii=0;ii<basicP->size();ii++) {
        basicPTwin.push_back( basicP->at(ii) );
      }
      basicP->clear();
      *basicP = dtTransformerP->apply( &basicPTwin );
    }

    //
    // point output
    //
    for (int ii=0;ii<basicP->size();ii++) {
      dt__debug(
			  createBasic(),
        << "created point:" << dt__point2d( (basicP->at(ii)) )
			);
    }    

  }

  void dtXmlParserBase::createBasic(
    ::QDomElement const * toBuildP,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    lvH_analyticGeometry const * const aG,
    std::vector< dtReal > * basicP
  ) {
    dt__throwIf(!is("float", *toBuildP), createBasic());    
    
    if ( hasAttribute("value", *toBuildP) ) {
      dtReal val = getAttributeFloatMuParse("value", *toBuildP, cV, aF, aG);
      basicP->push_back( val );
    }
    else dt__throwUnexpected(createBasic());
  }
    
  void dtXmlParserBase::createBasic(
	  ::QDomElement const * toBuildP,
		baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF, 
		lvH_analyticGeometry const * const aG,
		std::vector< dtPoint2 > * basicP
	) {
    //
    // create transformer if necessary
    //
    ptrHandling< dtTransformer > dtT(
		  createTransformer(toBuildP, bC, cV, aF, aG)
		);
		::QDomElement wEl = *toBuildP;
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }
		vectorHandling< dtPoint2 > pp;
    createBasic(&wEl, bC, cV, aF, aG, dtT.get(), &pp);
		dt__forAllIndex(pp, ii) basicP->push_back( pp[ii] );		
  }

  void dtXmlParserBase::createAdvanced( 
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    lvH_analyticGeometry const * const aG,            
    lvH_analyticGeometry * advancedP 
  ) {
    createAdvanced( &toBuild, bC, cV, aF, aG, advancedP );
  }  
  
  void dtXmlParserBase::createAdvanced( 
    ::QDomElement const * toBuildP,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    lvH_analyticGeometry const * const aG,            
    lvH_analyticGeometry * advancedP 
  ) {

    //
    // create transformer if necessary
    //
    ptrHandling< dtTransformer > dtT(
      createTransformer(toBuildP, bC, cV, aF, aG)
    );
    ::QDomElement wEl = *toBuildP;
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
    ::QDomElement const * toBuildP,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    lvH_analyticGeometry const * const aG, 
    dtTransformer const * const dtTransformerP,                          
    lvH_analyticGeometry * advancedP 
  ) {
    //
    // get label
    //
    std::string label = getAttributeStr("label", *toBuildP);

    if ( qtXmlBase::isWildcard(label) ) {
      //
      // string contains "*" --> return set of analyticGeometries
      //				
      for (int ii=0;ii<aG->size();ii++) { 
        if ( qtXmlBase::matchWildcard(label, aG->at(ii)->getLabel()) ) {
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
        dt__throwIf( bC==NULL, createAdvanced() );
        dtTransformer const * const dtT 
        = 
        bC->ptrTransformerContainer()->get(transLabels[ii]);
        toTrans.reset(dtT->apply( toTrans.get() ));
      }
      advancedP->push_back( toTrans->clone() );
    }
    else if ( stringContains("{", label) && stringContains("}", label) ) {
      //
      // string contains "{" and "}" --> return set of analyticGeometries
      //				
      std::vector< std::string > labelV
      = 
      convertToStringVector("{", "}", label );
      dt__forAllIndex(labelV, ii) advancedP->push_back( aG->get(labelV[ii]) );
    }
    else advancedP->push_back( aG->get( label )->clone() );

    dt__throwIfWithMessage(
      advancedP->size()==0, createAdvanced(), << "Cannot find part " << label 
    );

    //
    // copy
    //
    lvH_analyticGeometry advancedTwin;
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
    ::QDomElement const * toBuildP,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    lvH_analyticGeometry const * const aG
  ) {
    lvH_analyticGeometry advancedVec;
    createAdvanced(toBuildP, bC, cV, aF, aG, &advancedVec);
    if (advancedVec.size() != 1) {
      dt__throw(createAdvanced(), << dt__eval( advancedVec.size() ) );
    }
    return advancedVec[0];
  }
		
  analyticGeometry * dtXmlParserBase::createAnalyticGeometry( 
    ::QDomElement const & toBuildP,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    lvH_analyticGeometry const * const aG
  ) {
    return createAnalyticGeometry(&toBuildP, bC, cV, aF, aG);
  } 
  
  analyticGeometry * dtXmlParserBase::createAnalyticGeometry( 
    ::QDomElement const * toBuildP,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    lvH_analyticGeometry const * const aG
  ) {
    return createAnalyticGeometry(toBuildP, NULL, cV, aF, aG);			
  }		
  
  analyticGeometry * dtXmlParserBase::createAnalyticGeometry( 
    ::QDomElement const & toBuildP,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    lvH_analyticGeometry const * const aG
  ) {
    return createAnalyticGeometry(&toBuildP, NULL, cV, aF, aG);			
  }		
  
  void dtXmlParserBase::createAdvanced( 
    ::QDomElement const * toBuildP,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    dtTransformer const * const dtTransformerP,                          
    lvH_analyticFunction * advancedP 
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
    else if ( stringContains("{", label) && stringContains("}", label) ) {
      //
      // string contains "{" and "}" --> return set of analyticFunctions
      //				
      std::vector< std::string > labelV
      = 
      convertToStringVector("{", "}", label );
      dt__forAllIndex(labelV, ii) advancedP->push_back( aF->get(labelV[ii]) );
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
    lvH_analyticFunction advancedTwin;
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
    ::QDomElement const * toBuildP,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    lvH_analyticFunction * advancedP 
  ) {
    //
    // create transformer if necessary
    //
    ptrHandling< dtTransformer > dtTransformerP(
      createTransformer(toBuildP, bC, cV, aF)
    );
    ::QDomElement wEl = *toBuildP;
    if ( is("transformer", *toBuildP) ) {
      wEl = toBuildP->nextSiblingElement();
    }
    createAdvanced(&wEl, bC, cV, aF, dtTransformerP.get(), advancedP);      
  }
    
  void dtXmlParserBase::createAdvanced( 
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF, 
    lvH_analyticFunction * advancedP 
  ) {
    createAdvanced( &toBuild, bC, cV, aF, advancedP );
  }  
  
  analyticFunction * dtXmlParserBase::createAnalyticFunction( 
    ::QDomElement const * toBuildP,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF 
  ) {
    lvH_analyticFunction advancedVec;
    createAdvanced(toBuildP, bC, cV, aF, &advancedVec);
    
    dt__throwIf(advancedVec.size()!=1, createAdvanced());
    return advancedVec[0];
  }

  analyticFunction * dtXmlParserBase::createAnalyticFunction( 
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF 
  ) {
    return createAnalyticFunction(&toBuild, bC, cV, aF);
  }  
}
