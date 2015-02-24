#ifndef MESHKIT_dtPostBL_HPP
#define MESHKIT_dtPostBL_HPP

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
//#include <moab/VerdictWrapper.hpp>

/*!
 * \class dtPostBL
 * \brief Options and Keywords Used in dtPostBL Algorithm
 *
 *  RUNNING: Postmesh Boundary Layer Tool can be run using the test_postbl executable in test/algs directory
 example:-  test_postbl <name>.inp, where, <name> is the name of the input file containing the keywords below:
 - bias    \verbatim      <double>    bias b/w different layers of boundary layer is always greater than zero. \endverbatim
 - meshfile \verbatim     <string>    input mesh file name support by moab reader. \endverbatim
 - surfaces  \verbatim    <integer>   id of the surface on which boundary layer needs to be created.  \endverbatim
 - neumannset  \verbatim  <integer>   id of the neumann set on which boundary layer needs to be created.  \endverbatim
 - material    \verbatim  <integer>   material id to which the newly created hexes will be assigned, default is 999999. \endverbatim
 - thickness  \verbatim   <double>    boundary layer thickness. \endverbatim
 - debug     \verbatim    <1 or 0>    print all the debug o/p if set to 1. A .log file is always created.  \endverbatim
 - outfile   \verbatim    <string>    name of output mesh file, can be any format that is supported by moab.  \endverbatim
 - end        \verbatim               this marks the end of input file for boundary layer generation.  \endverbatim
 - Sample keyword file can be found here: data/test_postbl.inp
*/

namespace MeshKit {

#define DEFAULT_TEST_POSTBL  "test_postbl.inp"

  class MKCore;

  class dtPostBL : public MeshScheme
  {
  public:
    /* \brief Constructor
     *
     * Create a new dtPostBL instance
     * \param impl the iGeom instance handle for the Geom
     */
    dtPostBL(MKCore *mk, const MEntVector &me_vec);

    /* \brief Destructor
     */
    virtual ~dtPostBL();

    /**\brief Get class name */
    static const char* name();

    /**\brief Function returning whether this scheme can mesh entities of t
     *        the specified dimension.
     *\param dim entity dimension
     */
    static bool can_mesh(iBase_EntityType dim);

    /** \brief Function returning whether this scheme can mesh the specified entity
     *
     * Used by MeshOpFactory to find scheme for an entity.
     * \param me ModelEnt being queried
     * \return If true, this scheme can mesh the specified ModelEnt
     */
    static bool can_mesh(ModelEnt *me);

    /**\brief Get list of mesh entity types that can be generated.
     *\return array terminated with \c moab::MBMAXTYPE
     */
    static const moab::EntityType* output_types();

    /** \brief Return the mesh entity types operated on by this scheme
     * \return array terminated with \c moab::MBMAXTYPE
     */
    virtual const moab::EntityType* mesh_types_arr() const;

    /** \brief Re-implemented here so we can check topological dimension of model_ent
     * \param model_ent ModelEnt being added
     */
    virtual bool add_modelent(ModelEnt *model_ent);

    //! Setup is a no-op, but must be provided since it's pure virtual
    virtual void setup_this();

    //! execute dtPostBL graph node
    virtual void execute_this();

    void init( double const & thickness, int const & intervals, double const & bias );
    void debug( bool const & debug );
    void addPosRange( moab::Range const & toAdd );
    void addNegRange( moab::Range const & toAdd );    
  private:
    /** \brief get the normals given connectivity of a quad
     * \param conn connectivity array type EntityHandle
     *	\param v return normal vector
     */
    void get_normal_quad (std::vector<moab::EntityHandle>conn, moab::CartVect &v);
  private:
    //! iGeom Impl for calling geometry creation/manipulation operations
    iGeom *igeom;

    //! iMesh Impl for calling mesh creation/manipulation operations
    iMesh *imesh;

    //! MOAB Impl for calling mesh creation/manipulation operations
    moab::Interface *mb;

    //
    // boundary layer specification
    // pos: positive normal
    // neg: negative normal
    //
    moab::Range m_QuadsPos;
    moab::Range m_QuadsNeg;
    //
    // boundary layer specification
    //    
    double m_Thickness;
    int m_Intervals;    
    double m_Bias;
    bool m_Debug;

    mstream m_LogFile;
  };

  inline const char* dtPostBL::name()
  {
    return "dtPostBL";
  }

  inline bool dtPostBL::can_mesh(iBase_EntityType)
  {
    return false;
  }

  inline bool dtPostBL::can_mesh(ModelEnt *)
  {
    return true;
  }

  inline const moab::EntityType* dtPostBL::mesh_types_arr() const
  {
    return output_types();
  }

} // namespace MeshKit
#endif
