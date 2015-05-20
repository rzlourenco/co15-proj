#ifndef __PWN_NOOBNODE_H__
#define __PWN_NOOBNODE_H__

#include <cdk/ast/simple_value_node.h>

namespace pwn {

  /**
   * Class for describing syntactic tree leaves for holding noobs.
   */
  class noob_node: public cdk::simple_value_node<double *> {
    
  public:
    inline noob_node(int lineno) :
        cdk::simple_value_node<double *>(lineno, nullptr) {
    }

    virtual void accept(basic_ast_visitor *sp, int level) {
      sp->do_noob_node(this, level);
    }

  };

} // pwn

#endif
