#ifndef __PWN_NEXTNODE_H__
#define __PWN_NEXTNODE_H__

#include <cdk/ast/basic_node.h>
#include <cstddef>

namespace pwn {

  /**
   * Class for describing the next keyword
   */
  class next_node: public cdk::basic_node {
    std::size_t _next;
  public:
    /**
     * @param lineno source code line number for this node
     * @param next cycle depth
     */
    inline next_node(int lineno, std::size_t next=1) :
        cdk::basic_node(lineno), _next(next)  {
    }

    inline std::size_t next() {
      return _next;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_next_node(this, level);
    }

  };

}

#endif
