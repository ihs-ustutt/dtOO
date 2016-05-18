#include "UcylInChannel.h"

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
#include <interpolationCellPoint.H>

#include <logMe/dtParMacros.h>

namespace dtOO {  
  UcylInChannel::UcylInChannel() { 
  }

  UcylInChannel::~UcylInChannel() {
  }

	void UcylInChannel::init( 
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
//		name="UcylInChannel" 
//		label="UcylInChannel"
//    numPoints="{10}{10}"
//	>
//    <Point_3 x="0." y="0." z="0."/>
//    <Vector_3 x="0." y="0." z="0."/>
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
    
//    //
//    // get field label
//    //
//    _field = dtXmlParser::getAttributeStr("field", element);

    //
    // get number of points
    //
    _nP 
    = 
    dtXmlParser::getAttributeIntVectorMuParse("numPoints", element, cV, aF);   
    
    _axis
    =
    dtXmlParser::getDtVector3(
      dtXmlParser::getChild("Vector_3", element), bC
    );
    _origin
    =
    dtXmlParser::getDtPoint3(
      dtXmlParser::getChild("Point_3", element), bC
    );    
	}
		
  void UcylInChannel::apply(void) {    
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
          ::Foam::FatalError.exit();
        }

        //
        // create time
        //
        ::Foam::Time runTime(
          ::Foam::Time::controlDictName,
          args.rootPath(),
          args.caseName(),
          "system",
          "constant",
          !args.optionFound("noFunctionObjects")
        );
        
        //
        // create mesh
        //
        ::Foam::fvMesh mesh(
          ::Foam::IOobject(
            ::Foam::fvMesh::defaultRegion,
            runTime.timeName(),
            runTime,
            ::Foam::IOobject::MUST_READ
          )
        );
        
        //
        // radius and height of cell center
        //
        ::Foam::vector axis(_axis.x(), _axis.y(), _axis.z());
        ::Foam::point origin(_origin.x(), _origin.y(), _origin.z());
        
        const ::Foam::vectorField& c = mesh.C();
        ::Foam::vector hatAxis = axis / ::Foam::mag(axis);
        ::Foam::vectorField r_c = (c - origin);
        
        ::Foam::vectorField RField =  r_c - (hatAxis & r_c)*hatAxis;
        ::Foam::vectorField ZField =  (hatAxis & r_c)*hatAxis;
        
        //
        // lastTime
        //
        runTime.setTime(runTime.times().last(), runTime.times().size()-1);
        
        dt__info(apply(), << dt__eval(runTime.timeName()));
        
        //
        // read header of Ucyl and U
        //
        ::Foam::IOobject UcylHeader(
          "Ucyl",
          runTime.timeName(),
          mesh,
          ::Foam::IOobject::MUST_READ
        );
        ::Foam::IOobject UHeader(
          "U",
          runTime.timeName(),
          mesh,
          ::Foam::IOobject::MUST_READ
        );
        
        //
        // only volVectorField
        // 
        dt__throwIf( 
          !UcylHeader.headerOk()
          &&
          !UHeader.headerOk()
          &&
          UcylHeader.headerClassName() != "volVectorField"
          &&
          UHeader.headerClassName() != "volVectorField", 
          apply()
        );
        
        //
        // update field 
        //
        mesh.readUpdate();

        //
        // read U and Ucyl
        //
        ::Foam::volVectorField UcylField(UcylHeader, mesh);
        ::Foam::volVectorField UField(UHeader, mesh);

        //
        // interpolation
        //
        ::Foam::interpolationCellPoint< ::Foam::vector > interUCyl(UcylField);
        ::Foam::interpolationCellPoint< ::Foam::vector > interU(UField);
        
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
//          float sumArea = 0.;          
          twoDArrayHandling< dtVector3 > U(grid.size(0), grid.size(1));
          twoDArrayHandling< dtVector3 > Ucyl(grid.size(0), grid.size(1));
          twoDArrayHandling< float > U_n(grid.size(0), grid.size(1));
          twoDArrayHandling< float > Ucyl_n(grid.size(0), grid.size(1));          
          twoDArrayHandling< float > R(grid.size(0), grid.size(1));
          twoDArrayHandling< float > Z(grid.size(0), grid.size(1));
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
                ::Foam::vector UcylValue;

