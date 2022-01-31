#include "feOpenFOAMSetupRule.h"

#include <logMe/logMe.h>
#include <xmlHeaven/qtXmlBase.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <volFields.H>
#include <fixedValueFvPatchField.H>
#include <zeroGradientFvPatchField.H>
#include <epsilonWallFunctionFvPatchScalarField.H>
#include <kqRWallFunctionFvPatchField.H>
#include <nutWallFunctionFvPatchScalarField.H>
#include <turbulentIntensityKineticEnergyInletFvPatchScalarField.H>
#include <turbulentMixingLengthDissipationRateInletFvPatchScalarField.H>
#include <inletOutletFvPatchField.H>
#include <rotatingWallVelocityFvPatchVectorField.H>
#include <flowRateInletVelocityFvPatchVectorField.H>
#include <slipFvPatchField.H>
#include <movingWallVelocityFvPatchVectorField.H>
#include <dictionary.H>
#include <boost/assign.hpp>

namespace dtOO {
  dt__pVH(feOpenFOAMSetupRule) feOpenFOAMSetupRule::_rules;
    
  bool feOpenFOAMSetupRule::_registrated 
  =
  feOpenFOAMSetupRule::registrate(
    dt__tmpPtr(feOpenFOAMSetupRule, new feOpenFOAMSetupRule())
  );
  
  feOpenFOAMSetupRule::feOpenFOAMSetupRule() {
  }

  feOpenFOAMSetupRule::~feOpenFOAMSetupRule() {
  }
  
  std::vector< std::string > feOpenFOAMSetupRule::factoryAlias( void ) const {
    return ::boost::assign::list_of("OpenFOAMSetupRule");
  }  
  
  bool feOpenFOAMSetupRule::registrate( 
    feOpenFOAMSetupRule const * const rule 
  ) {
    dt__forAllRefAuto( _rules, aRule ) {
      if ( aRule.virtualClassName() == rule->virtualClassName() ) {
        return false;
      }
    }  
    _rules.push_back( rule->create() );
    
    return true;
  }  
  
  feOpenFOAMSetupRule * feOpenFOAMSetupRule::create( 
    std::string const & str 
  ) {
    dt__info( create(), << "str = " << str);
    dt__forAllRefAuto( _rules, aRule ) {
      //
      // check virtual class name
      //
      if ( aRule.virtualClassName() == str ) {
        return aRule.create();
      }
            
      //
      // check alias
      //
      dt__forAllRefAuto(aRule.factoryAlias(), anAlias) {
        if ( anAlias == str ) return aRule.create();
      }
    }
    
    std::vector< std::string > av;
    dt__forAllRefAuto( _rules, aRule ) {
      av.push_back( aRule.virtualClassName() );
      dt__forAllRefAuto(aRule.factoryAlias(), anAlias) {
        av.push_back("  -> "+anAlias);
      }      
    }
    dt__throw(
      create(), 
      << str <<  " could not be created." << std::endl
      << "Implemented rules:" << std::endl
      << logMe::vecToString(av,1) << std::endl
    );   
  }

  void feOpenFOAMSetupRule::init(
    baseContainer const * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
    dCPtrVec const * const dC
  ) {
    _bC = bC;
    _cV = cV;
    _aF = aF;
    _aG = aG;
    _bV = bV;
    _dC = dC;
  }
    
  void feOpenFOAMSetupRule::executeOnMesh(
    std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
  ) const {
    //
    // get boundary patch and id
    //
    ::Foam::polyBoundaryMesh & bM
    =
    const_cast<::Foam::polyBoundaryMesh&>(mesh.boundaryMesh());
    ::Foam::label id = bM.findPatchID( rule[1] );
    dt__throwIfWithMessage(
      id<0, 
      executeOnMesh(),
      << "rule = " << rule
    );
    
    //
    // replace old patch
    //
    bM.set(
      id,
      new ::Foam::polyPatch(
        bM[ id ].name(), 
        bM[ id ].size(),
        bM[ id ].start(), 
        bM[ id ].index(),
        bM[ id ].boundaryMesh()
      )
    );
  }  
  
