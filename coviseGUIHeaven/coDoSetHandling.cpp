#include "coDoSetHandling.h"
#include <interfaceHeaven/renderInterface.h>
#include <do/coDoSet.h>
#include <do/coDoData.h>
#include <do/coDoPolygons.h>
#include <do/coDoUnstructuredGrid.h>
#include <discrete2dPoints.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>
#include <solid2dLine.h>
#include <solid3dLine.h>
#include <solid3dSurface.h>
#include <unstructured3dMesh.h>
#include <unstructured3dSurfaceMesh.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <boost/smart_ptr/scoped_ptr.hpp>

namespace dtOO {  
  coDoSetHandling::coDoSetHandling() {
  }

  coDoSetHandling::~coDoSetHandling() {
  }

  covise::coDoSet * coDoSetHandling::render3d(
    coDoSetHandling::const_iterator const & it, char const * str
  ) const {   
    return renderElement3d(*it, str);
	}
	
  covise::coDoSet * coDoSetHandling::render3d(
    coDoSetHandling::const_iterator const & first, 
    coDoSetHandling::const_iterator const & last, 
    char const * str
  ) const {
    covise::coDoSet * bigCoDoSetP = NULL; 
		int counter = 0;
    for (coDoSetHandling::const_iterator it = first; it != last; ++it) {
      //
      // check if object has the correct interface
      //
      renderInterface const * cDSI = *it;
      
      //
      // find fist element
      //
      if (bigCoDoSetP == NULL) {
        bigCoDoSetP = renderElement3d(cDSI, str);
        continue;
      }
      else {
        char * objName = new char[strlen(str)+5];
        sprintf(objName,"%s_%d", str, counter);
        covise::coDoSet * smallCoDoSetP = renderElement3d(cDSI, objName);
        if (smallCoDoSetP != NULL) {
          for (int jj=0;jj<smallCoDoSetP->getNumElements();jj++) {
            bigCoDoSetP->addElement(smallCoDoSetP->getElement(jj));
          }
          delete smallCoDoSetP;
        }
      }
			counter++;
    }
    
    return bigCoDoSetP;   
	}
	
  covise::coDoSet * coDoSetHandling::render3d( 
                      coDoSetHandling const & vec,
                      char const * str
                    ) const {
		return render3d(vec.begin(), vec.end(), str);
  }

  covise::coDoSet * coDoSetHandling::render2d( 
                      coDoSetHandling const & vec,
                      char const * str
                    ) const {
    covise::coDoSet * bigCoDoSetP = NULL;  
		float xMin = 0.;
		float xMax = 0.; 
		float yMin = 0.; 
		float yMax = 0.;		
    
		std::string plbuf;
		std::ostringstream outStream;      
		outStream << "\0"
							<< "SETS LINESTYLE 1\n"
							<< "SETS LINEWIDTH 2\n"
							<< "FRAME ON\n"
							<< "LEGEND ON \n"
							<< "LEGEND BOX ON \n"
							<< "LEGEND BOX FILL ON\n"
							<< "LEGEND BOX FILL COLOR 0\n";
    int nLines = 0;
    for (int ii=0; ii<vec.size(); ii++) {
      //
      // check if object has the correct interface
      //
      renderInterface const * cDSI = vec[ii];

		  if ( vec[ii]->getMin(0) < xMin ) xMin = vec[ii]->getMin(0);
			if ( vec[ii]->getMin(1) < yMin ) yMin = vec[ii]->getMin(1);
		  if ( vec[ii]->getMax(0) > xMax ) xMax = vec[ii]->getMax(0);
			if ( vec[ii]->getMax(1) > yMax ) yMax = vec[ii]->getMax(1);
      
      //
      // find fist element
      //
      if (bigCoDoSetP == NULL) {
        bigCoDoSetP = renderElement2d(cDSI, str);
		    xMin = vec[ii]->getMin(0); 
		    xMax = vec[ii]->getMax(0); 
		    yMin = vec[ii]->getMin(1); 
		    yMax = vec[ii]->getMax(1);	        
        //continue;
      }
      else {
        char * objName = new char[strlen(str)+5];
        sprintf(objName,"%s_%d", str, ii);
        covise::coDoSet * smallCoDoSetP = renderElement2d(cDSI, objName);
        if (smallCoDoSetP != NULL) {
          for (int jj=0;jj<smallCoDoSetP->getNumElements();jj++) {
            bigCoDoSetP->addElement(smallCoDoSetP->getElement(jj));
          }
          delete smallCoDoSetP;
        }
      }
      
//      optionGroupInt oG = vec[ii]->refOptionHandling().getOptionGroupInt("covise_0");
//      int opCounter = 0;
	    for (int kk=nLines;kk<bigCoDoSetP->getNumElements();kk++) {
        outStream << "S" << kk << " COLOR " << ii+1 << "\n";
//        std::cout << "S" << nLines << " " << oG[kk].first[0] << " " << oG[kk].second << "\n";
//        nLines++;
      }      
      nLines = bigCoDoSetP->getNumElements();
    }


//	  for (int ii=0;ii<bigCoDoSetP->getNumElements();ii++) {
//      outStream << "S" << ii << " COLOR " << ii+1 << "\n";
//    }
		outStream << "WORLD " << xMin << "," << yMin << "," << xMax << "," << yMax << "\n";
		outStream << "YAXIS TICK MAJOR " << (yMax-yMin)/5 << "\nYAXIS TICK MINOR " << (yMax-yMin)/10 << "\n"
							<< "XAXIS TICK MAJOR " << (xMax-xMin)/5 << "\nXAXIS TICK MINOR " << (xMax-xMin)/10 << "\n";
		plbuf = outStream.str();      
		bigCoDoSetP->addAttribute("COMMANDS", plbuf.c_str());    
    
    return bigCoDoSetP;   
  }
     
