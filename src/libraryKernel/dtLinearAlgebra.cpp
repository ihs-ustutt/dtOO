/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "dtLinearAlgebra.h"

#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector_double.h>
#include <logMe/logMe.h>
#include "dtGaussLegendreIntegration.h"
#include "logMe/dtMacros.h"
#include <CGAL/bounding_box.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/squared_distance_3.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_linalg.h>

namespace dtOO {
	dtVector3 dtLinearAlgebra::unitNormal0 = dtVector3(1,0,0);
	dtVector3 dtLinearAlgebra::unitNormal1 = dtVector3(0,1,0);
	dtVector3 dtLinearAlgebra::unitNormal2 = dtVector3(0,0,1);
	
	dtLinearAlgebra::dtLinearAlgebra() {

	}

	dtLinearAlgebra::~dtLinearAlgebra() {
	}

	dtAffTransformation3 dtLinearAlgebra::getRotation(
		dtVector3 const vector, 
		const dtReal angle 
	) {  
		dtReal const one = 1.;
		dtReal const zero = 0.;
		dtMatrix EE(3,3); EE(0,0) = one; EE(1,1) = one; EE(2,2) = one;
		dtMatrix dSnake(3,3, zero );
		dtMatrix SS(3,3);
		dtReal sinAngle = sin(angle);
		dtReal cosAngle = cos(angle);
		dtReal length;

		//matrix of rotating vector
		length = sqrt(vector.squared_length());
		dSnake(0,1) = -vector.z()/length;
		dSnake(0,2) = vector.y()/length;
		dSnake(1,0) = vector.z()/length;
		dSnake(1,2) = -vector.x()/length;
		dSnake(2,0) = -vector.y()/length;
		dSnake(2,1) = vector.x()/length;

		SS = (EE + ( dSnake*sinAngle ) + ( (dSnake*dSnake) * (one-cosAngle) ));

		return dtAffTransformation3(
      SS(0,0), SS(0,1), SS(0,2),
			SS(1,0), SS(1,1), SS(1,2),
			SS(2,0), SS(2,1), SS(2,2),
			1
		);
	}
  
	dtAffTransformation3 dtLinearAlgebra::getTranslation(
		dtVector3 const vector
	) {  
		return dtAffTransformation3( dtAffTranslation(), vector );
	}  

	dtVector3 dtLinearAlgebra::crossProduct(
		dtVector3 const & v0, 
		dtVector3 const & v1
	) {
		return CGAL::cross_product(v0, v1);
	}

	dtReal dtLinearAlgebra::dotProduct(
		dtVector3 const & v0, 
		dtVector3 const & v1
	) {
		return v0 * v1;
	}	

	dtVector3 dtLinearAlgebra::normalize(
		dtVector3 const & v0
	) {
		dtReal sqL = v0.squared_length();
		if (sqL == 0.) return dtVector3(0.,0.,0.);
		return (1./sqrt(sqL)) * v0;
	}	

	dtVector2 dtLinearAlgebra::normalize(
		dtVector2 const & v0
	) {
		dtReal sqL = v0.squared_length();
		if (sqL == 0.) return dtVector2(0.,0.);
		return (1./sqrt(sqL)) * v0;
	}	  
	
  dtVector3 dtLinearAlgebra::sum( std::vector< dtVector3 > const & vv ) {
		dtVector3 nn(0, 0, 0);
		dt__forAllConstIter(std::vector< dtVector3 >, vv, it) {
			nn = nn + (*it);
		}
		return nn;
	}
  
  dtReal dtLinearAlgebra::sum( std::vector< dtReal > const & vv ) {
		dtReal nn = 0.;
		dt__forAllConstIter(std::vector< dtReal >, vv, it) {
			nn = nn + (*it);
		}
		return nn;
	}  

  dtReal dtLinearAlgebra::sum( 
    std::vector< dtReal > const & vv, dtInt const & from, dtInt const & to 
  ) {
		dtReal nn = 0.;
		dt__forFromToIndex(from, to, ii) {
			nn = nn + vv[ii];
		}
		return nn;    
  }
	
  dtReal dtLinearAlgebra::euclidianNorm( std::vector< dtReal > const & vv ) {
		dtReal nn = 0.;
		dt__forAllRefAuto(vv, it) {
			nn = nn + it*it;
		}
		return nn;
	}  
  
