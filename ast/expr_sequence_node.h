// $Id: expr_sequence_node.h,v 1.3 2014/02/26 18:53:32 david Exp $ -*- c++ -*-
#ifndef __PWN_EXPRSEQUENCENODE_H__
#define __PWN_EXPRSEQUENCENODE_H__

#include <vector>
#include <cdk/ast/expression_node.h>

namespace pwn {

  /**
   * Class representing an expression sequence, for instance, in an
   * instruction block or in an argument list. Mostly copied from sequence_node.
   */
  class expr_sequence_node: public cdk::expression_node {
    typedef std::vector<cdk::expression_node *> sequence_type;
    sequence_type _nodes;

  public:
    expr_sequence_node(int lineno, cdk::expression_node *item, expr_sequence_node *sequence = nullptr) :
        cdk::expression_node(lineno) {
      if (sequence != nullptr)
        _nodes = sequence->nodes();
      _nodes.push_back(item);
    }

  public:
    inline ~expr_sequence_node() {
      for (auto node : _nodes)
        delete node;
      _nodes.clear();
    }

  public:
    cdk::expression_node *node(size_t i) {
      return _nodes[i];
    }
    sequence_type &nodes() {
      return _nodes;
    }

    virtual basic_type *type() override {
        return _nodes.back()->type();
    }

    size_t size() {
      return _nodes.size();
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_expr_sequence_node(this, level);
    }

  };

} // pwn

#endif //_ _PWN_EXPRSEQUENCENODE_H__
