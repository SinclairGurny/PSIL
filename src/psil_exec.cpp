/** 
    psil_exec.cpp
    PSIL Execution Implementation
    @author Sinclair Gurny
    @version 0.1
    July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

  // ===================================================================================

  token_ptr copy_tk( const token_ptr & tk ) {
    if ( tk == nullptr ) return nullptr;
    token_ptr tmp( new psil_parser::token_t( tk->type_name ) );
    for ( auto itr = tk->aspects.begin(); itr != tk->aspects.end(); ++itr ) {
      if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	std::unique_ptr<psil_parser::token_elem_t>
	  elem_tmp( new psil_parser::token_elem_t( copy_tk( (*itr)->tk ) ) );
	tmp->aspects.push_back( std::move( elem_tmp ) );
      } else {
	std::unique_ptr<psil_parser::token_elem_t>
	  elem_tmp( new psil_parser::token_elem_t( std::string( (*itr)->str ) ) );
	tmp->aspects.push_back( std::move( elem_tmp ) );
      }
    }
    return tmp;
  }

  // === Checks type of expression
  VarType check_type( const token_ptr & tk ) {
    if ( tk->aspects.size() == 1 && tk->aspects.front()->elem_type == TE_Type::TOKEN ) {
      if ( tk->aspects.front()->tk->type_name == "<constant>" ) {
	auto const_type = tk->aspects.front()->tk.get()->aspects.front()->tk.get();
	if ( const_type->type_name == "<boolean>" ) {
	  return VarType::BOOL;
	} else if ( const_type->type_name == "<number>" ) {
	  return VarType::NUM;
	} else {
	  return VarType::CHAR;
	}
      } else if ( tk->aspects.front()->tk->type_name == "<variable>" ) {
	std::cerr << "Cannot look up variables yet" << std::endl;
	return VarType::UNKNOWN;
      } else if ( tk->aspects.front()->tk->type_name == "<application>" ) {
	//exec( tk->aspects.front()->tk, r );
	std::cerr << "Cannot run functions yet" << std::endl;
	return VarType::UNKNOWN;
      }
    } else if ( tk->aspects.size() == 5 && tk->aspects[1]->elem_type == TE_Type::STRING ) {
      if ( tk->aspects[1]->str == "lambda" ) { // LAMBDA
	return VarType::PROC;
      } else if ( tk->aspects[1]->str == "cond" ) { // COND
	/*bool r = false;
	exec_cond( tk, r );
	if ( !r )
	  exec( tk, r );
	  if ( r ) return VarType::UNKNOWN;*/
	//return check_type( tk );
	return VarType::UNKNOWN;
      } else if ( tk->aspects[1]->str == "set!" ) { // SET!
	//exec_set( tk );
	return VarType::ERROR;
      }
    } else if ( tk->aspects.size() == 6 && tk->aspects[1]->elem_type == TE_Type::STRING &&
		tk->aspects[1]->str == "if" ) { // IF
      /*
      bool r = false;
      exec_if( tk, r );
      if ( !r )
	exec( tk, r );
	if ( r ) { rem = true; }*/
      return VarType::UNKNOWN;
    }
    return VarType::UNKNOWN;
  }

  // ===================================================================================

  void stack_t::push() {
    ++current_scope;
    if ( current_scope > 1000 ) {
      throw std::string( "Stack above debug limits" );
    }
  }

  void stack_t::pop() {
    if ( current_scope == 0 ) {
      throw std::string( "Cannot pop an empty stack" );
      --current_scope;
    }
  }

  stack_t::ExistsType stack_t::exists( std::string n ) {
    auto gret = global_table.find( n );
    if ( gret != global_table.end() )
      return stack_t::ExistsType::GLOBAL;
    auto lret = table.find( n );
    if ( lret != table.end() )
      return stack_t::ExistsType::LOCAL;
    return stack_t::ExistsType::NO;
  }

  token_ptr stack_t::get( std::string n, stack_t::ExistsType e ) {
    if ( e == stack_t::ExistsType::GLOBAL ) {
      auto itr = global_table.find( n );
      if ( itr != global_table.end() )
	return copy_tk( itr->second->value );
    } else if ( e == stack_t::ExistsType::LOCAL ) {
      auto itr = table.find( n );
      if ( itr != table.end() )
	return copy_tk( itr->second->value );
    }
    return nullptr;
  }
  
  void stack_t::add( std::string n, const token_ptr & v ) {
    VarType t = check_type( v );
    if ( t == VarType::ERROR ) throw std::string( "Could not determine type of expression" );
    std::unique_ptr<stack_elem_t> se( new stack_elem_t( n, t, v ) );
    table.insert( std::make_pair( n, std::move( se ) ) );
  }

  void stack_t::update( std::string n, stack_t::ExistsType e, const token_ptr & v) {
    if ( e == stack_t::ExistsType::GLOBAL ) {
      auto itr = global_table.find( n );
      if ( itr != global_table.end() ) {
	itr->second->value.reset(); // delete old value
	itr->second->value = std::move( copy_tk( v ) );
      }
    } else if ( e == stack_t::ExistsType::LOCAL ) {
      auto itr = table.find( n );
      if ( itr != table.end() ) {
	itr->second->value.reset(); // delete old value
	itr->second->value = std::move( copy_tk( v ) );
      }
    }
  }

  void stack_t::init() {
    // Logical
    auto tmp = std::make_unique<stack_elem_t>( "and", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "or", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "not", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    // EQUIVALENCE
    tmp = std::make_unique<stack_elem_t>( "equal?", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    // NUMBERS
    //   ARITHMETIC
    tmp = std::make_unique<stack_elem_t>( "+", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "-", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "*", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "*", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "/", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "abs", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "mod", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    //   APPROX
    tmp = std::make_unique<stack_elem_t>( "floor", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "ceil", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "trunc", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "round", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    //   INEQUALITIES
    tmp = std::make_unique<stack_elem_t>( "<", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "<=", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( ">", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( ">=", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "=", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    //   MISC
    tmp = std::make_unique<stack_elem_t>( "zero?", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    // CHARACTERS
    tmp = std::make_unique<stack_elem_t>( "ch<", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "ch<=", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "ch>", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "ch>=", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "ch=", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    // LISTS
    tmp = std::make_unique<stack_elem_t>( "first!", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "second", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "nth", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "first!", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "second!", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "nth!", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "null?", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "quote", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "unquote", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    // IDENTITY PREDICATES
    tmp = std::make_unique<stack_elem_t>( "boolean?", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "number?", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "char?", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "symbol?", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "proc?", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "list?", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    // INPUT/OUTPUT
    tmp = std::make_unique<stack_elem_t>( "print", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "println", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
    tmp = std::make_unique<stack_elem_t>( "read", VarType::PROC, nullptr );
    global_table.insert( std::make_pair( tmp->var_name, std::move( tmp ) ) );
  }
  
  // ===================================================================================
  
  void repl( const std::unique_ptr<psil_parser::language_t> & lang, std::string input ) {
    
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
	auto stack = std::make_unique<stack_t>();
	exec( stack, ast, rem );
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
  
  // ===================================================================================
  
  // === Execute abstract syntax tree
  void exec( stack_ptr & s, token_ptr & ast, bool& rem ) {
    if ( ast->type_name == "<program>" ) {
      if ( !ast->aspects.empty() && ast->aspects.front()->elem_type == TE_Type::TOKEN ) {
	bool r = false;
	exec( s, ast->aspects.front()->tk, r );
	if ( r ) { // all forms erased, erase program
	  ast->aspects.clear();
	  rem = true;
	}
      }
      return;
    } else if ( ast->type_name == "<form>" ) {
      size_t tk_count = 0, rem_count = 0;
      for ( auto itr = ast->aspects.begin(); itr != ast->aspects.end();  ) {
	if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	  ++tk_count;
	  bool r = false;
	  exec( s, (*itr)->tk, r );
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
      if ( ast->aspects.size() == 5 && ast->aspects[1]->elem_type == TE_Type::STRING ) {
	if ( ast->aspects[1]->str == "lambda" ) { // LAMBDA
	  //exec_lambda( s, ast, rem );
	  return;
	} else if ( ast->aspects[1]->str == "cond" ) { // COND
	  exec_cond( s, ast, rem );
	} else if ( ast->aspects[1]->str == "set!" ) { // SET!
	  rem = true; // always erase set! from AST
	  exec_set( s, ast );
	  return;
	}
      } else if ( ast->aspects.size() == 6 && ast->aspects[1]->elem_type == TE_Type::STRING &&
		  ast->aspects[1]->str == "if" ) { // IF
	exec_if( s, ast, rem );
      } else if ( ast->aspects.size() == 1 && ast->aspects.front()->elem_type == TE_Type::TOKEN ) {
	if ( ast->aspects.front()->tk->type_name == "<application>" ) { // APPLICATION
	  exec_app( s, ast->aspects.front()->tk, rem );
	} else if ( ast->aspects.front()->tk->type_name == "<variable>" ) { // VARIABLE
	  exec_var( s, ast, rem );
	} else if ( ast->aspects.front()->tk->type_name == "<constant>" ) { // CONSTANT
	  return;
	}
      } else if ( ast->aspects.size() == 4 && ast->aspects[1]->elem_type == TE_Type::STRING &&
		  ast->aspects[1]->str == "quote" ) { // QUOTE
	return;
      } else {
	throw std::string( "Unknown expression type" );
	return;
      }
      
    } else if ( ast->type_name == "<definition>" ) { // DEFINITION
      rem = true; // always erase definitions from AST
      exec_def( s, ast );
      return;
    }
    if ( rem ) return;
    exec( s, ast, rem );
  }

  // =================================================================================================
  
  // === Execute if expression
  void exec_if( stack_ptr & s, token_ptr & node, bool& rem ) {
    bool r = false;
    exec( s, node->aspects[2]->tk, r );
    if ( r || is_true( s, node->aspects[2]->tk ) ) {
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
  void exec_cond( stack_ptr & s, token_ptr & node, bool& rem ) {
    bool r = false; // placeholder, not needed
    exec( s, node->aspects[2]->tk, r );
    if ( r || is_true( s, node->aspects[2]->tk ) ) {
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
  void exec_lambda( stack_ptr & s, token_ptr & node, bool& rem ) {
    // TODO
  }

  // === Execute application expression
  void exec_app( stack_ptr & s, token_ptr & node, bool& rem ) {
    // TODO
  }

  void exec_set( stack_ptr & s, token_ptr & node ) {
    auto iden = node->aspects[2]->tk->aspects.front()->tk->aspects.front()->str;
    std::cout << "set grabbed " << iden << std::endl;
    auto ret = s->exists( iden );
    if ( ret == stack_t::ExistsType::GLOBAL ) {
      throw std::string( "Cannot set! a global procedure" );
    } else if ( ret == stack_t::ExistsType::LOCAL ) {
      s->update( iden, ret, node->aspects[3]->tk );
    } else { // NO - variable is not known
      throw std::string( "Cannot set a variable that has not been defined" );
    }
  }
  
  // === Execute definition
  void exec_def( stack_ptr & s, token_ptr & node ) {
    auto iden = node->aspects[2]->tk->aspects.front()->tk->aspects.front()->str;
    std::cout << "grabbed " << iden << std::endl;
    auto ret = s->exists( iden );
    if ( ret == stack_t::ExistsType::GLOBAL ) {
      throw std::string( "Cannot redefine a global procedure" );
    } else if ( ret == stack_t::ExistsType::LOCAL ) {
      throw std::string( "Cannot redefine a local variable, use set!" );
    } else { // NO - variable is not known
      s->add( iden, node->aspects[3]->tk );
    }
  }

  void exec_var( stack_ptr & s, token_ptr & node, bool& rem ) {
    auto var_name = node->aspects.front()->tk->aspects.front()->tk->aspects.front()->str;
    std::cout << "Looking for " << var_name << std::endl;
    auto ret = s->exists( var_name );
    if ( ret == stack_t::ExistsType::GLOBAL ) {
      std::cerr <<  "Cannot access global procedures yet" << std::endl;
      rem = true;
    } else if ( ret == stack_t::ExistsType::LOCAL ) {
      auto value = s->get( var_name, ret );
      node.reset(); // erase current token
      node = std::move( value ); // replace with value
    } else { // NO - variable is not known
      throw std::string( "Variable does not exist" );
    }
  }
  
  
  // ========================================================================================
  
  
  bool is_true( stack_ptr & s, token_ptr & node ) {
    if ( node->aspects.size() == 1 && node->aspects.front()->elem_type == TE_Type::TOKEN ) {
      if ( node->aspects.front()->tk->type_name == "<constant>" ) {
	auto const_type = node->aspects.front()->tk.get()->aspects.front()->tk.get();
	if ( const_type->type_name == "<boolean>" ) {
	  return const_type->aspects.front()->str == "#t";
	} else if ( const_type->type_name == "<number>" ) {
	  return !is_zero( node->aspects.front()->tk.get()->aspects.front()->tk );
	} else {
	  return true;
	}
      } else if ( node->aspects.front()->tk->type_name == "<variable>" ) {
	//std::cerr << "Cannot look up variables yet" << std::endl;
	bool r = false;
	exec( s, node->aspects.front()->tk, r );
	return is_true( s, node );
      } else if ( node->aspects.front()->tk->type_name == "<application>" ) {
	bool r = false;
	exec( s, node->aspects.front()->tk, r );
	return is_true( s, node->aspects.front()->tk );
      }
    }
    bool r = false; // placeholder, not needed
    exec( s, node, r );
    return is_true( s, node );
  }
  
  bool is_zero( token_ptr & node ) {
    if ( node->aspects.front()->tk->type_name == "<integer>" ) {
      auto int_tk = node->aspects.front()->tk.get();
      if ( int_tk->aspects.front()->elem_type == TE_Type::STRING &&
	   int_tk->aspects.front()->str == "0" )
      return true;
      else
	return false;
    } else if ( node->aspects.front()->tk->type_name == "<decimal>" ) {
      auto int_tk = node->aspects.front()->tk.get();
      if ( int_tk->aspects.front()->elem_type == TE_Type::STRING &&
	   int_tk->aspects.front()->str == "0.0" )
	return true;
      else
	return false;
  }
    return false;
  }
  
}