  std::vector< dtReal > dtLinearAlgebra::subtract( 
    std::vector< dtReal > const & aa, std::vector< dtReal > const & bb 
  ) {
    dt__throwIf( aa.size()!=bb.size(), subtract() );
    
    std::vector< dtReal > ret(aa.size());
    dt__forAllIndex(aa, ii) ret[ii] = aa[ii] - bb[ii];
    
    return ret;
  }

  std::vector< dtReal > dtLinearAlgebra::add( 
    std::vector< dtReal > const & aa, std::vector< dtReal > const & bb 
  ) {
    dt__throwIf( aa.size()!=bb.size(), add() );
    
    std::vector< dtReal > ret(aa.size());
    dt__forAllIndex(aa, ii) ret[ii] = aa[ii] + bb[ii];
    
    return ret;
  }

  std::vector< dtReal > dtLinearAlgebra::multiply( 
    dtReal const & aa, std::vector< dtReal > const & bb 
  ) {
    std::vector< dtReal > ret(bb.size());
    dt__forAllIndex(bb, ii) ret[ii] = aa * bb[ii];
    
    return ret;    
  }
    
	dtReal dtLinearAlgebra::length( dtVector3 const & v0 ) {
		return sqrt(v0.squared_length());
	}

	dtReal dtLinearAlgebra::length( dtVector2 const & v0 ) {
		return sqrt(v0.squared_length());
	}
  
	dtVector3 dtLinearAlgebra::toDtVector3(dtPoint3 const & pp) {
		return pp - CGAL::ORIGIN;
	}

	dtVector2 dtLinearAlgebra::toDtVector2(dtPoint2 const & pp) {
		return pp - CGAL::ORIGIN;
	}    

	dtPoint3 dtLinearAlgebra::toDtPoint3(dtVector3 const & pp) {
		return CGAL::ORIGIN + pp;
	}

  std::vector< dtPoint3 > dtLinearAlgebra::toDtPoint3(
    std::vector< dtVector3 > const & vv
  ) {
    std::vector< dtPoint3 > pp;
		dt__forAllRefAuto(vv, aV) pp.push_back( toDtPoint3(aV) );
    return pp;
	}
  
	dtPoint2 dtLinearAlgebra::toDtPoint2(dtVector2 const & pp) {
		return CGAL::ORIGIN + pp;
	}        

  std::vector< dtVector3 > dtLinearAlgebra::toDtVector3(
    std::vector< dtPoint3 > const & pp
  ) {
    std::vector< dtVector3 > vv;
		dt__forAllRefAuto(pp, aP) vv.push_back( toDtVector3(aP) );
    return vv;
	}
  
	std::vector< dtPoint3 > dtLinearAlgebra::toDtPoint3Vector(
		std::vector< double > const & xx,
		std::vector< double > const & yy,
		std::vector< double > const & zz
	) {
		dt__throwIf( xx.size() != yy.size(), toDtPoint3Vector() );
		dt__throwIf( yy.size() != zz.size(), toDtPoint3Vector() );
		
		std::vector< dtPoint3 > pp(xx.size());
		dt__forAllIndex(pp, ii) {
		  pp[ii] = dtPoint3(xx[ii], yy[ii], zz[ii]);
		}
		
		return pp;
	}
  
  std::vector< dtReal > dtLinearAlgebra::toStdVector( dtPoint3 const & pp ) {
    return std::vector< dtReal > {
      static_cast<dtReal>(pp.x()), 
      static_cast<dtReal>(pp.y()), 
      static_cast<dtReal>(pp.z())
    };
  }
  
  std::vector< dtReal > dtLinearAlgebra::toStdVector( dtPoint2 const & pp) {
    return std::vector< dtReal > {
      static_cast<dtReal>(pp.x()), 
      static_cast<dtReal>(pp.y())
    };
  }
  
  std::vector< dtReal > dtLinearAlgebra::toStdVector( dtVector3 const & pp ) {
    return std::vector< dtReal > {
      static_cast<dtReal>(pp.x()), 
      static_cast<dtReal>(pp.y()), 
      static_cast<dtReal>(pp.z())
    };
  }
  
