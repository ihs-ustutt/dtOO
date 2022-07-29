#include "bSplineSurface_patchByPatchConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>

#include <geometryEngine/geoBuilder/trimmedCurve_uBounds.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointInterpolateConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/geomFillBoundWithSurf_surfaceConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_bSplineCurveSplitConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_constrainedFillingConstructOCC.h>
#include <geometryEngine/geoBuilder/compositeSurface_surfaceConnectConstructOCC.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineSurface_patchByPatchConstructOCCAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineSurface_patchByPatchConstructOCCAFXmlBuilder, 
      new bSplineSurface_patchByPatchConstructOCCAFXmlBuilder()
    )
  );
  
  bSplineSurface_patchByPatchConstructOCCAFXmlBuilder
    ::bSplineSurface_patchByPatchConstructOCCAFXmlBuilder() {
  }

  bSplineSurface_patchByPatchConstructOCCAFXmlBuilder
    ::~bSplineSurface_patchByPatchConstructOCCAFXmlBuilder() {

  }

  void bSplineSurface_patchByPatchConstructOCCAFXmlBuilder::createWire(
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    cVPtrVec const * const cV, 
    aFPtrVec const * const aF,          
    std::vector< dtReal > const & wire,
    twoDArrayHandling< dt__pH(dtCurve) > & cc,
    twoDArrayHandling< dtPoint3 > & pp
  ) const {
    
    logContainer< bSplineSurface_patchByPatchConstructOCCAFXmlBuilder > logC(
      TLogLevel::logINFO, "createWire()"
    );
    
    std::vector< ::QDomElement > elV 
    = 
    dtXmlParserBase::getChildVector("analyticFunction", toBuild);
    
    cc.resize( elV.size(), wire.size()+1 );    
    pp.resize( elV.size(), wire.size()+2 );    
    
    logC()
      << "Create wire: size " << elV.size() << " x " << wire.size() << " )\n";
       
    //
    // loop all analyticFunctions
    //
    dt__forAllIndex( elV, ii ) {
      logC() << "Process Curve[ " << ii << " ]\n";
      
      //
      // cast
      //
      dt__pH(analyticFunction) theF(
        dtXmlParserBase::createAnalyticFunction( elV[ii], bC, cV, aF )
      );
      dt__ptrAss(
        vec3dCurveOneD const * v3d1d,
        vec3dCurveOneD::ConstDownCast(theF.get())
      );
      
      //
      // check if split is necessary
      //
      if (wire.size()==0) { // no split necessary
        logC() << "  - no split necessary.\n";
        cc[ii][0].reset( v3d1d->ptrConstDtCurve()->clone() );
      }
      else { // split curves at points in wire
        logC() 
          << "  - split 0% <-> " << wire.front()*100. << "%\n";
        cc[ii][0].reset( 
          trimmedCurve_uBounds(
            v3d1d->ptrConstDtCurve(), 
            v3d1d->ptrConstDtCurve()->u_uPercent(0.0), 
            v3d1d->ptrConstDtCurve()->u_uPercent(wire.front())
          ).result()
        );
        dt__forFromToIndex(1, wire.size(), jj) {
          logC() 
            << "  - split " 
            << wire[jj-1]*100. << "% <-> " << wire[jj]*100. << "%\n";
          cc[ii][jj].reset( 
            trimmedCurve_uBounds(
              v3d1d->ptrConstDtCurve(), 
              v3d1d->ptrConstDtCurve()->u_uPercent(wire[jj-1]), 
              v3d1d->ptrConstDtCurve()->u_uPercent(wire[jj])
            ).result()
          );          
        }
        logC() 
          << "  - split " << wire.back()*100. << "% <-> 1.0 \n";
        cc[ii].back().reset( 
          trimmedCurve_uBounds(
            v3d1d->ptrConstDtCurve(), 
            v3d1d->ptrConstDtCurve()->u_uPercent(wire.back()), 
            v3d1d->ptrConstDtCurve()->u_uPercent(1.0)
          ).result()
        );
      }
      
      //
      // insert start point
      //
      logC() << "  - insert point 0%\n";
      pp[ii][0] = cc[ii][0]->pointPercent( 0.0 );     
      
      //
      // inser points in between
      //
      dt__forAllIndex(wire, jj) {
        logC() << "  - insert point " << wire[jj]*100. << "%\n";
        pp[ii][jj+1] = cc[ii][jj+1]->pointPercent( 0.0 );
      }
      //
      // insert end point
      //      
      logC() << "  - insert point 100%\n";
      pp[ii].back() = cc[ii].back()->pointPercent( 1.0 );      
    }
  }
        
  dt__pH(dtSurface) bSplineSurface_patchByPatchConstructOCCAFXmlBuilder::sortAndInit(
    dtInt const & fixJ, twoDArrayHandling< dt__pH(dtCurve) > const & cc,
    vectorHandling< dtCurve const * > & ccFill,
    dtInt const & skin_nInitCuts, 
    dtInt const & skin_minDeg, 
    dtInt const & skin_maxDeg, 
    dtInt const & skin_nIter
  ) const {
    vectorHandling< dtCurve const * > ccSkin;
    dt__forFromToIndex(0, cc.size(0), ii ) {
      if ( ii < skin_nInitCuts ) ccSkin.push_back( cc[ii][fixJ].get() );
      else ccFill.push_back( cc[ii][fixJ].get() );
    }

    return 
      dt__pH(dtSurface)(
        bSplineSurface_skinConstructOCC(
          ccSkin, skin_minDeg, skin_maxDeg, skin_nIter
        ).result()
      );       
  }
        
  dtSurface * bSplineSurface_patchByPatchConstructOCCAFXmlBuilder::fillPatch(
    dtSurface const * const boundSurf_0,
    dtCurve const * const boundCurv_1, //dtC[1]
    dtCurve const * const boundCurv_2,//ccFill[kk]
    dtCurve const * const boundCurv_3, //dtC[0]
    dtInt const & fill_maxDeg,
    dtInt const & fill_maxSeg
  ) const {
    dtReal uMin = boundSurf_0->minPara(0);
    dtReal uMax = boundSurf_0->maxPara(0);
    dtReal vMax = boundSurf_0->maxPara(1);
    dt__pH(dtCurve) bound_0(
      geomFillBoundWithSurf_surfaceConstructOCC( 
        boundSurf_0, 
        dt__pH(dtCurve2d)(
          bSplineCurve2d_pointConstructOCC(
            dtPoint2(uMin, vMax), dtPoint2(uMax, vMax)
          ).result()
        ).get()
      ).result()
    );
    dt__pH(dtCurve) bound_1(
      bSplineCurve_bSplineCurveSplitConstructOCC( 
        boundCurv_1,
        boundCurv_1->reparam( bound_0->pointPercent(1.0) ), 
        boundCurv_1->reparam( boundCurv_2->pointPercent(1.0) )
      ).result()
    );
    dt__pH(dtCurve) bound_2( boundCurv_2->clone() );
    dt__pH(dtCurve) bound_3(
      bSplineCurve_bSplineCurveSplitConstructOCC( 
        boundCurv_3, 
        boundCurv_3->reparam( bound_0->pointPercent(0.0) ),               
        boundCurv_3->reparam( boundCurv_2->pointPercent(0.0) )
      ).result()
    );
    
    return bSplineSurface_constrainedFillingConstructOCC(
      bound_0.get(), bound_1.get(), bound_2.get(), bound_3.get(),
      fill_maxDeg, fill_maxSeg
    ).result();
  }
    
  void bSplineSurface_patchByPatchConstructOCCAFXmlBuilder::buildPart(
		::QDomElement const & toBuild, 
		baseContainer * const bC,
		cVPtrVec const * const cV, 
		aFPtrVec const * const aF,
		aFPtrVec * result 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticFunction", toBuild)
      ||
      !dtXmlParserBase::hasAttribute(
        "fill_maxSeg|fill_maxDeg|skin_nInitCuts|skin_minDeg|skin_maxDeg|"
        "skin_nIter|pointInterpolate_degMin|pointInterpolate_degMax|"
        "composite|wire", 
        toBuild
      ),
      buildPart()
    );   

    //
    // logContainer
    //    
    logContainer< bSplineSurface_patchByPatchConstructOCCAFXmlBuilder > logC(
      TLogLevel::logINFO, "buildPart()"
    );
    
    std::vector< dtReal > wire 
    =  
    dtXmlParserBase::getAttributeFloatVectorMuParse("wire", toBuild, cV, aF);
    
    //
    // check type of analyticFunctions and get start and end points
    //
    twoDArrayHandling< dt__pH(dtCurve) > cc;
    twoDArrayHandling< dtPoint3 > pp;
    
    createWire(toBuild, bC, cV, aF, wire, cc, pp);
   
    //
    // init pointer vector handlings
    //
    twoDArrayHandling< dt__pH(dtSurface) > dtS(wire.size()+1, 0);
    logC() << "dtS size ( " << dtS.size(0) << " x " << dtS.size(1) << " )\n";

    dt__forFromToIndex(0, cc.size(1), jj) {      
      logC() << "  - Handle curves cc[*][" << jj << "].\n";
      
      //
      // pointInterpolateConstruct start and end points
      //
      logC() << "  - Create B-Spline at fixJ( " << jj << " )\n";      
      vectorHandling< dt__pH(dtCurve) > dtC;
      dtC.push_back(
        dt__pH(dtCurve)(
          bSplineCurve_pointInterpolateConstructOCC(
            pp.fixJ(jj), 
            dtXmlParserBase::getAttributeInt(
              "pointInterpolate_degMin", toBuild
            ),
            dtXmlParserBase::getAttributeInt(
              "pointInterpolate_degMax", toBuild
            )
          ).result()
        )
      );
      logC() << "  - Create B-Spline at fixJ( " << jj+1 << " )\n";
      dtC.push_back(
        dt__pH(dtCurve)(
          bSplineCurve_pointInterpolateConstructOCC(
            pp.fixJ(jj+1), 
            dtXmlParserBase::getAttributeInt(
              "pointInterpolate_degMin", toBuild
            ),
            dtXmlParserBase::getAttributeInt(
              "pointInterpolate_degMax", toBuild
            )
          ).result()
        )
      );
      
      vectorHandling< dtCurve const * > ccFill;
      //
      // skin first cuts
      //
      dtS[jj].push_back(
        sortAndInit(
          jj, cc, ccFill, 
          dtXmlParserBase::getAttributeInt("skin_nInitCuts", toBuild),
          dtXmlParserBase::getAttributeInt("skin_minDeg", toBuild),
          dtXmlParserBase::getAttributeInt("skin_maxDeg", toBuild),
          dtXmlParserBase::getAttributeInt("skin_nIter", toBuild)
        )
      );
      logC() << "  - Push back surface to dtS[" << jj << "] -> dtS size ( " 
        << dtS.size(0) << " x " << dtS.size(1) << " )\n";             
      
      //
      // fill other curves
      //
      dt__forFromToIndex(0, ccFill.size(), kk) {
        logC() 
          << "    - Filling curve " << kk << " / " << ccFill.size() << " .\n";
        dtS[jj].push_back( 
          dt__pH(dtSurface)(
            fillPatch(
              dtS[jj].back().get(), dtC[1].get(), ccFill[kk], dtC[0].get(),
              dtXmlParserBase::getAttributeInt("fill_maxDeg", toBuild),
              dtXmlParserBase::getAttributeInt("fill_maxSeg", toBuild)            
            )
          )
        );
        logC() 
          << "    |- dtS size ( " 
          << dtS.size(0) << " x " << dtS.size(1) << " )\n";
      } 
    }
    cc.clear();
    
    //
    // composite
    //
    if ( dtXmlParserBase::getAttributeBool("composite", toBuild) ) {
      logC()
        << "Create composite.\n"
        << "dtS size ( " << dtS.size(0) << " x " << dtS.size(1) << " )\n";
      
      twoDArrayHandling< dtSurface const * > ssV0(dtS.size(0), 1);
      twoDArrayHandling< dtSurface const * > ssV1(dtS.size(0), dtS.size(1)-1);
      dt__forAllIndex(dtS, ii) dt__forAllIndex(dtS[ii], jj) {
        if ( jj==0 ) ssV0[ii][jj] = dtS[ii][jj].get();
        else {
          ssV1[ii][jj-1] = dtS[ii][jj].get();
        }
        
      };
      result->push_back( 
        new vec3dSurfaceTwoD(
          dt__tmpPtr(
            dtSurface, 
            compositeSurface_surfaceConnectConstructOCC(ssV0).result()
          )
        )
      );      
      result->push_back( 
        new vec3dSurfaceTwoD(
          dt__tmpPtr(
            dtSurface, 
            compositeSurface_surfaceConnectConstructOCC(ssV1).result()
          )
        )
      );      
    }
    else {
      dt__forAllIndex(dtS, ii) dt__forAllIndex(dtS[ii], jj) {      
        result->push_back( new vec3dSurfaceTwoD( dtS[ii][jj].get() ) );              
      }
    }
  }
}