#include "dtMeshCustom2x3TransfiniteGFace.h"
#include "dtMeshGFace.h"
#include <progHelper.h>

#include <xmlHeaven/qtXmlPrimitive.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <gmsh/meshGFace.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/Context.h>

#include <analyticFunctionHeaven/scaOneD.h>

namespace dtOO {
  void dtMeshCustom2x3TransfiniteGFace::computeEdgeLoops(
    const ::GFace *gf, 
    std::vector< ::MVertex * > &all_mvertices, 
    std::vector< dtInt > &indices,
    std::vector<MVertex*> & corners,
    std::vector <MVertex *> & m_vertices    
  ) {
    findTransfiniteCorners(const_cast<::GFace*>(gf), corners);
    dt__throwIf(corners.size()!= 4, computeEdgeLoops());
    
    std::vector<GEdge*> edges = gf->edges();
    std::vector<int> ori = gf->orientations();
    std::vector<GEdge*>::iterator it = edges.begin();
    std::vector<int>::iterator ito = ori.begin();

    indices.push_back(0);
    GVertex *start = ((*ito) == 1) ? (*it)->getBeginVertex() : (*it)->getEndVertex();
    GVertex *v_end = ((*ito) != 1) ? (*it)->getBeginVertex() : (*it)->getEndVertex();
    all_mvertices.push_back(start->mesh_vertices[0]);
    if(*ito == 1)
      for(dtUnsInt i = 0; i < (*it)->mesh_vertices.size(); i++)
        all_mvertices.push_back((*it)->mesh_vertices[i]);
    else
      for(int i = (*it)->mesh_vertices.size() - 1; i >= 0; i--)
        all_mvertices.push_back((*it)->mesh_vertices[i]);

    GVertex *v_start = start;
    while(1){
      ++it;
      ++ito;
      if(v_end == start){
        indices.push_back(all_mvertices.size());
        if(it == edges.end ()) break;
        start = ((*ito) == 1) ? (*it)->getBeginVertex() : (*it)->getEndVertex();
        v_end = ((*ito) != 1) ? (*it)->getBeginVertex() : (*it)->getEndVertex();
        v_start = start;
      }
      else{
        if(it == edges.end ()){
          Msg::Error("Something wrong in edge loop computation");
          return;
        }
        v_start = ((*ito) == 1) ? (*it)->getBeginVertex() : (*it)->getEndVertex();
        if(v_start != v_end){
          Msg::Error("Something wrong in edge loop computation");
          return;
        }
        v_end = ((*ito) != 1) ? (*it)->getBeginVertex() : (*it)->getEndVertex();
      }
      all_mvertices.push_back(v_start->mesh_vertices[0]);
      if(*ito == 1)
        for(dtUnsInt i = 0; i < (*it)->mesh_vertices.size(); i++)
          all_mvertices.push_back((*it)->mesh_vertices[i]);
      else
        for(int i = (*it)->mesh_vertices.size()-1; i >= 0; i--)
          all_mvertices.push_back((*it)->mesh_vertices[i]);
    }
    
    dt__throwIf(indices.size()!=2, operator());

    // create a list of all boundary vertices, starting at the first
    // transfinite corner

    dtUnsInt I;
    for(I = 0; I < all_mvertices.size(); I++)
      if(all_mvertices[I] == corners[0]) break;
    for(dtUnsInt j = 0; j < all_mvertices.size(); j++)
      m_vertices.push_back(all_mvertices[(I + j) % all_mvertices.size()]);

    // make the ordering of the list consistent with the ordering of the
    // first two corners (if the second found corner is not the second
    // corner, just reverse the list)
    bool reverse = false;
    for(dtUnsInt i = 1; i < m_vertices.size(); i++){
      MVertex *v = m_vertices[i];
      if(v == corners[1] || v == corners[2] ||
         (corners.size() == 4 && v == corners[3])){
        if(v != corners[1]) reverse = true;
        break;
      }
    }
    if(reverse){
      std::vector <MVertex *> tmp;
      tmp.push_back(m_vertices[0]);
      for(int i = m_vertices.size() - 1; i > 0; i--)
        tmp.push_back(m_vertices[i]);
      m_vertices = tmp;
    }    
  }

