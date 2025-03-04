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

#ifndef DTTRANSFORMER_H
#define DTTRANSFORMER_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <jsonHeaven/jsonPrimitive.h>
#include <logMe/dtMacros.h>
#include <string>
#include <vector>
#include <xmlHeaven/dtXmlParserBase.h>

class QDomElement;

namespace pugg {
class Kernel;
}

namespace dtOO {
class baseContainer;
class constValue;
class analyticGeometry;
class analyticFunction;

class dtTransformer : public labelHandling, public optionHandling {
public:
  dt__classOnlyName(dtTransformer);
  dtTransformer();
  virtual ~dtTransformer();
  dtTransformer(dtTransformer const &orig);
  dtTransformer(jsonPrimitive const &jE);
  //
  // overload
  //
  virtual dtTransformer *clone(void) const = 0;
  virtual dtTransformer *create(void) const = 0;
  virtual void jInit(
    jsonPrimitive const &jE,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG
  );
  virtual void init(
    ::QDomElement const *tE,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG
  ) = 0;
  virtual std::vector<std::string> factoryAlias(void) const;
  virtual bool isNecessary(void) const = 0;
  //
  // optional overload
  //
  virtual std::vector<dtPoint2> apply(std::vector<dtPoint2> const *const toTrans
  ) const;
  virtual std::vector<dtPoint3> apply(std::vector<dtPoint3> const *const toTrans
  ) const;
  virtual lvH_analyticGeometry apply(lvH_analyticGeometry const *const toTrans
  ) const;
  virtual lvH_analyticFunction apply(lvH_analyticFunction const *const toTrans
  ) const;
  virtual std::vector<dtPoint3>
  retract(std::vector<dtPoint3> const *const toRetract) const;
  virtual std::vector<dtPoint2>
  retract(std::vector<dtPoint2> const *const toRetract) const;
  virtual void handleBool(std::string const name, bool const value);
  virtual void handleInt(std::string const name, dtInt const value);
  virtual void handleFloat(std::string const name, dtReal const value);
  virtual void handleDtPoint2(std::string const name, dtPoint2 const value);
  virtual void handleDtVector3(std::string const name, dtVector3 const value);
  virtual void handleDtVector2(std::string const name, dtVector2 const value);
  virtual void
  handleAnalyticGeometry(std::string const name, analyticGeometry const *value);
  virtual void
  handleAnalyticFunction(std::string const name, analyticFunction const *value);
  //
  //
  //
  void init(
    ::QDomElement const *tE,
    baseContainer *const bC,
    lvH_constValue const *const cValP,
    lvH_analyticFunction const *const sFunP
  );
  dtPoint3 apply(dtPoint3 const &pp) const;
  dtPoint2 apply(dtPoint2 const &pp) const;
  dtPoint3 retract(dtPoint3 const &toRetract) const;
  dtPoint2 retract(dtPoint2 const &toRetract) const;
  dtPoint3 operator()(dtPoint3 const &pp, dtInt const dir = 1) const;
  analyticFunction *apply(analyticFunction const *const sF) const;
  analyticGeometry *apply(analyticGeometry const *const aG) const;
  //
  // pugg stuff
  //
  static const dtInt version = 1;
  static const std::string server_name() { return "dtTransformerServer"; }
  void setKernel(::pugg::Kernel *kernel);

protected:
  jsonPrimitive &config(void);
  jsonPrimitive const &config(void) const;

private:
  jsonPrimitive _config;
  dt__pH(::pugg::Kernel) _kernel;
};
dt__H_addCloneForpVH(dtTransformer);
} // namespace dtOO

#endif /* DTTRANSFORMER_H */
