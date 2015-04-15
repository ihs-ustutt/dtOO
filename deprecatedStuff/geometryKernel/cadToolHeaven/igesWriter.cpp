#include "igesWriter.h"
#include "logMe/logMe.h"

#include <logMe/logMe.h>
#include <sisl.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/igesWritable.h>
#include <geometryEngine/dtSislBase.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSislSurf.h>

namespace dtOO {
  DTCLASSLOGMETHODI(igesWriter,
          << dt__eval(&(thisRef._toWriteCurve)) << std::endl
          << dt__eval(&(thisRef._toWriteSurf)) << std::endl
          << dt__eval(&(thisRef._SizeOfCurveDouble)) << std::endl
          << dt__eval(&(thisRef._SizeOfSurfDouble)) << std::endl
          << dt__eval(&(thisRef._CurveDataDouble)) << std::endl
          << dt__eval(&(thisRef._SurfDataDouble)) << std::endl
          << dt__eval(&(thisRef._CurveDataInt)) << std::endl
          << dt__eval(&(thisRef._SurfDataInt)) << std::endl
          << dt__eval(thisRef.paraSequ) << std::endl
          << dt__eval(thisRef.DESequ));
  
  igesWriter::igesWriter() {
  }

  igesWriter::~igesWriter() {
  }

  void igesWriter::InitCurve(SISLCurve const * curve)
  {
    _toWriteCurve.push_back(curve);


    SISLCurve * curvecopy = NULL;
    curvecopy = copyCurve(
                  const_cast< SISLCurve * >(
                    _toWriteCurve.back()
                  )
                );

  //Entries for Parameter Data Section in vector

    int planar = 0;		//nonplanar (0) or planar (1)
    int curvekind = 0;	//rational (0) or polynomial (1)
    int periodic = 0;	//nonperiodic (0) or periodic (1)

    int VertNo = curvecopy->in;			//number vertices
    int order = curvecopy->ik;			//order of curve
    int open = curvecopy->cuopen;		//open or closed
    int dimension = curvecopy->idim;	//dimension
    int EntityNo = 126;	
    int index = VertNo-1;
    int degree = order - 1;


    if (curvecopy->ikind == 1 || curvecopy->ikind == 3)		//rational or polynomial?
      {
      curvekind = 1;
      }

    for (int i = 0; i < (index+order); i++)		//periodic or nonperiodic?
      {
      if (*(curvecopy->et+i) == *(curvecopy->et+(i+1)))
        {
        periodic = 1;
        }
      else
        {
        periodic = 0;
        break;
        }
      }

    _CurveDataInt.push_back(EntityNo);
    _CurveDataInt.push_back(index);
    _CurveDataInt.push_back(degree);
    _CurveDataInt.push_back(planar);
    _CurveDataInt.push_back(open);
    _CurveDataInt.push_back(curvekind);
    _CurveDataInt.push_back(periodic);

    for (int i=0; i <(index+order+1);i++)
      {
      _CurveDataDouble.push_back(*(curvecopy->et+i));			//Knots in container
      }

    if (curvekind == 0)
      {
      for (int i=1; i < VertNo+1; i++)
        {
        _CurveDataDouble.push_back(*(curvecopy->rcoef+i*(dimension+1)-1));		//weights in container if rational

        }
      }	
    else
      {		
      for (int i=0; i < VertNo; i++)	
        {	
        _CurveDataDouble.push_back(1);			//weights(all=1) in container if polynomial
        }	
      }


    for (int i=0; i <(VertNo*dimension);i++)
      {
      _CurveDataDouble.push_back(*(curvecopy->ecoef+i));		//vertices in container
      }

    _CurveDataDouble.push_back(*(curvecopy->et+0));			
    _CurveDataDouble.push_back(*(curvecopy->et+(index+order)));

    if (planar == 1)		//if planar -> unit vector in container (not yet implemented)
      {					//when implemented, parameter data section to be changed also
      }
    _SizeOfCurveDouble.push_back(_CurveDataDouble.size());
  }

