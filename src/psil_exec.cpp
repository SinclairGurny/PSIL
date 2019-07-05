/** 
    psil_exec.cpp
    PSIL Execution Implementation
    @author Sinclair Gurny
    @version 0.1
    July 2019
*/

#include "psil_exec.h"

void psil_exec::repl( const std::unique_ptr<psil_parser::language_t> & lang, std::string input ) {

  auto ast = psil_parser::parse( lang, input );
  if ( ast ) {
    ast->print();

    // eval

    // exec
  } else {
    std::cerr << "Error while parsing input" << std::endl;
  }
}
  
