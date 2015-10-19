#ifndef dtParMacros_H
#define	dtParMacros_H

#include <interfaceHeaven/staticPropertiesHandler.h>

#define dt__onlyMaster \
  if (dtOO::staticPropertiesHandler::getInstance()->thisRank() == 0)

#endif	/* dtParMacros_H */