  std::vector< dtReal > dtLinearAlgebra::toStdVector( dtVector2 const & pp ) {
    return std::vector< dtReal > {
      static_cast<dtReal>(pp.x()), 
      static_cast<dtReal>(pp.y())
    };
  }
		
	dtVector2 dtLinearAlgebra::unitNormal( dtVector2 const & vv) {
	  dtVector2 nn = dtVector2(-vv.y(), vv.x());
		return (1./sqrt(nn.squared_length())) * nn;
	}
	
	dtMatrix dtLinearAlgebra::createMatrixGiveColumns( 
	  std::vector< dtVector3 > const & cols 
	) {
		dtMatrix mat(3, cols.size());
		dt__forAllConstIter(std::vector< dtVector3 >, cols, it) {
			int ii = it-cols.begin();
			mat(0, ii) = it->x();
			mat(1, ii) = it->y();
			mat(2, ii) = it->z();
		}
		
		return mat;
	}
	
	dtMatrix dtLinearAlgebra::createMatrixGiveRows( 
	  std::vector< dtVector3 > const & rows 
	) {
		dtMatrix mat(rows.size(), 3);
		dt__forAllConstIter(std::vector< dtVector3 >, rows, it) {
			int ii = it-rows.begin();
			mat(ii, 0) = it->x();
			mat(ii, 1) = it->y();
			mat(ii, 2) = it->z();
		}
		
		return mat;
	}	
	
  dtMatrixVector dtLinearAlgebra::createMatrixVector( dtVector3 const & vec ) {
		dtMatrixVector ret(3);
		
		ret[0] = vec.x();
		ret[1] = vec.y();
		ret[2] = vec.z();
		
		return ret;
	}
	
  dtMatrixVector dtLinearAlgebra::createMatrixVector( dtVector2 const & vec ) {
		dtMatrixVector ret(2);
		
		ret[0] = vec.x();
		ret[1] = vec.y();
		
		return ret;		
	}
	
	dtVector2 dtLinearAlgebra::toDtVector2(dtMatrixVector const & vec) {
		dt__throwIf(vec.dimension()!=2, toDtVector2);
		
		return dtVector2(vec[0], vec[1]);
	}
	
	dtVector3 dtLinearAlgebra::toDtVector3(dtMatrixVector const & vec) {
		dt__throwIf(vec.dimension()!=3, toDtVector3);
		
		return dtVector3(vec[0], vec[1], vec[2]);
	}	

  gsl_matrix * convertToGSLMatrix( dtMatrix const & mat ) {
    dtInt const R = mat.row_dimension();
    dtInt const C = mat.column_dimension();
    gsl_matrix * m = gsl_matrix_alloc(R, C);
  
    dt__forFromToIndex(0, R, r) 
      dt__forFromToIndex(0, C, c)
        gsl_matrix_set(m, r, c, mat(r,c));
  
    return m;
  }

  gsl_vector * convertToGSLVector( dtMatrixVector const & vec ) {
    dtInt const R = vec.dimension();
    gsl_vector * v = gsl_vector_alloc(R);
  
    dt__forFromToIndex(0, R, r) gsl_vector_set(v, r, vec[r]);
  
    return v;
  }


	dtMatrix dtLinearAlgebra::invertMatrix(dtMatrix const & mat) {
    // dimensions
    dtInt const & M = mat.row_dimension();
    dtInt const & N = mat.column_dimension();
    
    // LU decomposition
    gsl_matrix * A = convertToGSLMatrix(mat);
    gsl_permutation * p = gsl_permutation_alloc(N);
    dtInt signum = 0;
    dtInt status;
    status = gsl_linalg_LU_decomp(A, p, &signum);
    dt__throwIfWithMessage(
      status, 
      invertMatrix(), 
      << std::string(gsl_strerror(status))
    );
    gsl_matrix * inverse = gsl_matrix_alloc(N,M);
    status = gsl_linalg_LU_invert(A, p, inverse);
    dt__throwIfWithMessage(
      status, 
      invertMatrix(), 
      << std::string(gsl_strerror(status))
    );

		// convert
		dtMatrix invMat = transposeMatrix(mat);
		dt__forFromToIndex(0, M, r)//for (int rr=0; rr<invMat.row_dimension();rr++) {
  		dt__forFromToIndex(0, N, c)//for (int cc=0; cc<invMat.column_dimension();cc++) {		
		    invMat(r, c) = gsl_matrix_get(inverse, r, c);

    // free
    gsl_matrix_free(A);
    gsl_permutation_free(p);
    gsl_matrix_free(inverse);

		return invMat;
	}

