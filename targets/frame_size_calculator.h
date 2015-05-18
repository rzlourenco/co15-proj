#pragma once
#include "ast/all.h"
#include <vector>
#include <cstddef>

using std::size_t;

namespace pwn {

class frame_size_calculator: public basic_ast_visitor {
	private:
		std::vector<size_t> _list_needs;
		std::vector<size_t> _var_need_stack;
		std::vector<size_t> _trash_needs_count;
		size_t _max_need;

	public:
		frame_size_calculator(std::shared_ptr<cdk::compiler> compiler) : basic_ast_visitor(compiler) {}

  	void do_function_def_node(pwn::function_def_node * const node, int lvl);
  	void do_block_node(pwn::block_node * const node, int lvl);
  	void do_variable_node(pwn::variable_node * const node, int lvl);
  	void do_sequence_node(cdk::sequence_node * const node, int lvl);
  	size_t get_max_need() const;
};
}