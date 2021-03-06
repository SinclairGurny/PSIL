/** 
    psil_exec.h
    PSIL Execution Library
    @author Sinclair Gurny
    @version 1.0
    July 2019
 */

#pragma once

#include "psil_parser.h"
#include "psil_eval.h"
#include <iterator>
#include <functional>
#include <fstream>
#include <cmath>

namespace psil_exec {

  // Forward declarations

  struct stack_t;
  struct stack_elem_t;

  // ===================================================================================
  // === Typedefs ======================================================================
  // ===================================================================================
  
  // redeclare for ease of use
  using TE_Type = psil_parser::token_elem_t::TE_Type;
  // shorten long types
  using token_ptr = std::unique_ptr<psil_parser::token_t>;
  using stack_ptr = std::unique_ptr<stack_t>;
  using symbol_table_t = std::map<std::string, std::unique_ptr<stack_elem_t> >;

  // types of variables
  enum VarType { BOOL, CHAR, NUM, LIST, PROC, SYMBOL, UNKNOWN, ERROR };
  
  // ===================================================================================
  // ========== Internal Helper Functions ==============================================
  // ===================================================================================
  
  /**
     Copies token recursively
  */
  token_ptr copy_tk( const token_ptr & tk );

  /**
     Compares two tokens recursively
  */
  bool equal_tk( const token_ptr & tk1, const token_ptr & tk2 );

  /**
     Finds token 'what' within token 'where', and replaces occurences with token 'that'
     which is copied to the new location
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
  // ========= Symbol Table Stack ======================================================
  // ===================================================================================
  

  /**
     Represents a single element in symbol table,
     Stores value and some information about the variable
  */
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


  /**
     Represents 
  */
  struct stack_t {
    // Used to represent where a variable is defined
    enum ExistsType { NO, GLOBAL, LOCAL };

    stack_t() { init(); }
    
    void init();
    void push();
    void pop();

    ExistsType exists( std::string n );
    void add( std::string n, const token_ptr & v );
    void update( std::string n, ExistsType e, const token_ptr & v );
    token_ptr get( std::string n, ExistsType e );

    symbol_table_t global_table;
    std::vector< symbol_table_t > table;
  };
  
  // ===================================================================================

  /**
     Read, Evaluate, Print, Loop
     This function does the evaluation and printing
     @param lang - anguage to parse input using
     @param input - input to evaluate
  */
  void repl( const std::unique_ptr<psil_parser::language_t> & lang, std::string input );

  // Perform single read evaluate print cycle for contents of file
  void run_file( const std::unique_ptr<psil_parser::language_t> & lang, std::string filename );

  // ===================================================================================
  // ================== Exec functions =================================================
  // ===================================================================================
  
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

  //   Replaces variables with their value, returns true if global procedure name
  bool exec_var( stack_ptr & s, token_ptr & node, bool& rem );

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

  // ===================================================================================
  // ===================================================================================
  
  // Applies the global functions, given its name fun
  void apply_global_proc( stack_ptr & s, token_ptr & node, bool& rem, std::string fun );
  

  // ===================================================================================
  // ============ Small helper functions ===============================================
  // ===================================================================================

  // Character converstion
  std::string psil_char( std::string ch );
  std::string psil_char( char c );

  // Convert constant to printable format
  std::string tk_to_string( token_ptr & tk );
  
  // Helper functions to make tokens
  token_ptr make_boolean( bool val );
  token_ptr make_character( std::string val );
  token_ptr make_number( std::string val, bool int_or_dec );

  // ===================================================================================
  // ===== Global functions ============================================================
  // ===================================================================================
  
  // Input/Output =======================================
  // Print given token to cout
  void print( token_ptr & node, bool newline );
  // cin and return result as character list
  void psil_read( token_ptr & node );
  // Boolean ============================================
  // Logical and of arguments
  void psil_and( stack_ptr & s, token_ptr & node );
  // Logical or of arguments
  void psil_or( stack_ptr & s, token_ptr & node );
  // Logical not of argument
  void psil_not( stack_ptr & s, token_ptr & node );
  // Checks if arguments have the same value
  void psil_is_equal( stack_ptr & s, token_ptr & node );
  // Math ===============================================
  // Operators
  // Add numbers
  void psil_add( stack_ptr & s, token_ptr & node );
  // Subtract numbers
  void psil_sub( stack_ptr & s, token_ptr & node );
  // Multiply numbers
  void psil_mult( stack_ptr & s, token_ptr & node );
  // Divide numbers
  void psil_div( stack_ptr & s, token_ptr & node );
  // Return arg1 % arg2
  void psil_mod( token_ptr & node );
  // Approx ===========================================
  // Performs generic operation on number
  void psil_round( token_ptr & node, std::function<long double(long double)> op );
  // Inequalities =======================================
  // Compare the numbers given using the operation given
  void psil_num_compare( token_ptr & node, std::function<bool(long double, long double)> comp );
  // Character
  // Compare the characters given using the operation given
  void psil_char_compare( token_ptr & node, std::function<bool(std::string, std::string)> comp );
  // List ===============================================
  // Return length of list
  void psil_length( token_ptr & node );
  // Return the pos element of list
  void psil_get_list( token_ptr & node, long pos );
  // Return the nth element of list
  void psil_get_nth(  token_ptr & node );
  // Update the pos element of a list
  void psil_set_list( token_ptr & node, long pos );
  // Update the nth element of a list
  void psil_set_nth( token_ptr & node );
  // Append datum to end of list
  void psil_append( token_ptr & node, long location );
  // Insert datum to end of list
  void psil_insert( token_ptr & node );
  // Remove datum from list
  void psil_pop( token_ptr & node );
  // Check if the list is null ()
  void psil_is_null( token_ptr & node );
  // Quote
  // Convert psil code into quoted datums
  void psil_quote( stack_ptr & s, token_ptr & node );
  // Convert quoted datum's into runable code
  void psil_unquote( stack_ptr & s, token_ptr & node );
  // Identity predicates ================================
  // Checks if the token is of that type
  void psil_type_check( token_ptr & node, VarType t );
  // Checks if token is integer or decimal
  void psil_num_check( token_ptr & node, bool int_or_dec );  
}

