#include "volVectorInChannelFieldRange.h"

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
#include <meshEngine/dtFoamLibrary.h>
#include <argList.H>
#include <Time.H>
#include <polyMesh.H>
#include <volFields.H>
#include <interpolationCellPoint.H>

#include <logMe/dtParMacros.h>
#include "dtPluginFactory.h"

namespace dtOO {  
  bool volVectorInChannelFieldRange::_registrated 
  =
  dtPluginFactory::registrate(
    dt__tmpPtr(volVectorInChannelFieldRange, new volVectorInChannelFieldRange())
  );
  
  volVectorInChannelFieldRange::volVectorInChannelFieldRange() { 
  }

  volVectorInChannelFieldRange::~volVectorInChannelFieldRange() {
  }

  void volVectorInChannelFieldRange::init( 
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
//		name="volVectorInChannelFieldRange" 
//		label="volVectorInChannelFieldRange"
//    numPoints="{10}{10}"
//    field="U"    
//    min="0.00"
//    max="0.02"    
//    analyticGeometryLabel="myAnalyticGeometry"
//	>
//    <case label="myCase"/>
//  </plugin>
    dt__throwIf(
      !dtXmlParser::hasChild("case", element)
      &&
      !dtXmlParser::hasAttribute("analyticGeometryLabel", element)
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
    dt__throwIf(cV->empty(), init());
    _parser = dtXmlParser::ptr();
    
    //
    // get analyticGeometry
    //
    _aG 
    = 
    aG->get( dtXmlParser::getAttributeStr("analyticGeometryLabel", element) );
    
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
		
  void volVectorInChannelFieldRange::apply(void) {    
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
        dt__throwIf(fieldHeader.headerClassName() != "volVectorField", apply());
        
        //
        // read desired field
        //
        ::Foam::volVectorField volField(fieldHeader, mesh);

        //
        // read U header
        //
        ::Foam::IOobject UHeader(
          "U",
          runTime.timeName(),
          mesh,
          ::Foam::IOobject::MUST_READ
        );
        
        //
        // update field 
        //
        mesh.readUpdate();
//
//        //
//        // read UField
//        //
        ::Foam::volVectorField UField(UHeader, mesh);

        //
        // interpolation
        //
        ::Foam::interpolationCellPoint< ::Foam::vector > interU(UField);
        ::Foam::interpolationCellPoint< ::Foam::vector > interVolField(volField);

        //
        // only two dimensional mappings
        //
        dt__ptrAss(map2dTo3d const * m2d, map2dTo3d::ConstDownCast(_aG));

        //
        // create surface and value grid
        //
        twoDArrayHandling< dtPoint3 > gridPre 
        = 
        dtPoint3_map2dTo3dPoint(m2d, _nP[0]+1, _nP[1]+1).result();

        //
        // create grid by averaging 4 points
        //
        twoDArrayHandling< dtPoint3 > grid(_nP[0], _nP[1]);
        twoDArrayHandling< float > A(_nP[0], _nP[1]);
        twoDArrayHandling< dtVector3 > N(_nP[0], _nP[1]);          
        #pragma omp parallel
        {
          #pragma omp for
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
              N[ii][jj] 
              = 
              dtLinearAlgebra::normalize(
                dtLinearAlgebra::crossProduct(
                  gridPre[ii+1][jj  ] - gridPre[ii][jj],
                  gridPre[ii  ][jj+1] - gridPre[ii][jj]
                )
              );              
              A[ii][jj]
              =
              dtLinearAlgebra::area(
                gridPre[ii  ][jj  ], gridPre[ii+1][jj  ], 
                gridPre[ii+1][jj+1], gridPre[ii  ][jj+1]
              );
            }
          }
        }
          
        //
        // get values
        // 
        twoDArrayHandling< dtVector3 > val(grid.size(0), grid.size(1));
        twoDArrayHandling< dtVector3 > sf(grid.size(0), grid.size(1));
        twoDArrayHandling< float > phi(grid.size(0), grid.size(1));

        #pragma omp parallel
        {
          #pragma omp for
          dt__forAllIndex(grid, ii) {
            dt__forAllIndex(grid[ii], jj) {
              //
              // get location
              //
              dtPoint3 const & xyz = grid[ii][jj];

              //
              // create FOAM vector and search cell that contains point
              //
              ::Foam::vector probePoint(xyz.x(), xyz.y(), xyz.z());
              ::Foam::label cId = mesh.findCell(probePoint);

              ::Foam::vector UValue;
              ::Foam::vector volFieldValue;

              //
              // set not found cells area to zero
              //              
              if (cId == -1) {
                A[ii][jj] = 0.;
                UValue = ::Foam::vector::zero;
                volFieldValue = ::Foam::vector::zero;
                dt__warning(apply(), << "Ignore point at " dt__point3d(xyz) );
              }
              else {
                //
                // do interpolation
                //            
                UValue
                =
                interU.interpolate(
                  ::Foam::vector(xyz.x(), xyz.y(), xyz.z()), cId
                );                  
                volFieldValue
                =
                interVolField.interpolate(
                  ::Foam::vector(xyz.x(), xyz.y(), xyz.z()), cId
                );                        
              }

              //
              // assign values
              //
              val[ii][jj] 
              = 
              dtVector3(
                volFieldValue.x(), volFieldValue.y(), volFieldValue.z()
              );
              phi[ii][jj] 
              = 
              A[ii][jj]
              *
              dtLinearAlgebra::dotProduct(
                dtVector3( UValue.x(), UValue.y(), UValue.z() ),
                N[ii][jj]
              );
              sf[ii][jj] = A[ii][jj] * N[ii][jj];
            }
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
        << "# 7  sfX " << std::endl          
        << "# 8  sfY " << std::endl
        << "# 9  sfZ " << std::endl
        << "# 10 phi" << std::endl;

        //
        // write values
        //
        dt__forFromToIndex(0, grid.size(0), ii) {
          dt__forFromToIndex(0, grid.size(1), jj) {
            if ( phi[ii][jj] == 0.) continue;
            
            of 
              << logMe::dtFormat(
                "%16.8e, %16.8e, %16.8e, "
                "%16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e"
              ) 
              % grid[ii][jj].x() 
              % grid[ii][jj].y() 
              % grid[ii][jj].z() 
              % val[ii][jj].x()
              % val[ii][jj].y()
              % val[ii][jj].z()
              % sf[ii][jj].x()
              % sf[ii][jj].y()
              % sf[ii][jj].z()
              % phi[ii][jj]
              << std::endl;
          }
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