  dtMatrixVector dtLinearAlgebra::solveMatrix(
	  dtMatrix const & mat, dtMatrixVector const & rhs
	) {
    // get dimensions
    dtInt const M = mat.row_dimension();
    dtInt const N = mat.column_dimension();
    dt__warnIf(M<N, solveMatrix());

    // SVD decomposition
    gsl_matrix * A = convertToGSLMatrix(mat);
    gsl_matrix * V = gsl_matrix_alloc(N, N);
    gsl_vector * S = gsl_vector_alloc(N);
    gsl_vector * work = gsl_vector_alloc(N);
    dtInt status = 0;
    status = gsl_linalg_SV_decomp(A, V, S, work);
    dt__throwIfWithMessage(
      status, 
      solveMatrix(), 
      << std::string(gsl_strerror(status))
    );

    // solve
    gsl_vector * b = convertToGSLVector(rhs);
    gsl_vector * x = gsl_vector_alloc(N);
    status = gsl_linalg_SV_solve(A, V, S, b, x);
    dt__throwIfWithMessage(
      status, 
      solveMatrix(), 
      << std::string(gsl_strerror(status))
    );

    // convert
    dtMatrixVector sol(N);
		dt__forFromToIndex(0, N, i) sol[i] = gsl_vector_get(x,i);

    // free
    gsl_matrix_free(A);
    gsl_matrix_free(V);
    gsl_vector_free(S);
    gsl_vector_free(work);
    gsl_vector_free(b);
    gsl_vector_free(x);

		return sol;
	}
	
	dtMatrix dtLinearAlgebra::transposeMatrix(dtMatrix const mat) {
		return CGAL::Linear_algebraCd< dtReal >::transpose(mat);
	}    

	dtMatrix dtLinearAlgebra::invert2x3Matrix(dtMatrix const mat) {
		dtMatrix AA(9,6,0.);
		AA(0,0) = AA(3,1) = AA(6,2) = mat(0,0);
		AA(1,0) = AA(4,1) = AA(7,2) = mat(0,1);
		AA(2,0) = AA(5,1) = AA(8,2) = mat(0,2);
		AA(0,3) = AA(3,4) = AA(6,5) = mat(1,0);
		AA(1,3) = AA(4,4) = AA(7,5) = mat(1,1);
		AA(2,3) = AA(5,4) = AA(8,5) = mat(1,2);
		dtMatrixVector yy(9, 0.);
		yy[0] = yy[4] = yy[8] = 1.;

		dtMatrix AAInv = invertMatrix( transposeMatrix(AA) * AA );
		dtMatrixVector alpha = AAInv * (transposeMatrix(AA) * yy);

		dtMatrix matInv(3,2);

		matInv(0,0) = alpha[0]; matInv(0,1) = alpha[3]; 
		matInv(1,0) = alpha[1]; matInv(1,1) = alpha[4]; 
		matInv(2,0) = alpha[2]; matInv(2,1) = alpha[5]; 

		return matInv;      
	}

	dtPoint3 dtLinearAlgebra::returnFarthestPointTo(
    dtPoint3 pp, dtPoint3 p0, dtPoint3 p1
  ) {
		if ( (pp - p0).squared_length() >  (pp - p1).squared_length() ) {
			return p0;
		}
		else {
			return p1;
		}      
	}

  dtInt dtLinearAlgebra::returnNearestPointIndexTo(
    dtPoint3 const & pp, std::vector< dtPoint3 > const & pV
  ) {
    dtReal dist = std::numeric_limits<dtReal>::max();
    dtInt minIndex = -1;
    dt__forAllIndex(pV, ii) {      
      dtReal newDist = distance(pp, pV[ii]);
      if ( newDist < dist ) {
        dist = newDist;
        minIndex = ii;
      }
    }
    dt__throwIf(minIndex<0, returnNearestPointIndexTo());
    
    return minIndex;
  }
    
