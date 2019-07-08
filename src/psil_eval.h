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

  // redeclare for ease of use
  using upoint = std::pair<size_t, size_t>;
  using TE_Type = psil_parser::token_elem_t::TE_Type;

  /**
     Checks expressions for semantic errors
     Assumes token is expression

     @param - token to check
     @throws - std::string of semantic error
     @returns - bool, true if no errors found
  */
  bool check_expression( psil_parser::token_t * node );

  /**
     Checks lambda expressions for semantic errors
     Assumes token is contains lambda

     @param - token to check
     @throws - std::string of semantic error
     @returns - bool, true if no errors found
  */
  bool check_lambda( psil_parser::token_t * node );

  /**
     Checks definitions for semantic errors
     Assumes token is definition

     @param - token to check
     @throws - std::string of semantic error
     @returns - bool, true if no errors found
  */
  bool check_definition( psil_parser::token_t * node );

  /**
     Checks any type of token for semantic errors

     @param - token to check
     @throws - std::string of semantic error
     @returns - bool, true if no errors found
  */
  bool check_node( psil_parser::token_t * tk );
  
}

