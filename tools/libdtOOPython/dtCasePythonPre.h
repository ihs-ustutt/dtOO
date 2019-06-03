class dtCaseWrap 
  : 
  public dtOO::dtCase, 
  public boost::python::wrapper< dtOO::dtCase > {

  void runCurrentState( void ) {
    this->get_override("runCurrentState")();
  }  
};