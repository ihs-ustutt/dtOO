class boundedVolumeWrap 
  : 
  public dtOO::boundedVolume, 
  public boost::python::wrapper< dtOO::boundedVolume > {
  dtOO::boundedVolume * create( void ) const {
    return this->get_override("create")();
  }  
  void makeGrid( void ) {
    this->get_override("makeGrid")();
  }
  void makePreGrid( void ) {
    this->get_override("makePreGrid")();
  }
  std::vector< std::string > getMeshTags( void ) const {
    return this->get_override("getMeshTags")();
  }
  dtOO::dtGmshFace * getFace( std::string const & tag ) const {
    return this->get_override("getFace")(tag);
  }  
  dtOO::dtGmshRegion * getRegion( std::string const & tag ) const {
    return this->get_override("getRegion")(tag);
  }  
  dtOO::dtGmshModel * getModel( void ) const {
    return this->get_override("getModel")();
  }    
};