  covise::coDoSet * coDoSetHandling::createCoDoSet( 
		covise::coDistributedObject * const cdo, char const * str
	) {
		covise::coDistributedObject** cdoa = new covise::coDistributedObject*[2];
		
		cdoa[0] = cdo;
		cdoa[1] = NULL;
    covise::coDoSet * set = new covise::coDoSet(str, cdoa);
		
		delete cdoa;
		
    return set;   
  }     
	
  covise::coDoSet * coDoSetHandling::toCoDoSet( 
                      covise::coDoSet * setOne,
                      covise::coDoSet * setTwo,
                      char const * str
                    ) {
		dt__THROW_IF(setOne==NULL, toCoDoSet());
		
		//
		// find fist element
		//
		if (setTwo != NULL) {
			for (int jj=0;jj<setTwo->getNumElements();jj++) {
				setOne->addElement(setTwo->getElement(jj));
			}
			delete setTwo;
		}
    return setOne;   
  }      
	
	covise::coDoSet * coDoSetHandling::toCoDoSet( renderInterface const * const rI, char const * str ) {
		discrete2dPoints const * const d2dP = discrete2dPoints::ConstDownCast(rI);
		discrete3dPoints const * const d3dP = discrete3dPoints::ConstDownCast(rI);
		solid2dLine const * const s2dL = solid2dLine::ConstDownCast(rI);
		solid3dLine const * const s3dL = solid3dLine::ConstDownCast(rI);
		solid3dSurface const * const s3dS = solid3dSurface::ConstDownCast(rI);
		unstructured3dMesh const * const u3dM = unstructured3dMesh::ConstDownCast(rI);
		unstructured3dSurfaceMesh const * const u3dSM = unstructured3dSurfaceMesh::ConstDownCast(rI);
		discrete3dVector const * const d3dV = discrete3dVector::ConstDownCast(rI);
		
		if (d2dP) {
			return discrete2dPointsToCoDoSet(d2dP, str);
		}
		else if (d3dP) {
			return discrete3dPointsToCoDoSet(d3dP, str);
		}
		else if (s2dL) {
			return solid2dLineToCoDoSet(s2dL, str);
		}
		else if (s3dL) {
			return solid3dLineToCoDoSet(s3dL, str);
		}
		else if (s3dS) {
			return solid3dSurfaceToCoDoSet(s3dS, str);
		}
		else if (u3dM) {
			return unstructured3dMeshToCoDoSet(u3dM, str);
		}		
		else if (u3dSM) {
			return unstructured3dSurfaceMeshToCoDoSet(u3dSM, str);
		}			
		else if (d3dV) {
			return discrete3dVectorToCoDoSet(d3dV, str);
		}				
		else {
			dt__THROW(
				coDoSetHandling(), 
				<< "Unknown renderInterface type." << LOGDEL
				<< DTLOGEVAL(rI->className())
			);
		}
	}
	
