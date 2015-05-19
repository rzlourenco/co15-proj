#include "frame_size_calculator.h"

  void pwn::frame_size_calculator::do_function_def_node(pwn::function_def_node * const node, int lvl) {
  	_total_need = 0;

  	if(node->body() == nullptr) {
  		return;
  	}

  	node->body()->accept(this, lvl + 2);

  }

  void pwn::frame_size_calculator::do_block_node(pwn::block_node * const node, int lvl) {
  	if(node->decls() != nullptr) {
  		node->decls()->accept(this, lvl+2);
  	}

  	if(node->stmts() != nullptr) {
  		node->decls()->accept(this, lvl+2);
  	}
  }



  void pwn::frame_size_calculator::do_variable_node(pwn::variable_node* const node, int lvl) {
    _total_need += node->type()->size();
  }


  void pwn::frame_size_calculator::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  	for(auto n : node->nodes()) {
  		n->accept(this, lvl+2);
  	}
  }

  size_t pwn::frame_size_calculator::total_need() const {
  	return _total_need;
  }
