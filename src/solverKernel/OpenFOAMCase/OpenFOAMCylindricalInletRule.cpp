#include "OpenFOAMCylindricalInletRule.h"

#include <logMe/logMe.h>

#include <interfaceHeaven/stringPrimitive.h>
#include <criticalHeaven/prepareOpenFOAM.h>
#include <polyMesh.H>
#include <polyBoundaryMesh.H>
#include <polyPatch.H>
#include <volFields.H>
#include <fixedValueFvPatchField.H>

#include <analyticFunctionHeaven/vec3dThreeD.h>

namespace dtOO {
  OpenFOAMCylindricalInletRule::OpenFOAMCylindricalInletRule() {
  }

  OpenFOAMCylindricalInletRule::~OpenFOAMCylindricalInletRule() {
  }

  void OpenFOAMCylindricalInletRule::executeOnVolVectorField(
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
        ) {
          ::Foam::vector axis = parseOptionVector("rotationAxis", thisRule);
          ::Foam::vector origin = parseOptionVector("origin", thisRule);          
          if (
            stringPrimitive::stringContains("value", thisRule)
          ) {
            ::Foam::vector value = parseOptionVector("value", thisRule);
            ::Foam::scalar const radial = value.x();
            ::Foam::scalar const tangential = value.y();
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
          else if (
            stringPrimitive::stringContains("function", thisRule)
          ) {
            //
            // scale r and z
            //
            bool scale = false;
            if ( stringPrimitive::stringContains("scale", thisRule) ) {
              scale = parseOptionBool("scale", thisRule);
            }

            //
            // get function (r, phi, z) -> (u_t, u_r, u_a)
            //
            dt__ptrAss(
              vec3dThreeD const * const v3d,
              vec3dThreeD::ConstDownCast(
                OpenFOAMSetupRule::refAF().get(
                  parseOptionStr("function", thisRule)
                )
              )
            );
            
            //
            // reference axis
            //
            ::Foam::vector refAxis 
            = 
            parseOptionVector("referenceAxis", thisRule);
           
            //
            // create and set new patch
            //
            bF.set(
              i, 
              new ::Foam::fixedValueFvPatchField< ::Foam::vector >(
                field.mesh().boundary()[i], field
              )
            );
  

            //
            // z axis
            //
            ::Foam::vector z_ = axis / ::Foam::mag(axis);
            
            // defining vector origin to face center
            ::Foam::vectorField d_ = (bF[i].patch().Cf() - origin);
            
            //
            // z coordinate
            //
            ::Foam::scalarField z = (z_ & d_);
            
            // radius vector
            ::Foam::vectorField r_ =  d_ - z * z_;
            
            //
            // r coordinate
            //
            ::Foam::scalarField r =  ::Foam::mag(r_);
            
            //
            // reference and reference2 axis
            //
            refAxis = refAxis / ::Foam::mag(refAxis);
            ::Foam::vector ref2Axis 
            = 
            (z_ ^ refAxis) / ::Foam::mag(z_ ^ refAxis);
            
            //
            // phi coordinate
            //
            ::Foam::scalarField phi = ::Foam::mag( r_ );
            forAll(bF[i], j) {
              phi[j] = atan2( ref2Axis & r_[j], refAxis & r_[j] );
            }
            
            //
            // scale
            //
            if (scale) {
              r = (r - ::Foam::min(r)) / (::Foam::max(r) - ::Foam::min(r));
              z = (z - ::Foam::min(z)) / (::Foam::max(z) - ::Foam::min(z));
            }
            
            //
            // volume and swirl flow
            //
            ::Foam::scalar volFlow(0.);
            ::Foam::vector swirlFlow = ::Foam::vector::zero;
            ::Foam::scalar patchArea(0.);
            const ::Foam::scalarField& magSf = bF[i].patch().magSf();
            const ::Foam::vectorField& Sf = bF[i].patch().Sf();
            
            //
            // set values and calculate volume and swirl flow
            //
            forAll(bF[i], j) {
              dtPoint3 u_ = v3d->YdtPoint3(r[j], phi[j], z[j]);
              bF[i][j] 
              = 
              u_.z() * axis // c_a
              + 
              u_.x() * ( r_[j] / ::Foam::mag(r_[j]) ) // c_r
              +
              u_.y() * ( (r_[j] ^ axis) / ::Foam::mag(r_[j] ^ axis) ); // c_t
              
              //
              // patch area and volume flow of patch
              //
              patchArea = patchArea + magSf[j];
              volFlow = volFlow + (Sf[j] & bF[i][j]);
              swirlFlow = swirlFlow + (r_[j] ^ bF[i][j]) * (Sf[j] & bF[i][j]);
            }
            
            //
            // prevent scaling if target values are set
            //
            //
            ::Foam::scalar targetVolFlow = volFlow;
            if ( stringPrimitive::stringContains("volumeFlow", thisRule) ) {
              targetVolFlow = parseOptionScalar("volumeFlow", thisRule);
            }
            ::Foam::scalar targetSwirlFlowZ = swirlFlow.z();
            if ( stringPrimitive::stringContains("swirlFlowZ", thisRule) ) {
              targetSwirlFlowZ = parseOptionScalar("swirlFlowZ", thisRule);
              if (targetSwirlFlowZ == 0.) swirlFlow.replace(2, 1.);
            }
            
            ::Foam::scalar volFlow2 = 0.;
            ::Foam::vector swirlFlow2 = ::Foam::vector::zero;
            forAll(bF[i], j) {
              //
              // extract components
              //
              ::Foam::vector n_m = Sf[j]/magSf[j];
              ::Foam::vector n_t = (r_[j] ^ axis) / ::Foam::mag(r_[j] ^ axis);
              ::Foam::vector cM = n_m * (bF[i][j] & n_m);
              ::Foam::vector cT = n_t * (bF[i][j] & n_t);
              ::Foam::vector cR = bF[i][j] - cM - cT;
              
              //
              // scale
              //
              bF[i][j] 
              = 
              cM * (targetVolFlow/volFlow)
              +
              cT * ( (targetSwirlFlowZ/swirlFlow.z()) / (targetVolFlow/volFlow) )
              +
              cR;
              
              //
              // sum of volume and swirl flow
              //
              volFlow2 = volFlow2 + (Sf[j] & bF[i][j]);
              swirlFlow2 = swirlFlow2 + (r_[j] ^ bF[i][j]) * (Sf[j] & bF[i][j]);                
            }
            
            dt__info(
              executeOnVolVectorField(), 
              << "patch area            : " << patchArea << std::endl
              << "volume flow           : " << volFlow << std::endl
              << "volume flow (target)  : " << targetVolFlow << std::endl    
              << "volume flow (adjust)  : " << volFlow2 << std::endl
              << "swirl flow X          : " << swirlFlow.x() << std::endl
              << "swirl flow Y          : " << swirlFlow.y() << std::endl
              << "swirl flow Z          : " << swirlFlow.z() << std::endl
              << "swirl flow Z (target) : " << targetSwirlFlowZ << std::endl
              << "swirl flow X (adjust) : " << swirlFlow2.x() << std::endl
              << "swirl flow Y (adjust) : " << swirlFlow2.y() << std::endl
              << "swirl flow Z (adjust) : " << swirlFlow2.z()
            );              
  
            return;
          }          
        }
      }
    }    
    //
    // base class
    //
    OpenFOAMSetupRule::executeOnVolVectorField(rule, field);
  }  
}