	covise::coDoSet * coDoSetHandling::renderElement2d( renderInterface const * const rI, char const * const str) {
		vectorHandling< renderInterface * > vec = rI->getRender();
		covise::coDoSet * set	= NULL;
		if (vec.size() != 0) {
			set = toCoDoSet(vec[0], str);
			float xMin = vec[0]->getMin(0); 
			float xMax = vec[0]->getMax(0); 
			float yMin = vec[0]->getMin(1); 
			float yMax = vec[0]->getMax(1);		
			for (int ii=1; ii<vec.size(); ii++) {
				if ( vec[ii]->getMin(0) < xMin ) xMin = vec[ii]->getMin(0);
				if ( vec[ii]->getMin(1) < yMin ) yMin = vec[ii]->getMin(1);
				if ( vec[ii]->getMax(0) > xMax ) xMax = vec[ii]->getMax(0);
				if ( vec[ii]->getMax(1) > yMax ) yMax = vec[ii]->getMax(1);
				set = toCoDoSet( set, toCoDoSet(vec[ii], str), str );
			}

			rI->setMin(0, xMin);
			rI->setMin(1, yMin);
			rI->setMax(0, xMax);
			rI->setMax(1, yMax);
		}
		return set;		
	}

	covise::coDoSet * coDoSetHandling::renderElement3d( renderInterface const * const rI, char const * const str) {
		vectorHandling< renderInterface * > vec = rI->getRender();
		covise::coDoSet * set	= NULL;
		if ( (vec.size() != 0) && !rI->mustExtRender() ) {
			set = toCoDoSet(vec[0], str);
		
			for (int ii=1; ii<vec.size(); ii++) {
				set 
				= 
				coDoSetHandling::toCoDoSet(
					set, 
					toCoDoSet(vec[ii], str),
					str
				);
			}	
			if (rI->mustExtRender()) {
				vec.destroy();
				vec = rI->getExtRender();
				dt__forAllIndex(vec, ii) {
					set 
					= 
					coDoSetHandling::toCoDoSet( set, toCoDoSet(vec[ii], str), str );
				}
			}
		}
		else {
			vectorHandling< renderInterface * > vec2 = rI->getExtRender();
			
      vec.reserve( vec.size() + vec2.size() );
      vec.insert( vec.end(), vec2.begin(), vec2.end() );

			if (vec.size()==0) return NULL;
			set = toCoDoSet(vec[0], str);			
			vectorHandling< renderInterface * >::const_iterator cit;
			DTINFOWF(renderElement3d(), << DTLOGEVAL(vec.size()) );
			if (vec.size() > 1) {
				for (cit = progHelper::next(vec.begin()); cit != vec.end(); ++cit) {
					set 
					= 
					coDoSetHandling::toCoDoSet(set, toCoDoSet(*cit, str), str);
				}
			}
		}
		
		return set;
	}	
	
	covise::coDoSet * coDoSetHandling::discrete2dPointsToCoDoSet( discrete2dPoints const * const rI, char const * str ) {
    float * xpl = new float[rI->refP2().size()];
    float * ypl = new float[rI->refP2().size()];
    float xMin = rI->getMin(0);
    float xMax = rI->getMax(0);  
    float yMin = rI->getMin(1);
    float yMax = rI->getMax(1);
    for (int ii=0;ii<rI->refP2().size();ii++) {
      xpl[ii] = rI->refP2()[ii].x();
      ypl[ii] = rI->refP2()[ii].y();
//      if (xpl[ii] < xMin) {
//        xMin = xpl[ii];
//      }
//      if (xpl[ii] > xMax) {
//        xMax = xpl[ii];
//      }
//      if (ypl[ii] < yMin) {
//        yMin = ypl[ii];
//      }
//      if (ypl[ii] > yMax) {
//        yMax = ypl[ii];
//      }
    }

    char * objName;
    objName = new char[strlen(str)+5];
    sprintf(objName,"%s_%d", str,0);
    ptrHandling<covise::coDoVec2> cDVec( 
		  new covise::coDoVec2(objName, rI->refP2().size(), xpl, ypl)
		);
		delete [] objName;
		
//		std::string plbuf;
//		std::ostringstream outStream;      
//		outStream << "\0"
//							<< "SETS LINESTYLE 1\n"
//							<< "S0 LINESTYLE 3\n"
//							<< "SETS LINEWIDTH 2\n"
//							<< "S0 LINEWIDTH 1\n"
//							<< "S0 SYMBOL 2\n";   
//							<< "WORLD " << xMin << "," << yMin << "," << xMax << "," << yMax << "\n"
//							<< "FRAME ON\n"
//							<< "LEGEND ON \n"
//							<< "LEGEND BOX ON \n"
//							<< "LEGEND BOX FILL ON\n"
//							<< "LEGEND BOX FILL COLOR 0\n"
//							<< "SETS COLOR " << 1 << "\n"
//							<< "S0 COLOR " << 1 << "\n"
//							<< "YAXIS TICK MAJOR " << (yMax-yMin)/5 << "\nYAXIS TICK MINOR " << (yMax-yMin)/10 << "\n"
//							<< "XAXIS TICK MAJOR " << (xMax-xMin)/5 << "\nXAXIS TICK MINOR " << (xMax-xMin)/10 << "\n";
//		plbuf = outStream.str();      

		covise::coDistributedObject ** cDObj 
		=
		new covise::coDistributedObject*[2];  
		cDObj[0] = cDVec.get();
		cDObj[1] = NULL;
		covise::coDoSet * returnSet = new covise::coDoSet(str, cDObj);  
		delete [] cDObj;
//		cDVec->addAttribute("COMMANDS", plbuf.c_str());
		return returnSet;		
	}
	
