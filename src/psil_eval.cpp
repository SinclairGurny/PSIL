/** 
    psil_eval.cpp
    PSIL Semantic Evaluation Implementation
    @author Sinclair Gurny
    @version 0.9
    July 2019
 */

#include "psil_eval.h"

namespace psil_eval {

  // Check expression for semantic erros
  bool check_expression( psil_parser::token_t * node ) {
    if ( node->aspects.size() == 1 && node->aspects.front()->elem_type == TE_Type::TOKEN ) {
      if ( node->aspects.front()->tk->type_name == "<constant>" ) {
	return true;
      } else if ( node->aspects.front()->tk->type_name == "<variable>" ) {
	return true;
      } else if ( node->aspects.front()->tk->type_name == "<lambda>" ) {
	return check_lambda( node->aspects.front()->tk.get() );
      } else if ( node->aspects.front()->tk->type_name == "<conditional>" ||
		  node->aspects.front()->tk->type_name == "<application>" ) {
	auto tmp = node->aspects.front()->tk.get();
	for ( auto itr = tmp->aspects.begin(); itr != tmp->aspects.end(); ++itr ) {
	  // Recurse on expressions
	  if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	    if ( !check_node( (*itr)->tk.get() ) ) return false;
	  }
	}	
      }
    } else { // (begin <expr>+)
      for ( auto itr = node->aspects.begin(); itr != node->aspects.end(); ++itr ) {
	// Recurse on expressions
	if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	  if ( !check_node( (*itr)->tk.get() ) ) return false;
	}
      }
    }
    return true;
  }
  

  // Check lambda expressions for semantic errors
  bool check_lambda( psil_parser::token_t * node ) {
    // Grab <formals> aspect
    auto lambda_args = node->aspects[2]->tk.get();
    for ( auto itr = lambda_args->aspects.begin(); itr != lambda_args->aspects.end(); ++itr ) {
      // Look for variables in <formals>
      if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	auto var = (*itr)->tk.get();
	if ( !var->aspects.empty() && var->aspects.front()->elem_type ==
	     TE_Type::TOKEN ) {
	  // grab <identifier> aspect
	  auto iden = var->aspects.front()->tk.get();
	  // check for <operator> or <keyword>
	  if ( !iden->aspects.empty() && iden->aspects.front()->elem_type ==
	       TE_Type::TOKEN ) {
	    auto var_type = iden->aspects.front()->tk.get();
	    if ( var_type->type_name == "<operator>" ) {
	      throw std::string( "Cannot bind operator as lambda argument" );
	      return false;
	    } else if ( var_type->type_name == "<keyword>" ) {
	      throw std::string( "Cannot bind keyword as lambda argument" );
	      return false;
	    }
	  }
	}
      }
    }
    // === Recurse on body of lambda ===
    auto body = node->aspects[3]->tk.get();
    for ( auto itr = body->aspects.begin(); itr != body->aspects.end(); ++itr ) {
      if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	if ( !check_node( (*itr)->tk.get() ) ) return false;
      }
    }
    return true;
  }


  // Check definitions for semantic errors
  bool check_definition( psil_parser::token_t * node ) {
    // === Check on variable capturing ===
    // grab <variable> aspect
    auto var = node->aspects[2]->tk.get();
    if ( !var->aspects.empty() && var->aspects.front()->elem_type ==
	 TE_Type::TOKEN ) {
      // grab <identifier> aspect
      auto iden = var->aspects.front()->tk.get();
      // check for <operator> or <keyword>
      if ( !iden->aspects.empty() && iden->aspects.front()->elem_type ==
	   TE_Type::TOKEN ) {
	auto var_type = iden->aspects.front()->tk.get();
	if ( var_type->type_name == "<operator>" ) {
	  throw std::string( "Cannot redefine operators" );
	  return false;
	} else if ( var_type->type_name == "<keyword>" ) {
	  throw std::string( "Cannot define keywords" );
	  return false;
	}
      }
    }
    // === Recurse on rest of definition ===
    return check_node( node->aspects[3]->tk.get() );
  }


  // Perform checks on specific type of nodes
  bool check_node( psil_parser::token_t * node ) {
    if ( node == nullptr ) {
      throw std::string( "Given nullptr as AST" );
    } else if ( node->type_name == "<program>" ) {
      if ( !node->aspects.empty() &&  node->aspects.front()->elem_type == TE_Type::TOKEN )
	return check_node( node->aspects.front()->tk.get() );
    } else if ( node->type_name == "<form>" ) {
      for ( auto itr = node->aspects.begin(); itr != node->aspects.end(); ++itr ) {
	if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	  if ( !check_node( (*itr)->tk.get() ) ) return false;
	}
      }
    } else if ( node->type_name == "<expression>" ) {
      return check_expression( node );
    } else if ( node->type_name == "<definition>" ) {
      return check_definition( node );
    }
    // Do not expand other types of tokens
    return true;
  }

}
