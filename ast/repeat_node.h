#ifndef __PWN_REPEATNODE_H__
#define __PWN_REPEATNODE_H__

#include <cdk/ast/basic_node.h>

namespace pwn {

  /**
   * Class for describing the repeat loop
   */
  class repeat_node: public cdk::basic_node {
    cdk::expression_node *_initializer; /*optional*/
    cdk::expression_node *_condition; /*optional*/
    cdk::expression_node *_increment; /*optional*/
    cdk::basic_node *_body; /*optional*/
    
  public:
    inline repeat_node(int lineno, cdk::expression_node *initializer, cdk::expression_node *condition, cdk::expression_node *increment, cdk::basic_node *body) :
        cdk::basic_node(lineno), _initializer(initializer), _condition(condition), _increment(increment), _body(body){
    }

  public:
    inline cdk::expression_node *initializer(){
      return _initializer;
    }
    inline cdk::expression_node *condition() {
      return _condition;
    }
    inline cdk::expression_node *increment() {
      return _increment;
    }
    inline cdk::basic_node *body() {
      return _body;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_repeat_node(this, level);
    }

  };

} // pwn

#endif