	covise::coDoSet * coDoSetHandling::discrete3dPointsToCoDoSet( discrete3dPoints const * const rI, char const * str ) {
    covise::coDistributedObject ** objects = new covise::coDistributedObject*[2];
    objects[1] = NULL;
    ptrHandling<char> objName( new char[strlen(str)+5] );
    sprintf(objName.get(),"%s_%d", str,0);

		//
		// create polygon object
		//
    int nPoints = rI->refP3().size() * 6;
    int nCorners = rI->refP3().size() * 8 * 3;
    int nPolygons = rI->refP3().size() * 8;
    ptrHandling<covise::coDoPolygons> oneObject(
		  new covise::coDoPolygons( objName.get(), nPoints, nCorners, nPolygons)
		);
		
		//
		// request internal data arraies
		//
		float* xCoords;
    float* yCoords;
    float* zCoords;
    int* firstList; //corners
    int* secondList; //polygons
    oneObject->getAddresses(&xCoords, &yCoords, &zCoords, &firstList, &secondList);
		
		//
		// get point height
		//
		float height 
    = 
    staticPropertiesHandler::getInstance()->getOptionFloat("point_render_diameter");  		
		//
		// fill data arraies
		//
    int counterPoints = 0;
    int counterCorners = 0;
    int counterPolygons = 0;
    int ids[6];		
    for(int ii=0;ii<rI->refP3().size();ii++) {
			dtPoint3 edge[6];
			dtVector3 xx(height,0,0);
			dtVector3 yy(0, height,0);
			dtVector3 zz(0, 0, height);			
      edge[0] = rI->refP3()[ii] + yy;
      edge[1] = rI->refP3()[ii] + xx;
      edge[2] = rI->refP3()[ii] + zz;
      edge[3] = rI->refP3()[ii] - xx;
      edge[4] = rI->refP3()[ii] - zz;
      edge[5] = rI->refP3()[ii] - yy;
      for (int jj=0;jj<6;jj++) {
        ids[jj] = counterPoints;
        *(xCoords+counterPoints) = edge[jj].x();
        *(yCoords+counterPoints) = edge[jj].y();
        *(zCoords+counterPoints) = edge[jj].z();
        counterPoints = counterPoints + 1;
      }
      secondList[counterPolygons] = counterCorners;
      firstList[counterCorners+ 0] = ids[0];
      firstList[counterCorners+ 1] = ids[1];
      firstList[counterCorners+ 2] = ids[2];
      counterPolygons = counterPolygons + 1;
      counterCorners = counterCorners + 3;      
      secondList[counterPolygons] = counterCorners;
      firstList[counterCorners+ 0] = ids[0];
      firstList[counterCorners+ 1] = ids[2];
      firstList[counterCorners+ 2] = ids[3];      
      counterPolygons = counterPolygons + 1;
      counterCorners = counterCorners + 3;      
      secondList[counterPolygons] = counterCorners;      
      firstList[counterCorners+ 0] = ids[0];
      firstList[counterCorners+ 1] = ids[3];
      firstList[counterCorners+ 2] = ids[4];
      counterPolygons = counterPolygons + 1;
      counterCorners = counterCorners + 3;      
      secondList[counterPolygons] = counterCorners;      
      firstList[counterCorners+ 0] = ids[0];
      firstList[counterCorners+ 1] = ids[4];
      firstList[counterCorners+ 2] = ids[1];       
      counterPolygons = counterPolygons + 1;
      counterCorners = counterCorners + 3;      
      secondList[counterPolygons] = counterCorners;      
      firstList[counterCorners+ 0] = ids[5];
      firstList[counterCorners+ 1] = ids[1];
      firstList[counterCorners+ 2] = ids[2];
      counterPolygons = counterPolygons + 1;
      counterCorners = counterCorners + 3;      
      secondList[counterPolygons] = counterCorners;      
      firstList[counterCorners+ 0] = ids[5];
      firstList[counterCorners+ 1] = ids[2];
      firstList[counterCorners+ 2] = ids[3];      
      counterPolygons = counterPolygons + 1;
      counterCorners = counterCorners + 3;      
      secondList[counterPolygons] = counterCorners;
      firstList[counterCorners+ 0] = ids[5];
      firstList[counterCorners+ 1] = ids[3];
      firstList[counterCorners+ 2] = ids[4];
      counterPolygons = counterPolygons + 1;
      counterCorners = counterCorners + 3;      
      secondList[counterPolygons] = counterCorners;      
      firstList[counterCorners+ 0] = ids[5];
      firstList[counterCorners+ 1] = ids[4];
      firstList[counterCorners+ 2] = ids[1];
      counterPolygons = counterPolygons + 1;
      counterCorners = counterCorners + 3;     
    }
		
		//
		// add attributes and create coDoSet
		//
    oneObject->addAttribute("COLOR", "green");
    objects[0] = oneObject.get();
		covise::coDoSet * retSet = new covise::coDoSet(str, objects);
		
		//
		// delete array and return set
		//
	  delete [] objects;
		return retSet;		
	}
	
