class map2dTo3dWrap 
  : 
  public dtOO::map2dTo3d, 
  public boost::python::wrapper< dtOO::map2dTo3d > {
    public:
      dtOO::map2dTo3d * clone( void ) const {
        return this->get_override("clone")();
      }
      dtOO::map2dTo3d * create( void ) const {
        return this->get_override("create")();
      }  
      dtOO::map2dTo3d * cloneTransformed( dtOO::dtTransformer const * const dtT  ) const {
        return this->get_override("cloneTransformed")(dtT);
      }  
      dtOO::dtPoint3 getPoint( dtOO::dtReal const & uu, dtOO::dtReal const & vv ) const {
        if ( boost::python::override f = this->get_override("getPoint") ) {
          return f(uu, vv); // *note*
        }
        return dtOO::map2dTo3d::getPoint(uu, vv);
      }    
};