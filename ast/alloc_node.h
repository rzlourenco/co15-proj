#ifndef __PWN_ALLOCNODE_H__
#define __PWN_ALLOCNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace pwn {

  /**
   * Class for describing the alloc ('[]') operator
   */
  class alloc_node: public cdk::unary_expression_node {
  public:
    inline alloc_node(int lineno, expression_node *arg) :
        cdk::unary_expression_node(lineno, arg) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_alloc_node(this, level);
    }

  };

}

#endif