  covise::coDoSet * coDoSetHandling::solid2dLineToCoDoSet( solid2dLine const * const rI, char const * str ) {
		return discrete2dPointsToCoDoSet(rI, str);
	}
	
  covise::coDoSet * coDoSetHandling::solid3dLineToCoDoSet( solid3dLine const * const rI, char const * str ) {
    char * objName = new char[strlen(str)+5];
    sprintf(objName,"%s_%d", str, 0);

    ptrHandling< covise::coDoLines > lines(
      new covise::coDoLines(objName, rI->refP3().size(), rI->refP3().size(), 1)
    );
    
    float * xx;
    float * yy;
    float * zz;
    int * cL;
    int * lL;      
    lines->getAddresses(&xx, &yy, &zz, &cL, &lL);
    *lL = 0;
    

//    float interval = (getUMax() - getUMin()) / (getRenderResolutionU()-1);
    for(int ii=0; ii<rI->refP3().size(); ii++){
//      float paraValue = getUMin() + ii * interval;
      dtPoint3 pp = rI->refP3()[ii];
      *(xx+ii) = pp.x();
      *(yy+ii) = pp.y();
      *(zz+ii) = pp.z();
      *(cL+ii) = ii;
    }
//    addAttributesToCoDoDis( lines.get() );
    
    ptrHandling< covise::coDistributedObject * > cdo( 
      new covise::coDistributedObject*[2] 
    );
    cdo.get()[0] = lines.get();
    cdo.get()[1] = NULL;
    
    covise::coDoSet * retSet = new covise::coDoSet(str, cdo.get());
    
    return retSet;		
	}
	
  covise::coDoSet * coDoSetHandling::solid3dSurfaceToCoDoSet( solid3dSurface const * const rI, char const * str ) {
		int renderResU = rI->refP3().size(0)-1;
		int renderResV = rI->refP3().size(1)-1;	
    //
    //create coDoPolygon
    //
    ptrHandling<char> objName( new char[strlen(str)+5] );
    int polyNumber = 0;
    sprintf(objName.get(), "%s_%d", str, polyNumber);
    int nPoints = (renderResU+1)*(renderResV+1);
    int nPolygons = (renderResU)*(renderResV);
    int nCorners = nPolygons*4;
    ptrHandling< covise::coDoPolygons > polygonP( 
		  new covise::coDoPolygons(objName.get(), nPoints, nCorners, nPolygons) 
		);
    float * xCoord;
    float * yCoord;
    float * zCoord;
    int * cornerList;
    int * polygonList;		
    polygonP->getAddresses(&xCoord, &yCoord, &zCoord, &cornerList, &polygonList);

    //
    //write points to coDoPolygons
    //
    int counter = 0;
		int counterTwo = 0;
    for(int ii=0;ii<=renderResV;ii++){
      for (int jj=0;jj<=renderResU;jj++) {
        xCoord[counter] = (rI->refP3()[jj][ii]).x();
        yCoord[counter] = (rI->refP3()[jj][ii]).y();
        zCoord[counter] = (rI->refP3()[jj][ii]).z();
        counter++;
      }
    }
    //
    //write corner and polygon list
    //
    counter = 0;
    counterTwo = 0;
    int offsetFirst = 1;
    int offsetSecond = renderResU + 1;			
    for(int ii=0;ii<renderResV;ii++){
      for (int jj=0;jj<renderResU;jj++) {
        polygonList[counterTwo] = counter;
        counterTwo++;
        cornerList[counter] = (jj + ii*offsetSecond);
        cornerList[counter+1] = (cornerList[counter]+offsetFirst);
        cornerList[counter+2] = (cornerList[counter]+offsetSecond+offsetFirst);      
        cornerList[counter+3] = (cornerList[counter]+offsetSecond);
        counter = counter+4;
      }
    }
    //finish lineSet
//    addAttributesToCoDoDis( polygonP.get() );
    covise::coDistributedObject ** polygonObjects = new covise::coDistributedObject* [2];
    polygonObjects[0] = polygonP.get();
    polygonObjects[1] = NULL;
    //create line set 
    covise::coDoSet * retSet = new covise::coDoSet(str, polygonObjects);  
    
    delete [] polygonObjects;
    
    return retSet;		
	}
	
