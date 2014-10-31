#include "dtOCCSurface.h"

#include <logMe/logMe.h>
#include "dtOCCSurfaceBase.h"
#include <progHelper.h>

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
		dt__TRYOCC(			
			_surface.reset( new dtOCCSurfaceBase() );		
			_surface->setOCC( 
				Handle(Geom_Surface)::DownCast(orig.getOCC()->Copy()) 
			);
			dt__MUSTDOWNCAST(_surface->getOCC().Access(), Geom_Surface const, _ptr);
		, << DTLOGEVAL(_ptr) << LOGDEL
		);
	}

	dtOCCSurface::~dtOCCSurface() {
		
	}

  float dtOCCSurface::minPara( int const dim ) const {
		Standard_Real U1;
		Standard_Real U2;
		Standard_Real V1;
		Standard_Real V2;		
		dt__TRYOCC(
			_ptr->Bounds(U1, U2, V1, V2);
			switch (dim) {
				case 0:		
					return static_cast<float>(U1);
				case 1:		
					return static_cast<float>(V1);
				default:
					dt__THROW(minPara(),
								<< DTLOGEVAL(dim) << LOGDEL
								<< "dim should be 0 or 1.");				
			}
		, << DTLOGEVAL(U1) << LOGDEL
		  << DTLOGEVAL(U2) << LOGDEL
			<< DTLOGEVAL(V1) << LOGDEL
			<< DTLOGEVAL(V2)
		);
	}
	
  float dtOCCSurface::maxPara( int const dim ) const {
		Standard_Real U1;
		Standard_Real U2;
		Standard_Real V1;
		Standard_Real V2;		
		dt__TRYOCC(	
			_ptr->Bounds(U1, U2, V1, V2);
			switch (dim) {
				case 0:		
					return static_cast<float>(U2);
				case 1:		
					return static_cast<float>(V2);
				default:
					dt__THROW(getMax(),
								<< DTLOGEVAL(dim) << LOGDEL
								<< "dim should be 0 or 1.");				
			}
		, << DTLOGEVAL(U1) << LOGDEL
		  << DTLOGEVAL(U2) << LOGDEL
			<< DTLOGEVAL(V1) << LOGDEL
			<< DTLOGEVAL(V2)
		);
	}
	
  bool dtOCCSurface::closed( int const dim ) const {
		dt__TRYOCC(			
			switch (dim) {
				case 0:		
					return static_cast<bool>(_ptr->IsUClosed());
				case 1:		
					return static_cast<bool>(_ptr->IsVClosed());
				default:
					dt__THROW(isClosed(),
								<< DTLOGEVAL(dim) << LOGDEL
								<< "dim should be 0 or 1.");				
			}		
		, << DTLOGEVAL(dim) );
	}
	
  dtPoint3 dtOCCSurface::point( float const uu, float const vv) const {
  	Standard_Real uR = static_cast<Standard_Real>(uu);
		Standard_Real vR = static_cast<Standard_Real>(vv);
		
		gp_Pnt pp;
		dt__TRYOCC(	
			pp = _ptr->Value(uR, vR);
		, << DTLOGEVAL(uu) << LOGDEL
			<< DTLOGEVAL(vv) << LOGDEL
			<< DTLOGEVAL(uR) << LOGDEL
			<< DTLOGEVAL(vR)			
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
		dt__TRYOCC(			
			GeomLProp_SLProps props(_surface->getOCC(), uR, vR, 1, 0.01);
			dir = props.Normal();
		,	<< DTLOGEVAL(uu) << LOGDEL
			<< DTLOGEVAL(vv) << LOGDEL
	    << DTLOGEVAL(uR) << LOGDEL
			<< DTLOGEVAL(vR)
		);
		
		return dtVector3( dir.Coord(1), dir.Coord(2), dir.Coord(3) );
	}
	
  std::vector<dtVector3> dtOCCSurface::firstDer( float const uu, float const vv) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		Standard_Real vR = static_cast<Standard_Real>(vv);
		gp_Pnt pp;
		gp_Vec D1U;
		gp_Vec D1V;

		dt__TRYOCC(	
			_ptr->D1(uR, vR, pp, D1U, D1V);
	  , << DTLOGEVAL(uu) << LOGDEL
			<< DTLOGEVAL(vv) << LOGDEL
			<< DTLOGEVAL(uR) << LOGDEL
			<< DTLOGEVAL(vR)
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
			
		dt__TRYOCC(			
			_ptr->D2(
				uR, vR, 
				pp, 
				D1U, D1V,
				D2U, D2V, D2UV
			);
		, << DTLOGEVAL(uu) << LOGDEL
			<< DTLOGEVAL(vv) << LOGDEL
			<< DTLOGEVAL(uR) << LOGDEL
			<< DTLOGEVAL(vR) 
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
		dt__TRYOCC(	
			gas.Load( _surface->getOCC() );
			ext.Initialize(
				gas, 
				U1, U2, V1, V2,				
				Utol, Vtol
			);
		ext.SetFlag( Extrema_ExtFlag::Extrema_ExtFlag_MIN );
		ext.SetAlgo(Extrema_ExtAlgo::Extrema_ExtAlgo_Grad);				
			ext.Perform(pp);
		, << DTLOGPOI3D(point) << LOGDEL
			<< DTLOGEVAL(Utol) << LOGDEL
			<< DTLOGEVAL(Vtol) << LOGDEL
			<< DTLOGEVAL(U) << LOGDEL
			<< DTLOGEVAL(V) << LOGDEL
			<< DTLOGPOI3D(getPointPercent3d(0.,0.)) << LOGDEL
			<< DTLOGPOI3D(getPointPercent3d(0.,1.)) << LOGDEL
			<< DTLOGPOI3D(getPointPercent3d(1.,0.)) << LOGDEL
			<< DTLOGPOI3D(getPointPercent3d(1.,1.)) << LOGDEL
		  << DTLOGEVAL(ext.NbExt())
		);
		
		if ( !ext.IsDone() ) {
			dt__THROW(
			  reparam(), 
				<< DTLOGEVAL(ext.IsDone()) 
			  << DTLOGPOI3D(point) << LOGDEL
			  << DTLOGEVAL(Utol) << LOGDEL
			  << DTLOGEVAL(Vtol) << LOGDEL
			  << DTLOGEVAL(U) << LOGDEL
			  << DTLOGEVAL(V) << LOGDEL
			  << DTLOGPOI3D(getPointPercent3d(0.,0.)) << LOGDEL
			  << DTLOGPOI3D(getPointPercent3d(0.,1.)) << LOGDEL
			  << DTLOGPOI3D(getPointPercent3d(1.,0.)) << LOGDEL
			  << DTLOGPOI3D(getPointPercent3d(1.,1.)) 
			);
		}
		Extrema_POnSurf epp = ext.Point(1); 
		epp.Parameter(U, V);
		
		return dtPoint2(static_cast<float>(U), static_cast<float>(V));
	}
	
	dtOCCSurfaceBase const & dtOCCSurface::OCCRef( void ) const {
		return *(_surface.get());
	}
}
