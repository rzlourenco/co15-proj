// -*- vim: ts=8 sw=2 sts=2 expandtab ft=cpp

#include "default_scope_visitor.h"
#include "ast/all.h"
#include <iostream>

void pwn::default_scope_visitor::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  if (phase == DONE) return;

  // find function definitions
  for (cdk::basic_node *child : node->nodes()) {
    child->accept(this, 0);
  }
  
  phase = DECLARATION;

  // match definition with declaration
  for (cdk::basic_node *child : node->nodes()) {
    child->accept(this, 0);
  }

  phase = DONE;
}

void pwn::default_scope_visitor::do_variable_node(pwn::variable_node * const node, int lvl) {
  if (phase != DEFINITION) return;
  
  auto it = definition.find(node->identifier());
  if (it != definition.end())
    throw std::string("variable ") + node->identifier() + " already defined";

  if (node->scp() == scope::DEFAULT)
    node->scp(scope::PUBLIC);

  definition.insert(node->identifier());
}

void pwn::default_scope_visitor::do_function_decl_node(pwn::function_decl_node * const node, int lvl) {
  if (phase != DECLARATION) return;
  if (node->scp() != scope::DEFAULT) return;

  auto it = definition.find(node->function());
  if (it == definition.end())
    node->scp(scope::IMPORT);
  else
    node->scp(scope::PUBLIC);
}

void pwn::default_scope_visitor::do_function_def_node(pwn::function_def_node * const node, int lvl) {
  if (phase != DEFINITION) return;

  auto it = definition.find(node->function());
  if (it != definition.end())
    throw std::string("function ") + node->function() + " already defined";

  if (node->scp() == scope::DEFAULT) {
    node->scp(scope::PUBLIC);
  }
  
  definition.insert(node->function());
}

