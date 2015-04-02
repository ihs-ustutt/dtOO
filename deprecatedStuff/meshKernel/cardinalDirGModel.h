#ifndef CARDINALDIRGMODEL_H
#define	CARDINALDIRGMODEL_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <dtGmshModel.h>

namespace dtOO {
  class cardinalDirTransfiniteVolume;
  class cardinalDirVolumeDefines;
  class analyticGeometry;
  
  class cardinalDirGModel : public dtGmshModel {
  public:
    dt__classOnlyName(cardinalDirGModel);  
    cardinalDirGModel(std::string name="");
    virtual ~cardinalDirGModel();
    void attachToBoundedVolume( boundedVolume const * const bV);
    boundedVolume const * getAttachedBoundedVolume( void );
    void convertToGmshModel( cardinalDirVolumeDefines const * cDDef = NULL);
    void convertOuterToGmshModel( cardinalDirVolumeDefines const * cDDef = NULL);
    void convertInternalToGmshModel( float const divideInt = 0.5, cardinalDirVolumeDefines const * cDDef = NULL);
    void addEdgeToGmshModel( 
      analyticGeometry const * const edge, 
      int const tag, 
      int const from, 
      int const to
    );    
  private:
    cardinalDirTransfiniteVolume const * _cardDir;
  };
}

#endif	/* CARDINALDIRGMODEL_H */

