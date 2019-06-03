template<typename T>
class labeledVectorHandlingWrap 
  : 
  public dtOO::labeledVectorHandling< T >, 
  public boost::python::wrapper< dtOO::labeledVectorHandling< T > > {
};