#include "dtPostBL.h"
#include "progHelper.h"
#include <interfaceHeaven/twoDArrayHandling.h>

#include <cassert>
#include <string>
#include <vector>
#include <set>
#include <math.h>
#include <iomanip>

#include <meshkit/Types.hpp>
#include <meshkit/Error.hpp>
#include <meshkit/MeshScheme.hpp>
#include <meshkit/ModelEnt.hpp>
#include <meshkit/MKCore.hpp>
#include <meshkit/SizingFunction.hpp>
#include <meshkit/RegisterMeshOp.hpp>

#include <meshkit/LocalSet.hpp>
#include <meshkit/LocalTag.hpp>
#include <meshkit/Matrix.hpp>

#include <meshkit/iMesh.hpp>
#include <meshkit/iGeom.hpp>
#include <MBCN.h>

#include <meshkit/SimpleArray.hpp>
#include <meshkit/parser.hpp>
#include <meshkit/clock.hpp>
#include <meshkit/mstream.hpp>

#include <MBiMesh.hpp>
#include <moab/Interface.hpp>
#include <moab/Range.hpp>
#include <moab/CartVect.hpp>


#include <moab/Skinner.hpp>
#include <moab/AdaptiveKDTree.hpp>
#include <moab/Matrix3.hpp>
#include <moab/VerdictWrapper.hpp>

namespace MeshKit {
	using dtOO::operator<<;
	
  RegisterMeshOp<MeshKit::dtPostBL> dtPostBL_GLOBAL_PROXY;
			
// static registration of this mesh scheme
  moab::EntityType dtPostBL_tps[] = {moab::MBHEX, moab::MBMAXTYPE};

	const moab::EntityType* dtPostBL::output_types(){ 
		return dtPostBL_tps; 
	}

	dtPostBL::dtPostBL( MKCore *mk, const MEntVector &me_vec)
			: MeshScheme( mk, me_vec),
				igeom(mk->igeom_instance()), imesh(mk->imesh_instance()),
				mb (mk->moab_instance())
			// ---------------------------------------------------------------------------
			//! Function: Constructor \n
			//! Input:    Initialize mesh and geometry instances and parameters \n
			//! Output:   none
			// ---------------------------------------------------------------------------
	{
			m_QuadsPos.clear();
			m_QuadsNeg.clear();
      m_Thickness = 0.0;
      m_Intervals = 0;    
      m_Bias = 0.0;
      m_Debug = false;
	}

	dtPostBL::~dtPostBL()
	// ---------------------------------------------------------------------------
	//! Function: Destructor, does nothing..\n
	//! Input:    none \n
	//! Output:   none \n
	// ---------------------------------------------------------------------------
	{}

	bool dtPostBL::add_modelent(ModelEnt *model_ent)
	// ---------------------------------------------------------------------------
	//! Function: Adds entities for PosBL graph node.\n
	//! Input:    ModelEnt \n
	//! Output:   none \n
	// ---------------------------------------------------------------------------
	{
			return MeshOp::add_modelent(model_ent);
	}

	void dtPostBL::setup_this()
	// ---------------------------------------------------------------------------
	//! Function: Setup the graph node for dtPostBL \n
	//! Input:    none \n
	//! Output:   none \n
	// ---------------------------------------------------------------------------
	{
		if (m_Debug) std::cout <<  "\nIn setup this : " <<  std::endl;
	}

