#ifndef __PWN_FUNCTIONNODE_H__
#define __PWN_FUNCTIONNODE_H__

#include "cdk/ast/basic_node.h"
#include "cdk/ast/expression_node.h"
#include "variable_node.h"
#include "cdk/basic_type.h"
#include <vector>
#include <string>


namespace pwn {

  /**
   * Class for describing a function declaration (ex: #argc(), #fibonacci(#n) ) and possible implementation
   */
  class function_node: public cdk::basic_node {
    basic_type *_return_type;
    std::string _function;
    typedef std::vector<cdk::expression_node *> parameters_t;
    parameters_t *_parameters; /*empty for functions without arguments*/
    typedef std::vector<cdk::basic_node *> body_t;
    body_t *_body; /*optional for declaration*/

  public:
    inline function_node(int lineno, basic_type *return_type, const std::string *function, parameters_t *parameters =nullptr, body_t *body =nullptr) :
        cdk::basic_node(lineno), _return_type(return_type), _function(*function), _parameters(parameters), _body(body) {
    }
    inline function_node(int lineno, basic_type *return_type, const std::string &function, parameters_t *parameters =nullptr, body_t *body =nullptr) :
        cdk::basic_node(lineno), _return_type(return_type), _function(function), _parameters(parameters), _body(body) {
    }
    inline function_node(int lineno, basic_type *return_type, const char *function, parameters_t *parameters =nullptr, body_t *body =nullptr) :
        cdk::basic_node(lineno), _return_type(return_type), _function(function), _parameters(parameters), _body(body) {
    }

  public:
    inline basic_type *return_type() {
      return _return_type;
    }
    inline std::string *function() {
      return &_function;
    }
    inline parameters_t *parameters() {
      return _parameters;
    }
    inline body_t *body() {
      return _body;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_node(this, level);
    }

  };

} // pwn

#endif
