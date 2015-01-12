#include "systemHandling.h"

#include <logMe/logMe.h>
#include <cstdlib>
#include <sys/stat.h>
#include <stdio.h>

namespace dtOO { 
  systemHandling::systemHandling() {
  }

  systemHandling::~systemHandling() {
  }
  
  void systemHandling::command( std::string & cmd ) {
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

  void systemHandling::commandAndWait( std::string & cmd ) {
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
      dt__THROW(deleteFile(),
              << "Error deleting " << DTLOGEVAL(filename) );
    }
  }
}
