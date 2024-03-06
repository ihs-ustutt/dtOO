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

#ifndef SYSTEMHANDLING_H
#define	SYSTEMHANDLING_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <string>

namespace dtOO {
  class systemHandling {
    public:
      dt__classOnlyName(systemHandling);
      virtual ~systemHandling();
      static void command(std::string const & cmd);
      static std::string commandAndWait(std::string const & cmd);
      static bool changeDirectory( std::string const & dir );
      static void createDirectory( std::string const & dirPath );
      static bool fileExists(std::string const & filename);
      static bool directoryExists(std::string const & dirname);
      static void deleteFile( std::string const & filename );
      static void renameFile( 
        std::string const & from, std::string const & to 
      );
      static void deleteDirectory( std::string const & dirname );    
      static void copyDirectory( 
        std::string const & from, std::string const & to
      );
      static std::string currentDirectory( void );
      static void unsetEnv(std::string const & envName);
      static std::vector< std::string > directoryList( 
        std::string const & path 
      );
    private:
      systemHandling();    
  };
}

#endif	/* SYSTEMHANDLING_H */
