/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >

    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef DTXMLPARSER_H
#define DTXMLPARSER_H

#include <dtOOTypeDef.h>

#include "dtXmlParserBase.h"
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>
#include <string>
#include <vector>

namespace dtOO {
class analyticGeometry;
class analyticFunction;
class constValue;
class baseContainer;
class boundedVolume;
class dtPlugin;
class dtCase;

class dtXmlParser : public dtXmlParserBase {
public:
  dt__classOnlyName(dtXmlParser);
  virtual ~dtXmlParser();
  static dtXmlParser *
  init(std::string const &inFile, std::string const &outFile);
  static dtXmlParser *reinit(void);
  static dtXmlParser *ptr(void);
  static dtXmlParser const &constReference(void);
  static dtXmlParser &reference(void);
  void parse(void);
  void write(lvH_constValue const *const cValP) const;
  void
  write(std::string const stateName, lvH_constValue const *const cValP) const;
  void write(
    std::vector<std::string> const stateName,
    std::vector<lvH_constValue> const &cValP
  ) const;
  void write(lvH_constValue const &cValP) const;
  void write(std::string const stateName, lvH_constValue const &cValP) const;
  void
  writeUpdate(std::string const stateName, lvH_constValue const &cValP) const;
  void extract(
    std::string const stateName,
    lvH_constValue const &cValP,
    std::string const fileName
  ) const;
  void extractAll(lvH_constValue &cValP);
  void remove(std::string const stateName) const;
  void load(void) const;
  std::vector<std::string> getStates(void) const;
  bool hasState(std::string const stateName) const;
  void loadStateToConst(std::string const stateName, lvH_constValue &cValRef);
  std::string currentState(void) const;
  void setState(std::string const &newState) const;
  void freeState(void) const;
  bool stateLoaded(void) const;
  void getLabels(
    std::string toLookFor, std::vector<std::string> *machinePartNames
  ) const;
  std::vector<std::string> getLabels(std::string lookType) const;
  void getLabel(std::string lookType, std::string *name) const;
  ::QDomElement
  getElement(std::string const lookType, std::string const lookName) const;
  ::QDomElement getElement(std::string const lookType) const;
  ::QDomElement getUnlabeledElement(std::string const lookType) const;
  std::vector<::QDomElement> getElementRecursive(std::string const lookType
  ) const;
  void createConstValue(lvH_constValue *cValP) const;
  void createConstValue(lvH_constValue &cValRef) const;
  void createAnalyticFunction(
    std::string const functionName,
    baseContainer *const bC,
    lvH_constValue const *const cVP,
    lvH_analyticFunction *aFP
  ) const;
  void createAnalyticFunction(
    baseContainer *const bC,
    lvH_constValue const *const cVP,
    lvH_analyticFunction *aFP
  ) const;
  void createAnalyticGeometry(
    std::string const label,
    baseContainer *const bC,
    lvH_constValue const *const cVP,
    lvH_analyticFunction const *const sFP,
    lvH_analyticGeometry *aGP
  ) const;
  void createAnalyticGeometry(
    baseContainer *const bC,
    lvH_constValue const *const cVP,
    lvH_analyticFunction const *const sFP,
    lvH_analyticGeometry *aGP
  ) const;
  void createBoundedVolume(
    std::string const label,
    baseContainer *const bC,
    lvH_constValue const *const cVP,
    lvH_analyticFunction const *const sFP,
    lvH_analyticGeometry const *const aGP,
    lvH_boundedVolume *bVP
  ) const;
  void createBoundedVolume(
    baseContainer *const bC,
    lvH_constValue const *const cVP,
    lvH_analyticFunction const *const sFP,
    lvH_analyticGeometry const *const aGP,
    lvH_boundedVolume *bVP
  ) const;
  void createPlugin(
    std::string const label,
    baseContainer *const bC,
    lvH_constValue const *const cVP,
    lvH_analyticFunction const *const sFP,
    lvH_analyticGeometry const *const aGP,
    lvH_boundedVolume *bVP,
    lvH_dtCase const *const dCP,
    lvH_dtPlugin *pLP
  ) const;
  void createPlugin(
    baseContainer *const bC,
    lvH_constValue const *const cVP,
    lvH_analyticFunction const *const sFP,
    lvH_analyticGeometry const *const aGP,
    lvH_boundedVolume *bVP,
    lvH_dtCase const *const dCP,
    lvH_dtPlugin *pLP
  ) const;
  void createCase(
    std::string const label,
    baseContainer *const bC,
    lvH_constValue const *const cVP,
    lvH_analyticFunction const *const sFP,
    lvH_analyticGeometry const *const aGP,
    lvH_boundedVolume *bVP,
    lvH_dtCase *dCP
  ) const;
  void createCase(
    baseContainer *const bC,
    lvH_constValue const *const cVP,
    lvH_analyticFunction const *const sFP,
    lvH_analyticGeometry const *const aGP,
    lvH_boundedVolume *bVP,
    lvH_dtCase *dCP
  ) const;
  void destroyAndCreate(
    baseContainer &bC,
    lvH_constValue &cV,
    lvH_analyticFunction &aF,
    lvH_analyticGeometry &aG,
    lvH_boundedVolume &bV,
    lvH_dtCase &dC,
    lvH_dtPlugin &pL
  ) const;
  void dump(void) const;

private:
  dtXmlParser(std::string const &inFile, std::string const &outFile);
  void parseFile(char const *const fileName);
  void checkFile(char const *const fileName, QDomDocument &xmlDocument) const;
  static void writeFile(std::string const &fileName, QDomDocument &xmlDocument);
  void setStaticProperties(void);
  //      void createConstValue(
  //        std::string const constValueLabel, cVPtrVec * cValP
  //      ) const;
private:
  std::vector<::QDomElement> _rootRead;
  std::vector<::QDomDocument> _rootReadDoc;
  mutable ::QDomElement _rootLoad;
  mutable ::QDomDocument _rootLoadDoc;
  mutable std::string _currentState;
  std::string const _inFile;
  std::string const _outFile;
  static dtXmlParser *_pH;
};
} // namespace dtOO
#endif /* DTXMLPARSER_H */
