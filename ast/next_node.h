#ifndef __PWN_NEXTNODE_H__
#define __PWN_NEXTNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace pwn {

  /**
   * Class for describing the next keyword
   */
  class next_node: public cdk::basic_node {
    cdk::expression_node *_next; /*maybe should be an int?*/
  public:
    /**
     * @param lineno source code line number for this node
     * @param return statement expression
     */
    inline next_node(int lineno, cdk::expression_node *next) :
        cdk::basic_node(lineno), _next(next)  {
    }

    inline cdk::expression_node *next() {
      return _next;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_next_node(this, level);
    }

  };

}

#endif
