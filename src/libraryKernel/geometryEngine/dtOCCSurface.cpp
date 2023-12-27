#include "dtOCCSurface.h"

#include <logMe/logMe.h>
#include "dtOCCSurfaceBase.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/calculationTypeHandling.h>

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
			dt__mustCast(_surface->getOCC().get(), Geom_Surface const, _ptr);
		, << dt__eval(_ptr) << std::endl
		);
	}

	dtOCCSurface::~dtOCCSurface() {
		
	}

  dtReal dtOCCSurface::minPara( dtInt const dim ) const {
		Standard_Real U1;
		Standard_Real U2;
		Standard_Real V1;
		Standard_Real V2;		
		dt__tryOcc(
			_ptr->Bounds(U1, U2, V1, V2);
			switch (dim) {
				case 0:		
					return floatHandling::nextIfSmaller(U1);//static_cast<dtReal>(U1);
				case 1:		
					return floatHandling::nextIfSmaller(V1);//static_cast<dtReal>(V1);
				default:
					dt__throw(
            minPara(),
						<< dt__eval(dim) << std::endl
						<< "dim should be 0 or 1."
          );
			}
		, << dt__eval(U1) << std::endl
		  << dt__eval(U2) << std::endl
			<< dt__eval(V1) << std::endl
			<< dt__eval(V2)
		);
	}
	
  dtReal dtOCCSurface::maxPara( dtInt const dim ) const {
		Standard_Real U1;
		Standard_Real U2;
		Standard_Real V1;
		Standard_Real V2;		
		dt__tryOcc(	
			_ptr->Bounds(U1, U2, V1, V2);
			switch (dim) {
				case 0:		
					return floatHandling::prevIfBigger(U2);//static_cast<dtReal>(U2);
				case 1:		
					return floatHandling::prevIfBigger(V2);//static_cast<dtReal>(V2);
				default:
					dt__throw(
            getMax(),
						<< dt__eval(dim) << std::endl
						<< "dim should be 0 or 1."
          );
			}
		, << dt__eval(U1) << std::endl
		  << dt__eval(U2) << std::endl
			<< dt__eval(V1) << std::endl
			<< dt__eval(V2)
		);
	}
	
  bool dtOCCSurface::closed( dtInt const dim ) const {
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
	
  dtPoint3 dtOCCSurface::point( dtReal const uu, dtReal const vv) const {
//  	Standard_Real uR = static_cast<Standard_Real>(uu);
//		Standard_Real vR = static_cast<Standard_Real>(vv);
		
    Standard_Real U1;
    Standard_Real U2;
    Standard_Real V1;
    Standard_Real V2;
    _ptr->Bounds(U1, U2, V1, V2);
    
    Standard_Real uR 
    = 
    calculationTypeHandling< Standard_Real, dtReal >::boundToRange(
      uu, U1, U2
    );
    Standard_Real vR 
    = 
    calculationTypeHandling< Standard_Real, dtReal >::boundToRange(
      vv, V1, V2
    );
    
		gp_Pnt pp;
		dt__tryOcc(	
			pp = _ptr->Value(uR, vR);
		, << dt__eval(uu) << std::endl
			<< dt__eval(vv) << std::endl
			<< dt__eval(uR) << std::endl
			<< dt__eval(vR)			
	  );
		return 
      dtPoint3(
        static_cast<dtReal>(pp.Coord(1)), 
				static_cast<dtReal>(pp.Coord(2)), 
				static_cast<dtReal>(pp.Coord(3))
      );
	}
	
  dtVector3 dtOCCSurface::normal( dtReal const uu, dtReal const vv) const {
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
	
  std::vector<dtVector3> 
  dtOCCSurface::firstDer( dtReal const uu, dtReal const vv) const {
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
			static_cast<dtReal>(D1U.Coord(1)), 
			static_cast<dtReal>(D1U.Coord(2)), 
			static_cast<dtReal>(D1U.Coord(3))
		);				
		retVec[1] 
		= 
		dtVector3(
			static_cast<dtReal>(D1V.Coord(1)), 
			static_cast<dtReal>(D1V.Coord(2)), 
			static_cast<dtReal>(D1V.Coord(3))
		);

		return retVec;		
		
	}
	
  std::vector<dtVector3> 
  dtOCCSurface::secondDer( dtReal const uu, dtReal const vv) const {
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
			static_cast<dtReal>(D2U.Coord(1)), 
			static_cast<dtReal>(D2U.Coord(2)), 
			static_cast<dtReal>(D2U.Coord(3))
		);				
		retVec[1] 
		= 
		dtVector3(
			static_cast<dtReal>(D2UV.Coord(1)), 
			static_cast<dtReal>(D2UV.Coord(2)), 
			static_cast<dtReal>(D2UV.Coord(3))
		);
		retVec[2] 
		= 
		dtVector3(
			static_cast<dtReal>(D2V.Coord(1)), 
			static_cast<dtReal>(D2V.Coord(2)), 
			static_cast<dtReal>(D2V.Coord(3))
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
      , 
      << dt__point3d(point) << std::endl
			<< dt__point3d(getPointPercent3d(0.,0.)) << std::endl
			<< dt__point3d(getPointPercent3d(0.,1.)) << std::endl
			<< dt__point3d(getPointPercent3d(1.,0.)) << std::endl
			<< dt__point3d(getPointPercent3d(1.,1.))
		);
      
    dt__warnIfWithSolution(
      !ext.IsDone(), return dtSurface::reparam(point), reparam()
    );
    dt__warnIfWithSolution(
      ext.NbExt()==0, return dtSurface::reparam(point), reparam() 
    );

		Extrema_POnSurf epp;
    dt__forFromToIndex( 0, ext.NbExt(), i ) {
      epp = ext.Point(i+1);
      epp.Parameter(U, V);
      if ( 
        dtSurface::inXYZTolerance(
          dtPoint3(epp.Value().X(), epp.Value().Y(), epp.Value().Z()), point
        ) 
      ) {
        return dtPoint2(
          floatHandling::boundToRange(U, minU(), maxU()),
          floatHandling::boundToRange(V, minV(), maxV())
        );        
      }
    }
    dt__warning( reparam(), << "No point found that is in tolerance.");
    return dtSurface::reparam(point);
  }
	
  dtInt dtOCCSurface::continuity( void ) const {
    dtInt ret = -1;
		dt__tryOcc(
			ret = static_cast< dtInt >(_ptr->Continuity());
		  , << dt__eval(ret)
		);
    return ret;
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
  dt__C_addCloneForpVH(dtOCCSurface);
}
