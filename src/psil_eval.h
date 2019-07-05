/** 
    psil_eval.h
    PSIL Semantic Evaluation Library
    @author Sinclair Gurny
    @version 0.1
    July 2019
 */

#pragma once

#include "psil_parser.h"

namespace psil_eval {

  std::unique_ptr<psil_parser::token_t> token_tree_copy( const std::unique_ptr<psil_parser::token_t> & root );
  
  bool check( const std::unique_ptr<psil_parser::token_t> & tk );
  
}

