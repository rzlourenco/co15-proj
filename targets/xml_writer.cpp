// $Id: xml_writer.cpp,v 1.1 2015/02/21 20:27:41 ist13500 Exp $ -*- c++ -*-
#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void pwn::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  open_element(node, lvl, std::make_pair("size", node->size()));

  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl + 2);
  }

  close_element(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  write_simple(node, lvl);
}

void pwn::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  write_simple(node, lvl);
}

//---------------------------------------------------------------------------

inline void pwn::xml_writer::process_unary_expression(cdk::unary_expression_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);

  open_element(node, lvl);
  node->argument()->accept(this, lvl + 2);
  close_element(node, lvl);
}

void pwn::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  process_unary_expression(node, lvl);
}
void pwn::xml_writer::do_not_node(pwn::not_node * const node, int lvl) {
  process_unary_expression(node, lvl);
}
void pwn::xml_writer::do_alloc_node(pwn::alloc_node * const node, int lvl) {
  process_unary_expression(node, lvl);
}
void pwn::xml_writer::do_addressof_node(pwn::addressof_node * const node, int lvl) {
  process_unary_expression(node, lvl);
}

//---------------------------------------------------------------------------

inline void pwn::xml_writer::process_binary_expression(cdk::binary_expression_node * const node, int lvl) {
  // CHECK_TYPES(_compiler, _symtab, node);

  write_element(node, lvl,
      std::make_tuple(),
      std::make_tuple(
        std::make_pair("left", node->left()),
        std::make_pair("right", node->right())));
}

void pwn::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_or_node(pwn::or_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}
void pwn::xml_writer::do_and_node(pwn::and_node * const node, int lvl) {
  process_binary_expression(node, lvl);
}

void pwn::xml_writer::do_function_call_node(pwn::function_call_node * const node, int lvl) {
  // CHECK_TYPES...

  write_element(node, lvl,
      std::make_tuple(
        std::make_pair("function", node->function())),
      std::make_tuple(
        std::make_pair("arguments", node->arguments())));
}

void pwn::xml_writer::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  //CHECK_TYPES...

  write_element(node, lvl,
      std::make_tuple(),
      std::make_tuple(
        std::make_pair("initializer", node->initializer()),
        std::make_pair("condition", node->condition()),
        std::make_pair("increment", node->increment())));
}
void pwn::xml_writer::do_return_node(pwn::return_node * const node, int lvl) {
  write_element(node, lvl);
}
void pwn::xml_writer::do_next_node(pwn::next_node * const node, int lvl) {
  write_element(node, lvl,
      std::make_tuple(
        std::make_pair("next", node->next())),
      std::make_tuple());
}
void pwn::xml_writer::do_stop_node(pwn::stop_node * const node, int lvl) {
  write_element(node, lvl,
      std::make_tuple(
        std::make_pair("stop", node->stop())),
      std::make_tuple());
}
void pwn::xml_writer::do_index_node(pwn::index_node * const node, int lvl) {
  //CHECK_TYPES...

  write_element(node, lvl,
      std::make_tuple(),
      std::make_tuple(
        std::make_pair("pointer", node->pointer()),
        std::make_pair("index", node->index())));
}
void pwn::xml_writer::do_identifier_node(pwn::identifierrr_node * const node, int lvl) {
  //CHECK_TYPES...

  write_element(node, lvl,
      std::make_tuple(
        std::make_pair("identifier", node->identifier())),
      std::make_tuple());
}
void pwn::xml_writer::do_noob_node(pwn::noob_node * const node, int lvl) {
  //CHECK_TYPES...

  write_element(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_function_decl_node(pwn::function_decl_node * const node, int lvl) {
  write_element(node, lvl,
      std::make_tuple(
        std::make_pair("import", node->import()),
        std::make_pair("name", node->name())),
      std::make_tuple(
        std::make_pair("parameters", node->parameters())));
}

void pwn::xml_writer::do_function_def_node(pwn::function_def_node * const node, int lvl) {
  write_element(node, lvl,
      std::make_tuple(
        std::make_pair("import", node->import()),
        std::make_pair("name", node->name())),
      std::make_tuple(
        std::make_pair("parameters", node->parameters()),
        std::make_pair("default_return", node->default_return()),
        std::make_pair("body", node->body())));
}

void pwn::xml_writer::do_variable_node(pwn::variable_node * const node, int lvl) {
  /* implement me*/
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);
  write_element(node, lvl,
      std::make_tuple(),
      std::make_tuple(
        std::make_pair("lvalue", node->lvalue())));
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_lvalue_node(pwn::lvalue_node * const node, int lvl) {
  throw 42;
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);

  write_element(node, lvl,
      std::make_tuple(),
      std::make_tuple(
        std::make_pair("lvalue", node->lvalue()),
        std::make_pair("rvalue", node->rvalue())));
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);

  write_element(node, lvl,
      std::make_tuple(),
      std::make_tuple(
        std::make_pair("expression", node->argument())));
}

void pwn::xml_writer::do_print_node(pwn::print_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);

  write_element(node, lvl,
      std::make_tuple(
        std::make_pair("new_line", node->new_line())),
      std::make_tuple(
        std::make_pair("argument", node->argument())));
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_read_node(pwn::read_node * const node, int lvl) {
  write_element(node, lvl,
      std::make_tuple(),
      std::make_tuple());
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_if_node(cdk::if_node * const node, int lvl) {
  //CHECK_TYPES...
  write_element(node, lvl,
      std::make_tuple(),
      std::make_tuple(
        std::make_pair("condition", node->condition()),
        std::make_pair("block", node->block())));
}

void pwn::xml_writer::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  //CHECK_TYPES...
  write_element(node, lvl,
      std::make_tuple(),
      std::make_tuple(
        std::make_pair("condition", node->condition()),
        std::make_pair("then", node->thenblock()),
        std::make_pair("else", node->elseblock())));
}

