#ifndef __PWN_NOOBNODE_H__
#define __PWN_NOOBNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

  /**
   * Class for describing syntactic tree leaves for holding noobs.
   */
  class noob_node: public cdk::expression_node {
  protected:
    inline noob_node(int lineno) :
        cdk::expression_node(lineno) {
    }
  public:

    virtual void accept(basic_ast_visitor *sp, int level) {
      sp->do_noob_node(this, level);
    }

  };

} // pwn

#endif
