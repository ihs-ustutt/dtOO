class dtCaseWrap 
  : 
  public dtOO::dtCase, 
  public boost::python::wrapper< dtOO::dtCase > {

  dtOO::dtCase * create( void ) const {
    return this->get_override("create")();
  }
  void runCurrentState( void ) {
    this->get_override("runCurrentState")();
  }  
};