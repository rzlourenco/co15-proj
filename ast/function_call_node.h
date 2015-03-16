#ifndef __PWN_FUNCTIONCALLNODE_H__
#define __PWN_FUNCTIONCALLNODE_H__

#include <cdk/ast/expression_node.h>
#include "lvalue_node.h"
#include <vector>
#include <string>

namespace pwn {

  /**
   * Class for describing function calls (ex: argc())
   */
  class function_call_node: public cdk::expression_node {
    std::string _function;
    typedef std::vector<cdk::expression_node *> arguments_t;
    arguments_t *_arguments;

  public:
    inline function_call_node(int lineno, const std::string *function, arguments_t *arguments =nullptr) :
        cdk::expression_node(lineno), _function(*function),  _arguments(arguments) {
    }
    inline function_call_node(int lineno, const std::string &function, arguments_t *arguments =nullptr) :
        cdk::expression_node(lineno), _function(function),  _arguments(arguments) {
    }
    inline function_call_node(int lineno, const char *function, arguments_t *arguments =nullptr) :
        cdk::expression_node(lineno), _function(function),  _arguments(arguments) {
    }

  public:
    inline std::string *function() {
      return &_function;
    }
    inline arguments_t *arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };

} // pwn

#endif