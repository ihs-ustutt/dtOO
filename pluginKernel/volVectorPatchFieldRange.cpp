#include "volVectorPatchFieldRange.h"

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
#include <argList.H>
#include <Time.H>
#include <polyMesh.H>
#include <volFields.H>
//#include <cellList.H>

#include <logMe/dtParMacros.h>

namespace dtOO {  
  volVectorPatchFieldRange::volVectorPatchFieldRange() { 
  }

  volVectorPatchFieldRange::~volVectorPatchFieldRange() {
  }

	void volVectorPatchFieldRange::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtCase * > const * const dC,
		vectorHandling< dtPlugin * > const * const pL
	) {   
	  dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);
    
//	<plugin 
//		name="volVectorPatchFieldRange" 
//		label="volVectorPatchFieldRange"
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
      !dtXmlParser::hasAttribute("patchName", element)
      &&
      !dtXmlParser::hasAttribute("weight", element),
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
    dt__throwIf(cV->empty(), init());
    _parser = &(cV->get(0)->constRefParser());
       
    //
    // get field label
    //
    _field = dtXmlParser::getAttributeStr("field", element);

    //
    // get field weight
    //
    _weight = dtXmlParser::getAttributeStr("weight", element);
    
    //
    // get min and max
    //
    if ( dtXmlParser::hasAttribute("min", element) ) {
      _min = dtXmlParser::getAttributeFloatMuParse("min", element, cV, aF);      
    }
    else _min = std::numeric_limits<float>::min();
    if ( dtXmlParser::hasAttribute("max", element) ) {
      _max = dtXmlParser::getAttributeFloatMuParse("max", element, cV, aF);      
    }
    else _max = std::numeric_limits<float>::max();
    
    //
    // patchName
    //
    _patchName = dtXmlParser::getAttributeStr("patchName", element);
	}
		
  void volVectorPatchFieldRange::apply(void) {
    //
    // get directory
    //
    std::string wDir = _case->getDirectory( _parser->currentState() );

    dt__onlyMaster {
      dt__throwIf( !systemHandling::directoryExists(wDir), apply() );
    
      //
      // enable exception throwing
      //
      ::Foam::FatalError.throwExceptions();    
      ::Foam::FatalIOError.throwExceptions();    

      //
      // argList
      //
      ::Foam::argList::noParallel();
      int argc = 3;
      std::vector< std::string > argvStr(3);
      argvStr[0] = getLabel();
      argvStr[1] = std::string("-case");
      argvStr[2] = wDir;
      char ** argv = new char*[3];
      argv[0] = const_cast< char *>(argvStr[0].c_str());
      argv[1] = const_cast< char *>(argvStr[1].c_str());
      argv[2] = const_cast< char *>(argvStr[2].c_str());

      try {
        // disable floating point exception trapping
        systemHandling::unsetEnv("FOAM_SIGFPE");

        //
        // create rootCase
        //
        ::Foam::argList args(argc, argv);
        if (!args.checkRootCase()) {
          Foam::FatalError.exit();
        }

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
        dt__throwIf(fieldHeader.headerClassName() != "volVectorField", apply());
        
        //
        // read desired field
        //
        ::Foam::volVectorField volField(fieldHeader, mesh);
       
        std::vector< dtPoint3 > pXYZ;
        std::vector< dtVector3 > val;
        std::vector< float > weight;
        
        //
        // read phi
        //
        ::Foam::surfaceScalarField phi(
          ::Foam::IOobject(
            "phi",
            runTime.timeName(),
            mesh,
            ::Foam::IOobject::MUST_READ
          ), 
          mesh
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
        ::Foam::volVectorField::PatchFieldType const & thisPatchField
        =
        volField.boundaryField()[ thisPatchID ];

        forAll( thisPatch, ii ) {
          ::Foam::scalar cVal = ::Foam::mag( thisPatchField[ ii ] );
          if ( (cVal > _min) && (cVal < _max) ) {
            dtPoint3 cPoint( 
              mesh.Cf().boundaryField()[thisPatchID][ii].component(0), 
              mesh.Cf().boundaryField()[thisPatchID][ii].component(1), 
              mesh.Cf().boundaryField()[thisPatchID][ii].component(2) 
            );
            pXYZ.push_back( cPoint );
            val.push_back( 
              dtVector3(
                thisPatchField[ ii ].component(0),
                thisPatchField[ ii ].component(1),
                thisPatchField[ ii ].component(2)
              )
            );
            if ( _weight == "area" ) {
              weight.push_back( mesh.magSf().boundaryField()[thisPatchID][ ii ] );
            }
            else if ( _weight == "massflow" ) {
              weight.push_back( phi.boundaryField()[thisPatchID][ ii ] );
            }
            else dt__throwUnexpected(apply());
          }
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
        "volVectorPatchFieldRange_"+fieldHeader.name()+".csv";
        std::fstream of;
        of.open( filename.c_str(), std::ios::out | std::ios::trunc );

        //
        // write header
        //
        of 
        << "# 1 x" << std::endl
        << "# 2 y" << std::endl
        << "# 3 z" << std::endl
        << "# 4 valueX" << std::endl
        << "# 5 valueY" << std::endl
        << "# 6 valueZ" << std::endl
        << "# 7 weight ( " << _weight << " )" << std::endl;

        //
        // write values
        //
        dt__forFromToIndex(0, pXYZ.size(), ii) {
          of 
            << logMe::dtFormat(
              "%16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e"
            ) 
            % pXYZ[ii].x() 
            % pXYZ[ii].y() 
            % pXYZ[ii].z() 
            % val[ii].x()
            % val[ii].y()
            % val[ii].z()
            % weight[ii]
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