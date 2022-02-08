class constValueWrap 
  : 
  public dtOO::constValue, 
  public boost::python::wrapper< dtOO::constValue > {
  dtOO::constValue * clone( void ) const {
    return this->get_override("clone")();
  }
  void dump( void ) const {
    this->get_override("dump")();
  }  
  void writeToElement( 
    ::QDomDocument & doc, ::QDomElement & element
  ) {
    this->get_override("writeToElement")(doc, element);
  }
  void setValue( dtOO::dtReal const toSet ) {
    this->get_override("setValue")(toSet);
  }    
};
