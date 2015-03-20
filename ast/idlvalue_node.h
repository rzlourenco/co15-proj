#ifndef __PWN_IDLVALUENODE_H__
#define __PWN_IDLVALUENODE_H__

#include "lvalue_node.h"
#include <string>

namespace pwn {

  /**
   * Class for describing syntactic tree leaves for holding identifier lvalues (the a in a = 3).
   */
  class idlvalue_node: public lvalue_node{
    std::string _identifier;
    
  public:
    inline idlvalue_node(int lineno, const std::string *identifier) :
        lvalue_node(lineno), _identifier(*identifier) {
    }
    inline idlvalue_node(int lineno, const std::string &identifier) :
        lvalue_node(lineno), _identifier(identifier) {
    }
    inline idlvalue_node(int lineno, const char *identifier) :
        lvalue_node(lineno), _identifier(identifier) {
    }
  public:
    inline std::string &identifier() {
      return _identifier;
    }

    virtual void accept(basic_ast_visitor *sp, int level) override {
      return sp->do_idlvalue_node(this, level);
    }

  };

} // pwn

#endif
