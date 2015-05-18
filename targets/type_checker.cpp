// $Id: type_checker.cpp,v 1.5 2015/04/08 10:23:35 ist176133 Exp $ -*- c++ -*-
#include <algorithm>
#include <string>
#include <cassert>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
{ if (node->type() != nullptr && \
    node->type()->name() != basic_type::TYPE_UNSPEC) return; }

using pwn::type_t;

/*
 * literals
 */

void pwn::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(pwn::make_type(basic_type::TYPE_INT));
}

void pwn::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(pwn::make_type(basic_type::TYPE_STRING));
}

void pwn::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(pwn::make_type(basic_type::TYPE_DOUBLE));
}

void pwn::type_checker::do_noob_node(pwn::noob_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(pwn::make_type(basic_type::TYPE_POINTER));
}

void pwn::type_checker::do_identifier_node(pwn::identifierrr_node * const node, int lvl) {
  auto &id = node->identifier();
  auto symbol = _symtab.find(id);

  if (symbol == nullptr) {
    throw id + " undeclared";
  }

  node->type(symbol->type());
}

/*
 * unary expressions
 */

void pwn::type_checker::do_identity_node(pwn::identity_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  auto node_type = node->argument()->type();
  if(is_int(node_type)) {
    node->type(pwn::make_type(basic_type::TYPE_INT));
  } else if (is_double(node_type)) {
    node->type(pwn::make_type(basic_type::TYPE_DOUBLE));
  } else {
    throw std::string("wrong type in argument of identity expression");
  }
}

void pwn::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  auto node_type = node->argument()->type();
  if (is_int(node_type)) {
    node->type(pwn::make_type(basic_type::TYPE_INT));
  } else if (is_double(node_type)) {
    node->type(pwn::make_type(basic_type::TYPE_DOUBLE));
  } else {
    throw std::string("wrong type in argument of neg expression");
  }
}

void pwn::type_checker::do_addressof_node(pwn::addressof_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  if (!is_double(node->argument()->type())) {
    throw std::string("Can't get address of non-real variable");
  }

  node->type(pwn::make_type(basic_type::TYPE_POINTER));
}

void pwn::type_checker::do_not_node(pwn::not_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (!is_int(node->argument()->type()))
    throw std::string("wrong type in argument of not expression");

  node->type(pwn::make_type(basic_type::TYPE_INT));
}

void pwn::type_checker::do_alloc_node(pwn::alloc_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (!is_int(node->argument()->type()))
    throw std::string("wrong type in argument of alloc expression");

  node->type(pwn::make_type(basic_type::TYPE_POINTER));
}

/*
 * binary expressions
 */

bool is_type_in_types(type_t type, const std::vector<type_t> &types) {
  return std::find(types.cbegin(), types.cend(), type) != types.cend();
}

void pwn::type_checker::assert_bin_args(cdk::binary_expression_node *const node, const std::vector<type_t> &types) {
  ASSERT_UNSPEC;

  node->left()->accept(this, 0);
  node->right()->accept(this, 0);

  auto left_type = node->left()->type()->name();
  if (!is_type_in_types(left_type, types)) {
    throw std::string("wrong type in left argument of binary expression");
  }
  auto right_type = node->right()->type()->name();
  if (!is_type_in_types(right_type, types)) {
    throw std::string("wrong type in right argument of binary expression");
  }
}

bool check_bin_exp_type_with_order(cdk::binary_expression_node *const node, type_t type1, type_t type2) {
  auto left_type = node->left()->type()->name();
  auto right_type = node->right()->type()->name();

  return (left_type == type1 && right_type == type2);
}

bool check_bin_exp_type(cdk::binary_expression_node *const node, type_t type1, type_t type2) {
  return check_bin_exp_type_with_order(node, type1, type2)
    || check_bin_exp_type_with_order(node, type2, type1);
}

bool check_bin_exp_type(cdk::binary_expression_node *const node, type_t type1) {
  return check_bin_exp_type(node, type1, type1);
}

