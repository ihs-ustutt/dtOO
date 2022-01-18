#include "meshWhisperer.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include <gmsh/GEntity.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>
#include <gmsh/MLine.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MPrism.h>
#include <gmsh/MElementCut.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include <algorithm>

namespace dtOO {
  meshWhisperer::meshWhisperer( dtGmshModel * const gm )
    : _gm(gm), 
      _thisRank(staticPropertiesHandler::getInstance()->thisRank()),
      _nRanks(staticPropertiesHandler::getInstance()->nRanks()) {        
  }

  meshWhisperer::~meshWhisperer() {
  }
  
  void meshWhisperer::add(::GEntity * ge ) {
    if ( dtGmshEdge::ConstDownCast(ge) ) {
      dtGmshEdge * thisEdge = dtGmshEdge::SecureCast(ge);
      
      std::vector< ::MVertex * > & vertices = thisEdge->mesh_vertices;
      dt__forAllRefAuto(vertices, aVert) {
        add(aVert);
      }

      std::vector< ::MLine * > & line = thisEdge->lines;
      dt__forAllRefAuto(line, aLine) {
        add( (::MElement*) aLine, thisEdge );
      }     
    }
    else if ( dtGmshFace::ConstDownCast(ge) ) {
      dtGmshFace * thisFace = dtGmshFace::SecureCast(ge);
      
      std::vector< ::MVertex * > & vertices = thisFace->mesh_vertices;
      dt__forAllRefAuto(vertices, aVert) {
        add(aVert);
      }

      std::vector< ::MTriangle * > & tri = thisFace->triangles;
      dt__forAllRefAuto(tri, aTri) {
        add( (::MElement*) aTri, thisFace );
      }      
      std::vector< ::MQuadrangle * > & quad = thisFace->quadrangles;
      dt__forAllRefAuto(quad, aQuad) {
        add( (::MElement*) aQuad, thisFace );
      }       
//      _generalInfo().push_back()
      std::vector< std::vector< ::MVertex * > > & tFV 
      = 
      thisFace->transfinite_vertices;
      if (tFV.size() != 0) {
        _generalInfo().append(
          ":transfinite_vertices(" 
          + 
          stringPrimitive::intToString(thisFace->tag()) + ","
          +
          stringPrimitive::intToString(tFV.size()) + ","
          + 
          stringPrimitive::intToString(tFV[0].size())
          +
          "):"
        );
      }
      dt__forAllIndex(tFV, ii) {
        dt__forAllIndex(tFV[ii], jj) {
          makeTransfinite( tFV[ii][jj], thisFace->tag(), ii, jj );
        }
      }             
    }
    else if ( dtGmshRegion::ConstDownCast(ge) ) {
      dtGmshRegion * thisRegion = dtGmshRegion::SecureCast(ge);
      
      std::vector< ::MVertex * > & vertices = thisRegion->mesh_vertices;
      dt__forAllRefAuto(vertices, aVert) {
        add(aVert);
      }

      std::vector< ::MHexahedron * > & hex = thisRegion->hexahedra;
      dt__forAllRefAuto(hex, aHex) {
        add( (::MElement*) aHex, thisRegion );
      }      
      std::vector< ::MPyramid * > & pyr = thisRegion->pyramids;
      dt__forAllRefAuto(pyr, aPyr) {
        add( (::MElement*) aPyr, thisRegion );
      }            
      std::vector< ::MTetrahedron * > & tet = thisRegion->tetrahedra;
      dt__forAllRefAuto(tet, aTet) {
        add( (::MElement*) aTet, thisRegion );
      }                  
      std::vector< ::MPrism * > & pri = thisRegion->prisms;
      dt__forAllRefAuto(pri, aPri) {
        add( (::MElement*) aPri, thisRegion );
      }                        
      std::vector< ::MPolyhedron * > & pol = thisRegion->polyhedra;
      dt__forAllRefAuto(pol, aPol) {
        add( (::MElement*) aPol, thisRegion );
      }                              
//      std::vector< ::MQuadrangle * > & quad = thisFace->quadrangles;
//      dt__forAllRefAuto(quad, aQuad) {
//        add( (::MElement*) aQuad, thisFace );
//      }       
//      std::vector< std::vector< ::MVertex * > > & tFV 
//      = 
//      thisFace->transfinite_vertices;
//      if (tFV.size() != 0) {
//        _generalInfo().append(
//          ":transfinite_vertices(" 
//          + 
//          stringPrimitive::intToString(thisFace->tag()) + ","
//          +
//          stringPrimitive::intToString(tFV.size()) + ","
//          + 
//          stringPrimitive::intToString(tFV[0].size())
//          +
//          "):"
//        );
//      }
//      dt__forAllIndex(tFV, ii) {
//        dt__forAllIndex(tFV[ii], jj) {
//          makeTransfinite( tFV[ii][jj], thisFace->tag(), ii, jj );
//        }
//      }      
    }    
    else dt__throwUnexpected(add());
    
  }
  
