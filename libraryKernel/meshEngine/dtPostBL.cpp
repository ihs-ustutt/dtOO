#include "dtPostBL.h"

namespace MeshKit
{
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
		if (m_Debug) m_LogFile <<  "\nIn setup this : " <<  std::endl;
	}

	void dtPostBL::execute_this()
	// ---------------------------------------------------------------------------
	//! Function: Read user input from file and run the dtPostBL algorithm \n
	//! Input:     Uses the file name (.inp) with keywords predefined by PosBL algorithm. \n
	//! Output:    Resulting mesh file is saved. \n
	// ---------------------------------------------------------------------------
	{

			m_LogFile << "\nIn execute this : creating boundary layer elements.." <<  std::endl;
			// start the timer
			CClock Timer;
			clock_t sTime = clock();
			std::string szDateTime;
			Timer.GetDateTime (szDateTime);
//			VerdictWrapper vw(mb);

			m_LogFile <<  "\nStarting out at : " << szDateTime << std::endl;

			moab::Range all_elems, all_verts;
			MBERRCHK(mb->get_entities_by_dimension(0, 3, all_elems,true),mb);

			//
			// set dimensions
			//
			int const GD = 3;
			int const BLD = 2;
			
			all_elems.clear();
			m_LogFile << "Geometric dimension of meshfile = "<< GD <<std::endl;

			// obtain existing tag handles
			moab::Tag GDTag;
			moab::Tag MTag, STag, MNTag, MatIDTag, BLNodeIDTag;
			MBERRCHK(mb->tag_get_handle("GEOM_DIMENSION", 1, moab::MB_TYPE_INTEGER, GDTag),mb);
			MBERRCHK(mb->tag_get_handle("MATERIAL_SET", 1, moab::MB_TYPE_INTEGER, MTag),mb);

			MBERRCHK(mb->tag_get_handle("mnode", 1, moab::MB_TYPE_INTEGER, MNTag,
																	moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT),mb);
			MBERRCHK(mb->tag_get_handle("matid", 1, moab::MB_TYPE_INTEGER, MatIDTag,
																	moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT),mb);
			MBERRCHK(mb->tag_get_handle("BLNODEID", 1, moab::MB_TYPE_INTEGER, BLNodeIDTag,
																	moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT),mb);

			// get all the entity sets with boundary layer geom dimension, neumann sets and material sets
			moab::Range m_sets;


			MBERRCHK(mb->get_entities_by_type_and_tag(0, moab::MBENTITYSET, &MTag, 0, 1 , m_sets),mb);

			// For specified surface: get the  all the quads and nodes in a range
			moab::Range quads;
			moab::Range nodes;

			//
			// create union of m_QuadsPos and m_QuadsNeg
			//
			quads.merge(m_QuadsPos);
			quads.merge(m_QuadsNeg);
			MBERRCHK(mb->get_adjacencies(quads, 0, false, nodes, moab::Interface::UNION),mb);

			if (m_Debug) {
				m_LogFile << "#Quads in this surface: " << quads.size() << std::endl;
				m_LogFile << "#Nodes in this surface: " << nodes.size() << std::endl;
				m_LogFile << "#New nodes to be created:" << m_Intervals*nodes.size() << std::endl;
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

			// declare variables before starting BL creation
			double coords_new_quad[3];
			moab::EntityHandle hex;

			//size of the following is based on element type
			std::vector<moab::EntityHandle> conn;
			std::vector<moab::EntityHandle> qconn;
			std::vector<moab::EntityHandle> adj_qconn;
      std::vector<moab::EntityHandle> new_vert(m_Intervals*nodes.size());

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

			// Tag all nodes on outer boundary with a unique number
			int node_id = 0;
			std::vector<int> NId(nodes.size());
			for(moab::Range::iterator nodes_iter = nodes.begin(); nodes_iter != nodes.end(); nodes_iter++){
					NId[node_id] = node_id;
					MBERRCHK(mb->tag_set_data(BLNodeIDTag, &(*nodes_iter),1, &NId[node_id]), mb);
					++node_id;
			}

			// Handling MaterialSet
			int mset_id = 0;
			for (moab::Range::iterator mset_it = m_sets.begin(); mset_it != m_sets.end(); mset_it++)  {
				moab::EntityHandle mthis_set = *mset_it;

				// get entity handle of MS specified in the input file
				MBERRCHK(mb->tag_get_data(MTag, &mthis_set, 1, &mset_id), mb);

				// get all the nodes in the material and tag bl nodes
				moab::Range mat_nodes, mat_hexes;
				if(connSize == 8)
						MBERRCHK(mb->get_entities_by_type(mthis_set, moab::MBHEX, mat_hexes, true), mb);
				else if(connSize ==4)
						MBERRCHK(mb->get_entities_by_type(mthis_set, moab::MBTET, mat_hexes, true), mb);
				// tag all the mat_hexes with matid
				std::vector<int> matID(mat_hexes.size(), mset_id);
				MBERRCHK(mb->tag_set_data(MatIDTag, mat_hexes, &matID[0]), mb);
				//
				MBERRCHK(mb->get_adjacencies(mat_hexes, 0, false, mat_nodes, Interface::UNION), mb);
				moab::Range mat_b_nodes = intersect(nodes, mat_nodes);

				//
				std::vector<int> bl_node_data(mat_b_nodes.size(), 0);
				std::vector<int> node_tag_data(mat_b_nodes.size(),-1);
				// don't error check, as it is supposed to give error when multiple material case is encountered
				mb->tag_get_data(MNTag, mat_b_nodes, &node_tag_data[0]);
				for(int i=0; i< (int)mat_b_nodes.size(); i++){
						// already a part of some material
						if(node_tag_data[i] != -1){
								bl_node_data[i] = node_tag_data[i]+1;
						}
				}
				MBERRCHK(mb->tag_set_data(MNTag, mat_b_nodes, &bl_node_data[0]), mb);
				mat_hexes.clear();
				mat_b_nodes.clear();
				mat_nodes.clear();
			} // end handling material set

			// COMPUTE NORMALS
			// get tag data and print
			std::vector<int> all_bl(nodes.size(), 0);
			mb->tag_get_data(MNTag, nodes, &all_bl[0]);
			int count = -1;
			for (Range::iterator kter = nodes.begin(); kter != nodes.end(); ++kter){
				++count;
				// only one normal in case of single material
				double xdisp = 0.0, ydisp = 0.0, zdisp = 0.0;
				// check if node belongs to one or more materials
				if(all_bl[count] == 0){
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
				}
				else if(all_bl[count] < 0 ){
					throw MeshKit::Error(
						MeshKit::MK_FAILURE, 
						"Material must have associated with BLNode %i", count
					);
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
					coords_new_quad[0] = coords_bl_quad[0]-move*xdisp;
					coords_new_quad[1] = coords_bl_quad[1]-move*ydisp;
					coords_new_quad[2] = coords_bl_quad[2]-move*zdisp;

					int nid = count*m_Intervals+j;
					// Possible TODO's
					//TODO: Check if this vertex is possible (detect possible collision with geometry)
					// TODO: See posibility of using ray tracing
					// TODO: Parallize: Profile T-junction model and try to device an algorithm
					// TODO: Modularize node creation part and use doxygen for all code and design of code, python design and test cases - current functions in code:
					// Setup this, Execute this -- break info sub functions and classes,
					// prepareIO --make this optional when using python,
					// get normal (2d and 3d) -- can be combined to one function
					// get det jacobian (hex elements) --needs check for other elements
					//
					MBERRCHK(mb->create_vertex(coords_new_quad, new_vert[nid]), mb);
				}
			}

			// swap nodal coordinates of input nodes with innermost BL nodes to push the bulk mesh
			count = -1;
			for (Range::iterator kter = nodes.begin(); kter != nodes.end(); ++kter){
				++count;
				if(all_bl[count] == 0 ){
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
				}
			}

			// check for the volume of penultimate elements if -ve volume encountered. Report.
			count = -1;
			// Try to move another layer attached to it.
			for (Range::iterator kter = nodes.begin(); kter != nodes.end(); ++kter){
				++count;
				for(int j=0; j< m_Intervals; j++){
					int nid = count*m_Intervals+j;
					double coords_new_quad[3];
					MBERRCHK(mb->get_coords(&new_vert[nid], 1, coords_new_quad),mb);
//							m_LogFile << std::setprecision (3) << std::scientific << " : NID:" << (nid)
//												<< " of " << new_vert.size() << " new nodes:- coords: " << coords_new_quad[0]
//												<< ", " << coords_new_quad[1] << ", " << coords_new_quad[2]  << std::endl;
				}
			}

//			// Now start creating New elements
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
						if(connSize == 8 && bElemNodesSize == 4){ // hex
							int nid = node_tag_id*m_Intervals + j;
							if(m_Intervals == 1){
								conn[connSize*j +i] = qconn[i];
								conn[connSize*j + i+bElemNodesSize] = new_vert[nid];
							}
							else if(j==0){
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
						else if(connSize == 4 && bElemNodesSize == 3){ // && hybrid == true make wedges aka prisms for tet mesh
							int nid = node_tag_id*m_Intervals+j;
							if(m_Intervals == 1){
								conn[connSize*j +i] = qconn[i];
								conn[connSize*j + i+bElemNodesSize] = new_vert[nid];
							}
							else if(j==0){
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
				for(int j=0; j< m_Intervals; j++){
					if(connSize == 8){
						MBERRCHK(mb->create_element(MBHEX, &conn[j*connSize], connSize, hex),mb);
					}
					else if(connSize==4) {//&& hybrid == true){
						MBERRCHK(mb->create_element(MBPRISM, &conn[j*6], 6, hex),mb);
					}
					// add this hex to a block
					moab::Range adj_hex_for_mat;

					MBERRCHK(mb->get_adjacencies(&(*kter), 1, GD, false, adj_hex_for_mat, moab::Interface::INTERSECT), mb);
					MBERRCHK(mb->add_adjacencies(hex, adj_hex_for_mat, true), mb);

					std::vector<int> hmat_id(adj_hex_for_mat.size(), 0);

					// this will lead to an error, so no error checking, new adj hexes don't have matidtag
					mb->tag_get_data(MatIDTag, adj_hex_for_mat, &hmat_id[0]);//, mb);
					for(int p=0; p< (int)hmat_id.size(); p++){
						if(hmat_id[p] !=0){
							// this is our mat id for this hex
							moab::EntityHandle mat_set = 0;
							for (moab::Range::iterator set_it = m_sets.begin(); set_it != m_sets.end(); set_it++)  {
								mat_set = *set_it;
								int set_id;
								MBERRCHK(mb->tag_get_data(MTag, &mat_set, 1, &set_id), mb);
								if(set_id == hmat_id[p]) break;
							}
							MBERRCHK(mb->add_entities(mat_set, &hex, 1), mb);
						}
					}						// mark entities for smoothing
					// add geom dim tag
					MBERRCHK(mb->add_entities(geom_set, &hex, 1), mb);
				}

			}

			// get the current date and time
			Timer.GetDateTime (szDateTime);
			m_LogFile << "Ending at : " << szDateTime;
			// report/compute the elapsed time
			m_LogFile <<  "Elapsed wall clock time: " << Timer.DiffTime ()
								 << " seconds or " << (Timer.DiffTime ())/60.0 << " mins\n";
			m_LogFile <<  "AL2 Total CPU time used: " << (double) (clock() - sTime)/CLOCKS_PER_SEC \
								 << " seconds" << std::endl;
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
} // namespace MeshKit