	void dtPostBL::execute_this()
	// ---------------------------------------------------------------------------
	//! Function: Read user input from file and run the dtPostBL algorithm \n
	//! Input:     Uses the file name (.inp) with keywords predefined by PosBL algorithm. \n
	//! Output:    Resulting mesh file is saved. \n
	// ---------------------------------------------------------------------------
	{
			//
			// set dimensions
			//
			int const GD = 3;
			int const BLD = 2;

			// obtain existing tag handles
			moab::Tag GDTag;
			moab::Tag BLNodeIDTag;
			MBERRCHK(mb->tag_get_handle("GEOM_DIMENSION", 1, moab::MB_TYPE_INTEGER, GDTag),mb);
			MBERRCHK(mb->tag_get_handle("BLNODEID", 1, moab::MB_TYPE_INTEGER, BLNodeIDTag,
																	moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT),mb);
			
			// For specified surface: get the  all the quads and nodes in a range
			moab::Range quads;
			moab::Range quadsFix;
			moab::Range nodes;
			moab::Range nodesFix;

			//
			// create union of m_QuadsPos and m_QuadsNeg
			//
			quads.merge(m_QuadsPos);
			quads.merge(m_QuadsNeg);

			//size of the following is based on element type
			std::vector<moab::EntityHandle> conn;
			std::vector<moab::EntityHandle> qconn;
			std::vector<moab::EntityHandle> adj_qconn;

      int hConnSize = 0;			
			int connSize = 0;
			int bElemNodesSize = 0;			
			// allocate space for connectivity/adjacency during the first pass of this loop
			if(mb->type_from_handle(quads.front()) == MBQUAD){
				connSize = 8;
				bElemNodesSize = 4;
				hConnSize = 8;
				//allocating based on element type
				conn.resize(m_Intervals*connSize);
				qconn.resize(bElemNodesSize);
				adj_qconn.resize(bElemNodesSize);
			}
			else if(mb->type_from_handle(quads.front()) == MBTRI){
				connSize = 4;
				bElemNodesSize = 3;
				//allocating based on element type - thrice the number of elements
				hConnSize = 6;
				conn.resize(m_Intervals*6);
				qconn.resize(bElemNodesSize);
				adj_qconn.resize(bElemNodesSize);
			}
			else if(connSize == 0 || bElemNodesSize == 0){
				throw MeshKit::Error(
					MeshKit::MK_FAILURE, 
					"This mesh type is not supported by this tool"
				);
			}
			
			MBERRCHK(mb->get_adjacencies(quads, 0, false, nodes, moab::Interface::UNION),mb);
			MBERRCHK(mb->get_adjacencies(m_QuadsFix, 0, false, nodesFix, moab::Interface::UNION),mb);
			nodes = moab::subtract(nodes, nodesFix);
			
			// remove critical elements
			for (Range::iterator quadIt = quads.begin(); quadIt != quads.end(); ++quadIt) {
				MBERRCHK(mb->get_connectivity(&(*quadIt), 1, qconn),mb);
				int counter = 0;
				for (int i=0; i<bElemNodesSize; i++){
					if (nodesFix.find(qconn[i]) != nodesFix.end()) {
						counter++;
					}
				}
				if ( counter == bElemNodesSize ) {
				  quadIt = dt__PRIOR(quads.erase(quadIt));
				}
				else if( counter > 0 ) {
				  quadsFix.insert(*quadIt);					
          quadIt = dt__PRIOR(quads.erase(quadIt));										
				}
			}

			if (m_Debug) {
				std::cout << "#m_QuadsNeg in this surface: " << m_QuadsNeg.size() << std::endl;
				std::cout << "#m_QuadsPos in this surface: " << m_QuadsPos.size() << std::endl;
				std::cout << "#m_QuadsFix in this surface: " << m_QuadsFix.size() << std::endl;
				std::cout << "#quads in this surface: " << quads.size() << std::endl;
				std::cout << "#QuadsFix in this surface: " << quadsFix.size() << std::endl;
				std::cout << "#nodes in this surface: " << nodes.size() << std::endl;
				std::cout << "#nodesFix in this surface: " << nodesFix.size() << std::endl;
				std::cout << "#New nodes to be created:" << m_Intervals*nodes.size() << std::endl;
			}

			if (quads.size() == 0 || nodes.size() == 0) {
				throw MeshKit::Error(
					MeshKit::MK_FAILURE, 
					"Invalid boundary layer specification, aborting.."
				);
			}

			// placeholder for storing gd on new entities
			moab::EntityHandle geom_set;
			MBERRCHK(mb->create_meshset(moab::MESHSET_SET, geom_set, 1), mb);
			MBERRCHK(mb->tag_set_data(GDTag, &geom_set, 1, &GD), mb);




			// Tag all nodes on outer boundary with a unique number
			int node_id = 0;
			for(moab::Range::iterator nodes_iter = nodes.begin(); nodes_iter != nodes.end(); nodes_iter++) {
					MBERRCHK(mb->tag_set_data(BLNodeIDTag, &(*nodes_iter),1, &node_id), mb);
					++node_id;
			}
			// Tag all nodes on outer boundary with a unique number
			for(moab::Range::iterator nodes_iter = nodesFix.begin(); nodes_iter != nodesFix.end(); nodes_iter++) {
					MBERRCHK(mb->tag_set_data(BLNodeIDTag, &(*nodes_iter),1, &node_id), mb);
					++node_id;
			}			

			// COMPUTE NORMALS
			// declare variables before starting BL creation
      std::vector<moab::EntityHandle> new_vert(m_Intervals*nodes.size());			
			int count = -1;
			for (Range::iterator kter = nodes.begin(); kter != nodes.end(); ++kter){
				++count;
				// only one normal in case of single material
				double xdisp = 0.0, ydisp = 0.0, zdisp = 0.0;

				moab::Range adj_for_normal;
				MBERRCHK(mb->get_adjacencies(&(*kter), 1, BLD, false, adj_for_normal, Interface::UNION), mb);
				//create the coordinates of the innermost node corresponding to this node

				moab::Range adj_for_norm = intersect(quads, adj_for_normal);							
				moab::Range adj_for_normPos = intersect(m_QuadsPos, adj_for_normal);
				moab::Range adj_for_normNeg = intersect(m_QuadsNeg, adj_for_normal);

				// now compute the average normal direction for this vertex
				moab::CartVect rt(0.0, 0.0, 0.0), v(0.0, 0.0, 0.0);

				for(Range::iterator qter = adj_for_norm.begin(); qter != adj_for_norm.end(); ++qter){
					MBERRCHK(mb->get_connectivity(&(*qter), 1, adj_qconn),mb);

					get_normal_quad (adj_qconn, v);

					if (adj_for_normPos.find(*qter) == adj_for_normPos.end()) v = -v;
					rt = rt + v;
				}					
				if(rt.length() !=0){
					xdisp=rt[0]/rt.length();
					ydisp=rt[1]/rt.length();
					zdisp=rt[2]/rt.length();
				}
				else{
					xdisp=0.0;
					ydisp=0.0;
					zdisp=0.0;
				}

				// after the normal computation is done create new BL nodes
				double coords_bl_quad[3], move = 0.0;
				MBERRCHK(mb->get_coords(&(*kter), 1, coords_bl_quad),mb);

				double temp = 0;
				double num = m_Thickness*(m_Bias-1)*(pow(m_Bias, m_Intervals -1));
				double deno = pow(m_Bias, m_Intervals) - 1;
				if (deno !=0)
						temp = num/deno;
				else
						temp = m_Thickness/m_Intervals;

				// loop thru intervals to create BL nodes
				for(int j=0; j< m_Intervals; j++){
					move+= temp/pow(m_Bias,j);
					// now compute the coords of the new vertex
			    double coords_new_quad[3];								
					coords_new_quad[0] = coords_bl_quad[0]-move*xdisp;
					coords_new_quad[1] = coords_bl_quad[1]-move*ydisp;
					coords_new_quad[2] = coords_bl_quad[2]-move*zdisp;

					int nid = count*m_Intervals+j;
					MBERRCHK(mb->create_vertex(coords_new_quad, new_vert[nid]), mb);
				}
			}

			// swap nodal coordinates of input nodes with innermost BL nodes to push the bulk mesh
			count = -1;
			for (Range::iterator kter = nodes.begin(); kter != nodes.end(); ++kter){
				++count;
					double coords_new_quad[3];
					double coords_old_quad[3];

					int nid = (count+1)*m_Intervals - 1;
					MBERRCHK(mb->get_coords(&new_vert[nid], 1, coords_new_quad),mb);

					MBERRCHK(mb->get_coords(&(*kter), 1, coords_old_quad),mb);

					MBERRCHK(mb->set_coords(&(*kter), 1, coords_new_quad),mb);

					MBERRCHK(mb->set_coords(&new_vert[nid], 1, coords_old_quad),mb);
//							if (m_Debug) {
//									m_LogFile << std::setprecision (3) << std::scientific << " : NID:" << (nid)
//														<< coords_old_quad[0]
//														<< ", " << coords_old_quad[1] << ", " << coords_old_quad[2] << " OLD:- coords: NEW" << coords_new_quad[0]
//														<< ", " << coords_new_quad[1] << ", " << coords_new_quad[2]  << std::endl;
//							}
//				}
			}

			// Now start creating New elements
			for (Range::iterator kter = quads.begin(); kter != quads.end(); ++kter){
				MBERRCHK(mb->get_connectivity(&(*kter), 1, qconn),mb);
				double one_node_in_quad[3];
				for (int i=0; i<bElemNodesSize; i++){

					int node_tag_id = 0;
					MBERRCHK(mb->tag_get_data(BLNodeIDTag, &qconn[i], 1, &node_tag_id) ,mb);
					MBERRCHK(mb->get_coords(&qconn[i], 1, one_node_in_quad),mb);
//							m_LogFile << std::setprecision (3) << std::scientific << " new nodes:- coords: " << one_node_in_quad[0]
//												<< ", " << one_node_in_quad[1] << ", " << one_node_in_quad[2]  << std::endl;

					//populate the connectivity after creating nodes for this BL node
					for(int j=0; j< m_Intervals; j++){
					  int nid = node_tag_id*m_Intervals+j;						
						if(connSize == 8 && bElemNodesSize == 4) { // hex
							if (j==0){
								conn[connSize*j +i] = qconn[i];
								conn[connSize*j + i+bElemNodesSize] = new_vert[nid + m_Intervals - 2];
							}
							else if(j==(m_Intervals-1)){
								conn[connSize*j +i] = new_vert[nid - m_Intervals + 1];
								conn[connSize*j + i+bElemNodesSize] = new_vert[nid];
							}
							else {
								conn[connSize*j +i] = new_vert[nid + m_Intervals - 2*j -1];
								conn[connSize*j + i+bElemNodesSize] = new_vert[nid + m_Intervals - 2*j -2];
							}
						}
						else if(connSize == 4 && bElemNodesSize == 3) { // prism
							if (j==0){
								conn[hConnSize*j +i] = qconn[i];
								conn[hConnSize*j + i+bElemNodesSize] = new_vert[nid + m_Intervals - 2];
							}
							else if(j==(m_Intervals-1)){
								conn[hConnSize*j +i] = new_vert[nid - m_Intervals + 1];
								conn[hConnSize*j + i+bElemNodesSize] = new_vert[nid];
							}
							else {
								conn[hConnSize*j +i] = new_vert[nid + m_Intervals - 2*j -1];
								conn[hConnSize*j + i+bElemNodesSize] = new_vert[nid + m_Intervals - 2*j -2];
							}
						}
						else {
							throw MeshKit::Error(
								MeshKit::MK_FAILURE, 
								"Cannot create BL elements: element type not supported."
							);
						}
					}
				}

				// create boundary layer hexes
//				std::cout << "conn = " << conn << std::endl;
				for(int j=0; j< m_Intervals; j++) {
			    moab::EntityHandle hex;					
					if(connSize == 8){
						MBERRCHK(mb->create_element(MBHEX, &conn[j*connSize], connSize, hex),mb);
					}
					else if(connSize==4) {
						//
						// renumber element to openfoam convention
						//
						std::vector< moab::EntityHandle > tmpConn(6);
						tmpConn[0] = conn[j*6+0];
						tmpConn[1] = conn[j*6+1];
						tmpConn[2] = conn[j*6+2];
						tmpConn[3] = conn[j*6+3];
						tmpConn[4] = conn[j*6+4];
						tmpConn[5] = conn[j*6+5];
						tmpConn = renumberPrism(tmpConn);
            MBERRCHK(mb->create_element(MBPRISM, &(tmpConn[0]), 6, hex),mb);						
					}
					// add this hex to a block
					moab::Range adj_for_mat;

					MBERRCHK(
						mb->get_adjacencies(
							&(*kter), 1, GD, false, adj_for_mat, moab::Interface::INTERSECT
					  ), 
						mb
					);
					MBERRCHK(mb->add_adjacencies(hex, adj_for_mat, true), mb);

					// add geom dim tag
					MBERRCHK(mb->add_entities(geom_set, &hex, 1), mb);
				}
			}
			
			for (Range::iterator kter = quadsFix.begin(); kter != quadsFix.end(); ++kter) {
        dtOO::twoDArrayHandling< moab::EntityHandle > critConn(m_Intervals, 0);
				std::vector< int > fixedNodePosConn;
				
				MBERRCHK(mb->get_connectivity(&(*kter), 1, qconn),mb);
				double one_node_in_quad[3];
				
				
				for (int i=0; i<bElemNodesSize; i++) {
					int node_tag_id = 0;
					MBERRCHK(mb->tag_get_data(BLNodeIDTag, &qconn[i], 1, &node_tag_id) ,mb);
					MBERRCHK(mb->get_coords(&qconn[i], 1, one_node_in_quad),mb);

					bool fixedNode = false;
					if ( nodesFix.find(qconn[i]) != nodesFix.end() ) {
						fixedNode = true;
						fixedNodePosConn.push_back(i);
					}
					
					//populate the connectivity after creating nodes for this BL node
					for(int j=0; j< m_Intervals; j++) {
						if(connSize == 4 && bElemNodesSize == 3) { // prism
							int nid = node_tag_id*m_Intervals+j;
							if(j==0) {
								critConn[j].push_back( qconn[i] );
								if (!fixedNode) {
									critConn[j].push_back( new_vert[nid + m_Intervals - 2] );
								}
							}
							else if(j==(m_Intervals-1)){
								if (!fixedNode) {
									critConn[j].push_back( new_vert[nid - m_Intervals + 1] );									
									critConn[j].push_back( new_vert[nid] );
								}
								else {
								  critConn[j].push_back( qconn[i] );									
								}
							}
							else {
								if (!fixedNode) {
									critConn[j].push_back( new_vert[nid + m_Intervals - 2*j - 1] );
									critConn[j].push_back( new_vert[nid + m_Intervals - 2*j - 2] );
								}
								else {
								  critConn[j].push_back( qconn[i] );									
								}								
							}
						}
						else {
							throw MeshKit::Error(
								MeshKit::MK_FAILURE, 
								"Cannot create BL elements: element type not supported."
							);
						}
					}
				}

				// create boundary layer hexes
				for(int j=0; j< m_Intervals; j++){
//				  std::cout 
//					  << "#Adding fixed quad" << std::endl
//						<< "critConn[" << j << "] = " << critConn[j] << std::endl
//				    << "fixedNodePosConn = " << fixedNodePosConn << std::endl;

					int num_nodes_element = critConn[j].size();//m_Intervals;
					if( num_nodes_element == 4 ) {
						std::vector< moab::EntityHandle > tmpConn;
						tmpConn = renumberTetrahedra(critConn[j]);
						moab::EntityHandle tet;					
						MBERRCHK(
							mb->create_element(
								MBTET, &(tmpConn[0]), num_nodes_element, tet
							),
							mb
						);
						moab::Range adj_for_mat;
						MBERRCHK(
							mb->get_adjacencies(
								&(*kter), 1, GD, false, adj_for_mat, moab::Interface::INTERSECT
							), 
							mb
						);
						MBERRCHK(mb->add_adjacencies(tet, adj_for_mat, true), mb);
						MBERRCHK(mb->add_entities(geom_set, &tet, 1), mb);						
					}
					else if( num_nodes_element == 5 ) {
						std::vector< moab::EntityHandle > tmpConn(5);
						if (fixedNodePosConn[0] == 0) {
							tmpConn[0] = critConn[j][3];
							tmpConn[1] = critConn[j][4];
							tmpConn[2] = critConn[j][2];
							tmpConn[3] = critConn[j][1];
							tmpConn[4] = critConn[j][0];
						}
						else if (fixedNodePosConn[0] == 1) {
							tmpConn[0] = critConn[j][3];
							tmpConn[1] = critConn[j][4];
							tmpConn[2] = critConn[j][1];
							tmpConn[3] = critConn[j][0];
							tmpConn[4] = critConn[j][2];
						}
						else if (fixedNodePosConn[0] == 2) {
							tmpConn[0] = critConn[j][2];
							tmpConn[1] = critConn[j][3];
							tmpConn[2] = critConn[j][1];
							tmpConn[3] = critConn[j][0];
							tmpConn[4] = critConn[j][4];

						}
						else {
							throw MeshKit::Error(
								MeshKit::MK_FAILURE, 
								"Unexpected position %i of fixed node.", fixedNodePosConn[0] 
							);								
						}						
						tmpConn = renumberPyramid(tmpConn);
						moab::EntityHandle pyr;
						MBERRCHK(mb->create_element(MBPYRAMID, &(tmpConn[0]), 5, pyr), mb);				
						moab::Range adj_for_mat;
						MBERRCHK(
							mb->get_adjacencies(
								&(*kter), 1, GD, false, adj_for_mat, moab::Interface::INTERSECT
							), 
							mb
						);
						MBERRCHK(mb->add_adjacencies(pyr, adj_for_mat, true), mb);
						MBERRCHK(mb->add_entities(geom_set, &pyr, 1), mb);												
					}
			  }			
			}
	}

