#pragma once
#include <basic_type.h>

namespace pwn {
  typedef basic_type::type type_t;

  basic_type *make_type(type_t type);
  basic_type *make_const_type(basic_type *);

  bool is_same_raw_type(type_t l, type_t r);
  bool is_same_raw_type(type_t l, basic_type *r);
  bool is_same_raw_type(basic_type *l, type_t r);
  bool is_same_raw_type(basic_type *l, basic_type *r);

  bool is_const_type(type_t type);
  bool is_const_type(basic_type *type);
}

