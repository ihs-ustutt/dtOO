#include "dtMeshTransfinite2DOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>
#include "dtGmshFace.h"
#include <gmsh/meshGFace.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/Context.h>

#include <meshEngine/dtGmshModel.h>
#include <interfaceHeaven/threadSafe.h>
#include <analyticGeometryHeaven/map2dTo3d.h>

#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
  twoDArrayHandling< dtPoint2 > 
  dtMeshTransfinite2DOperator::computeEdgeLoops( dtGmshFace * gf ) {
    std::vector<MVertex*> corners;
    std::vector<MVertex*> all_mvertices;
    std::vector<int> indices;
    std::vector <MVertex *> m_vertices;
    
    findTransfiniteCorners(gf, corners);
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
      else {
        if(it == edges.end ()){
          dt__throw(
            computeEdgeLoops(), << "Something wrong in edge loop computation"
          );
        }
        v_start = ((*ito) == 1) ? (*it)->getBeginVertex() : (*it)->getEndVertex();
        if(v_start != v_end){
          dt__throw(
            computeEdgeLoops(), << "Something wrong in edge loop computation"
          );
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
    
    dt__throwIf(indices.size()!=2, operator());

    // create a list of all boundary vertices, starting at the first
    // transfinite corner

    unsigned int I;
    for(I = 0; I < all_mvertices.size(); I++)
      if(all_mvertices[I] == corners[0]) break;
    for(unsigned int j = 0; j < all_mvertices.size(); j++)
      m_vertices.push_back(all_mvertices[(I + j) % all_mvertices.size()]);

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

//    int iCorner = 0, N[4] = {0, 0, 0, 0};  
//    for(unsigned int i = 0; i < m_vertices.size(); i++){
//      MVertex *v = m_vertices[i];
//      if(
//        v == corners[0] || v == corners[1] 
//        || 
//        v == corners[2] || v == corners[3]
//      ) {
//        dt__throwIf(iCorner>3, operator());
//        N[iCorner++] = i;
//      }
//      SPoint2 param;
//      reparamMeshVertexOnFace(m_vertices[i], gf, param);
//      U[i] = param[0];
//      V[i] = param[1];    
//    }
    std::vector< dtPoint2 > reparamUV(m_vertices.size());
    #pragma omp parallel     
    {
      //
      // make threadSafe
      //
      threadSafe< dt__pH(map2dTo3d) > m2d;
      m2d().reset( gf->getMap2dTo3d()->clone() );
      
      #pragma omp for        
      dt__forAllIndex(m_vertices, ii) {
        reparamUV[ii] 
        = 
        m2d()->reparamOnFace( dtGmshModel::extractPosition(m_vertices[ii]) );        
      } 
    }
    
    int iCorner = 0, N[4] = {0, 0, 0, 0};  
    for(unsigned int i = 0; i < m_vertices.size(); i++) {
      MVertex *v = m_vertices[i];
      if(v == corners[0] || v == corners[1] || v == corners[2] ||
         (corners.size() == 4 && v == corners[3])) {
        dt__throwIf(iCorner>3, operator());
        N[iCorner++] = i;
      }
      U[i] = reparamUV[i].x();
      V[i] = reparamUV[i].y();
    }    

    int N1 = N[0], N2 = N[1], N3 = N[2], N4 = N[3];
    int L = N2 - N1, H = N3 - N2;

    int Lb = N4 - N3, Hb = m_vertices.size() - N4;
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

    std::vector<std::vector<MVertex*> > &tab(gf->transfinite_vertices);
    twoDArrayHandling< dtPoint2 > pUV(L+1, H+1);

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
    
    std::vector< dtPoint2 > bound0P = pUV.fixJ(0);
    std::vector< dtPoint2 > bound1P = pUV.fixI(L);
    std::vector< dtPoint2 > bound2P = pUV.fixJ(H);
    std::vector< dtPoint2 > bound3P = pUV.fixI(0);
    std::vector< dtReal > bound[4];
    dt__forAllRefAuto(bound0P, aPoint) bound[0].push_back(aPoint.x());
    dt__forAllRefAuto(bound1P, aPoint) bound[1].push_back(aPoint.y());
    dt__forAllRefAuto(bound2P, aPoint) bound[2].push_back(aPoint.x());
    dt__forAllRefAuto(bound3P, aPoint) bound[3].push_back(aPoint.y());
    
    int nCorrLoops 
    = 
    staticPropertiesHandler::getInstance()->getOptionInt(
      "transfinite_correctionLoops"
    );
    dt__forFromToIndex(0, 4, ii) {
      logContainer<dtMeshTransfinite2DOperator> logC(
        logDEBUG, "computeEdgeLoops()"
      );
      bool asc = std::is_sorted(bound[ii].begin(), bound[ii].end());
      bool desc = std::is_sorted(bound[ii].rbegin(), bound[ii].rend());      
      dt__forFromToIndex(0, nCorrLoops, cc) {
        asc = std::is_sorted(bound[ii].begin(), bound[ii].end());
        desc = std::is_sorted(bound[ii].rbegin(), bound[ii].rend());

        //
        // break if sorted correctly
        //      
        if (asc || desc) break;

        int boundSize = bound[ii].size();
        if ( bound[ii].front() < bound[ii].back() ) {
          logC() 
            << logMe::dtFormat( 
              "[ %3d ] ( %3d ) : Ascending Edge %10.2f < %10.2f"
            ) % ii % cc % bound[ii].front() % bound[ii].back() << std::endl;
          dt__forInnerIndex(bound[ii], jj) {
            if ( bound[ii][jj-1] > bound[ii][jj] ) {
              bound[ii][jj] = .5 * (bound[ii][jj+1] + bound[ii][jj-1]);
              logC() 
                << logMe::dtFormat( 
                  "[ %3d ]    <    : %3d -> ... %10.7f < %10.7f < %10.7f ..."
                ) % ii % jj % bound[ii][jj-1] % bound[ii][jj] % bound[ii][jj+1]
                << std::endl;
            }
          }
          // last
          if ( 
            bound[ii][boundSize-2] > bound[ii][boundSize-1] 
          ) {
            bound[ii][boundSize-2] 
            = 
            .5 * (
              bound[ii][boundSize-3] + bound[ii][boundSize-1]
            );
            logC() 
              << logMe::dtFormat( 
                "[ %3d ]    <    : end -> ... %10.7f < %10.7f < %10.7f ..."
              ) 
                % ii
                % bound[ii][boundSize-3] 
                % bound[ii][boundSize-2] 
                % bound[ii][boundSize-1] << std::endl;              
          }     
        }
        // descending
        else {
          logC() 
            << logMe::dtFormat( 
              "[ %3d ] ( %3d ) : Descending Edge %10.2f > %10.2f"
            ) % ii % cc % bound[ii].front() % bound[ii].back() << std::endl;            
          dt__forInnerIndex(bound[ii], jj) {
            if ( bound[ii][jj-1] < bound[ii][jj] ) {
              bound[ii][jj] = .5 * (bound[ii][jj+1] + bound[ii][jj-1]);
              logC() 
                << logMe::dtFormat( 
                  "[ %3d ]    >    : %3d -> ... %10.7f > %10.7f > %10.7f ..."
                ) % ii % jj % bound[ii][jj-1] % bound[ii][jj] % bound[ii][jj+1]
                << std::endl;             
            }
          }
          // last
          if ( 
            bound[ii][boundSize-2] < bound[ii][boundSize-1] 
          ) {
            bound[ii][boundSize-2] 
            = 
            .5 * (
              bound[ii][boundSize-3] + bound[ii][boundSize-1]
            );
            logC() 
              << logMe::dtFormat( 
                "[ %3d ]    >    : end -> ... %10.7f > %10.7f > %10.7f ..."
              ) 
                % ii
                % bound[ii][boundSize-3] 
                % bound[ii][boundSize-2] 
                % bound[ii][boundSize-1] << std::endl;                     
          }             
        }        

        dt__forAllIndex(bound[ii], jj) {
          if      (ii==0) pUV[jj][0] = dtPoint2(bound[ii][jj], pUV[jj][0].y());
          else if (ii==1) pUV[L][jj] = dtPoint2(pUV[L][jj].x(), bound[ii][jj]);
          else if (ii==2) pUV[jj][H] = dtPoint2(bound[ii][jj], pUV[jj][H].y());
          else if (ii==3) pUV[0][jj] = dtPoint2(pUV[0][jj].x(), bound[ii][jj]);
        }
      }

      //
      // throw error if sorting fails
      //
      dt__throwIfWithMessage(
        !asc && !desc,
        computeEdgeLoops(), 
        << "Cannot correct edge loop " << ii << "."
      );

      logC() << logMe::dtFormat("[ %3d ]   Ok") % ii << std::endl;
    }
    
    return pUV;
  }
  
  void dtMeshTransfinite2DOperator::createTransfiniteElements(
    dtGmshFace * const dtgf
  ) {
    std::vector< std::vector< ::MVertex * > > & tab 
    = 
    dtgf->transfinite_vertices;
    int L = tab.size()-1;
    int H = tab[0].size()-1;
    
    // create elements
    int maxLin = L * H;
    if (
      CTX::instance()->mesh.recombineAll 
      || 
      dtgf->meshAttributes.recombine      
    ) {
      dt__throwIf(dtgf->quadrangles.size(), operator()());
      dtgf->quadrangles.resize(maxLin);      
    }
    else {
      dt__throwIf(dtgf->triangles.size(), operator()());
      dtgf->triangles.resize(2*maxLin);
    }
    #pragma omp parallel
    {
      #pragma omp for
      for(int i = 0; i < L ; i++) {
        for(int j = 0; j < H; j++) {
          int lin = i * H + j;
          MVertex *v1 = tab[i][j];
          MVertex *v2 = tab[i + 1][j];
          MVertex *v3 = tab[i + 1][j + 1];
          MVertex *v4 = tab[i][j + 1];
          if (
            CTX::instance()->mesh.recombineAll 
            || 
            dtgf->meshAttributes.recombine
          ) {
//            dtgf->quadrangles.push_back( new MQuadrangle(v1, v2, v3, v4) );
            dtgf->quadrangles[lin] = new MQuadrangle(v1, v2, v3, v4);
          }
          else if (
            dtgf->meshAttributes.transfiniteArrangement == 1 
            ||
            (
              dtgf->meshAttributes.transfiniteArrangement == 2 
              &&
              (
                (i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)
              )
            ) 
            ||
            (
              dtgf->meshAttributes.transfiniteArrangement == -2 
              &&
              (
                (i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)
              )
            )
          ) {
            dtgf->triangles[lin] = new MTriangle(v1, v2, v3);
            dtgf->triangles[maxLin + lin] = new MTriangle(v3, v4, v1);
          }
          else {
            dtgf->triangles[lin] = new MTriangle(v1, v2, v4);
            dtgf->triangles[maxLin + lin] = new MTriangle(v4, v2, v3);            
          }
        }
      }
    }
  }

  dtMeshTransfinite2DOperator::dtMeshTransfinite2DOperator() {
    
  }

  dtMeshTransfinite2DOperator::dtMeshTransfinite2DOperator(
    const dtMeshTransfinite2DOperator& orig
  ) {
    
  }

  dtMeshTransfinite2DOperator::~dtMeshTransfinite2DOperator() {
  }
}
