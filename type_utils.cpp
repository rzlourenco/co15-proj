#include "type_utils.h"
#include "pwn_type.h"
#include "ast/variable_node.h"

namespace pwn {
  basic_type *make_type(type_t type) {
    size_t type_size = 0;

    switch (type) {
      case basic_type::TYPE_INT:
      case basic_type::TYPE_POINTER:
      case basic_type::TYPE_STRING:
        type_size = 4;
      case basic_type::TYPE_DOUBLE:
        type_size = 8;
    };

    return new basic_type(type_size, type);
  }

  static std::vector<type_t> get_argument_types(cdk::sequence_node *node) {
    std::vector<type_t> ret;

    if (node == nullptr) {
      return ret;
    }

    for (auto p : node->nodes()) {
      const variable_node *param = (const variable_node *)p;
      ret.push_back(param->type()->name());
    }
    return ret;   
  }

  std::vector<type_t> get_argument_types(function_def_node *node) {
    return get_argument_types(node->parameters());
  }

  std::vector<type_t> get_argument_types(function_decl_node *node) {
    return get_argument_types(node->parameters());
  }

  std::vector<type_t> get_argument_types(function_call_node *node) {
    std::vector<type_t> ret;
    
    if (node == nullptr || node->arguments() == nullptr) {
      return ret;
    }

    for (auto p: node->arguments()->nodes()) {
      cdk::expression_node *arg = (cdk::expression_node *)p;
      ret.push_back(arg->type()->name());
    }

    return ret;
  }

  basic_type *make_const_type(basic_type *base_type) {
    return new basic_type(base_type->size(), base_type->name() | pwn_type::TYPE_CONST);
  }
  
  bool is_same_raw_type(type_t l, type_t r) {
    l = l & ~pwn_type::TYPE_CONST;
    r = r & ~pwn_type::TYPE_CONST;
    return l & r;
  } 
  bool is_same_raw_type(type_t l, const basic_type *r) {
    // basic_type->name() isn't const, pls fix cdk
    basic_type _r {*r};
    return is_same_raw_type(l, _r.name());
  }
  bool is_same_raw_type(const basic_type *l, type_t r) {
    basic_type _l {*l};
    return is_same_raw_type(_l.name(), r);
  }
  bool is_same_raw_type(const basic_type *l, const basic_type *r) {
    basic_type _l {*l};
    basic_type _r {*r};
    return is_same_raw_type(_l.name(), _r.name());
  }

  bool is_const_type(type_t type) {
    return type & pwn_type::TYPE_CONST;
  }

  bool is_const_type(basic_type *type) {
    return is_const_type(type->name());
  }
}

