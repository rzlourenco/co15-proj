#ifndef __PWN_INDEXNODE_H__
#define __PWN_INDEXNODE_H__

#include "cdk/ast/expression_node.h"
#include "lvalue_node.h"

namespace pwn {

  /**
   * Class for describing the index ('[]') operator
   */
  class index_node: public lvalue_node {
    lvalue_node *_lvalue;
    cdk::expression_node *_index;
 
  public:
    inline index_node(int lineno, lvalue_node *lvalue, cdk::expression_node *index) :
        lvalue_node(lineno, "index"), _lvalue(lvalue), _index(index) {
    }

    inline lvalue_node *lvalue() {
      return _lvalue;
    }
    inline cdk::expression_node *index() {
      return _index;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

}

#endif
