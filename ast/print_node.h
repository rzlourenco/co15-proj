// $Id: print_node.h,v 1.1 2015/02/21 20:27:30 ist13500 Exp $ -*- c++ -*-
#ifndef __PWN_PRINTNODE_H__
#define __PWN_PRINTNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

  /**
   * Class for describing print nodes.
   */
  class print_node: public cdk::basic_node {
    cdk::expression_node *_argument;
    bool _new_line;

  public:
    inline print_node(int lineno, cdk::expression_node *argument, bool new_line) :
        cdk::basic_node(lineno), _argument(argument), _new_line(new_line) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    inline bool new_line() {
      return _new_line;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_print_node(this, level);
    }

  };

} // pwn

#endif
