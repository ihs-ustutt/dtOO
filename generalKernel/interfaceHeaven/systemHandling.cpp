#include "systemHandling.h"

#include <logMe/logMe.h>
#include <cstdlib>
#include <sys/stat.h>
#include <stdio.h>
#include <boost/foreach.hpp>
#include <iostream> 

namespace dtOO { 
  systemHandling::systemHandling() {
  }

  systemHandling::~systemHandling() {
  }
  
  void systemHandling::command( std::string const & cmd ) {
    //
    // call system
    //
    int nullStatus = system(NULL);
    int status = system( cmd.c_str() );
    int wExitStatus = WEXITSTATUS(status);
    
    dt__info(command(),
            << dt__eval(cmd) << std::endl
            << dt__eval(nullStatus) << std::endl
            << dt__eval(status) << std::endl
            << dt__eval(wExitStatus) );
  }

  void systemHandling::commandAndWait( std::string const & cmd ) {
    //
    // call system
    //
    int nullStatus = system(NULL);
    FILE * status = popen( cmd.c_str(), "r" );
//    int wExitStatus = WEXITSTATUS(status);
    dt__throwIf(status == 0, commandAndWait());

    const int BUFSIZE = 80;
    char buf[ BUFSIZE ];
    while( fgets( buf, BUFSIZE,  status ) ) {
      dt__info(
				commandAndWait(),
				<< dt__eval(buf)  
			);
    }
    pclose( status );
    
    dt__info(
			command(),
      << dt__eval(cmd) << std::endl
      << dt__eval(nullStatus) << std::endl
      << dt__eval(status)
		);
  }
  
  bool systemHandling::createDirectory(std::string const & dirPath) {
    //
    // create directory
    //
    int status = mkdir(dirPath.c_str(), S_IRWXU);

    //
    // output
    //    
    dt__info(createDirectory(),
            << dt__eval(dirPath) << std::endl
            << dt__eval(status) );
    if (status) {
      return false;
    }
    return true;
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
//    if( status != 0 ) {
//      dt__THROW(
//				deleteDirectory(), 
//				<< "Error deleting " << dt__eval(status) << std::endl
//				<< "Error deleting " << dt__eval(dirname) 
//			);
//    }
  }	
	
	void systemHandling::copyDirectory(
		std::string const & from, std::string const & to
	) {
		boost::filesystem::path source(from);
		boost::filesystem::path destination(to);
		namespace fs = boost::filesystem;
		try
		{
			// Check whether the function call is valid
			if(
					!fs::exists(source) ||
					!fs::is_directory(source)
			)
			{
	//				std::cerr << "Source directory " << source.string()
	//						<< " does not exist or is not a directory." << '\n'
	//				;
				dt__throw( 
					copyDirectory(), 
					<< "Source directory " << source.string() 
					<< " does not exist or is not a directory."
				);							
//					return false;
			}
			if(fs::exists(destination))
			{
	//				std::cerr << "Destination directory " << destination.string()
	//						<< " already exists." << '\n'
	//				;
				dt__throw( 
					copyDirectory(), 
					<< "Destination directory " << destination.string() << " already exists."
				);				
//					return false;
			}
			// Create the destination directory
			if(!fs::create_directory(destination))
			{
	//			std::cerr << "Unable to create destination directory"
	//					<< destination.string() << '\n'
	//			;
				dt__throw( 
					copyDirectory(), 
					<< "Unable to create destination directory" << destination.string() 
				);			
//				return false;
			}
		}
		catch(fs::filesystem_error const & e)
		{
			std::cerr << e.what() << '\n';
//			return false;
		}
		// Iterate through the source directory
		for(
			fs::directory_iterator file(source);
			file != fs::directory_iterator(); ++file
		)
		{
			try
			{
				fs::path current(file->path());
				if(fs::is_directory(current))
				{
					// Found directory: Recursion
//					if(
							copyDirectory(
								current.string(),
								(destination / current.filename()).string()
							);
//					)
//					{
//							return false;
//					}
				}
				else
				{
//
// produces compiler bug:
// see: https://svn.boost.org/trac/boost/ticket/6124
//
					// Found file: Copy
//					fs::copy_file(
//						current,
//						destination / current.filename()
//					);
					command(
						"cp "+current.string()+" "+(destination / current.filename()).string()
					);
				}
			}
			catch(fs::filesystem_error const & e)
			{
				//std:: cerr << e.what() << '\n';
				dt__throw( copyDirectory(), << dt__eval(e.what()) );
			}
		}
//		return true;
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
  
  std::vector< fpath > systemHandling::directoryList( 
    std::string const & path
  ) {
    ::boost::filesystem::path targetDir(path);

    dt__info( directoryList(), << dt__eval(path) );
    ::boost::filesystem::directory_iterator iter(targetDir), eod;

    std::vector< fpath > dL;
    BOOST_FOREACH(
      ::boost::filesystem::path const & i, std::make_pair(iter, eod)
    ) {
      if ( ::boost::filesystem::is_directory(i) ) {
//        dt__quickdebug( << "<directory> " << dt__eval(i.string()));
        dL.push_back( i.string() );
      }
//      else {
//        dt__quickdebug( << dt__eval(i.string()) );
//      }
    }    
    
    return dL;
  }
}