	void dtLinearAlgebra::makeOrdered(std::vector< dtPoint3 > & pp) {
    dt__throwIfWithMessage(
      pp.size()!=8,
      makeOrdered(), 
      << "Currently only supported for vectors with size 8."
    );
      
		dtPoint3 p0 = pp[0]; //0
		dtPoint3 p1 = pp[1]; //0
		dtPoint3 q0 = pp[2]; //1
		dtPoint3 q1 = pp[3]; //1
		dtPoint3 r0 = pp[4]; //2
		dtPoint3 r1 = pp[5]; //2
		dtPoint3 s0 = pp[6]; //3
		dtPoint3 s1 = pp[7]; //3

		if ( (p1-q0).squared_length() > (p0-q0).squared_length() ) {
			p0 = pp[1]; p1 = pp[0];
		}
		if ( (p1-q0).squared_length() > (p1-q1).squared_length() ) {
			q0 = pp[3]; q1 = pp[2];
		}
		if ( (q1-r0).squared_length() > (q1-r1).squared_length() ) {
			r0 = pp[5]; r1 = pp[4];

		}      
		if ( (r1-s0).squared_length() > (r1-s1).squared_length() ) {
			s0 = pp[7]; s1 = pp[6];
		}         

		pp.clear();
		pp.push_back(p0); pp.push_back(q0); pp.push_back(r0); pp.push_back(s0);
	}    

	std::vector<dtPoint2> dtLinearAlgebra::getGaussLegendre(int const & nPoints) {
		dtReal const * tt = NULL;
		dtReal const * ww = NULL;
		dtGaussLegendreIntegration().gmshGaussLegendre1D(nPoints, &tt, &ww);

		if ( !tt || !ww ) {
			dt__throw(getGaussLegendre(),
							<< dt__eval(tt) << std::endl
							<< dt__eval(ww) << std::endl
							<< dt__eval(nPoints) );
		}
		std::vector<dtPoint2> p2(nPoints);
		for ( dtInt ii=0; ii<nPoints; ii++ ) {
			p2[ii] = dtPoint2(tt[ii], ww[ii]);
		}

		return p2;
	}
	
	std::pair< dtPoint2, dtPoint2 > dtLinearAlgebra::boundingBox( 
    std::vector< dtPoint2 > const & pp 
  ) {
		 dtKernel::Iso_rectangle_2 c2 = CGAL::bounding_box(pp.begin(), pp.end());
		 std::pair< dtPoint2, dtPoint2 > minMax(c2.min(), c2.max());
		 return minMax;
	}

	std::pair< dtPoint3, dtPoint3 > dtLinearAlgebra::boundingBox( 
    std::vector< dtPoint3 > const & pp 
  ) {
		 dtKernel::Iso_cuboid_3 c3 = CGAL::bounding_box(pp.begin(), pp.end());
		 std::pair< dtPoint3, dtPoint3 > minMax(c3.min(), c3.max());			
		 return minMax;
	}
	
	bool dtLinearAlgebra::isStraightLine( 
    std::pair< dtPoint3, dtPoint3 > const & bBox, dtReal const & eps 
  ) {
		dtVector3 diff = bBox.first - bBox.second;
		std::vector< bool > isSmall(3, false);

		if ( fabs(diff.x()) < eps ) isSmall[0] = true;
		if ( fabs(diff.y()) < eps ) isSmall[1] = true;
		if ( fabs(diff.z()) < eps ) isSmall[2] = true;
		if (
					 (isSmall[0]&&isSmall[1]) 
				|| (isSmall[0]&&isSmall[2])
				|| (isSmall[1]&&isSmall[2])
			 ) {
			dt__info(
				isStraightLine(), 
				<< logMe::dtFormat("Bounding box: %f, %f, %f") 
					% diff.x() % diff.y() % diff.z() << std::endl
				<< "This is a straight line."
			);	
			return true;
		}			
		return false;
	}	
	
	dtReal dtLinearAlgebra::distance( dtPoint2 const & p0, dtPoint2 const & p1 ) {
		return sqrt(CGAL::squared_distance(p0, p1));
	}
	
	dtReal dtLinearAlgebra::distance( dtPoint3 const & p0, dtPoint3 const & p1 ) {
		return sqrt(CGAL::squared_distance(p0, p1));
	}
	
	dtReal dtLinearAlgebra::angle( dtVector3 const & v0, dtVector3 const & v1 ) {
		return acos(
			dtLinearAlgebra::dotProduct(v0, v1)
			/
			( dtLinearAlgebra::length(v0) * dtLinearAlgebra::length(v1) )
		);
	}

