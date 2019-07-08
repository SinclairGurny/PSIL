/** 
    psil_eval.cpp
    PSIL Semantic Evaluation Implementation
    @author Sinclair Gurny
    @version 0.1
    July 2019
 */

#include "psil_eval.h"

// Check expression for semantic erros
bool psil_eval::check_expression( psil_parser::token_t * node ) {
  // Look for lambda expressions
  if ( node->aspects.size() == 5 && node->aspects[1]->elem_type == psil_eval::TE_Type::STRING
       && node->aspects[1]->str == "lambda" ) {
    return psil_eval::check_lambda( node );
  }
  // Look for application
  else if ( node->aspects.size() == 1 && node->aspects.front()->elem_type == psil_eval::TE_Type::TOKEN
	    && node->aspects.front()->tk->type_name == "<application>" ) {
    auto app = node->aspects.front()->tk.get();
    for ( auto itr = app->aspects.begin(); itr != app->aspects.end(); ++itr ) {
      // Recurse on expressions
      if ( (*itr)->elem_type == psil_eval::TE_Type::TOKEN ) {
	if ( !check_node( (*itr)->tk.get() ) ) return false;
      }
    }
  }
  // Any other type of expression
  else {
    for ( auto itr = node->aspects.begin(); itr != node->aspects.end(); ++itr ) {
      // Recurse on expressions
      if ( (*itr)->elem_type == psil_eval::TE_Type::TOKEN ) {
	if ( !check_node( (*itr)->tk.get() ) ) return false;
      }
    }
  }
  return true;
}
  

// Check lambda expressions for semantic errors
bool psil_eval::check_lambda( psil_parser::token_t * node ) {
  // Grab <formals> aspect
  auto lambda_args = node->aspects[2]->tk.get();
  for ( auto itr = lambda_args->aspects.begin(); itr != lambda_args->aspects.end(); ++itr ) {
    // Look for variables in <formals>
    if ( (*itr)->elem_type == psil_eval::TE_Type::TOKEN ) {
      auto var = (*itr)->tk.get();
      if ( !var->aspects.empty() && var->aspects.front()->elem_type ==
	   psil_eval::TE_Type::TOKEN ) {
	// grab <identifier> aspect
	auto iden = var->aspects.front()->tk.get();
	// check for <operator> or <keyword>
	if ( !iden->aspects.empty() && iden->aspects.front()->elem_type ==
	     psil_eval::TE_Type::TOKEN ) {
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
    if ( (*itr)->elem_type == psil_eval::TE_Type::TOKEN ) {
      if ( !check_node( (*itr)->tk.get() ) ) return false;
    }
  }
  return true;
}


// Check definitions for semantic errors
bool psil_eval::check_definition( psil_parser::token_t * node ) {
  // === Check on variable capturing ===
  // grab <variable> aspect
  auto var = node->aspects[2]->tk.get();
  if ( !var->aspects.empty() && var->aspects.front()->elem_type ==
       psil_eval::TE_Type::TOKEN ) {
    // grab <identifier> aspect
    auto iden = var->aspects.front()->tk.get();
    // check for <operator> or <keyword>
    if ( !iden->aspects.empty() && iden->aspects.front()->elem_type ==
	 psil_eval::TE_Type::TOKEN ) {
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
bool psil_eval::check_node( psil_parser::token_t * node ) {
  if ( node == nullptr ) {
    throw std::string( "Given nullptr as AST" );
  } else if ( node->type_name == "<program>" ) {
    if ( !node->aspects.empty() &&  node->aspects.front()->elem_type == psil_eval::TE_Type::TOKEN )
      return psil_eval::check_node( node->aspects.front()->tk.get() );
  } else if ( node->type_name == "<form>" ) {
    for ( auto itr = node->aspects.begin(); itr != node->aspects.end(); ++itr ) {
      if ( (*itr)->elem_type == psil_eval::TE_Type::TOKEN ) {
	if ( !psil_eval::check_node( (*itr)->tk.get() ) ) return false;
      }
    }
  } else if ( node->type_name == "<expression>" ) {
    return psil_eval::check_expression( node );
  } else if ( node->type_name == "<definition>" ) {
    return psil_eval::check_definition( node );
  }
  // Do not expand other types of tokens
  return true;
}
