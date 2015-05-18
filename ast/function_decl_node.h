#ifndef __PWN_FUNCTIONDECLNODE_H__
#define __PWN_FUNCTIONDECLNODE_H__

#include <cdk/basic_type.h>
#include <cdk/ast/basic_node.h>
#include "../scope.h"

namespace pwn {

    class function_decl_node : public cdk::basic_node {
        basic_type *_return_type;
        std::string _function;
        cdk::sequence_node *_parameters;
        scope _scope;

    public:
        function_decl_node(int lineno
                          , scope scp
                          , basic_type *return_type
                          , const std::string *function
                          , cdk::sequence_node *parameters)
            : cdk::basic_node(lineno)
            , _return_type(return_type)
            , _function(*function)
            , _parameters(parameters)
            , _scope(scp) { }

        scope scp() {
            return _scope;
        }

        basic_type *return_type() {
            return _return_type;
        }

        const std::string &function() {
            return _function;
        }

        cdk::sequence_node *parameters() {
            return _parameters;
        }

        void accept(basic_ast_visitor *sp, int level) {
            sp->do_function_decl_node(this, level);
        }
    };
}

#endif // __PWN_FUNCTIONDECLNODE_H__
