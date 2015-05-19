// -*- vim: ts=8 sw=2 sts=2 expandtab ft=cpp

#include "default_return_replace.h"
#include "ast/all.h"
#include "type_utils.h"

void pwn::default_return_replace::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (auto *child : node->nodes())
    child->accept(this, 0);
}

void pwn::default_return_replace::do_function_def_node(pwn::function_def_node * const node, int lvl) {
  if(node->default_return() != nullptr) {
    return;
  }

  if(is_int(node->return_type())) {
    node->default_return(new cdk::integer_node(node->lineno(), 0));
  } else if (is_pointer(node->return_type())) {
    node->default_return(new pwn::noob_node(node->lineno()));
  }

}

