#ifndef __PWN_COMMANODE_H__
#define __PWN_COMMANODE_H__

#include <cdk/ast/binary_expression_node.h>

namespace pwn {

  /**
   * Class for describing the comma (',') operator
   *
   * Used, por example, in:
   *
   * #fib(#n) = 1 {
   *   #prev; #cur;
   *   repeat (prev=1, cur=1; n > 0; n = n-1) {
   *     fib = prev + cur;
   *     prev = cur;
   *     cur = fib;
   *   }
   * }
   */
  class comma_node: public cdk::binary_expression_node {
  public:
    inline comma_node(int lineno, expression_node *left, expression_node *right) :
        cdk::binary_expression_node(lineno, left, right) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_comma_node(this, level);
    }

  };

}

#endif
