#include "volVectorFieldVersusXYZ.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map2dTo3dPoint.h>
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

#include <logMe/dtParMacros.h>

namespace dtOO {  
  volVectorFieldVersusXYZ::volVectorFieldVersusXYZ() { 
  }

  volVectorFieldVersusXYZ::~volVectorFieldVersusXYZ() {
  }

	void volVectorFieldVersusXYZ::init( 
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
//		name="volVectorFieldVersusXYZ" 
//		label="volVectorFieldVersusXYZ"
//    field="URel"   
//    numPoints="{10}{10}"
//	>
//    <case label="myCase"/>
//    <analyticGeometry label="myAnalyticGeometry"
//  </plugin>  
    dt__throwIf(
      !dtXmlParser::hasChild("case", element)
      &&
      !dtXmlParser::hasChild("analyticGeometry", element)
      &&
      !dtXmlParser::hasAttribute("field", element)
      &&
      !dtXmlParser::hasAttribute("numPoints", element), 
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
    // get analyticGeometries
    //
    dt__forAllRefAuto(
      dtXmlParser::getChildVector("analyticGeometry", element), anEl
    ) {
      _aG.push_back( 
        aG->get( dtXmlParser::getAttributeStr("label", anEl) )
      );
    }
    
    //
    // get field label
    //
    _field = dtXmlParser::getAttributeStr("field", element);

    //
    // get number of points
    //
    _nP 
    = 
    dtXmlParser::getAttributeIntVectorMuParse("numPoints", element, cV, aF);   
	}
		
  void volVectorFieldVersusXYZ::apply(void) {    
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
        // check header and make sure it is a volVectorField
        // 
        dt__throwIf(
          !fieldHeader.headerOk()
          ||
          fieldHeader.headerClassName() != "volVectorField", 
          apply()
        );
        
        //
        // update field 
        //
        mesh.readUpdate();

        //
        // read desired field
        //
        ::Foam::volVectorField volField(fieldHeader, mesh);

        dt__forAllRefAuto(_aG, anAG) {
          //
          // only two dimensional mappings
          //
          dt__ptrAss(map2dTo3d const * m2d, map2dTo3d::ConstDownCast(anAG));

          //
          // create surface and value grid
          //
          twoDArrayHandling< dtPoint3 > gridPre 
          = 
          dtPoint3_map2dTo3dPoint(m2d, _nP[0]+1, _nP[1]+1).result();
          
          twoDArrayHandling< dtPoint3 > grid(_nP[0], _nP[1]);
          twoDArrayHandling< float > area(_nP[0], _nP[1]);
          twoDArrayHandling< dtVector3 > normal(_nP[0], _nP[1]);
          dt__forFromToIndex(0, _nP[0], ii) {
            dt__forFromToIndex(0, _nP[1], jj) {
              grid[ii][jj]
              =
              dtLinearAlgebra::toDtPoint3(
                (
                  dtLinearAlgebra::toDtVector3(gridPre[ii][jj])
                  +
                  dtLinearAlgebra::toDtVector3(gridPre[ii+1][jj])
                  +
                  dtLinearAlgebra::toDtVector3(gridPre[ii+1][jj+1])
                  +
                  dtLinearAlgebra::toDtVector3(gridPre[ii][jj+1])
                )
                /
                4.
              );
              normal[ii][jj] 
              = 
              dtLinearAlgebra::normalize(
                dtLinearAlgebra::crossProduct(
                  gridPre[ii+1][jj  ] - gridPre[ii][jj],
                  gridPre[ii  ][jj+1] - gridPre[ii][jj]
                )
              );
              area[ii][jj]
              =
              dtLinearAlgebra::area(
                gridPre[ii  ][jj  ], gridPre[ii+1][jj  ], 
                gridPre[ii+1][jj+1], gridPre[ii  ][jj+1]
              );
            }
          }
          
          //
          // get values
          //          
          float sumArea = 0.;
          twoDArrayHandling< dtVector3 > value(grid.size(0), grid.size(1));
          twoDArrayHandling< float > valueN(grid.size(0), grid.size(1));
          float sumValueN = 0.;
          dt__forAllIndex(grid, ii) {
            dt__forAllIndex(grid[ii], jj) {
              dtPoint3 const & xyz = grid[ii][jj];
              ::Foam::vector probePoint(xyz.x(), xyz.y(), xyz.z());
              ::Foam::label cId = mesh.findCell(probePoint);
              //
              // set not found cells area to zero
              //
              ::Foam::vector ofValue;
              if (cId == -1) {
                area[ii][jj] = 0.;
                ofValue = ::Foam::vector::zero;
              }
              else {
                ofValue = volField[ cId ];
              }
              
              sumArea = sumArea + area[ii][jj];              
              
              
              value[ii][jj] = dtVector3( ofValue.x(), ofValue.y(), ofValue.z() );
              valueN[ii][jj] 
              = 
              dtLinearAlgebra::dotProduct(value[ii][jj], normal[ii][jj]);
              sumValueN = sumValueN + valueN[ii][jj];
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
          virtualClassName()+"_"+getLabel()+"_"+fieldHeader.name()
          +"_"+m2d->getLabel()+".csv";
          std::fstream of;
          of.open( filename.c_str(), std::ios::out | std::ios::trunc );
          
          //
          // write header
          //
          of 
          << "# 1  i" << std::endl
          << "# 2  j" << std::endl
          << "# 3  x" << std::endl
          << "# 4  y" << std::endl
          << "# 5  z" << std::endl
          << "# 6  valueX" << std::endl
          << "# 7  valueY" << std::endl
          << "# 8  valueZ" << std::endl
          << "# 9  area" << std::endl
          << "# 10 valueN" << std::endl
          << "# 11 area/sum(area)" << std::endl
          << "# 12 valueN/sum(valueN)" << std::endl;          
          
          //
          // write values
          //
          dt__forFromToIndex(0, value.size(), ii) {
            dt__forFromToIndex(0, value[ii].size(), jj) {
              if (area[ii][jj] == 0.) continue;
              
              of 
              << logMe::dtFormat(
                "%16i, %16i, "
                "%16.8e, %16.8e, %16.8e, "
                "%16.8e, %16.8e, %16.8e, "
                "%16.8e, %16.8e, "
                "%16.8e, %16.8e"
                ) 
                % ii % jj
                % grid[ii][jj].x() % grid[ii][jj].y() % grid[ii][jj].z()
                % value[ii][jj].x() % value[ii][jj].y() % value[ii][jj].z()
                % area[ii][jj] % valueN[ii][jj] 
                % (area[ii][jj]/sumArea) % (valueN[ii][jj]/sumValueN)
              << std::endl;
            }
          }
          of.close();
        }
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


