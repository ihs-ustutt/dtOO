class map1dTo3dWrap 
  : 
  public dtOO::map1dTo3d, 
  public boost::python::wrapper< dtOO::map1dTo3d > {
    public:
      dtOO::map1dTo3d * clone( void ) const {
        return this->get_override("clone")();
      }
      dtOO::map1dTo3d * create( void ) const {
        return this->get_override("create")();
      }  
      dtOO::map1dTo3d * cloneTransformed( dtOO::dtTransformer const * const dtT  ) const {
        return this->get_override("cloneTransformed")(dtT);
      }  
      dtOO::dtPoint3 getPoint( dtOO::dtReal const & uu ) const {
        if ( boost::python::override f = this->get_override("getPoint") ) {
          return f(uu); // *note*
        }
        return dtOO::map1dTo3d::getPoint(uu);
      }    
};