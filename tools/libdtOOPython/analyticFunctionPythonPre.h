class analyticFunctionWrap 
  : 
  public dtOO::analyticFunction, 
  public boost::python::wrapper< dtOO::analyticFunction > {
  dtOO::analyticFunction * clone( void ) const {
    return this->get_override("clone")();
  }
  dtOO::analyticFunction * cloneTransformed( dtOO::dtTransformer const * const dtT  ) const {
    if ( boost::python::override f = this->get_override("cloneTransformed") ) {
      return f(dtT); // *note*
    }
    return dtOO::analyticFunction::cloneTransformed(dtT);
  }
  dtOO::analyticFunction * create( void ) const {
    return this->get_override("create")();
  }  
  dtOO::aFY Y( dtOO::aFX const & xx ) const {
    return this->get_override("Y")(xx);
  }  
  dtOO::dtInt xDim( void ) const {
    return this->get_override("xDim")();
  }
  dtOO::dtInt yDim( void ) const {
    return this->get_override("yDim")();
  }  
  dtOO::dtReal xMin( dtOO::dtInt const & dir ) const {
    return this->get_override("xMin")(dir);
  }    
  dtOO::dtReal xMax( dtOO::dtInt const & dir ) const {
    return this->get_override("xMax")(dir);
  }      
};
