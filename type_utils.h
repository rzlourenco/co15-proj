#pragma once
#include <basic_type.h>

namespace pwn {
    typedef basic_type::type type_t;

    basic_type *make_type(type_t type);
    basic_type *make_const_type(basic_type *);
}