  covise::coDoSet * coDoSetHandling::unstructured3dMeshToCoDoSet( unstructured3dMesh const * const rI, char const * str ) {
    int nElemTot = rI->getNHex() + rI->getNTet() + rI->getNPri() + rI->getNPyr();
		std::string objName = std::string(str)+"_uns3dGrid";
		ptrHandling< covise::coDoUnstructuredGrid > cug( 
      new covise::coDoUnstructuredGrid( 
        objName.c_str(),
        nElemTot, 
          4*rI->getNTet() //tetrahedra
        + 8*rI->getNHex() //hexahedra
        + 6*rI->getNPri() //prisms
        + 5*rI->getNPyr() //pyramids
        + 0*0, //polyhedra - not yet supported
        rI->refP3().size(), 
        1 
      )
    );
    int * elem;
    int * conn;
    float * xx;
    float * yy;
    float * zz;
    int * tl;
        
    cug->getAddresses(&elem, &conn, &xx, &yy, &zz);
    cug->getTypeList(&tl);
		
		dt__forAllIndex(rI->refP3(), ii) {
			xx[ii] = static_cast< float >(rI->refP3()[ii].x());
			yy[ii] = static_cast< float >(rI->refP3()[ii].y());
			zz[ii] = static_cast< float >(rI->refP3()[ii].z());
	  }

    int cellC = 0;
    int connC = 0;  

		for( int ii=0; ii<rI->refEl().size(); ii++ ) {
			//
			// tetrahedron
			//				
			if ( rI->refEl()[ii].size() == 4 ) {
				elem[cellC] = connC;
				tl[cellC] = TYPE_TETRAHEDER;      
				conn[elem[cellC]+0] = rI->refEl()[ii][0];
				conn[elem[cellC]+1] = rI->refEl()[ii][1];
				conn[elem[cellC]+2] = rI->refEl()[ii][2];
				conn[elem[cellC]+3] = rI->refEl()[ii][3];
				connC = connC + 4;
				cellC++;      
			}      
			//
			// pyramid
			//
			else if ( rI->refEl()[ii].size() == 5 ) {
				elem[cellC] = connC;
				tl[cellC] = TYPE_PYRAMID;      
				conn[elem[cellC]+0] = rI->refEl()[ii][0];
				conn[elem[cellC]+1] = rI->refEl()[ii][1];
				conn[elem[cellC]+2] = rI->refEl()[ii][2];
				conn[elem[cellC]+3] = rI->refEl()[ii][3];
				conn[elem[cellC]+4] = rI->refEl()[ii][4];
				connC = connC + 5;
				cellC++;      
			}			
			//
			// prism
			//
			else if ( rI->refEl()[ii].size() == 6 ) {
				elem[cellC] = connC;
				tl[cellC] = TYPE_PRISM;      
				conn[elem[cellC]+0] = rI->refEl()[ii][0];
				conn[elem[cellC]+1] = rI->refEl()[ii][1];
				conn[elem[cellC]+2] = rI->refEl()[ii][2];
				conn[elem[cellC]+3] = rI->refEl()[ii][3];
				conn[elem[cellC]+4] = rI->refEl()[ii][4];
				conn[elem[cellC]+5] = rI->refEl()[ii][5];
				connC = connC + 6;
				cellC++;      
			}						
			//
			// hexahedron
			//
			else if ( rI->refEl()[ii].size() == 8 ) {
				elem[cellC] = connC;
				tl[cellC] = TYPE_HEXAEDER;      
				conn[elem[cellC]+0] = rI->refEl()[ii][0];
				conn[elem[cellC]+1] = rI->refEl()[ii][1];
				conn[elem[cellC]+2] = rI->refEl()[ii][2];
				conn[elem[cellC]+3] = rI->refEl()[ii][3];
				conn[elem[cellC]+4] = rI->refEl()[ii][4];
				conn[elem[cellC]+5] = rI->refEl()[ii][5];
				conn[elem[cellC]+6] = rI->refEl()[ii][6];
				conn[elem[cellC]+7] = rI->refEl()[ii][7];
				connC = connC + 8;
				cellC++;      
			}
			else {
				dt__THROW(
					unstructured3dMeshToCoDoSet(),
					<< "Element is not yet supported." << LOGDEL
					<< DTLOGEVAL(rI->refEl()[ii].size()) );
			}
		}
		
    ptrHandling< covise::coDistributedObject * > cdo( new covise::coDistributedObject*[2] );
    cdo.get()[0] = cug.get();
    cdo.get()[1] = NULL;
    return new covise::coDoSet(str, cdo.get());				
	}

