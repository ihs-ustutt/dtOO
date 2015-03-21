#include "transIntCube.h"

#include <logMe/logMe.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
//#include <interfaceHeaven/twoDArrayHandling.h>

//#define __assignToVec(pN, sId, uu, vv, vec, vId) \
//    pN = _aS[sId]->getPointPercent(uu, vv); \
//    vec[0][vId] = pN.x(); vec[1][vId] = pN.y(); vec[2][vId] = pN.z()
//#define __assignToVec2(pN, vec, vId) \
//    vec[0][vId] = pN.x(); vec[1][vId] = pN.y(); vec[2][vId] = pN.z()
namespace dtOO {  
  transIntCube::transIntCube() {
  }

  transIntCube::transIntCube(
    map2dTo3d const * const aS0, 
    map2dTo3d const * const aS1,
    map2dTo3d const * const aS2, 
    map2dTo3d const * const aS3, 
    map2dTo3d const * const aS4,
    map2dTo3d const * const aS5
  ) {
    _aS.push_back(aS0->clone());
    _aS.push_back(aS1->clone());
    _aS.push_back(aS2->clone());
    _aS.push_back(aS3->clone());
    _aS.push_back(aS4->clone());
    _aS.push_back(aS5->clone());

    _aS_1_0_0 = _aS[1]->getPointPercentVector(0.,0.);
    _aS_1_0_1 = _aS[1]->getPointPercentVector(0.,1.);
    _aS_1_1_0 = _aS[1]->getPointPercentVector(1.,0.);
    _aS_1_1_1 = _aS[1]->getPointPercentVector(1.,1.);
    _aS_3_0_0 = _aS[3]->getPointPercentVector(0.,0.);
    _aS_3_0_1 = _aS[3]->getPointPercentVector(0.,1.);
    _aS_3_1_0 = _aS[3]->getPointPercentVector(1.,0.);
    _aS_3_1_1 = _aS[3]->getPointPercentVector(1.,1.);
                  
    DTINFOWF(transIntCube(),
      << "Creating transfinite interpolation with:" << LOGDEL
      << DTLOGEVAL(aS0->getLabel()) << LOGDEL
      << DTLOGEVAL(aS1->getLabel()) << LOGDEL
      << DTLOGEVAL(aS2->getLabel()) << LOGDEL
      << DTLOGEVAL(aS3->getLabel()) << LOGDEL
      << DTLOGEVAL(aS4->getLabel()) << LOGDEL
      << DTLOGEVAL(aS5->getLabel()) << LOGDEL
    );    
  } 

  transIntCube::transIntCube(const transIntCube& orig) {
    dt__forAllIndex(orig._aS, ii) _aS.push_back( orig._aS[ii]->clone() );
    _aS_1_0_0 = orig._aS_1_0_0;
    _aS_1_0_1 = orig._aS_1_0_1;
    _aS_1_1_0 = orig._aS_1_1_0;
    _aS_1_1_1 = orig._aS_1_1_1;
    _aS_3_0_0 = orig._aS_3_0_0;
    _aS_3_0_1 = orig._aS_3_0_1;
    _aS_3_1_0 = orig._aS_3_1_0;
    _aS_3_1_1 = orig._aS_3_1_1;
  }

  transIntCube::~transIntCube() {
    _aS.destroy();
  }
  
