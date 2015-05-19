// -*- vim: ts=8 sw=2 sts=2 expandtab ft=cpp

#include "duplicate_definition_visitor.h"
#include "ast/all.h"

void pwn::duplicate_definition_visitor::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (auto *child : node->nodes())
    child->accept(this, 0);
}

void pwn::duplicate_definition_visitor::do_variable_node(pwn::variable_node * const node, int lvl) {
  define(node->identifier());  
}

void pwn::duplicate_definition_visitor::do_function_decl_node(pwn::function_decl_node * const node, int lvl) {
  declare(node->function());
}

void pwn::duplicate_definition_visitor::do_function_def_node(pwn::function_def_node * const node, int lvl) {
  define(node->function());

  push();

  define(node->function());  
  if (node->parameters())
    node->parameters()->accept(this, 0);
  node->body()->accept(this, 0);

  pop();
}

void pwn::duplicate_definition_visitor::do_block_node(pwn::block_node * const node, int lvl) {
  push();

  node->decls()->accept(this, 0);
  node->stmts()->accept(this, 0);
  
  pop();
}