  dtMeshCustom2x3TransfiniteGFace::dtMeshCustom2x3TransfiniteGFace() : dtMesh2DOperator() {
    
  }

  dtMeshCustom2x3TransfiniteGFace::dtMeshCustom2x3TransfiniteGFace(
    const dtMeshCustom2x3TransfiniteGFace& orig
  ) : dtMesh2DOperator(orig) {
    
  }

  dtMeshCustom2x3TransfiniteGFace::~dtMeshCustom2x3TransfiniteGFace() {
    
  }

  void dtMeshCustom2x3TransfiniteGFace::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO      
  ) {
    dtMesh2DOperator::init(element, bC, cV, aF, aG, bV, mO);
    
    
    dt__ptrAss( 
      scaOneD const * alpha_1, 
      scaOneD::ConstDownCast(aF->get(qtXmlBase::getAttributeStr("alpha_1", element))) 
    );
    _alpha_1.reset( alpha_1->clone() );
    dt__ptrAss( 
      scaOneD const * alpha_2, 
      scaOneD::ConstDownCast(aF->get(qtXmlBase::getAttributeStr("alpha_2", element))) 
    );
    _alpha_2.reset( alpha_2->clone() );
    dt__ptrAss( 
      scaOneD const * beta_1, 
      scaOneD::ConstDownCast(aF->get(qtXmlBase::getAttributeStr("beta_1", element))) 
    );
    _beta_1.reset( beta_1->clone() );    
    dt__ptrAss( 
      scaOneD const * beta_2, 
      scaOneD::ConstDownCast(aF->get(qtXmlBase::getAttributeStr("beta_2", element))) 
    );
    _beta_2.reset( beta_2->clone() );     
    dt__ptrAss( 
      scaOneD const * beta_3, 
      scaOneD::ConstDownCast(aF->get(qtXmlBase::getAttributeStr("beta_3", element))) 
    );
    _beta_3.reset( beta_3->clone() );         
  }
      
  void dtMeshCustom2x3TransfiniteGFace::operator()( dtGmshFace * dtgf ) {

    dtgf->model()->setCurrentMeshEntity(dtgf);

    dt__throwIf(dtgf->meshAttributes.method!=MESH_TRANSFINITE, operator()); 

    // destroy the mesh if it exists
    deMeshGFace()(dtgf);    
    
    Msg::Info("Meshing surface %d ( dtMeshCustom2x3TransfiniteGFace )", dtgf->tag());

    std::vector<MVertex*> corners;
    std::vector<MVertex*> d_vertices;
    std::vector<int> indices;
    std::vector <MVertex *> m_vertices;
    computeEdgeLoops(dtgf, d_vertices, indices, corners, m_vertices);


    // get the indices of the interpolation corners as well as the u,v
    // coordinates of all the boundary vertices

    std::vector< double > U( m_vertices.size(), 0. );
    std::vector< double > V( m_vertices.size(), 0. );

    dtInt iCorner = 0, N[4] = {0, 0, 0, 0};  
    for(dtUnsInt i = 0; i < m_vertices.size(); i++){
      MVertex *v = m_vertices[i];
      if(
        v == corners[0] || v == corners[1] 
        || 
        v == corners[2] || v == corners[3]
      ) {
        dt__throwIf(iCorner>3, operator());
        N[iCorner++] = i;
      }
      SPoint2 param;
      reparamMeshVertexOnFace(m_vertices[i], dtgf, param);
      U[i] = param[0];
      V[i] = param[1];    
    }

    dtInt N1 = N[0], N2 = N[1], N3 = N[2], N4 = N[3];
    dtInt L = N2 - N1, H = N3 - N2;

    dtInt Lb = N4 - N3, Hb = m_vertices.size() - N4;
    dt__throwIf(Lb != L || Hb != H, operator());

    
    /*             k
        2L+H +------------+ L+H
             |            |
             |            |
           l |            | j
             |            |
       2L+2H +------------+
             0     i      L
    */

    std::vector<std::vector<MVertex*> > &tab(dtgf->transfinite_vertices);
    twoDArrayHandling< dtPoint2 > pUV(L+1, H+1, dtPoint2(0,0));
    twoDArrayHandling< dtVector3 > nXYZ(L+1, H+1, dtVector3(0,0,0));
    twoDArrayHandling< dtVector2 > nUV(L+1, H+1, dtVector2(0,0));
    twoDArrayHandling< dtVector2 > dF(L+1, H+1, dtVector2(0,0));
    twoDArrayHandling< dtVector2 > ddF(L+1, H+1, dtVector2(0,0));

    tab.resize(L + 1);
    for(int i = 0; i <= L; i++) tab[i].resize(H + 1);

    tab[0][0] = m_vertices[0];
    tab[L][0] = m_vertices[L];
    tab[L][H] = m_vertices[L+H];
    tab[0][H] = m_vertices[2*L+H];
    pUV[0][0] = dtPoint2(U[0], V[0]);
    pUV[L][0] = dtPoint2(U[L], V[L]);
    pUV[L][H] = dtPoint2(U[L+H], V[L+H]);
    pUV[0][H] = dtPoint2(U[2*L+H], V[2*L+H]);  
    for (int i = 1; i < L; i++){
      tab[i][0] = m_vertices[i];
      tab[i][H] = m_vertices[2*L+H-i];
      pUV[i][0] = dtPoint2(U[i], V[i]);
      pUV[i][H] = dtPoint2(U[2*L+H-i], V[2*L+H-i]);      
    }  
    for(int i = 1; i < H; i++){
      tab[L][i] = m_vertices[L+i];
      tab[0][i] = m_vertices[2*L+2*H-i];
      pUV[L][i] = dtPoint2(U[L+i], V[L+i]);
      pUV[0][i] = dtPoint2(U[2*L+2*H-i], V[2*L+2*H-i]);     
    }
  
    for (int i = 1; i < L; i++){
      nXYZ[i][0] 
      = 
      dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(
          dtGmshModel::extractPosition(tab[i+1][0]) 
          - 
          dtGmshModel::extractPosition(tab[i][0])
          ,
          dtgf->getMap2dTo3d()->normal(pUV[i][0])
        )
      )
      +
      dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(
          dtGmshModel::extractPosition(tab[i][0]) 
          - 
          dtGmshModel::extractPosition(tab[i-1][0])
          ,
          dtgf->getMap2dTo3d()->normal(pUV[i][0])
        )
      );     
      nXYZ[i][H] 
      = 
      dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(
          dtGmshModel::extractPosition(tab[i+1][H]) 
          - 
          dtGmshModel::extractPosition(tab[i][H])
          ,
          dtgf->getMap2dTo3d()->normal(pUV[i][H])
        )
      )
      +
      dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(
          dtGmshModel::extractPosition(tab[i][H]) 
          - 
          dtGmshModel::extractPosition(tab[i-1][H])
          ,
          dtgf->getMap2dTo3d()->normal(pUV[i][H])
        )
      );      
    }
    for(int i = 1; i < H; i++){
      nXYZ[L][i] 
      = 
      dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(
          dtGmshModel::extractPosition(tab[L][i+1]) 
          - 
          dtGmshModel::extractPosition(tab[L][i])
          ,
          dtgf->getMap2dTo3d()->normal(pUV[L][i])
        )
      )
      +
      dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(
          dtGmshModel::extractPosition(tab[L][i]) 
          - 
          dtGmshModel::extractPosition(tab[L][i-1])
          ,
          dtgf->getMap2dTo3d()->normal(pUV[L][i])
        )
      );     
      nXYZ[0][i] 
      = 
      dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(
          dtGmshModel::extractPosition(tab[0][i+1]) 
          - 
          dtGmshModel::extractPosition(tab[0][i])
          ,
          dtgf->getMap2dTo3d()->normal(pUV[0][i])
        )
      )
      +
      dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(
          dtGmshModel::extractPosition(tab[0][i]) 
          - 
          dtGmshModel::extractPosition(tab[0][i-1])
          ,
          dtgf->getMap2dTo3d()->normal(pUV[0][i])
        )
      );     
    }

    dtReal d_0_1 = (tab[0][0]->distance(tab[0][1]));
    dtReal d_0_2 = (tab[L][0]->distance(tab[L][1]));
    dtReal d_H_1 = (tab[0][H]->distance(tab[0][H-1]));
    dtReal d_H_2 = (tab[L][H]->distance(tab[L][H-1]));  

  
    for (int i = 0; i < L+1; i++) {
      dtReal iF = static_cast< dtReal >(i);
      nUV[i][0]
      =
      dtLinearAlgebra::toDtVector2(
        dtLinearAlgebra::solveMatrix(
          dtgf->getMap2dTo3d()->jacobi(pUV[i][0])
          ,
          dtLinearAlgebra::createMatrixVector(
            -(d_0_1 + (iF/L) * (d_0_2-d_0_1)) * dtLinearAlgebra::normalize( nXYZ[i][0] )      
          )
        )
      );
      //nUV[i][0] = dtVector2(1./L, nUV[i][0].y());
      nUV[i][H]
      =
      dtLinearAlgebra::toDtVector2(
        dtLinearAlgebra::solveMatrix(
          dtgf->getMap2dTo3d()->jacobi(pUV[i][H])
          ,
          dtLinearAlgebra::createMatrixVector(
            -(d_H_1 + (iF/L) * (d_H_2-d_H_1)) * dtLinearAlgebra::normalize( nXYZ[i][H] )
          )
        )
      );     
    }
    dtReal e_0_1 = (tab[1][0]->distance(tab[0][0]));
    dtReal e_0_2 = (tab[L][0]->distance(tab[L-1][0]));
    dtReal e_L_1 = (tab[1][H]->distance(tab[0][H]));
    dtReal e_L_2 = (tab[L][H]->distance(tab[L-1][H]));    

    for (int i = 0; i < H+1; i++) {
      dtReal iF = static_cast< dtReal >(i);    
      nUV[0][i]
      =
      dtLinearAlgebra::toDtVector2(
        dtLinearAlgebra::solveMatrix(
          dtgf->getMap2dTo3d()->jacobi(pUV[0][i])
          ,
          dtLinearAlgebra::createMatrixVector(
            (e_0_1 + (iF/H) * (e_0_2-e_0_1)) * dtLinearAlgebra::normalize( nXYZ[0][i] )
          )
        )      
      );
      nUV[L][i]
      =
      dtLinearAlgebra::toDtVector2(
        dtLinearAlgebra::solveMatrix(
          dtgf->getMap2dTo3d()->jacobi(pUV[L][i])
          ,
          dtLinearAlgebra::createMatrixVector(
            (e_L_1 + (iF/H) * (e_L_2-e_L_1)) * dtLinearAlgebra::normalize( nXYZ[L][i] )
          )
        )      
      );
    }  
    nUV[0][0] = .5 * (nUV[1][0] + nUV[0][1]);
    nUV[L][0] = .5 * (nUV[L][1] + nUV[L-1][0]);
    nUV[L][H] = .5 * (nUV[L][H-1] + nUV[L-1][H]);
    nUV[0][H] = .5 * (nUV[1][H] + nUV[0][H-1]);
    for (int i = 1; i < L; i++) {  
      ddF[i][0] = (nUV[i+1][0] - nUV[i-1][0])/(2./L);
      ddF[i][H] = (nUV[i+1][H] - nUV[i-1][H])/(2./L);
    }
    for (int i = 1; i < H; i++) {  
      ddF[0][i] = (nUV[0][i+1] - nUV[0][i-1])/(2./H);
      ddF[L][i] = (nUV[L][i+1] - nUV[L][i-1])/(2./H);    
    }
    //create points using transfinite interpolation
    for(int i = 1; i < L; i++){
      for(int j = 1; j < H; j++){
        dtReal iF = static_cast< dtReal >(i);
        dtReal jF = static_cast< dtReal >(j);

        double psi = iF / (L);
        double eps = jF / (H);
        dtReal alpha_1 = _alpha_1->YFloat(psi);
        dtReal alpha_2 = _alpha_2->YFloat(psi);

        dtReal beta_1 = _beta_1->YFloat(eps);
        dtReal beta_2 = _beta_2->YFloat(eps);
        dtReal beta_3 = _beta_3->YFloat(eps);
        
        dtVector2 P_u
        =
          alpha_1 * dtLinearAlgebra::toDtVector2(pUV[0][j])
        + alpha_2 * dtLinearAlgebra::toDtVector2(pUV[L][j]);
        dtVector2 P_v
        =
          beta_1 * dtLinearAlgebra::toDtVector2(pUV[i][0])
        + beta_2 * nUV[i][0]
        + beta_3 * dtLinearAlgebra::toDtVector2(pUV[i][H]);        
        dtVector2 P_uP_v
        =
          alpha_1 * beta_1 * dtLinearAlgebra::toDtVector2(pUV[0][0])
        + alpha_1 * beta_2 * nUV[0][0]          
        + alpha_1 * beta_3 * dtLinearAlgebra::toDtVector2(pUV[0][H])          
        + alpha_2 * beta_1 * dtLinearAlgebra::toDtVector2(pUV[L][0])          
        + alpha_2 * beta_2 * nUV[L][0]            
        + alpha_2 * beta_3 * dtLinearAlgebra::toDtVector2(pUV[L][H])
        ;        
        
        GPoint gp 
        = 
        dtgf->point(
          P_u.x() + P_v.x() - P_uP_v.x(), 
          P_u.y() + P_v.y() - P_uP_v.y()
        );
        
        pUV[i][j] = dtPoint2(0,0) + P_u + P_v -P_uP_v;
        MFaceVertex * newv 
        = 
        new MFaceVertex(
          gp.x(), gp.y(), gp.z(), 
          dtgf, 
          P_u.x() + P_v.x() - P_uP_v.x(), 
          P_u.y() + P_v.y() - P_uP_v.y()
        );
        dtgf->mesh_vertices.push_back(newv);
        tab[i][j] = newv;
        
      }
    }
  
    //
    // output
    //
    std::fstream of;
    of.open(
      (dtgf->getPhysicalString()+"_uv.dat").c_str(), 
      std::ios::out | std::ios::trunc
    );	
    of.precision(8);
    of.fixed;
    of 
      << "x coord, y coord, z coord, "
      << "normal x, normal y, normal z, "
      << "cross x, cross y, cross z" << std::endl;
    for (int i = 0; i < L+1; i++) {
      for (int j = 0; j < H+1; j++) {
        of 
        << pUV[i][j].x() << ", " << pUV[i][j].y() << ", 0. , "
        << nUV[i][j].x() << ", " << nUV[i][j].y() << ", 0., "
        << ddF[i][j].x() << ", " << ddF[i][j].y() << ", 0., "
        << std::endl;   
      }  
    }
    of.close();
  
    
    // create elements
    for(int i = 0; i < L ; i++){
      for(int j = 0; j < H; j++){
        MVertex *v1 = tab[i][j];
        MVertex *v2 = tab[i + 1][j];
        MVertex *v3 = tab[i + 1][j + 1];
        MVertex *v4 = tab[i][j + 1];
        if(CTX::instance()->mesh.recombineAll || dtgf->meshAttributes.recombine)
          dtgf->quadrangles.push_back(new MQuadrangle(v1, v2, v3, v4));
        else if(dtgf->meshAttributes.transfiniteArrangement == 1 ||
                (dtgf->meshAttributes.transfiniteArrangement == 2 &&
                 ((i % 2 == 0 && j % 2 == 1) ||
                  (i % 2 == 1 && j % 2 == 0))) ||
		(dtgf->meshAttributes.transfiniteArrangement == -2 &&
                 ((i % 2 == 0 && j % 2 == 0) ||
                  (i % 2 == 1 && j % 2 == 1)))
		){
          dtgf->triangles.push_back(new MTriangle(v1, v2, v3));
          dtgf->triangles.push_back(new MTriangle(v3, v4, v1));
        }
        else{
          dtgf->triangles.push_back(new MTriangle(v1, v2, v4));
          dtgf->triangles.push_back(new MTriangle(v4, v2, v3));
        }
      }
    }

    dtgf->meshStatistics.status = GFace::DONE;
  }
}