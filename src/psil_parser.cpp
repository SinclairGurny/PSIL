/** 
    psil_parser.cpp
    PSIL Parser implementation
    @author Sinclair Gurny
    @version 0.1
    July 2019
 */


#include "psil_parser.h"
using namespace psil;

// =========================================================================================================
// ===================== STRUCT IMPLEMENTATIONS ============================================================
// =========================================================================================================

// ========== Token ==================================================

// === Prints token with breadth-first search
void psil::token_t::print() {
  std::cout << "PRINTING" << std::endl;
  std::vector<const psil::token_elem_t * > current;
  std::vector<const psil::token_elem_t * > next;

  //const token_elem_t * tmp = new token_elem_t( this );
  current.push_back( new token_elem_t( this ) );
  
  while ( current.size() > 0 ) {
    for ( auto elem : current ) {
      if ( elem->type ) { // string
	std::cout << "  Aspect:: " << elem->str;
      } else { // token
	std::cout << "  Tk:type: " << elem->tk->type;
	for ( auto it = elem->tk->aspects.begin(); it != elem->tk->aspects.end(); ++it ) {
	  next.push_back( it->get() );
	}
      }
    }
    std::cout << std::endl;
    current = next;
    next.clear();
  }
  std::cout << std::endl;
}


// ========== Parser =================================================


// === Constructs parser and tokenize rules
psil::parser_t::parser_t( std::string n, std::string rs ) :  name(n) {
  // split rule list into parts
  std::string rule;
  size_t loc;
  while ( (loc = rs.find("|")) != std::string::npos ) {
      rule = rs.substr(0, loc);
      rs.erase(0, loc+1);
      rule = std::regex_replace(rule, std::regex("^ +| +$|( ) +"), "$1");
      str_vec rvec = psil::tokenize_rule( rule );
      rules.push_back( rvec );
  }
  if ( rs.size() > 0 ) {
    rs = std::regex_replace( rs, std::regex("^ +| +$|( ) +"), "$1");
    str_vec rvec = psil::tokenize_rule( rs );
    rules.push_back( rvec );
  }
}


// === Prints parser
void psil::parser_t::print( int depth = 1 ) const {
  //if ( this == nullptr ) return;
  std::cout << std::string(depth*2, ' ') << name << std::endl;
  std::cout << std::string(depth*3, ' ');
  for ( auto rule : rules ) {
    print_vec( rule, false );
    std::cout << " | ";
  }
  std::cout << std::endl;
}

// ========== Group ===============================================

// === Prints group
void psil::group_t::print( int depth = 1 ) const {
  std::cout << std::string(depth*2, ' ') << name << std::endl;
  for ( auto elem = items.begin(); elem != items.end(); ++elem ) {
    if ( (*elem)->type ) {
      (*elem)->p->print( depth + 1 );
    } else {
      (*elem)->g->print( depth + 1 );
    }
  }
}

// ========== Language ===========================================

// === Adds group to language
psil::group_t * psil::language_t::add( psil::group_t * g ) {
  std::unique_ptr<lang_elem_t> elem( new lang_elem_t( g ) );
  this->items.push_back( std::move(elem) );

  return g;
}

// === Adds parser to lanuage
psil::parser_t * psil::language_t::add( psil::parser_t * p ) {
  all_parsers.insert( std::make_pair( p->name, p ) );

  std::unique_ptr<lang_elem_t> elem( new lang_elem_t( p ) );
  this->items.push_back( std::move(elem) );

  return p;
}

// Add parser to group
psil::parser_t * psil::language_t::add( psil::group_t * g, psil::parser_t * p ) {
  all_parsers.insert( std::make_pair( p->name, p ) );

  std::unique_ptr<lang_elem_t> elem( new lang_elem_t( p ) );
  g->items.push_back( std::move(elem) );

  return p;
}

