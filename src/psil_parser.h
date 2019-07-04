/**
   psil_parser.h
   PSIL Parser libary 
   @author Sinclair Gurny
   @version 0.1
   July 2019
*/

#ifndef __PSIL_H__
#define __PSIL_H__

#include <iostream>
#include <string>
#include <cctype>
#include <memory>
#include <regex>
#include <vector>
#include <map>

#ifdef DEBUG_MODE
#include <chrono>
#include <thread>
#endif

namespace psil {

  // ===== Struct forward declarations =====

  struct token_st;
  struct token_elem_st;
  struct language_st;
  struct lang_elem_st;
  struct parser_st;
  struct group_st;

  // ====== Typedefs =============

  // === Shortening of struct names
  typedef struct token_st token_t;
  typedef struct token_elem_st token_elem_t;
  typedef struct language_st language_t;
  typedef struct lang_elem_st lang_elem_t;
  typedef struct parser_st parser_t;
  typedef struct group_st group_t;

  // === Typedef of struct pointers
  typedef std::shared_ptr<token_t> token_ptr;
  typedef std::shared_ptr<token_elem_t> token_elem_ptr;
  typedef std::shared_ptr<parser_t> parser_ptr;
  typedef std::shared_ptr<group_t> group_ptr;
  typedef std::shared_ptr<lang_elem_t> lang_elem_ptr;
  typedef std::shared_ptr<language_t> language_ptr;

  // === Other typedefs
  typedef std::vector<std::string> str_vec;
  typedef std::pair<size_t, size_t> upoint;
  
  // ===== Token =======================================

  /**
     Token Element
     Holds either a string or another token
     Is used in the aspects vector in the token struct
     type: true - object holds a string, false - object holds a token
   */
  struct token_elem_st {
    token_elem_st( std::string s ) : type(true), str(s) {}
    token_elem_st( token_ptr p ) : type(false), tk(p) {}
    
    bool type;
    std::string str;
    token_ptr tk;
  };

  /**
     Token
     Represents the nodes in the abstract syntax tree
  */
  struct token_st {
    std::string type;
    std::vector<token_elem_ptr > aspects;
  };


  // ===== Parser =======================================
  
  /**
     Parser
     Holds list of rules for each named leaf 
       category of the language tree
  */
  struct parser_st {
    language_ptr lang;
    std::string name;
    std::vector<str_vec> rules;
  };

  // ===== Parser Group =================================

  /**
     Group
     Used to organize language structure
     Holds map of language element pointers
  */
  struct group_st {
    std::shared_ptr<language_t> lang;
    std::string name;
    std::map<std::string, lang_elem_ptr > items;

    void add_group( std::string s, group_ptr g );
    void add_parser( std::string s, parser_ptr p );
  };

  // ===== Parser Language ==============================

  /**
     Language Element
     Holds either a parser or a group
     Used in items map of language struct and group struct

     type - true -> holds parser, false -> holds group
  */
  struct lang_elem_st {
    lang_elem_st( parser_ptr _p ) : type(true), p(_p) {}
    lang_elem_st( group_ptr _g ) : type(false), g(_g) {}
    bool type;
    parser_ptr p;
    group_ptr g;
  };

  /**
     Language
     Holds full structure of language
     items is used to mantain structure
     all_parsers is used for fast parser lookup
  */
  struct language_st {
    std::string name;
    std::map<std::string, lang_elem_ptr > items;
    std::map<std::string, parser_ptr > all_parsers;

    void add_group( std::string s, group_ptr g );
    void add_parser( std::string s, parser_ptr p );

    parser_ptr get_parser( std::string pn );

    std::vector<parser_ptr> get_top_parsers();
  };

  // ============================================================================================================
  // =============================== FUNCTIONS ==================================================================
  // ============================================================================================================


  // Constructor functions
  /**
     Creates parser and updates language object

     @param l - language object that gets updated
     @param n - name of parser
     @param rs - rule string
     @return pointer to new parser
  */
  parser_ptr make_parser( language_ptr l, std::string n, std::string rs );

  /**
     Creates parser and updates language object

     @param l - language object that gets updated
     @param n - name of group
     @return pointer to new group
  */
  group_ptr make_group( language_ptr l, std::string n );

  /**
     Creates parser and updates language object

     @param l - pointer to language object
     @param n - name of parser
     @param rs - rule string
     @return pointer to new parser
  */
  language_ptr make_language( std::string n );


  // === Destructor functions ===

