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

/**************************************************************************\ 
 **                                                              2002      **
 **                                                                        **
 ** Description:  COVISE VeloIHS     New application module               **
 **                                                                        **
 **                                                                        **
 **                                                                        **
 ** Author:  M. Becker                                                     **
 **                                                                        **
 **                                                                        **
 ** Date:  14.10.04  V1.0                                                  **
\**************************************************************************/

#include <util/coviseCompat.h>
#include <api/coSimpleModule.h>
using namespace covise;

class relabs : public coSimpleModule
{

   private:

      virtual int compute(const char *port);

      // parameters

      enum RotAxisChoice
      {
         RotX = 0x00,
         RotY = 0x01,
         RotZ = 0x02
      };

      coChoiceParam *p_rotaxis;
      char *s_rotaxis[3];

      enum DirectionChoice
      {
         Abs2Rel = 0x00,
         Rel2Abs = 0x01
      };

      coChoiceParam *p_direction;
      char *s_direction[2];

      coFloatParam *p_revolutions;

      // ports
      coInputPort *p_grid;
      coInputPort *p_velo_in;

      coOutputPort *p_velo_out;

      // private data

   public:

      relabs(int argc, char *argv[]);

};