	dtReal dtLinearAlgebra::angleDegree( 
    dtVector3 const & v0, dtVector3 const & v1 
  ) {
		return dtLinearAlgebra::angle(v0, v1) * 180./M_PI;
	}

  dtVector2 dtLinearAlgebra::ignoreZ( dtVector3 const & v0 ) {
		return dtVector2(v0.x(), v0.y());
	}
	
  dtPoint2 dtLinearAlgebra::ignoreZ( dtPoint3 const & v0 ) {
		return dtPoint2(v0.x(), v0.y());
	}
	
	std::string dtLinearAlgebra::directionString( dtVector3 const & vv ) {
		if ( (vv.x() == 1.) && (vv.y() == 0.) && (vv.z() == 0.) ) {
			return "X";
		}
		else if ( (vv.x() == 0.) && (vv.y() == 1.) && (vv.z() == 0.) ) {
			return "Y";
		}
		else if ( (vv.x() == 0.) && (vv.y() == 0.) && (vv.z() == 1.) ) {
			return "Z";
		}
		else {
			return "";
		}
	}
	
	bool dtLinearAlgebra::intersects(
    dtTriangle3 const & triangle, dtLine3 const & line
  ) {
		dtPoint3 iPoint;   
		dtSegment3 iSegment;   
    
		CGAL::Object res = CGAL::intersection(triangle, line);
    
    if (CGAL::assign(iPoint, res)) {
//			dtReal dP 
//			= 
//		  dtLinearAlgebra::dotProduct(
//				line.to_vector(),
//				iPoint - line.point(0)
//			);
//      dt__debug(
//			  intersects(), 
//				<< dt__point3d(iPoint) << std::endl
//			  << dt__eval(dP ) << std::endl
//				<< dt__eval( dtLinearAlgebra::length(line.to_vector()) ) << std::endl
//				<< dt__eval( dtLinearAlgebra::length(iPoint - line.point(0)) )
//			);			
			if ( 
				dtLinearAlgebra::length(line.to_vector())
				>
				dtLinearAlgebra::length(iPoint - line.point(0))
			){
				return true;
			}
    }
//		dt__throwIfWithMessage(CGAL::assign(iSegment, res),
//			intersects(), 
//			<< dt__point3d(iSegment.point(0)) << std::endl
//			<< dt__point3d(iSegment.point(1)) << std::endl
//			<< dt__eval(iSegment.squared_length())
//		);
		
		return false;
	}
	
	bool dtLinearAlgebra::intersects(
    dtTriangle3 const & triangle0, dtTriangle3 const & triangle1
  ) {
		dtPoint3 iPoint;   
		dtSegment3 iSegment;   
		dtTriangle3 iTriangle;   
		std::vector< dtPoint3 > iPointV;   
    
		CGAL::Object res = CGAL::intersection(triangle0, triangle1);
    
    if (CGAL::assign(iPoint, res)) {
				return true;
    }
		else if (CGAL::assign(iSegment, res)) {
			return true;
		}
		else if (CGAL::assign(iTriangle, res)) {
			return true;			
		}
		else if (CGAL::assign(iPointV, res)) {
			return true;			
		}		
		return false;
	}

	dtPoint2 dtLinearAlgebra::intersectionPoint(
    dtLine2 const & line0, dtLine2 const & line1
  ) {
		dtPoint2 iPoint;   
    
		CGAL::Object res = CGAL::intersection(line0, line1);
    
    if (CGAL::assign(iPoint, res)) {
				return iPoint;
    }
    else dt__throwUnexpected(intersectionPoint());
	}
  
  std::vector< dtReal > dtLinearAlgebra::solveQuadraticEquation(
    dtReal const & aa, dtReal const & bb, dtReal const & cc
  ) {
    dtReal dis = bb*bb - 4.0*aa*cc;

    dt__throwIf(dis<0.0, invY());

    if (dis == 0.) return std::vector< dtReal > ( 1, -bb/(2.0*aa) );
    
    dis = sqrt(dis);
    
    std::vector< dtReal > sol(2);
    sol[0] = (-bb - dis)/(2.0*aa);
    sol[1] = (-bb + dis)/(2.0*aa);    
    
    return sol;
  }
  
