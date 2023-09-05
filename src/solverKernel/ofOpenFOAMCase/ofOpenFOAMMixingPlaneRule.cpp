#include "ofOpenFOAMMixingPlaneRule.h"
#include "fvMesh.H"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <meshEngine/dtFoamLibrary.h>

#include <typeInfo.H>
#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <mixingInterface/mixingInterfaceFvPatchField.H>
#include <mixingInterface/mixingInterfacePolyPatch.H>
#include <volFields.H>
#include <boost/assign.hpp>

namespace dtOO {
  bool ofOpenFOAMMixingPlaneRule::_registrated 
  =
  ofOpenFOAMSetupRule::registrate(
    dt__tmpPtr(ofOpenFOAMMixingPlaneRule, new ofOpenFOAMMixingPlaneRule())
  );
  
  ofOpenFOAMMixingPlaneRule::ofOpenFOAMMixingPlaneRule() {
  }

  ofOpenFOAMMixingPlaneRule::~ofOpenFOAMMixingPlaneRule() {
  }
  
  std::vector< std::string > ofOpenFOAMMixingPlaneRule::factoryAlias( 
    void 
  ) const {
    return ::boost::assign::list_of("OpenFOAMMixingPlaneRule");
  }  
  
  void ofOpenFOAMMixingPlaneRule::executeOnMesh(
    std::vector< std::string > const & rule, ::Foam::polyMesh & mesh
  ) const {
    //
    // get patch labels
    //
    std::vector< std::string > csvString
    =
    stringPrimitive::convertToCSVStringVector(rule[1]);
    dt__throwIf(csvString.size()!=2, executeOnPolyPatch());
    
    //
    // get corresponding patch ids
    //
    ::Foam::polyBoundaryMesh & bM
    = 
    const_cast<::Foam::polyBoundaryMesh&>(mesh.boundaryMesh());      
    ::Foam::label id0 = bM.findPatchID(csvString[0]);
    ::Foam::label id1 = bM.findPatchID(csvString[1]);
    dt__throwIfWithMessage( id0<0, executeOnMesh(), << csvString[0] );
    dt__throwIfWithMessage( id1<0, executeOnMesh(), << csvString[1] );
    
    //
    // create new mixingPlane patch (master side)
    //
    ::Foam::dictionary dd0 = parseOptionDict(csvString[0], rule[2]);  
    dd0.add("nFaces", bM[ id0 ].size());
    dd0.add("startFace", bM[ id0 ].start());
    dd0.add("shadowPatch", ::Foam::word(csvString[1]));

    ::Foam::mixingInterfacePolyPatch * mix0    
    =
    new ::Foam::mixingInterfacePolyPatch(        
      bM[ id0 ].name(), 
      dd0,
      bM[ id0 ].index(),
      bM[ id0 ].boundaryMesh(),
      "mixingInterfacePatch"
    );
    
    //
    // replace old patch with new patch
    //
    bM.set( id0, mix0);
     
    //
    // create new mixingPlane patch (shadow side)
    //
    ::Foam::dictionary dd1 = parseOptionDict(csvString[1], rule[2]);
     dd1.add("nFaces", bM[ id1 ].size());
     dd1.add("startFace", bM[ id1 ].start());
     dd1.add("shadowPatch", ::Foam::word(csvString[0]));
     
    ::Foam::mixingInterfacePolyPatch * mix1
     =
     new ::Foam::mixingInterfacePolyPatch(        
       bM[ id1 ].name(), 
       dd1,
       bM[ id1 ].index(),
       bM[ id1 ].boundaryMesh(),
       "mixingInterfacePatch"
     );

    //
    // replace old patch with new patch
    // 
    bM.set( id1, mix1);
  }
  
