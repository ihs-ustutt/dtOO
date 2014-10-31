#include "stlWriter.h"
#include <logMe/logMe.h>
#include <geometryEngine/dtSislSurf.h>


namespace dtOO {
    DTCLASSLOGMETHODI(stlWriter,
          << "No member attributes.");
    
  stlWriter::stlWriter() {
  }

  stlWriter::stlWriter(const stlWriter& orig) {
  }

  stlWriter::~stlWriter() {
  }
  
  void stlWriter::writeStlFile(const char* filename, std::vector<dtSislSurf *> surface) {
      std::fstream stlFile;
      stlFile.open(filename, std::ios::out | std::ios::trunc);
      
      //dtSislSurf * surfcopy = const_cast< dtSislSurf *> (surface);
      
      dtVector3 normalVec;
      std::vector<dtPoint3> vertex;
      float myResU = 50 ; 
      float myResV = 50 ;
      //float tmpUU, tmpVV ;
      int sizeUU = myResU + 1;
      int sizeVV = myResV + 1;
      dtSislSurf * surfcopy;
      
      DTINFOWF(writeStl,
            << DTLOGEVAL(vertex.max_size())  << LOGDEL
             << DTLOGEVAL(sizeUU)  << LOGDEL
                  << DTLOGEVAL(sizeVV)  << LOGDEL
              );
      
      for (int kk=0; kk<surface.size(); kk++) {
          surfcopy = surface.at(kk);
          vertex.clear();
          
      for (int UU=0; UU<myResU ; UU++) {
          for (int VV=0; VV<myResV ; VV++) {
              vertex.push_back( surfcopy->getPointPercent3d( (UU/myResU), (VV/myResV) ) );
              //tmpVV = tmpVV + myResV ;
          }
          vertex.push_back( surfcopy->getPointPercent3d( (UU/myResU), 1.) );
          //tmpUU = tmpUU + myResU ; 
      }
      for (int VV=0; VV<myResV ; VV++) {
              vertex.push_back( surfcopy->getPointPercent3d(1., (VV/myResV) ) );
              //tmpVV = tmpVV + myResV ;
      } 
      vertex.push_back( surfcopy->getPointPercent3d(1., 1.) );    
          DTINFOWF(writeStl,
            << DTLOGEVAL(vertex.size()) 
              );
      stlFile << "solid part " << kk <<std::endl;
      for (int ii=0; ii<(vertex.size()-sizeVV); ii+=sizeVV) { 
          for (int nn=0; nn<(sizeVV-1); nn++) {
              int jj = nn + ii;
              dtVector3 vecLowOne( vertex.at(jj), vertex.at(jj+sizeVV) );
              dtVector3 vecLowTwo( vertex.at(jj), vertex.at(jj+sizeVV+1) );
              dtVector3 vecNormLow = cross_product( vecLowOne, vecLowTwo );
              dtVector3 vecUpTwo( vertex.at(jj), vertex.at(jj+1) );
              dtVector3 vecNormUp = cross_product( vecLowTwo, vecUpTwo );
              
              stlFile << "facet normal " << vecNormLow << std::endl;
              stlFile << "outer loop" << std::endl;
              stlFile << "vertex " << vertex.at(jj) << std::endl;
              stlFile << "vertex " << vertex.at(jj+sizeVV) << std::endl;
              stlFile << "vertex " << vertex.at(jj+sizeVV+1) << std::endl;
              stlFile << "endloop" << std::endl;
              stlFile << "endfacet" << std::endl;
              
              stlFile << "facet normal " << vecNormUp << std::endl;
              stlFile << "outer loop" << std::endl;
              stlFile << "vertex " << vertex.at(jj) << std::endl;
              stlFile << "vertex " << vertex.at(jj+sizeVV+1) << std::endl;
              stlFile << "vertex " << vertex.at(jj+1) << std::endl;
              stlFile << "endloop" << std::endl;
              stlFile << "endfacet" << std::endl;
             
               DTINFOWF(writeStl(),
            << DTLOGEVAL(nn)  << LOGDEL
             << DTLOGEVAL(ii)  
              );                        
          }
      
      }
      stlFile << "endsolid part " << kk <<std::endl;
      }
      stlFile.close();
  }
  
//  void stlWriter::initFile(const char * filename) {
//      std::fstream stlFile;
//      stlFile.open(filename, std::ios::out | std::ios::app);
//      
//      
//  }
  
  void stlWriter::writeSurf(dtSislSurf const * surface) {
      
  }
  
//  void stlWriter::closeFile(const char * filename) {
//      
//      
//  }
  
}