// Add group to group
psil::group_t * psil::language_t::add( psil::group_t * g_up, group_t * g_down ) {
  std::unique_ptr<lang_elem_t> elem( new lang_elem_t( g_down ) );
  g_up->items.push_back( std::move(elem) );

  return g_down;
}

/**
   Find parser by name inside language

   @param pn - name of parser
   @return parser pointer if found, nullptr if not
*/
psil::parser_t * psil::language_t::get_parser( std::string pn ) const {
  std::string name = ( pn[pn.size()-1] == '+' ) ? pn.substr(0, pn.size()-1) : pn;
  auto ret_itr = all_parsers.find( name );
  if ( ret_itr != all_parsers.end() ) {
    return ret_itr->second.get();
  }
  return nullptr;

}

/**
   Find all top level parsers inside language

   @return vector of parser pointers
*/
std::vector<parser_t * > psil::language_t::get_top_parsers() const {
  
  std::vector<parser_t * > ret;
  for ( auto it = items.begin(); it != items.end(); ++it ) {
    if ( (*it)->type ) {
      ret.push_back( (*it)->p );
    }
  }
  return ret;
}


// === Prints language
void psil::language_t::print() const {
  std::cout << "Language: " << name << " ";
  std::cout << items.size() << " " << all_parsers.size() << std::endl;
  for ( auto elem = items.begin(); elem != items.end(); ++elem ) {
    if ( (*elem)->type ) {
      (*elem)->p->print();
    } else {
      (*elem)->g->print();
    }
  }
  std::cout<<std::endl;
}


// ========================== Printing functions ==============================================================


// === Prints vector of strings
void psil::print_vec( const std::vector<std::string> &v, bool newline = true ) {
  for ( size_t i = 0; i < v.size(); ++i ) {
    if ( i!= 0 ) { std::cout << " "; }
    std::cout << v[i];
  }
  if ( newline ) { std::cout << std::endl; }
}



// ========================== Helping functions ==============================================================

// === Checks whether input is a <rule> that can be expanded
bool psil::is_rule( std::string input ) {
  if ( input.size() >= 3 ) {
    if ( input[0] == '<' && input[input.size()-1] == '>' ) {
      return true;
    }
  }
  if ( input.size() >= 4 ) {
    size_t len = input.size();
    if ( input[0] == '<' && input[len-2] == '>' &&
	 ( input[len-1] == '*' || input[len-1] == '+' ) ) {
      return true;
    }
  }
  return false;
}

// === Checks whether input is a {regex} expression
bool psil::is_regex( std::string input ) {
  if ( input.size() >= 3 ) {
    if ( input[0] == '{' && input[input.size()-1] == '}' ) {
      return true;
    }
  }
  return false;
}

// === Runs regex expression and returns whether it matches
bool psil::match_regex( std::string expression, std::string input ) {
  std::string expr = expression.substr(1,expression.size()-2);
  std::regex re( expr );
  return std::regex_match( input, re );
}

// === Verify entire string has matching parens
bool psil::check_parens( std::string input ) {
  std::vector<char> memory;
  for ( char c : input ) {
    if ( c == '(' || c == '[' ) {
      memory.push_back(c);
    }
    if ( c == ')' ) {
      if ( memory.size() == 0 ) { return false; }
      if ( memory.back() == '(' ) {
	memory.pop_back();
      } else {
	return false;
      }
    }
    if ( c == ']' ) {
      if ( memory.size() == 0 ) { return false; }
      if ( memory.back() == '[' ) {
	memory.pop_back();
      } else {
	return false;
      }
    }
  }
  return memory.size() == 0;
}