  void igesWriter::InitSurf(SISLSurf const * surface)
  {
    _toWriteSurf.push_back(surface);

    SISLSurf * surfcopy = NULL;
    surfcopy = copySurface(
                 const_cast< SISLSurf * >(
                   _toWriteSurf.back()
                 )
               );

  //Entries for Parameter Data Section in vector

    int curvekind = 0;	//rational (0) or polynomial (1)
    int periodic1 = 0;	//nonperiodic (0) or periodic (1) direction 1
    int periodic2 = 0;	//nonperiodic (0) or periodic (1) direction 2

    int forcounter = 1;

    int VertNo1 = surfcopy->in1;		//number vertices direction 1
    int VertNo2 = surfcopy->in2;		//number vertices direction 2
    int order1 = surfcopy->ik1;			//order of surface in direction 1
    int order2 = surfcopy->ik2;			//order of surface in direction 2
    int open1 = surfcopy->cuopen_1;		//open or closed direction 1
    int open2 = surfcopy->cuopen_2;		//open or closed direction 2
    int dimension = surfcopy->idim;	//dimension
    int EntityNo = 128;	
    int index1 = VertNo1 - 1;
    int index2 = VertNo2 - 1;
    int degree1 = order1 - 1;
    int degree2 = order2 - 1;


    if (surfcopy->ikind == 1 || surfcopy->ikind == 3)		//rational or polynomial?
      {
      curvekind = 1;
      }

    for (int i = 0; i < (index1+order1); i++)		//periodic or nonperiodic direction 1
      {
      if (*(surfcopy->et1+i) == *(surfcopy->et1+(i+1)))
        {
        periodic1 = 1;
        }
      else
        {
        periodic1 = 0;
        break;
        }
      }

    for (int i = 0; i < (index2+order2); i++)		//periodic or nonperiodic direction 2
      {
      if (*(surfcopy->et2+i) == *(surfcopy->et2+(i+1)))
        {
        periodic2 = 1;
        }
      else
        {
        periodic2 = 0;
        break;
        }
      }

    _SurfDataInt.push_back(EntityNo);
    _SurfDataInt.push_back(index1);
    _SurfDataInt.push_back(index2);
    _SurfDataInt.push_back(degree1);
    _SurfDataInt.push_back(degree2);
    _SurfDataInt.push_back(open1);
    _SurfDataInt.push_back(open2);
    _SurfDataInt.push_back(curvekind);
    _SurfDataInt.push_back(periodic1);
    _SurfDataInt.push_back(periodic2);

    for (int i=0; i <(index1+order1+1);i++)
      {
      _SurfDataDouble.push_back(*(surfcopy->et1+i));			//Knots direction 1 in container
      }
    for (int i=0; i <(index2+order2+1);i++)
      {
      _SurfDataDouble.push_back(*(surfcopy->et2+i));			//Knots direction 2 in container
      }

    if (curvekind == 0)
      {
      for (int i=1; i < VertNo1*VertNo2+1; i++) 
        {
        _SurfDataDouble.push_back(*(surfcopy->rcoef+i*(dimension+1)-1));		//weights in container if rational
        }

  /*	Einlesen der Vertices, die eine Gewichtung haben (rcoef), führt aber zu Fehlern in der Geometrie, deshalb werden die nicht-gewichteten Vertices genommen (ecoef)
      for (int i=1; i < VertNo1*VertNo2*(dimension+1)+1;i++)
        {
        if (forcounter*(dimension+1) == i)
          {
          forcounter++;
          }
        else
          {
          std::cout << i << ". Eintrag Vertices: " << *(surfcopy->rcoef+i-1) << std::endl;
          _SurfDataDouble.push_back(*(surfcopy->rcoef+i-1));		//vertices in container
          }
        }*/
      for (int i=0; i < VertNo1*VertNo2*dimension;i++)
        {
        _SurfDataDouble.push_back(*(surfcopy->ecoef+i));		//vertices in container
        }
      }	

    else
      {		
      for (int i=0; i < VertNo1*VertNo2; i++)	
        {	
        _SurfDataDouble.push_back(1);			//weights(all=1) in container if polynomial
        }	
      for (int i=0; i < VertNo1*VertNo2*dimension;i++)
        {
        _SurfDataDouble.push_back(*(surfcopy->ecoef+i));		//vertices in container
        }
      }

    _SurfDataDouble.push_back(*(surfcopy->et1+0));			
    _SurfDataDouble.push_back(*(surfcopy->et1+(index1+order1)));
    _SurfDataDouble.push_back(*(surfcopy->et2+0));			
    _SurfDataDouble.push_back(*(surfcopy->et2+(index2+order2)));

    _SizeOfSurfDouble.push_back(_SurfDataDouble.size());

  }

