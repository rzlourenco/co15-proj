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
    basic_type *make_const_type(basic_type *base_type) {
        return new basic_type(base_type->size(), base_type->name() | pwn_type::TYPE_CONST);
    }
}

