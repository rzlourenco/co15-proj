#ifndef __PWN_ADDRESSOFNODE_H__
#define __PWN_ADDRESSOFNODE_H__

#include <cdk/ast/unary_expression_node.h>
#include "lvalue_node.h"

namespace pwn {

  /**
   * Class for describing the address of lvalue('?') operator
   */
  class addressof_node: public cdk::unary_expression_node {
  public:
    inline addressof_node(int lineno, lvalue_node *arg) :
        cdk::unary_expression_node(lineno, arg) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_addressof_node(this, level);
    }

  };

}

#endif
