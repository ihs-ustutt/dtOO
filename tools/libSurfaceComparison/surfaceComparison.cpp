#include "surfaceComparison.h"
#include <iostream>
#include <string>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <fstream>

using namespace std;

namespace dtOO {
  surfaceComparison::surfaceComparison() : dtPlugin() {
    dt__info(
      surfaceComparison(), 
      << "Create surfaceComparison"
    );
  }

  surfaceComparison::~surfaceComparison() {
    dt__info(
      surfaceComparison(), 
      << "Destroy surfaceComparison"
    );
  }
  
  dtPlugin * surfaceComparison::create( void ) const {
    return new surfaceComparison();
  }
  
  double surfaceComparison::UAbl_R ( double const * xx )const {
    dt__ptrAss( map2dTo3d  *surface2, map2dTo3d::DownCast( _refSurface ) );
    dtPoint2 Help=surface2->uv_percent( 
    static_cast< dtReal >(xx[0]), static_cast< dtReal >(xx[1]));
    const dtReal x=Help.x();
    const dtReal y=Help.y();
    return (_point - surface2->getPoint(x,y))
           * (surface2->firstDerU(x,y));
  }
    
  double surfaceComparison::VAbl_R ( double const * xx ) const {
    dt__ptrAss( map2dTo3d  *surface2, map2dTo3d::DownCast( _refSurface ) );
    dtPoint2  Help=surface2->uv_percent (
    static_cast< dtReal >(xx[0]), static_cast< dtReal >(xx[1]));
    const dtReal x=Help.x();
    const dtReal y=Help.y();
    return (_point - surface2->getPoint(x,y))
           * (surface2->firstDerV(x,y));
  }
        
  double surfaceComparison::Euclidean_Distance (double const *Wert) const {
    dt__ptrAss( map2dTo3d  *surface2, map2dTo3d::DownCast( _refSurface ));
    return sqrt((_point - surface2->getPointPercent(
            static_cast< dtReal >(Wert[0]),static_cast< dtReal >(Wert[1])))
            *(_point - surface2->getPointPercent(static_cast< dtReal >(Wert[0]), 
            static_cast< dtReal >(Wert[1]))));
  }
   
  dtInt surfaceComparison::Grinding(int Teiler) {
    double r1=_extension[0]-(_interval/(Teiler/10));
    if (r1<0)
      r1=0;
    double r2=_extension[0]+(_interval/(Teiler/10));
    if (r2>1)
      r2=1;    
    double g1=_extension[1]-(_interval/(Teiler/10));
    if (g1<0)
      g1=0;
    double g2=_extension[1]+(_interval/(Teiler/10));
    if (g2>1)
      g2=1;          
    
    for (double r=r1;r<r2;r=r+(_interval/(Teiler))) {
      for (double g=g1;g<g2;g=g+(_interval/(Teiler))) {       
        double x[2]={r,g};
        if (abs(UAbl_R(x)) < _auxiliary && abs(VAbl_R(x)) < _auxiliary) {                 
          if (abs(UAbl_R(x))<abs(VAbl_R(x)))
            _auxiliary=abs(VAbl_R(x));
          else
            _auxiliary=abs(UAbl_R(x));
          _extension[0]=x[0];
          _extension[1]=x[1];
        }
        if (abs(UAbl_R(x)) < _tolerance && abs(VAbl_R(x)) < _tolerance) {
          if (_printlevel>0) {
            cout << UAbl_R(x) << ", " << VAbl_R(x) << endl ;
            cout << "Root located!" << endl;
          }     
          _result[0]=x[0];
          _result[1]=x[1];
          return 0;
        }    
      }
    }
    return 1;
  }
    
  dtInt surfaceComparison::RootFinding() {
    double x[2];
    _auxiliary=1000;
    
    for (double i=0;i<=1.1;i=i+_interval) {
      for (double j=0;j<=1.1;j=j+_interval) {        
        double x[2]={i,j};
        if (abs(UAbl_R(x)) < _auxiliary && abs(VAbl_R(x)) < _auxiliary) {
          if (abs(UAbl_R(x))<abs(VAbl_R(x)))
            _auxiliary=abs(VAbl_R(x));
          else
            _auxiliary=abs(UAbl_R(x));
          _extension[0]=x[0];
          _extension[1]=x[1];
        }
        if (abs(UAbl_R(x)) < _tolerance && abs(VAbl_R(x)) < _tolerance) {
          cout << UAbl_R(x) << ", " << VAbl_R(x) << endl;
          if (_printlevel>0)
            cout << "Root located!" << endl;
          _result[0]=x[0];
          _result[1]=x[1];
          return 0;
        }
        if (j>1 && i>1) {
          dtInt a=10,f=0;
          do {
            dtInt k=Grinding(a);
            a=a*10;
            f++;
            if (k==0)
              return 0;
          }while (f<_precision);
          if (_printlevel>0)
            cout << "Root not found" << endl;
          if (_auxiliary>0.15)
            return 2;
          else
            return 1;
        }
      }
    }
  }
   
