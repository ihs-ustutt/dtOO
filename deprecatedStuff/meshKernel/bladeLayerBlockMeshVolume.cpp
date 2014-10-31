#include "bladeLayerBlockMeshVolume.h"
#include "bladeLayerGmshVolume.h"
#include <do/coDoSet.h>

#include <ReadFOAM.h>
#include <interfaceHeaven/systemHandling.h>

namespace dtOO {
  bladeLayerBlockMeshVolume::bladeLayerBlockMeshVolume() {
  }

  bladeLayerBlockMeshVolume::bladeLayerBlockMeshVolume(const bladeLayerBlockMeshVolume& orig) {
    _caseDirectory = orig._caseDirectory;
  }

  bladeLayerBlockMeshVolume::~bladeLayerBlockMeshVolume() {
  }

  void bladeLayerBlockMeshVolume::init(
    QDomElement const & element,
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< scaFunction * > const * const sFunP,
    vectorHandling< analyticGeometry * > const * const depAGeoP 
  ) {
    //
    // init cardinalDirVolume
    //
    bladeLayerVolume::init(element, cValP, sFunP, depAGeoP);

    //
    // case directory
    //
    _caseDirectory = getOption("workingDirectory")+"/"+getName();
  }
  
  void bladeLayerBlockMeshVolume::writeGrid( void ) {
    //
    // push to simpleBlockStructure
    //
    pushGridToSimpleBlockStructure();
       
    std:string cmd;
    systemHandling systemH;
    
    //
    //prepare case
    //
    DTINFOWF(writeGrid(),
            << "Preparing case: " << _caseDirectory );    
    cmd = "sh " + getOption("prepareCase_script") + " " + _caseDirectory;    
    systemH.command(cmd);

    //
    // write blockMeshDict
    //    
    writeDict( _caseDirectory );

    //
    // create grid with shell script
    //
    cmd = "sh " + _caseDirectory + "/run_blockMesh.sh";
    systemH.command(cmd);
  }
  
  void bladeLayerBlockMeshVolume::writeCheapGrid( void ) {
    //
    // push to blockMeshDictWriter
    //
    pushGridToSimpleBlockStructure();
  }  

  covise::coDoSet * bladeLayerBlockMeshVolume::toCoDoUnstructuredGrid(char const * const str) const {
    covise::coDistributedObject ** elem = new covise::coDistributedObject*[2];
    
        
    elem[0] = ReadFOAM(0, NULL).loadMesh(
                _caseDirectory+"/constant/polyMesh", 
                _caseDirectory+"/constant/polyMesh",
                ("coVolume::coDoUnstructuredGrid::"+getName()).c_str()
              );
    elem[1] = NULL;
    return new covise::coDoSet(str, elem);
  }

