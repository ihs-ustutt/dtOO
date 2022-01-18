class analyticGeometryWrap 
  : 
  public dtOO::analyticGeometry, 
  public boost::python::wrapper< dtOO::analyticGeometry > {
    public:
  dtOO::analyticGeometry * clone( void ) const {
    return this->get_override("clone")();
  }
  dtOO::analyticGeometry * create( void ) const {
    return this->get_override("create")();
  }  
  dtOO::dtInt dim( void ) const {
    return this->get_override("dim")();
  }
  bool isClosed( dtOO::dtInt const & dir ) const {
    return this->get_override("isClosed")(dir);
  }
  dtOO::dtReal getMin( dtOO::dtInt const & dir ) const {
    return this->get_override("getMin")(dir);
  }    
  dtOO::dtReal getMax( dtOO::dtInt const & dir ) const {
    return this->get_override("getMax")(dir);
  }      
  dtOO::dtPoint3 getPoint( dtOO::dtReal const * const uvw ) const {
    if ( boost::python::override f = this->get_override("getPoint") ) {
      return f(uvw); // *note*
    }
    return dtOO::analyticGeometry::getPoint(uvw);
  }  
};