  covise::coDoSet * coDoSetHandling::unstructured3dSurfaceMeshToCoDoSet( unstructured3dSurfaceMesh const * const rI, char const * str ) {
    int nElemTot = rI->getNQuads() + rI->getNTris();
		std::string objName = std::string(str)+"_uns3dSGrid";
		ptrHandling< covise::coDoUnstructuredGrid > cug( 
      new covise::coDoUnstructuredGrid( 
        objName.c_str(),
        nElemTot, 
        4*rI->getNQuads() // quadrangle
				+ 3*rI->getNTris(), // triangles
        rI->refP3().size(), 
        1 
      )
    );
    int * elem;
    int * conn;
    float * xx;
    float * yy;
    float * zz;
    int * tl;
        
    cug->getAddresses(&elem, &conn, &xx, &yy, &zz);
    cug->getTypeList(&tl);
		
		dt__forAllIndex(rI->refP3(), ii) {
			xx[ii] = static_cast< float >(rI->refP3()[ii].x());
			yy[ii] = static_cast< float >(rI->refP3()[ii].y());
			zz[ii] = static_cast< float >(rI->refP3()[ii].z());
	  }

    int cellC = 0;
    int connC = 0;  

		for( int ii=0; ii<rI->refEl().size(); ii++ ) {
			//
			// quadrangle
			//				
			if ( rI->refEl()[ii].size() == 4 ) {
				elem[cellC] = connC;
				tl[cellC] = TYPE_QUAD;      
				conn[elem[cellC]+0] = rI->refEl()[ii][0];
				conn[elem[cellC]+1] = rI->refEl()[ii][1];
				conn[elem[cellC]+2] = rI->refEl()[ii][2];
				conn[elem[cellC]+3] = rI->refEl()[ii][3];
				connC = connC + 4;
				cellC++;      
			}      
			//
			// triangle
			//
			else if ( rI->refEl()[ii].size() == 3 ) {
				elem[cellC] = connC;
				tl[cellC] = TYPE_TRIANGLE;      
				conn[elem[cellC]+0] = rI->refEl()[ii][0];
				conn[elem[cellC]+1] = rI->refEl()[ii][1];
				conn[elem[cellC]+2] = rI->refEl()[ii][2];
				connC = connC + 3;
				cellC++;      
			}      			
			else {
				dt__THROW(
					unstructured3dSurfaceMeshToCoDoSet(),
					<< "Element is not yet supported." << LOGDEL
					<< DTLOGEVAL(rI->refEl()[ii].size()) );
			}
		}
		
    ptrHandling< covise::coDistributedObject * > cdo( new covise::coDistributedObject*[2] );
    cdo.get()[0] = cug.get();
    cdo.get()[1] = NULL;
    return new covise::coDoSet(str, cdo.get());				
	}
	
