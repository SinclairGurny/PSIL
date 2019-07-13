/** 
    psil_exec.cpp
    PSIL Execution Implementation
    @author Sinclair Gurny
    @version 0.5
    July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

  // ===================================================================================

  // Copies tk and returns an identical token tree
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

  // Compares tk1 and tk2 structure and content for being identical
  bool equal_tk( const token_ptr & tk1, const token_ptr & tk2 ) {
    if ( (tk1 == nullptr && tk2 != nullptr) || (tk1 != nullptr && tk2 == nullptr) ) return false;
    if ( tk1->type_name != tk2->type_name ) return false;
    if ( tk1->aspects.size() != tk2->aspects.size() ) return false;
    auto itr1 = tk1->aspects.begin(); auto itr2 = tk2->aspects.begin();
    for ( ; itr1 != tk1->aspects.end() && itr2 != tk2->aspects.end(); ++itr1, ++itr2 ) {
      if ( (*itr1)->elem_type == TE_Type::TOKEN && (*itr1)->elem_type == TE_Type::TOKEN ) {
	bool ret = equal_tk( (*itr1)->tk, (*itr2)->tk );
	if ( !ret ) return false;
      } else if ( (*itr1)->elem_type == TE_Type::STRING && (*itr1)->elem_type == TE_Type::STRING ) {
	if ( (*itr1)->str != (*itr2)->str ) return false;
      } else {
	return false;
      }
    }
    return true;
  }

  // Finds token What in token Where and replaces occurence of What with That
  bool find_replace( token_ptr & where, const token_ptr & what, token_ptr & that ) {
    if ( where == nullptr ) { return false; }
    bool is_expr = where->type_name == "<expression>" && where->aspects.size() == 1
      && where->aspects.front()->elem_type == TE_Type::TOKEN;
    if ( is_expr && equal_tk( where->aspects.front()->tk, what ) ) {
      // Found location
      where.reset();
      where = std::move( that );
      return true;
    } else {
      // Keep looking
      for ( auto itr = where->aspects.begin(); itr != where->aspects.end(); ++itr ) {
	if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	  bool ret = find_replace( (*itr)->tk, what, that );
	  if ( ret ) return true;
	}
      }
    }
    return false;
  }

  // === Checks type of expression
  VarType check_type( const token_ptr & tk ) {
    if ( tk->aspects.size() == 1 && tk->aspects.front()->elem_type == TE_Type::TOKEN ) {
      if ( tk->aspects.front()->tk->type_name == "<constant>"  ) { // CONSTANT OR DATUM
	auto const_type = tk->aspects.front()->tk.get()->aspects.front()->tk.get();
	if ( const_type->type_name == "<boolean>" ) {
	  return VarType::BOOL;
	} else if ( const_type->type_name == "<number>" ) {
	  return VarType::NUM;
	} else if ( const_type->type_name == "<character>" ) {
	  return VarType::CHAR;
	} else if ( const_type->type_name == "<list_def>" ) {
	  return check_type( const_type->aspects[2]->tk );
	}
	return VarType::ERROR;
      } else if ( tk->aspects.front()->tk->type_name == "<variable>" ) { // VARIABLE
	std::cerr << "Cannot look up variables yet" << std::endl;
	return VarType::UNKNOWN;
      } else if ( tk->aspects.front()->tk->type_name == "<lambda>" ) { // LAMBDA
	return VarType::PROC;
      } else if ( tk->aspects.front()->tk->type_name == "<conditional>" ) { // CONDITIONAL
	// lazy eval, don't run
	return VarType::UNKNOWN;
      } else if ( tk->aspects.front()->tk->type_name == "<application>" ) { // APPLICATION
	// lazy eval, don't run
	return VarType::UNKNOWN;
      } else if ( tk->type_name == "<datum>" ) {
	auto datum_type = tk->aspects.front()->tk->type_name;
	if ( datum_type == "<boolean>" ) {
	  return VarType::BOOL;
	} else if ( datum_type == "<number>" ) {
	  return VarType::NUM;
	} else if ( datum_type == "<character>" ) {
	  return VarType::CHAR;
	} else if ( datum_type == "<symbol>" ) {
	  return VarType::UNKNOWN;
	} else if ( datum_type == "<list>" ) {
	  return VarType::LIST;
	}
      }
    }
    return VarType::UNKNOWN;
  }


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
  


  // ===================================================================================

  // Push to stack
  void stack_t::push() {
    ++current_scope;
    if ( current_scope > 1000 ) {
      throw std::string( "Stack above debug limits" );
    }
  }
  
  // Pop stack
  void stack_t::pop() {
    if ( current_scope == 0 ) {
      throw std::string( "Cannot pop an empty stack" );
      --current_scope;
    }
  }

  // Check if variable exists in symbol table
  stack_t::ExistsType stack_t::exists( std::string n ) {
    auto gret = global_table.find( n );
    if ( gret != global_table.end() )
      return stack_t::ExistsType::GLOBAL;
    auto lret = table.find( n );
    if ( lret != table.end() )
      return stack_t::ExistsType::LOCAL;
    return stack_t::ExistsType::NO;
  }

  // Gets variable from symbol table
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

  // Adds variable and its value to symbol table
  void stack_t::add( std::string n, const token_ptr & v ) {
    VarType t = check_type( v );
    if ( t == VarType::ERROR ) throw std::string( "Could not determine type of expression" );
    std::unique_ptr<stack_elem_t> se( new stack_elem_t( n, t, v ) );
    table.insert( std::make_pair( n, std::move( se ) ) );
  }

  // Updates variable's value in symbol table
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

  // Initialize global procedures in symbol table
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
    tmp = std::make_unique<stack_elem_t>( "first", VarType::PROC, nullptr );
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
#ifdef DEBUG_MODE
      ast->print();
#endif      
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
#ifdef DEBUG_MODE
	if ( !rem ) {
	  ast->print();
	}
#endif	
	std::cout << std::endl;
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
	exec( s, ast->aspects.front()->tk, rem );
	// all forms erased, erase program
	if ( rem ) ast->aspects.clear();
      }
      return;
    } else if ( ast->type_name == "<form>" ) {
      size_t tk_count = 0, rm_count = 0;
      for ( auto itr = ast->aspects.begin(); itr != ast->aspects.end();  ) {
	if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	  ++tk_count;
	  bool r = false;
	  exec( s, (*itr)->tk, r );
	  if ( r ) { // erase aspect
	    ++rm_count;
	    itr = ast->aspects.erase( itr );
	    continue;
	  }
	}
	++itr;
      }
      // all tokens erased, erase current token
      if ( tk_count == rm_count )
	rem = true;
      else if ( ast->aspects.size() == 4 && tk_count > rm_count && tk_count - rm_count == 1 ) {
	auto tmp = std::move( ast->aspects[2] );
	ast->type_name = tmp->tk->type_name;
	ast->aspects.clear();
	std::move( tmp->tk->aspects.begin(), tmp->tk->aspects.end(), std::back_inserter( ast->aspects ) );
      }
      return;
    } else if ( ast->type_name == "<expression>" ) {
#ifdef DEBUG_MODE
      std::cout << "EXP" << std::endl;
#endif
      auto expr = ast->aspects.front()->tk.get();
      if ( ast->aspects.size() == 1 ) {
	if ( expr->type_name == "<constant>" ) {
#ifdef DEBUG_MODE
	  std::cout << "CONST" << std::endl;
#endif
	  return;
	} else if ( expr->type_name == "<variable>" ) {
	  exec_var( s, ast, rem );
	} else if ( expr->type_name == "<lambda>" ) {
	  return;
	} else if ( expr->type_name == "<conditional>" ) {
	  if ( expr->aspects[1]->str == "cond" ) { // COND
	    exec_cond( s, ast, rem );
	  } else { // IF
	    exec_if( s, ast, rem );
	  }
	} else if ( expr->type_name == "<application>" ) {
	  exec_app( s, ast, rem );
	}
      } else if ( ast->aspects.size() >= 4 ) { // (begin <expr>+)
	size_t tk_count = 0, rm_count = 0;
	for ( auto itr = ast->aspects.begin(); itr != ast->aspects.end(); ) {
	  if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	    ++tk_count;
	    bool r = false;
	    exec( s, (*itr)->tk, r );
	    if ( r ) {
	      ++rm_count;
	      itr = ast->aspects.erase( itr );
	      continue;
	    }
	  }
	  ++itr;
	}
	if ( tk_count == rm_count ) {
	  rem = true;
	} else if ( tk_count > rm_count && tk_count - rm_count == 1 ) {
	  auto tmp = std::move( ast->aspects[2] );
	  ast->type_name = tmp->tk->type_name;
	  ast->aspects.clear();
	  std::move( tmp->tk->aspects.begin(), tmp->tk->aspects.end(), std::back_inserter( ast->aspects ) );
	}
	return;
      } else {
	throw std::string( "Unknown expression type" );
      }	
    } else if ( ast->type_name == "<definition>" ) {
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
#ifdef DEBUG_MODE
    std::cout << "IF" << std::endl;
#endif
    auto cond = node->aspects.front()->tk.get();
    bool r = false;
    exec( s, cond->aspects[2]->tk, r );
    if ( r || is_true( s, cond->aspects[2]->tk ) ) {
      auto tmp_ans = std::move( cond->aspects[3]->tk );
      node.reset();
      node = std::move( tmp_ans );
    } else {
      auto tmp_ans = std::move( cond->aspects[4]->tk );
      node.reset();
      node = std::move( tmp_ans );
    }
  }
  
  // === Execute cond expression
  void exec_cond( stack_ptr & s, token_ptr & node, bool& rem ) {
#ifdef DEBUG_MODE
    std::cout << "COND" << std::endl;
#endif
    auto cond = node->aspects.front()->tk.get();
    bool r = false; // placeholder, not needed
    exec( s, cond->aspects[2]->tk, r );
    if ( r || is_true( s, cond->aspects[2]->tk ) ) {
      auto tmp_ans = std::move( cond->aspects[3]->tk );
      node.reset();
      node = std::move( tmp_ans );
    } else {
      rem = true;
      node->aspects.clear();
    }
  }

  // === Execute definition
  void exec_def( stack_ptr & s, token_ptr & node ) {
#ifdef DEBUG_MODE
    std::cout << "DEF" << std::endl;
#endif
    if ( node->aspects[1]->str == "define" ) {
      auto iden = node->aspects[2]->tk->aspects.front()->tk->aspects.front()->str;
#ifdef DEBUG_MODE
      std::cout << "grabbed " << iden << std::endl;
#endif
      auto ret = s->exists( iden );
      if ( ret == stack_t::ExistsType::GLOBAL ) {
	throw std::string( "Cannot redefine a global procedure" );
      } else if ( ret == stack_t::ExistsType::LOCAL ) {
	throw std::string( "Cannot redefine a local variable, use set!" );
      } else { // NO - variable is not known
	s->add( iden, node->aspects[3]->tk );
      }
    } else if ( node->aspects[1]->str == "update" ) {
      auto iden = node->aspects[2]->tk->aspects.front()->tk->aspects.front()->str;
#ifdef DEBUG_MODE
      std::cout << "update grabbed " << iden << std::endl;
#endif
      auto ret = s->exists( iden );
      if ( ret == stack_t::ExistsType::GLOBAL ) {
	throw std::string( "Cannot set! a global procedure" );
      } else if ( ret == stack_t::ExistsType::LOCAL ) {
	s->update( iden, ret, node->aspects[3]->tk );
      } else { // NO - variable is not known
	throw std::string( "Cannot set a variable that has not been defined" );
      }
    } else {
      throw std::string( "Unknown definition type" );
    }
  }

  // === Execute variable expansion
  void exec_var( stack_ptr & s, token_ptr & node, bool& rem ) {
#ifdef DEBUG_MODE
    std::cout << "Var" << std::endl;
#endif
    auto var_name = node->aspects.front()->tk->aspects.front()->tk->aspects.front()->str;
#ifdef DEBUG_MODE
    std::cout << "Looking for " << var_name << std::endl;
#endif
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
  


  // === Execute application of procedures
  void exec_app( stack_ptr & s, token_ptr & node, bool & rem ) {
#ifdef DEBUG_MODE
    std::cout << "App" << std::endl;
#endif
    size_t idx = 0; // Keeps track of location within expression
    std::string func_name; // Used to lookup proper global procedures
    stack_t::ExistsType func_loc = stack_t::ExistsType::NO; // Says whether function is lambda or builtin
    // === Perform checks and get info about application ===
    auto itr = node->aspects.front()->tk->aspects.begin();
    for ( ; itr != node->aspects.front()->tk->aspects.end(); ++itr, ++idx ) {
      if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	if ( idx == 1 ) { // Function name
	  if ( (*itr)->tk->aspects.size() == 1 &&
	       (*itr)->tk->aspects.front()->elem_type == TE_Type::TOKEN ) {
	    auto exp_tmp = (*itr)->tk->aspects.front()->tk.get();
	    auto exp_type = exp_tmp->type_name;
	    if ( exp_type == "<variable>" ) {
	      //         <variable>              <identifier>
	      auto iden = exp_tmp->aspects.front()->tk.get();
	      if ( iden->aspects.front()->elem_type == TE_Type::TOKEN ) { // Keyword or Operator
		func_name = iden->aspects.front()->tk->aspects.front()->str;
		if ( iden->aspects.front()->tk->type_name == "<operator>" ) {
		  func_loc = stack_t::ExistsType::GLOBAL;
		} else {
		  func_loc = s->exists( func_name );
		  if ( func_loc != stack_t::ExistsType::GLOBAL ) {
		    throw std::string( "Could not find proc" );
		  }
		}
	      } else { // Locally defined operation
		bool r = false;
		exec( s, (*itr)->tk, r );
		if ( r )
		  throw std::string( "Missing function in application expression" );
		//throw std::string( "Error in application: Not a procedure, identifier found" );
		func_loc = stack_t::ExistsType::LOCAL;
	      }
	    } else if ( exp_type == "<lambda>" ) {
	      std::cerr << "Lambda evaluation is not yet implmented" << std::endl;;
	      func_loc = stack_t::ExistsType::LOCAL;
	    } else if ( exp_type == "<constant>" ) {
	      throw std::string( "Cannot apply a constant" );
	    } else {
	      bool r = false;
	      exec( s, (*itr)->tk, r );
	      if ( r )
		throw std::string( "Missing function in application expression" );
	      else {
		exec_app( s, node, rem );
		return;
	      }
	      //throw std::string( "Error Application: Not a procedure, not valid expression" );
	    }
	  } else {
	    bool r = false;
	    exec( s, (*itr)->tk, r );
	    if ( r )
	      throw std::string( "Missing function in application expression" );
	    else {
	      exec_app( s, node->aspects.front()->tk, rem );
	      return;
	    }
	  }
	} else if ( idx > 1 && idx < (node->aspects.front()->tk->aspects.size()-1) ) { // Arguments
	  bool r = false;
	  exec( s, (*itr)->tk, r );
	  if ( r ) node->aspects.front()->tk->aspects.erase( itr );
	}
      }
    }
    // === Run operations ===
    if ( func_loc == stack_t::ExistsType::GLOBAL ) {
      apply_global_proc( s, node, rem, func_name );
    } else if ( func_loc == stack_t::ExistsType::LOCAL ) {
      // Run lambda expression
      apply_lambda( s, node, rem );
    }
  }
  

  void apply_lambda( stack_ptr & s, token_ptr & node, bool& rem ) {
    //       <expression>           <application>
    auto app = node->aspects.front()->tk.get();
    //           <application>     <expression>        <lambda>
    auto lambda = app->aspects[1]->tk->aspects.front()->tk.get();

    size_t lambda_args = lambda->aspects[2]->tk->aspects.size() - 2;
    size_t app_args = app->aspects.size() - 3;
    if ( lambda_args != app_args ) { // Arity Error
      std::string err = "Arity mismatch expected:" + std::to_string( lambda_args );
      err += " given:" + std::to_string( app_args );
      throw std::string( err );
    }

    // Substitute arguments
    auto lambda_end = lambda->aspects[2]->tk.get();
    auto litr = lambda->aspects[2]->tk->aspects.begin();
    std::advance( litr, 1 );
    auto aitr = app->aspects.begin();
    std::advance( aitr, 2 );

    for ( ; litr != lambda_end->aspects.end() && (*litr)->elem_type == TE_Type::TOKEN;  ) {
      // Replace arguments
      find_replace( lambda->aspects[3]->tk, (*litr)->tk, (*aitr)->tk );
      // Erase arguments from lambda and application
      (*litr).reset(); (*aitr).reset();
      litr = lambda->aspects[2]->tk->aspects.erase( litr );
      aitr = app->aspects.erase( aitr );
    }
    
    // Check arguments again
    lambda_args = lambda->aspects[2]->tk->aspects.size() - 2;
    app_args = app->aspects.size() - 3;

    if ( lambda_args != 0 || app_args != 0 ) {
      throw std::string( "Error while applying lambda expression" );
    }

    auto body = std::move( lambda->aspects[3]->tk );
    node->aspects.clear();
    node->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( "(" ) );
    node->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( "begin" ) );
    for ( auto itr = body->aspects.begin(); itr != body->aspects.end(); ++itr ) {
      node->aspects.push_back( std::move( *itr ) );
    }
    node->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( ")" ) );
  }

}
