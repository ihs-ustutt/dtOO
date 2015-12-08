#include "openFOAMCylindricalInletRule.h"

#include <logMe/logMe.h>

#include <interfaceHeaven/stringPrimitive.h>
#include <criticalHeaven/prepareOpenFOAM.h>
#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <volFields.H>
#include <fixedValueFvPatchField.H>

namespace dtOO {
  openFOAMCylindricalInletRule::openFOAMCylindricalInletRule() {
  }

  openFOAMCylindricalInletRule::~openFOAMCylindricalInletRule() {
  }

  void openFOAMCylindricalInletRule::executeOnVolVectorField(
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
          stringPrimitive::stringContains("rotationAxis", thisRule)
          &&
          stringPrimitive::stringContains("origin", thisRule)
          &&
          stringPrimitive::stringContains("value", thisRule)
        ) {
          ::Foam::vector axis = parseOptionVector("rotationAxis", thisRule);
          ::Foam::vector origin = parseOptionVector("origin", thisRule);
          ::Foam::vector value = parseOptionVector("value", thisRule);
          ::Foam::scalar const tangential = value.x();
          ::Foam::scalar const radial = value.y();
          ::Foam::scalar const axial = value.z();
          
          //
          // create and set new patch
          //
          bF.set(
            i, 
            new ::Foam::fixedValueFvPatchField< ::Foam::vector >(
              field.mesh().boundary()[i], field
            )
          );

          const ::Foam::vectorField& cf = bF[i].patch().Cf();
          // unit vector of axis of rotation
          ::Foam::vector hatAxis = axis/ ::Foam::mag(axis);
          // defining vector origin to face centre
          ::Foam::vectorField r = (cf - origin);
          // subtract out axial-component
          ::Foam::vectorField d =  r - (hatAxis & r)*hatAxis;
          // create unit-drection vector
          ::Foam::vectorField dhat = d/::Foam::mag(d);
          // tangentialVelocity * zhat X rhat
          ::Foam::vectorField tangVelo =  (tangential)*(hatAxis)^dhat;
  
          forAll(bF[i], j) {
            bF[i][j] 
            = 
            ::Foam::vector(
              tangVelo[j] + hatAxis*axial + radial*dhat[j]              
            );
          }

          return;
        }
        else openFOAMSetupRule::executeOnVolVectorField(rule, field);
      }
    }    
  }  
}

