// $Id: type_checker.cpp,v 1.5 2015/04/08 10:23:35 ist176133 Exp $ -*- c++ -*-
#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include "type_utils.h"

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//change to namespace pwn?
typedef pwn::type_t type_t;

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

/*
 * unary expressions
 */

inline void pwn::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in argument of unary expression");

  // in Simple, expressions are always int
  node->type(pwn::make_type(basic_type::TYPE_INT));
}


void pwn::type_checker::do_identity_node(pwn::identity_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  auto node_type = node->argument()->type()->name();
  if(node_type == basic_type::TYPE_INT) {
    node->type(pwn::make_type(basic_type::TYPE_INT));
  } else if (node_type == basic_type::TYPE_DOUBLE) {
    node->type(pwn::make_type(basic_type::TYPE_DOUBLE));
  } else {
    throw std::string("wrong type in argument of identity expression");
  }
}

void pwn::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  auto node_type = node->argument()->type()->name();
  if(node_type == basic_type::TYPE_INT) {
    node->type(pwn::make_type(basic_type::TYPE_INT));
  } else if (node_type == basic_type::TYPE_DOUBLE) {
    node->type(pwn::make_type(basic_type::TYPE_DOUBLE));
  } else {
    throw std::string("wrong type in argument of neg expression");
  }
}

void pwn::type_checker::do_addressof_node(pwn::addressof_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  node->type(pwn::make_type(basic_type::TYPE_POINTER));
}

void pwn::type_checker::do_not_node(pwn::not_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if(node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in argument of not expression");

  node->type(pwn::make_type(basic_type::TYPE_INT));
}

void pwn::type_checker::do_alloc_node(pwn::alloc_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if(node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in argument of alloc expression");

  node->type(pwn::make_type(basic_type::TYPE_POINTER));
}




/*
 * binary expressions
 */

inline void pwn::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in right argument of binary expression");

  // in Simple, expressions are always int
  node->type(pwn::make_type(basic_type::TYPE_INT));
}

inline bool is_type_in_types(type_t type, const std::vector<type_t> &types) {
  for(auto t : types) {
    if (type == t) {
      return true;
    }
  }
  return false;
}

inline void assert_bin_args(cdk::binary_expression_node *const node, const std::vector<type_t> &types) {
  ASSERT_UNSPEC;
  auto left_type = node->left()->type()->name();
  if (is_type_in_types(left_type, types)) {
    throw std::string("wrong type in left argument of binary expression");
  }

  auto right_type = node->right()->type()->name();
  if (is_type_in_types(right_type, types)) {
    throw std::string("wrong type in right argument of binary expression");
  }
}

inline bool check_bin_exp_type_with_order(cdk::binary_expression_node *const node, type_t type1, type_t type2) {
  auto left_type = node->left()->type()->name();
  auto right_type = node->right()->type()->name();
  return (left_type == type1 && right_type == type2);
}

inline bool check_bin_exp_type(cdk::binary_expression_node *const node, type_t type1, type_t type2) {
  return check_bin_exp_type_with_order(node, type1, type2)
      || check_bin_exp_type_with_order(node, type2, type1);
}

inline bool check_bin_exp_type(cdk::binary_expression_node *const node, type_t type1) {
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

//same as the add, but we can't allow to subtraction of int - pointer
void pwn::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  assert_bin_args(node, std::vector<type_t>{basic_type::TYPE_INT, basic_type::TYPE_DOUBLE, basic_type::TYPE_POINTER});
  if ( check_bin_exp_type(node, basic_type::TYPE_INT)) {
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

inline void do_arithmetic_binary_expression(cdk::binary_expression_node *const node) {
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
  assert_bin_args(node, std::vector<type_t>{basic_type::TYPE_INT, basic_type::TYPE_DOUBLE});
  node->type(pwn::make_type(basic_type::TYPE_INT));
}

void pwn::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::type_checker::do_or_node(pwn::or_node * const node, int lvl) {
  /* implement me*/
}
void pwn::type_checker::do_and_node(pwn::and_node * const node, int lvl) {
  /* implement me*/
}


void pwn::type_checker::do_variable_node(pwn::variable_node * const node, int lvl) {
  /* implement me*/
}
void pwn::type_checker::do_function_call_node(pwn::function_call_node * const node, int lvl) {
  /* implement me*/
}
void pwn::type_checker::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  /* implement me*/
}

void pwn::type_checker::do_return_node(pwn::return_node * const node, int lvl) {
  /* implement me*/
}
void pwn::type_checker::do_next_node(pwn::next_node * const node, int lvl) {
  /* implement me*/
}
void pwn::type_checker::do_stop_node(pwn::stop_node * const node, int lvl) {
  /* implement me*/
}

void pwn::type_checker::do_identifier_node(pwn::identifierrr_node * const node, int lvl) {
  /* implement me*/
}

void pwn::type_checker::do_index_node(pwn::index_node * const node, int lvl) {
  /* implement me*/
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_lvalue_node(pwn::lvalue_node * const node, int lvl) {
  const std::string id;
  /*const std::string &id = node->value()*/;
  std::shared_ptr<pwn::symbol> symbol = _symtab.find(id);
  if (symbol == nullptr) throw id + " undeclared";
  // hackish stuff...
  node->type(pwn::make_type(basic_type::TYPE_INT));
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;

  // DAVID: horrible hack!
  // (this is caused by Simple not having explicit variable declarations)
    const std::string id;
  /*const std::string &id = node->lvalue()->value();*/
  if (!_symtab.find(id)) {
    _symtab.insert(id, std::make_shared<pwn::symbol>(pwn::make_type(basic_type::TYPE_INT), id, -1)); // put in the symbol table
  }

  node->lvalue()->accept(this, lvl + 2);
  if (node->lvalue()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in left argument of assignment expression");

  node->rvalue()->accept(this, lvl + 2);
  if (node->rvalue()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in right argument of assignment expression");

  // in Simple, expressions are always int
  node->type(pwn::make_type(basic_type::TYPE_INT));
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void pwn::type_checker::do_print_node(pwn::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_read_node(pwn::read_node * const node, int lvl) {
  //node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_while_node(cdk::while_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void pwn::type_checker::do_if_node(cdk::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void pwn::type_checker::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void pwn::type_checker::do_block_node(pwn::block_node * const node, int lvl) {
  /* implement me*/
}
