#pragma once
#include <basic_type.h>

namespace pwn {
    typedef decltype(basic_type::TYPE_INT) type_t;

    basic_type *make_type(type_t type);
    basic_type *make_const_type(basic_type *);
}

