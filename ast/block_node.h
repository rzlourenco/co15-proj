#ifndef __PWN_BLOCKNODE_H__
#define __PWN_BLOCKNODE_H__

#include <cdk/basic_type.h>
#include <cdk/ast/basic_node.h>

namespace pwn {

    class block_node : public cdk::basic_node {
	cdk::sequence_node *_decls;
	cdk::sequence_node *_stmts;

    public:
        block_node(int lineno, cdk::sequence_node *decls, cdk::sequence_node *stmts)
            : cdk::basic_node(lineno), _decls (decls), _stmts(stmts) {
	}

	cdk::sequence_node *decls() {
		return _decls;
	}
	cdk::sequence_node *stmts() {
		return _stmts;
	}

        void accept(basic_ast_visitor *sp, int level) {
            sp->do_block_node(this, level);
        }
    };
}

#endif // __PWN_BLOCKNODE_H__