void pwn::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  assert_bin_args(node, std::vector<type_t>{basic_type::TYPE_INT, basic_type::TYPE_DOUBLE, basic_type::TYPE_POINTER});

  if ( check_bin_exp_type(node, basic_type::TYPE_INT)) {
    node->type(pwn::make_type(basic_type::TYPE_INT));
  } else if ( check_bin_exp_type(node, basic_type::TYPE_INT, basic_type::TYPE_DOUBLE)
      || check_bin_exp_type(node, basic_type::TYPE_DOUBLE)) {
    node->type(pwn::make_type(basic_type::TYPE_DOUBLE));
  } else if (check_bin_exp_type(node, basic_type::TYPE_INT, basic_type::TYPE_POINTER)) {
    node->type(pwn::make_type(basic_type::TYPE_POINTER));
  } else {
    throw std::string("wrong type in arguments of add expression");
  }
}

// Same as the add operator but we can't allow int - ptr
// Also, we must allow ptr - ptr so we can know how many elements are between the pointers
void pwn::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  assert_bin_args(node, std::vector<type_t>{basic_type::TYPE_INT, basic_type::TYPE_DOUBLE, basic_type::TYPE_POINTER});
  if ( check_bin_exp_type(node, basic_type::TYPE_INT)
      || check_bin_exp_type(node, basic_type::TYPE_POINTER)) {
    node->type(pwn::make_type(basic_type::TYPE_INT));
  } else if ( check_bin_exp_type(node, basic_type::TYPE_INT, basic_type::TYPE_DOUBLE)
      || check_bin_exp_type(node, basic_type::TYPE_DOUBLE)) {
    node->type(pwn::make_type(basic_type::TYPE_DOUBLE));
  } else if (check_bin_exp_type_with_order(node, basic_type::TYPE_POINTER, basic_type::TYPE_INT)) {
    node->type(pwn::make_type(basic_type::TYPE_POINTER));
  } else {
    throw std::string("wrong type in arguments of sub expression");
  }
}

void pwn::type_checker::do_arithmetic_binary_expression(cdk::binary_expression_node *const node) {
  ASSERT_UNSPEC;

  assert_bin_args(node, std::vector<type_t>{basic_type::TYPE_INT, basic_type::TYPE_DOUBLE});

  if ( check_bin_exp_type(node, basic_type::TYPE_INT)) {
    node->type(pwn::make_type(basic_type::TYPE_INT));
  } else if ( check_bin_exp_type(node, basic_type::TYPE_INT, basic_type::TYPE_DOUBLE)
      || check_bin_exp_type(node, basic_type::TYPE_DOUBLE)) {
    node->type(pwn::make_type(basic_type::TYPE_DOUBLE));
  } else {
    throw std::string("wrong type in arguments of ___ expression");
  }
}

void pwn::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_arithmetic_binary_expression(node);
}
void pwn::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  do_arithmetic_binary_expression(node);
}
void pwn::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  assert_bin_args(node, std::vector<type_t>{basic_type::TYPE_INT});
  node->type(pwn::make_type(basic_type::TYPE_INT));
}

void pwn::type_checker::do_comparison_binary_expression(cdk::binary_expression_node * const node) {
  assert_bin_args(node, std::vector<type_t> { basic_type::TYPE_INT, basic_type::TYPE_DOUBLE, basic_type::TYPE_POINTER });

  if (check_bin_exp_type(node, basic_type::TYPE_POINTER, basic_type::TYPE_DOUBLE)) {
    throw std::string("Cannot compare double with pointer!");
  }

  if (1) {
    // ???
    if (check_bin_exp_type(node, basic_type::TYPE_POINTER, basic_type::TYPE_INT)) {
      throw std::string("Cannot compare int with pointer!");
    }
  }

  node->type(pwn::make_type(basic_type::TYPE_INT));
}

void pwn::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  assert_bin_args(node, std::vector<type_t> { basic_type::TYPE_INT, basic_type::TYPE_DOUBLE });

  node->type(pwn::make_type(basic_type::TYPE_INT));
}
void pwn::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  assert_bin_args(node, std::vector<type_t> { basic_type::TYPE_INT, basic_type::TYPE_DOUBLE });

  node->type(pwn::make_type(basic_type::TYPE_INT));
}
void pwn::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  assert_bin_args(node, std::vector<type_t> { basic_type::TYPE_INT, basic_type::TYPE_DOUBLE });

  node->type(pwn::make_type(basic_type::TYPE_INT));
}
void pwn::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  assert_bin_args(node, std::vector<type_t> { basic_type::TYPE_INT, basic_type::TYPE_DOUBLE });

  node->type(pwn::make_type(basic_type::TYPE_INT));
}

