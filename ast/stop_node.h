#ifndef __PWN_STOPNODE_H__
#define __PWN_STOPNODE_H__

#include <cdk/ast/basic_node.h>
#include <cstddef>

namespace pwn {

  /**
   * Class for describing the stop keyword
   */
  class stop_node: public cdk::basic_node {
    std::size_t _stop;
  public:
    /**
     * @param lineno source code line number for this node
     * @param stop cycle depth
     */
    inline stop_node(int lineno, std::size_t stop=1) :
        cdk::basic_node(lineno), _stop(stop)  {
    }

    inline std::size_t stop() {
      return _stop;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_stop_node(this, level);
    }

  };

}

#endif