  void dtPostBL::init( double const & thickness, int const & intervals, double const & bias ) {
		m_Thickness = thickness;
		m_Intervals = intervals;
		m_Bias = bias;
	}
		
	void dtPostBL::debug( bool const & debug ) {
		m_Debug = debug;
	}
	
	void dtPostBL::addPosRange( moab::Range const & toAdd ) {
		m_QuadsPos = toAdd;
	}
	
	void dtPostBL::addNegRange( moab::Range const & toAdd ) {
		m_QuadsNeg = toAdd;
	}	

	void dtPostBL::addFixRange( moab::Range const & toAdd ) {
		m_QuadsFix = toAdd;
	}	
	
	void dtPostBL::get_normal_quad (std::vector<EntityHandle>conn, moab::CartVect &v)
	// ---------------------------------------------------------------------------
	//! Function: Get normal of a quad \n
	//! Input:    conn \n
	//! Output:   moab::CartVect v \n
	// ---------------------------------------------------------------------------
	{
			moab::CartVect coords[3];
			MBERRCHK(mb->get_coords(&conn[0], 3, (double*) &coords[0]), mb);
			moab::CartVect AB(coords[1] - coords[0]);
			moab::CartVect BC(coords[2] - coords[1]);
			moab::CartVect normal = AB*BC;
			normal = normal/normal.length();
			v = normal;
	}

