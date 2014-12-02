#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>
#include <catchMeanline.h>
#include <ffMeanline.h>
#include <constValueHeaven/constValue.h>
#include <interfaceHeaven/vectorHandling.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <vector>

#include <iostream>
#include <fstream>

using namespace dtOO;

int main(int argc, char **argv)
{
    try
    {
      
    pointContainer pC;
    vectorContainer vC;
    std::vector<double> meanlineParam;
    std::vector<std::string > label;
    vectorHandling<constValue *> cV1, cV2;
    vectorHandling<constValue *> * cVptr1;
    vectorHandling<constValue *> * cVptr2;
    cVptr1 = &cV1;
    cVptr2 = &cV2;
    dtXmlParser parser1, parser2;
    float max;
    float min;

    // neuer kommentar

  //Aufruf des Gesamten Algorithmuses//

	meanlineParam = catchMeanline(argc, argv);	//als Rückgabe werden die Individuen in einen Vector geschrieben, wichtig zum Umrechnen in die tatsächlichen Parameter
	
	parser1.openFileAndParse("surface.xml");	//constValues der Meanlineparameter in ein Xml-File schreiben. Dafür werden die constValues aus surface.xml eingelesen, und die nur die Werte verändert.

	parser1.getNames("constValue", &label);
	for (int ii = 0;ii<label.size();ii++) {
	    parser1.createConstValue(label[ii], &cV1);	//Erzeugen der ursprünglichen cV-Werte des 3PMeanline
	    }
	
	label.clear();
	
	for (int ii = 0; ii<3; ii++){
	  
	    min = cV1[ii]->getMin();
	    max = cV1[ii]->getMax();

	    cV1[ii]->setValue(fabs(min+(max-min)*(static_cast<float>(meanlineParam[ii]))));	//Überschreiben der Werte der constValues der 3PMeanline mit den Ergebnissen des Algorithmus
	  
	  }
	
	parser2.openFileAndParse("thickness.xml");	//constValues der Meanlineparameter in ein Xml-File schreiben. Dafür werden die constValues aus surface.xml eingelesen, und die nur die Werte verändert.

	parser2.getNames("constValue", &label);
	for (int ii = 0;ii<label.size();ii++) {
	    parser2.createConstValue(label[ii], &cV2);	//Erzeugen der ursprünglichen cV-Werte des 3PMeanline
	    }
	label.clear();
 
	min = cV2[1]->getMin();
	max = cV2[1]->getMax();

	cV2[1]->setValue(fabs(min+(max-min)*(static_cast<float>(meanlineParam[3]))));	//Überschreiben der Werte der constValues der 3PMeanline mit den Ergebnissen des Algorithmus
	
	std::ofstream ofs1 ("Solution1.xml", std::ofstream::trunc);			//Solution.xml wird geleert
	ofs1.close();

	std::ofstream ofs2 ("Solution2.xml", std::ofstream::trunc);			//Solution.xml wird geleert
	ofs2.close();
	
	parser1.openFileAndWrite("Solution1.xml", cVptr1);
	parser2.openFileAndWrite("Solution2.xml", cVptr2);
	
	cV1.destroy();
	cV2.destroy();
		
    }
    catch (dtOO::eGeneral & eGenRef)
    {
	std::cout << "Exception: " << eGenRef.what() << '\n';
    }

    return 1;
}
  
