#include "dtSislBase.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <sstream>

namespace dtOO {
  dtSislBase::dtSislBase() {
    _minPointDistance
    = 
    staticPropertiesHandler::getInstance()->getOptionFloat("minPointDistance");
  
    _geometricResolution
    =
    staticPropertiesHandler::getInstance()->getOptionFloat("geometricResolution");
    
    setErrStr();
    setKindStr();
  }

  dtSislBase::dtSislBase(const dtSislBase& orig) {
    _minPointDistance = orig._minPointDistance;
    _geometricResolution =orig._geometricResolution;
    
    setErrStr();
    setKindStr();
  }

  dtSislBase::~dtSislBase() {
    dt__FORALL(errStr, ii,
      errStr[ii].clear();
    );
    errStr.clear();
    dt__FORALL(curveKindStr, ii,
      curveKindStr[ii].clear();
    );
    curveKindStr.clear();
    dt__FORALL(surfKindStr, ii,
      surfKindStr[ii].clear();
    );
    surfKindStr.clear(); 
    dt__FORALL(intCurveKindStr, ii,
      intCurveKindStr[ii].clear();
    );
    intCurveKindStr.clear();     
  }

  double & dtSislBase::refToMinPointDistance( void ) {
    return _minPointDistance;
  }
  
  double const & dtSislBase::refToMinPointDistance( void ) const {
    return _minPointDistance;
  }
  
  double & dtSislBase::refToGeometricResolution( void ) {
    return _geometricResolution;
  }
  
  double const & dtSislBase::refToGeometricResolution( void ) const {
    return _geometricResolution;
  }  
  
  bool dtSislBase::allRight(const int errFlag) const {
    return allRight(errFlag, __FILE__, __LINE__ );
  }
  
  bool dtSislBase::allRight(const int errFlag, std::string const file, int const line) const {
    if (errFlag < 0) {
      dt__THROW(allRight(),
              << DTLOGEVAL(errFlag) << LOGDEL
              << "> " << "called from:" << LOGDEL
              << "> " << DTLOGEVAL(file) << LOGDEL
              << "> " << DTLOGEVAL(line) << LOGDEL
              << "SISL ERROR MESSAGE::> " << errStr[-errFlag-100-1].c_str() );
      return false;
    }
    else if (errFlag > 0) {
      DTWARNINGWF(allRight(),
              << DTLOGEVAL(errFlag) );
    }
    return true;
  }

  void dtSislBase::setKindStr( void ) {
    curveKindStr.clear();
    curveKindStr.push_back("Polynomial B-spline curve");
    curveKindStr.push_back("Rational B-spline curve");
    curveKindStr.push_back("Polynomial Bezier curve");
    curveKindStr.push_back("Rational Bezier curve");
    surfKindStr.clear();
    surfKindStr.push_back("Polynomial B-spline tensor-product");
    surfKindStr.push_back("Rational B-spline tensor-product");
    surfKindStr.push_back("Polynomial Bezier tensor-product");
    surfKindStr.push_back("Rational Bezier tensor-product");  
    intCurveKindStr.clear();
    intCurveKindStr.push_back("Straight line");
    intCurveKindStr.push_back("Closed loop. No singularities.");
    intCurveKindStr.push_back("Closed loop. One singularity. Not used.");
    intCurveKindStr.push_back("Open curve. No singularity.");
    intCurveKindStr.push_back("Open curve. Singularity at the beginning of the curve.");
    intCurveKindStr.push_back("Open curve. Singularity at the end of the curve.");
    intCurveKindStr.push_back("Open curve. Singularity at the beginning  and end of the curve.");
    intCurveKindStr.push_back("An isolated singularity. Not used.");
    intCurveKindStr.push_back("The curve is exact, pgeom and either ppar1 or ppar2 is set.");
  }
  
