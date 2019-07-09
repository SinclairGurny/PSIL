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

  // Forward declarations

  struct stack_t;

  // === Typedefs ======================================================================
  
  using TE_Type = psil_parser::token_elem_t::TE_Type;
  using token_ptr = std::unique_ptr<psil_parser::token_t>;
  using stack_ptr = std::unique_ptr<stack_t>;

  enum VarType { BOOL, CHAR, NUM, LIST, PROC, UNKNOWN, ERROR };
  
  // ===================================================================================
  
  /**
     Copies token recursively
  */
  token_ptr copy_tk( const token_ptr & tk );

  /**
     Checks type of token
     Assumes top level token is an expression
  */
  VarType check_type( const token_ptr & tk );
  
  struct stack_elem_t {
    stack_elem_t( std::string n, VarType t, const token_ptr & v ) :
      var_name(n), type(t), value( copy_tk( v ) ), scope_lvl(0) {}
    stack_elem_t( std::string n, VarType t, const token_ptr & v, size_t sl ) :
      var_name(n), type(t), value( copy_tk( v ) ), scope_lvl(sl) {}

    std::string var_name;
    VarType type;
    token_ptr value;
    size_t scope_lvl;
  };
  
  struct stack_t {
    enum ExistsType { NO, GLOBAL, LOCAL };

    stack_t() : current_scope(0) { init(); }
    
    void add( std::string n, const token_ptr & v );
    void init();
    void push();
    void pop();
    ExistsType exists( std::string n );
    token_ptr get( std::string n, ExistsType e );

    size_t current_scope;
    std::map<std::string, std::unique_ptr<stack_elem_t> > global_table;
    std::map<std::string, std::unique_ptr<stack_elem_t> > table;
  };
  
  // ===================================================================================

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
  void exec( stack_ptr & s, token_ptr & ast, bool& rem );

  /**
     Executes the if expression given
     Assumes the node given is the expression token containing the if
     if rem is true, then delete that branch
  */
  void exec_if( stack_ptr & s, token_ptr & node, bool& rem );

  /**
     Executes the cond expression given
     Assumes the node given is the expression token containing the cond
     if rem is true, then delete that branch
  */
  void exec_cond( stack_ptr & s, token_ptr & node, bool& rem );

  /**
     Executes the lambda expression given
     Assume the node given is the expression token containing the lambda
     if rem is true, then delete that branch
  */
  void exec_lambda( stack_ptr & s, token_ptr & node, bool& rem );

  /**
     Executes the application of procedures
     Assumes the node given is the expression token containing the application
     if rem is true, then delete that branch. ie application returned void
  */
  void exec_app( stack_ptr & s, token_ptr & node, bool& rem );

  /**
     Executes the definition given
     Assumes the node given is the definition token
     if rem is true, then delete that branch
  */
  void exec_def( stack_ptr & s, token_ptr & node  );

  /**
     Replaces variables with their value
  */
  void exec_var( stack_ptr & s, token_ptr & node, bool& rem );
  

  // ==============================================================================

  /**
     Checks to see if a expression evaluates to true
  */
  bool is_true( stack_ptr & s, token_ptr & node );


  /**
     Checks to see if a number is zero (0 or 0.0)
  */
  bool is_zero( token_ptr & node );

  
}
