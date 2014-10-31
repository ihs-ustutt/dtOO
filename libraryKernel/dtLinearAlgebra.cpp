#include "dtLinearAlgebra.h"

#include <logMe/logMe.h>
#include "dtGaussLegendreIntegration.h"
#include <interfaceHeaven/twoDArrayHandling.h>
#include <CGAL/bounding_box.h>
#include <TMatrixD.h>
#include <TDecompSVD.h>

namespace dtOO {
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
		return (1./sqrt(v0.squared_length())) * v0;
	}	
	
	float dtLinearAlgebra::length( dtVector3 const & v0 ) {
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

	dtVector2 dtLinearAlgebra::unitNormal( dtVector2 const & vv) {
	  dtVector2 nn = dtVector2(-vv.y(), vv.x());
		return (1./sqrt(nn.squared_length())) * nn;
	}
	
	dtMatrix dtLinearAlgebra::invertMatrix(dtMatrix const & mat) {
//		twoDArrayHandling<float> mat2d(mat.dimension().first, mat.dimension().second);
//		for (int ii=0; ii<mat2d.size(0);ii++) {
//			for (int jj=0; jj<mat2d.size(1);jj++) {
//        mat2d[ii][jj] = mat(ii,jj);
//			}	
//		}
//		float cgalDet = CGAL::Linear_algebraCd<float>::determinant(mat);
//		DTDEBUGWF(
//			invertMatrix(), 
//			<< DTLOGEVAL(cgalDet) << LOGDEL
//			<< DTLOGEVAL(mat.row_dimension()) << LOGDEL
//			<< DTLOGEVAL(mat.column_dimension()) << LOGDEL
//			<< "mat = " << LOGDEL
//			<< floatMatrixToString(mat2d) 
//		);

		//
		// determine size and copy to root matrix
		//
		int aS = mat.row_dimension();
		if (mat.column_dimension() > mat.row_dimension()) {
			aS = mat.column_dimension();
		}
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
//			DTINFOWF(invertMatrix(), << "Using TDecompSVD");
			TDecompSVD lu(rootMat);
			lu.SetTol(1.e-16);
			bool ok = lu.Decompose();
			if (ok) {
				bool invOk = lu.Invert(invRootMat);
				if (!invOk) {
				twoDArrayHandling<float> mat2d(mat.dimension().first, mat.dimension().second);
				for (int ii=0; ii<mat2d.size(0);ii++) {
					for (int jj=0; jj<mat2d.size(1);jj++) {
						mat2d[ii][jj] = mat(ii,jj);
					}	
				}
				DTDEBUGWF(
					invertMatrix(), 
					<< DTLOGEVAL(mat.row_dimension()) << LOGDEL
					<< DTLOGEVAL(mat.column_dimension()) << LOGDEL
					<< "mat = " << LOGDEL
					<< floatMatrixToString(mat2d) 
				);					
				dt__THROW(
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
				DTDEBUGWF(
					invertMatrix(), 
					<< DTLOGEVAL(mat.row_dimension()) << LOGDEL
					<< DTLOGEVAL(mat.column_dimension()) << LOGDEL
					<< "mat = " << LOGDEL
					<< floatMatrixToString(mat2d) 
				);					
				dt__THROW(
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
//		DTDEBUGWF(
//		  invertMatrix(), 
//      << "ROOT(det) = " << rootDet << LOGDEL
//      << DTLOGEVAL(rootMat.Norm1()) << LOGDEL
//			<< "inv = " << LOGDEL
//			<< floatMatrixToString(inv2d) << LOGDEL
//		);
		
//		twoDArrayHandling<float> cont2d(invMat.row_dimension(), mat.column_dimension());
//		dtMatrix cont = invMat * mat;
//		for (int ii=0; ii<cont2d.size(0);ii++) {
//			for (int jj=0; jj<cont2d.size(1);jj++) {
//        cont2d[ii][jj] = cont(ii,jj);
//			}	
//		}
//		DTDEBUGWF(
//		  invertMatrix(), 
//			<< "invMat * mat = " << LOGDEL
//			<< floatMatrixToString(cont2d) << LOGDEL
//		);
		
		return invMat;
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
//		DTDEBUGWF(
//			invertMatrix(), 
//			<< DTLOGEVAL(CGAL::Linear_algebraCd<float>::determinant(mat)) << LOGDEL
//			<< "mat = " << LOGDEL
//			<< floatMatrixToString(mat2d) 
//		);		
//		twoDArrayHandling<float> inv2d(matInv.dimension().first, matInv.dimension().second);
//		for (int ii=0; ii<inv2d.size(0);ii++) {
//			for (int jj=0; jj<inv2d.size(1);jj++) {
//        inv2d[ii][jj] = matInv(ii,jj);
//			}	
//		}
//		DTDEBUGWF(
//		  invertMatrix(), 
//			<< "inv = " << LOGDEL
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
			dt__THROW(makeOrdered(), << "Currently only supported for vectors with size 8.");
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
			dt__THROW(getGaussLegendre(),
							<< DTLOGEVAL(tt) << LOGDEL
							<< DTLOGEVAL(ww) << LOGDEL
							<< DTLOGEVAL(nPoints) );
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
}