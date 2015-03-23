#ifndef __PWN_FUNCTIONDECLNODE_H__
#define __PWN_FUNCTIONDECLNODE_H__

#include <cdk/basic_type.h>
#include <cdk/ast/basic_node.h>

namespace pwn {

    class function_decl_node : public cdk::basic_node {
        basic_type *_return_type;
        std::string _name;
        cdk::sequence_node *_parameters;
        bool _import;

    public:
        function_decl_node(int lineno
                          , basic_type *return_type
                          , const std::string &name
                          , cdk::sequence_node *parameters
                          , bool import = false)
            : cdk::basic_node(lineno)
            , _return_type(return_type)
            , _name(name)
            , _parameters(parameters)
            , _import(import) { }

        bool import() const {
            return _import;
        }

        basic_type *return_type() {
            return _return_type;
        }

        const std::string &name() const {
            return _name;
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
