
#include <logMe/logMe.h>
#include "dtBmDevelopStuff.h"
#include <geometryEngine/dtSislCurve.h>
#include <geometryEngine/dtSislSurf.h>

namespace dtOO {
  DTCLASSLOGMETHODIN(dtBmDevelopStuff);

dtBmDevelopStuff::dtBmDevelopStuff() {
}

dtBmDevelopStuff::dtBmDevelopStuff(const dtBmDevelopStuff& orig) {
}

dtBmDevelopStuff::~dtBmDevelopStuff() {
}

std::vector<dtSurface *> dtBmDevelopStuff::createBlocks() {
    
    //BlockI
        dtPoint3 pointA(0.,0.,0.) ;
        dtPoint3 pointB(30.,0.,10.) ;
        dtPoint3 pointC(40.,-10.,-30.) ;
        dtPoint3 pointD(0.,0.,-30.) ;
        dtPoint3 pointE(0.,30.,0.) ;
        dtPoint3 pointF(30.,40.,0.) ;
        dtPoint3 pointG(30.,30.,-40.) ;
        dtPoint3 pointH(0.,30.,-30.) ;
        dtPoint3 pointaa(10.,0.,10.) ;
        dtPoint3 pointab(20.,0.,-10.) ;
        dtPoint3 pointbb(40.,20.,0.) ;
        dtPoint3 pointcc(50.,20.,-40.) ;
        dtPoint3 pointcd(20.,0.,-40.) ;
        dtPoint3 pointdd(10.,0.,-20.) ;
        dtPoint3 pointda(-10.,0.,-10.) ;
        dtPoint3 pointdh(-10.,10.,-30.) ;
        dtPoint3 pointhh(10.,20.,-30.) ;
        //BlockII
        dtPoint3 pointI(60.,0.,0.) ;
        dtPoint3 pointJ(60.,0.,-30.) ;
        dtPoint3 pointK(70.,30.,0.) ;
        dtPoint3 pointL(60.,30.,-40.) ;
        dtPoint3 pointbi(50.,10.,0.) ;
        dtPoint3 pointlj(50.,20.,-30.) ;
        dtPoint3 pointjl(70.,10.,-20.) ;
        dtPoint3 pointfk(40.,30.,-10.) ;
        dtPoint3 pointkf(50.,30.,10.) ;
        //BlockIII
        dtPoint3 pointM(30.,0.,-60.) ;
        dtPoint3 pointN(0.,0.,-60.) ;
        dtPoint3 pointO(30.,30.,-70.) ;
        dtPoint3 pointP(-10.,30.,-60.) ;
        dtPoint3 pointmn(20.,0.,-55.) ;
        dtPoint3 pointnm(10.,0.,-65.) ;
        //BlockIV
        dtPoint3 pointQ(-10.,60.,0.) ;
        dtPoint3 pointR(30.,60.,0.) ;
        dtPoint3 pointS(30.,70.,-40.) ;
        dtPoint3 pointT(0.,60.,-30.) ;
        dtPoint3 pointqr(10.,60.,-10.) ;
        dtPoint3 pointrq(20.,50.,0.) ;
        
        //BlockI
  std::vector<dtPoint3> pointVecA;
  std::vector<dtPoint3> pointVecB;
  std::vector<dtPoint3> pointVecC;
  std::vector<dtPoint3> pointVecD;
  std::vector<dtPoint3> pointVecE;
  std::vector<dtPoint3> pointVecF;
  std::vector<dtPoint3> pointVecG;
  std::vector<dtPoint3> pointVecH;
  std::vector<dtPoint3> pointVecI;
  std::vector<dtPoint3> pointVecJ;
  std::vector<dtPoint3> pointVecK;
  std::vector<dtPoint3> pointVecL;
  //BlockII
  std::vector<dtPoint3> pointVecM;
  std::vector<dtPoint3> pointVecN;
  std::vector<dtPoint3> pointVecO;
  std::vector<dtPoint3> pointVecP;
  std::vector<dtPoint3> pointVecQ;
  std::vector<dtPoint3> pointVecR;
  std::vector<dtPoint3> pointVecS;
  std::vector<dtPoint3> pointVecT;
  //BlockIII
  std::vector<dtPoint3> pointVecU;
  std::vector<dtPoint3> pointVecV;
  std::vector<dtPoint3> pointVecW;
  std::vector<dtPoint3> pointVecX;
  std::vector<dtPoint3> pointVecY;
  std::vector<dtPoint3> pointVecZ;
  std::vector<dtPoint3> pointVecAlpha;
  std::vector<dtPoint3> pointVecBeta;
  //BlockIV
  std::vector<dtPoint3> pointVecOne;
  std::vector<dtPoint3> pointVecTwo;
  std::vector<dtPoint3> pointVecThree;
  std::vector<dtPoint3> pointVecFour;
  std::vector<dtPoint3> pointVecFive;
  std::vector<dtPoint3> pointVecSix;
  std::vector<dtPoint3> pointVecSeven;
  std::vector<dtPoint3> pointVecEight;
  //BlockI
  std::vector<dtCurve const *> curveVecN;
  std::vector<dtCurve const *> curveVecS;
  std::vector<dtCurve const *> curveVecE;
  std::vector<dtCurve const *> curveVecW;
  std::vector<dtCurve const *> curveVecF;
  std::vector<dtCurve const *> curveVecB;
  //BlockII
  std::vector<dtCurve const *> curveVecNii;
  std::vector<dtCurve const *> curveVecSii;
  std::vector<dtCurve const *> curveVecEii;
  std::vector<dtCurve const *> curveVecWii;
  std::vector<dtCurve const *> curveVecFii;
  std::vector<dtCurve const *> curveVecBii;
  //BlockIII
  std::vector<dtCurve const *> curveVecNiii;
  std::vector<dtCurve const *> curveVecSiii;
  std::vector<dtCurve const *> curveVecEiii;
  std::vector<dtCurve const *> curveVecWiii;
  std::vector<dtCurve const *> curveVecFiii;
  std::vector<dtCurve const *> curveVecBiii;
  //BlockIV
  std::vector<dtCurve const *> curveVecNiv;
  std::vector<dtCurve const *> curveVecSiv;
  std::vector<dtCurve const *> curveVecEiv;
  std::vector<dtCurve const *> curveVecWiv;
  std::vector<dtCurve const *> curveVecFiv;
  std::vector<dtCurve const *> curveVecBiv;
  
  //Sort Points for Curves
  //BlockI
  pointVecA.push_back(pointA);
  pointVecA.push_back(pointaa);
  pointVecA.push_back(pointab);
  pointVecA.push_back(pointB);
  
  pointVecB.push_back(pointD);
  pointVecB.push_back(pointC);
  
  pointVecC.push_back(pointH);
  pointVecC.push_back(pointG);
  
  pointVecD.push_back(pointE);
  pointVecD.push_back(pointF);
  
  pointVecE.push_back(pointA);
  pointVecE.push_back(pointD);
  
  pointVecF.push_back(pointB);
  pointVecF.push_back(pointC);
  
  pointVecG.push_back(pointF);
  pointVecG.push_back(pointG);
  
  pointVecH.push_back(pointE);
  pointVecH.push_back(pointH);
  
  pointVecI.push_back(pointA);
  pointVecI.push_back(pointE);
  
  pointVecJ.push_back(pointB);
  pointVecJ.push_back(pointF);
  
  pointVecK.push_back(pointC);
  //pointVecK.push_back(pointcc);
  pointVecK.push_back(pointG);
  
  pointVecL.push_back(pointD);
  //pointVecL.push_back(pointdh);
  //pointVecL.push_back(pointhh);
  pointVecL.push_back(pointH);
  
  //BlockII
  pointVecM.push_back(pointB);
  pointVecM.push_back(pointbi);
  pointVecM.push_back(pointI);
  
  pointVecN.push_back(pointC);
  pointVecN.push_back(pointJ);
  
  pointVecO.push_back(pointG);
  pointVecO.push_back(pointL);
  
  pointVecP.push_back(pointF);
  pointVecP.push_back(pointfk);
  pointVecP.push_back(pointkf);
  pointVecP.push_back(pointK);
  
  pointVecQ.push_back(pointI);
  pointVecQ.push_back(pointJ);
  
  pointVecR.push_back(pointK);
  pointVecR.push_back(pointL);
  
  pointVecS.push_back(pointI);
  pointVecS.push_back(pointK);
  
  pointVecT.push_back(pointJ);
//  pointVecT.push_back(pointjl);
//  pointVecT.push_back(pointlj);
  pointVecT.push_back(pointL);
  
  //BlockIII
  pointVecU.push_back(pointN);
  pointVecU.push_back(pointnm);
  pointVecU.push_back(pointmn);
  pointVecU.push_back(pointM);
  
  pointVecV.push_back(pointP);
  pointVecV.push_back(pointO);
  
  pointVecW.push_back(pointD);
  pointVecW.push_back(pointN);
  
  pointVecX.push_back(pointC);
  pointVecX.push_back(pointM);
  
  pointVecY.push_back(pointG);
  pointVecY.push_back(pointO);
  
  pointVecZ.push_back(pointH);
  pointVecZ.push_back(pointP);
  
  pointVecAlpha.push_back(pointM);
  pointVecAlpha.push_back(pointO);
  
  pointVecBeta.push_back(pointN);
  pointVecBeta.push_back(pointP);
  
  //BlockIV
  pointVecOne.push_back(pointT);
  pointVecOne.push_back(pointS);
  
  pointVecTwo.push_back(pointQ);
  pointVecTwo.push_back(pointqr);
  pointVecTwo.push_back(pointrq);
  pointVecTwo.push_back(pointR);
  
  pointVecThree.push_back(pointR);
  pointVecThree.push_back(pointS);
  
  pointVecFour.push_back(pointQ);
  pointVecFour.push_back(pointT);
  
  pointVecFive.push_back(pointE);
  pointVecFive.push_back(pointQ);
  
  pointVecSix.push_back(pointF);
  pointVecSix.push_back(pointR);
  
  pointVecSeven.push_back(pointG);
  pointVecSeven.push_back(pointS);
  
  pointVecEight.push_back(pointH);
  pointVecEight.push_back(pointT);
  
  
  //fill CurveVectors for SurfaceCreation
  //BlockI
  curveVecN.push_back(new dtSislCurve(&pointVecD, 2));
  curveVecN.push_back(new dtSislCurve(&pointVecC, 2));
  
  curveVecS.push_back(new dtSislCurve(&pointVecA, 4));
  curveVecS.push_back(new dtSislCurve(&pointVecB, 2));
  
  curveVecW.push_back(new dtSislCurve(&pointVecE, 2));
  curveVecW.push_back(new dtSislCurve(&pointVecH, 2));
  
  curveVecE.push_back(new dtSislCurve(&pointVecF, 2));
  curveVecE.push_back(new dtSislCurve(&pointVecG, 2));
  
  curveVecF.push_back(new dtSislCurve(&pointVecA, 4));
  curveVecF.push_back(new dtSislCurve(&pointVecD, 2));
  
  curveVecB.push_back(new dtSislCurve(&pointVecB, 2));
  curveVecB.push_back(new dtSislCurve(&pointVecC, 2));
  
  //BlockII
  curveVecNii.push_back(new dtSislCurve(&pointVecP, 4));
  curveVecNii.push_back(new dtSislCurve(&pointVecO, 2));
  
  curveVecSii.push_back(new dtSislCurve(&pointVecM, 3));
  curveVecSii.push_back(new dtSislCurve(&pointVecN, 2));
  
  curveVecWii.push_back(new dtSislCurve(&pointVecF, 2));
  curveVecWii.push_back(new dtSislCurve(&pointVecG, 2));
  
  curveVecEii.push_back(new dtSislCurve(&pointVecQ, 2));
  curveVecEii.push_back(new dtSislCurve(&pointVecR, 2));
    
  curveVecFii.push_back(new dtSislCurve(&pointVecM, 3));
  curveVecFii.push_back(new dtSislCurve(&pointVecP, 4));
  
  curveVecBii.push_back(new dtSislCurve(&pointVecN, 2));
  curveVecBii.push_back(new dtSislCurve(&pointVecO, 2));
  
  //BlockIII
  curveVecNiii.push_back(new dtSislCurve(&pointVecC, 2));
  curveVecNiii.push_back(new dtSislCurve(&pointVecV, 2));
  
  curveVecSiii.push_back(new dtSislCurve(&pointVecB, 2));
  curveVecSiii.push_back(new dtSislCurve(&pointVecU, 4));
  
  curveVecWiii.push_back(new dtSislCurve(&pointVecW, 2));
  curveVecWiii.push_back(new dtSislCurve(&pointVecZ, 2));
  
  curveVecEiii.push_back(new dtSislCurve(&pointVecX, 2));
  curveVecEiii.push_back(new dtSislCurve(&pointVecY, 2));
    
  curveVecFiii.push_back(new dtSislCurve(&pointVecB, 2));
  curveVecFiii.push_back(new dtSislCurve(&pointVecC, 2));
  
  curveVecBiii.push_back(new dtSislCurve(&pointVecU, 4));
  curveVecBiii.push_back(new dtSislCurve(&pointVecV, 2));
  
  //BlockIV
  curveVecNiv.push_back(new dtSislCurve(&pointVecTwo, 4));
  curveVecNiv.push_back(new dtSislCurve(&pointVecOne, 2));
  
  curveVecSiv.push_back(new dtSislCurve(&pointVecD, 2));
  curveVecSiv.push_back(new dtSislCurve(&pointVecC, 2));
  
  curveVecWiv.push_back(new dtSislCurve(&pointVecH, 2));
  curveVecWiv.push_back(new dtSislCurve(&pointVecFour, 2));
  
  curveVecEiv.push_back(new dtSislCurve(&pointVecG, 2));
  curveVecEiv.push_back(new dtSislCurve(&pointVecThree, 2));
    
  curveVecFiv.push_back(new dtSislCurve(&pointVecD, 2));
  curveVecFiv.push_back(new dtSislCurve(&pointVecTwo, 4));
  
  curveVecBiv.push_back(new dtSislCurve(&pointVecC, 2));
  curveVecBiv.push_back(new dtSislCurve(&pointVecOne, 2));
  
      
   //create BlockSurfaces:
  std::vector<dtSurface *> blockSurf;
  //std::vector<dtSurface *> blockSurfII;
  //BlockI
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecN);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecS);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecW);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecE);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecF);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecB);
// BlockII
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecNii);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecSii);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecWii);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecEii);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecFii);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecBii);
  // BlockIII
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecNiii);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecSiii);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecWiii);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecEiii);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecFiii);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecBiii);
  // BlockIV
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecNiv);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecSiv);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecWiv);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecEiv);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecFiv);
  blockSurf.push_back( new dtSislSurf() );
  blockSurf.back()->setLoftDirectionOrder(2);
  blockSurf.back()->createMe(&curveVecBiv);
  
  return blockSurf;
  
}

