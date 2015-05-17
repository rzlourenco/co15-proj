#ifndef __PWN_VARIABLENODE_H__
#define __PWN_VARIABLENODE_H__

#include "cdk/ast/basic_node.h"
#include "cdk/ast/expression_node.h"
#include "cdk/basic_type.h"
#include "../scope.h"


namespace pwn {

  /**
   * Class for describing a variable(ex: %a) and initialized variables(ex #a = 4)
   */
  class variable_node: public cdk::basic_node {
    scope _scope;
    basic_type *_type;
    std::string _identifier;
    cdk::expression_node *_initializer;

  public:
    inline variable_node(int lineno, scope scp, basic_type *type, const std::string &identifier, cdk::expression_node *initializer) :
        cdk::basic_node(lineno), _scope(scp), _type(type), _identifier(identifier), _initializer(initializer) {
    }
    inline variable_node(int lineno, scope scp, basic_type *type, const std::string *identifier, cdk::expression_node *initializer) :
        cdk::basic_node(lineno), _scope(scp), _type(type), _identifier(*identifier), _initializer(initializer) {
    }

  public:
    inline scope scp() const {
      return _scope;
    }
    inline basic_type *type() const {
      return _type;
    }
    inline const std::string &identifier() const {
      return _identifier;
    }
    inline cdk::expression_node *initializer() const {
      return _initializer;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_node(this, level);
    }

  };

} // pwn

#endif