  void igesWriter::InitAnalyticGeometry(analyticGeometry * aGeoP) {   
    igesWritable * toWrite = dynamic_cast< igesWritable * > (aGeoP);
    if (toWrite) {
      std::vector< dtSislBase * > dtSislObj = toWrite->getDtSislObjects();
      for (int ii=0;ii<dtSislObj.size();ii++) {
        dtCurve * dtSislCurveP = dynamic_cast< dtCurve * >(dtSislObj[ii]);
        dtSislSurf * dtSislSurfP = dynamic_cast< dtSislSurf * >(dtSislObj[ii]);
        if (dtSislCurveP) {
          InitCurve( dtSislCurveP->getSISLCurve() );
        }
        else if (dtSislSurfP) {
          InitSurf( dtSislSurfP->getSISLSurf() );
        }
        else {
          dt__warning(InitAnalyticGeometry,
                  << dt__eval(dtSislCurveP) << std::endl
                  << dt__eval(dtSislCurveP));
        }
      }
    }
    else {
      dt__warning(InitAnalyticGeometry,
              << "analyticGeometry " << aGeoP->getLabel() 
              << " has not correct type igesWritable");
    }
  }
  
  void igesWriter::writeGeometry(const char *filename)
  {
    int CurveCounterInt = 0;				//global counter for int vector curve
    int CurveCounterDouble = 0;				//global counter for double vector curve
    int SurfCounterInt = 0;					//global counter for int vector surf
    int SurfCounterDouble = 0;				//global counter for double vector surf
    paraSequ = 1;
    DESequ = 1;
    int lineLength = 16;
    int tempCounter = 0;
    int DEpointer = 1;
    int ParaData = 1;
    int EntityNo=126, StatusNo=0, LineWeightNo=1, Color=2, ParaLineCount=1, FormNo=0, EntSubNo=0, Struc=0, LineFont=1, Level=2, View=0, TransMatrix=0, LabDisAsso=0, Res=0, EntLab=0;

    SISLCurve *curvecopy = NULL;
    SISLSurf *surfcopy = NULL;

    std::fstream datei;
    datei.open(filename, std::ios::out |std::ios::app);	
    datei.precision(8);
    datei.fixed;
  //----------------------------------------------------------------------------------------
  //Directory Entry Section
  //----------------------------------------------------------------------------------------
  //for Curve
  //----------------------------------------------------------------------------------------
    for (int gCounter=0; gCounter < _toWriteCurve.size(); gCounter++)
      {

      if (gCounter == 1)
        {
        ParaData = ParaData + 2 + _SizeOfCurveDouble[gCounter-1]/(64/(lineLength+1));

        if (_SizeOfCurveDouble[gCounter-1]%(64/(lineLength+1)) > 0)
          {
          ParaData = ParaData+1;
          }
        }
      if (gCounter > 1)
        {
        ParaData = ParaData + 2 +(_SizeOfCurveDouble[gCounter-1]-_SizeOfCurveDouble[gCounter-2])/(64/(lineLength+1));

        if ((_SizeOfCurveDouble[gCounter-1]-_SizeOfCurveDouble[gCounter-2])%(64/(lineLength+1)) > 0)
          {
          ParaData = ParaData+1;
          }
        }
      int EntityValue[18]={EntityNo,ParaData,Struc,LineFont,Level,View,TransMatrix,LabDisAsso, StatusNo, EntityNo, LineWeightNo, Color, ParaLineCount, FormNo,Res,Res,EntLab, EntSubNo};		

      for (int i=0; i<9;i++)
        {
        datei.width(8);
        datei << EntityValue[i];
        if (i==8)
          {
          datei << "D";
          datei.width(7);
          datei << DESequ;
          DESequ++;
          }
        }
      datei << std::endl;
      for (int i=9; i<18;i++)
        {
        datei.width(8);
        datei << EntityValue[i];
        if (i==17)
          {
          datei << "D";
          datei.width(7);
          datei << DESequ;
          DESequ++;
          }
        }
      datei << std::endl;
      tempCounter = gCounter;
      }

  //----------------------------------------------------------------------------------------
  //for Surface
  //----------------------------------------------------------------------------------------
    if (tempCounter > 0)
      {
      ParaData = ParaData + 2 +(_SizeOfCurveDouble[tempCounter-1]-_SizeOfCurveDouble[tempCounter-2])/(64/(lineLength+1));
      }
    for (int gCounter=0; gCounter < _toWriteSurf.size(); gCounter++)
      {
      EntityNo=128;
      if (gCounter == 1)
        {
        ParaData = ParaData + 1 + _SizeOfSurfDouble[gCounter-1]/(64/(lineLength+1));

        if (_SizeOfSurfDouble[gCounter-1]%(64/(lineLength+1)) > 0)
          {
          ParaData = ParaData+1;
          }
        }
      if (gCounter > 1)
        {
        ParaData = ParaData + 1 +(_SizeOfSurfDouble[gCounter-1]-_SizeOfSurfDouble[gCounter-2])/(64/(lineLength+1));

        if ((_SizeOfSurfDouble[gCounter-1]-_SizeOfSurfDouble[gCounter-2])%(64/(lineLength+1)) > 0)
          {
          ParaData = ParaData+1;
          }
        }
      int EntityValue[18]={EntityNo,ParaData,Struc,LineFont,Level,View,TransMatrix,LabDisAsso, StatusNo, EntityNo, LineWeightNo, Color, ParaLineCount, FormNo,Res,Res,EntLab, EntSubNo};		

      for (int i=0; i<9;i++)
        {
        datei.width(8);
        datei << EntityValue[i];
        if (i==8)
          {
          datei << "D";
          datei.width(7);
          datei << DESequ;
          DESequ++;
          }
        }
      datei << std::endl;
      for (int i=9; i<18;i++)
        {
        datei.width(8);
        datei << EntityValue[i];
        if (i==17)
          {
          datei << "D";
          datei.width(7);
          datei << DESequ;
          DESequ++;
          }
        }
      datei << std::endl;
      }

  //----------------------------------------------------------------------------------------
  //Parameter Data Section
  //----------------------------------------------------------------------------------------
  //for curve
  //----------------------------------------------------------------------------------------
    for (int gCounter=0; gCounter < _toWriteCurve.size(); gCounter++)
      {

      for (int i=0; i < 7; i++)	//first line out of array CurveDataField (int)
        {	
          datei.width(8);
          datei << _CurveDataInt[CurveCounterInt];
          datei << ",";
          CurveCounterInt++;
        }

      datei << " ";
      datei.width(8);
      datei << DEpointer;
      datei << "P";
      datei.width(7);
      datei << paraSequ << std::endl;
      paraSequ++;

      int row =0;
      int p=0;

      while(p < _SizeOfCurveDouble[gCounter])	//remaining lines out of vector _CurveDataDouble (double)
      {
  //--------------------------------------------
  //1st vector
  //--------------------------------------------		
      if (gCounter == 0)

        {	

        for (int i=0; i < (64/(lineLength+1)); i++)	
          {
  //if end of vector 1, break for loop
          if (p == _SizeOfCurveDouble[gCounter])
            {
            break;
            }	
          datei.width(lineLength);
          datei << _CurveDataDouble[CurveCounterDouble];
          datei << ",";
          CurveCounterDouble++;
          p++;
          }
  //if end of vector 1, break while loop
        if (p == _SizeOfCurveDouble[gCounter])
          {
          if (_SizeOfCurveDouble[gCounter]%(64/(lineLength+1)) == 0)
            {
            row = 72-(64/(lineLength+1)*(lineLength+1));
            }
          else
            {
            row = (64/(lineLength+1)-_SizeOfCurveDouble[gCounter]%(64/(lineLength+1)))*(lineLength+1)+72-(64/(lineLength+1)*(lineLength+1));
            }
          datei.width(row);
          datei << DEpointer;
          datei << "P";
          datei.width(7);
          datei << paraSequ << std::endl;
          paraSequ++;
          break;
          }
        datei.width(72-64/(lineLength+1)*(lineLength+1));

        datei << DEpointer;		
        datei << "P";
        datei.width(7);
        datei << paraSequ << std::endl;
        paraSequ++;
        }
  //--------------------------------------------
  //2nd + following curves
  //--------------------------------------------
      else
        {
        for (int i=0; i < (64/(lineLength+1)); i++)	
          {
  //if end of vector, break for loop
          if (p == (_SizeOfCurveDouble[gCounter]-_SizeOfCurveDouble[gCounter-1]))
            {
            break;
            }	
          datei.width(lineLength);
          datei << _CurveDataDouble[CurveCounterDouble];
          datei << ",";
          CurveCounterDouble++;
          p++;
          }
  //if end of vector, break while loop
        if (p == (_SizeOfCurveDouble[gCounter]-_SizeOfCurveDouble[gCounter-1]))
          {
          if ((_SizeOfCurveDouble[gCounter]-_SizeOfCurveDouble[gCounter-1])%(64/(lineLength+1)) == 0)
            {
            row = 72-(64/(lineLength+1)*(lineLength+1));
            }
          else
            {
            row = (64/(lineLength+1)-(_SizeOfCurveDouble[gCounter]-_SizeOfCurveDouble[gCounter-1])%(64/(lineLength+1)))*(lineLength+1)+72-(64/(lineLength+1)*(lineLength+1));
            }
          datei.width(row);
          datei << DEpointer;
          datei << "P";
          datei.width(7);
          datei << paraSequ << std::endl;
          paraSequ++;
          break;
          }
        datei.width(72-64/(lineLength+1)*(lineLength+1));

        datei << DEpointer;		
        datei << "P";
        datei.width(7);
        datei << paraSequ << std::endl;
        paraSequ++;
        }
      }
    datei.width(lineLength);
    datei << "0";
    datei << ",";
    datei.width(lineLength);
    datei << "0";
    datei << ",";
    datei.width(lineLength);
    datei << "0";
    datei << ";";
    datei.width(72-3*(lineLength +1));
    datei << DEpointer;
    datei << "P";
    datei.width(7);
    datei << paraSequ << std::endl;
    paraSequ++;
    DEpointer = DEpointer +2;
    }

  //----------------------------------------------------------------------------------------
  //for surface
  //----------------------------------------------------------------------------------------
    for (int gCounter=0; gCounter < _toWriteSurf.size(); gCounter++)
      {

      for (int i=0; i < 10; i++)	//first line out of array SurfDataField (int)
        {	
          datei.width(5);
          datei << _SurfDataInt[SurfCounterInt];
          datei << ",";
          SurfCounterInt++;
        }
      datei.width(12);
      datei << DEpointer;
      datei << "P";
      datei.width(7);
      datei << paraSequ << std::endl;
      paraSequ++;

      int row =0;
      int p=0;

      while(p < _SizeOfSurfDouble[gCounter])	//remaining lines out of vector _SurfDataDouble (double)
      {
  //--------------------------------------------
  //1st vector
  //--------------------------------------------		
      if (gCounter == 0)

        {	

        for (int i=0; i < (64/(lineLength+1)); i++)	
          {
  //if end of vector 1, break for loop
          if (p == _SizeOfSurfDouble[gCounter])
            {
            break;
            }	
          datei.width(lineLength);
          datei << _SurfDataDouble[SurfCounterDouble];
          datei << ",";
          SurfCounterDouble++;
          p++;
          }
  //if end of vector 1, break while loop
        if (p == _SizeOfSurfDouble[gCounter])
          {
          if (_SizeOfSurfDouble[gCounter]%(64/(lineLength+1)) == 0)
            {
            row = 72-(64/(lineLength+1)*(lineLength+1));
            }
          else
            {
            row = (64/(lineLength+1)-_SizeOfSurfDouble[gCounter]%(64/(lineLength+1)))*(lineLength+1)+72-(64/(lineLength+1)*(lineLength+1));
            }
          datei.width(row);
          datei << DEpointer;
          datei << "P";
          datei.width(7);
          datei << paraSequ << std::endl;
          paraSequ++;
          break;
          }
        datei.width(72-64/(lineLength+1)*(lineLength+1));

        datei << DEpointer;		
        datei << "P";
        datei.width(7);
        datei << paraSequ << std::endl;
        paraSequ++;
        }
  //--------------------------------------------
  //2nd + following surfaces
  //--------------------------------------------
      else
        {
        for (int i=0; i < (64/(lineLength+1)); i++)	
          {
  //if end of vector, break for loop
          if (p == (_SizeOfSurfDouble[gCounter]-_SizeOfSurfDouble[gCounter-1]))
            {
            break;
            }	
          datei.width(lineLength);
          datei << _SurfDataDouble[SurfCounterDouble];
          datei << ",";
          SurfCounterDouble++;
          p++;
          }
  //if end of vector, break while loop
        if (p == (_SizeOfSurfDouble[gCounter]-_SizeOfSurfDouble[gCounter-1]))
          {
          if ((_SizeOfSurfDouble[gCounter]-_SizeOfSurfDouble[gCounter-1])%(64/(lineLength+1)) == 0)
            {
            row = 72-(64/(lineLength+1)*(lineLength+1));
            }
          else
            {
            row = (64/(lineLength+1)-(_SizeOfSurfDouble[gCounter]-_SizeOfSurfDouble[gCounter-1])%(64/(lineLength+1)))*(lineLength+1)+72-(64/(lineLength+1)*(lineLength+1));
            }
          datei.width(row);
          datei << DEpointer;
          datei << "P";
          datei.width(7);
          datei << paraSequ << std::endl;
          paraSequ++;
          break;
          }
        datei.width(72-64/(lineLength+1)*(lineLength+1));

        datei << DEpointer;		
        datei << "P";
        datei.width(7);
        datei << paraSequ << std::endl;
        paraSequ++;
        }
      }
    DEpointer = DEpointer +2;
    }


  }

