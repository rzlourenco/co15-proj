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
                         , bool import
                         , basic_type *return_type
                         , const std::string *name
                         , cdk::sequence_node *parameters
                         , cdk::expression_node *default_return
                         , cdk::basic_node *body)
            : function_decl_node(lineno, return_type, *name, parameters, import)
            , _default_return(default_return)
            , _body(body) { }

        cdk::expression_node *default_return() {
            return _default_return;
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