  void meshWhisperer::add( ::MVertex * mv ) {
    _mv.push_back( mv );
    _x().push_back( mv->x() );
    _y().push_back( mv->y() );
    _z().push_back( mv->z() );
    _onWhatTag().push_back( mv->onWhat()->tag() );
    _onWhatDim().push_back( mv->onWhat()->dim() );
    _num().push_back( mv->getNum() );
    _mvType().push_back(0);  
    _u().push_back(0.);
    _v().push_back(0.);
      
    if ( dynamic_cast< ::MEdgeVertex * >(mv) ) {
      _mvType().back() = 1;  
      dynamic_cast< ::MEdgeVertex * >(mv)->getParameter(0, _u().back());      
    }
    else if ( dynamic_cast< ::MFaceVertex * >(mv) ) {
      _mvType().back() = 2;      
      dynamic_cast< ::MFaceVertex * >(mv)->getParameter(0, _u().back());      
      dynamic_cast< ::MFaceVertex * >(mv)->getParameter(1, _v().back());      
    }
  }

  void meshWhisperer::makeTransfinite( 
    ::MVertex const * const mv, 
    dtInt const & onWhatTag, 
    dtInt const & posU, dtInt const & posV 
  ) {
    std::vector< ::MVertex * >::iterator it 
    =
    std::find(_mv.begin(), _mv.end(), mv);
    dtInt pos = std::distance(_mv.begin(), it);
    if (it == _mv.end()) {
      pos = -1 * mv->getNum();
    }
    _tFVertexOnWhatTag().push_back(onWhatTag);
    _tFVertexPosU().push_back(posU);
    _tFVertexPosV().push_back(posV);
    _tFVertexPosition().push_back(pos);
  }  
  
  void meshWhisperer::add( ::MElement * me, ::GEntity * ge ) {
    _me.push_back( me );
    _meNum().push_back( me->getNum() );
    _meOnWhatDim().push_back( ge->dim() );
    _meOnWhatTag().push_back( ge->tag() );
    _meTypeMSH().push_back( me->getTypeForMSH() );
  }
  
  void meshWhisperer::distribute( void ) {
    if (staticPropertiesHandler::mpiParallel()) {
      dt__info(distribute(), << "Distributing " << _mv.size() << " vertices.");
    
      _generalInfo.distribute();
      _x.distribute();
      _y.distribute();
      _z.distribute();
      _u.distribute();
      _v.distribute();
      _mvType.distribute();
      _onWhatDim.distribute();
      _onWhatTag.distribute();
      _num.distribute();
      _tFVertexPosition.distribute();
      _tFVertexOnWhatTag.distribute();
      _tFVertexPosU.distribute();
      _tFVertexPosV.distribute();
      renumberVertices(0);
      addRenumberedVertices();

      dt__info(distribute(), << "Distributing " << _me.size() << " elements.");
        
      dt__forAllRefAuto(_me, anMe) {
        _meElementStartPosition().push_back( _meVertexNum().size() );        
        dt__forFromToIndex(0, anMe->getNumVertices(), ii) {
          _meVertexNum().push_back( anMe->getVertex(ii)->getNum() );
        }      
      }
      _meOnWhatTag.distribute();
      _meOnWhatDim.distribute();
      _meNum.distribute();
      _meElementStartPosition.distribute();
      _meVertexNum.distribute();      
      _meTypeMSH.distribute();         
      renumberElements(0);
      addRenumberedElements();   
      
      //
      // clear meshWhisperer
      //
      _x.clear();
      _y.clear();
      _z.clear();
      _u.clear();
      _v.clear();
      _mvType.clear();
      _onWhatTag.clear();
      _onWhatDim.clear();
      _num.clear();
      _mv.clear();      
      _meOnWhatTag.clear();
      _meOnWhatDim.clear();
      _meNum.clear();
      _meElementStartPosition.clear();
      _meVertexNum.clear();
      _meTypeMSH.clear();
      _me.clear(); 
      _tFVertexPosition.clear();
      _tFVertexOnWhatTag.clear();
      _tFVertexPosU.clear();
      _tFVertexPosV.clear();      
    }
  }
  