  void dtSislBase::setErrStr( void ) {
    errStr.clear();
    errStr.push_back("err101 -101 Error in memory allocation.");
    errStr.push_back("err102 -102 Error in input. Dimension less than 1.");
    errStr.push_back("err103 -103 Error in input. Dimension less than 2.");
    errStr.push_back("err104 -104 Error in input. Dimension not equal 3.");
    errStr.push_back("err105 -105 Error in input. Dimension not equal 2 or 3.");
    errStr.push_back("err106 -106 Error in input. Conflicting dimensions.");
    errStr.push_back("err107 -107");
    errStr.push_back("err108 -108 Error in input. Dimension not equal 2.");
    errStr.push_back("err109 -109 Error in input. Order less than 2.");
    errStr.push_back("err110 -110 Error in Curve description. Order less than 1.");
    errStr.push_back("err111 -111 Error in Curve description. Number of vertices less than order.");
    errStr.push_back("err112 -112 Error in Curve description. Error in knot vector.");
    errStr.push_back("err113 -113 Error in Curve description. Unknown kind of Curve.");
    errStr.push_back("err114 -114 Error in Curve description. Open Curve when expecting closed.");
    errStr.push_back("err115 -115 Error in Surf description. Order less than 1.");
    errStr.push_back("err116 -116 Error in Surf description. Number of vertices less than order.");
    errStr.push_back("err117 -117 Error in Surf description. Error in knot vector.");
    errStr.push_back("err118 -118 Error in Surf description. Unknown kind of Surf.");
    errStr.push_back("err119 -119");
    errStr.push_back("err120 -120 Error in input. Negative relative tolerance.");
    errStr.push_back("err121 -121 Error in input. Unknown kind of Object.");
    errStr.push_back("err122 -122 Error in input. Unexpected kind of Object found.");
    errStr.push_back("err123 -123 Error in input. Parameter direction does not exist.");
    errStr.push_back("err124 -124 Error in input. Zero length parameter interval.");
    errStr.push_back("err125 -125");
    errStr.push_back("err126 -126");
    errStr.push_back("err127 -127 Error in input. The whole curve lies on axis.");
    errStr.push_back("err128 -128");
    errStr.push_back("err129 -129");
    errStr.push_back("err130 -130 Error in input. Parameter value is outside parameter area.");
    errStr.push_back("err131 -131");
    errStr.push_back("err132 -132");
    errStr.push_back("err133 -133");
    errStr.push_back("err134 -134");
    errStr.push_back("err135 -135 Error in data structure. Intersection point exists when it should not.");
    errStr.push_back("err136 -136 Error in data structure. Intersection list exists when it should not.");
    errStr.push_back("err137 -137 Error in data structure. Expected intersection point not found.");
    errStr.push_back("err138 -138 Error in data structure. Wrong number of intersections on edges/endpoints.");
    errStr.push_back("err139 -139 Error in data structure. Edge intersection does not lie on edge/endpoint.");
    errStr.push_back("err140 -140 Error in data structure. Intersection interval crosses subdivision line when not expected to.");
    errStr.push_back("err141 -141 Error in input. Illegal edge point requested.");
    errStr.push_back("err142 -142");
    errStr.push_back("err143 -143");
    errStr.push_back("err144 -144 Unknown kind of intersection curve.");
    errStr.push_back("err145 -145 Unknown kind of intersection list (internal format).");
    errStr.push_back("err146 -146 Unknown kind of intersection type.");
    errStr.push_back("err147 -147");
    errStr.push_back("err148 -147");
    errStr.push_back("err149 -149");
    errStr.push_back("err150 -150 Error in input. NULL pointer was given.");
    errStr.push_back("err151 -151 Error in input. One or more illegal input values.");
    errStr.push_back("err152 -152 Too many knots to insert.");
    errStr.push_back("err153 -153 Lower level routine reported error. SHOULD use label ""error"".");
    errStr.push_back("err154 -154");
    errStr.push_back("err155 -155");
    errStr.push_back("err156 -156 Illegal derivative requested. Change this label to err178.");
    errStr.push_back("err157 -157");
    errStr.push_back("err158 -158 Intersection point outside Curve.");
    errStr.push_back("err159 -159 No of vertices less than 1. SHOULD USE err111 or err116.");
    errStr.push_back("err160 -160 Error in dimension of interpolation problem.");
    errStr.push_back("err161 -161 Error in interpolation problem.");
    errStr.push_back("err162 -162 Matrix may be noninvertible.");
    errStr.push_back("err163 -163 Matrix part contains diagonal elements.");
    errStr.push_back("err164 -164 No point conditions specified in interpolation problem.");
    errStr.push_back("err165 -165 Error in interpolation problem.");
    errStr.push_back("err166 -166");
    errStr.push_back("err167 -167");
    errStr.push_back("err168 -168");
    errStr.push_back("err169 -169");
    errStr.push_back("err170 -170 Internal error: Error in moving knot values.");
    errStr.push_back("err171 -171 Memory allocation failure: Could not create curve or surface.");
    errStr.push_back("err172 -172 Input error, inarr < 1 || inarr > 3.");
    errStr.push_back("err173 -173 Direction vector zero length.");
    errStr.push_back("err174 -174 Degenerate condition.");
    errStr.push_back("err175 -175 Unknown degree/type of implicit surface.");
    errStr.push_back("err176 -176 Unexpected iteration situation.");
    errStr.push_back("err177 -177 Error in input. Negative step length requested.");
    errStr.push_back("err178 -178 Illegal derivative requested.");
    errStr.push_back("err179 -179 No. of Curves < 2.");
    errStr.push_back("err180 -180 Error in torus description.");
    errStr.push_back("err181 -181 Too few points as input.");
    errStr.push_back("err182 -182");
    errStr.push_back("err183 -183 Order(s) specified to low.");
    errStr.push_back("err184 -184 Negative tolerance given.");
    errStr.push_back("err185 -185 Only degenerate or singular guide points.");
    errStr.push_back("err186 -186 Special error in traversal of curves.");
    errStr.push_back("err187 -187 Error in description of input curves.");
    errStr.push_back("err188 -188");
    errStr.push_back("err189 -189");
    errStr.push_back("err190 -190 Too small array for storing Curve segments.");
    errStr.push_back("err191 -191 Error in inserted parameter number.");
    errStr.push_back("err192 -192");
    errStr.push_back("err193 -193");
    errStr.push_back("err194 -194");
    errStr.push_back("err195 -195");
    errStr.push_back("err196 -196");
    errStr.push_back("err197 -197");
    errStr.push_back("err198 -198");
    errStr.push_back("err199 -199 Error in vectors?");
  }
}