  void bladeLayerBlockMeshVolume::writeDict( std::string const casePath ) const {

    //
    // create foam file for post-processing
    //
    std::ofstream dictFile;
    std::string dictPath = casePath+"/foam.foam";
    dictFile.open( dictPath.c_str(), std::ofstream::out | std::ofstream::trunc );
    dictFile.close();
    
    //
    // create blockMeshDict file
    //
    dictPath = casePath+"/constant/polyMesh/blockMeshDict";
    dictFile.open( dictPath.c_str(), std::ofstream::out | std::ofstream::trunc );

    //
    // set precision
    //
    dictFile.precision(32);
    dictFile.fixed;
    
    //
    // write header
    //
    dictFile << "/*--------------------------------*- C++ -*----------------------------------*\\" << std::endl;
    dictFile << "| =========                 |                                                 |" << std::endl;
    dictFile << "| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |" << std::endl;
    dictFile << "|  \\    /   O peration     | Version:  2.1.x                                 |" << std::endl;
    dictFile << "|   \\  /    A nd           | Web:      www.OpenFOAM.org                      |" << std::endl;
    dictFile << "|    \\/     M anipulation  |                                                 |" << std::endl;
    dictFile << "\\*---------------------------------------------------------------------------*/" << std::endl;
    dictFile << "FoamFile" << std::endl;
    dictFile << "{" << std::endl;
    dictFile << "    version     2.0;" << std::endl;
    dictFile << "    format      ascii;" << std::endl;
    dictFile << "    class       dictionary;" << std::endl;
    dictFile << "    object      blockMeshDict;" << std::endl;
    dictFile << "}" << std::endl;
    dictFile << "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //" << std::endl;
    dictFile << "convertToMeters 1.;" << std::endl;
    dictFile << " " << std::endl;

    //loop over all blocks
    //
    // write vertices
    //
    dictFile << "vertices" << std::endl;
    dictFile << "(" << std::endl;  
    dt__FORALL(_vertex, ii,
//        dictFile << "(" << _vertex[ii].x() << " " 
//                        << _vertex[ii].y() << " " 
//                        << _vertex[ii].z() << ")" << std::endl;
        dictFile << "(" << getVertex(ii).x() << " " 
                        << getVertex(ii).y() << " " 
                        << getVertex(ii).z() << ")" << std::endl;
    );
    dictFile << ");" << std::endl;  

    //
    // write edges
    //
    dictFile << "edges" << std::endl;
    dictFile << "(" << std::endl;  
    dt__FORALL(_edge, ii,
        dictFile << "polyLine " << _edgeFrom[ii] << " " << _edgeTo[ii] << std::endl;
        dictFile << "(" << std::endl;
        std::vector< dtPoint3 > vertex = getPointsOnEdge(
                                          _edge[ii], 
                                          _edgeUStart[ii], 
                                          _edgeUEnd[ii], 
                                          _edgeNPointsOnSpline[ii],
                                          _edgeFrom[ii],
                                          _edgeTo[ii]
                                        );
        for (int kk=0;kk<vertex.size();kk++) {
          dictFile << "(" 
                   << vertex[kk].x() << " " 
                   << vertex[kk].y() << " " 
                   << vertex[kk].z() << ")" << std::endl;
        }
        vertex.clear();
        dictFile << ")" << std::endl;
    );
    dictFile << ");" << std::endl;  

    //
    // write blocks
    //
    dictFile << "blocks" << std::endl;
    dictFile << "(" << std::endl;
    dt__FORALL(_block, ii,
      //vertices
      dictFile << "hex ( ";
      for (int jj=0; jj<_block[ii].size();jj++) {
        dictFile << _block.at(ii)[jj] << " ";
      }
      dictFile << " ) ";
      dictFile
          << " ( " 
          << _blockNElem[ii][0] << " " 
          << _blockNElem[ii][1] << " "
          << _blockNElem[ii][2] 
          << " ) ";        
      if ( _blockGrading[ii].size() == 3 ) {
        dictFile 
          << "simpleGrading ( " 
          << _blockGrading[ii][0] << " " 
          << _blockGrading[ii][1] << " "
          << _blockGrading[ii][2] 
          << " ) ";        
      }
      else if ( _blockGrading[ii].size() == 12 ) {
        dictFile 
          << "edgeGrading ( " 
          << _blockGrading[ii][ 0] << " " 
          << _blockGrading[ii][ 1] << " "
          << _blockGrading[ii][ 2] << " "
          << _blockGrading[ii][ 3] << " "
          << _blockGrading[ii][ 4] << " "
          << _blockGrading[ii][ 5] << " "
          << _blockGrading[ii][ 6] << " "
          << _blockGrading[ii][ 7] << " "
          << _blockGrading[ii][ 8] << " "
          << _blockGrading[ii][ 9] << " "                      
          << _blockGrading[ii][10] << " " 
          << _blockGrading[ii][11]
          << " ) ";   
      }
      else {
        dt__THROW(writeDict(),
                << "_blockGrading[" << ii << "].size() = " << _blockGrading[ii].size() << LOGDEL
                << "Should be 3 or 12.");
      }
      dictFile << std::endl;
    );
    dictFile << ");" << std::endl;
    
    //
    // write blocks
    //
    dictFile << "patches" << std::endl;
    dictFile << "(" << std::endl;
    
    dt__FORALL(_patchName, ii,
      dictFile << "patch" << std::endl;    
      dictFile << _patchName[ii] << std::endl;    
      dictFile << "(" << std::endl;
      for (int kk=0;kk<_patchVertex[ii].size();kk = kk + 4) {
        dictFile << "( ";        
        dictFile << _patchVertex[ii].at(kk) << " ";
        dictFile << _patchVertex[ii].at(kk+1) << " ";
        dictFile << _patchVertex[ii].at(kk+2) << " ";
        dictFile << _patchVertex[ii].at(kk+3) << " ";
        dictFile << ")" << std::endl;                    
      }
      dictFile << ")" << std::endl;              
    );
    dictFile << ");" << std::endl;        
    
    //
    // close file
    //
    dictFile.close();
  }    
}
