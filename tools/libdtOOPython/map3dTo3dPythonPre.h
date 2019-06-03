class map3dTo3dWrap 
  : 
  public dtOO::map3dTo3d, 
  public boost::python::wrapper< dtOO::map3dTo3d > {
    public:
      dtOO::map3dTo3d * clone( void ) const {
        return this->get_override("clone")();
      }
      dtOO::map3dTo3d * create( void ) const {
        return this->get_override("create")();
      }  
      dtOO::map3dTo3d * cloneTransformed( dtOO::dtTransformer const * const dtT  ) const {
        return this->get_override("cloneTransformed")(dtT);
      }  
      dtOO::dtPoint3 getPoint( float const & uu, float const & vv, float const & ww ) const {
        if ( boost::python::override f = this->get_override("getPoint") ) {
          return f(uu, vv, ww); // *note*
        }
        return dtOO::map3dTo3d::getPoint(uu, vv, ww);
      }    
};