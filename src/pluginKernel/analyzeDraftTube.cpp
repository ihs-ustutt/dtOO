#include "analyzeDraftTube.h"

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

#include <meshEngine/dtFoamLibrary.h>
#include <Time.H>
#include <polyMesh.H>
#include <volFields.H>
#include <linear.H>
#include <singlePhaseTransportModel.H>
#include <RASModel.H>
#include <autoPtr.H>
#include <logMe/dtParMacros.h>

#undef ClassNameNoDebug
#undef ClassName
#undef TypeName
#include <TH1F.h>

namespace dtOO {  
  analyzeDraftTube::analyzeDraftTube() { 
  }

  analyzeDraftTube::~analyzeDraftTube() {
  }

	void analyzeDraftTube::init( 
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
//		name="analyzeDraftTube" 
//		label="analyzeDraftTube"
//    wallName="DT_WALL"
//    inletName="DT_INLET"
//    outletName="DT_OUTLET"      
//	>
//    <case label="myCase"/>
//  </plugin>  
    dt__throwIf(
      !dtXmlParser::hasChild("case", element)
      &&
      !dtXmlParser::hasAttribute("wallName", element)
      &&
      !dtXmlParser::hasAttribute("inletName", element)
      &&
      !dtXmlParser::hasAttribute("outletName", element)
      &&
      !dtXmlParser::hasAttribute("quantileWallShear", element),
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
    // patchName
    //
    _wallName = dtXmlParser::getAttributeStr("wallName", element);
    _inletName = dtXmlParser::getAttributeStr("inletName", element);
    _outletName = dtXmlParser::getAttributeStr("outletName", element);
    
    //
    // values
    //
    _quantileWallShear 
    = 
    dtXmlParser::getAttributeDoubleVectorMuParse(
      "quantileWallShear", element, cV, aF
    );
    if ( dtXmlParser::hasAttribute("maxWallShear", element) ) {
      _max 
      = 
      dtXmlParser::getAttributeFloatMuParse("maxWallShear", element, cV, aF);    
    }
    else _max = std::numeric_limits< float >::max();
    
    _nBins
    =
    dtXmlParser::getAttributeIntMuParse("nBins", element, cV, aF);    
	}
		
  void analyzeDraftTube::apply(void) {    
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
        // read velocity
        //        
        ::Foam::volVectorField U(
          ::Foam::IOobject(
            "U",
            runTime.timeName(),
            mesh,
            ::Foam::IOobject::MUST_READ,
            ::Foam::IOobject::AUTO_WRITE
          ),
          mesh
        );
        
        //
        // read face flux
        //
        ::Foam::surfaceScalarField phi(
          ::Foam::IOobject(
            "phi",
            runTime.timeName(),
            mesh,
            ::Foam::IOobject::READ_IF_PRESENT,
            ::Foam::IOobject::AUTO_WRITE
          ),
          ::Foam::linearInterpolate(U) & mesh.Sf()
        );

        //
        // update field 
        //
        mesh.readUpdate();
        
        ::Foam::singlePhaseTransportModel laminarTransport(U, phi);

        ::Foam::autoPtr< ::Foam::incompressible::RASModel > RASModel(
          ::Foam::incompressible::RASModel::New(U, phi, laminarTransport)
        );

        ::Foam::volSymmTensorField Reff( RASModel->devReff() );

        ::Foam::volVectorField wallShearStress(
          ::Foam::IOobject(
            "wallShearStress",
            runTime.timeName(),
            mesh,
            ::Foam::IOobject::NO_READ,
            ::Foam::IOobject::NO_WRITE
          ),
          mesh,
          ::Foam::dimensionedVector(
            "wallShearStress",
            Reff.dimensions(),
            ::Foam::vector::zero
          )
        );

        //
        // wall patch
        //    
        ::Foam::label wallPatchI = mesh.boundaryMesh().findPatchID(_wallName);
        ::Foam::polyPatch const & wallPatch = mesh.boundaryMesh()[ wallPatchI ];
        ::Foam::volVectorField::PatchFieldType & wallPatchWallShearStress
        =
        wallShearStress.boundaryField()[ wallPatchI ];

        wallShearStress.boundaryField()[ wallPatchI ] 
        =
        (
          -mesh.Sf().boundaryField()[wallPatchI]
          /
          mesh.magSf().boundaryField()[ wallPatchI ]
        ) 
        & 
        Reff.boundaryField()[wallPatchI];



//        dt__info(
//          apply(),
//          << dt__eval(::Foam::min( ::Foam::mag( wallPatchWallShearStress ) )) << std::endl
//          << dt__eval(::Foam::max( ::Foam::mag( wallPatchWallShearStress ) )) << std::endl
//          << dt__eval(
//            ::Foam::sum( ::Foam::mag( wallPatchWallShearStress ) ) / wallPatch.size()
//          ) << std::endl
//        );

        ::TH1F hh(
          "hh", 
          "hh",
          _nBins,
          ::Foam::min( ::Foam::mag( wallPatchWallShearStress ) ),
          ::Foam::max( ::Foam::mag( wallPatchWallShearStress ) )
        );

        std::vector< dtPoint3 > pXYZ;
        std::vector< float > value;
        std::vector< float > area;

        forAll( wallPatch, ii ) {
           ::Foam::scalar cVal = ::Foam::mag( wallPatchWallShearStress[ ii ] );
           hh.Fill( cVal );//, 1.);//mesh.magSf()[ ii ] ); //count++;
           if ( cVal < _max ) {
             dtPoint3 cPoint( 
              mesh.Cf().boundaryField()[wallPatchI][ii].component(0), 
              mesh.Cf().boundaryField()[wallPatchI][ii].component(1), 
              mesh.Cf().boundaryField()[wallPatchI][ii].component(2) 
             );
             pXYZ.push_back( cPoint );
             value.push_back( cVal );
             area.push_back( mesh.magSf().boundaryField()[wallPatchI][ ii ] );
           }
         }
         std::vector< double > resQuantile(_quantileWallShear.size(), 0.);

         hh.GetQuantiles( 
           _quantileWallShear.size(),&(resQuantile[0]),&(_quantileWallShear[0]) 
         );
//         dt__info(
//           apply(),
//           << dt__eval( hh.GetMinimum() ) << std::endl
//           << dt__eval( hh.GetMaximum() ) << std::endl
//           << dt__eval( hh.GetMean() ) << std::endl
//           << dt__eval( hh.GetStdDev() )
//         );


         //
         // open file
         //
         std::fstream of;
         of.open( 
           (        
           _case->getDirectory( _parser->currentState() )
           +
           "/"
           +
           "analyzeDraftTube_distribution.csv"
           ), 
           std::ios::out | std::ios::trunc 
        );

        //
        // write header
        //
        of 
        << "# 1 bin" << std::endl
        << "# 2 nEntries" << std::endl
        << "# 3 min" << std::endl
        << "# 4 max" << std::endl
        << "# 5 mean" << std::endl;

        //
        // write values
        //
        dt__forFromToIndex( 0, hh.GetNbinsX(), ii ) {
          of 
          << logMe::dtFormat("%16i, %16i, %16.8e, %16.8e, %16.8e") 
            % ii
            % hh.GetBinContent(ii+1)
            % hh.GetBinLowEdge(ii+1)
            % (hh.GetBinLowEdge(ii+1) + hh.GetBinWidth(ii+1))
            % hh.GetBinCenter(ii+1)
          << std::endl;
        }
        of.close();

        //
        // open file
        //
        of.open( 
          (        
          _case->getDirectory( _parser->currentState() )
          +
          "/"
          +
          "analyzeDraftTube_value.csv"
          ), 
          std::ios::out | std::ios::trunc 
        );

        //
        // write header
        //
        of
          << "# 1 x" << std::endl
          << "# 2 y" << std::endl
          << "# 3 z" << std::endl
          << "# 4 value ( < " << _max << " )" << std::endl
          << "# 5 area" << std::endl;

        //
        // write values
        //
        dt__forFromToIndex(0, pXYZ.size(), ii) {
          of 
            << logMe::dtFormat("%16.8e, %16.8e, %16.8e, %16.8e, %16.8e") 
              % pXYZ[ii].x() 
              % pXYZ[ii].y() 
              % pXYZ[ii].z() 
              % value[ii]
              % area[ii]
            << std::endl;
        }
        of.close();        

        //
        // open file
        //
        of.open( 
          (        
          _case->getDirectory( _parser->currentState() )
          +
          "/"
          +
          "analyzeDraftTube_quantile.csv"
          ), 
          std::ios::out | std::ios::trunc 
        );
        
        //
        // write header
        //
        of
          << "# 1 quantile" << std::endl
          << "# 2 value" 
          << std::endl;

        //
        // write values
        //
        dt__forAllIndex(_quantileWallShear, ii) {
          of 
            << logMe::dtFormat("%16.8e, %16.8e") 
              % _quantileWallShear[ii]
              % resQuantile[ii]
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