void pwn::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_comparison_binary_expression(node);
}
void pwn::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_comparison_binary_expression(node);
}

void pwn::type_checker::do_logic_binary_expression(cdk::binary_expression_node * const node) {
  assert_bin_args(node, std::vector<type_t>({ basic_type::TYPE_INT }));

  node->type(pwn::make_type(basic_type::TYPE_INT));
}

void pwn::type_checker::do_or_node(pwn::or_node * const node, int lvl) {
  do_logic_binary_expression(node);
}
void pwn::type_checker::do_and_node(pwn::and_node * const node, int lvl) {
  do_logic_binary_expression(node);
}

void pwn::type_checker::do_index_node(pwn::index_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->pointer()->accept(this, lvl+2);
  node->index()->accept(this, lvl+2);

  if (!is_pointer(node->pointer()->type())) {
    throw std::string("Cannot index non-pointer!");
  }
  if (!is_int(node->index()->type())) {
    throw std::string("Cannot use non-integer indexes!");
  }

  node->type(pwn::make_type(basic_type::TYPE_DOUBLE));
}

/*
 * N-ary expressions
 */

void pwn::type_checker::do_function_call_node(pwn::function_call_node * const node, int lvl) {
  auto id = "." + node->function();

  auto symb = _symtab.find(id);
  if (symb == nullptr) {
    throw std::string("unknown function in function call: ") + node->function();
  }

  if (node->arguments() != nullptr) {
    node->arguments()->accept(this, lvl + 2);
    if (symb->argument_types() != get_argument_types(node)) {
      throw std::string("function call arguments mismatch");
    }
  } else if (symb->argument_types().size() > 0) {
    throw std::string("function call with too few parameters. Given 0");
  }

  node->type(pwn::make_type(symb->type()->name()));
}

/*
 * Declarations
 */

void pwn::type_checker::do_variable_node(pwn::variable_node * const node, int lvl) {
  const std::string &id = node->identifier();
  auto symb = _symtab.find_local(id);

  if (symb != nullptr) {
    throw std::string("duplicate variable declaration");
  }

  if (node->initializer() != nullptr) {
    node->initializer()->accept(this, lvl+2);

    if (!is_same_raw_type(node->initializer()->type(), node->type())) {
      throw std::string("Variable initializer must have the same type as the variable!");
    }
  }

  switch (node->scp()) {
  case scope::PUBLIC:
    symb = make_public_variable(node->type(), id, id);
    break;
  case scope::LOCAL:
    symb = make_local_variable(node->type(), id, id);
    break;
  case scope::IMPORT:
    symb = make_import_variable(node->type(), id);
    break;
  case scope::BLOCK:
    //FIXME: fix offset
    symb = make_block_variable(node->type(), id, 0);
    break;
  }

  _symtab.insert(id, symb);
}

void pwn::type_checker::do_function_def_node(pwn::function_def_node * const node, int lvl) {
  if (node->default_return() != nullptr) {
    node->default_return()->accept(this, lvl+2);

    //TODO: raw_type comparison is probably right because the integer literal has type int but return could be <#>
    if (!pwn::is_same_raw_type(node->default_return()->type(), node->return_type())) {
      throw std::string("function default return must have same type as return");
    }
  }

  // DAVID: horrible hack
  const std::string &id = "." + node->name();
  std::shared_ptr<pwn::symbol> symb = _symtab.find(id);
  if (symb != nullptr) {
    if (symb->definition()) {
      throw std::string("more than one definition for function ") + node->name();
    }
    if (symb->scope() == scope::IMPORT) {
      throw std::string("cannot define import function ") + node->name();
    }
    if (symb->type()->name() != node->return_type()->name()) {
      throw std::string("function ") + node->name() + " has already been declared with different return type";
    }
    if (symb->argument_types() != get_argument_types(node)) {
      throw std::string("function ") + node->name() + " has already been declared with different argument types";
    }

    symb->definition(true);
  } else {
    // DAVID: horrible hack
    switch (node->scp()) {
    case scope::PUBLIC:
      symb = make_public_function(node->return_type(), id, get_argument_types(node), id);
      break;
    case scope::LOCAL:
      symb = make_local_function(node->return_type(), id, get_argument_types(node), id);
      break;
    default:
      assert(false && "trying to define block/import functions which isn't possible");
    }

    _symtab.insert(id, symb);
  }

}

