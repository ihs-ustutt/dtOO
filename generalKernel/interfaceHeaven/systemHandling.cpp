#include "systemHandling.h"

#include <logMe/logMe.h>
#include <cstdlib>
#include <sys/stat.h>
#include <stdio.h>
#include <boost/filesystem.hpp>
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
    
    DTINFOWF(command(),
            << DTLOGEVAL(cmd) << LOGDEL
            << DTLOGEVAL(nullStatus) << LOGDEL
            << DTLOGEVAL(status) << LOGDEL
            << DTLOGEVAL(wExitStatus) );
  }

  void systemHandling::commandAndWait( std::string const & cmd ) {
    //
    // call system
    //
    int nullStatus = system(NULL);
    FILE * status = popen( cmd.c_str(), "r" );
//    int wExitStatus = WEXITSTATUS(status);
    dt__THROW_IF(status == 0, commandAndWait());

    const int BUFSIZE = 80;
    char buf[ BUFSIZE ];
    while( fgets( buf, BUFSIZE,  status ) ) {
      DTINFOWF(
				commandAndWait(),
				<< DTLOGEVAL(buf)  
			);
    }
    pclose( status );
    
    DTINFOWF(
			command(),
      << DTLOGEVAL(cmd) << LOGDEL
      << DTLOGEVAL(nullStatus) << LOGDEL
      << DTLOGEVAL(status)
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
    DTINFOWF(createDirectory(),
            << DTLOGEVAL(dirPath) << LOGDEL
            << DTLOGEVAL(status) );
    if (status) {
      return false;
    }
    return true;
  }

  bool systemHandling::fileExists(std::string const filename) {
    if ( FILE *file = fopen(filename.c_str(), "r") ) {
      fclose(file);
      return true;
    } 
    else {
      return false;
    }   
  }
  
  void systemHandling::deleteFile( std::string const filename ) {
    if( remove( filename.c_str() ) != 0 ) {
      dt__THROW(deleteFile(), << "Error deleting " << DTLOGEVAL(filename) );
    }
  }

  void systemHandling::deleteDirectory( std::string const dirname ) {
  try {
		boost::filesystem::remove_all( 
		  boost::filesystem::path(dirname.c_str()) 
		);
  }
	catch( boost::filesystem::filesystem_error const & e) {
      dt__THROW(
				deleteDirectory(), 
				<< "Error deleting " << DTLOGEVAL(e.what()) << LOGDEL
				<< "Error deleting " << DTLOGEVAL(dirname) 
			);

  }		
//    if( status != 0 ) {
//      dt__THROW(
//				deleteDirectory(), 
//				<< "Error deleting " << DTLOGEVAL(status) << LOGDEL
//				<< "Error deleting " << DTLOGEVAL(dirname) 
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
				dt__THROW( 
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
				dt__THROW( 
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
				dt__THROW( 
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
				dt__THROW( copyDirectory(), << DTLOGEVAL(e.what()) );
			}
		}
//		return true;
	}	
}