  void igesWriter::InitFile(const char *filename)
  {

  //----------------------------------------------------------------------------------------
  //Start Section
  //----------------------------------------------------------------------------------------
    int Scounter=3;
    std::fstream datei;
    datei.open(filename, std::ios::out);
    datei.precision(8);
    datei.fixed;
    for (int i=1;i<Scounter;i++)
    {	if (i==1){
        datei.width(72);
        datei.setf(std::ios_base::left,std::ios_base::adjustfield);
        datei << "Start Section";
      }
      else
      {
        datei.width(72);
        datei.setf(std::ios_base::left,std::ios_base::adjustfield);
        datei << " ";
      }
      datei << "S";
      datei.setf(std::ios_base::right,std::ios_base::adjustfield);
      datei.width(7);
      datei << i << std::endl;
    }

  //----------------------------------------------------------------------------------------
  //Global Section
  //----------------------------------------------------------------------------------------

    datei.width(72);
    datei.setf(std::ios_base::left,std::ios_base::adjustfield);
    datei << ",,,IGESCurve,chrbrueggemann,,32,38,6,38,15,IGESCurve,,2,2HMM,,0.1,";
    datei << "G";
    datei.setf(std::ios_base::right,std::ios_base::adjustfield);
    datei.width(7);
    datei << 1;
    datei << std::endl;
    datei.width(72);
    datei.setf(std::ios_base::left,std::ios_base::adjustfield);
    datei << "15H20130709.092614,0.0001,100.,cbruegge,IHS,11,0;";
    datei << "G";
    datei.setf(std::ios_base::right,std::ios_base::adjustfield);
    datei.width(7);
    datei << 2;
    datei << std::endl;
    datei.close();
  }

