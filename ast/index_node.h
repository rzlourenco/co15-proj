#ifndef __PWN_INDEXNODE_H__
#define __PWN_INDEXNODE_H__

#include <cdk/ast/expression_node.h>
#include "lvalue_node.h"

namespace pwn {

  /**
   * Class for describing syntactic tree leaves for holding index lvalues (a[3], (a+1)[2][3], etc).
   * It can either be implemented as an expression_node for the pointer and a sequence_node for the indexes or
   *   an expression_node for the pointer and an expresion_node for the index(and multiple indexes are viewed
   *   as nested lvalues, for example a[3][4] is viewed as  (a[3])[4] ). We choose the second form
   */
  class index_node: public lvalue_node {
    cdk::expression_node *_pointer;
    cdk::expression_node *_index;
    
  public:
    inline index_node(int lineno, cdk::expression_node *pointer, cdk::expression_node *index) :
        lvalue_node(lineno), _pointer(pointer), _index(index) {
    }
  public:
    inline cdk::expression_node *pointer() {
      return _pointer;
    }
    inline cdk::expression_node *index() {
      return _index;
    }

    virtual void accept(basic_ast_visitor *sp, int level) override {
      return sp->do_index_node(this, level);
    }

  };

} // pwn

#endif
