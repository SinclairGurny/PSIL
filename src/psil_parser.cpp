/** 
    psil_parser.cpp
    PSIL Parser implementation
    @author Sinclair Gurny
    @version 0.1
    July 2019
 */


#include "psil_parser.h"
using namespace psil;

// ============================================================================================================
// ===================== STRUCT IMPLEMENTATIONS ===============================================================
// ============================================================================================================

/**
   Adds group to group

   @param s - name of group
   @param g - group to be added
*/
void psil::group_t::add_group( std::string s, group_ptr g ) {
  lang_elem_ptr elem( new lang_elem_t( g ) );
  items.insert( std::make_pair(s, elem) );
}

/**
   Add parser to group
   
   @param s - name of parser
   @param p - parser to be added
*/
void psil::group_t::add_parser( std::string s, parser_ptr p ) {
  psil::lang_elem_ptr elem( new psil::lang_elem_t( p ) );
  items.insert( std::make_pair(s, elem) );
  p->lang->all_parsers.insert( std::make_pair(s, p) );
}

/**
   Add group to language
   
   @param s - name of group
   @param g - group to be added
*/
void psil::language_t::add_group( std::string s, group_ptr g ) {
  psil::lang_elem_ptr elem( new psil::lang_elem_t( g ) );
  items.insert( std::make_pair(s, elem) );
}


/**
   Add parser to language

   @param s - name of parser
   @param p - parser to be added
*/
void psil::language_t::add_parser( std::string s, parser_ptr p ) {
  psil::lang_elem_ptr elem( new psil::lang_elem_t( p ) );
  items.insert( std::make_pair(s, elem) );
  all_parsers.insert( std::make_pair(s, p) );
}

/**
   Find parser by name inside language

   @param pn - name of parser
   @return parser pointer if found, nullptr if not
*/
psil::parser_ptr psil::language_t::get_parser( std::string pn ) {
  auto ret_itr = all_parsers.find( pn );
  if ( ret_itr != all_parsers.end() ) {
    return ret_itr->second;
  }
  return nullptr;
}

/**
   Find all top level parsers inside language

   @return vector of parser pointers
*/
std::vector<psil::parser_ptr> psil::language_t::get_top_parsers() {
  std::vector<psil::parser_ptr> ret;
  for ( auto it : items ) {
    if ( it.second->type ) {
      ret.push_back( it.second->p );
    }
  }
  return ret;
}

// ============================================================================================================
// =============================== CONSTRUCTOR FUNCTIONS ======================================================


psil::parser_ptr psil::make_parser( psil::language_ptr l, std::string n, std::string rs ) {
  psil::parser_ptr p( new psil::parser_t() );
  p->lang = l;
  p->name = n;
  p->rules = std::vector<str_vec>();
  // split rule list into parts
  std::string rule;
  size_t loc;
  while ( (loc = rs.find("|")) != std::string::npos ) {
      rule = rs.substr(0, loc);
      rs.erase(0, loc+1);
      rule = std::regex_replace(rule, std::regex("^ +| +$|( ) +"), "$1");
      str_vec rvec = psil::tokenize_rule( rule );
      p->rules.push_back( rvec );
  }
  if ( rs.size() > 0 ) {
    rs = std::regex_replace( rs, std::regex("^ +| +$|( ) +"), "$1");
    str_vec rvec = psil::tokenize_rule( rs );
    p->rules.push_back( rvec );
  }
  return p;
}


psil::group_ptr psil::make_group( psil::language_ptr l, std::string n ) {
  psil::group_ptr g( new psil::group_t() );
  g->lang = l;
  g->name = n;
  g->items = std::map<std::string, psil::lang_elem_ptr >();
  return g;
}


psil::language_ptr psil::make_language( std::string n ) {
  psil::language_ptr l( new language_t() );
  l->name = n;
  l->items = std::map<std::string, lang_elem_ptr >();
  l->all_parsers = std::map<std::string, parser_ptr >();
  return l;
}


// =============================== DESTRUCTOR FUNCTIONS =======================================================


// === Deletes parser
void psil::delete_parser( psil::parser_ptr arg ) {
  arg.reset();
  //TODO?
}

// === Deletes group
void psil::delete_group( psil::group_ptr arg ) {
  for ( auto elem : arg->items ) {
    if ( elem.second->type ) {
      psil::delete_parser( elem.second->p );
      elem.second.reset();
    } else {
      delete_group( elem.second->g );
      elem.second.reset();
    }    
  }
  arg.reset();
}