  void feOpenFOAMSetupRule::executeOnVolVectorField(
    std::vector< std::string > const & rule, ::Foam::volVectorField & field
  ) const {
    std::string thisRule = getRuleOfField(field.name(), rule);
    if (thisRule.empty()) return;
        
    ::Foam::volVectorField::GeometricBoundaryField & bF 
    = 
    const_cast<::Foam::volVectorField::GeometricBoundaryField&>(
      field.boundaryField()
    );

    forAll(bF, i) {
      //
      // get patch according to rule
      //
      if ( field.mesh().boundary()[i].name() ==  rule[1] ) {
        dt__info(
          executeOnVolVectorField(),
          << "Executing" << std::endl
          << dt__eval(thisRule) << std::endl
          << "on" << std::endl
          << "field.mesh().boundary()[" << i << "].name() = " 
          << field.mesh().boundary()[i].name() << std::endl
          << "of" << std::endl
          << dt__eval(field.name())
        );        
        if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) == "fixedValue" 
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::fixedValueFvPatchField< ::Foam::vector >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("fixedValue", thisRule)
            )
          );
          
          return;
        }
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) == "zeroGradient"           
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::zeroGradientFvPatchField< ::Foam::vector >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("zeroGradient", thisRule)
            )            
          );
          
          return;
        }
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          == 
          "inletOutlet"           
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::inletOutletFvPatchField< ::Foam::vector >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("inletOutlet", thisRule)
            )            
          );
          
          return;
        }   
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          == 
          "rotatingWallVelocity"           
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i,
            new ::Foam::rotatingWallVelocityFvPatchVectorField(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("rotatingWallVelocity", thisRule)
            )
          );
          
          return;
        }
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          == 
          "flowRateInletVelocity"           
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::flowRateInletVelocityFvPatchVectorField(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("flowRateInletVelocity", thisRule)
            )            
          );
          
          return;
        }           
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) == "slip" 
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::slipFvPatchField< ::Foam::vector >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("slip", thisRule)
            )
          );
          
          return;
        }        
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          == 
          "movingWallVelocity"           
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i,
            new ::Foam::movingWallVelocityFvPatchVectorField(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("movingWallVelocity", thisRule)
            )
          );
          
          return;
        }        
        else dt__throwUnexpected(executeOnVolVectorField());
      }
    }
  }
    
  void feOpenFOAMSetupRule::executeOnVolScalarField(
    std::vector< std::string > const & rule, ::Foam::volScalarField & field
  ) const {
    std::string thisRule = getRuleOfField(field.name(), rule);
    if (thisRule.empty()) return;
        
    ::Foam::volScalarField::GeometricBoundaryField & bF 
    = 
    const_cast<::Foam::volScalarField::GeometricBoundaryField&>(
      field.boundaryField()
    );

    forAll(bF, i) {
      //
      // get patch according to rule
      //
      if ( field.mesh().boundary()[i].name() ==  rule[1] ) {
        dt__info(
          executeOnVolScalarField(),
          << "Executing" << std::endl
          << dt__eval(thisRule) << std::endl
          << "on" << std::endl
          << "field.mesh().boundary()[" << i << "].name() = " 
          << field.mesh().boundary()[i].name() << std::endl
          << "of" << std::endl
          << dt__eval(field.name())
        );
        if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          ==
          "fixedValue"                     
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::fixedValueFvPatchField< ::Foam::scalar >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("fixedValue", thisRule)
            )
          );
          
          return;
        }
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          ==
          "zeroGradient"           
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::zeroGradientFvPatchField< ::Foam::scalar >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("zeroGradient", thisRule)
            )
          );
          
          return;
        }
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          ==
          "kqRWallFunction"            
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::incompressible::RASModels
              ::kqRWallFunctionFvPatchField< ::Foam::scalar >(
                field.mesh().boundary()[i], 
                field, 
                parseOptionDict("kqRWallFunction", thisRule)
            )
          );
          
          return;          
        }
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          ==
          "epsilonWallFunction"                          
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::incompressible::RASModels
              ::epsilonWallFunctionFvPatchScalarField(
                field.mesh().boundary()[i], 
                field, 
                parseOptionDict("epsilonWallFunction", thisRule)
            ) 
          );
          
          return;          
        }      
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          ==
          "nutWallFunction"                    
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::incompressible::RASModels
              ::nutWallFunctionFvPatchScalarField(
                field.mesh().boundary()[i], 
                field, 
                parseOptionDict("nutWallFunction", thisRule)
            ) 
          );
          
          return;          
        }            
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          ==
          "turbulentIntensityKineticEnergyInlet"          
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::turbulentIntensityKineticEnergyInletFvPatchScalarField(
                field.mesh().boundary()[i], 
                field, 
                parseOptionDict(
                  "turbulentIntensityKineticEnergyInlet", thisRule
                )
            ) 
          );
          
          return;          
        }     
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) 
          ==
          "turbulentMixingLengthDissipationRateInlet"  
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::incompressible
              ::turbulentMixingLengthDissipationRateInletFvPatchScalarField(
                field.mesh().boundary()[i], 
                field, 
                parseOptionDict(
                  "turbulentMixingLengthDissipationRateInlet", thisRule
                )
            ) 
          );
          
          return;          
        }         
        else if ( 
          stringPrimitive::getStringBetween("", "(", thisRule) == "slip" 
        ) {
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::slipFvPatchField< ::Foam::scalar >(
              field.mesh().boundary()[i], 
              field, 
              parseOptionDict("slip", thisRule)
            )
          );
          
          return;
        }          
        else dt__throwUnexpected(executeOnVolScalarField());
      }
    }
  }
    
  std::string feOpenFOAMSetupRule::getRuleOfField(
    std::string const & fieldName, std::vector< std::string > const & rule
  ) {  
    //
    // start at position 3
    //
    for(int ii = 3; ii < rule.size(); ii=ii+2) {
      //
      // if rule[ii] is equal field name return
      //
      if ( rule[ii] == fieldName ) {
        dt__throwIf( (ii+1)>(rule.size()-1), getRuleOfField());
        return rule[ii+1];
      }
    }
    return std::string("");    
  }
  
  std::string feOpenFOAMSetupRule::parseOptionStr(
    std::string const & name, std::string const & str
  ) {
    if ( stringPrimitive::stringContains(name, str) ) {
      return stringPrimitive::getStringBetweenRespectOcc(
        "(", ")", str.substr(str.find(name))
      );
    
    }
    dt__warning(
      parseOptionStr(),
      << "Cannot find option. Return empty string." << std::endl
      << dt__eval(name) << std::endl
      << dt__eval(str)
    );
    return std::string("");
  }    
  
  bool feOpenFOAMSetupRule::parseOptionBool(
    std::string const & name, std::string const & str
  ) {
    if ( parseOptionStr(name, str) == "true") return true;
    return false;    
  }
   
  ::Foam::vector feOpenFOAMSetupRule::parseOptionVector(
    std::string const & name, std::string const & str
  ) {
    std::vector< dtReal > ff
    =
    qtXmlBase::muParseCSString( parseOptionStr(name, str) );
    
    dt__throwIf(ff.size()!=3, parseOptionVector());
    
    return ::Foam::vector(ff[0], ff[1], ff[2]);
  }

  ::Foam::scalar feOpenFOAMSetupRule::parseOptionScalar(
    std::string const & name, std::string const & str
  ) {
    return ::Foam::scalar(
      qtXmlBase::muParseString( parseOptionStr(name, str) )
    );
  }  

  ::Foam::dictionary feOpenFOAMSetupRule::parseOptionDict(
    std::string const & name, std::string const & str
  ) {
    ::Foam::IStringStream is(
      "{"
      +
      parseOptionStr(name, str)
      +
      "}"
    );
    
    return ::Foam::dictionary(is());
  }    
  
  aFPtrVec const & feOpenFOAMSetupRule::refAF( 
    void 
  ) const {
    return *_aF;
  }
}
