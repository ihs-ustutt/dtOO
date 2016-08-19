#include "volVectorOnRotatingLineFieldRange.h"

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
#include <dtTransformerHeaven/xYz_rPhiZ.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <meshEngine/dtFoamLibrary.h>
#include <argList.H>
#include <Time.H>
#include <polyMesh.H>
#include <volFields.H>
#include <interpolationCellPoint.H>

#include <logMe/dtParMacros.h>

namespace dtOO {  
  volVectorOnRotatingLineFieldRange::volVectorOnRotatingLineFieldRange() { 
  }

  volVectorOnRotatingLineFieldRange::~volVectorOnRotatingLineFieldRange() {
  }

	void volVectorOnRotatingLineFieldRange::init( 
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
//		name="volVectorOnRotatingLineFieldRange" 
//		label="volVectorOnRotatingLineFieldRange"
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
    dtXmlParser::getAttributeIntMuParse("numPoints", element, cV, aF);   

    _relBandwith
    = 
    dtXmlParser::getAttributeFloatMuParse("relBandwith", element, cV, aF); 
    
    _origin
    =
	  dtXmlParserBase::getDtPoint3(
      dtXmlParserBase::getAttributeStr("origin", element), bC
    );
    _rotAxis
    =
	  dtLinearAlgebra::normalize(
      dtXmlParserBase::getDtVector3(
        dtXmlParserBase::getAttributeStr("rotationAxis", element), bC
      )
    );
    _refAxis
    =
    dtLinearAlgebra::normalize(
      dtXmlParserBase::getDtVector3(
        dtXmlParserBase::getAttributeStr("referenceAxis", element), bC
      )
    );       
	}
		
