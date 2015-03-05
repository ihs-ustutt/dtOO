
#include <logMe/logMe.h>
#include "dtBmGrid.h"
#include <baseContainer/pointContainer.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <do/coDoUnstructuredGrid.h>
#include <do/coDoStructuredGrid.h>
#include <do/coDoSet.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>
#include <analyticFunctionHeaven/scaFunction.h>


#define FALSE 0
#define TRUE 1

namespace dtOO {
    DTCLASSLOGMETHODIN(dtBmGrid);
//    std::ostream& operator<<(std::ostream& oo, const dtBmGrid& thisRef) { 
//    return oo << " " << LOGDEL 
//          << " << _internalState_ >> " << LOGDEL 
////          << thisRef.getClassName() << LOGDEL 
//          << "Not yet implemented."; 
//  }

dtBmGrid::dtBmGrid() {
}

dtBmGrid::dtBmGrid(const dtBmGrid& orig) {
}

dtBmGrid::~dtBmGrid() {

}


dtBmGrid::dtBmGrid(string const str, std::vector<dtSurface *> const blockSurf, 
                   int const nx, int const ny, int const nz, coDoSetHandling< scaFunction *> const * const sFunP) {
    
   
    
    _nx = nx;
    _ny = ny;
    _nz = nz;
    _gridPoints = createPoints(blockSurf, _nx, _ny, _nz, sFunP);
    
};

dtBmGrid::dtBmGrid(std::vector<dtCurve *> const surfEdges, 
                   int const nx, int const ny) {
    
    _surfGridPoints = createPoints(surfEdges, nx, ny);
    _nx = nx;
    _ny = ny;
    
    
    
};



std::vector< std::pair<dtVector3 , int> > dtBmGrid::createPoints(std::vector<dtSurface *> const blockSurf, 
                                                                int const nx, int const ny, int const nz, 
                                                                coDoSetHandling< scaFunction *> const * const sFunP) {
    dtPoint3 orig (0.,0.,0.); 
//    int nx = static_cast<int> ((1/xRes) + 1.5);
//    int ny = static_cast<int> ((1/yRes) + 1.5);
//    int nz = static_cast<int> ((1/zRes) + 1.5);
    float nxFloat = static_cast<float>(nx);
    float nyFloat = static_cast<float>(ny);
    float nzFloat = static_cast<float>(nz);
    float xRes = 1/(nxFloat-1);
    float yRes = 1/(nyFloat-1);
    float zRes = 1/(nzFloat-1);
    float aa = 1/( (nxFloat-1)*(nxFloat-1) );
    float xPos = 0.;
    float yPos = 0.;
    float zPos = 0.;
    float xx = 0.;
    float yy = 0.;
    float zz = 0.;
    std::vector<float> gradVecX;
    std::vector<float> gradVecY;
    std::vector<float> gradVecZ;
    std::vector< std::pair<dtVector3 , int> > gridPoints;
    bool surfSouth ;
    bool surfNorth ;
    bool surfWest ;
    bool surfEast ;
    bool surfFront ;
    bool surfBack ;
    int ownerIndex ;
    
    
    //grading-function f(u) has to be defined between u=0, u=1 and f(u)=0, f(u)=1.
    for (int ii = 0 ; ii<(nx-1) ; ii++) {
//      xPos = aa * (ii*ii);   
      xPos = sFunP->get("f_x_grading")->getValue( (ii/(nxFloat-1)) );
      gradVecX.push_back(xPos);
    }
    gradVecX.push_back(1.);
    
    for (int ii = 0 ; ii<(ny-1) ; ii++) {
      yPos = sFunP->get("f_y_grading")->getValue( (ii/(nyFloat-1)) );
      gradVecY.push_back(yPos);
    }
    gradVecY.push_back(1.);
    
    for (int ii = 0 ; ii<(nz-1) ; ii++) {
      zPos = sFunP->get("f_z_grading")->getValue( (ii/(nzFloat-1)) );
      gradVecZ.push_back(zPos);
//      zPos += zRes;
    }
    gradVecZ.push_back(1.);
  
  // searching and writing knotPoints:    
    for (int iz = 0 ; iz<gradVecZ.size() ; iz++) {
      
      zz = gradVecZ.at(iz);
            
    for (int iy = 0 ; iy<gradVecY.size() ; iy++) {
      
      yy = gradVecY.at(iy);
            
    for (int ix = 0 ; ix<gradVecX.size() ; ix++) {
      xx = gradVecX.at(ix);
      
      //BM2, u/v 09.10.13 MS (NICHT Standard):
//  dtVector3 surfPointN ( orig, blockSurf.at(0)->getPointPercent3d(xx, yy) );
//  dtVector3 surfPointS ( orig, blockSurf.at(1)->getPointPercent3d(xx, yy) );
//  dtVector3 surfPointE ( orig, blockSurf.at(2)->getPointPercent3d(zz, yy) );
//  dtVector3 surfPointW ( orig, blockSurf.at(3)->getPointPercent3d(zz, yy) );
//  dtVector3 surfPointF ( orig, blockSurf.at(4)->getPointPercent3d(xx, zz) );
//  dtVector3 surfPointB ( orig, blockSurf.at(5)->getPointPercent3d(zz, xx) );
  
//  dtVector3 EdgePointA ( orig, blockSurf.at(1)->getPointPercent3d(xx,0) );
//  dtVector3 EdgePointB ( orig, blockSurf.at(1)->getPointPercent3d(xx,1) );
//  dtVector3 EdgePointC ( orig, blockSurf.at(0)->getPointPercent3d(xx,1) );
//  dtVector3 EdgePointD ( orig, blockSurf.at(0)->getPointPercent3d(xx,0) );
//  dtVector3 EdgePointE ( orig, blockSurf.at(1)->getPointPercent3d(0,yy) );
//  dtVector3 EdgePointF ( orig, blockSurf.at(1)->getPointPercent3d(1,yy) );
//  dtVector3 EdgePointG ( orig, blockSurf.at(0)->getPointPercent3d(1,yy) );
//  dtVector3 EdgePointH ( orig, blockSurf.at(0)->getPointPercent3d(0,yy) );
//  dtVector3 EdgePointI ( orig, blockSurf.at(3)->getPointPercent3d(zz,0) );
//  dtVector3 EdgePointJ ( orig, blockSurf.at(2)->getPointPercent3d(zz,0) );
//  dtVector3 EdgePointK ( orig, blockSurf.at(2)->getPointPercent3d(zz,1) );
//  dtVector3 EdgePointL ( orig, blockSurf.at(3)->getPointPercent3d(zz,1) );
//  
//  dtVector3 pointAVec ( orig, blockSurf.at(1)->getPointPercent3d(0,0) );
//  dtVector3 pointBVec ( orig, blockSurf.at(1)->getPointPercent3d(1,0) );
//  dtVector3 pointCVec ( orig, blockSurf.at(1)->getPointPercent3d(1,1) );
//  dtVector3 pointDVec ( orig, blockSurf.at(1)->getPointPercent3d(0,1) );
//  dtVector3 pointEVec ( orig, blockSurf.at(0)->getPointPercent3d(0,0) );
//  dtVector3 pointFVec ( orig, blockSurf.at(0)->getPointPercent3d(1,0) );
//  dtVector3 pointGVec ( orig, blockSurf.at(0)->getPointPercent3d(1,1) );
//  dtVector3 pointHVec ( orig, blockSurf.at(0)->getPointPercent3d(0,1) );
      
  //BM2, u/v Standard:
  dtVector3 surfPointN ( orig, blockSurf.at(0)->getPointPercent3d(xx, yy) );
  dtVector3 surfPointS ( orig, blockSurf.at(1)->getPointPercent3d(xx, yy) );
  dtVector3 surfPointE ( orig, blockSurf.at(2)->getPointPercent3d(yy, zz) );
  dtVector3 surfPointW ( orig, blockSurf.at(3)->getPointPercent3d(yy, zz) );
  dtVector3 surfPointF ( orig, blockSurf.at(4)->getPointPercent3d(xx, zz) );
  dtVector3 surfPointB ( orig, blockSurf.at(5)->getPointPercent3d(xx, zz) );
  
  dtVector3 EdgePointA ( orig, blockSurf.at(1)->getPointPercent3d(xx,0) );
  dtVector3 EdgePointB ( orig, blockSurf.at(1)->getPointPercent3d(xx,1) );
  dtVector3 EdgePointC ( orig, blockSurf.at(0)->getPointPercent3d(xx,1) );
  dtVector3 EdgePointD ( orig, blockSurf.at(0)->getPointPercent3d(xx,0) );
  dtVector3 EdgePointE ( orig, blockSurf.at(1)->getPointPercent3d(0,yy) );
  dtVector3 EdgePointF ( orig, blockSurf.at(1)->getPointPercent3d(1,yy) );
  dtVector3 EdgePointG ( orig, blockSurf.at(0)->getPointPercent3d(1,yy) );
  dtVector3 EdgePointH ( orig, blockSurf.at(0)->getPointPercent3d(0,yy) );
  dtVector3 EdgePointI ( orig, blockSurf.at(3)->getPointPercent3d(0,zz) );
  dtVector3 EdgePointJ ( orig, blockSurf.at(2)->getPointPercent3d(0,zz) );
  dtVector3 EdgePointK ( orig, blockSurf.at(2)->getPointPercent3d(1,zz) );
  dtVector3 EdgePointL ( orig, blockSurf.at(3)->getPointPercent3d(1,zz) );
  
  dtVector3 pointAVec ( orig, blockSurf.at(1)->getPointPercent3d(0,0) );
  dtVector3 pointBVec ( orig, blockSurf.at(1)->getPointPercent3d(1,0) );
  dtVector3 pointCVec ( orig, blockSurf.at(1)->getPointPercent3d(1,1) );
  dtVector3 pointDVec ( orig, blockSurf.at(1)->getPointPercent3d(0,1) );
  dtVector3 pointEVec ( orig, blockSurf.at(0)->getPointPercent3d(0,0) );
  dtVector3 pointFVec ( orig, blockSurf.at(0)->getPointPercent3d(1,0) );
  dtVector3 pointGVec ( orig, blockSurf.at(0)->getPointPercent3d(1,1) );
  dtVector3 pointHVec ( orig, blockSurf.at(0)->getPointPercent3d(0,1) );
  
  dtVector3 tmpInternalPointVec;
  
  tmpInternalPointVec = yy*surfPointB + (1-yy)*surfPointF + (1-xx)*surfPointW + xx*surfPointE
               + (1-zz)*surfPointS + zz*surfPointN
               - yy* ((1-xx)*EdgePointL + xx*EdgePointK) - (1-yy)* ((1-xx)*EdgePointI + xx*EdgePointJ)
               - (1-xx)* ((1-zz)*EdgePointE + zz*EdgePointH) - xx* ((1-zz)*EdgePointF + zz*EdgePointG)
               - (1-zz)* (yy*EdgePointB + (1-yy)*EdgePointA) - zz* (yy*EdgePointC + (1-yy)*EdgePointD)
    + yy* ((1-xx)*((1-zz)*pointDVec + zz*pointHVec)
               + xx*((1-zz)*pointCVec + zz*pointGVec))
    + (1-yy)*     ((1-xx)*((1-zz)*pointAVec + zz*pointEVec)
              + xx*((1-zz)*pointBVec + zz*pointFVec)); 
  
  ownerIndex = 0;
  surfSouth = FALSE;
  surfNorth = FALSE;
  surfWest = FALSE;
  surfEast = FALSE;
  surfFront = FALSE;
  surfBack = FALSE;
  
  if ( ix == 0 ) {
      surfWest = TRUE;
      ownerIndex = 3;
  }
  if ( ix == (gradVecX.size()-1) ) {
      surfEast = TRUE;
      ownerIndex = 4;
  }
  if ( iy == 0 ) {
      surfFront = TRUE;
      ownerIndex = 5;
  }
  if ( iy == (gradVecY.size()-1) ) {
      surfBack = TRUE;
      ownerIndex = 6;
  }
  if ( iz == 0 ) {
      surfSouth = TRUE;
      ownerIndex = 1;
  }
  if ( iz == (gradVecZ.size()-1) ) {
      surfNorth = TRUE;
      ownerIndex = 2;
  }
  if ( surfSouth && surfWest ) {
      ownerIndex = 13;
  }
  if ( surfSouth && surfEast ) {
      ownerIndex = 14;
  }
  if ( surfSouth && surfFront ) {
      ownerIndex = 15;
  }
  if ( surfSouth && surfBack ) {
      ownerIndex = 16;
  }
  if ( surfNorth && surfWest ) {
      ownerIndex = 23;
  }
  if ( surfNorth && surfEast ) {
      ownerIndex = 24;
  }
  if ( surfNorth && surfFront ) {
      ownerIndex = 25;
  }
  if ( surfNorth && surfBack ) {
      ownerIndex = 26;
  }
  if ( surfWest && surfFront ) {
      ownerIndex = 35;
  }
  if ( surfWest && surfBack ) {
      ownerIndex = 36;
  }
  if ( surfEast && surfFront ) {
      ownerIndex = 45;
  }
  if ( surfEast && surfBack ) {
      ownerIndex = 46;
  }
  if ( surfSouth && surfWest && surfFront ) {
      ownerIndex = 135;
  }
  if ( surfSouth && surfWest && surfBack ) {
      ownerIndex = 136;
  }
  if ( surfSouth && surfEast && surfFront ) {
      ownerIndex = 145;
  }
  if ( surfSouth && surfEast && surfBack ) {
      ownerIndex = 146;
  }
  if ( surfNorth && surfWest && surfFront ) {
      ownerIndex = 235;
  }
  if ( surfNorth && surfWest && surfBack ) {
      ownerIndex = 236;
  }
  if ( surfNorth && surfEast && surfFront ) {
      ownerIndex = 245;
  }
  if ( surfNorth && surfEast && surfBack ) {
      ownerIndex = 246;
  }
  
  gridPoints.push_back( std::make_pair(tmpInternalPointVec , ownerIndex) );
    
  
  }
  
  }
  
  }
  
    return gridPoints;
};


std::vector< dtPoint3 > dtBmGrid::createPoints(std::vector<dtCurve *> const surfEdges, 
                                                                int const nx, int const ny){
dtPoint3 orig (0.,0.,0.); 

    float nxFloat = static_cast<float>(nx);
    float nyFloat = static_cast<float>(ny);
    float xRes = 1/(nxFloat-1);
    float yRes = 1/(nyFloat-1); 
    float xPos = 0.;
    float yPos = 0.;
    float xx = 0.;
    float yy = 0.;
    std::vector<float> gradVecX;
    std::vector<float> gradVecY;
    std::vector<dtPoint3> gridPoints;
    
    for (int ii = 0 ; ii<nx ; ii++) {
      gradVecX.push_back(xPos);
      xPos += xRes;
    }
    for (int ii = 0 ; ii<ny ; ii++) {
      gradVecY.push_back(yPos);
      yPos += yRes;
    }
    
  
  // searching and writing Points:    
            
    for (int iy = 0 ; iy<gradVecY.size() ; iy++) {
      
      yy = gradVecY.at(iy);
            
    for (int ix = 0 ; ix<gradVecX.size() ; ix++) {
      xx = gradVecX.at(ix);
      
     
  
  dtVector3 EdgePointAB ( orig, surfEdges.at(0)->getPointPercent3d(xx) );
  dtVector3 EdgePointBC ( orig, surfEdges.at(1)->getPointPercent3d(yy) );
  dtVector3 EdgePointDC ( orig, surfEdges.at(2)->getPointPercent3d(xx) );
  dtVector3 EdgePointAD ( orig, surfEdges.at(3)->getPointPercent3d(yy) );

 
  dtVector3 pointA ( orig, surfEdges.at(0)->getPointPercent3d(0) );
  dtVector3 pointB ( orig, surfEdges.at(0)->getPointPercent3d(1) );
  dtVector3 pointC ( orig, surfEdges.at(2)->getPointPercent3d(1) );
  dtVector3 pointD ( orig, surfEdges.at(2)->getPointPercent3d(0) );

  
  dtVector3 tmpInternalPointVec;
  
  tmpInternalPointVec = (1-xx)*EdgePointAD + xx*EdgePointBC
                        + (1-yy)*EdgePointAB + yy*EdgePointDC
                        -( (1-xx)*(1-yy)*pointA + xx*(1-yy)*pointB
                           + xx*yy*pointC + (1-xx)*yy*pointD);
 
  dtPoint3 tmpIntPoint;
  tmpIntPoint = orig + tmpInternalPointVec;
gridPoints.push_back( tmpIntPoint );
    
  
  }
  
  }
  
  
    return gridPoints;  
}


covise::coDoStructuredGrid * dtBmGrid::toCoDoStruct(string const str) const {

    float *XCoord, *YCoord, *ZCoord;
    coDoStructuredGrid * myStructGrid = new coDoStructuredGrid(str, _nx, _ny, _nz);
    myStructGrid->getAddresses(&XCoord, &YCoord, &ZCoord);
    int ii=0;

        for (int kk=0; kk<_nz; kk++) {
            for (int jj=0; jj<_ny; jj++) {
                for (int nn=0; nn<_nx; nn++) {
                        *( XCoord+(nn*_ny*_nz+jj*_nz+kk) ) = _gridPoints.at(ii).first.x();
                        *( YCoord+(nn*_ny*_nz+jj*_nz+kk) ) = _gridPoints.at(ii).first.y();
                        *( ZCoord+(nn*_ny*_nz+jj*_nz+kk) ) = _gridPoints.at(ii).first.z();
                        ii++;
                }
            }
        }

//    float xc, yc, zc;
//    myStructGrid->getPointCoordinates(5, &xc,0,&yc ,0,&zc);
//                DTINFOWF(structGrid(), 
//                    << DTLOGEVAL(xc) << LOGDEL
//                    << DTLOGEVAL(yc) << LOGDEL
//                    << DTLOGEVAL(zc) << LOGDEL
//                 
//           );
    
    return myStructGrid;
    
}


covise::coDoUnstructuredGrid * dtBmGrid::toCoDoUns(string const str) const {
    
        int nelem = (_nx-1)*(_ny-1)*(_nz-1);
	int nconn = 8*nelem;
	int ht = 1;

        
	int *outElemList, *outConnList, *outTypeList;
	float *outXCoord, *outYCoord, *outZCoord;
        //int i;
        

	coDoUnstructuredGrid *outGrid = new coDoUnstructuredGrid(str, nelem, nconn, _gridPoints.size(), ht);

	outGrid->getAddresses(&outElemList, &outConnList, &outXCoord, &outYCoord, &outZCoord);
	outGrid->getTypeList(&outTypeList);
        
        for (int ii=0;ii<nelem;ii++)
        {
                *outElemList = 8 * ii;
                outElemList++;
                
                *outTypeList = TYPE_HEXAGON;
                outTypeList++;
        }
        
        int itNyCount = 0;
        int itNxCount = 0;
        for (int ii=0;ii<( (_gridPoints.size())-1-(_nx*_ny)-_nx );ii++)
        {
//                *outConnList = connect [i];
//                outConnList++;
        if (ii == ((_ny-1)*_nx-1)+itNyCount * (_nx*_ny)) {
                itNyCount++;
                ii += _nx;
                itNxCount += 2 ;
        }
        else {
            if (ii == ( (_nx-1) + itNxCount * _nx ) ) {
            //ii++;
            itNxCount++;
            }
            else {
            *(outConnList+0) = ii;
            *(outConnList+1) = ii+1;
            *(outConnList+2) = ii+_nx+1;
            *(outConnList+3) = ii+_nx;
            *(outConnList+4) = ii+(_nx*_ny);
            *(outConnList+5) = ii+(_nx*_ny)+1;
            *(outConnList+6) = ii+(_nx*_ny)+_nx+1;
            *(outConnList+7) = ii+(_nx*_ny)+_nx;
            
//            DTINFOWF(connList(), 
//                    << DTLOGEVAL(*(outConnList+0)) << LOGDEL
//                    << DTLOGEVAL(*(outConnList+1)) << LOGDEL
//                    << DTLOGEVAL(*(outConnList+2)) << LOGDEL
//                    << DTLOGEVAL(*(outConnList+3)) << LOGDEL
//                    << DTLOGEVAL(*(outConnList+4)) << LOGDEL
//                    << DTLOGEVAL(*(outConnList+5)) << LOGDEL
//                    << DTLOGEVAL(*(outConnList+6)) << LOGDEL
//                    << DTLOGEVAL(*(outConnList+7)) << LOGDEL
//           );
            
            outConnList += 8;
            }
                
        }
	}
        
        for (int ii=0;ii<_gridPoints.size();ii++)
        {
                *outXCoord = _gridPoints.at(ii).first.x();
                outXCoord++;
                *outYCoord = _gridPoints.at(ii).first.y();
                outYCoord++;
                *outZCoord = _gridPoints.at(ii).first.z();
                outZCoord++;
        }
 
        return outGrid;
};

covise::coDoStructuredGrid * dtBmGrid::coupleStructBlocks(string const str,
                                                          coDoStructuredGrid * const gridI,
                                                          coDoStructuredGrid * const gridII, int const cm) const{
    
    int nxI, nyI, nzI;
    int nxII, nyII, nzII;
    
    gridI->getGridSize(&nxI, &nyI, &nzI);
    gridII->getGridSize(&nxII, &nyII, &nzII);
    float *XCoord, *YCoord, *ZCoord;
    float *XCoordI, *YCoordI, *ZCoordI;
    float *XCoordII, *YCoordII, *ZCoordII;
    gridI->getAddresses(&XCoordI, &YCoordI, &ZCoordI);
    gridII->getAddresses(&XCoordII, &YCoordII, &ZCoordII);
    coDoStructuredGrid * coupleStructGrid;
    
    switch (cm)
    {  
        case 1://*************east-west************

            if (nyI!=nyII || nzI!=nzII)
            {
                dt__THROW(coupleStructBlocks(),
                        << "Number of Knots in east-west coupling not corresponding" << LOGDEL
                        << DTLOGEVAL(nyI) << LOGDEL
                        << DTLOGEVAL(nyII) << LOGDEL
                        << DTLOGEVAL(nzI) << LOGDEL
                        << DTLOGEVAL (nzII)
                        );

                
            }
    coupleStructGrid = new coDoStructuredGrid(str, (nxI+nxII-1), nyI, nzI ); //eastwest
    coupleStructGrid->getAddresses(&XCoord, &YCoord, &ZCoord);
    for (int ii=0; ii<nxI*nyI*nzI; ii++)
    {
        *XCoord = *(XCoordI+ii);
        XCoord++;
        *YCoord = *(YCoordI+ii);
        YCoord++;
        *ZCoord = *(ZCoordI+ii);
        ZCoord++;
    }
    
    for (int ii=(nyII*nzII); ii<nxII*nyII*nzII; ii++)
    {
        *XCoord = *(XCoordII+ii);
        XCoord++;
        *YCoord = *(YCoordII+ii);
        YCoord++;
        *ZCoord = *(ZCoordII+ii);
        ZCoord++;

    }
    break; //*********ende east-west**********
    
        case 2: //*******back-front**********
            
            if (nxI!=nxII || nzI!=nzII)
            {
                dt__THROW(coupleStructBlocks(),
                        << "Number of Knots in back-front coupling not corresponding" << LOGDEL
                        << DTLOGEVAL(nxI) << LOGDEL
                        << DTLOGEVAL(nxII) << LOGDEL
                        << DTLOGEVAL(nzI) << LOGDEL
                        << DTLOGEVAL (nzII)
                        );
            }
    coupleStructGrid = new coDoStructuredGrid(str, nxI, (nyI+nyII-1), nzI ); //backfront
    coupleStructGrid->getAddresses(&XCoord, &YCoord, &ZCoord);
    for (int kk=0; kk<nxI; kk++)
    {
    for (int ii=0; ii<nyI*nzI; ii++)
    {
        *XCoord = *(XCoordI+ii+kk*nyI*nzI);
        XCoord++;
        *YCoord = *(YCoordI+ii+kk*nyI*nzI);
        YCoord++;
        *ZCoord = *(ZCoordI+ii+kk*nyI*nzI);
        ZCoord++;
    }
    for (int ii=nzII; ii<nyII*nzII; ii++)
    {
        *XCoord = *(XCoordII+ii+kk*nyII*nzII);
        XCoord++;
        *YCoord = *(YCoordII+ii+kk*nyII*nzII);
        YCoord++;
        *ZCoord = *(ZCoordII+ii+kk*nyII*nzII);
        ZCoord++;
    }
    }
    break; //*******ende back-front*****
    
        case 3: //********north-south********
            
            if (nxI!=nxII || nyI!=nyII)
            {
                dt__THROW(coupleStructBlocks(),
                        << "Number of Knots in north-south coupling not corresponding" << LOGDEL
                        << DTLOGEVAL(nxI) << LOGDEL
                        << DTLOGEVAL(nxII) << LOGDEL
                        << DTLOGEVAL(nyI) << LOGDEL
                        << DTLOGEVAL (nyII)
                        );
            }
    coupleStructGrid = new coDoStructuredGrid(str, nxI, nyI, (nzI+nzII-1) ); //northsouth
    coupleStructGrid->getAddresses(&XCoord, &YCoord, &ZCoord);
    for (int ll=0; ll<nxI; ll++)
    {
    for (int kk=0; kk<nyI; kk++)
    {
    for (int ii=0; ii<nzI; ii++)
    {
        *XCoord = *(XCoordI+ii+kk*nzI+ll*nyI*nzI);
        XCoord++;
        *YCoord = *(YCoordI+ii+kk*nzI+ll*nyI*nzI);
        YCoord++;
        *ZCoord = *(ZCoordI+ii+kk*nzI+ll*nyI*nzI);
        ZCoord++;
    }
    for (int ii=1; ii<nzII; ii++)
    {
        *XCoord = *(XCoordII+ii+kk*nzII+ll*nyII*nzII);
        XCoord++;
        *YCoord = *(YCoordII+ii+kk*nzII+ll*nyII*nzII);
        YCoord++;
        *ZCoord = *(ZCoordII+ii+kk*nzII+ll*nyII*nzII);
        ZCoord++;
    }
    }
    }
            //********ende north-south*******
    break;
    
        default:
            dt__THROW(coupleStructBlocks(),
                        << "unknown coupling index" << LOGDEL
                        << DTLOGEVAL(cm)
                        );
    } 
    
    return coupleStructGrid;
    
}

covise::coDoUnstructuredGrid * dtBmGrid::unsCoupleStructBlocks(string const str,
                                                            coDoStructuredGrid * const gridI,
                                                            coDoStructuredGrid * const gridII,
                                                            int const coupleIndex) const {
    int nxI, nyI, nzI;
    int nxII, nyII, nzII;
    
    gridI->getGridSize(&nxI, &nyI, &nzI);
    gridII->getGridSize(&nxII, &nyII, &nzII);
    
    int nelem = (nxI-1) * (nyI-1) * (nzI-1) + (nxII-1) * (nyII-1) * (nzII-1); //immer
    int nconn = 8 * nelem; //immer
    int ht = 1; //immer
    int *outElemList, *outConnList, *outTypeList; //immer
    float *outXCoord, *outYCoord, *outZCoord; //immer
    int ncoord;
    //*********if couple east-west *************
    switch (coupleIndex)
    {
        case 1:
    ncoord = ( nxI*nyI*nzI ) + ( (nxII-1)*nyII*nzII ); //eastwest
    if (nyII > nyI || nzII > nzI ) {
                dt__THROW(unsCoupleStructBlocks(),
                        << "forbidden number of knots in EW-coupling (nyII>nyI || nzII>nzI)" << LOGDEL
                        << DTLOGEVAL(nyI) << LOGDEL
                        << DTLOGEVAL(nyII) << LOGDEL
                        << DTLOGEVAL(nzI) << LOGDEL
                        << DTLOGEVAL(nzII) << LOGDEL
                        );
            }
    break;
    
        case 2:
            ncoord = ( nxI*nyI*nzI ) + ( nxII*(nyII-1)*nzII );//backfront
            if (nxII > nxI || nzII > nzI ) {
                dt__THROW(unsCoupleStructBlocks(),
                        << "forbidden number of knots in BF-coupling (nxII>nxI || nzII>nzI)" << LOGDEL
                        << DTLOGEVAL(nxI) << LOGDEL
                        << DTLOGEVAL(nxII) << LOGDEL
                        << DTLOGEVAL(nzI) << LOGDEL
                        << DTLOGEVAL(nzII) << LOGDEL
                        );
            }
            break;
            
        case 3:
            ncoord = ( nxI*nyI*nzI ) + ( nxII*nyII*(nzII-1) ); //northsouth
            if (nxII > nxI || nyII > nyI ) {
                dt__THROW(unsCoupleStructBlocks(),
                        << "forbidden number of knots in NS-coupling (nxII>nxI || nyII>nyI)" << LOGDEL
                        << DTLOGEVAL(nxI) << LOGDEL
                        << DTLOGEVAL(nxII) << LOGDEL
                        << DTLOGEVAL(nyI) << LOGDEL
                        << DTLOGEVAL(nyII) << LOGDEL
                        );
            }
            break;
            
        default:
            dt__THROW(unsCoupleStructBlocks(),
                        << "unknown coupling index" << LOGDEL
                        << DTLOGEVAL(coupleIndex)
                        );
    }
    coDoUnstructuredGrid *unsCoupleGrid = new coDoUnstructuredGrid(str, nelem, nconn, ncoord, ht); //immer
    unsCoupleGrid->getAddresses(&outElemList, &outConnList, &outXCoord, &outYCoord, &outZCoord); //immer
    unsCoupleGrid->getTypeList(&outTypeList); //immer
        
    for (int ii=0;ii<nelem;ii++) //ganze schleife immer
    {
                *outElemList = 8 * ii;
                outElemList++;
                *outTypeList = TYPE_HEXAGON;
                outTypeList++;
    }
    
    int itNyCount = 0; //immer
    int itNxCount = 0; //immer
    //connection-list blockI
    for (int ii=0;ii<( (nxI*nyI*nzI-1)-(nxI*nyI)-nxI );ii++) //ganze schleife immer
    {

        if (ii == ((nyI-1)*nxI-1)+itNyCount * (nxI*nyI)) {
                itNyCount++;
                ii += nxI;
                itNxCount += 2 ;
        }
        else {
            if (ii == ( (nxI-1) + itNxCount * nxI ) ) {
            itNxCount++;
            }
            else {
            *(outConnList+0) = ii;
            *(outConnList+1) = ii+1;
            *(outConnList+2) = ii+nxI+1;
            *(outConnList+3) = ii+nxI;
            *(outConnList+4) = ii+(nxI*nyI);
            *(outConnList+5) = ii+(nxI*nyI)+1;
            *(outConnList+6) = ii+(nxI*nyI)+nxI+1;
            *(outConnList+7) = ii+(nxI*nyI)+nxI;
            
            
            outConnList += 8;
            }
                
        }
    }
    //*******couple-connections*********
    int startPoint = nxI*nyI*nzI;
    
    switch (coupleIndex)
    {
        case 1:
    for (int kk=0; kk<(nzII-1); kk++) //ganze schleife eastwest
    {
    for (int ii=0; ii<(nyII-1); ii++) 
    {
    *(outConnList) = kk*nxI*nyI + ii*nxI + nxI-1;
    *(outConnList+1) = kk*(nxII-1)*nyII + ii*(nxII-1) + startPoint;
    *(outConnList+2) = kk*(nxII-1)*nyII + ii*(nxII-1) + startPoint + (nxII-1);
    *(outConnList+3) = kk*nxI*nyI + ii*nxI + nxI-1+nxI;
    *(outConnList+4) = kk*nxI*nyI + ii*nxI + nxI-1 + nxI*nyI;
    *(outConnList+5) = kk*(nxII-1)*nyII + ii*(nxII-1) + startPoint + (nxII-1)*nyII;
    *(outConnList+6) = kk*(nxII-1)*nyII + ii*(nxII-1) + startPoint + (nxII-1)*nyII+(nxII-1);
    *(outConnList+7) = kk*nxI*nyI + ii*nxI + nxI-1 + nxI*nyI+nxI;
    outConnList += 8;
    }
    }
    //*******end couple-connections*****
    
    //connection-list blockII
    itNxCount = 0;
    itNyCount = 0;
    for (int ii=0;ii<( ((nxII-1)*nyII*nzII-1)-((nxII-1)*nyII)-(nxII-1) );ii++) //ganze schleife eastwest
    {

        if (ii == ((nyII-1)*(nxII-1)-1)+itNyCount * ((nxII-1)*nyII)) {
                itNyCount++;
                ii += (nxII-1);
                itNxCount += 2 ;
        }
        else {
            if (ii == ( (nxII-1-1) + itNxCount * (nxII-1) ) ) {
            itNxCount++;
            }
            else {
            *(outConnList+0) = startPoint + ii;
            *(outConnList+1) = startPoint + ii+1;
            *(outConnList+2) = startPoint + ii+(nxII-1)+1;
            *(outConnList+3) = startPoint + ii+(nxII-1);
            *(outConnList+4) = startPoint + ii+((nxII-1)*nyII);
            *(outConnList+5) = startPoint + ii+((nxII-1)*nyII)+1;
            *(outConnList+6) = startPoint + ii+((nxII-1)*nyII)+(nxII-1)+1;
            *(outConnList+7) = startPoint + ii+((nxII-1)*nyII)+(nxII-1);
            
            
            outConnList += 8;
            }
                
        }
    }
    break;
    
        case 2:
            //*******couple-connections*********
    for (int kk=0; kk<(nzII-1); kk++) //ganze schleife front-back
    {
    for (int ii=0; ii<(nxII-1); ii++) 
    {
    *(outConnList) = kk*nxI*nyI + ii + (nxI*nyI-nxI);
    *(outConnList+1) = kk*nxI*nyI + ii+1 + (nxI*nyI-nxI);
    *(outConnList+2) = kk*nxII*(nyII-1) + ii+1 + startPoint;
    *(outConnList+3) = kk*nxII*(nyII-1) + ii + startPoint;
    *(outConnList+4) = kk*nxI*nyI + ii + (nxI*nyI-nxI) + nxI*nyI;
    *(outConnList+5) = kk*nxI*nyI + ii+1 + (nxI*nyI-nxI) + nxI*nyI;
    *(outConnList+6) = kk*nxII*(nyII-1) + ii+1 + startPoint + nxII*(nyII-1);
    *(outConnList+7) = kk*nxII*(nyII-1) + ii + startPoint + nxII*(nyII-1);
    outConnList += 8;
    }
    }
    //*******end couple-connections*****
    
    //connection-list blockII
    itNxCount = 0;
    itNyCount = 0;
    for (int ii=0;ii<( (nxII*(nyII-1)*nzII-1)-(nxII*(nyII-1))-nxII );ii++) //ganze schleife frontback
    {

        if (ii == (((nyII-1)-1)*nxII-1)+itNyCount * (nxII*(nyII-1))) {
                itNyCount++;
                ii += nxII;
                itNxCount += 2 ;
        }
        else {
            if (ii == ( (nxII-1) + itNxCount * nxII) ) {
            itNxCount++;
            }
            else {
            *(outConnList+0) = startPoint + ii;
            *(outConnList+1) = startPoint + ii+1;
            *(outConnList+2) = startPoint + ii+nxII+1;
            *(outConnList+3) = startPoint + ii+nxII;
            *(outConnList+4) = startPoint + ii+(nxII*(nyII-1));
            *(outConnList+5) = startPoint + ii+(nxII*(nyII-1))+1;
            *(outConnList+6) = startPoint + ii+(nxII*(nyII-1))+nxII+1;
            *(outConnList+7) = startPoint + ii+(nxII*(nyII-1))+nxII;
            
            
            outConnList += 8;
            }
                
        }
    }
            break;
            
        case 3:
            //*******couple-connections*********

    for (int kk=0; kk<(nyII-1); kk++) //ganze schleife northsouth
    {
    for (int ii=0; ii<(nxII-1); ii++) 
    {
    *(outConnList) = kk*nxI + nxI*nyI*(nzI-1) + ii;
    *(outConnList+1) = kk*nxI + nxI*nyI*(nzI-1) + ii+1;
    *(outConnList+2) = kk*nxI + nxI*nyI*(nzI-1) + ii+1 + nxI;
    *(outConnList+3) = kk*nxI + nxI*nyI*(nzI-1) + ii + nxI;
    *(outConnList+4) = kk*nxII + ii + startPoint;
    *(outConnList+5) = kk*nxII + ii+1 + startPoint;
    *(outConnList+6) = kk*nxII + ii+1 + startPoint + nxII;
    *(outConnList+7) = kk*nxII + ii + startPoint + nxII;
    outConnList += 8;
    }
    }
    //*******end couple-connections*****    
    
    //connection-list blockII
    itNxCount = 0;
    itNyCount = 0;
    for (int ii=0;ii<( (nxII*nyII*(nzII-1)-1)-(nxII*nyII)-nxII );ii++) //ganze schleife northsouth
    {

        if (ii == ((nyII-1)*nxII-1)+itNyCount * (nxII*nyII)) {
                itNyCount++;
                ii += nxII;
                itNxCount += 2 ;
        }
        else {
            if (ii == ( (nxII-1) + itNxCount * nxII ) ) {
            itNxCount++;
            }
            else {
            *(outConnList+0) = startPoint + ii;
            *(outConnList+1) = startPoint + ii+1;
            *(outConnList+2) = startPoint + ii+nxII+1;
            *(outConnList+3) = startPoint + ii+nxII;
            *(outConnList+4) = startPoint + ii+(nxII*nyII);
            *(outConnList+5) = startPoint + ii+(nxII*nyII)+1;
            *(outConnList+6) = startPoint + ii+(nxII*nyII)+nxII+1;
            *(outConnList+7) = startPoint + ii+(nxII*nyII)+nxII;
                       
            outConnList += 8;
            }
                
        }
    }
            
    }
     
    float tmpXCoord, tmpYCoord, tmpZCoord; //immer
    //coordinates-list blockI
    for (int zz=0;zz<nzI;zz++) //ganze schleife immer
    {
        for (int yy=0;yy<nyI;yy++)
        {
            for (int xx=0;xx<nxI;xx++)
            {
                gridI->getPointCoordinates(xx, &tmpXCoord, yy, &tmpYCoord, zz, &tmpZCoord );
                *outXCoord = tmpXCoord;
                outXCoord++;
                *outYCoord = tmpYCoord;
                outYCoord++;
                *outZCoord = tmpZCoord;
                outZCoord++;
            }
        }
    }
    //coordinates-list blockII
    switch (coupleIndex)
    {
        case 1:
    for (int zz=0;zz<nzII;zz++) //ganze schleife eastwest
    {
        for (int yy=0;yy<nyII;yy++)
        {
            for (int xx=1;xx<nxII;xx++)
            {
                gridII->getPointCoordinates(xx, &tmpXCoord, yy, &tmpYCoord, zz, &tmpZCoord );
                *outXCoord = tmpXCoord;
                outXCoord++;
                *outYCoord = tmpYCoord;
                outYCoord++;
                *outZCoord = tmpZCoord;
                outZCoord++;
            }
        }
    }
    break;
    
        case 2:
    for (int zz=0;zz<nzII;zz++) //ganze schleife frontback
    {
        for (int yy=1;yy<nyII;yy++)
        {
            for (int xx=0;xx<nxII;xx++)
            {
                gridII->getPointCoordinates(xx, &tmpXCoord, yy, &tmpYCoord, zz, &tmpZCoord );
                *outXCoord = tmpXCoord;
                outXCoord++;
                *outYCoord = tmpYCoord;
                outYCoord++;
                *outZCoord = tmpZCoord;
                outZCoord++;
            }
        }
    }
            break;
            
        case 3:
    for (int zz=1;zz<nzII;zz++) //ganze schleife northsouth
    {
        for (int yy=0;yy<nyII;yy++)
        {
            for (int xx=0;xx<nxII;xx++)
            {
                gridII->getPointCoordinates(xx, &tmpXCoord, yy, &tmpYCoord, zz, &tmpZCoord );
                *outXCoord = tmpXCoord;
                outXCoord++;
                *outYCoord = tmpYCoord;
                outYCoord++;
                *outZCoord = tmpZCoord;
                outZCoord++;
            }
        }
    }
    
    }
    //*********end if couple east-west *********
    return unsCoupleGrid;
}


