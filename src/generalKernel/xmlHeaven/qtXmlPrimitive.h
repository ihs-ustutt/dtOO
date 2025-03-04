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

#ifndef QTXMLPRIMITIVE_H
#define QTXMLPRIMITIVE_H

#include <dtOOTypeDef.h>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>
#include <string>
#include <vector>

namespace dtOO {
class qtXmlPrimitive : public stringPrimitive {
public:
  dt__classOnlyName(qtXmlPrimitive);
  virtual ~qtXmlPrimitive();
  static ::QDomElement
  getSibling(std::string const sibName, ::QDomElement const element);
  static std::vector<::QDomElement>
  getSiblingVector(std::string const sibName, ::QDomElement const element);
  static std::vector<::QDomElement> getSiblingVector(::QDomElement const element
  );
  static std::vector<::QDomElement>
  getChildVector(std::string const sibName, ::QDomElement const element);
  static twoDArrayHandling<::QDomElement>
  getChildMatrix(std::string const sibName, ::QDomElement const element);
  static std::vector<::QDomElement>
  getDescendantVector(std::string const sibName, ::QDomElement const element);
  static std::vector<::QDomElement> getChildVector(::QDomElement const element);
  static ::QDomElement
  getChild(std::string const sibName, ::QDomElement const element);
  static ::QDomElement getChildByName(
    std::string const sibName,
    std::string const name,
    ::QDomElement const element
  );
  static ::QDomElement getChildByAttributeMatch(
    std::string const sibName,
    std::string const attributeName,
    std::string const attributeValue,
    ::QDomElement const element
  );
  static bool hasChildElement(
    std::string const elementTag,
    std::string const labelAttributeVal,
    ::QDomElement const &parentElement
  );
  static ::QDomElement getChildElement(
    std::string const elementTag,
    std::string const labelAttributeVal,
    ::QDomElement const &parentElement
  );
  static void getChildLabels(
    std::string childName,
    std::vector<std::string> *labelValue,
    ::QDomElement const &parentElement
  );
  static dtInt
  getNChildren(std::string const sibName, ::QDomElement const element);
  static bool
  hasSibling(std::string const sibName, ::QDomElement const element);
  static bool hasChild(std::string const sibName, ::QDomElement const element);
  static ::QDomElement
  getNextSibling(std::string const sibName, ::QDomElement const element);
  static bool is(std::string const tagName, ::QDomElement const element);
  static bool isBuilder(::QDomElement const element);
  static bool
  hasAttribute(std::string const attName, ::QDomElement const element);
  static bool
  isAttributeVector(std::string const attName, ::QDomElement const element);
  static dtInt
  getAttributeInt(std::string const attName, ::QDomElement const element);
  static dtReal
  getAttributeFloat(std::string const attName, ::QDomElement const element);
  static std::string
  getAttributeStr(std::string const attName, ::QDomElement const element);
  static std::string getAttributeStr(
    std::string const attName,
    const ::QDomElement element,
    std::string const def
  );
  static std::string
  getAttributeRareStr(std::string const attName, ::QDomElement const element);
  static bool
  getAttributeBool(std::string const attName, const ::QDomElement element);
  static bool getAttributeBool(
    std::string const attName, const ::QDomElement element, bool const &dBool
  );
  static std::vector<bool> getAttributeBoolVector(
    std::string const attName, const ::QDomElement element
  );
  static std::vector<std::string>
  getAttributeStrVector(std::string const attName, ::QDomElement const element);
  static std::vector<std::string> getAttributeStrVector(
    std::string const attName,
    ::QDomElement const element,
    std::vector<std::string> const def
  );
  static std::vector<std::string> getAttributeRareStrVector(
    std::string const attName, ::QDomElement const element
  );
  static std::vector<dtInt>
  getAttributeIntVector(std::string const attName, ::QDomElement const element);
  static std::vector<dtReal> getAttributeFloatVector(
    std::string const attName, ::QDomElement const element
  );
  static std::vector<std::string>
  getAttributeLabelVector(::QDomElement const &element);
  static std::string convertToString(::QDomNode const node);
  static std::string getTagName(::QDomElement const element);
  static ::QDomElement createElement(QDomDocument &doc, std::string const name);
  static void appendChildElement(QDomDocument &doc, ::QDomElement &element);
  static void appendChildElementInElement(
    QDomDocument &doc,
    std::string const elName,
    std::string const elAttribute,
    std::string const elAttributeValue,
    ::QDomElement &element
  );
  static void appendChildElementInElement(
    ::QDomElement &documentElement,
    std::string const elName,
    std::string const elAttribute,
    std::string const elAttributeValue,
    ::QDomElement &element
  );
  static void replaceInAllAttributes(
    std::string const replace,
    std::string const with,
    ::QDomElement *const element
  );
  static void replaceRecursiveInAllAttributes(
    std::string const replace,
    std::string const with,
    ::QDomElement *const element
  );

protected:
  qtXmlPrimitive();
  static std::string _STRSIGN;

private:
  static std::vector<QDomAttr> getAttributeVector(::QDomElement const element);
};
} // namespace dtOO

#endif /* QTXMLPRIMITIVE_H */
