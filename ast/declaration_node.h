#ifndef __PWN_DECLARATIONNODE_H__
#define __PWN_DECLARATIONNODE_H__

#include "cdk/ast/basic_node.h"
#include "cdk/ast/expression_node.h"
#include "variable_node.h"


namespace pwn {

  /**
   * Class for describing a declaration (ex: #a = 5)
   */
  class declaration_node: public cdk::basic_node {
    pwn::variable_node *_variable; 
    cdk::expression_node *_expression;

  public:
    inline declaration_node(int lineno, pwn::variable_node *variable, cdk::expression_node *expression=nullptr) :
        cdk::basic_node(lineno), _variable(variable), _expression(expression) {
    }

  public:
    inline pwn::variable_node *variable() {
      return _variable;
    }
    inline cdk::expression_node *expression() {
      return _expression;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_declaration_node(this, level);
    }

  };

} // pwn

#endif
