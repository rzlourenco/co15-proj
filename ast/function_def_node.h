#ifndef __PWNFUNCTIONDEFNODE_H__
#define __PWNFUNCTIONDEFNODE_H__

#include <cdk/basic_type.h>
#include "function_decl_node.h"

namespace pwn {
    class function_def_node : public function_decl_node {
        cdk::expression_node *_default_return;
        cdk::basic_node *_body;

    public:
        function_def_node(int lineno
                         , scope scp
                         , basic_type *return_type
                         , const std::string *function
                         , cdk::sequence_node *parameters
                         , cdk::expression_node *default_return
                         , cdk::basic_node *body)
            : function_decl_node(lineno, scp, return_type, function, parameters)
            , _default_return(default_return)
            , _body(body) { }

        cdk::expression_node *default_return() {
            return _default_return;
        }
        
        void default_return(cdk::expression_node *def_ret) {
          _default_return = def_ret;
        }

        cdk::basic_node *body() {
            return _body;
        }

        void accept(basic_ast_visitor *sp, int level) {
            sp->do_function_def_node(this, level);
        }
    };
}

#endif // __PWNFUNCTIONDEFNODE_H__
