#pragma once
#include <basic_type.h>
#include <vector>
#include <ast/function_def_node.h>
#include <ast/function_decl_node.h>
#include <ast/function_call_node.h>

namespace pwn {
  typedef basic_type::type type_t;

  std::vector<type_t> get_argument_types(function_def_node *);
  std::vector<type_t> get_argument_types(function_decl_node *);
  std::vector<type_t> get_argument_types(function_call_node *);

  basic_type *make_type(type_t type);
  basic_type *make_const_type(basic_type *);

  bool is_same_raw_type(type_t l, type_t r);
  bool is_same_raw_type(type_t l, const basic_type *r);
  bool is_same_raw_type(const basic_type *l, type_t r);
  bool is_same_raw_type(const basic_type *l, const basic_type *r);

  bool is_int(type_t type);
  bool is_int(const basic_type *type);

  bool is_double(type_t type);
  bool is_double(const basic_type *type);

  bool is_pointer(type_t type);
  bool is_pointer(const basic_type *type);

  bool is_const_type(type_t type);
  bool is_const_type(basic_type *type);
}

