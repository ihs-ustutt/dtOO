#include "dtOCCSurface.h"

#include <logMe/logMe.h>
#include "dtOCCSurfaceBase.h"
#include <logMe/dtMacros.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <Geom_Surface.hxx>
#include <GeomLProp_SLProps.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <Extrema_ExtPS.hxx>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {	
	dtOCCSurface::dtOCCSurface() : dtSurface() {
		
	}

	dtOCCSurface::dtOCCSurface(const dtOCCSurfaceBase& orig) : dtSurface() {
		dt__tryOcc(			
			_surface.reset( new dtOCCSurfaceBase() );		
			_surface->setOCC( 
				Handle(Geom_Surface)::DownCast(orig.getOCC()->Copy()) 
			);
			dt__mustCast(_surface->getOCC().Access(), Geom_Surface const, _ptr);
		, << dt__eval(_ptr) << std::endl
		);
	}

	dtOCCSurface::~dtOCCSurface() {
		
	}

  float dtOCCSurface::minPara( int const dim ) const {
		Standard_Real U1;
		Standard_Real U2;
		Standard_Real V1;
		Standard_Real V2;		
		dt__tryOcc(
			_ptr->Bounds(U1, U2, V1, V2);
			switch (dim) {
				case 0:		
					return static_cast<float>(U1);
				case 1:		
					return static_cast<float>(V1);
				default:
					dt__throw(minPara(),
								<< dt__eval(dim) << std::endl
								<< "dim should be 0 or 1.");				
			}
		, << dt__eval(U1) << std::endl
		  << dt__eval(U2) << std::endl
			<< dt__eval(V1) << std::endl
			<< dt__eval(V2)
		);
	}
	
  float dtOCCSurface::maxPara( int const dim ) const {
		Standard_Real U1;
		Standard_Real U2;
		Standard_Real V1;
		Standard_Real V2;		
		dt__tryOcc(	
			_ptr->Bounds(U1, U2, V1, V2);
			switch (dim) {
				case 0:		
					return static_cast<float>(U2);
				case 1:		
					return static_cast<float>(V2);
				default:
					dt__throw(getMax(),
								<< dt__eval(dim) << std::endl
								<< "dim should be 0 or 1.");				
			}
		, << dt__eval(U1) << std::endl
		  << dt__eval(U2) << std::endl
			<< dt__eval(V1) << std::endl
			<< dt__eval(V2)
		);
	}
	
  bool dtOCCSurface::closed( int const dim ) const {
		dt__tryOcc(			
			switch (dim) {
				case 0:		
					return static_cast<bool>(_ptr->IsUClosed());
				case 1:		
					return static_cast<bool>(_ptr->IsVClosed());
				default:
					dt__throw(isClosed(),
								<< dt__eval(dim) << std::endl
								<< "dim should be 0 or 1.");				
			}		
		, << dt__eval(dim) );
	}
	
  dtPoint3 dtOCCSurface::point( float const uu, float const vv) const {
  	Standard_Real uR = static_cast<Standard_Real>(uu);
		Standard_Real vR = static_cast<Standard_Real>(vv);
		
		gp_Pnt pp;
		dt__tryOcc(	
			pp = _ptr->Value(uR, vR);
		, << dt__eval(uu) << std::endl
			<< dt__eval(vv) << std::endl
			<< dt__eval(uR) << std::endl
			<< dt__eval(vR)			
	  );
		return dtPoint3(
						static_cast<float>(pp.Coord(1)), 
						static_cast<float>(pp.Coord(2)), 
						static_cast<float>(pp.Coord(3))
		);
	}
	
  dtVector3 dtOCCSurface::normal( float const uu, float const vv) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		Standard_Real vR = static_cast<Standard_Real>(vv);

		gp_Dir dir;
		dt__tryOcc(			
			GeomLProp_SLProps props(_surface->getOCC(), uR, vR, 1, 0.01);
			dir = props.Normal();
		,	<< dt__eval(uu) << std::endl
			<< dt__eval(vv) << std::endl
	    << dt__eval(uR) << std::endl
			<< dt__eval(vR)
		);
		
		return dtVector3( dir.Coord(1), dir.Coord(2), dir.Coord(3) );
	}
	
  std::vector<dtVector3> dtOCCSurface::firstDer( float const uu, float const vv) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		Standard_Real vR = static_cast<Standard_Real>(vv);
		gp_Pnt pp;
		gp_Vec D1U;
		gp_Vec D1V;

		dt__tryOcc(	
			_ptr->D1(uR, vR, pp, D1U, D1V);
	  , << dt__eval(uu) << std::endl
			<< dt__eval(vv) << std::endl
			<< dt__eval(uR) << std::endl
			<< dt__eval(vR)
		);
		
		std::vector<dtVector3> retVec(2);
		retVec[0] 
		= 
		dtVector3(
			static_cast<float>(D1U.Coord(1)), 
			static_cast<float>(D1U.Coord(2)), 
			static_cast<float>(D1U.Coord(3))
		);				
		retVec[1] 
		= 
		dtVector3(
			static_cast<float>(D1V.Coord(1)), 
			static_cast<float>(D1V.Coord(2)), 
			static_cast<float>(D1V.Coord(3))
		);

		return retVec;		
		
	}
	
  std::vector<dtVector3> dtOCCSurface::secondDer( float const uu, float const vv) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		Standard_Real vR = static_cast<Standard_Real>(vv);
		gp_Pnt pp;
		gp_Vec D1U;
		gp_Vec D1V;		
		gp_Vec D2U;
		gp_Vec D2V;
		gp_Vec D2UV;
			
		dt__tryOcc(			
			_ptr->D2(
				uR, vR, 
				pp, 
				D1U, D1V,
				D2U, D2V, D2UV
			);
		, << dt__eval(uu) << std::endl
			<< dt__eval(vv) << std::endl
			<< dt__eval(uR) << std::endl
			<< dt__eval(vR) 
    );
		
		std::vector<dtVector3> retVec(3);
		retVec[0] 
		= 
		dtVector3(
			static_cast<float>(D2U.Coord(1)), 
			static_cast<float>(D2U.Coord(2)), 
			static_cast<float>(D2U.Coord(3))
		);				
		retVec[1] 
		= 
		dtVector3(
			static_cast<float>(D2UV.Coord(1)), 
			static_cast<float>(D2UV.Coord(2)), 
			static_cast<float>(D2UV.Coord(3))
		);
		retVec[2] 
		= 
		dtVector3(
			static_cast<float>(D2V.Coord(1)), 
			static_cast<float>(D2V.Coord(2)), 
			static_cast<float>(D2V.Coord(3))
		);
		return retVec;		
			
	}
	
  dtPoint2 dtOCCSurface::reparam(dtPoint3 const point) const {
		gp_Pnt pp(
			static_cast<Standard_Real>(point.x()),
			static_cast<Standard_Real>(point.y()),
			static_cast<Standard_Real>(point.z()) 
		);
		Standard_Real Utol 
		= 
		static_cast<Standard_Real>(
			staticPropertiesHandler::getInstance()->getOptionFloat(
				"reparamOnFace_precision"
			)
		);
		Standard_Real Vtol 
		= 
		static_cast<Standard_Real>(
			staticPropertiesHandler::getInstance()->getOptionFloat(
				"reparamOnFace_precision"
			)
		);		

		Standard_Real U;
		Standard_Real V;		
		Standard_Real U1;
		Standard_Real V1;		
		Standard_Real U2;
		Standard_Real V2;		
		
		_ptr->Bounds(U1, U2, V1, V2);
		
		GeomAdaptor_Surface gas;
		Extrema_ExtPS ext;	
		dt__tryOcc(	
			gas.Load( _surface->getOCC() );
			ext.Initialize(
				gas, 
				U1, U2, V1, V2,				
				Utol, Vtol
			);
		ext.SetFlag( Extrema_ExtFlag::Extrema_ExtFlag_MIN );
		ext.SetAlgo(Extrema_ExtAlgo::Extrema_ExtAlgo_Grad);				
			ext.Perform(pp);
		, << dt__point3d(point) << std::endl
			<< dt__eval(Utol) << std::endl
			<< dt__eval(Vtol) << std::endl
			<< dt__eval(U) << std::endl
			<< dt__eval(V) << std::endl
			<< dt__point3d(getPointPercent3d(0.,0.)) << std::endl
			<< dt__point3d(getPointPercent3d(0.,1.)) << std::endl
			<< dt__point3d(getPointPercent3d(1.,0.)) << std::endl
			<< dt__point3d(getPointPercent3d(1.,1.)) << std::endl
		  << dt__eval(ext.NbExt())
		);
		
		if ( !ext.IsDone() ) {
			dt__throw(
			  reparam(), 
				<< dt__eval(ext.IsDone()) 
			  << dt__point3d(point) << std::endl
			  << dt__eval(Utol) << std::endl
			  << dt__eval(Vtol) << std::endl
			  << dt__eval(U) << std::endl
			  << dt__eval(V) << std::endl
			  << dt__point3d(getPointPercent3d(0.,0.)) << std::endl
			  << dt__point3d(getPointPercent3d(0.,1.)) << std::endl
			  << dt__point3d(getPointPercent3d(1.,0.)) << std::endl
			  << dt__point3d(getPointPercent3d(1.,1.)) 
			);
		}
		Extrema_POnSurf epp = ext.Point(1); 
		epp.Parameter(U, V);
		
		return dtPoint2(static_cast<float>(U), static_cast<float>(V));
	}
	
	std::string dtOCCSurface::dumpToString( void ) const {
		std::stringstream ss;
		
		ss 
    << 
    dt__dumpToString( 
      << dt__eval(_surface->dumpToString())
    );
		
		return ss.str();
	}
  
	dtOCCSurfaceBase const & dtOCCSurface::OCCRef( void ) const {
		return *(_surface.get());
	}

	dtOCCSurfaceBase & dtOCCSurface::OCCRef( void ) {
		return *(_surface.get());
	}
}