  void surfaceComparison::init( 
    ::QDomElement const & element,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtCase const * const dC,          
    lvH_dtPlugin const * const pL
  ) {
    try{
    dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);

    string surface = qtXmlPrimitive::getAttributeStr("surface", element);
    string refSurface= qtXmlPrimitive::getAttributeStr("refSurface", element);
    
    dt__info(
      surfaceComparison(),
      << "Init surfaceComparison"
    );    
    
    _surface= aG->get(surface);
    _refSurface= aG->get(refSurface);
    _printlevel=dtXmlParserBase::getAttributeIntMuParse("Printlevel", element,cV);
    _tolerance=dtXmlParserBase::getAttributeFloatMuParse("tolerance", element,cV);
    _transition = dtXmlParserBase::getAttributeFloatMuParse("interval", element,cV);
    _span = dtXmlParserBase::getAttributeFloatMuParse("span", element,cV);
    _precision = dtXmlParserBase::getAttributeFloatMuParse("precision", element,cV);
    
    }
    catch (std::exception& e) {
    cout << "exception caught: " << e.what() << '\n';
    }
  }

  void surfaceComparison::apply(void) {
    dt__ptrAss( map2dTo3d  *surface, map2dTo3d::DownCast( _surface) );
    ofstream fout;
    fout.open ("Evaluation.cvs");
    fout.imbue( locale("german") );
    
    if (_printlevel>1)
      fout << "x-value surface;y-value surface;x-value refsurface;"
              "y-value refsurface;derivative with respect to u;" 
              "derivative with respect to v;distance"<< endl;
    
    double average=0,maximumValue=0,minimumValue=1000,sum=0,values=0,Toleranz,
           good=0, medium=0, bad=0, GP, SP;

    if (_printlevel>0)
      cout << "Start" << endl;
    
    for (dtReal j=0;j<=1.01;j=j+_span) {
      for (dtReal i=0;i<=1.01;i=i+_span) {
        _interval=_transition;
        _point = surface->getPointPercent(j,i);
        values++;
        if (_printlevel>0)
          cout << "---------------------------------------------------" << endl;
        dtInt t=RootFinding();
        
        switch (t) {
          case 0: {   
            double * const us=_result;
            good++;
            if (_printlevel>1)
              fout << j << ";" << i << ";" << us[0] << ";" << us[1] << ";" 
                   << UAbl_R(us) << ";"  << VAbl_R(us) << ";"
                   << Euclidean_Distance(us) << ";"<< endl;
            sum= sum+Euclidean_Distance(us);
            if (maximumValue<Euclidean_Distance(us))
              maximumValue=Euclidean_Distance(us);
            if (minimumValue>Euclidean_Distance(us))
              minimumValue=Euclidean_Distance(us);
            }
            break;
            
          case 1: {
            double * const os=_extension;
            medium++;
            sum= sum+Euclidean_Distance(os);
            if (_printlevel>1)
              fout << j << ";" << i << ";" << os[0] << ";" << os[1] << ";" 
                   << UAbl_R(os) << ";"  << VAbl_R(os) << ";"
                   << Euclidean_Distance(os) << ";"<< endl;
            if (maximumValue<Euclidean_Distance(os))
              maximumValue=Euclidean_Distance(os);
            if (minimumValue>Euclidean_Distance(os))
              minimumValue=Euclidean_Distance(os);
            }
            break;
            
          case 2 : {
            double * const es=_extension;
            bad++;
            if (_printlevel>1)
              fout << j << ";" << i << ";" << es[0] << ";" << es[1] 
                   << ";" << UAbl_R(es) << ";"  << VAbl_R(es) << ";"
                   << Euclidean_Distance(es) << ";"<< endl;
            } 
            break;
          } 
      }
    }
    GP=100-(good/values)*100;
    SP=(bad/values)*100;
    values=values-bad;
    average=sum/values;
    fout << endl;
    fout << endl;
    fout << GP << "% ; of the roots are not beneath the tolerance " << endl;
    fout << SP << "% ; of the roots are not usabel (>0.15)" << endl;
    fout << "Average ;" << average << endl;
    fout << "Minimum Value ;" << minimumValue << endl;
    fout << "Maximum Value;" << maximumValue<< endl;
    fout.close();
    
    dt__info(
      surfaceComparison(), 
      << "Apply surfaceComparison"
    );          
  }
}
//
// pugg driver
//
surfaceComparisonDriver::surfaceComparisonDriver() : dtPluginDriver(
  std::string("surfaceComparisonDriver"), dtOO::dtPlugin::version
) { 

}

//
// pugg plugin creation via driver
//
dtOO::dtPlugin * surfaceComparisonDriver::create() {
  return new dtOO::surfaceComparison();
}

//
// registrate plugin in driver
//
#include <pluginEngine/pugg/Kernel.h>
extern "C" void register_pugg_plugin(pugg::Kernel* kernel) 
  {kernel->add_driver(new surfaceComparisonDriver());}


