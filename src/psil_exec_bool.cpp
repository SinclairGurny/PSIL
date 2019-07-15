/**
   psil_exec_bool.cpp
   PSIL Execution Library
   Boolean function implementations
   @author Sinclair Gurny
   @version 0.9
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

  token_ptr make_boolean( bool val ) {
    // Make expression
    auto tmp_exp = std::make_unique<psil_parser::token_t>( "<expression>" );
    // Make constant
    auto tmp_con = std::make_unique<psil_parser::token_t>( "<constant>" );
    // Make number
    auto tmp_boo = std::make_unique<psil_parser::token_t>( "<boolean>" );
    // Add val to integer
    std::string str_val = ( val ) ? "#t" : "#f";
    auto tmp_val = std::make_unique<psil_parser::token_elem_t>( str_val );
    tmp_boo->aspects.push_back( std::move( tmp_val ) );
    // Add boolean to const
    auto elem_boo = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_boo ) );
    tmp_con->aspects.push_back( std::move( elem_boo ) );
    // Add const to expression
    auto elem_con = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_con ) );
    tmp_exp->aspects.push_back( std::move( elem_con ) );
    
    return tmp_exp;
   }

  // ========================= BOOLEAN OPERATIONS ================================================
  
  // Performs logical and on all arguments
  void psil_and( stack_ptr & s, token_ptr & node ) {
    std::cout << "AND" << std::endl;
    size_t idx = 0;
    std::string val = "";
    bool ret = true;
    auto itr = node->aspects.front()->tk->aspects.begin();
    for ( ; itr != node->aspects.front()->tk->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.front()->tk->aspects.size() - 1 ) { // Just arguments of function
	if ( !is_true( s, (*itr)->tk ) ) {
	  ret = false;
	  break;
	}
      }
    }

    auto ret_bool = make_boolean( ret );
    node = std::move( ret_bool );
  }

  // Performs logical or on all arguments
  void psil_or( stack_ptr & s, token_ptr & node ) {
    //std::cout << "OR" << std::endl;
    size_t idx = 0;
    bool ret = false;
    auto itr = node->aspects.front()->tk->aspects.begin();
    for ( ; itr != node->aspects.front()->tk->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.front()->tk->aspects.size() - 1 ) { // Just arguments of function
	if ( is_true( s, (*itr)->tk ) ) {
	  ret = true;
	  break;
	}
      }
    }
    auto ret_bool = make_boolean( ret );
    node = std::move( ret_bool );
  }

  // Performs logical negation on all arguments
  void psil_not( stack_ptr & s, token_ptr & node ) {
    bool ret = is_true( s, node->aspects.front()->tk->aspects[2]->tk );
    auto ret_bool = make_boolean( ret );
    node = std::move( ret_bool );
  }

  // Checks the two arguments for equality
  void psil_is_equal( stack_ptr & s, token_ptr & node ) {
    std::string val;
    auto app = node->aspects.front()->tk.get();
    bool ret = equal_tk( app->aspects[2]->tk, app->aspects[3]->tk );
    auto ret_bool = make_boolean( ret );
    node = std::move( ret_bool );
  }
 
}
