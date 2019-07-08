/** 
    psil_exec.cpp
    PSIL Execution Implementation
    @author Sinclair Gurny
    @version 0.1
    July 2019
*/

#include "psil_exec.h"

void psil_exec::repl( const std::unique_ptr<psil_parser::language_t> & lang, std::string input ) {

  auto ast = psil_parser::parse( lang, input );
  if ( ast ) {
    
    ast->print();

    // eval
    try {
      if ( !psil_eval::check_node( ast.get() ) )
	std::cerr << "Unknown Error while verifying code!" << std::endl;
    } catch ( std::string exp ) {
      std::cerr << "Error while verifying code:: " << exp << std::endl;
      return;
    }

    // exec
    try {
      bool rem = false;
      psil_exec::exec( ast, rem );
      if ( !rem ) {
	ast->print();
      } else {
	std::cout << "No output" << std::endl;
      }
    } catch ( std::string exp ) {
      std::cerr << "Runtime error:: " << exp << std::endl;
    }
    
  } else {
    std::cerr << "Error while parsing input" << std::endl;
  }
}

// === Execute abstract syntax tree
void psil_exec::exec( token_ptr & ast, bool& rem ) {
  if ( ast->type_name == "<program>" ) {
    if ( !ast->aspects.empty() && ast->aspects.front()->elem_type == psil_exec::TE_Type::TOKEN ) {
      bool r = false;
      psil_exec::exec( ast->aspects.front()->tk, r );
      if ( r ) { // all forms erased, erase program
	ast->aspects.clear();
	rem = true;
      }
      return;
    }	
  } else if ( ast->type_name == "<form>" ) {
    size_t tk_count = 0, rem_count = 0;
    for ( auto itr = ast->aspects.begin(); itr != ast->aspects.end();  ) {
      if ( (*itr)->elem_type == psil_exec::TE_Type::TOKEN ) {
	++tk_count;
	bool r = false;
	psil_exec::exec( (*itr)->tk, r );
	if ( r ) { // erase aspect
	  ++rem_count;
	  ast->aspects.erase( itr );
	  continue;
	}
      }
      ++itr;
    }
    // all tokens erased, erase current token
    if ( tk_count == rem_count )
      rem = true;
    return;
  } else if ( ast->type_name == "<expression>" ) {
    if ( ast->aspects.size() == 5 && ast->aspects[1]->elem_type == psil_exec::TE_Type::STRING ) {
      if ( ast->aspects[1]->str == "lambda" ) { // LAMBDA
	//exec_lambda( ast );
      } else if ( ast->aspects[1]->str == "cond" ) { // COND
	bool r = false;
	exec_cond( ast, r );
	if ( !r )
	  exec( ast, r );
	if ( r ) rem = true;
      } else if ( ast->aspects[1]->str == "set!" ) { // SET!
	//exec_set( ast );
      }
      std::cout << "EXEC lambda" << std::endl;
    } else if ( ast->aspects.size() == 6 && ast->aspects[1]->elem_type == psil_exec::TE_Type::STRING &&
		ast->aspects[1]->str == "if" ) { // IF
      bool r = false;
      exec_if( ast, r );
      if ( !r )
	exec( ast, r );
      if ( r ) { rem = true; }
    } else if ( ast->aspects.size() == 1 && ast->aspects.front()->elem_type == psil_exec::TE_Type::TOKEN &&
		ast->aspects.front()->tk->type_name == "<application>" ) { // APPLICATION
      //exec_application( ast->aspects.front()->tk );
    }
  } else if ( ast->type_name == "<definition>" ) { // DEFINITION
    rem = true; // always erase definitions from AST
    exec_def( ast );
  }  
}

// === Execute if expression
void psil_exec::exec_if( token_ptr & node, bool& rem ) {
  bool r = false;
  psil_exec::exec( node->aspects[2]->tk, r );
  if ( r || psil_exec::is_true( node->aspects[2]->tk ) ) {
    auto tmp_ans = std::move( node->aspects[3]->tk );
    node->aspects.clear();
    for ( auto tmp_itr = tmp_ans->aspects.begin(); tmp_itr != tmp_ans->aspects.end(); ++tmp_itr ) {
      node->aspects.push_back( std::move( *tmp_itr ) );
    }
  } else {
    auto tmp_ans = std::move( node->aspects[4]->tk );
    node->aspects.clear();
    for ( auto tmp_itr = tmp_ans->aspects.begin(); tmp_itr != tmp_ans->aspects.end(); ++tmp_itr ) {
      node->aspects.push_back( std::move( *tmp_itr ) );
    }
  }
}

// === Execute cond expression
void psil_exec::exec_cond( token_ptr & node, bool& rem ) {
  bool r = false; // placeholder, not needed
  psil_exec::exec( node->aspects[2]->tk, r );
  if ( psil_exec::is_true( node->aspects[2]->tk ) ) {
    auto tmp_ans = std::move( node->aspects[3]->tk );
    node->aspects.clear();
    for ( auto tmp_itr = tmp_ans->aspects.begin(); tmp_itr != tmp_ans->aspects.end(); ++tmp_itr ) {
      node->aspects.push_back( std::move( *tmp_itr ) );
    }
  } else {
    rem = true;
    node->aspects.clear();
  }
}

// === Execute lambda expression
void psil_exec::exec_lambda( token_ptr & node, bool& rem ) {
  // TODO
}

// === Execute definition
void psil_exec::exec_def( token_ptr & node ) {
  // TODO
}


// ========================================================================================


bool psil_exec::is_true( token_ptr & node ) {
  if ( node->aspects.size() == 1 && node->aspects.front()->elem_type == psil_exec::TE_Type::TOKEN ) {
    if ( node->aspects.front()->tk->type_name == "<constant>" ) {
      auto const_type = node->aspects.front()->tk.get()->aspects.front()->tk.get();
      if ( const_type->type_name == "<boolean>" ) {
	return const_type->aspects.front()->str == "#t";
      } else if ( const_type->type_name == "<number>" ) {
	return !psil_exec::is_zero( node->aspects.front()->tk.get()->aspects.front()->tk );
      } else {
	return true;
      }
    } else if ( node->aspects.front()->tk->type_name == "<variable>" ) {
      std::cerr << "Cannot look up variables yet" << std::endl;
      return true;
    } else if ( node->aspects.front()->tk->type_name == "<application>" ) {
      bool r = false;
      psil_exec::exec( node->aspects.front()->tk, r );
      return psil_exec::is_true( node->aspects.front()->tk );
    }
  }
  bool r = false; // placeholder, not needed
  psil_exec::exec( node, r );
  return psil_exec::is_true( node );
  
}
  
bool psil_exec::is_zero( token_ptr & node ) {
  if ( node->aspects.front()->tk->type_name == "<integer>" ) {
    auto int_tk = node->aspects.front()->tk.get();
    if ( int_tk->aspects.front()->elem_type == psil_exec::TE_Type::STRING &&
	 int_tk->aspects.front()->str == "0" )
      return true;
    else
      return false;
  } else if ( node->aspects.front()->tk->type_name == "<decimal>" ) {
    auto int_tk = node->aspects.front()->tk.get();
    if ( int_tk->aspects.front()->elem_type == psil_exec::TE_Type::STRING &&
	 int_tk->aspects.front()->str == "0.0" )
      return true;
    else
      return false;
  }
  return false;
}
