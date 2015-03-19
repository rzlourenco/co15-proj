#ifndef __PWN_FUNCTIONNODE_H__
#define __PWN_FUNCTIONNODE_H__

#include "cdk/ast/basic_node.h"
#include "cdk/ast/sequence_node.h"
#include "cdk/basic_type.h"
#include <vector>
#include <string>


namespace pwn {

  /**
   * Class for describing a function declaration (ex: #argc(), #fibonacci(#n) ) and possible implementation
   */
  class function_node: public cdk::basic_node {
    bool _import;
    basic_type *_return_type;
    std::string _function;
    cdk::expression_node *_default_return; /*default return value if it exists. it shouldn't be implemented as an expression, correct later*/
    cdk::sequence_node *_parameters; /*empty for functions without arguments*/
    cdk::sequence_node *_body; /*optional for declaration*/

  public:
    inline function_node(int lineno, basic_type *return_type, bool import, const std::string *function, cdk::expression_node *default_return, cdk::sequence_node *parameters, cdk::sequence_node *body) :
        cdk::basic_node(lineno), _import(import), _return_type(return_type), _function(*function), _default_return(default_return), _parameters(parameters), _body(body) {
    }
    inline function_node(int lineno, basic_type *return_type, bool import, const std::string &function, cdk::expression_node *default_return, cdk::sequence_node *parameters, cdk::sequence_node *body) :
        cdk::basic_node(lineno), _import(import), _return_type(return_type), _function(function), _default_return(default_return), _parameters(parameters), _body(body) {
    }
    inline function_node(int lineno, basic_type *return_type, bool import, const char *function, cdk::expression_node *default_return, cdk::sequence_node *parameters, cdk::sequence_node *body) :
        cdk::basic_node(lineno), _import(import), _return_type(return_type), _function(function), _default_return(default_return), _parameters(parameters), _body(body) {
    }

  public:
    inline bool import() {
      return _import;
    }
    inline basic_type *return_type() {
      return _return_type;
    }
    inline std::string &function() {
      return _function;
    }
    inline cdk::expression_node *default_return() {
      return _default_return;
    }
    inline cdk::sequence_node *parameters() {
      return _parameters;
    }
    inline cdk::sequence_node *body() {
      return _body;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_node(this, level);
    }

  };

} // pwn

#endif