  std::vector< moab::EntityHandle > dtPostBL::renumberTetrahedra( std::vector< moab::EntityHandle > const & orig ) {
		moab::CartVect coords[4];
    mb->get_coords(&(orig[0]), 4, (double*) &coords[0]);
		
		moab::CartVect normal;
		get_normal_quad(orig, normal);
		
		moab::CartVect v03(coords[3] - coords[0]);
		
		std::vector< moab::EntityHandle > newOrder(4);
		if ( (v03 % normal) < 0. ) {
			newOrder[0] = orig[2];
			newOrder[1] = orig[1];
			newOrder[2] = orig[0];
			newOrder[3] = orig[3];
		}
		else {
			newOrder[0] = orig[0];
			newOrder[1] = orig[1];
			newOrder[2] = orig[2];
			newOrder[3] = orig[3];			
		}
		
		return newOrder;
  }

  std::vector< moab::EntityHandle > dtPostBL::renumberPyramid( std::vector< moab::EntityHandle > const & orig ) {
		moab::CartVect coords[5];
    mb->get_coords(&(orig[0]), 5, (double*) &coords[0]);
		
		moab::CartVect normal;
		get_normal_quad(orig, normal);
		
		moab::CartVect v04(coords[4] - coords[0]);
		
		std::vector< moab::EntityHandle > newOrder(5);
		if ( (v04 % normal) < 0. ) {
			newOrder[0] = orig[3];
			newOrder[1] = orig[2];
			newOrder[2] = orig[1];
			newOrder[3] = orig[0];
			newOrder[4] = orig[4];
		}
		else {
			newOrder[0] = orig[0];
			newOrder[1] = orig[1];
			newOrder[2] = orig[2];
			newOrder[3] = orig[3];			
			newOrder[4] = orig[4];			
		}
		
		return newOrder;
  }	

