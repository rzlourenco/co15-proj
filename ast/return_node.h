#ifndef __PWN_RETURNNODE_H__
#define __PWN_RETURNNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace pwn {

  /**
   * Class for describing the return keyword
   */
  class return_node: public cdk::basic_node {
    cdk::expression_node *_return_value;
  public:
    /**
     * @param lineno source code line number for this node
     * @param return statement expression
     */
    inline return_node(int lineno, cdk::expression_node *return_value) :
        cdk::basic_node(lineno), _return_value(return_value)  {
    }

    inline cdk::expression_node *return_value() {
      return _return_value;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

}

#endif
