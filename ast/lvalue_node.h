// $Id: lvalue_node.h,v 1.1 2015/02/21 20:27:30 ist13500 Exp $
#ifndef __PWN_NODE_EXPRESSION_LEFTVALUE_H__
#define __PWN_NODE_EXPRESSION_LEFTVALUE_H__

#include <cdk/ast/simple_value_node.h>
#include <string>

namespace pwn {

  /**
   * Class for describing syntactic tree leaves for holding lvalues.
   */
  class lvalue_node: public cdk::simple_value_node<std::string> {
  public:
    inline lvalue_node(int lineno, const char *s) :
        cdk::simple_value_node<std::string>(lineno, s) {
    }
    inline lvalue_node(int lineno, const std::string &s) :
        cdk::simple_value_node<std::string>(lineno, s) {
    }
    inline lvalue_node(int lineno, const std::string *s) :
        cdk::simple_value_node<std::string>(lineno, *s) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    virtual void accept(basic_ast_visitor *sp, int level) {
      sp->do_lvalue_node(this, level);
    }

  };

} // pwn

#endif
