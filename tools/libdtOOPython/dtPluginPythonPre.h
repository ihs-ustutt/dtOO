class dtPluginWrap 
  : 
  public dtOO::dtPlugin, 
  public boost::python::wrapper< dtOO::dtPlugin > {
  dtOO::dtPlugin * create( void ) const {
    return this->get_override("create")();
  }
  void apply( void ) {
    this->get_override("apply")();
  }  
};