std::vector<dtCurve *> dtBmDevelopStuff::createSurf() {
    
        dtPoint3 pointA(0.,0.,0.) ;
        dtPoint3 pointB(30.,0.,10.) ;
        dtPoint3 pointC(40.,-10.,-30.) ;
        dtPoint3 pointD(0.,0.,-30.) ;
        dtPoint3 pointaa(10.,0.,10.) ;
        dtPoint3 pointab(20.,0.,-10.) ;
        dtPoint3 pointcd(20.,0.,-40.) ;
        dtPoint3 pointdd(10.,0.,-20.) ;
        dtPoint3 pointda(-10.,0.,-10.) ;
        
        std::vector<dtPoint3> pointVecAB;
        std::vector<dtPoint3> pointVecBC;
        std::vector<dtPoint3> pointVecDC;
        std::vector<dtPoint3> pointVecAD;
  
        pointVecAB.push_back(pointA);
        pointVecAB.push_back(pointaa);
        pointVecAB.push_back(pointab);
        pointVecAB.push_back(pointB);
        
        pointVecBC.push_back(pointB);
        pointVecBC.push_back(pointC);
        
        pointVecDC.push_back(pointD);
        pointVecDC.push_back(pointcd);
        pointVecDC.push_back(pointC);
        
        pointVecAD.push_back(pointA);
        pointVecAD.push_back(pointda);
        pointVecAD.push_back(pointdd);
        pointVecAD.push_back(pointD);
        
    
    std::vector<dtCurve *> curveVecSurf;
    
    curveVecSurf.push_back(new dtSislCurve(&pointVecAB, 4));
    curveVecSurf.push_back(new dtSislCurve(&pointVecBC, 2));
    curveVecSurf.push_back(new dtSislCurve(&pointVecDC, 3));
    curveVecSurf.push_back(new dtSislCurve(&pointVecAD, 4));
    
    return curveVecSurf;
    
}

}