    covise::coDoUnstructuredGrid * dtBmGrid::coupleBlocks(std::vector< std::pair<dtVector3 , int> > const blockI,
                                                          int const surfIndexI,
                                                          std::vector< std::pair<dtVector3 , int> > blockII,
                                                          int const surfIndexII) {
    
//        int countx=0;
//        int county=0;
//        int countz=0;
//        
//        for (int ii=0; ii<blockI.size(); ii++)
//        {
//            if (blockI.at(ii).second == 15) {
//                countx++;
//            }
//            if (blockI.at(ii).second == 13) {
//                county++;
//            }
//            if (blockI.at(ii).second == 35) {
//                countz++;
//            }
//        }
//        
//        int nxI = countx + 2;
//        int nyI = county + 2;
//        int nzI = countz + 2;
//        
//        countx=0;
//        county=0;
//        countz=0;
//        
//        for (int ii=0; ii<blockII.size(); ii++)
//        {
//            if (blockII.at(ii).second == 15) {
//                countx++;
//            }
//            if (blockII.at(ii).second == 13) {
//                county++;
//            }
//            if (blockII.at(ii).second == 35) {
//                countz++;
//            }
//        }
//        
//        int nxII = countx + 2;
//        int nyII = county + 2;
//        int nzII = countz + 2;
//        
//        
//     //   pointList = blockI;
//        int nn = (blockI.size()-1);
//        
//        if (surfIndexI == 4 && surfIndexII == 3) { //coupling east-west
//            
//            for (int ii=(blockII.size()-nxII/*-1?*/); ii=0 ;) {
//                
//                blockII.erase(blockII.begin()+ii);
//                for(int kk=(cornerII.size()-1); kk=0; kk--) {
//                    if (cornerII.at(kk) == ii) {
//                        cornerII.at(kk) = nn;
//                        nn-=nxI;
//                        break;
//                    }
//                }
//                ii-=nxII;
//            }
//            
//           
//            
//        }
//        
//        int nelem = (nxI-1)*(nyI-1)*(nzI-1)+(nxII-1)*(nyII-1)*(nzII-1);
//	int nconn = 8*nelem;
//	int ht = 1;
//
//        
//	int *outElemList, *outConnList, *outTypeList;
//	float *outXCoord, *outYCoord, *outZCoord;
//        string str = "coupleGrid";
//        
//
//	coDoUnstructuredGrid *firstGrid = new coDoUnstructuredGrid(str, nelem, nconn, ( blockI.size()+blockII.size() ), ht);
//
//	firstGrid->getAddresses(&outElemList, &outConnList, &outXCoord, &outYCoord, &outZCoord);
//	firstGrid->getTypeList(&outTypeList);
//        
//        for (int ii=0;ii<nelem;ii++)
//        {
//                *outElemList = 8 * ii;
//                outElemList++;
//                
//                *outTypeList = TYPE_HEXAGON;
//                outTypeList++;
//        }
//        
//        
//        for (int ii=0; ii<cornerI.size(); ii++) {
//            *(outConnList+ii) = cornerI.at(ii);
//        }
//        for (int ii=cornerI.size(); ii<( cornerI.size()+cornerII.size() ); ii++) {
//            *(outConnList+ii) = cornerII.at(ii-cornerI.size());
//        }
//        
////        std::vector<int> ConnectionListI;
////        int kk=0;
////        int itNyCount = 0;
////        int itNxCount = 0;
////        
////        for (int ii=0;ii<( (blockI.size())-1-(nxI*nyI)-nxI );ii++)
////        {
////
////        if (ii == ((nyI-1)*nxI-1)+itNyCount * (nxI*nyI)) {
////                itNyCount++;
////                ii += nxI;
////                itNxCount += 2 ;
////        }
////        else {
////            if (ii == ( (nxI-1) + itNxCount * nxI ) ) {
////            itNxCount++;
////            }
////            else {
////            ConnectionListI.at(kk) = ii;
////            ConnectionListI.at(kk+1) = ii+1;
////            ConnectionListI.at(kk+2) = ii+nxI+1;
////            ConnectionListI.at(kk+3) = ii+nxI;
////            ConnectionListI.at(kk+4) = ii+(nxI*nyI);
////            ConnectionListI.at(kk+5) = ii+(nxI*nyI)+1;
////            ConnectionListI.at(kk+6) = ii+(nxI*nyI)+nxI+1;
////            ConnectionListI.at(kk+7) = ii+(nxI*nyI)+nxI;
////            
////            kk += 8;
////            }
////                
////        }
////	}
//        
//        for (int ii=0;ii<blockI.size(); ii++)
//        {
//                *outXCoord = blockI.at(ii).first.x();
//                outXCoord++;
//                *outYCoord = blockI.at(ii).first.y();
//                outYCoord++;
//                *outZCoord = blockI.at(ii).first.z();
//                outZCoord++;
//        }
//        for (int ii=0;ii<blockII.size(); ii++)
//        {
//                *outXCoord = blockII.at(ii).first.x();
//                outXCoord++;
//                *outYCoord = blockII.at(ii).first.y();
//                outYCoord++;
//                *outZCoord = blockII.at(ii).first.z();
//                outZCoord++;
//        }
// 
//        return firstGrid;
};


covise::coDoSet * dtBmGrid::toCoDoUns2D(string const str) const {
    pointContainer myPContainer;
    coDoSet * returnset;
    for (int ii=0;ii<(_surfGridPoints.size());ii++)
        {
        myPContainer.add(_surfGridPoints.at(ii), stringPrimitive().intToString(ii) );
    }
    returnset = myPContainer.toCoDoSet(str);
    return returnset;
};


}

