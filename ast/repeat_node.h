#ifndef __PWN_REPEATNODE_H__
#define __PWN_REPEATNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace pwn {

  /**
   * Class for describing the repeat loop
   */
  class repeat_node: public cdk::basic_node {
    cdk::sequence_node *_initializer; /*optional*/
    cdk::sequence_node *_condition; /*optional*/
    cdk::sequence_node *_repetition; /*optional*/
    cdk::sequence_node *_body; /*optional*/
    
  public:
    inline repeat_node(int lineno, cdk::sequence_node *initializer=nullptr, cdk::sequence_node *condition=nullptr, cdk::sequence_node *repetition=nullptr, cdk::sequence_node *body=nullptr) :
        cdk::basic_node(lineno), _initializer(initializer), _condition(condition), _repetition(repetition), _body(body){
    }

  public:
    inline cdk::sequence_node *initializer(){
      return _initializer;
    }
    inline cdk::sequence_node *condition() {
      return _condition;
    }
    inline cdk::sequence_node *repetition() {
      return _repetition;
    }
    inline cdk::sequence_node *body() {
      return _body;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_repeat_node(this, level);
    }

  };

} // pwn

#endif
