/** 
    psil_parser.cpp
    PSIL Parser implementation
    @author Sinclair Gurny
    @version 0.5
    July 2019
 */


#include "psil_parser.h"


namespace psil_parser {
  // =========================================================================================================
  // ===================== STRUCT IMPLEMENTATIONS ============================================================
  // =========================================================================================================
  
  // ========== Token ==================================================
  
  // === Prints token with breadth-first search
  void token_t::print() {
    std::cout << std::endl;
    std::vector<const token_elem_t * > current;
    std::vector<const token_elem_t * > next;

    //const token_elem_t * tmp = new token_elem_t( this );
    current.push_back( new token_elem_t( this ) );
  
    while ( current.size() > 0 ) {
      for ( auto elem : current ) {
	if ( elem->elem_type == token_elem_t::TE_Type::STRING) { // string
	  std::cout << "  Aspect:: " << elem->str;
	} else { // token
	  std::cout << "  Tk:type: " << elem->tk->type_name;
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
  parser_t::parser_t( std::string n, std::string rs ) :  name(n) {
    // split rule list into parts
    std::string rule;
    size_t loc;
    while ( (loc = rs.find("|")) != std::string::npos ) {
      rule = rs.substr(0, loc);
      rs.erase(0, loc+1);
      rule = std::regex_replace(rule, std::regex("^ +| +$|( ) +"), "$1");
      if ( !check_parens( rule ) ) { throw std::string( "Error: mismatching parens" ); }
      str_vec rvec = tokenize_rule( rule );
      rules.push_back( rvec );
    }
    if ( rs.size() > 0 ) {
      rs = std::regex_replace( rs, std::regex("^ +| +$|( ) +"), "$1");
      if ( !check_parens( rs ) ) { throw std::string( "Error: mismatching parens" ); }
      str_vec rvec = tokenize_rule( rs );
      rules.push_back( rvec );
    }
  }


  // === Prints parser
  void parser_t::print( int depth = 1 ) const {
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
  void group_t::print( int depth = 1 ) const {
    std::cout << std::string(depth*2, ' ') << name << std::endl;
    for ( auto elem = items.begin(); elem != items.end(); ++elem ) {
      if ( (*elem)->elem_type == lang_elem_t::LE_Type::PARSER) {
	(*elem)->p->print( depth + 1 );
      } else {
	(*elem)->g->print( depth + 1 );
      }
    }
  }

  // ========== Language ===========================================

  // === Adds group to language
  group_t * language_t::add( group_t * g ) {
    std::unique_ptr<lang_elem_t> elem( new lang_elem_t( g ) );
    this->items.push_back( std::move(elem) );

    return g;
  }

  // === Adds parser to lanuage
  parser_t * language_t::add( parser_t * p ) {
    auto ret = all_parsers.insert( std::make_pair( p->name, p ) );
    if ( !ret.second ) {
      std::string str( "Error: Parser of same name already in language" );
      throw str;
    }

    std::unique_ptr<lang_elem_t> elem( new lang_elem_t( p ) );
    this->items.push_back( std::move(elem) );

    return p;
  }

  // Add parser to group
  parser_t * language_t::add( group_t * g, parser_t * p ) {
    auto ret = all_parsers.insert( std::make_pair( p->name, p ) );
    if ( !ret.second ) {
      std::string str( "Error: Parser of same name already in language" );
      throw str;
    }
  
    std::unique_ptr<lang_elem_t> elem( new lang_elem_t( p ) );
    g->items.push_back( std::move(elem) );

    return p;
  }

  // Add group to group
  group_t * language_t::add( group_t * g_up, group_t * g_down ) {
    std::unique_ptr<lang_elem_t> elem( new lang_elem_t( g_down ) );
    g_up->items.push_back( std::move(elem) );

    return g_down;
  }

  /**
     Find parser by name inside language

     @param pn - name of parser
     @return parser pointer if found, nullptr if not
  */
  parser_t * language_t::get_parser( std::string pn ) const {
    size_t len = pn.size();
    std::string name = ( pn[len-1] == '+' || pn[len-1] == '*' ) ? pn.substr(0, pn.size()-1) : pn;
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
  std::vector<parser_t * > language_t::get_top_parsers() const {
  
    std::vector<parser_t * > ret;
    for ( auto it = items.begin(); it != items.end(); ++it ) {
      if ( (*it)->elem_type == lang_elem_t::LE_Type::PARSER ) {
	ret.push_back( (*it)->p );
      }
    }
    return ret;
  }


  // === Prints language
  void language_t::print() const {
    std::cout << "Language: " << name << " ";
    std::cout << items.size() << " " << all_parsers.size() << std::endl;
    for ( auto elem = items.begin(); elem != items.end(); ++elem ) {
      if ( (*elem)->elem_type == lang_elem_t::LE_Type::PARSER) {
	(*elem)->p->print();
      } else {
	(*elem)->g->print();
      }
    }
    std::cout<<std::endl;
  }


  // ========================== Printing functions =======================================================


  // === Prints vector of strings
  void print_vec( const std::vector<std::string> &v, bool newline = true ) {
    for ( size_t i = 0; i < v.size(); ++i ) {
      if ( i!= 0 ) { std::cout << " "; }
      std::cout << v[i];
    }
    if ( newline ) { std::cout << std::endl; }
  }



  // ========================== Helping functions =======================================================

  // === Checks whether input is a <rule> that can be expanded
  bool is_rule( std::string input ) {
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
  bool is_regex( std::string input ) {
    if ( input.size() >= 3 ) {
      if ( input[0] == '{' && input[input.size()-1] == '}' ) {
	return true;
      }
    }
    return false;
  }

  // === Runs regex expression and returns whether it matches
  bool match_regex( std::string expression, std::string input ) {
    std::string expr = expression.substr(1,expression.size()-2);
    std::regex re( expr );
    return std::regex_match( input, re );
  }

  // === Verify entire string has matching parens
  bool check_parens( std::string input ) {
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
  std::pair<size_t, std::string> match_parens( size_t start, std::string input ) {
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
  std::pair<size_t, std::string> match_parens( size_t start, str_vec input ) {
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


  // =========================================================================================================
  // =============================== PARSING FUNCTIONS =======================================================
  // =========================================================================================================

  // === Split rule into tokens
  std::vector<std::string> tokenize_rule( std::string rule ) {
    std::vector<std::string> ret;
    size_t loc = 0;
  
    for ( size_t r = 0; r < rule.size(); ++r ) {
      if ( rule[r] == '(' ) {
	// find closing paren
	auto mp_ret = match_parens( r, rule );
	if ( mp_ret.second.size() == 0 ) {
	  auto tmp_vec = tokenize_rule( rule.substr( r+1, mp_ret.first-r-1) );
	  ret.push_back(rule.substr(r,1));
	  ret.insert( ret.end(), tmp_vec.begin(), tmp_vec.end() );
	  ret.push_back(rule.substr(mp_ret.first, 1));
	  r = mp_ret.first;
	} else { // Error occurred
	  std::cerr << "Error while parsing rule\n";
	  std::cerr << "Error: " << mp_ret.second;
	  std::cerr << "Rule: " << rule << std::endl;
	  ret.clear();
	  return ret;
	}
      } else if ( rule[r] == '<' ) {
	// find closing bracket and tokenize
	if ( (loc = rule.find_first_of(">", r)) != std::string::npos ) {
	  int x = 0;
	  if ( loc+1 < rule.size() && (rule[loc+1] == '+' || rule[loc+1] == '*') ) { x=1; }
	  std::string tmp = rule.substr( r, loc-r+x+1 );
	  ret.push_back( tmp );
	  r = loc+x;
	} else { // Error occurred
	  std::cerr << "Error while parsing <> in rule\n";
	  std::cerr << "Rule: " << rule << std::endl;
	  ret.clear();
	  return ret;
	}
      } else if ( rule[r] == '{' ) {
	// complete regex
	loc = r+1;
	for ( ; loc < rule.size(); ++loc ) {
	  if ( rule[loc] == '}' ) {
	    ++loc;
	    break;
	  }
	}
	if ( loc < rule.size() ) {
	  std::string tmp = rule.substr( r, loc-r );
	  ret.push_back( tmp );
	  r = loc;
	} else {
	  std::string tmp = rule.substr( r );
	  ret.push_back( tmp );
	  r = rule.size();
	}
      } else if ( rule[r] != ' ' ) {
	// find next space and tokenize
	loc = r+1;
	for ( ; loc < rule.size(); ++loc ) {
	  if ( rule[loc] == ' ' || rule[loc] == '<' || rule[loc] == '('
	       || rule[loc] == '[' || rule[loc] == ')' || rule[loc] == ']'
	       || rule[loc] == '{' || rule[loc] == '}' ) {
	    break;
	  }
	}
	if ( loc < rule.size() ) {
	  std::string tmp = rule.substr( r, loc-r );
	  ret.push_back( tmp );
	  r = loc-1;
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
  std::vector<std::string> tokenize_input( std::string input ) {
    std::vector<std::string> ret;
    size_t loc = 0;
  
    for ( size_t i = 0; i < input.size(); ++i ) {
      if ( input[i] == '('  ) {
	// find closing paren
	auto mp_ret = match_parens( i, input );
	if ( mp_ret.second.size() == 0 ) {
	  auto tmp_vec = tokenize_input( input.substr( i+1, mp_ret.first-i-1) );
	  ret.push_back(input.substr(i, 1));
	  ret.insert( ret.end(), tmp_vec.begin(), tmp_vec.end() );
	  ret.push_back(input.substr(mp_ret.first,1));
	  i = mp_ret.first;
	} else { // Error occurred
	  std::cerr << "Error while parsing input\n";
	  std::cerr << "Error: " << mp_ret.second;
	  std::cerr << "Input: " << input << std::endl;
	  ret.clear();
	  return ret;
	}
      } else if ( input[i] != ' ' ) {
	// find next space and tokenize
	loc = i+1;
	for ( ; loc < input.size(); ++loc ) {
	  if ( input[loc] == ' ' || input[loc] == '('
	       || input[loc] == '[' || input[loc] == ')' || input[loc] == ']' ) {
	    break;
	  }
	}

	if ( loc != input.size() ) {
	  std::string tmp = input.substr( i, loc-i );
	  ret.push_back( tmp );
	  i = loc-1;
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
  std::unique_ptr<token_t> match_rule( const std::unique_ptr<language_t> & lang,
				       std::string pn, str_vec rule,
				       str_vec input, upoint pt, bool& match) {
  
    std::unique_ptr<token_t> tptr( new token_t( pn ) );
    size_t in_itr = pt.first;
    size_t ru_itr = 0;
    bool try_again = false;
  
    for ( ; in_itr <= pt.second && ru_itr < rule.size(); ) {
      // Match parens
      if ( rule[ru_itr] == "(" || rule[ru_itr]  == ")" ||
	   rule[ru_itr] == "[" || rule[ru_itr]  == "]" ) {
	if ( rule[ru_itr] == input[in_itr] ) {
#ifdef DEBUG_MODE
	  std::cerr << "MATCHED PAREN" << pn << std::endl;
#endif
	  // Add result to token
	  std::unique_ptr<token_elem_t> tke( new token_elem_t( input[in_itr] ) );
	  tptr->aspects.push_back( std::move(tke) );
	
	  ++ru_itr; ++in_itr;
	  continue;
	} else {
#ifdef DEBUG_MODE
	  std::cerr << "Exit PAREN" << pn << std::endl;
#endif
	  return nullptr;
	}
      }
      // Check for expansion
      else if ( is_rule( rule[ru_itr] ) ) {
	parser_t * next_p( lang->get_parser( rule[ru_itr] ) );
	if ( next_p != nullptr ) {
	  bool new_match = false;
	  upoint new_pt;
	  if ( input[in_itr] == "(" ) {
	    auto p_loc = match_parens( in_itr, input );
	    if ( p_loc.second.size() == 0 ) {
#ifdef DEBUG_MODE
	      std::cerr << "Broke" << in_itr << " " << p_loc.first << pn <<std::endl;
#endif
	      new_pt = std::make_pair( in_itr, p_loc.first );
	    } else {
#ifdef DEBUG_MODE
	      std::cerr << "Exit Could not find matching paren" << pn << std::endl;
#endif
	      return nullptr;
	    }
	  } else {
	    new_pt = std::make_pair( in_itr, in_itr );
	  }
	
#ifdef DEBUG_MODE
	  std::cerr << "RECURSION" << pn << "->" << next_p->name << std::endl;
#endif
	  auto new_ret = apply_parser( lang, next_p, input, new_pt, new_match );

	  if ( rule[ru_itr].back() == '*' ) {
	    try_again = true;
	  }
	  
	  if ( new_match ) {
	    // Add result to current token
	    std::unique_ptr<token_elem_t> tke( new token_elem_t( std::move(new_ret) ) );
	    tptr->aspects.push_back( std::move(tke) );

	    if ( rule[ru_itr].back() == '+' ) {
	      try_again = true;
	    }

	    if ( !try_again ) {
	      ++ru_itr;
	    }
	  
	    in_itr = new_pt.second+1;
	    continue;
	  } else {
#ifdef DEBUG_MODE
	    std::cerr << "Exit No recursion match" << pn << std::endl;
#endif
	    if ( try_again ) {
	      try_again = false;
	      ++ru_itr;
	      continue;
	    }
	    return nullptr;
	  }
	} else {
#ifdef DEBUG_MODE
	  std::cerr << "Exit No rule found" << pn << std::endl;
#endif
	  throw std::string("Error: Rule not found" + rule[ru_itr]);
	  return nullptr;
	}
      }
      // Check for regex
      else if ( is_regex( rule[ru_itr] ) ) {
	if ( match_regex( rule[ru_itr], input[in_itr] ) ) {
#ifdef DEBUG_MODE
	  std::cerr << "REGEX_MATCH" << pn << std::endl;
#endif
	  std::unique_ptr<token_elem_t> tke( new token_elem_t( input[in_itr] ) );
	  tptr->aspects.push_back( std::move(tke) );
	  ++ru_itr; ++in_itr;
	} else {
#ifdef DEBUG_MODE
	  std::cerr << "Exit no regex match" << pn << std::endl;
#endif
	  return nullptr;
	}
      }
      // Check for exactness
      else if ( rule[ru_itr] == input[in_itr] ) {
#ifdef DEBUG_MODE
	std::cerr << "EXACT" << pn << std::endl;
#endif
	// Add result to current token
	std::unique_ptr<token_elem_t> tke( new token_elem_t( input[in_itr] ) );
	tptr->aspects.push_back( std::move(tke) );
	++ru_itr; ++in_itr;
	continue;
      } else {
#ifdef DEBUG_MODE
	std::cerr << "Exit Not exact" << pn << std::endl;
#endif
	return nullptr;
      }
    
    }
  
    match = true;
    return tptr;
  }

  // === Try all rules of a parser
  std::unique_ptr<token_t>
  apply_parser( const std::unique_ptr<language_t> & lang,
		parser_t * par, str_vec input_tks,
		upoint pt, bool& match ) {
#ifdef DEBUG_MODE
    std::cerr << "APPLYING PARSER" << par->name << std::endl;
#endif
    bool new_match = false;
    for ( str_vec rule : par->rules ) {
      auto ret = match_rule( lang, par->name, rule, input_tks, pt, new_match );
      if ( new_match ) { match = true; return ret; }
    }
    return nullptr;
  }

  // === Parsing driver function
  std::unique_ptr<token_t>
  parse( const std::unique_ptr<language_t> & lang, std::string input ) {

    if ( input.size() == 0 ) {
      std::cerr << "Empty input" << std::endl;
      return nullptr;
    }
    if ( !check_parens( input ) ) {
      std::cerr << "Mismatching parens" << std::endl;
      return nullptr;
    }

    bool match = false;
    // === Split input into tokens ===
    str_vec input_tokens = tokenize_input( input );
#ifdef DEBUG_MODE
    print_vec( input_tokens );
#endif
    upoint pt = std::make_pair( 0, input_tokens.size()-1 );

    try {
      for ( auto parser : lang->get_top_parsers() ) { // reduce to top level parsers
	auto ret = apply_parser( lang, parser, input_tokens, pt, match );
	if ( match ) { return ret; }
      }
    } catch ( std::string exp ) {
      std::cerr << "Error: While parsing input" << std::endl;
      std::cerr << exp << std::endl;
    }
    return nullptr;
  }
				   

  // ========================== Premade functionality =======================================================

  // === Make PSIL language
  std::unique_ptr<language_t> make_psil_lang() {
    try {
      std::unique_ptr<language_t> lang( new language_t( "PSIL" ) );
    
      lang->add( new parser_t( "<program>", "<form>" ) );
  
      group_t * gf = lang->add( new group_t( "FORMS" ) );
      lang->add( gf, new parser_t( "<form>", "(begin <form>+) | <definition> | <expression>" ) );
  
      group_t * gd = lang->add( new group_t( "DEFINITIONS" ) );
      lang->add( gd, new parser_t( "<definition>",
				   "(define <variable> <expression>)"
				   "| (update <variable> <expression>)" ) );
      lang->add( gd, new parser_t( "<variable>", "<identifier>" ) );
      
      group_t * ge = lang->add( new group_t( "EXPRESSIONS" ) );
      lang->add( ge, new parser_t( "<expression>",
				   "(begin <definition>* <expression>+) | <constant> | <variable>"
				   "| <lambda> | <conditional> | <application>" ) );
	
      lang->add( ge, new parser_t( "<constant>", "<boolean> | <number> | <character> | <list_def>" ) );

      lang->add( ge, new parser_t( "<lambda>", "(lambda <formals> <body>)" ) );
      lang->add( ge, new parser_t( "<formals>", "(<variable>*)" ) );
      lang->add( gd, new parser_t( "<body>", "<expression>") );
	    
      lang->add( ge, new parser_t( "<conditional>",
				   "(cond <expression> <expression>)"
				   "| (if <expression> <expression> <expression>)") );
      lang->add( ge, new parser_t( "<application>", "(<expression>+)" ) );
		 
      group_t * gi = lang->add( new group_t( "IDENTIFIERS" ) );
      lang->add( gi, new parser_t( "<identifier>",
				   "<keyword> | <operator> | {^[a-zA-Z_](?!.)}"
				   "| {^[a-zA-Z_][a-zA-Z_0-9\\!]+(?!.)}" ) );
      lang->add( gi, new parser_t( "<operator>", "+ | - | * | /" ) );
      lang->add( gi, new parser_t( "<keyword>",
				   "define | update | lambda | if | cond | begin | length"
				   "| and | or | not | equal? | floor | ceil | trunc | round"
				   "| zero? | first | second | nth | first! | second! | nth!"
				   "| null? | ch_lt | ch_lte | ch_gt | ch_gte | ch_eq"
				   "| lt | lte | gt | gte | eq | append | insert | pop"
				   "| boolean? | number? | char? | symbol? | proc? | list?"
				   "| abs | mod | print | println | read | quote | unquote" ) );
    
      group_t * gda = lang->add( new group_t( "DATA" ) );
      lang->add( gda, new parser_t( "<list_def>", "(quote <datum>)" ) );
      lang->add( gda, new parser_t( "<datum>",
				    "<boolean> | <number> | <character> | <symbol> | <list>" ) );
      lang->add( gda, new parser_t( "<boolean>", "#t | #f" ) );
      lang->add( gda, new parser_t( "<character>",
				    "{^(#\\\\).(?!.)} | #\\newline | #\\space | #\\tab"
				    "| #\\oparen | #\\cparen | #\\osqbrac | #\\csqbrac" ) );
      lang->add( gda, new parser_t( "<symbol>", "<identifier>" ) );
      lang->add( gda, new parser_t( "<list>", "() | (<datum>+)" ) );
    
      group_t * gn = lang->add( gda, new group_t( "NUMBERS" ) );
      lang->add( gn, new parser_t( "<number>", "<integer> | <decimal>" ) );
      lang->add( gn, new parser_t( "<integer>", "0 | {\\d+(?!\\w)} | {-\\d+(?!\\w)}" ) );
      lang->add( gn, new parser_t( "<decimal>",
				   "0.0 | {\\d+(?!\\w)\\.\\d+(?!\\w)}"
				   "| {-\\d+(?!\\w)\\.\\d+(?!\\w)}" ) );
      return lang;
    } catch ( std::string exp ) {
      std::cerr << "Error: While creating language" << std::endl;
      std::cerr << exp << std::endl;
      exit(1);
    }
    return nullptr;
  }
				   
}