  dtPoint3 transIntCube::getValue(float const xx, float const yy, float const zz) const {
    dtVector3 rV =
        (1.-xx) * _aS[1]->getPointPercentVector(yy, zz) + xx * _aS[3]->getPointPercentVector(yy, zz)
      + (1.-yy) * _aS[4]->getPointPercentVector(xx, zz) + yy * _aS[5]->getPointPercentVector(xx, zz)
      + (1.-zz) * _aS[2]->getPointPercentVector(xx, yy) + zz * _aS[0]->getPointPercentVector(xx, yy)
      - (1.-xx) * ( (1.-yy) * _aS[1]->getPointPercentVector(0., zz) + yy * _aS[1]->getPointPercentVector(1., zz) )
      - xx      * ( (1.-yy) * _aS[3]->getPointPercentVector(0., zz) + yy * _aS[3]->getPointPercentVector(1., zz) )
      - (1.-yy) * ( (1.-zz) * _aS[4]->getPointPercentVector(xx, 0.) + zz * _aS[4]->getPointPercentVector(xx, 1.) )
      - yy      * ( (1.-zz) * _aS[5]->getPointPercentVector(xx, 0.) + zz * _aS[5]->getPointPercentVector(xx, 1.) )
      - (1.-zz) * ( (1.-xx) * _aS[2]->getPointPercentVector(0., yy) + xx * _aS[2]->getPointPercentVector(1., yy) )
      - zz      * ( (1.-xx) * _aS[0]->getPointPercentVector(0., yy) + xx * _aS[0]->getPointPercentVector(1., yy) )
      + (1.-xx) * ( 
          (1.-yy) * ( 
            (1.-zz) * _aS_1_0_0 + zz * _aS_1_0_1
          )
          +
          yy * ( 
            (1.-zz) * _aS_1_1_0 + zz * _aS_1_1_1
          )            
        )
      + xx * ( 
          (1.-yy) * ( 
            (1.-zz) * _aS_3_0_0 + zz * _aS_3_0_1
          )
          +
          yy * ( 
            (1.-zz) * _aS_3_1_0 + zz * _aS_3_1_1
          )            
        );            
    return dtLinearAlgebra::toDtPoint3( rV );
  }
  
//  dtPoint3 transIntCube::getValue(float const xx, float const yy, float const zz) const {
//    float xxN = 1.-xx;
//    float yyN = 1.-yy;
//    float zzN = 1.-zz;
//    
//    float pp[3][26];
//    
//    dtPoint3 pN;
//    __assignToVec(pN, 1, yy, zz, pp, 0);
//    __assignToVec(pN, 3, yy, zz, pp, 1);
//    __assignToVec(pN, 4, xx, zz, pp, 2);
//    __assignToVec(pN, 5, xx, zz, pp, 3);
//    __assignToVec(pN, 2, xx, yy, pp, 4);
//    __assignToVec(pN, 0, xx, yy, pp, 5);
//    
//    __assignToVec(pN, 1, 0., zz, pp, 6);
//    __assignToVec(pN, 1, 1., zz, pp, 7);
//    __assignToVec(pN, 3, 0., zz, pp, 8);
//    __assignToVec(pN, 3, 1., zz, pp, 9);
//    __assignToVec(pN, 4, xx, 0., pp, 10);
//    __assignToVec(pN, 4, xx, 1., pp, 11);
//    __assignToVec(pN, 5, xx, 0., pp, 12);
//    __assignToVec(pN, 5, xx, 1., pp, 13);
//    __assignToVec(pN, 2, 0., yy, pp, 14);
//    __assignToVec(pN, 2, 1., yy, pp, 15);
//    __assignToVec(pN, 0, 0., yy, pp, 16);
//    __assignToVec(pN, 0, 1., yy, pp, 17);    
//    
//    __assignToVec2(_aS_1_0_0, pp, 18);
//    __assignToVec2(_aS_1_0_1, pp, 19);
//    __assignToVec2(_aS_1_1_0, pp, 20);
//    __assignToVec2(_aS_1_1_1, pp, 21);
//    __assignToVec2(_aS_3_0_0, pp, 22);
//    __assignToVec2(_aS_3_0_1, pp, 23);
//    __assignToVec2(_aS_3_1_0, pp, 24);
//    __assignToVec2(_aS_3_1_1, pp, 25);
//
//    float rV[3];
//    for (int ii=0;ii<3;ii++) {
//      rV[ii] =
//          xxN * pp[ii][0] + xx * pp[ii][1]
//        + yyN * pp[ii][2] + yy * pp[ii][3]
//        + zzN * pp[ii][4] + zz * pp[ii][5]
//        - xxN * ( yyN * pp[ii][6] + yy * pp[ii][7] )
//        - xx      * ( yyN * pp[ii][8] + yy * pp[ii][9] )
//        - yyN * ( zzN * pp[ii][10] + zz * pp[ii][11] )
//        - yy      * ( zzN * pp[ii][12] + zz * pp[ii][13] )
//        - zzN * ( xxN * pp[ii][14] + xx * pp[ii][15] )
//        - zz      * ( xxN * pp[ii][16] + xx * pp[ii][17] )
//        + xxN * ( 
//            yyN * ( 
//              zzN * pp[ii][18] + zz * pp[ii][19]
//            )
//            +
//            yy * ( 
//              zzN * pp[ii][20] + zz * pp[ii][21]
//            )            
//          )
//        + xx * ( 
//            yyN * ( 
//              zzN * pp[ii][22] + zz * pp[ii][23]
//            )
//            +
//            yy * ( 
//              zzN * pp[ii][24] + zz * pp[ii][25]
//            )            
//          );            
//    }
//    return dtPoint3( rV[0], rV[1], rV[2] );
//  }
  
  vectorHandling< map2dTo3d const * > const & transIntCube::getConstRefToMap2dTo3d( void ) const {
    return _aS;
  }
}