  std::vector< moab::EntityHandle > dtPostBL::renumberPrism( std::vector< moab::EntityHandle > const & orig ) {
		moab::CartVect coords[6];
    mb->get_coords(&(orig[0]), 6, (double*) &coords[0]);
		
		std::vector< moab::EntityHandle > newOrder(6);
		moab::CartVect normalOne;
		moab::CartVect normalTwo;
		get_normal_quad(orig, normalOne);
		newOrder[0] = orig[3];
		newOrder[1] = orig[4];
		newOrder[2] = orig[5];
		get_normal_quad(newOrder, normalTwo);
		
		moab::CartVect v03(coords[3] - coords[0]);
		
		
		if ( (v03 % normalOne) > 0. ) {
			newOrder[0] = orig[2];
			newOrder[1] = orig[1];
			newOrder[2] = orig[0];
		}
		else {
			newOrder[0] = orig[0];
			newOrder[1] = orig[1];
			newOrder[2] = orig[2];
		}
		
		if ( (v03 % normalTwo) > 0. ) {
			newOrder[3] = orig[5];
			newOrder[4] = orig[4];
			newOrder[5] = orig[3];
		}
		else {
			newOrder[3] = orig[3];			
			newOrder[4] = orig[4];			
			newOrder[5] = orig[5];
	}		
		return newOrder;
  }		
} // namespace MeshKit

