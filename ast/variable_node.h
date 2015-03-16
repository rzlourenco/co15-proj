#ifndef __PWN_VARIABLENODE_H__
#define __PWN_VARIABLENODE_H__

#include "cdk/ast/basic_node.h"
#include "cdk/ast/expression_node.h"
#include "cdk/basic_type.h"


namespace pwn {

  /**
   * Class for describing a variable(ex: %a) and initialized variables(ex #a = 4)
   */
  class variable_node: public cdk::basic_node {
    basic_type *_type;
    std::string _identifier;
    cdk::expression_node *_initializer;

  public:
    inline variable_node(int lineno, basic_type *type, const std::string *identifier, cdk::expression_node *initializer) :
        cdk::basic_node(lineno), _type(type), _identifier(*identifier), _initializer(initializer) {
    }

    inline variable_node(int lineno, basic_type *type, const std::string &identifier, cdk::expression_node *initializer) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _initializer(initializer) {
    }
    inline variable_node(int lineno, basic_type *type, const char *identifier, cdk::expression_node *initializer) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _initializer(initializer) {
    }

  public:
    inline basic_type *type() {
      return _type;
    }
    inline std::string *identifier() {
      return &_identifier;
    }
    inline cdk::expression_node *initializer() {
      return _initializer;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_node(this, level);
    }

  };

} // pwn

#endif