void pwn::type_checker::do_function_decl_node(pwn::function_decl_node * const node, int lvl) {
  if (node->parameters() != nullptr) {
    node->parameters()->accept(this, lvl+2);
  }

  // DAVID: horrible hack
  const std::string &id = "." + node->name();
  std::shared_ptr<pwn::symbol> symb = _symtab.find(id);
  if (symb != nullptr) {
    if (symb->scope() != node->scp()) {
      throw node->name() + std::string(" declared twice with different scopes");
    }
    if (symb->type() != node->return_type()) {
      throw std::string("function ") + node->name() + " has already been declared with different return type";
    }
    if (symb->argument_types() != get_argument_types(node)) {
      throw std::string("function ") + node->name() + " has already been declared with different argument types";
    }
  } else {
    switch(node->scp()) {
    case scope::PUBLIC:
      symb = make_public_function(node->return_type(), id, get_argument_types(node), id);
      break;
    case scope::LOCAL:
      symb = make_local_function(node->return_type(), id, get_argument_types(node), id);
      break;
    case scope::IMPORT:
      symb = make_import_function(node->return_type(), id, get_argument_types(node));
      break;
    default:
      assert( false && "can't declare block function");
    }
    _symtab.insert(id, symb);
  }
}

/*
 * Syntax
 */

void pwn::type_checker::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  if (node->condition() != nullptr) {
    node->condition()->accept(this, lvl+2);

    if (!pwn::is_same_raw_type(node->condition()->type(), basic_type::TYPE_INT)) {
      throw std::string("repeat condition must be empty or an integer expression");
    }
  }
}

void pwn::type_checker::do_return_node(pwn::return_node * const node, int lvl) { }
void pwn::type_checker::do_next_node(pwn::next_node * const node, int lvl) { }
void pwn::type_checker::do_stop_node(pwn::stop_node * const node, int lvl) { }

//---------------------------------------------------------------------------

void pwn::type_checker::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);

  node->type(new basic_type(*node->lvalue()->type()));
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl+2);
  node->rvalue()->accept(this, lvl+2);

  if (pwn::is_const_type(node->lvalue()->type())) {
    throw std::string("Cannot assign to const");
  }
  if (pwn::is_same_raw_type(node->lvalue()->type(), node->rvalue()->type())) {
    node->type(pwn::make_type(node->rvalue()->type()->name()));
  } else if (is_int(node->lvalue()->type())
      && is_double(node->rvalue()->type())) {
    node->type(pwn::make_type(basic_type::TYPE_INT));
  } else if (is_double(node->lvalue()->type())
      && is_int(node->rvalue()->type())) {
    node->type(pwn::make_type(basic_type::TYPE_DOUBLE));
  } else {
    throw std::string("assignment with incompatible types");
  }
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_evaluation_node(pwn::evaluation_node * const node, int lvl) { }

void pwn::type_checker::do_print_node(pwn::print_node * const node, int lvl) { }

//---------------------------------------------------------------------------

void pwn::type_checker::do_read_node(pwn::read_node * const node, int lvl) {
  // TODO
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_if_node(cdk::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);

  if (!pwn::is_same_raw_type(node->condition()->type(), basic_type::TYPE_INT)) {
    throw std::string("if condition must be an integer expression");
  }
}

void pwn::type_checker::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);

  if (!pwn::is_same_raw_type(node->condition()->type(), basic_type::TYPE_INT)) {
    throw std::string("if condition must be an integer expression");
  }
}

void pwn::type_checker::do_block_node(pwn::block_node * const node, int lvl) { }

//---------------------------------------------------------------------------

void pwn::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for(auto n : node->nodes()) {
    n->accept(this, lvl+2);
  }
}

