#include "dtMeshTransfiniteGFace.h"
#include "dtMeshGFace.h"

#include <xmlHeaven/qtXmlPrimitive.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <gmsh/meshGFace.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/Context.h>

#define SQU(a)      ((a)*(a))

/*
   s4 +-----c3-----+ s3
      |            |
      |            |
     c4            c2
      |            |
      |            |
   s1 +-----c1-----+ s2
*/

// f(u,v) = (1-u) c4(v) + u c2(v) + (1-v) c1(u) + v c3(u)
//          - [ (1-u)(1-v) s1 + u(1-v) s2 + uv s3 + (1-u)v s4 ]
#define TRAN_QUA(c1,c2,c3,c4,s1,s2,s3,s4,u,v) \
   (1.-u)*c4+u*c2+(1.-v)*c1+v*c3-((1.-u)*(1.-v)*s1+u*(1.-v)*s2+u*v*s3+(1.-u)*v*s4)

namespace dtOO {
  void dtMeshTransfiniteGFace::computeEdgeLoops(
    const ::GFace *gf, 
    std::vector< ::MVertex * > &all_mvertices, 
    std::vector< int > &indices
  ) {
    std::list<GEdge*> edges = gf->edges();
    std::list<int> ori = gf->orientations();
    std::list<GEdge*>::iterator it = edges.begin();
    std::list<int>::iterator ito = ori.begin();

    indices.push_back(0);
    GVertex *start = ((*ito) == 1) ? (*it)->getBeginVertex() : (*it)->getEndVertex();
    GVertex *v_end = ((*ito) != 1) ? (*it)->getBeginVertex() : (*it)->getEndVertex();
    all_mvertices.push_back(start->mesh_vertices[0]);
    if(*ito == 1)
      for(unsigned int i = 0; i < (*it)->mesh_vertices.size(); i++)
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
        for(unsigned int i = 0; i < (*it)->mesh_vertices.size(); i++)
          all_mvertices.push_back((*it)->mesh_vertices[i]);
      else
        for(int i = (*it)->mesh_vertices.size()-1; i >= 0; i--)
          all_mvertices.push_back((*it)->mesh_vertices[i]);
    }
  }

  dtMeshTransfiniteGFace::dtMeshTransfiniteGFace() : dtMesh2DOperator() {
    
  }

  dtMeshTransfiniteGFace::dtMeshTransfiniteGFace(
    const dtMeshTransfiniteGFace& orig
  ) : dtMesh2DOperator(orig) {
    
  }

  dtMeshTransfiniteGFace::~dtMeshTransfiniteGFace() {
    
  }

  void dtMeshTransfiniteGFace::operator()( dtGmshFace * dtgf ) {

    dtgf->model()->setCurrentMeshEntity(dtgf);

    if(dtgf->meshAttributes.method != MESH_TRANSFINITE) {
      dtMeshGFace()(dtgf);
      return;
    }

    // destroy the mesh if it exists
    deMeshGFace()(dtgf);    
    Msg::Info("Meshing surface %d ( dtMeshTransfiniteGFace )", dtgf->tag());

    std::vector<MVertex*> corners;
    findTransfiniteCorners(dtgf, corners);
    dt__throwIf(corners.size ()!= 4, operator());

    std::vector<MVertex*> d_vertices;
    std::vector<int> indices;
    computeEdgeLoops(dtgf, d_vertices, indices);

    dt__throwIf(indices.size ()!=2, operator());

    // create a list of all boundary vertices, starting at the first
    // transfinite corner
    std::vector <MVertex *> m_vertices;
    unsigned int I;
    for(I = 0; I < d_vertices.size(); I++)
      if(d_vertices[I] == corners[0]) break;
    for(unsigned int j = 0; j < d_vertices.size(); j++)
      m_vertices.push_back(d_vertices[(I + j) % d_vertices.size()]);

    // make the ordering of the list consistent with the ordering of the
    // first two corners (if the second found corner is not the second
    // corner, just reverse the list)
    bool reverse = false;
    for(unsigned int i = 1; i < m_vertices.size(); i++){
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

    // get the indices of the interpolation corners as well as the u,v
    // coordinates of all the boundary vertices

    std::vector< double > U( m_vertices.size(), 0. );
    std::vector< double > V( m_vertices.size(), 0. );

    int iCorner = 0, N[4] = {0, 0, 0, 0};  
    for(unsigned int i = 0; i < m_vertices.size(); i++){
      MVertex *v = m_vertices[i];
      if(v == corners[0] || v == corners[1] || v == corners[2] ||
         (corners.size() == 4 && v == corners[3])) {
        dt__throwIf(iCorner>3, operator());
        N[iCorner++] = i;
      }
      SPoint2 param;
      reparamMeshVertexOnFace(m_vertices[i], dtgf, param);
      U[i] = param[0];
      V[i] = param[1];    
    }

    int N1 = N[0], N2 = N[1], N3 = N[2], N4 = N[3];
    int L = N2 - N1, H = N3 - N2;
    if(corners.size() == 4){
      int Lb = N4 - N3, Hb = m_vertices.size() - N4;
      dt__throwIf(Lb != L || Hb != H, operator());
    }
    else{
      int Lb = m_vertices.size() - N3;
      dt__throwIf(Lb != L, operator());
    }

    std::vector<double> lengths_i, lengths_j;
    double L_i = 0, L_j = 0;
    lengths_i.push_back(0.);
    lengths_j.push_back(0.);
    for(int i = 0; i < L; i++){
      MVertex *v1 = m_vertices[i];
      MVertex *v2 = m_vertices[i + 1];
      L_i += v1->distance(v2);
      lengths_i.push_back(L_i);
    }
    for(int i = L; i < L + H; i++){
      MVertex *v1 = m_vertices[i];
      MVertex *v2 = m_vertices[i + 1];
      L_j += v1->distance(v2);
      lengths_j.push_back(L_j);
    }

  /*
      2L+H +------------+ L+H
           |            |
           |            |
           |            |
           |            |
   2L+2H+2 +------------+
           0            L
  */

    std::vector<std::vector<MVertex*> > &tab(dtgf->transfinite_vertices);
    tab.resize(L + 1);
    for(int i = 0; i <= L; i++) tab[i].resize(H + 1);
  
    tab[0][0] = m_vertices[0];
    tab[L][0] = m_vertices[L];
    tab[L][H] = m_vertices[L+H];
    tab[0][H] = m_vertices[2*L+H];
    for (int i = 1; i < L; i++){
      tab[i][0] = m_vertices[i];
      tab[i][H] = m_vertices[2*L+H-i];
    }
    for(int i = 1; i < H; i++){
      tab[L][i] = m_vertices[L+i];
      tab[0][i] = m_vertices[2*L+2*H-i];
    }

  double UC1 = U[N1], UC2 = U[N2], UC3 = U[N3];
  double VC1 = V[N1], VC2 = V[N2], VC3 = V[N3];

  //create points using transfinite interpolation
    double UC4 = U[N4];
    double VC4 = V[N4];
    for(int i = 1; i < L; i++){
      double u = lengths_i[i] / L_i;
      for(int j = 1; j < H; j++){
        double v = lengths_j[j] / L_j;
        int iP1 = N1 + i;
        int iP2 = N2 + j;
        int iP3 = N4 - i;
        int iP4 = (N4 + (N3 - N2) - j) % m_vertices.size();
        double Up = TRAN_QUA(U[iP1], U[iP2], U[iP3], U[iP4], UC1, UC2, UC3, UC4, u, v);
        double Vp = TRAN_QUA(V[iP1], V[iP2], V[iP3], V[iP4], VC1, VC2, VC3, VC4, u, v);
        GPoint gp = dtgf->point(Up, Vp);
        MFaceVertex *newv = new MFaceVertex(gp.x(), gp.y(), gp.z(), dtgf, Up, Vp);
        dtgf->mesh_vertices.push_back(newv);
        tab[i][j] = newv;
      }
    }

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

