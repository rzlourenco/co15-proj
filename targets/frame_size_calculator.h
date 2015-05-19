#pragma once
#include "ast/all.h"
#include <vector>
#include <cstddef>

using std::size_t;

namespace pwn {

class frame_size_calculator: public basic_ast_visitor {
	private:
		size_t _total_need;

	public:
		frame_size_calculator(std::shared_ptr<cdk::compiler> compiler) : basic_ast_visitor(compiler) {}

  	void do_function_def_node(pwn::function_def_node * const node, int lvl);
  	void do_block_node(pwn::block_node * const node, int lvl);
  	void do_variable_node(pwn::variable_node * const node, int lvl);
  	void do_sequence_node(cdk::sequence_node * const node, int lvl);
  	size_t total_need() const;
};
}