  /**
     Deallocate parser memory
     
     @param parser to be deleted
  */
  void delete_parser( parser_ptr arg );

  /**
     Deallocate group memory
     
     @param group to be deleted
  */
  void delete_group( group_ptr arg );

  /**
     Deallocate language memory
     
     @param language to be deleted
  */
  void delete_lang( language_ptr lang );
  
  // ========================== Printing functions ==============================================================

  /**
     Print out contents of vector of strings
     
     @param v - vector to be printed
     @param newline - default to true, whether to add newline after printing
  */
  void print_vec( const std::vector<std::string> &v, bool newline );

  /**
    Print entire contents of language

    @param lang - language to be printed
  */
  void lang_print( language_ptr lang );

  /**
    Print entire contents of parser
    
    @param arg - parser to be printed
    @depth - how many layers deep is the print call stack
              used to tabulate things properly
  */
  void parser_print( parser_ptr arg, int depth );

  /**
     Print entire contents of group

     @param arg - group to be printed
     @depth - how many layers deep is the print call stack
              used to tabulate things properly
  */
  void group_print( group_ptr arg, int depth );

  /**
     Print contents of abstract syntax tree
     constructed from parsing user input

     @param tokens - pointer to top level token
  */
  void tk_print( token_ptr tokens );

  // ========================== Helping functions ==============================================================

  /**
     Checks if the input is a rule that can be expanded
     within angle brackets ex: <name>

     @param input - string to be checked
     @return whether the input string is a rule
  */
  bool is_rule( std::string input );

  /**
     Checks if the input is a regex expression
     within curly braces ex: {[a-z]}

     @param input - string to be checked
     @return whether the input string is a regex
  */
  bool is_regex( std::string input );

  /**
     Takes if the input matches the regex expression

     @param expression - string that has been proven to be a regex
     @param input - string to be matched against the regex
     @return whether the regex matches the entire input string
  */
  bool match_regex( std::string expression, std::string input );

  /**
     Verifies the entire input string has matching parens

     @param input - string to be checked
     @return whether the input string has matching parens
  */
  bool check_parens( std::string input );

  /**
     Find next matching paren of same type
     
     @param start - starting index of open paren
     @param input - structure to be searched
     @return pair of size_t a and string b
     size_t a - location if found, undefined if not
     string b - error message if not found, "" if found
  */
  std::pair<size_t, std::string> match_parens( size_t start, std::string input );
  std::pair<size_t, std::string> match_parens( size_t start, str_vec input );

  // ========================== Parsing functions ==============================================================

  /**
     Break parser's rule into tokens

     @param rule - rule to be tokenized
     @return vector of tokens, written as strings
  */
  std::vector<std::string> tokenize_rule( std::string rule );

  /**
     Break user input string into tokens

     @param input - string to be tokenized
     @return vector of tokens, written as string
  */
  std::vector<std::string> tokenize_input( std::string input );

  /**
     Convert user's tokenized input to tokens
     by using rules from the language
     
     @param lang - pointer to language being used to parse
     @param pn - name of parser
     @param rule - tokenized rule, vector of strings
     @param input - tokenized user input, vector of strings
     @param pt - pair of size_t values that represents starting and ending (inclusive) 
                 locations of where function is parsing in input
     @param match - pass by reference flag to return when a proper match has been found
     @return pointer to abstract syntax tree if success, otherwise nullptr
  */
  token_ptr match_rule( language_ptr lang, std::string pn, str_vec rule,
			str_vec input, upoint pt, bool& match);

  /**
     Calls all rules of a parser
     
     @param lang - pointer to language being used to parse
     @param pare - name of parser
     @param input_tks - tokenized user input, vector of strings
     @param pt - pair of size_t values that represents starting and ending (inclusive) 
                 locations of where function is parsing in input
     @param match - pass by reference flag to return when a proper match has been found
     @return pointer to abstract syntax tree if success, otherwise nullptr
  */
  token_ptr apply_parser( language_ptr lang, parser_ptr par,
			  str_vec input_tks, upoint pt, bool& match );

  /**
     Primary parsing driver function
     Tokenizes input and calls apply_parser on all top level parsers

     @param lang - pointer to language being used to parse
     @param input - string of user input
     @return pointer to abstract syntax tree if success, otherwise nullptr
  */
  token_ptr parse( language_ptr lang, std::string input );
  
  // ========================== Premade functionality ===========================================================

  /**
     Creates PSIL language

     @returns pointer to language object
  */
  language_ptr make_psil_lang();
  
} // end of psil namespace

#endif