// === Deletes language
void psil::delete_lang( psil::language_ptr lang ) {
  for ( auto elem : lang->items ) {
    if ( elem.second->type ) {
      psil::delete_parser( elem.second->p );
      elem.second.reset();
    } else {
      psil::delete_group( elem.second->g );
      elem.second.reset();
    }
  }

  for ( auto elem2 : lang->all_parsers ) {
    psil::delete_parser( elem2.second );
  }

  lang.reset();
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


// === Prints parser
void psil::parser_print( parser_ptr arg, int depth ) {
  std::cout << std::string(depth*2, ' ') << arg->name << std::endl;
  std::cout << std::string(depth*2+2, ' ');
  for ( auto rule : arg->rules ) {
    print_vec( rule, false );
    std::cout << " | ";
  }
  std::cout << std::endl;
}

// === Prints group
void psil::group_print( group_ptr arg, int depth ) {
  std::cout << std::string(depth*2, ' ') << arg->name << std::endl;
  for ( auto elem : arg->items ) {
    if ( elem.second->type ) {
      psil::parser_print( elem.second->p, depth+1 );
    } else {
      psil::group_print( elem.second->g, depth+1 );
    }
  }
}

// === Prints language
void psil::lang_print( language_ptr lang ) {
  std::cout << "Language: " << lang->name << " ";
  std::cout << lang->items.size() << " " << lang->all_parsers.size() << std::endl;
  for ( auto elem : lang->items ) {
    if ( elem.second->type ) {
      psil::parser_print( elem.second->p, 1 );
    } else {
      psil::group_print( elem.second->g, 1 );
    }
  }
  std::cout<<std::endl;
}


// === Prints token
void psil::tk_print( psil::token_ptr token ) {
  if ( !token ) return;

  std::vector<psil::token_elem_ptr > current;
  std::vector<psil::token_elem_ptr > next;

  psil::token_elem_ptr ptr( new token_elem_t( token ) );
  current.push_back(ptr);
  
  while ( current.size() > 0 ) {
    for ( auto elem : current ) {
      if ( elem->type ) { // string
	std::cout << "  Aspect:: " << elem->str;
      } else { // token
	std::cout << "  Tk:type: " << elem->tk->type;
	for ( auto n : elem->tk->aspects ) {
	  next.push_back(n);
	}
      }
    }
    std::cout << std::endl;
    current = next;
    next.clear();
  }

  std::cout << std::endl;
}

// ========================== Helping functions ==============================================================

// === Checks whether input is a <rule> that can be expanded
bool psil::is_rule( std::string input ) {
  if ( input.size() >= 3 ) {
    if ( input[0] == '<' && input[input.size()-1] == '>' ) {
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
      if ( memory.back() == '(' ) {
	memory.pop_back();
      } else {
	return false;
      }
    }
    if ( c == ']' ) {
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
    if ( rule[r] == '('  ) {
      // find closing paren
      auto mp_ret = psil::match_parens( r, rule );
      if ( mp_ret.second.size() == 0 ) {
	auto tmp_vec = psil::tokenize_rule( rule.substr( r+1, mp_ret.first-r-1) );
	ret.push_back(rule.substr(r,1));
	ret.insert( ret.end(), tmp_vec.begin(), tmp_vec.end() );
	ret.push_back(rule.substr(mp_ret.first));
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
	if ( loc+1 < rule.size() && (rule[loc+1] == '*' || rule[loc+1] == '+') ) { x=1; }
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
psil::token_ptr psil::match_rule( psil::language_ptr lang, std::string pn, psil::str_vec rule,
				  psil::str_vec input, psil::upoint pt, bool& match) {
  
  psil::token_ptr tptr( new token_t() );
  tptr->type = pn;
  
  size_t in_itr = pt.first;
  size_t ru_itr = 0;
  for ( ; in_itr <= pt.second && ru_itr < rule.size(); ) {
    // Match parens
    if ( rule[ru_itr] == "(" || rule[ru_itr]  == ")" ||
	 rule[ru_itr] == "[" || rule[ru_itr]  == "]" ) {
      if ( rule[ru_itr] == input[in_itr] ) {
	#ifdef DEBUG_MODE
	std::cout << "MATCHED PAREN" << pn << std::endl;
	#endif
 	// Add result to token
	token_elem_ptr tke( new token_elem_t( input[in_itr] ) );
	tptr->aspects.push_back( tke );
	
	++ru_itr; ++in_itr;
	continue;
      } else {
	std::cout << "Exit PAREN" << pn << std::endl;
	return nullptr;
      }
    }
    // Check for expansion
    else if ( psil::is_rule( rule[ru_itr] ) ) {
      psil::parser_ptr next_p = lang->get_parser( rule[ru_itr] );
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
	    return nullptr;
	  }
	} else {
	  new_pt = std::make_pair( in_itr, in_itr );
	}
	#ifdef DEBUG_MODE
	std::cout << "RECURSION" << pn << "->" << next_p->name << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	#endif
	auto new_ret = psil::apply_parser( lang, next_p, input, new_pt, new_match );
	if ( new_match ) {
	  // Add result to current token
	  token_elem_ptr tke( new token_elem_t( new_ret ) );
	  tptr->aspects.push_back( tke );

	  ++ru_itr; in_itr = new_pt.second+1;
	  continue;
	} else {
	  #ifdef DEBUG_MODE
	  std::cout << "Exit No recursion match" << pn << std::endl;
	  #endif
	  return nullptr;
	}
      } else {
        #ifdef DEBUG_MODE
	std::cout << "Exit No rule found" << pn << std::endl;
	#endif
	return nullptr;
      }
    }
    // Check for regex
    else if ( psil::is_regex( rule[ru_itr] ) ) {
      if ( psil::match_regex( rule[ru_itr], input[in_itr] ) ) {
        #ifdef DEBUG_MODE
	std::cout << "REGEX_MATCH" << pn << std::endl;
	#endif
	token_elem_ptr tke( new token_elem_t( input[in_itr] ) );
	tptr->aspects.push_back( tke );
	++ru_itr; ++in_itr;
      } else {
        #ifdef DEBUG_MODE
	std::cout << "Exit no regex match" << pn << std::endl;
	#endif
	return nullptr;
      }
    }
    // Check for exactness
    else if ( rule[ru_itr] == input[in_itr] ) {
      #ifdef DEBUG_MODE
      std::cout << "EXACT" << pn << std::endl;
      #endif
      // Add result to current token
      token_elem_ptr tke( new token_elem_t( input[in_itr] ) );
      tptr->aspects.push_back( tke );
      ++ru_itr; ++in_itr;
      continue;
    } else {
      #ifdef DEBUG_MODE
      std::cout << "Exit Not exact" << pn << std::endl;
      #endif
      ++ru_itr;
      return nullptr;
    }
    
  }
  
  //delete_token( tptr );
  match = true;
  return tptr;
}

// === Try all rules of a parser
psil::token_ptr psil::apply_parser( psil::language_ptr lang, psil::parser_ptr par,
				    psil::str_vec input_tks, psil::upoint pt, bool& match ) {
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
psil::token_ptr psil::parse( psil::language_ptr lang, std::string input ) {

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
psil::language_ptr psil::make_psil_lang() {
  psil::language_ptr lang( psil::make_language( "PSIL" ) );

  lang->add_parser( "<program>", psil::make_parser( lang, "<program>", "<form>" ) );
  psil::group_ptr gf = psil::make_group( lang, "FORMS" );
  lang->add_group( "FORMS", gf );
  gf->add_parser( "<form>", psil::make_parser( lang, "<form>", "<var_def> | <expression>" ) );
  
  psil::group_ptr gd = psil::make_group( lang, "DEFINITIONS" );
  lang->add_group( "DEFINITIONS", gd );
  gd->add_parser( "<var_def>", psil::make_parser( lang, "<var_def>", "(define <variable> <expression>)" ) );
  gd->add_parser( "<variable>", psil::make_parser( lang, "<variable>", "<identifier>") );

  psil::group_ptr ge = psil::make_group( lang, "EXPRESSIONS" );
  lang->add_group( "EXPRESSIONS", ge );
  ge->add_parser( "<expression>", psil::make_parser( lang, "<expression>",
						     "<constant> | <variable> | <application>") );
  ge->add_parser( "<constant>", psil::make_parser( lang, "<constant>", "<boolean> | <number> | <character>") );
  ge->add_parser( "<application>", psil::make_parser( lang, "<application>", "(<expression> <expression>)") );
  
  psil::group_ptr gi = psil::make_group( lang, "IDENTIFIERS" );
  lang->add_group( "IDENTIFIERS", gi );
  gi->add_parser( "<identifier>", psil::make_parser( lang, "<identifier>",
		  "{^[a-zA-Z_](?!.)} | <operator> | {^[a-zA-Z_][a-zA-Z_\\!]+(?!.)}") );
  gi->add_parser( "<operator>", psil::make_parser( lang, "<operator>", "+ | - | * | \\ | %") );
  
  psil::group_ptr gda = psil::make_group( lang, "DATA" );
  lang->add_group( "DATA", gda );
  gda->add_parser( "<boolean>", psil::make_parser( lang, "<boolean>", "#t | #f") );
  gda->add_parser( "<number>", psil::make_parser( lang, "<number>", "<integer> | <decimal>") );
  gda->add_parser( "<integer>", psil::make_parser( lang, "<integer>", "<positive_num> | <negative_num>") );
  gda->add_parser( "<decimal>", psil::make_parser( lang, "<decimal>", "<positive_dec> | <negative_dec>") );
  gda->add_parser( "<positive_num>", psil::make_parser( lang, "<positive_num>", "{\\+\\d+(?!\\w)}") );
  gda->add_parser( "<negative_num>", psil::make_parser( lang, "<negative_num>", "{-\\d+(?!\\w)}") );
  gda->add_parser( "<positive_dec>", psil::make_parser( lang, "<positive_dec>", "{\\+\\d+(?!\\w)\\.\\d+(?!\\w)}") );
  gda->add_parser( "<negative_dec>", psil::make_parser( lang, "<negative_dec>", "{-\\d+(?!\\w)\\.\\d+(?!\\w)}") );
  gda->add_parser( "<character>", psil::make_parser( lang, "<character>",
						     "{^(#\\\\).(?!.)} | #\\newline | #\\space") );


  
  return lang;
}
				   
