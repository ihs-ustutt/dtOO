#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>
#include <catchMeanline.h>
#include <ffMeanline.h>
#include <constValueHeaven/constValue.h>
#include <interfaceHeaven/vectorHandling.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <vector>

#include <iostream>

using namespace dtOO;

int main(int argc, char **argv)
{
    try
    {
      
    //pointContainer pC;
    //vectorContainer vC;
    std::vector<double> meanlineParam(3);
    //std::vector<std::string > label;
    //vectorHandling<constValue *> cV;
    //vectorHandling<constValue *> * cVptr;
    //cVptr = &cV;
    //dtXmlParser parser1;
    //float max;
    //float min;

    // neuer kommentar

  //Aufruf des Gesamten Algorithmuses//

	meanlineParam = catchMeanline(argc, argv);	//als Rückgabe werden die Individuen in einen Vector geschrieben, wichtig zum Umrechnen in die tatsächlichen Parameter
	
	/*parser1.openFileAndParse("surface.xml");	//constValues der Meanlineparameter in ein Xml-File schreiben. Dafür werden die constValues aus surface.xml eingelesen, und die nur die Werte verändert.

	parser1.getNames("constValue", &label);
	for (int ii = 0;ii<label.size();ii++) {
	    parser1.createConstValue(label[ii], &cV);	//Erzeugen der ursprünglichen cV-Werte des 3PMeanline
	    }
	
	label.clear();
	
	for (int ii = 0; ii<3; ii++){
	  
	    min = cV[ii]->getMin();
	    max = cV[ii]->getMax();

	    cV[ii]->setValue(fabs(min+(max-min)*(static_cast<float>(meanlineParam[ii]))));	//Überschreiben der Werte der constValues der 3PMeanline mit den Ergebnissen des Algorithmus
	  
	  }
	
	//Gibt es eine Möglichkeit die Datei vor aufruf zu löschen um nur die aktuelle Lösung darin abzubilden?	
	parser1.openFileAndWrite("Solution.xml",  cVptr);
	
	*/
	
	std::cout<<"HELLO"<<std::endl;
	
	//cV.destroy();
		
    }
    catch (dtOO::eGeneral & eGenRef)
    {
	std::cout << "Exception: " << eGenRef.what() << '\n';
    }

    return 1;
}
  