                //
                // set not found cells area to zero
                //              
                if (cId == -1) {
                  A[ii][jj] = 0.;
                  UValue = ::Foam::vector::zero;
                  UcylValue = ::Foam::vector::zero;
                }
                else {
//                  UValue = UField[ cId ];
//                  UcylValue = UcylField[ cId ];
                  //
                  // do interpolation
                  //            
                  UValue
                  =
                  interU.interpolate(
                    ::Foam::vector(xyz.x(), xyz.y(), xyz.z()), cId
                  );                  
                  UcylValue
                  =
                  interUCyl.interpolate(
                    ::Foam::vector(xyz.x(), xyz.y(), xyz.z()), cId
                  );                                      
                }

                //
                // assign values
                //
                U[ii][jj] = dtVector3(UValue.x(), UValue.y(), UValue.z());
                Ucyl[ii][jj] 
                = 
                dtVector3(UcylValue.x(), UcylValue.y(), UcylValue.z());

                //
                // R and Z in cylindrical coordinate frame
                //
                R[ii][jj] = ::Foam::mag( RField[ cId ] );
                Z[ii][jj] = ::Foam::mag( ZField[ cId ] );

                //
                // normal components
                //
                U_n[ii][jj] = dtLinearAlgebra::dotProduct(U[ii][jj], N[ii][jj]);              
                Ucyl_n[ii][jj] 
                = 
                dtLinearAlgebra::dotProduct(Ucyl[ii][jj], N[ii][jj]);              
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
          m2d->getLabel()+"_UcylInChannel.csv";
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
          << "# 6  r" << std::endl
          << "# 7  z" << std::endl
          << "# 8  U_x" << std::endl
          << "# 9  U_y" << std::endl
          << "# 10 U_z" << std::endl
          << "# 11 Ucyl_x" << std::endl
          << "# 12 Ucyl_y" << std::endl
          << "# 13 Ucyl_z" << std::endl
          << "# 14 U_n" << std::endl
          << "# 15 Ucyl_n" << std::endl
          << "# 16 A" << std::endl
          ;                
          
          //
          // write values
          //
          dt__forFromToIndex(0, grid.size(), ii) {
            dt__forFromToIndex(0, grid[ii].size(), jj) {              
              of 
              << logMe::dtFormat(
                "%4i, %4i, "
                "%16.8e, %16.8e, %16.8e, "
                "%16.8e, %16.8e, "
                "%16.8e, %16.8e, %16.8e, "
                "%16.8e, %16.8e, %16.8e, "
                "%16.8e, %16.8e, "
                "%16.8e"
                ) 
                % ii % jj
                % grid[ii][jj].x() % grid[ii][jj].y() % grid[ii][jj].z()
                % R[ii][jj] % Z[ii][jj] 
                % U[ii][jj].x() % U[ii][jj].y() % U[ii][jj].z()
                % Ucyl[ii][jj].x() % Ucyl[ii][jj].y() % Ucyl[ii][jj].z()
                % U_n[ii][jj] % Ucyl_n[ii][jj]
                % A[ii][jj]
              << std::endl;
            }
          }          
          of.close();
          
