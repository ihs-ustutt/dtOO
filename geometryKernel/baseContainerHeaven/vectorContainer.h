#ifndef VECTORCONTAINER_H
#define	VECTORCONTAINER_H

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class vectorContainer {
  public:    
    dt__CLASSNAME(vectorContainer);
    vectorContainer();
    vectorContainer(const vectorContainer& orig);
    virtual ~vectorContainer();
    void add(dtVector3 const * const vectorP, std::string const * const stringP );
    void add(dtVector3 const * const vectorP, std::string const  string );
    void add(dtVector3 const vector, std::string const * const stringP );
    void add(dtVector3 const vector, std::string const string );
    void add(dtVector3 const * const vector, std::string const string, dtPoint3 const point );//<--
    void add(dtVector3 const * const vector, std::string const * const string, dtPoint3 const point );
    void add(dtVector3 const * const vector, std::string const * const string, dtPoint3 const * const point );
    void add(dtVector3 const vector, std::string const string, dtPoint3 const point );
    void add(dtVector3 const vector, std::string const * const string, dtPoint3 const point );
    void add(dtVector3 const vector, std::string const * const string, dtPoint3 const * const point );
    dtVector3 get(std::string const string ) const;
    dtVector3 get(std::string const * const stringP) const;
    bool has(std::string const string) const;
    bool has(std::string const * const stringP) const;  
    bool isEmpty( void ) const;
  private:
    std::vector< std::pair <std::pair< dtVector3, dtPoint3 >, std::string > > _pair;
  };
}
#endif	/* VECTORCONTAINER_H */