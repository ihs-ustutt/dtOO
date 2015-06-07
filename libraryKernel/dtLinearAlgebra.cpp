#include "dtLinearAlgebra.h"

#include <logMe/logMe.h>
#include "dtGaussLegendreIntegration.h"
#include <CGAL/bounding_box.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/squared_distance_3.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <TMatrixD.h>
#include <TDecompSVD.h>

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
		const float angle 
	) {  
		float const one = 1.;
		float const zero = 0.;
		dtMatrix EE(3,3); EE(0,0) = one; EE(1,1) = one; EE(2,2) = one;
		dtMatrix dSnake(3,3, zero );
		dtMatrix SS(3,3);
		float sinAngle = sin(angle);
		float cosAngle = cos(angle);
		float length;

		//matrix of rotating vector
		length = sqrt(vector.squared_length());
		dSnake(0,1) = -vector.z()/length;
		dSnake(0,2) = vector.y()/length;
		dSnake(1,0) = vector.z()/length;
		dSnake(1,2) = -vector.x()/length;
		dSnake(2,0) = -vector.y()/length;
		dSnake(2,1) = vector.x()/length;

		SS = (EE + ( dSnake*sinAngle ) + ( (dSnake*dSnake) * (one-cosAngle) ));

		return dtAffTransformation3(SS(0,0), SS(0,1), SS(0,2),
																SS(1,0), SS(1,1), SS(1,2),
																SS(2,0), SS(2,1), SS(2,2),
																1
		);
	}

	dtAffTransformation3 dtLinearAlgebra::getDiagTrans( dtVector3 const vector) {
		return dtAffTransformation3(vector.x(), 0., 0.,
																0., vector.y(), 0.,
																0., 0., vector.z(),
																1.
		);      
	}

	dtVector3 dtLinearAlgebra::crossProduct(
		dtVector3 const & v0, 
		dtVector3 const & v1
	) {
		return CGAL::cross_product(v0, v1);
	}

	float dtLinearAlgebra::dotProduct(
		dtVector3 const & v0, 
		dtVector3 const & v1
	) {
		return v0 * v1;
	}	

	dtVector3 dtLinearAlgebra::normalize(
		dtVector3 const & v0
	) {
		float sqL = v0.squared_length();
		if (sqL == 0.) return dtVector3(0.,0.,0.);
		return (1./sqrt(sqL)) * v0;
	}	

	dtVector2 dtLinearAlgebra::normalize(
		dtVector2 const & v0
	) {
		float sqL = v0.squared_length();
		if (sqL == 0.) return dtVector2(0.,0.);
		return (1./sqrt(sqL)) * v0;
	}	  
	
  dtVector3 dtLinearAlgebra::meanAverage( std::vector< dtVector3 > const & vv ) {
		dtVector3 nn(0, 0, 0);
		dt__forAllConstIter(std::vector< dtVector3 >, vv, it) {
			nn = nn + (*it);
		}
		return normalize(nn);
	}
	
	float dtLinearAlgebra::length( dtVector3 const & v0 ) {
		return sqrt(v0.squared_length());
	}

	float dtLinearAlgebra::length( dtVector2 const & v0 ) {
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

	dtPoint2 dtLinearAlgebra::toDtPoint2(dtVector2 const & pp) {
		return CGAL::ORIGIN + pp;
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
	
	dtMatrix dtLinearAlgebra::invertMatrix(dtMatrix const & mat) {
//		twoDArrayHandling<float> mat2d(mat.dimension().first, mat.dimension().second);
//		for (int ii=0; ii<mat2d.size(0);ii++) {
//			for (int jj=0; jj<mat2d.size(1);jj++) {
//        mat2d[ii][jj] = mat(ii,jj);
//			}	
//		}
//		float cgalDet = CGAL::Linear_algebraCd<float>::determinant(mat);
//		dt__debug(
//			invertMatrix(), 
//			<< dt__eval(cgalDet) << std::endl
//			<< dt__eval(mat.row_dimension()) << std::endl
//			<< dt__eval(mat.column_dimension()) << std::endl
//			<< "mat = " << std::endl
//			<< floatMatrixToString(mat2d) 
//		);

		TMatrixD rootMat(TMatrixD::kZero, TMatrixD(mat.row_dimension(),mat.column_dimension()));
		for (int rr=0; rr<mat.row_dimension(); rr++) {
			for (int cc=0; cc<mat.column_dimension(); cc++) {		
		    rootMat(rr,cc) = mat(rr, cc);
			}
		}
		
    //
    // invert matrix
    //		
		double rootDet;
		TMatrixD invRootMat = rootMat.Invert(&rootDet);		
		
		if (fabs(rootDet) == 0.) {
//			dt__info(invertMatrix(), << "Using TDecompSVD");
			TDecompSVD svd(rootMat);
			svd.SetTol(1.e-16);
			bool ok = svd.Decompose();
			if (ok) {
				bool invOk = svd.Invert(invRootMat);
				if (!invOk) {		
					twoDArrayHandling<float> mat2d(mat.dimension().first, mat.dimension().second);
					for (int ii=0; ii<mat2d.size(0);ii++) {
						for (int jj=0; jj<mat2d.size(1);jj++) {
							mat2d[ii][jj] = mat(ii,jj);
						}	
					}
					dt__debug(
						invertMatrix(), 
						<< dt__eval(mat.row_dimension()) << std::endl
						<< dt__eval(mat.column_dimension()) << std::endl
						<< "mat = " << std::endl
						<< logMe::floatMatrixToString(mat2d) 
					);					
					dt__throw(
						invertMatrix(),
						<< "Inversion of TDecompSVD fails."
					);					
				}
			}
			else {
				twoDArrayHandling<float> mat2d(mat.dimension().first, mat.dimension().second);
				for (int ii=0; ii<mat2d.size(0);ii++) {
					for (int jj=0; jj<mat2d.size(1);jj++) {
						mat2d[ii][jj] = mat(ii,jj);
					}	
				}
				dt__debug(
					invertMatrix(), 
					<< dt__eval(mat.row_dimension()) << std::endl
					<< dt__eval(mat.column_dimension()) << std::endl
					<< "mat = " << std::endl
					<< logMe::floatMatrixToString(mat2d) 
				);					
				dt__throw(
				  invertMatrix(),
					<< "Decomposition fails."
				);
			}
		}

    //
		// create inverse matrix and copy to cgal matrix
		//
		dtMatrix invMat = transposeMatrix(mat);
		for (int rr=0; rr<invMat.row_dimension();rr++) {
			for (int cc=0; cc<invMat.column_dimension();cc++) {		
		    invMat(rr, cc) = invRootMat(rr, cc);
			}
		}

//		twoDArrayHandling<float> inv2d(invMat.dimension().first, invMat.dimension().second);
//		for (int ii=0; ii<inv2d.size(0);ii++) {
//			for (int jj=0; jj<inv2d.size(1);jj++) {
//        inv2d[ii][jj] = invMat(ii,jj);
//			}	
//		}
//		dt__debug(
//		  invertMatrix(), 
//      << "ROOT(det) = " << rootDet << std::endl
//      << dt__eval(rootMat.Norm1()) << std::endl
//			<< "inv = " << std::endl
//			<< floatMatrixToString(inv2d) << std::endl
//		);
		
//		twoDArrayHandling<float> cont2d(invMat.row_dimension(), mat.column_dimension());
//		dtMatrix cont = invMat * mat;
//		for (int ii=0; ii<cont2d.size(0);ii++) {
//			for (int jj=0; jj<cont2d.size(1);jj++) {
//        cont2d[ii][jj] = cont(ii,jj);
//			}	
//		}
//		dt__debug(
//		  invertMatrix(), 
//			<< "invMat * mat = " << std::endl
//			<< floatMatrixToString(cont2d) << std::endl
//		);
		
		return invMat;
	}

  dtMatrixVector dtLinearAlgebra::solveMatrix(
	  dtMatrix const & mat, dtMatrixVector const & rhs
	) {
		TMatrixD rootMat(TMatrixD::kZero, TMatrixD(mat.row_dimension(),mat.column_dimension()));
		for (int rr=0; rr<mat.row_dimension(); rr++) {
			for (int cc=0; cc<mat.column_dimension(); cc++) {		
		    rootMat(rr,cc) = mat(rr, cc);
			}
		}
    TVectorD rootVec(rhs.dimension());
		for (int rr=0; rr<rhs.dimension(); rr++) {
			rootVec(rr) = rhs[rr];
		}
		
		TDecompSVD svd(rootMat);	
		
		bool ok;
		TVectorD rootSol = svd.Solve(rootVec, ok);
		if (!ok) dt__throw(solveMatrix(), << dt__eval(ok));
						
    dtMatrixVector sol(rootSol.GetNrows());
		for (int rr=0; rr<rootSol.GetNrows(); rr++) {
			sol[rr] = rootSol(rr);
		}
		
		return sol;
		
	}
	
	dtMatrix dtLinearAlgebra::transposeMatrix(dtMatrix const mat) {
		return CGAL::Linear_algebraCd< float >::transpose(mat);
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

//		twoDArrayHandling<float> mat2d(mat.dimension().first, mat.dimension().second);
//		for (int ii=0; ii<mat2d.size(0);ii++) {
//			for (int jj=0; jj<mat2d.size(1);jj++) {
//        mat2d[ii][jj] = mat(ii,jj);
//			}	
//		}
//		dt__debug(
//			invertMatrix(), 
//			<< dt__eval(CGAL::Linear_algebraCd<float>::determinant(mat)) << std::endl
//			<< "mat = " << std::endl
//			<< floatMatrixToString(mat2d) 
//		);		
//		twoDArrayHandling<float> inv2d(matInv.dimension().first, matInv.dimension().second);
//		for (int ii=0; ii<inv2d.size(0);ii++) {
//			for (int jj=0; jj<inv2d.size(1);jj++) {
//        inv2d[ii][jj] = matInv(ii,jj);
//			}	
//		}
//		dt__debug(
//		  invertMatrix(), 
//			<< "inv = " << std::endl
//			<< floatMatrixToString(inv2d)
//		);		
		return matInv;      
	}
//
//	dtMatrix dtLinearAlgebra::ultraInvert2x3Matrix(dtMatrix const mat) const {      
//		dtMatrixVector alpha;
//		dtMatrix Tmat = transposeMatrix(mat);
//		dtMatrix AAInv = invertMatrix( mat * Tmat );  AAInv = AAInv * mat;
//		dtMatrix matInv(3,2);
//		dtMatrixVector yy(3, 0.);
//
//		yy[0] = 1.; yy[1] = 0.; yy[2] = 0.;
//		alpha = AAInv * yy;
//		matInv(0,0) = alpha[0]; matInv(0,1) = alpha[1]; 
//
//		yy[0] = 0.; yy[1] = 1.; yy[2] = 0.;
//		alpha = AAInv * yy;
//		matInv(1,0) = alpha[0]; matInv(1,1) = alpha[1]; 
//
//		yy[0] = 0.; yy[1] = 0.; yy[2] = 1.;
//		alpha = AAInv * yy;
//		matInv(2,0) = alpha[0]; matInv(2,1) = alpha[1]; 
//
//		return matInv;      
//	}        

	dtPoint3 dtLinearAlgebra::returnFarthestPointTo(dtPoint3 pp, dtPoint3 p0, dtPoint3 p1) {
		if ( (pp - p0).squared_length() >  (pp - p1).squared_length() ) {
			return p0;
		}
		else {
			return p1;
		}      
	}

	void dtLinearAlgebra::makeOrdered(std::vector< dtPoint3 > & pp) {
		if (pp.size() != 8) {
			dt__throw(makeOrdered(), << "Currently only supported for vectors with size 8.");
		}
		dtPoint3 p0 = pp[0]; //0
		dtPoint3 p1 = pp[1]; //0
		dtPoint3 q0 = pp[2]; //1
		dtPoint3 q1 = pp[3]; //1
		dtPoint3 r0 = pp[4]; //2
		dtPoint3 r1 = pp[5]; //2
		dtPoint3 s0 = pp[6]; //3
		dtPoint3 s1 = pp[7]; //3

		if ( dtVector3(p1-q0).squared_length() > dtVector3(p0-q0).squared_length() ) {
//        retVec[0]->revert();
			p0 = pp[1]; p1 = pp[0];
		}
		if ( dtVector3(p1-q0).squared_length() > dtVector3(p1-q1).squared_length() ) {
//        retVec[1]->revert();
			q0 = pp[3]; q1 = pp[2];
		}
		if ( dtVector3(q1-r0).squared_length() > dtVector3(q1-r1).squared_length() ) {
//        retVec[2]->revert();
			r0 = pp[5]; r1 = pp[4];

		}      
		if ( dtVector3(r1-s0).squared_length() > dtVector3(r1-s1).squared_length() ) {
//        retVec[3]->revert();
			s0 = pp[7]; s1 = pp[6];
		}         

		pp.clear();
		pp.push_back(p0); pp.push_back(q0); pp.push_back(r0); pp.push_back(s0);
	}    

	std::vector<dtPoint2> dtLinearAlgebra::getGaussLegendre(int const & nPoints) {
		float const * tt = NULL;
		float const * ww = NULL;
		dtGaussLegendreIntegration().gmshGaussLegendre1D(nPoints, &tt, &ww);

		if ( !tt || !ww ) {
			dt__throw(getGaussLegendre(),
							<< dt__eval(tt) << std::endl
							<< dt__eval(ww) << std::endl
							<< dt__eval(nPoints) );
		}
		std::vector<dtPoint2> p2(nPoints);
		for ( int ii=0; ii<nPoints; ii++ ) {
			p2[ii] = dtPoint2(tt[ii], ww[ii]);
		}

		return p2;
	}
	
	std::pair< dtPoint2, dtPoint2 > dtLinearAlgebra::boundingBox( std::vector< dtPoint2 > const & pp ) {
		 dtKernel::Iso_rectangle_2 c2 = CGAL::bounding_box(pp.begin(), pp.end());
		 std::pair< dtPoint2, dtPoint2 > minMax(c2.min(), c2.max());
		 return minMax;
	}

	std::pair< dtPoint3, dtPoint3 > dtLinearAlgebra::boundingBox( std::vector< dtPoint3 > const & pp ) {
		 dtKernel::Iso_cuboid_3 c3 = CGAL::bounding_box(pp.begin(), pp.end());
		 std::pair< dtPoint3, dtPoint3 > minMax(c3.min(), c3.max());			
		 return minMax;
	}
	
	bool dtLinearAlgebra::isStraightLine( std::pair< dtPoint3, dtPoint3 > const & bBox, float const & eps ) {
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
	
	float dtLinearAlgebra::distance( dtPoint2 const & p0, dtPoint2 const & p1 ) {
		return sqrt(CGAL::squared_distance(p0, p1));
	}
	
	float dtLinearAlgebra::distance( dtPoint3 const & p0, dtPoint3 const & p1 ) {
		return sqrt(CGAL::squared_distance(p0, p1));
	}
	
	float dtLinearAlgebra::angle( dtVector3 const & v0, dtVector3 const & v1 ) {
		return acos(
			dtLinearAlgebra::dotProduct(v0, v1)
			/
			( dtLinearAlgebra::length(v0) * dtLinearAlgebra::length(v1) )
		);
	}

	float dtLinearAlgebra::angleDegree( dtVector3 const & v0, dtVector3 const & v1 ) {
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
	
	bool dtLinearAlgebra::intersects(dtTriangle3 const & triangle, dtLine3 const & line) {
		dtPoint3 iPoint;   
		dtSegment3 iSegment;   
    
		CGAL::Object res = CGAL::intersection(triangle, line);
    
    if (CGAL::assign(iPoint, res)) {
//			float dP 
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
	
	bool dtLinearAlgebra::intersects(dtTriangle3 const & triangle0, dtTriangle3 const & triangle1) {
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

  std::vector< float > dtLinearAlgebra::solveQuadraticEquation(
    float const & aa, float const & bb, float const & cc
  ) {
    float dis = bb*bb - 4.0*aa*cc;

    dt__throwIf(dis<0.0, invY());

    if (dis == 0.) return std::vector< float > ( 1, -bb/(2.0*aa) );
    
    dis = sqrt(dis);
    
    std::vector< float > sol(2);
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
  
  std::vector< float > dtLinearAlgebra::unitGrid(int const & nU) {
    std::vector< float > grid(nU);
    
    float distU = 1./(nU-1);
		for (int ii=0; ii<nU; ii++) {
      dt__toFloat(float iiF, ii);
      grid[ii] = distU * iiF;
		}
    
    return grid;
  }
    
  twoDArrayHandling< dtPoint2 > dtLinearAlgebra::unitGrid( 
    int const & nU, int const & nV
  ) {
    twoDArrayHandling< dtPoint2 > grid(nU, nV);
    
    float distU = 1./(nU-1);
    float distV = 1./(nV-1);
		for (int ii=0; ii<nU; ii++) {
      for (int jj=0; jj<nV; jj++) {
			  dt__toFloat(float iiF, ii);
        dt__toFloat(float jjF, jj);
			  grid[ii][jj] = dtPoint2(distU * iiF, distV * jjF);
      }
		}		
    
    return grid;
  }
}