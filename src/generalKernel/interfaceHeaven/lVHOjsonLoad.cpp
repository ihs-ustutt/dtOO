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

#include "lVHOjsonLoad.h"

#include <constValueHeaven/constValue.h>
#include <fstream>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <interfaceHeaven/systemHandling.h>
#include <jsonHeaven/jsonPrimitive.h>
#include <logMe/logMe.h>

namespace dtOO {
lVHOjsonLoad::lVHOjsonLoad(void) : lVHOInterface(jsonPrimitive()) {}

lVHOjsonLoad::lVHOjsonLoad(jsonPrimitive const &config, lVHOSubject *subj)
  : lVHOInterface(config, subj)
{
  if (systemHandling::fileExists(this->config().lookup<std::string>("_filename")
      ))
  {
    // create file stream
    std::fstream fStr = std::fstream(
      this->config().lookup<std::string>("_filename"), std::ios::in
    );

    // search state in jsonPrimitive
    dt__forAllRefAuto(
      jsonPrimitive(fStr).lookupDef<std::vector<jsonPrimitive>>(
        "state", std::vector<jsonPrimitive>()
      ),
      aState
    )
    {
      if (aState.lookup<std::string>("label") ==
          this->config().lookup<std::string>("_label"))
      {
        _cV.reset(new lvH_constValue(
          jsonPrimitive(aState).lookup<std::vector<constValue *>>("")
        ));
      }
    }
  }
  dt__warnIfWithMessage(
    !_cV, lVHOjsonLoad(), << "_cV is empty. Nothing will be read."
  );
}

lVHOjsonLoad::~lVHOjsonLoad()
{
  if (_cV)
    _cV->destroy();
}

void lVHOjsonLoad::observeSet(labelHandling *lH)
{
  if (!_cV)
    return;

  if (constValue::MustDownCast(lH)->loadable())
  {
    dt__info(
      observeSet(),
      << "Load constValue[ " << lH->getLabel() << " ]: set from "
      << constValue::MustDownCast(lH)->getValue() << " to "
      << _cV->get(lH->getLabel())->getValue()
    );

    constValue::MustDownCast(lH)->setValue(_cV->get(lH->getLabel())->getValue()
    );
    constValue::MustDownCast(lH)->setState(
      this->config().lookup<std::string>("_label")
    );
  }
  else
  {
    dt__info(
      observeSet(), << "constValue[ " << lH->getLabel() << " ] not loadable"
    );
  }
}
} // namespace dtOO