// === Find matching parens in a string
std::pair<size_t, std::string> psil::match_parens( size_t start, std::string input ) {
  if ( start >= input.size() ) { return std::make_pair( start, "Input not large enough" ); }
  size_t count = 1;
  char paren_open = input[start];
  for ( size_t i = start+1; i < input.size(); ++i ) {
    if ( input[i] == '(' || input[i] == '[' ) {
      ++count;
    } else if ( input[i] == ')' || input[i] == ']' ) {
      --count;
      if ( count == 0 ) {
	if ( (paren_open == '(' && input[i] == ')') ||
	     (paren_open == '[' && input[i] == ']') ) {
	  return std::make_pair( i, "" );
	} else {
	  return std::make_pair( i, "Mismatching parens" );
	}
      }
    }
  }

  if ( count > 0 ) {
    return std::make_pair( count, "Missing closing paren" );
  }
  return std::make_pair( count, "Error" );
}

// === Find matching parens in a vector of strings
std::pair<size_t, std::string> psil::match_parens( size_t start, str_vec input ) {
  if ( start >= input.size() ) { return std::make_pair( start, "Input not large enough" ); }
  size_t count = 1;
  std::string paren_open = input[start];
  for ( size_t i = start+1; i < input.size(); ++i ) {
    if ( input[i] == "(" || input[i] == "[" ) {
      ++count;
    } else if ( input[i] == ")" || input[i] == "]" ) {
      --count;
      if ( count == 0 ) {
	if ( (paren_open == "(" && input[i] == ")") ||
	     (paren_open == "[" && input[i] == "]") ) {
	  return std::make_pair( i, "" );
	} else {
	  return std::make_pair( i, "Mismatching parens" );
	}
      }
    }
  }
  if ( count > 0 ) {
    return std::make_pair( count, "Missing closing paren" );
  }
  return std::make_pair( count, "Error" );
}


// ============================================================================================================
// =============================== PARSING FUNCTIONS ==========================================================
// ============================================================================================================

// === Split rule into tokens
std::vector<std::string> psil::tokenize_rule( std::string rule ) {
  std::vector<std::string> ret;
  size_t loc = 0;
  
  for ( size_t r = 0; r < rule.size(); ++r ) {
    if ( rule[r] == '(' ) {
      // find closing paren
      auto mp_ret = psil::match_parens( r, rule );
      if ( mp_ret.second.size() == 0 ) {
	auto tmp_vec = psil::tokenize_rule( rule.substr( r+1, mp_ret.first-r-1) );
	ret.push_back(rule.substr(r,1));
	ret.insert( ret.end(), tmp_vec.begin(), tmp_vec.end() );
	ret.push_back(rule.substr(mp_ret.first, 1));
	r = mp_ret.first;
      } else { // Error occurred
	std::cout << "Error while parsing rule\n";
	std::cout << "Error: " << mp_ret.second;
	std::cout << "Rule: " << rule << std::endl;
	ret.clear();
	return ret;
      }
    } else if ( rule[r] == '<' ) {
      // find closing bracket and tokenize
      if ( (loc = rule.find_first_of(">", r)) != std::string::npos ) {
	int x = 0;
	if ( loc+1 < rule.size() && rule[loc+1] == '+' ) { x=1; }
	std::string tmp = rule.substr( r, loc-r+x+1 );
	ret.push_back( tmp );
	r = loc+x;
      } else { // Error occurred
	std::cout << "Error while parsing <> in rule\n";
	std::cout << "Rule: " << rule << std::endl;
	ret.clear();
	return ret;
      }
    } else if ( rule[r] != ' ' ) {
      // find next space and tokenize
      if ( (loc = rule.find_first_of(" ", r)) != std::string::npos ) {
	std::string tmp = rule.substr( r, loc-r );
	ret.push_back( tmp );
	r = loc;
      } else {
	std::string tmp = rule.substr( r );
	ret.push_back( tmp );
	r = rule.size();
      }
    }
  }

  return ret;
}

