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

  using TE_Type = psil_parser::token_elem_t::TE_Type;
  using token_ptr = std::unique_ptr<psil_parser::token_t>;

  /**
     Read, Evaluate, Print, Loop
     This function does the evaluation and printing

     @param lang -language to parse input using
     @param input - input to evaluate
   */
  void repl( const std::unique_ptr<psil_parser::language_t> & lang, std::string input );

  // ================== Exec functions ================================================
  
  /**
     Executes the abstract syntax tree given
     if rem is true, then delete that branch
  */
  void exec( token_ptr & ast, bool& rem );

  /**
     Executes the if expression given
     Assumes the node given is the expression token containing the if
     if rem is true, then delete that branch
  */
  void exec_if( token_ptr & node, bool& rem );

  /**
     Executes the cond expression given
     Assumes the node given is the expression token containing the cond
     if rem is true, then delete that branch
  */
  void exec_cond( token_ptr & node, bool& rem );

  /**
     Executes the lambda expression given
     Assume the node given is the expression token containing the lambda
     if rem is true, then delete that branch
  */
  void exec_lambda( token_ptr & node, bool& rem );

  /**
     Executes the definition given
     Assumes the node given is the definition token
     if rem is true, then delete that branch
  */
  void exec_def( token_ptr & node  );
  

  // ==============================================================================

  /**
     Checks to see if a expression evaluates to true
  */
  bool is_true( token_ptr & node );


  /**
     Checks to see if a number is zero (0 or 0.0)
  */
  bool is_zero( token_ptr & node );

  
}
