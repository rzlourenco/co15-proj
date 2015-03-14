#ifndef __PWN_INDEXNODE_H__
#define __PWN_INDEXNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace pwn {

  /**
   * Class for describing the index ('[]') operator
   */
  class index_node: public cdk::unary_expression_node {
  public:
    /**
     * @param lineno source code line number for this node
     * @param left first operand
     * @param right second operand
     */
    inline index_node(int lineno, expression_node *arg) :
        cdk::unary_expression_node(lineno, arg) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

}

#endif