  void volVectorOnRotatingLineFieldRange::apply(void) {    
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
        // create xYz_rPhiZ transformer
        //
        xYz_rPhiZ dtT( _origin, _rotAxis, _refAxis);
        
        //
        // convert to cylindrical
        //
        // rotation axis      
        ::Foam::vector rotAxis(_rotAxis.x(), _rotAxis.y(), _rotAxis.z());
        rotAxis = rotAxis / ::Foam::mag(rotAxis);
        // origin
        ::Foam::vector origin(_origin.x(), _origin.y(), _origin.z());
        // reference axis
        ::Foam::vector refAxis(_refAxis.x(), _refAxis.y(), _refAxis.z());        
        refAxis = refAxis / ::Foam::mag(refAxis);
        ::Foam::vector ref2Axis = rotAxis ^ refAxis;
        ref2Axis = ref2Axis / ::Foam::mag(ref2Axis);
        
        const ::Foam::vectorField& C = mesh.C();        
        ::Foam::vectorField coord = C - origin;

        ::Foam::vectorField r_ = coord - (rotAxis & coord)*rotAxis;
        ::Foam::scalarField r = ::Foam::mag( r_ );
        
        ::Foam::scalarField ang = ::Foam::atan2( ref2Axis & r_, refAxis & r_ );
        
        ::Foam::vectorField z_ = (rotAxis & coord)*rotAxis;
        ::Foam::scalarField z = (rotAxis & coord);
        

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
        // read UField
        //
        ::Foam::volVectorField UField(UHeader, mesh);

        //
        // interpolation
        //
        ::Foam::interpolationCellPoint< ::Foam::vector > interU(UField);
        ::Foam::interpolationCellPoint< ::Foam::vector > interVolField(volField);

        //
        // only one dimensional mappings
        //
        dt__ptrAss(map1dTo3d const * m1d, map1dTo3d::ConstDownCast(_aG));

        //
        // create curve grid
        //
        std::vector< dtPoint3 > gridPre 
        = 
        dtPoint3_map1dTo3dPoint(m1d, _nP).result();

        //
        // create grid by averaging 2 points
        //
        std::vector< dtPoint3 > grid_cart(_nP);
        #pragma omp parallel
        {
          #pragma omp for
          dt__forFromToIndex(0, _nP, ii) {
            grid_cart[ii] = gridPre[ii];
//              dtLinearAlgebra::toDtPoint3(
//                (
//                  dtLinearAlgebra::toDtVector3(gridPre[ii])
//                  +
//                  dtLinearAlgebra::toDtVector3(gridPre[ii+1])
//                )
//                /
//                2.
//              );
          }
        }
        std::vector< dtPoint3 > grid_cyl = dtT.retract( &grid_cart );
        
        twoDArrayHandling< std::pair< int, dtPoint3 > > meshPair(grid_cyl.size(), 0);
        float deltaR = _relBandwith * m1d->length();
        float deltaZ = _relBandwith * m1d->length();
        forAll(mesh.cells(), ii) {
          dt__forAllIndex(grid_cyl, jj) {
            dtPoint3 const & aPoint = grid_cyl[jj];
            ::Foam::scalar thisR = r[ii];
            ::Foam::scalar thisAng = ang[ii];
            ::Foam::scalar thisZ = z[ii];
            
              
            if (
              ( (aPoint.x() + deltaR) > thisR )
              &&
              ( (aPoint.x() - deltaR) < thisR )
              &&
              ( (aPoint.z() + deltaZ) > thisZ )
              &&
              ( (aPoint.z() - deltaZ) < thisZ )
            ) {
              meshPair[jj].push_back( 
                std::pair< int, dtPoint3 >(
                  ii, dtPoint3(thisR, thisAng, thisZ)
                ) 
              );
            }
          }
        }
        
        //
        // sort angles in ascending order
        //
        dt__forAllIndex(meshPair, ii) {        
          std::sort( 
            meshPair[ii].begin( ), 
            meshPair[ii].end( ), 
            []( 
              const std::pair< int, dtPoint3 >& lhs, 
              const std::pair< int, dtPoint3 >& rhs 
            ) -> bool {
             return lhs.second.y() < rhs.second.y();
            }
          );
        }

        dt__forAllIndex(meshPair, ii) {
          dt__info( 
            apply(), 
            << "meshPair[ " << ii << " ] contains : " 
            << meshPair[ii].size() << " elements." << std::endl
          );
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
        // get values
        // 
        dt__forFromToIndex(1, meshPair.size(), ii) {
          float r0 = grid_cyl[ii-1].x();
          float r1 = grid_cyl[ii].x();            
          dt__forFromToIndex(1, meshPair[ii].size(), jj) {
            float phi0 = meshPair[ii][jj-1].second.y();
            float phi1 = meshPair[ii][jj].second.y();
            float z0 = meshPair[ii][jj-1].second.z();
            float z1 = meshPair[ii][jj].second.z();

            dtPoint3 p0 = dtT( dtPoint3( r0, phi0, 0.5*(z0+z1) ) );
            dtPoint3 p1 = dtT( dtPoint3( r0, phi1, 0.5*(z0+z1) ) );
            dtPoint3 p2 = dtT( dtPoint3( r1, phi1, 0.5*(z0+z1) ) );
            dtPoint3 p3 = dtT( dtPoint3( r1, phi0, 0.5*(z0+z1) ) );

            //
            // get location
            //
            dtPoint3 xyz 
            = 
            dtLinearAlgebra::toDtPoint3(
              0.25 * (
                dtLinearAlgebra::toDtVector3(p0) 
                + 
                dtLinearAlgebra::toDtVector3(p1)
                + 
                dtLinearAlgebra::toDtVector3(p2)
                + 
                dtLinearAlgebra::toDtVector3(p3)
              )
            );

            dtVector3 N
            = 
            dtLinearAlgebra::normalize(
              dtLinearAlgebra::crossProduct( p1 - p0, p3 - p0 )
            );              
            float A = dtLinearAlgebra::area( p0, p1, p2, p3 );


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
              A = 0.;
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
            dtVector3 val
            = 
            dtVector3(
              volFieldValue.x(), volFieldValue.y(), volFieldValue.z()
            );
            float phi
            = 
            A
            *
            dtLinearAlgebra::dotProduct(
              dtVector3( UValue.x(), UValue.y(), UValue.z() ), N
            );
            dtVector3 sf = A * N;


            //
            // write values
            //
            if ( phi == 0.) continue;

            of 
              << logMe::dtFormat(
                "%16.8e, %16.8e, %16.8e, "
                "%16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e, %16.8e"
              ) 
              % xyz.x() 
              % xyz.y() 
              % xyz.z() 
              % val.x()
              % val.y()
              % val.z()
              % sf.x()
              % sf.y()
              % sf.z()
              % phi
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


