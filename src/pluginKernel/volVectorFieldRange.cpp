#include "volVectorFieldRange.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticFunctionHeaven/scaThreeD.h>
#include <boundedVolume.h>
#include <dtCase.h>

#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/systemHandling.h>
#include <logMe/dtParMacros.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <meshEngine/dtFoamLibrary.h>
#include <argList.H>
#include <Time.H>
#include <polyMesh.H>
#include <volFields.H>

namespace dtOO {  
  volVectorFieldRange::volVectorFieldRange() { 
  }

  volVectorFieldRange::~volVectorFieldRange() {
  }

	void volVectorFieldRange::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG,
		bVPtrVec const * const bV,
    dCPtrVec const * const dC,
		dPPtrVec const * const pL
	) {   
	  dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);
    
//	<plugin 
//		name="volVectorFieldRange" 
//		label="volVectorFieldRange"
//    field="U"    
//    inRangeFunction="myFun"
//	>
//    <case label="myCase"/>
//  </plugin>
    dt__throwIf(
      !dtXmlParser::hasChild("case", element)
      &&
      !dtXmlParser::hasAttribute("field", element), 
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
    
    //
    // get analyticFunctions
    //
    if ( dtXmlParser::hasAttribute("inRangeFunction", element) ) {    
      _inRange.reset(
        scaThreeD::MustDownCast(      
          aF->get( 
            dtXmlParser::getAttributeStr("inRangeFunction", element)
          )
        )->clone()
      );
    }
    if ( dtXmlParser::hasAttribute("inPositionFunction", element) ) {
      _inPosition.reset(
        scaThreeD::MustDownCast(
          aF->get( 
            dtXmlParser::getAttributeStr("inPositionFunction", element)
          )
        )->clone()
      );
    }
    
    //
    // get field label
    //
    _field = dtXmlParser::getAttributeStr("field", element);
	}
		
  void volVectorFieldRange::apply(void) {
    //
    // get directory
    //
    std::string wDir = _case->getDirectory( dtXmlParser::ptr()->currentState() );

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
          _field, runTime.timeName(), mesh, ::Foam::IOobject::MUST_READ
        );
        
        //
        // only volScalarField
        // 
        dt__throwIf( 
          !fieldHeader.headerOk() 
          || 
          fieldHeader.headerClassName() != "volVectorField", 
          apply()
        );
        
        //
        // read desired field
        //
        ::Foam::volVectorField volField(fieldHeader, mesh);

        //
        // update field 
        //
        mesh.readUpdate();
         
        //
        // get values
        // 
        std::vector< dtPoint3 > coord;         
        std::vector< dtVector3 > val;
        std::vector< float > vol;

        forAll(volField, cId) {
          ::Foam::vector volFieldValue = volField[cId];

          if (
            !_inRange.isNull()
            &&
            (
              _inRange->YFloat(
                volFieldValue.x(), volFieldValue.y(), volFieldValue.z()
              ) 
              == 
              0
            )
          ) continue;
          if ( 
            !_inPosition.isNull()
            &&
            (
              _inPosition->YFloat( 
                mesh.C()[cId].component(0), 
                mesh.C()[cId].component(1),
                mesh.C()[cId].component(2) 
              ) 
              == 
              0
            )
          ) continue;
          
          //
          // get location
          //
          coord.push_back(
            dtPoint3( 
              mesh.C()[cId].component(0),
              mesh.C()[cId].component(1),
              mesh.C()[cId].component(2)
            )
          );

          //
          // assign values
          //
          val.push_back(
            dtVector3(
              volFieldValue.x(), volFieldValue.y(), volFieldValue.z()
            )
          );
          vol.push_back( mesh.cellVolumes()[cId] );
        }
        
        //
        // open file
        //
        std::string filename 
        = 
        _case->getDirectory( dtXmlParser::constReference().currentState() )
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
        << "# 1  x" << std::endl
        << "# 2  y" << std::endl
        << "# 3  z" << std::endl
        << "# 4  valueX" << std::endl
        << "# 5  valueY" << std::endl
        << "# 6  valueZ" << std::endl
        << "# 7  volume" << std::endl;

        //
        // write values
        //
        dt__forFromToIndex(0, coord.size(), ii) {
          of 
            << logMe::dtFormat(
              "%16.8e, %16.8e, %16.8e, "
              "%16.8e, %16.8e, %16.8e, %16.8e"
            ) 
            % coord[ii].x() 
            % coord[ii].y() 
            % coord[ii].z() 
            % val[ii].x()
            % val[ii].y()
            % val[ii].z()
            % vol[ii]
            << std::endl;
        }

        //
        // close file
        //        
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


