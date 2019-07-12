/** 
    psil_exec.h
    PSIL Execution Library
    @author Sinclair Gurny
    @version 0.5
    July 2019
 */

#pragma once

#include "psil_parser.h"
#include "psil_eval.h"
#include <iterator>
#include <functional>

namespace psil_exec {

  // Forward declarations

  struct stack_t;

  // === Typedefs ======================================================================
  
  using TE_Type = psil_parser::token_elem_t::TE_Type;
  using token_ptr = std::unique_ptr<psil_parser::token_t>;
  using stack_ptr = std::unique_ptr<stack_t>;

  enum VarType { BOOL, CHAR, NUM, LIST, PROC, UNKNOWN, ERROR };
  
  // ===================================================================================
  // Internal Helper Functions
  
  /**
     Copies token recursively
  */
  token_ptr copy_tk( const token_ptr & tk );

  /**
     Compares two tokens recursively
  */
  bool equal_tk( const token_ptr & tk1, const token_ptr & tk2 );

  /**
     Finds token what within token where, and replaces occurence with token that
     that is moved to the new location
  */
  bool find_replace( token_ptr & where, const token_ptr & what, token_ptr & that );
  
  /**
     Checks type of token
     Assumes top level token is an expression
  */
  VarType check_type( const token_ptr & tk );

  
  /**
     Checks to see if a expression evaluates to true
  */
  bool is_true( stack_ptr & s, token_ptr & node );


  /**
     Checks to see if a number is zero (0 or 0.0)
  */
  bool is_zero( token_ptr & node );

  // ===================================================================================
  
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
    void update( std::string n, ExistsType e, const token_ptr & v );

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
     Executes the definition given
     Assumes the node given is the definition token
     if rem is true, then delete that branch
  */
  void exec_def( stack_ptr & s, token_ptr & node  );

  /**
     Replaces variables with their value
  */
  void exec_var( stack_ptr & s, token_ptr & node, bool& rem );

    /**
     Executes the application of procedures
     Assumes the node given is the expression token containing the application
     if rem is true, then delete that branch. ie application returned void
  */
  void exec_app( stack_ptr & s, token_ptr & node, bool& rem );

  
  /**
     Executes the lambda expression given
     Assume the node given is the expression token containing the lambda
     if rem is true, then delete that branch
  */
  void apply_lambda( stack_ptr & s, token_ptr & node, bool& rem );


  // ==============================================================================


  /**
     Applies the global functions
  */
  void apply_global_proc( stack_ptr & s, token_ptr & node, bool& rem, std::string fun );
  

  // ==============================================================================


  // =============================================================================
  // ===== Global functions ======================================================

  // Input/Output
  // Print given token to cout
  void print( token_ptr & node, bool newline );
  // cin and return result as character list
  void psil_read( token_ptr & node );
  // Boolean
  // Logical and of arguments
  void psil_and( stack_ptr & s, token_ptr & node );
  // Logical or of arguments
  void psil_or( stack_ptr & s, token_ptr & node );
  // Logical not of argument
  void psil_not( stack_ptr & s, token_ptr & node );
  // Checks if arguments have the same value
  void psil_is_equal( stack_ptr & s, token_ptr & node );
  // Math
  // Operators
  // Add numbers
  void psil_add( stack_ptr & s, token_ptr & node );
  // Subtract numbers
  void psil_sub( stack_ptr & s, token_ptr & node );
  // Multiply numbers
  void psil_mult( stack_ptr & s, token_ptr & node );
  // Divide numbers
  void psil_div( stack_ptr & s, token_ptr & node );
  // Take absolute value of a number
  void psil_abs( stack_ptr & s, token_ptr & node );
  // Return arg1 % arg2
  void psil_mod( stack_ptr & s, token_ptr & node );
  // Approx
  // Round the number down to nearest integer
  void psil_floor( stack_ptr & s, token_ptr & node );
  // Round the number up to nearest integer
  void psil_ceil( stack_ptr & s, token_ptr & node );
  // Truncate the decimal to convert to integer
  void psil_trunc( stack_ptr & s, token_ptr & node );
  // Round the number to the nearest integer
  void psil_round( stack_ptr & s, token_ptr & node );
  // Inequalities
  // Compare the numbers given using the operation given
  void psil_num_compare( stack_ptr & s, token_ptr & node );
  // Check if the number is equal to zero
  void psil_is_zero( stack_ptr & s, token_ptr & node );
  // Character
  // Compare the characters given using the operation given
  void psil_char_compare( stack_ptr & s, token_ptr & node );
  // List
  // Return first element of list
  void psil_first( stack_ptr & s, token_ptr & node );
  // Return second element of list
  void psil_second( stack_ptr & s, token_ptr & node );
  // Return nth element of list
  void psil_nth( stack_ptr & s, token_ptr & node );
  // Update the first element of a list
  void psil_set_first( stack_ptr & s, token_ptr & node );
  // Update the second element of a list
  void psil_set_second( stack_ptr & s, token_ptr & node );
  // Update the nth element of a list
  void psil_set_nth( stack_ptr & s, token_ptr & node );
  // Check if the list is null ()
  void psil_is_null( stack_ptr & s, token_ptr & node );
  // Quote
  void psil_quote( stack_ptr & s, token_ptr & node );
  void psil_unquote( stack_ptr & s, token_ptr & node );
  // Identity predicates
  // Checks if the token is of that type
  void psil_isbool( stack_ptr & s, token_ptr & node );
  void psil_isnum( stack_ptr & s, token_ptr & node );
  void psil_ischar( stack_ptr & s, token_ptr & node );
  void psil_issymbol( stack_ptr & s, token_ptr & node );
  void psil_isproc( stack_ptr & s, token_ptr & node );
  void psil_islist( stack_ptr & s, token_ptr & node );
  
}

