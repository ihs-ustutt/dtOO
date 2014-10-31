
#ifndef DTBMGRID_H
#define	DTBMGRID_H

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/coDoSetHandling.h>

namespace covise {
    class coDoUnstructuredGrid;
    class coDoStructuredGrid;
    class coDoSet;
}
namespace dtOO {

    class dtSurface;
    class dtCurve;
    class scaFunction;
    
class dtBmGrid {
public:
    DTCLASSNAMEMETHOD(dtBmGrid);
//    virtual std::string getClassName( void ) const { 
//      return std::string("dtBmGrid");
//    }
    DTCLASSLOGMETHOD(dtBmGrid);    
    dtBmGrid();
    dtBmGrid(const dtBmGrid& orig);
    dtBmGrid(std::vector<dtSurface *> const blockSurf, 
             int const nx, int const ny, int const nz);
    dtBmGrid(string const str, std::vector<dtSurface *> const blockSurf, 
             int const nx, int const ny, int const nz, coDoSetHandling< scaFunction *> const * const sFunP);
    dtBmGrid(std::vector<dtCurve *> const surfEdges, 
             int const nx, int const ny);
    virtual ~dtBmGrid();
    
    bool checkBlockNomenclature(std::vector<dtSurface *> const blockSurf);
    std::vector< std::pair<dtVector3 , int> > createPoints(std::vector<dtSurface *> const blockSurf, 
                                                          int const nx, int const ny, 
                                                          int const nz, coDoSetHandling< scaFunction *> const * const sFunP);
    std::vector< dtPoint3 > createPoints(std::vector<dtCurve *> const surfEdges,
                                                          int const nx, int const ny);
    covise::coDoStructuredGrid * coupleStructBlocks(string const str,
                                                              coDoStructuredGrid * const gridI,
                                                              coDoStructuredGrid * const gridII, int const cm) const;
    covise::coDoUnstructuredGrid * unsCoupleStructBlocks(string const str, 
                                                      coDoStructuredGrid * const gridI,
                                                      coDoStructuredGrid * const gridII, int const coupleIndex) const;
    covise::coDoUnstructuredGrid * coupleBlocks(std::vector< std::pair<dtVector3 , int> > const blockI,
                                                          int const surfIndexI,
                                                          std::vector< std::pair<dtVector3 , int> > blockII,
                                                          int const surfIndexII);
    covise::coDoUnstructuredGrid * toCoDoUns(string const str) const;
    covise::coDoStructuredGrid * toCoDoStruct(string const str) const;
    covise::coDoSet * toCoDoUns2D(string const str) const;
    
    
private:
    std::vector< std::pair<dtVector3 , int> > _gridPoints;
    std::vector<dtPoint3> _surfGridPoints;
    int _nx, _ny, _nz;
    
    
};

}

#endif	/* DTBMGRID_H */