          //
          // open file
          //
          filename 
          = 
          _case->getDirectory( _parser->currentState() )
          +
          "/"
          +
          m2d->getLabel()+"_UcylInChannel_iAvg.csv";
          of.open( filename.c_str(), std::ios::out | std::ios::trunc );   
          //
          // write second header
          //
          of 
          << "# 1  j" << std::endl
          << "# 2  avg(x)" << std::endl
          << "# 3  avg(y)" << std::endl
          << "# 4  avg(z)" << std::endl
          << "# 5  avg(r)" << std::endl
          << "# 6  avg(z)" << std::endl
          << "# 7  avg(U_x)" << std::endl
          << "# 8  avg(U_y)" << std::endl
          << "# 9  avg(U_z)" << std::endl
          << "# 10 avg(Ucyl_x)" << std::endl
          << "# 11 avg(Ucyl_y)" << std::endl
          << "# 12 avg(Ucyl_z)" << std::endl                  
          << "# 13 avg(U_n)" << std::endl
          << "# 14 avg(Ucyl_n)" << std::endl
          << "# 15 sum(Ucyl_x*A*U_n)/sum(A*U_n)" << std::endl
          << "# 16 sum(Ucyl_y*A*U_n)/sum(A*U_n)" << std::endl
          << "# 17 sum(Ucyl_z*A*U_n)/sum(A*U_n)" << std::endl
          << "# 18 sum(U_n*A*U_n)/sum(A*U_n)" << std::endl;
          
          //
          // get values
          //
          dt__forFromToIndex(0, grid.size(1), jj) {
            dtVector3 avgXYZ(0,0,0);
            float avgR = 0.;
            float avgZ = 0.;
            dtVector3 avgU(0,0,0);
            dtVector3 avgUcyl(0,0,0);
            float sumAU_n = 0.;
            dtVector3 sumUcylAU_n(0,0,0);
            float sumU_nAU_n = 0.;
            float avgU_n = 0.;
            float avgUcyl_n = 0.;
            int nSamples = grid.fixJ(jj).size();
            dt__forFromToIndex(0, nSamples, ii) {
              if (A[ii][jj] == 0.) continue;
            
              avgXYZ = avgXYZ + dtLinearAlgebra::toDtVector3(grid[ii][jj]);
              avgR = avgR + R[ii][jj];
              avgZ = avgZ + Z[ii][jj];
              avgU = avgU + U[ii][jj];
              avgUcyl = avgUcyl + Ucyl[ii][jj];
              sumAU_n = sumAU_n + A[ii][jj]*U_n[ii][jj];
              sumUcylAU_n = sumUcylAU_n + Ucyl[ii][jj] * A[ii][jj]*U_n[ii][jj];
              sumU_nAU_n = sumU_nAU_n + U_n[ii][jj] * A[ii][jj]*U_n[ii][jj];
              avgU_n = avgU_n + U_n[ii][jj];
              avgUcyl_n = avgUcyl_n + Ucyl_n[ii][jj];
            }
            //
            // average value and position
            //
            avgXYZ = avgXYZ / nSamples;
            avgR = avgR / nSamples;
            avgZ = avgZ / nSamples;
            avgU = avgU / nSamples;
            avgUcyl = avgUcyl / nSamples;
            avgU_n = avgU_n / nSamples;
            avgUcyl_n = avgUcyl_n / nSamples;
            //
            // write values
            //            
            of 
            << logMe::dtFormat(
              "%16i, "
              "%16.8e, %16.8e, %16.8e, "
              "%16.8e, %16.8e, "
              "%16.8e, %16.8e, %16.8e, "
              "%16.8e, %16.8e, %16.8e, "
              "%16.8e, %16.8e, "
              "%16.8e, %16.8e, %16.8e, "
              "%16.8e"
              ) 
              % jj
              % avgXYZ.x() % avgXYZ.y() % avgXYZ.z()
              % avgR % avgZ
              % avgU.x() % avgU.y() % avgU.z()
              % avgUcyl.x() % avgUcyl.y() % avgUcyl.z()
              % avgU_n % avgUcyl_n
              % ( sumUcylAU_n.x() / sumAU_n ) 
              % ( sumUcylAU_n.y() / sumAU_n )
              % ( sumUcylAU_n.z() / sumAU_n )
              % ( sumU_nAU_n / sumAU_n )
            << std::endl;            
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