// === Split input string into tokens
std::vector<std::string> psil::tokenize_input( std::string input ) {
  std::vector<std::string> ret;
  size_t loc = 0;
  
  for ( size_t i = 0; i < input.size(); ++i ) {
    if ( input[i] == '('  ) {
      // find closing paren
      auto mp_ret = psil::match_parens( i, input );
      if ( mp_ret.second.size() == 0 ) {
	auto tmp_vec = psil::tokenize_input( input.substr( i+1, mp_ret.first-i-1) );
	ret.push_back(input.substr(i, 1));
	ret.insert( ret.end(), tmp_vec.begin(), tmp_vec.end() );
	ret.push_back(input.substr(mp_ret.first,1));
	i = mp_ret.first;
      } else { // Error occurred
	std::cout << "Error while parsing input\n";
	std::cout << "Error: " << mp_ret.second;
	std::cout << "Input: " << input << std::endl;
	// EXIT
	ret.clear();
	return ret;
      }
    } else if ( input[i] != ' ' ) {
      // find next space and tokenize
      if ( (loc = input.find_first_of(" ", i)) != std::string::npos ) {
	std::string tmp = input.substr( i, loc-i );
	ret.push_back( tmp );
	i = loc;
      } else {
	std::string tmp = input.substr( i );
	ret.push_back( tmp );
	i = input.size();
      }
    }
  }

  return ret;
}


// ================================== Primary Parsing Functions =====================================

// === Match specific rule to list of tokens
std::unique_ptr<psil::token_t> psil::match_rule( const std::unique_ptr<psil::language_t> & lang,
						 std::string pn, psil::str_vec rule,
						 psil::str_vec input, psil::upoint pt, bool& match) {
  
  std::unique_ptr<psil::token_t> tptr( new token_t( pn ) );
  size_t in_itr = pt.first;
  size_t ru_itr = 0;
  size_t count = 0;
  
  for ( ; in_itr <= pt.second && ru_itr < rule.size(); ) {
    // Match parens
    if ( rule[ru_itr] == "(" || rule[ru_itr]  == ")" ||
	 rule[ru_itr] == "[" || rule[ru_itr]  == "]" ) {
      if ( rule[ru_itr] == input[in_itr] ) {
	#ifdef DEBUG_MODE
	std::cout << "MATCHED PAREN" << pn << std::endl;
	#endif
 	// Add result to token
	std::unique_ptr<psil::token_elem_t> tke( new token_elem_t( input[in_itr] ) );
	tptr->aspects.push_back( std::move(tke) );
	
	++ru_itr; ++in_itr;
	continue;
      } else {
	#ifdef DEBUG_MODE
	std::cout << "Exit PAREN" << pn << std::endl;
	#endif
	if ( count > 0 ) { ++ru_itr; count = 0; continue; }
	return nullptr;
      }
    }
    // Check for expansion
    else if ( psil::is_rule( rule[ru_itr] ) ) {
      psil::parser_t * next_p( lang->get_parser( rule[ru_itr] ) );
      if ( next_p ) {
	bool new_match = false;
	psil::upoint new_pt;
	if ( input[in_itr] == "(" ) {
	  auto p_loc = match_parens( in_itr, input );
	  if ( p_loc.second.size() == 0 ) {
	    #ifdef DEBUG_MODE
	    std::cout << "Broke" << in_itr << " " << p_loc.first << pn <<std::endl;
	    #endif
	    new_pt = std::make_pair( in_itr, p_loc.first );
	  } else {
	    #ifdef DEBUG_MODE
	    std::cout << "Exit Could not find matching paren" << pn << std::endl;
	    #endif
	    if ( count > 0 ) { ++ru_itr; count = 0; continue; }
	    return nullptr;
	  }
	} else {
	  new_pt = std::make_pair( in_itr, in_itr );
	}
	#ifdef DEBUG_MODE
	std::cout << "RECURSION" << pn << "->" << next_p->name << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(DEBUG_DELAY));
	#endif
	auto new_ret = psil::apply_parser( lang, next_p, input, new_pt, new_match );
	if ( new_match ) {
	  // Add result to current token
	  std::unique_ptr<psil::token_elem_t> tke( new token_elem_t( std::move(new_ret) ) );
	  tptr->aspects.push_back( std::move(tke) );
	  
	  if ( rule[ru_itr][rule[ru_itr].size()-1] == '+' ) {
	    ++count;
	  } else {
	    ++ru_itr;
	  }
	  in_itr = new_pt.second+1;
	  continue;
	} else {
	  #ifdef DEBUG_MODE
	  std::cout << "Exit No recursion match" << pn << std::endl;
	  #endif
	  if ( count > 0 ) { ++ru_itr; continue; }
	  return nullptr;
	}
      } else {
        #ifdef DEBUG_MODE
	std::cout << "Exit No rule found" << pn << std::endl;
	#endif
	if ( count > 0 ) { ++ru_itr; count = 0; continue; }
	return nullptr;
      }
    }
    // Check for regex
    else if ( psil::is_regex( rule[ru_itr] ) ) {
      if ( psil::match_regex( rule[ru_itr], input[in_itr] ) ) {
        #ifdef DEBUG_MODE
	std::cout << "REGEX_MATCH" << pn << std::endl;
	#endif
	std::unique_ptr<psil::token_elem_t> tke( new token_elem_t( input[in_itr] ) );
	tptr->aspects.push_back( std::move(tke) );
	++ru_itr; ++in_itr;
      } else {
        #ifdef DEBUG_MODE
	std::cout << "Exit no regex match" << pn << std::endl;
	#endif
	if ( count > 0 ) { ++ru_itr; count = 0; continue; }
	return nullptr;
      }
    }
    // Check for exactness
    else if ( rule[ru_itr] == input[in_itr] ) {
      #ifdef DEBUG_MODE
      std::cout << "EXACT" << pn << std::endl;
      #endif
      // Add result to current token
      std::unique_ptr<psil::token_elem_t> tke( new token_elem_t( input[in_itr] ) );
      tptr->aspects.push_back( std::move(tke) );
      ++ru_itr; ++in_itr;
      continue;
    } else {
      #ifdef DEBUG_MODE
      std::cout << "Exit Not exact" << pn << std::endl;
      #endif
      if ( count > 0 ) { ++ru_itr; count = 0; continue; }
      return nullptr;
    }
    
  }
  
  match = true;
  return tptr;
}

