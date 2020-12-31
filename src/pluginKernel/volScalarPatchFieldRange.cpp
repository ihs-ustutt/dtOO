#include "volScalarPatchFieldRange.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dPoint.h>
#include <boundedVolume.h>
#include <dtCase.h>

#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/systemHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <meshEngine/dtFoamLibrary.h>
#include <argList.H>
#include <Time.H>
#include <polyMesh.H>
#include <volFields.H>
#include <linear.H>

#include <logMe/dtParMacros.h>

namespace dtOO {  
  volScalarPatchFieldRange::volScalarPatchFieldRange() { 
  }

  volScalarPatchFieldRange::~volScalarPatchFieldRange() {
  }

  void volScalarPatchFieldRange::init( 
    ::QDomElement const & element,
    baseContainer * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
    dCPtrVec const * const dC,
    dPPtrVec const * const pL
  ) {   
    dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);
    
//	<plugin 
//		name="volScalarPatchFieldRange" 
//		label="volScalarPatchFieldRange"
//    field="wallShearStress"
//    patchName="DT_WALL"    
//    min="0.00"
//    max="0.02"
//	>
//    <case label="myCase"/>
//  </plugin>  
    dt__throwIf(
      !dtXmlParser::hasChild("case", element)
      &&
      !dtXmlParser::hasAttribute("field", element)
      &&
      !dtXmlParser::hasAttribute("patchName", element),
      init()
    );
    
    //
    // get parser and case
    //
    _case 
    = 
    dC->get(
      dtXmlParser::getAttributeStr(
        "label", dtXmlParser::getChild("case", element)
      )
    );
    _parser = dtXmlParser::ptr();
       
    //
    // get field label
    //
    _field = dtXmlParser::getAttributeStr("field", element);
    
    //
    // get min and max
    //
    _noRange = true;
    if ( dtXmlParser::hasAttribute("min", element) ) {
      _min = dtXmlParser::getAttributeFloatMuParse("min", element, cV, aF);      
      _noRange = false;
    }
    else _min = std::numeric_limits<float>::min();
    if ( dtXmlParser::hasAttribute("max", element) ) {
      _max = dtXmlParser::getAttributeFloatMuParse("max", element, cV, aF);      
      _noRange = false;
    }
    else _max = std::numeric_limits<float>::max();
    
    //
    // patchName
    //
    _patchName = dtXmlParser::getAttributeStr("patchName", element);
	}
		
  void volScalarPatchFieldRange::apply(void) {
    //
    // get directory
    //
    std::string wDir = _case->getDirectory( _parser->currentState() );

    dt__onlyMaster {
      dt__throwIf( !systemHandling::directoryExists(wDir), apply() );

      try {
        ::Foam::argList args = dtFoamLibrary::initCase( getLabel(), wDir );        

        //
        // create time
        //
        ::Foam::Time runTime(
          Foam::Time::controlDictName,
          args.rootPath(),
          args.caseName(),
          "system",
          "constant",
          !args.optionFound("noFunctionObjects")
        );
        
        //
        // create mesh
        //
        Foam::fvMesh mesh(
          Foam::IOobject(
            Foam::fvMesh::defaultRegion,
            runTime.timeName(),
            runTime,
            Foam::IOobject::MUST_READ
          )
        );
        
        //
        // lastTime
        //
        runTime.setTime(runTime.times().last(), runTime.times().size()-1);
        
        //
        // read field header
        //
        ::Foam::IOobject fieldHeader(
          _field,
          runTime.timeName(),
          mesh,
          ::Foam::IOobject::MUST_READ
        );
        
        //
        // only volScalarField
        // 
        dt__throwIf( !fieldHeader.headerOk(), apply());
        dt__throwIf(fieldHeader.headerClassName() != "volScalarField", apply());
        
        //
        // read desired field
        //
        ::Foam::volScalarField volField(fieldHeader, mesh);
       
        std::vector< dtPoint3 > pXYZ;
        std::vector< float > val;
        std::vector< dtVector3 > sf;
        std::vector< float > phi;
        
        //
        // read velocity
        //        
        ::Foam::volVectorField U(
          ::Foam::IOobject(
            "U",
            runTime.timeName(),
            mesh,
            ::Foam::IOobject::MUST_READ,
            ::Foam::IOobject::NO_WRITE
          ),
          mesh
        );
        
        //
        // read face flux
        //
        ::Foam::surfaceScalarField phiField(
          ::Foam::IOobject(
            "phi",
            runTime.timeName(),
            mesh,
            ::Foam::IOobject::READ_IF_PRESENT,
            ::Foam::IOobject::NO_WRITE
          ),
          ::Foam::linearInterpolate(U) & mesh.Sf()
        );

        //
        // update field 
        //
        mesh.readUpdate();
        
        //
        // cell list
        //
        ::Foam::label thisPatchID 
        = 
        mesh.boundaryMesh().findPatchID( _patchName );
        ::Foam::polyPatch const & thisPatch 
        =
        mesh.boundaryMesh()[ thisPatchID ];
        ::Foam::volScalarField::PatchFieldType const & thisPatchField
        =
        volField.boundaryField()[ thisPatchID ];

        forAll( thisPatch, ii ) {
          ::Foam::scalar cVal = ::Foam::mag( thisPatchField[ ii ] );

          //
          // range check
          //
          if ( !_noRange && ( (cVal < _min) || (cVal > _max) ) )  continue;
            
          dtPoint3 cPoint( 
            mesh.Cf().boundaryField()[thisPatchID][ii].component(0), 
            mesh.Cf().boundaryField()[thisPatchID][ii].component(1), 
            mesh.Cf().boundaryField()[thisPatchID][ii].component(2) 
          );
          pXYZ.push_back( cPoint );
          val.push_back( thisPatchField[ ii ] );
          sf.push_back(
            dtVector3(
              mesh.Sf().boundaryField()[thisPatchID][ ii ].component(0),
              mesh.Sf().boundaryField()[thisPatchID][ ii ].component(1),
              mesh.Sf().boundaryField()[thisPatchID][ ii ].component(2)
            )
          );
          phi.push_back( phiField.boundaryField()[thisPatchID][ ii ] );   
//          }
        }
        //
        // open file
        //
        std::string filename 
        = 
        _case->getDirectory( _parser->currentState() )
        +
        "/"
        +
        virtualClassName()+"_"+getLabel()+"_"+fieldHeader.name()+".csv";
        std::fstream of;
        of.open( filename.c_str(), std::ios::out | std::ios::trunc );

        //
        // write header
        //
        of 
        << "# 1 x" << std::endl
        << "# 2 y" << std::endl
        << "# 3 z" << std::endl
        << "# 4 value" << std::endl
        << "# 5 sfX " << std::endl          
        << "# 6 sfY " << std::endl
        << "# 7 sfZ " << std::endl
        << "# 8 phi" << std::endl;

        //
        // write values
        //
        dt__forFromToIndex(0, pXYZ.size(), ii) {
          of 
            << logMe::dtFormat(
              "%16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e"
            )
            % pXYZ[ii].x() 
            % pXYZ[ii].y() 
            % pXYZ[ii].z() 
            % val[ii]
            % sf[ii].x()
            % sf[ii].y()
            % sf[ii].z()
            % phi[ii]
            << std::endl;
        }
        of.close();
      }
      catch (::Foam::error & err) {
        dt__throw(
          apply(), 
          << "Instance of ::Foam::error thrown." << std::endl
          << dt__eval(err.what()) << std::endl
          << dt__eval(err.message())
        );
      }      
    }    
  }
}