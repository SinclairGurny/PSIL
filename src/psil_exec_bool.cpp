/**
   psil_exec_bool.cpp
   PSIL Execution Library
   Boolean function implementations
   @author Sinclair Gurny
   @version 0.5
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {


  // ========================= BOOLEAN OPERATIONS ================================================
  
  // Performs logical and on all arguments
  void psil_and( stack_ptr & s, token_ptr & node ) {
    std::cout << "AND" << std::endl;
    size_t idx = 0;
    std::string val = "";
    auto itr = node->aspects.front()->tk->aspects.begin();
    for ( ; itr != node->aspects.front()->tk->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.front()->tk->aspects.size() - 1 ) { // Just arguments of function
	if ( !is_true( s, (*itr)->tk ) ) {
	  val = "#f";
	  break;
	}
      }
    }
    if ( val.empty() ) { val = "#t"; }
    std::cout << "AND: " << val << std::endl;
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // create boolean token
    auto tmp_bool = std::make_unique<psil_parser::token_t>("<boolean>");
    // add value
    tmp_bool->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
    // add boolean token to expression token
    auto tmp_elem = std::make_unique<psil_parser::token_elem_t>(std::move(tmp_bool));
    node->aspects.front()->tk->aspects.push_back( std::move( tmp_elem ) );
  }

  // Performs logical or on all arguments
  void psil_or( stack_ptr & s, token_ptr & node ) {
    //std::cout << "OR" << std::endl;
    size_t idx = 0;
    std::string val = "";
    auto itr = node->aspects.front()->tk->aspects.begin();
    for ( ; itr != node->aspects.front()->tk->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.front()->tk->aspects.size() - 1 ) { // Just arguments of function
	if ( is_true( s, (*itr)->tk ) ) {
	  val = "#t";
	  break;
	}
      }
    }
    if ( val.empty() ) { val = "#f"; }
    //std::cout << "OR: " << val << std::endl;
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // create boolean token
    auto tmp_bool = std::make_unique<psil_parser::token_t>("<boolean>");
    // add value
    tmp_bool->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
    // add boolean token to expression token
    auto tmp_elem = std::make_unique<psil_parser::token_elem_t>(std::move(tmp_bool));
    node->aspects.front()->tk->aspects.push_back( std::move( tmp_elem ) );
  }

  // Performs logical negation on all arguments
  void psil_not( stack_ptr & s, token_ptr & node ) {
    std::string val;
    if ( is_true( s, node->aspects.front()->tk->aspects[2]->tk ) ) {
      val = "#f";
    } else {
      val = "#t";
    }
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // create boolean token
    auto tmp_bool = std::make_unique<psil_parser::token_t>("<boolean>");
    // add value
    tmp_bool->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
    // add boolean token to expression token
    auto tmp_elem = std::make_unique<psil_parser::token_elem_t>(std::move(tmp_bool));
    node->aspects.front()->tk->aspects.push_back( std::move( tmp_elem ) );
  }

  // Checks the two arguments for equality
  void psil_is_equal( stack_ptr & s, token_ptr & node ) {
    std::string val;
    auto app = node->aspects.front()->tk.get();
    if ( equal_tk( app->aspects[2]->tk, app->aspects[3]->tk ) ) {
      val = "#t";
    } else {
      val = "#f";
    }
    
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // create boolean token
    auto tmp_bool = std::make_unique<psil_parser::token_t>("<boolean>");
    // add value
    tmp_bool->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
    // add boolean token to expression token
    auto tmp_elem = std::make_unique<psil_parser::token_elem_t>( std::move(tmp_bool) );
    node->aspects.front()->tk->aspects.push_back( std::move( tmp_elem ) );
  }
 


}