  void meshWhisperer::renumberVertices( dtInt const & leader ) {
    std::vector< dtInt > & refNum = _num.global()[ leader ];
    dtInt maxNum = *(std::max_element( refNum.begin(), refNum.end() ) );
    if (_thisRank == leader) {
      dt__throwIf( 
        _gm->getMaxVertexNumber()!= maxNum, renumberVertices()
      );
    }


    dt__forFromToIndex(0, _nRanks, ii) {
      if ( ii == leader ) continue;

      std::vector< dtInt > & aNum = _num.global()[ ii ];
      dt__forAllRefAuto(aNum, aNumEl) {
        //_gm->setMaxVertexNumber( _gm->getMaxVertexNumber() + 1 );
        maxNum++;
        aNumEl = maxNum;
      }
    }
    
    _gm->destroyMeshCaches();
  }
  
  void meshWhisperer::renumberElements( dtInt const & leader ) {
    std::vector< dtInt > & refNum = _meNum.global()[ leader ];
    dtInt maxNum = *(std::max_element( refNum.begin(), refNum.end() ) );
    if (_thisRank == leader) {
      dt__throwIf( 
        _gm->getMaxElementNumber()!= maxNum, renumberElements()
      );
    }

    dt__forFromToIndex(0, _nRanks, ii) {
      if ( ii == leader ) continue;

      std::vector< dtInt > & aNum = _meNum.global()[ ii ];
      dt__forAllRefAuto(aNum, aNumEl) {
//        //_gm->setMaxVertexNumber( _gm->getMaxVertexNumber() + 1 );
        maxNum++;
        aNumEl = maxNum;
      }
    }
    
    _gm->destroyMeshCaches();
//    addRenumberedVertices();    
  }
  
  void meshWhisperer::addRenumberedVertices( void ) {
    dtInt globalMaxVertexNum = -1;
    dt__forFromToIndex(0, _nRanks, rr) {
      dt__forFromToIndex(0, _num.global()[ rr ].size(), ii) {
        dtInt const thisNum = _num.global()[ rr ][ii];
        dtInt const thisOnWhatDim = _onWhatDim.global()[ rr ][ii];
        dtInt const thisOnWhatTag = _onWhatTag.global()[ rr ][ii];
        dtReal const thisX = _x.global()[ rr ][ii];
        dtReal const thisY = _y.global()[ rr ][ii];
        dtReal const thisZ = _z.global()[ rr ][ii];
        dtInt const thisMvType = _mvType.global()[ rr ][ii];
        double const thisU = _u.global()[ rr ][ii];
        double const thisV = _v.global()[ rr ][ii];        
        globalMaxVertexNum = std::max(globalMaxVertexNum, thisNum);
        
        if (rr != _thisRank) {
          ::MVertex * aNewMv = NULL;
          if (thisMvType == 0) {
            aNewMv = new ::MVertex(thisX, thisY, thisZ, NULL, thisNum);            
          }
          else if (thisMvType == 1) {
            aNewMv 
            = 
            new ::MEdgeVertex(thisX, thisY, thisZ, NULL, thisU, thisNum, -1.0);
          }
          else if (thisMvType == 2) {
            aNewMv 
            = 
            new ::MFaceVertex(thisX, thisY, thisZ, NULL, thisU, thisV, thisNum);
          } 
          else dt__throwUnexpected(addRenumberedVertices());
          
          if (thisOnWhatDim == 0) {
            _gm->getDtGmshVertexByTag( thisOnWhatTag )->addMeshVertex(
              aNewMv              
            );
            aNewMv->setEntity(_gm->getDtGmshVertexByTag( thisOnWhatTag ));
          }
          else if (thisOnWhatDim == 1) {
            _gm->getDtGmshEdgeByTag( thisOnWhatTag )->addMeshVertex(
              aNewMv              
            );            
            aNewMv->setEntity(_gm->getDtGmshEdgeByTag( thisOnWhatTag ));
          }
          else if (thisOnWhatDim == 2) {
            _gm->getDtGmshFaceByTag( thisOnWhatTag )->addMeshVertex(
              aNewMv              
            );                        
            aNewMv->setEntity(_gm->getDtGmshFaceByTag( thisOnWhatTag ));
          }
          else if (thisOnWhatDim == 3) {
            _gm->getDtGmshRegionByTag( thisOnWhatTag )->addMeshVertex(
              aNewMv              
            );                            
            aNewMv->setEntity(_gm->getDtGmshRegionByTag( thisOnWhatTag ));
          }
          else dt__throwUnexpected(addRenumberedVertices());
        }
        else {
          _mv[ ii ]->setNum( thisNum );
        }
      }
    }
    _gm->setMaxVertexNumber( globalMaxVertexNum );
    _gm->destroyMeshCaches();  
    
    dt__forFromToIndex(0, _nRanks, rr) {
      if (rr != _thisRank) {      
        std::string const & thisGeneralInfo = _generalInfo.global()[rr];
        std::vector< std::string > gVector 
        = 
        stringPrimitive::convertToStringVector(":", ":", thisGeneralInfo);
        dt__forAllRefAuto(gVector, anEntry) {
          dt__info(addRenumberedVertices(), << dt__eval(anEntry) );
          if ( 
            stringPrimitive::stringContains("transfinite_vertices", anEntry)
          ) {
            dt__info(
              addRenumberedVertices(), 
              << dt__eval(
                stringPrimitive::getStringBetween(
                  "transfinite_vertices(", ")", anEntry
                )
              )
            );
            std::vector< std::string > tFRule 
            = 
            stringPrimitive::convertToCSVStringVector(
              stringPrimitive::getStringBetween(
                "transfinite_vertices(", ")", anEntry
              )
            );
            dt__info(addRenumberedVertices(), << "tFRule = " << tFRule );            
            _gm->getDtGmshFaceByTag(
              stringPrimitive::stringToInt(tFRule[0])
            )->transfinite_vertices
            =
            std::vector< std::vector< ::MVertex * > >(
              stringPrimitive::stringToInt(tFRule[1]), 
              std::vector< ::MVertex * >(
                stringPrimitive::stringToInt(tFRule[2]), NULL
              )
            );
          }
        }
        dt__forFromToIndex(0, _tFVertexOnWhatTag.global()[ rr ].size(), ii) {
          dtInt const & tFVPosition = _tFVertexPosition.global()[ rr ][ii];
          dtInt const & tFVOnWhatTag = _tFVertexOnWhatTag.global()[ rr ][ii];
          dtInt const & tFVPosU = _tFVertexPosU.global()[ rr ][ii];
          dtInt const & tFVPosV = _tFVertexPosV.global()[ rr ][ii];
          
          dtInt globalNum;
          if (tFVPosition < 0) {
            globalNum = -1 * tFVPosition;
          }
          else {
            globalNum = _num.global()[ rr ][tFVPosition];
          }
          _gm->getDtGmshFaceByTag( 
            tFVOnWhatTag 
          )->transfinite_vertices[tFVPosU][tFVPosV]
          =
          _gm->getMeshVertexByTag( globalNum );
        }
      }
    }  
  }
  
