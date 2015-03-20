#ifndef __PWN_LVALUENODE_H__
#define __PWN_LVALUENODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

  /**
   * Class for describing syntactic tree leaves for holding lvalues.
   * This class is only useful to describe the concept of lvalues, because 
   */
  class lvalue_node: public cdk::expression_node {
  protected:
    inline lvalue_node(int lineno) :
        cdk::expression_node(lineno) {
    }
  public:

    virtual void accept(basic_ast_visitor *sp, int level) = 0; /*so index node and other types of lvalues have to implement it*/

  };

} // pwn

#endif