  bool dtLinearAlgebra::isInsideQuadrangle(
    dtPoint2 const & pt,
    dtPoint2 const & p0, dtPoint2 const & p1, 
    dtPoint2 const & p2, dtPoint2 const & p3
  ) {
    std::vector< dtPoint2 > pgn(4);
    pgn[0] = p0;
    pgn[1] = p1;
    pgn[2] = p2;
    pgn[3] = p3;
    switch( CGAL::bounded_side_2(pgn.begin(), pgn.end(), pt) ) {
      case CGAL::ON_BOUNDED_SIDE:
        return true;
      case CGAL::ON_BOUNDARY:
        return true;
      case CGAL::ON_UNBOUNDED_SIDE:
        return false;
    }
  }

  bool dtLinearAlgebra::isInsideHexahedron(
    dtPoint3 const & pt,
    dtPoint3 const & p000, dtPoint3 const & p100,
    dtPoint3 const & p010, dtPoint3 const & p110,
    dtPoint3 const & p001, dtPoint3 const & p101,
    dtPoint3 const & p011, dtPoint3 const & p111
  ) {
    if (
      dtTetrahedron3(p000, p001, p011, p101).bounded_side(pt) 
      != 
      CGAL::ON_UNBOUNDED_SIDE
    ) return true;
    if (
      dtTetrahedron3(p011, p101, p111, p110).bounded_side(pt) 
      != 
      CGAL::ON_UNBOUNDED_SIDE
    ) return true;
    if (
      dtTetrahedron3(p000, p100, p110, p101).bounded_side(pt) 
      != 
      CGAL::ON_UNBOUNDED_SIDE
    ) return true;
    if (
      dtTetrahedron3(p000, p010, p110, p011).bounded_side(pt) 
      != 
      CGAL::ON_UNBOUNDED_SIDE
    ) return true;
    if (
      dtTetrahedron3(p011, p101, p000, p110).bounded_side(pt) 
      != 
      CGAL::ON_UNBOUNDED_SIDE
    ) return true;    
    
    return false;
  }  

  dtReal dtLinearAlgebra::area(
    dtPoint3 const & p0, dtPoint3 const & p1, 
    dtPoint3 const & p2, dtPoint3 const & p3
  ) {
    return  
      sqrt(dtTriangle3(p0, p1, p2).squared_area()) 
      + 
      sqrt(dtTriangle3(p2, p3, p0).squared_area());
  }
  

  dtReal dtLinearAlgebra::volume(
    dtPoint3 const & p000, dtPoint3 const & p100,
    dtPoint3 const & p010, dtPoint3 const & p110,
    dtPoint3 const & p001, dtPoint3 const & p101,
    dtPoint3 const & p011, dtPoint3 const & p111
  ) {
    return  
      fabs( dtTetrahedron3(p000, p001, p011, p101).volume() )
      +
      fabs( dtTetrahedron3(p011, p101, p111, p110).volume() )
      +
      fabs( dtTetrahedron3(p000, p100, p110, p101).volume() )
      +
      fabs( dtTetrahedron3(p000, p010, p110, p011).volume() )
      +
      fabs( dtTetrahedron3(p011, p101, p000, p110).volume() );
  }  
  
  std::vector< dtReal > dtLinearAlgebra::unitGrid(int const & nU) {
    std::vector< dtReal > grid(nU);
    
    dtReal distU = 1./(nU-1);
		for (int ii=0; ii<nU; ii++) {
      dt__toFloat(dtReal iiF, ii);
      grid[ii] = distU * iiF;
		}
    
    return grid;
  }
    
  twoDArrayHandling< dtPoint2 > dtLinearAlgebra::unitGrid( 
    dtInt const & nU, dtInt const & nV
  ) {
    twoDArrayHandling< dtPoint2 > grid(nU, nV);
    
    dtReal distU = 1./(nU-1);
    dtReal distV = 1./(nV-1);
		for (int ii=0; ii<nU; ii++) {
      for (int jj=0; jj<nV; jj++) {
			  dt__toFloat(dtReal iiF, ii);
        dt__toFloat(dtReal jjF, jj);
			  grid[ii][jj] = dtPoint2(distU * iiF, distV * jjF);
      }
		}		
    
    return grid;
  }
}