  void ofOpenFOAMMixingPlaneRule::executeOnVolVectorField(
    std::vector< std::string > const & rule, ::Foam::volVectorField & field
  ) const {
    //
    // patch labels
    //
    std::vector< std::string > csvString
    =
    stringPrimitive::convertToCSVStringVector(rule[1]);    
    dt__throwIf(csvString.size()!=2, executeOnVolVectorField());
    
    //
    // rule for this field
    //
    std::string thisRule = getRuleOfField(field.name(), rule);
    
    
    if ( !thisRule.empty() ) {
      std::vector< std::string > options = optionVector(thisRule);
      // throw if size is not two
      dt__throwIfWithMessage(
        options.size()!=2,
        executeOnVolVectorField(),
        << "avOptions must have two entries." << std::endl
        << "avOptions.size() = " << options.size()
      );
      
      dt__forAllIndex(options, ii) {
        if ( stringPrimitive::stringContains("mixingInterface", options[ii]) ) {
          ::Foam::volVectorField::Boundary & bF 
          = 
          const_cast<::Foam::volVectorField::Boundary&>(
            field.boundaryField()
          );

          forAll(bF, jj) {
            //
            // get patch according to rule
            //
            if ( field.mesh().boundary()[jj].name() ==  csvString[ii] ) {
              dt__info(
                executeOnVolVectorField(),
                << "Executing" << std::endl
                << dt__eval(options[ii]) << std::endl
                << "on" << std::endl
                << "field.mesh().boundary()[" << jj << "].name() = " 
                << field.mesh().boundary()[jj].name() << std::endl
                << "of" << std::endl
                << dt__eval(field.name())
              );        
              //
              // create and set new patch
              //
              bF.set(
                jj, 
                new ::Foam::mixingInterfaceFvPatchField< ::Foam::vector >(
                  field.mesh().boundary()[jj], 
                  field, 
                  parseOptionDict("mixingInterface", options[ii])
                )
              );
            }
          }
        }
        // execute rules on base class
        else {
          ofOpenFOAMSetupRule::executeOnVolVectorField(
            ::boost::assign::list_of
              (rule[0])(csvString[ii])("")(field.name())(options[ii]), 
            field
          );
        }
      }
    }
    
    ::Foam::volVectorField::Boundary & bF 
    = 
    const_cast<::Foam::volVectorField::Boundary&>(
      field.boundaryField()
    );
    
    forAll(bF, i) {
      //
      // get patch according to rule
      //
      if ( 
        (field.mesh().boundary()[i].name() ==  csvString[0])
        ||
        (field.mesh().boundary()[i].name() ==  csvString[1])
      ) {
        bF[i] = field.internalField();
      } 
    }
  }
  
  void ofOpenFOAMMixingPlaneRule::executeOnVolScalarField(
    std::vector< std::string > const & rule, ::Foam::volScalarField & field
  ) const {
    //
    // patch labels
    //
    std::vector< std::string > csvString
    =
    stringPrimitive::convertToCSVStringVector(rule[1]);    
    dt__throwIf(csvString.size()!=2, executeOnVolScalarField());
    
    //
    // rule for this field
    //
    std::string thisRule = getRuleOfField(field.name(), rule);
    
    
    if ( !thisRule.empty() ) {
      std::vector< std::string > options = optionVector(thisRule);
      // throw if size is not two
      dt__throwIfWithMessage(
        options.size()!=2,
        executeOnVolScalarField(),
        << "avOptions must have two entries." << std::endl
        << "avOptions.size() = " << options.size()
      );
      
      dt__forAllIndex(options, ii) {
        if ( stringPrimitive::stringContains("mixingInterface", options[ii]) ) {
          ::Foam::volScalarField::Boundary & bF 
          = 
          const_cast<::Foam::volScalarField::Boundary&>(
            field.boundaryField()
          );

          forAll(bF, jj) {
            //
            // get patch according to rule
            //
            if ( field.mesh().boundary()[jj].name() ==  csvString[ii] ) {
              dt__info(
                executeOnVolScalarField(),
                << "Executing" << std::endl
                << dt__eval(options[ii]) << std::endl
                << "on" << std::endl
                << "field.mesh().boundary()[" << jj << "].name() = " 
                << field.mesh().boundary()[jj].name() << std::endl
                << "of" << std::endl
                << dt__eval(field.name())
              );        
              //
              // create and set new patch
              //
              bF.set(
                jj, 
                new ::Foam::mixingInterfaceFvPatchField< ::Foam::scalar >(
                  field.mesh().boundary()[jj], 
                  field, 
                  parseOptionDict("mixingInterface", options[ii])
                )
              );
            }
          }
        }
        // execute rules on base class
        else {
          ofOpenFOAMSetupRule::executeOnVolScalarField(
            ::boost::assign::list_of
              (rule[0])(csvString[ii])("")(field.name())(options[ii]), 
            field
          );
        }
      }
    }
    
    ::Foam::volScalarField::Boundary & bF 
    = 
    const_cast<::Foam::volScalarField::Boundary&>(
      field.boundaryField()
    );
    
    forAll(bF, i) {
      //
      // get patch according to rule
      //
      if ( 
        (field.mesh().boundary()[i].name() ==  csvString[0])
        ||
        (field.mesh().boundary()[i].name() ==  csvString[1])
      ) {
        bF[i] = field.internalField();
      } 
    }
  }  
}