  covise::coDoSet * coDoSetHandling::discrete3dVectorToCoDoSet( discrete3dVector const * const rI, char const * str ) {
    float tipSize
		=
		staticPropertiesHandler::getInstance()->getOptionFloat("vector_render_size");
		
    ptrHandling<char> objName(new char[strlen(str)+5]);
    sprintf(objName.get(),"%s_%d", str,0);

		//
		// create polygon object
		//
    int nPolyPoints = rI->refP3().size() * 5;
    int nPolyCorners = rI->refP3().size() * 6; 
    int nPolyPolygons = rI->refP3().size() * 2;  
    ptrHandling< covise::coDoPolygons > onePolyObject(
		  new covise::coDoPolygons(objName.get() ,nPolyPoints, nPolyCorners, nPolyPolygons)
		); 
    float* xCoords;
    float* yCoords;
    float* zCoords;
    int* polyPolyList; 
    int* polyCornerList; 		
    onePolyObject->getAddresses(&xCoords, &yCoords, &zCoords, &polyCornerList, &polyPolyList);
    int polyCoordsCounter = 0;
    int polyCornerCounter = 0;
    int polyPolyCounter = 0;

    //
		// create line object
		//
//    int nLinePoints = rI->refP3().size() * 2; 
//    int nLineCorners = rI->refP3().size() * 2; 
//    int nLines = rI->refP3().size();  
//    ptrHandling< covise::coDoLines > oneLineObject(
//		  new covise::coDoLines(objName.get(), nLinePoints, nLineCorners, nLines)
//		); 
//    float* xLineCoords;
//    float* yLineCoords;
//    float* zLineCoords;
//    int* lineList; 
//    int* lineCornerList; 		
//    oneLineObject->getAddresses(&xLineCoords, &yLineCoords, &zLineCoords, &lineCornerList, &lineList);

//    int lineCoordsCounter = 0;
//    int lineCornerCounter = 0;
//    int lineLineCounter = 0;
    for(int ii=0;ii<rI->refP3().size();ii++) {
      dtPoint3 corner[5];			
      float vecLengthU = sqrt( rI->refV3()[ii].squared_length() );
      dtVector3 normVecU = rI->refV3()[ii] / (vecLengthU);
      dtPoint3 topCorner = rI->refP3()[ii] + rI->refV3()[ii];
      dtPoint3 baseMiddle = topCorner - tipSize * normVecU;
      corner[0] = topCorner;
      float ny, nx, uyx;
      if (rI->refV3()[ii].x() == 0) {
        nx = 1.;
        ny = 0.;
      }
      else {
        uyx = rI->refV3()[ii].y() / rI->refV3()[ii].x();
        ny = sqrt(1 / (1 + (uyx * uyx)));
        nx = -uyx * ny;
      }

      dtVector3 nVec(nx,ny,0.);
      corner[1] = baseMiddle - tipSize * nVec;
      corner[2] = baseMiddle + tipSize * nVec;
      dtVector3 nVecOrtho = dtLinearAlgebra::crossProduct (rI->refV3()[ii], nVec);
      float nVecOrthoLength = sqrt( nVecOrtho.squared_length() );
      dtVector3 nVecOrthoNorm = nVecOrtho / (nVecOrthoLength);
      corner[3] = baseMiddle - tipSize * nVecOrthoNorm;
      corner[4] = baseMiddle + tipSize * nVecOrthoNorm;

      int countPoints = 0;
      int idC[5];
      for (int jj=0;jj<5;jj++) {
        idC[jj] = countPoints+polyCoordsCounter;
        *(xCoords+countPoints+polyCoordsCounter) = corner[jj].x();
        *(yCoords+countPoints+polyCoordsCounter) = corner[jj].y();
        *(zCoords+countPoints+polyCoordsCounter) = corner[jj].z();
        countPoints = countPoints + 1;
      }      
      polyCoordsCounter = polyCoordsCounter + 5;

      polyPolyList[polyPolyCounter+0] = polyCornerCounter+0;
      polyCornerList[polyCornerCounter+0] = idC[0];
      polyCornerList[polyCornerCounter+1] = idC[1];
      polyCornerList[polyCornerCounter+2] = idC[2];
      polyPolyList[polyPolyCounter+1] = polyCornerCounter+3;
      polyCornerList[polyCornerCounter+3] = idC[0];
      polyCornerList[polyCornerCounter+4] = idC[3];
      polyCornerList[polyCornerCounter+5] = idC[4];
      polyCornerCounter = polyCornerCounter+6;
      polyPolyCounter = polyPolyCounter+2;     


//      *(xLineCoords+lineCoordsCounter) = topCorner.x();
//      *(yLineCoords+lineCoordsCounter) = topCorner.y();
//      *(zLineCoords+lineCoordsCounter) = topCorner.z();
//      *(xLineCoords+lineCoordsCounter+1) = rI->refP3()[ii].x();
//      *(yLineCoords+lineCoordsCounter+1) = rI->refP3()[ii].y();
//      *(zLineCoords+lineCoordsCounter+1) = rI->refP3()[ii].z();
//      lineList[0+lineLineCounter] = 0+lineCornerCounter;
//      lineCornerList[0+lineCornerCounter] = lineCoordsCounter+0;
//      lineCornerList[1+lineCornerCounter] = lineCoordsCounter+1;
//      lineCoordsCounter = lineCoordsCounter+2;
//      lineLineCounter++;      
//      lineCornerCounter = lineCornerCounter + 2;
    }
    
		//
		// create set
		//
    covise::coDistributedObject ** objects = new covise::coDistributedObject*[2];
    objects[0] = onePolyObject.get();
//    objects[1] = oneLineObject.get();
		objects[1] = NULL;
    objects[0]->addAttribute("COLOR", "magenta");
    
    covise::coDoSet * retSet = new covise::coDoSet(str, objects);

		delete [] objects;
		
		return retSet;
	}
}