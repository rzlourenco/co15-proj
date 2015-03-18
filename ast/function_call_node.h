#ifndef __PWN_FUNCTIONCALLNODE_H__
#define __PWN_FUNCTIONCALLNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>
#include <vector>
#include <string>

namespace pwn {

  /**
   * Class for describing function calls (ex: argc())
   */
  class function_call_node: public cdk::expression_node {
    std::string _function;
    cdk::sequence_node *_arguments;

  public:
    inline function_call_node(int lineno, const std::string *function, cdk::sequence_node *arguments =nullptr) :
        cdk::expression_node(lineno), _function(*function),  _arguments(arguments) {
    }
    inline function_call_node(int lineno, const std::string &function, cdk::sequence_node *arguments =nullptr) :
        cdk::expression_node(lineno), _function(function),  _arguments(arguments) {
    }
    inline function_call_node(int lineno, const char *function, cdk::sequence_node *arguments =nullptr) :
        cdk::expression_node(lineno), _function(function),  _arguments(arguments) {
    }

  public:
    inline std::string *function() {
      return &_function;
    }
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };

} // pwn

#endif