// === Try all rules of a parser
std::unique_ptr<psil::token_t> psil::apply_parser( const std::unique_ptr<psil::language_t> & lang,
						 psil::parser_t * par, psil::str_vec input_tks,
						 psil::upoint pt, bool& match ) {
  #ifdef DEBUG_MODE
  std::cout << "APPLYING PARSER" << par->name << std::endl;
  #endif
  bool new_match = false;
  for ( psil::str_vec rule : par->rules ) {
    auto ret = psil::match_rule( lang, par->name, rule, input_tks, pt, new_match );
    if ( new_match ) { match = true; return ret; }
  }
  return nullptr;
}

// === Parsing driver function
std::unique_ptr<psil::token_t> psil::parse( const std::unique_ptr<psil::language_t> & lang, std::string input ) {

  if ( input.size() == 0 ) {
    std::cout << "Empty input" << std::endl;
    return nullptr;
  }
  if ( !check_parens( input ) ) {
    std::cout << "Mismatching parens" << std::endl;
    return nullptr;
  }

  bool match = false;
  // === Split input into tokens ===
  str_vec input_tokens = psil::tokenize_input( input );
  print_vec( input_tokens );

  psil::upoint pt = std::make_pair( 0, input_tokens.size()-1 );


  for ( auto parser : lang->get_top_parsers() ) { // reduce to top level parsers
    auto ret = apply_parser( lang, parser, input_tokens, pt, match );
    if ( match ) { return ret; }
  }
  return nullptr;
}
				   

// ========================== Premade functionality ===========================================================

