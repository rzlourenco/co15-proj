#include "frame_size_calculator.h"

  void pwn::frame_size_calculator::do_function_def_node(pwn::function_def_node * const node, int lvl) {
  	_list_needs = std::vector<size_t>();
  	_var_need_stack = std::vector<size_t>();
  	_trash_needs_count = std::vector<size_t>();
  	_max_need = 0;

  	if(node->body() == nullptr) {
  		return;
  	}

  	node->body()->accept(this, lvl + 2);

  	for(size_t i : _list_needs) {
  		if (i > _max_need) {
  			_max_need = i;
  		}
  	}
  }

  void pwn::frame_size_calculator::do_block_node(pwn::block_node * const node, int lvl) {
  	_trash_needs_count.push_back(0);

  	if(node->decls() != nullptr) {
  		node->decls()->accept(this, lvl+2);
  	}

  	if(node->stmts() != nullptr) {
  		node->decls()->accept(this, lvl+2);
  	}


  	size_t total_need = 0;
  	for (size_t i : _var_need_stack) {
  		total_need += i;
  	}
  	_list_needs.push_back(total_need);

  	for(size_t i = 0; i < _trash_needs_count.back(); i++) {
  		_var_need_stack.pop_back();
  	}

  	_trash_needs_count.pop_back();  }

  void pwn::frame_size_calculator::do_variable_node(pwn::variable_node* const node, int lvl) {
  	_trash_needs_count.back()++;
  	_var_need_stack.push_back(node->type()->size());
  }


  void pwn::frame_size_calculator::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  	for(auto n : node->nodes()) {
  		n->accept(this, lvl+2);
  	}
  }

  size_t pwn::frame_size_calculator::get_max_need() const {
  	return _max_need;
  }