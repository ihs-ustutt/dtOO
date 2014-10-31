#ifndef CHARACTERIZABLE_H
#define	CHARACTERIZABLE_H

#include <logMe/dtMacros.h>
#include <map>
#include <vector>
#include <string>

namespace dtOO {
  class labelHandling;
  
  typedef std::map< std::string, float > resValueMap;
  class characterizable {
  public:
    dt__CLASSNAME(characterizable);    
    characterizable();
    characterizable(const characterizable& orig);
    virtual ~characterizable();
    virtual float characterizeMe( void ) const = 0;
    virtual float characterizeMeFailed( void ) const;    
    void characterize(labelHandling const * const lH) const;
    resValueMap getResValuesFromFile( void ) const;
    resValueMap getFailedValuesFromFile( void ) const;
    void sendCanRead( void ) const;
    bool canRead( void ) const;
    void removeFiles( void ) const;
  private:
    static std::string const _canReadFileName;
    static std::string const _resValueFileName;
    static std::string const _failedValueFileName;
    
  };
}
#endif	/* CHARACTERIZABLE_H */

