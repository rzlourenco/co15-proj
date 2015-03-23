#ifndef __PWN_IDENTIFIERNODE_H__
#define __PWN_IDENTIFIERNODE_H__

#include "lvalue_node.h"
#include <string>

namespace pwn {

  /**
   * Class for describing syntactic tree leaves for holding identifier lvalues (the a in a = 3).
   */
  class identifier_node : public lvalue_node {
    std::string _identifier;

  public:
    inline identifier_node(int lineno, const std::string &identifier) :
        lvalue_node(lineno), _identifier(identifier) {
    }

  public:
    inline const std::string &identifier() {
      return _identifier;
    }

    virtual void accept(basic_ast_visitor *sp, int level) override {
      return sp->do_identifier_node(this, level);
    }

  };

} // pwn

#endif
