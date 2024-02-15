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

#include "systemHandling.h"

#include <logMe/logMe.h>
#include <logMe/logContainer.h>
#include <cstdlib>
#include <sys/stat.h>
#include <stdio.h>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iostream> 
#include <errno.h>

namespace dtOO { 
  systemHandling::systemHandling() {
  }

  systemHandling::~systemHandling() {
  }
  
  void systemHandling::command( std::string const & cmd ) {
    //
    // call system
    //
    dtInt nullStatus = system(NULL);
    dtInt status = system( cmd.c_str() );
    dtInt wExitStatus = WEXITSTATUS(status);
    
    dt__debug(
      command(),
      << dt__eval(cmd) << std::endl
      << dt__eval(nullStatus) << std::endl
      << dt__eval(status) << std::endl
      << dt__eval(wExitStatus) 
    );
  }

  std::string systemHandling::commandAndWait( std::string const & cmd ) {
    //
    // call system
    //
    dtInt nullStatus = system(NULL);
    FILE * status = popen( cmd.c_str(), "r" );
    dt__throwIf(status == 0, commandAndWait());

    logContainer< systemHandling > logC(logINFO, "commandAndWait()");
    const dtInt BUFSIZE = 80;
    char buf[ BUFSIZE ];
    while( fgets( buf, BUFSIZE,  status ) ) {
      logC() << buf;
    }
    pclose( status );

    //
    // return only output of command
    //    
    std::string retStr = logC().str();

    logC()       
      << std::endl
      << dt__eval(cmd) << std::endl
      << dt__eval(nullStatus) << std::endl
      << dt__eval(status);
    
    return retStr;
  }
  
  bool systemHandling::changeDirectory( std::string const & dirPath ) {
    //
    // change directory
    //
    dtInt status = chdir(dirPath.c_str());

    //
    // output
    //    
    dt__debug(
      changeDirectory(),
      << dt__eval(dirPath) << std::endl
      << dt__eval(status) 
    );
    
    if (status) return false;
    
    return true;
  }
  
  void systemHandling::createDirectory(std::string const & dirPath) {
    //
    // create directory
    //
    dtInt status = mkdir(dirPath.c_str(), S_IRWXU);

    //
    // output
    //    
    dt__debug(
      createDirectory(),
      << dt__eval(dirPath) << std::endl
      << dt__eval(status) 
    );
    dt__throwIfWithMessage(
      status!=0,
      createDirectory(),
      << "Cannot create directory " << dirPath << std::endl
      << "errno = " << static_cast<int>(errno) 
    );
  }

  bool systemHandling::fileExists(std::string const & filename) {
    if ( FILE *file = fopen(filename.c_str(), "r") ) {
      fclose(file);
      return true;
    } 
    else {
      return false;
    }   
  }
  
  bool systemHandling::directoryExists(std::string const & dirname) {
    return ::boost::filesystem::is_directory(dirname);  
  }
  
  void systemHandling::deleteFile( std::string const & filename ) {
    if( remove( filename.c_str() ) != 0 ) {
      dt__throw(deleteFile(), << "Error deleting " << dt__eval(filename) );
    }
  }

  void systemHandling::renameFile( 
    std::string const & from, std::string const & to 
  ) {
    ::boost::filesystem::rename( from.c_str(), to.c_str() );
  }
  
  void systemHandling::deleteDirectory( std::string const & dirname ) {
    try {
  		boost::filesystem::remove_all( 
  		  boost::filesystem::path(dirname.c_str()) 
  		);
    }
  	catch( boost::filesystem::filesystem_error const & e) {
      dt__throw(
  			deleteDirectory(), 
  			<< "Error deleting " << dt__eval(e.what()) << std::endl
  			<< "Error deleting " << dt__eval(dirname) 
  		);
    }		
  }	
	
	void systemHandling::copyDirectory(
		std::string const & from, std::string const & to
	) {
		boost::filesystem::path source(from);
		boost::filesystem::path destination(to);
		namespace fs = boost::filesystem;
		try {
			// Check whether the function call is valid
			if(
					!fs::exists(source) ||
					!fs::is_directory(source)
			) 	{
				dt__throw( 
					copyDirectory(), 
					<< "Source directory " << source.string() 
					<< " does not exist or is not a directory."
				);							
			}
			if(fs::exists(destination)) {
				dt__throw( 
					copyDirectory(), 
					<< "Destination directory " << destination.string() << " already exists."
				);				
			}
			// Create the destination directory
			if(!fs::create_directory(destination)) {
				dt__throw( 
					copyDirectory(), 
					<< "Unable to create destination directory" << destination.string() 
				);			
			}
		}
		catch(fs::filesystem_error const & e) {
			dt__throw( copyDirectory(), << dt__eval(e.what()) );
		}
		// Iterate through the source directory
		for(
			fs::directory_iterator file(source);
			file != fs::directory_iterator(); ++file
		) {
			try {
				fs::path current(file->path());
				if(fs::is_directory(current))	{
					// Found directory: Recursion
						copyDirectory(
							current.string(),
							(destination / current.filename()).string()
						);
				}
				else {
          //
          // produces compiler bug:
          // see: https://svn.boost.org/trac/boost/ticket/6124
          //
          // Found file: Copy
          //fs::copy_file(
          //	current,
          //	destination / current.filename()
          //);
					command(
						"cp "
            +
            current.string()
            +
            " "
            +
            (destination / current.filename()).string()
					);
				}
			}
			catch(fs::filesystem_error const & e)	{
				dt__throw( copyDirectory(), << dt__eval(e.what()) );
			}
		}
	}	
  
  std::string systemHandling::currentDirectory( void ) {
    return ::boost::filesystem::path( 
      ::boost::filesystem::current_path() 
    ).string();    
  }
  
  void systemHandling::unsetEnv(std::string const & envName) {
    if (unsetenv(envName.c_str()) < 0) {
      dt__throw(
        unsetEnv(), 
        << "Cannot unset environment variable " << envName
      );
    }
  }
  
  std::vector< std::string > systemHandling::directoryList( 
    std::string const & path
  ) {
    ::boost::filesystem::path targetDir(path);

    dt__info( directoryList(), << dt__eval(path) );
    ::boost::filesystem::directory_iterator iter(targetDir), eod;

    std::vector< std::string > dL;
    BOOST_FOREACH(
      ::boost::filesystem::path const & i, std::make_pair(iter, eod)
    ) {
      if ( ::boost::filesystem::is_directory(i) ) {
        dL.push_back( i.string() );
      }
    }    
    
    return dL;
  }
}
