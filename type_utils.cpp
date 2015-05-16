#include "type_utils.h"
#include "pwn_type.h"

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

  std::vector<type_t> get_argument_types(const function_def_node *node) {
    std::vector<type_t> ret;

    if (node->parameters() == nullptr) {
      return ret;
    }

    for (auto p : node->parameters()->nodes()) {
      const variable_node *param = (const variable_node *)p;
      // TODO
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
  bool is_same_raw_type(type_t l, basic_type *r) {
    return is_same_raw_type(l, r->name());
  }
  bool is_same_raw_type(basic_type *l, type_t r) {
    return is_same_raw_type(l->name(), r);
  }
  bool is_same_raw_type(basic_type *l, basic_type *r) {
    return is_same_raw_type(l->name(), r->name());
  }

  bool is_const_type(type_t type) {
    return type & pwn_type::TYPE_CONST;
  }

  bool is_const_type(basic_type *type) {
    return is_const_type(type->name());
  }
}