  void meshWhisperer::addRenumberedElements( void ) {
    dtInt globalMaxElementNum = -1;
    dt__forFromToIndex(0, _nRanks, rr) {
      std::vector< dtInt > const & thisMeVertexNum = _meVertexNum.global()[ rr ];
      dt__forFromToIndex(0, _meNum.global()[ rr ].size(), ii) {
        dtInt const thisMeNum = _meNum.global()[ rr ][ii];
        dtInt const thisMeOnWhatDim = _meOnWhatDim.global()[ rr ][ii];
        dtInt const thisMeOnWhatTag = _meOnWhatTag.global()[ rr ][ii];
        dtInt const thisMeElementStartPosition 
        = 
        _meElementStartPosition.global()[ rr ][ii];
        dtInt const thisMeTypeMSH = _meTypeMSH.global()[ rr ][ii];     
        
        globalMaxElementNum = std::max(globalMaxElementNum, thisMeNum);
        
        std::vector< ::MVertex * > verts;
        dt__forFromToIndex(
          thisMeElementStartPosition, 
          thisMeElementStartPosition + ::MElement::getInfoMSH(thisMeTypeMSH),
          jj
        ) {
          verts.push_back( 
            _gm->getMeshVertexByTag( thisMeVertexNum[jj] )
          );
          dt__throwIf(
            _gm->getMeshVertexByTag( thisMeVertexNum[jj] )->getNum()
            !=
            thisMeVertexNum[jj],
            addRenumberedElements()
          );
        }
        if (rr != _thisRank) {
        
          ::MElement * aNewMe 
          = 
          ::MElementFactory().create(thisMeTypeMSH, verts, thisMeNum);          
          if (thisMeOnWhatDim == 1) {
            _gm->getDtGmshEdgeByTag( thisMeOnWhatTag )->addElement(aNewMe);
          }          
          else if (thisMeOnWhatDim == 2) {
            _gm->getDtGmshFaceByTag( thisMeOnWhatTag )->addElement(aNewMe);
          }
          else if (thisMeOnWhatDim == 3) {
            _gm->getDtGmshRegionByTag( thisMeOnWhatTag )->addElement(aNewMe);
          }
          else dt__throwUnexpected(addRenumberedElements());
        }
        else {
          _me[ ii ]->forceNum( thisMeNum );
        }
      }
    }    
    _gm->setMaxElementNumber( globalMaxElementNum );
    
    _gm->destroyMeshCaches();
  }
}