  void igesWriter::CloseFile(const char *filename)
  {

    std::fstream datei;
    datei.open(filename, std::ios::out | std::ios::app);
    datei.precision(8);
    datei.fixed;
    
    datei << "S";
    datei.width(7);
    datei << "2"; //pointer to last Starting Section entry

    datei << "G";
    datei.width(7);
    datei << "2"; //pointer to last Global Section entry

    datei << "D";
    datei.width(7);
    datei << DESequ-1; //pointer to last Directory Entry Section entry

    datei << "P";
    datei.width(7);
    datei << paraSequ-1; //pointer to last Parameter Data Section entry

    datei.width(40);
    datei << " ";

    datei << "T";
    datei.width(7);
    datei << 1;	

  }
}

//int main()
//{
//	const char *filename = "igesdatei.iges";
//
//	SISLCurve *curve=NULL;
//	SISLCurve *arc_seg=NULL;
//	SISLSurf *surface=NULL;
//	SISLSurf *cylinder=NULL;
//	int number = 3; 	//number of vertices in the curve
//	int order = 3; 		//order of curve
//	double knots[14];	//knot vector of curve
//	double coef[30];	//vertices of curve. These can either be the dim dimensional non-rational vertices or the (dim+1) dimensional rational vertices
//	int kind = 2;		//Type of curve (1= polyno Bspline, 2=nurbs, 3=polyn Bezier,4=rational Bezier)
//	int dim = 3;		// dimension of the space in which the curve lies
//	int copy = 1;		// Flage (0=set pointer to input arrays, 1=copy input arrays, 2=set pointer and remember to free arrays)
//	double astpar=0.0;	//parameter at the start of the curve
//	int iopen=1;		//closed/open (0/1)
//	int jstat=0;		//output: error (<0), ok (=0), warning (>0)
//	double shape = 0.5;	//ellipse(<0.5), parabola(=0.5), hyperbola(>0.5), if >= 1 then 0.999
//	double start_pos[3];
//	double top_pos[3];
//	double end_pos[3];
//
//	double bottom_pos[3] = {0,0,0};
//	double bottom_axis[3] = {20,0,0};
//	double ellipse_ratio = 1;
//	double axis_dir[3] = {0,0,1};
//	double height = 10;
//	double cone_angle =45;
//
//	double bottom_pos2[3] = {0,0,10};
//	double bottom_axis2[3] = {3.803,0,0};
//	double ellipse_ratio2 = 1;
//	double axis_dir2[3] = {0,0,1};
//	double height2 = 10;
//
//	
//	igesWriter kurve;
//	kurve.InitFile(filename);
//
///*	for (int anzahl=0; anzahl<4; ++anzahl)
//		{
//		std::cout << "Bitte geben Sie die Vertices an:" <<std::endl;
//		for (int i=0; i < number*dim; ++i)
//			{
//			std::cin >> coef[i];
//			}
//
//		s1630(coef, number, astpar, iopen, dim, order, &curve, &jstat);
//
//		std::cout << "Bitte geben Sie die Koordinaten des Start-Punktes an: " << std::endl;
//		for (int i = 0; i < 3; i++)
//			{
//			std::cin >> start_pos[i];
//			}
//		std::cout << "Bitte geben Sie die Koordinaten des Top-Punktes an: " << std::endl;
//		for (int i = 0; i < 3; i++)
//			{
//			std::cin >> top_pos[i];
//			}
//		std::cout << "Bitte geben Sie die Koordinaten des End-Punktes an: " << std::endl;
//		for (int i = 0; i < 3; i++)
//			{
//			std::cin >> end_pos[i];
//			}	
//		
//		s1011(start_pos,top_pos, end_pos, shape, dim, &arc_seg, &jstat);
//
//		kurve.InitCurve(arc_seg);
//
//		}
//*/
//	s1022(bottom_pos, bottom_axis, ellipse_ratio, axis_dir, cone_angle, height, &surface, &jstat);
//	s1021(bottom_pos2, bottom_axis2, ellipse_ratio2, axis_dir2, height2, &cylinder, &jstat);
//
//	kurve.InitSurf(cylinder);
//	kurve.InitSurf(surface);
//	kurve.writeGeometry(filename);
//	kurve.CloseFile(filename);	
//
//	return 0;
//}