// === Make PSIL language
std::unique_ptr<psil::language_t> psil::make_psil_lang() {
  std::unique_ptr<psil::language_t> lang( new psil::language_t( "PSIL" ) );

  lang->add( new psil::parser_t( "<program>", "<form>+" ) );
  
  psil::group_t * gf = lang->add( new psil::group_t( "FORMS" ) );
  lang->add( gf, new psil::parser_t( "<form>", "<definition> | <expression>" ) );
  
  psil::group_t * gd( new psil::group_t( "DEFINITIONS" ) );
  lang->add( gd );
  lang->add( gd, new psil::parser_t( "<definition", "<var_def> | (begin <definition>+)" ) );
  lang->add( gd, new psil::parser_t( "<var_def>",
			       "(define <variable> <expression>)"
			       "| (define (<variable>+) <expression>)" ) );
  lang->add( gd, new psil::parser_t( "<variable>", "<identifier>" ) );
  lang->add( gd, new psil::parser_t( "<body>", "<expression>+ | <definition>+ <expression>+" ) );
				     
  psil::group_t * ge = lang->add( new psil::group_t( "EXPRESSIONS" ) );
  lang->add( ge, new psil::parser_t( "<expression>",
				     "<constant> | <variable> | (lambda <formals> <body>)"
				     "| (if <expression> <expression> <expression>)"
				     "| (cond <expression> <expression>)"
				     "| (set! <variable> <expression>)"
				     "| (quote <datum>) | <application>" ) );
  lang->add( ge, new psil::parser_t( "<constant>", "<boolean> | <number> | <character>" ) );
  lang->add( ge, new psil::parser_t( "<formals>", "<variable> | (<variable>+)" ) );
  lang->add( ge, new psil::parser_t( "<application>", "(<expression>+)" ) );

  psil::group_t * gi = lang->add( new psil::group_t( "IDENTIFIERS" ) );
  lang->add( gi, new psil::parser_t( "<identifier>",
				     "{^[a-zA-Z_](?!.)} | <operator>"
				     "| {^[a-zA-Z_][a-zA-Z_0-9\\!]+(?!.)}" ) );
  lang->add( gi, new psil::parser_t( "<operator>", "+ | - | * | \\ | %" ) );
  
  psil::group_t * gda = lang->add( new psil::group_t( "DATA" ) );
  lang->add( gda, new psil::parser_t( "<datum>", "<boolean> | <number> | <character> | <symbol> | <list>" ) );
  lang->add( gda, new psil::parser_t( "<boolean>", "#t | #f" ) );
  lang->add( gda, new psil::parser_t( "<character>", "{^(#\\\\).(?!.)} | #\\newline | #\\space" ) );
  lang->add( gda, new psil::parser_t( "<symbol>", "<identifier>" ) );
  lang->add( gda, new psil::parser_t( "<list>", "() | (<datum>+)" ) );


  psil::group_t * gn = lang->add( gda, new psil::group_t( "NUMBERS" ) );
  lang->add( gn, new psil::parser_t( "<number>", "<integer> | <decimal>" ) );
  lang->add( gn, new psil::parser_t( "<integer>", "<pos_int> | <neg_int>" ) );
  lang->add( gn, new psil::parser_t( "<decimal>", "<pos_dec> | <neg_dec>" ) );
  lang->add( gn, new psil::parser_t( "<pos_int>", "{\\+\\d+(?!\\w)}" ) );
  lang->add( gn, new psil::parser_t( "<neg_int>", "{-\\d+(?!\\w)}" ) );
  lang->add( gn, new psil::parser_t( "<pos_dec>", "{\\+\\d+(?!\\w)\\.\\d+(?!\\w)}" ) );
  lang->add( gn, new psil::parser_t( "<neg_dec>", "{-\\d+(?!\\w)\\.\\d+(?!\\w)}" ) );

  return lang;
}
				   
