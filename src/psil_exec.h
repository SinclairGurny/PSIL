/** 
    psil_exec.h
    PSIL Execution Library
    @author Sinclair Gurny
    @version 0.1
    July 2019
 */

#pragma once

#include "psil_parser.h"
#include "psil_eval.h"

namespace psil_exec {

  void repl( const std::unique_ptr<psil_parser::language_t> & lang, std::string input );
  
}
