#include "symbol.h"

namespace pwn {

std::shared_ptr<symbol> make_block_variable(basic_type *type, const std::string &name, ptrdiff_t offset) {
    return std::shared_ptr<symbol>(new symbol(
        scope::BLOCK,
        type,
        name,
        arglist_t (),
        false,
        false,
        "",
        offset
    ));
}

std::shared_ptr<symbol> make_local_variable(basic_type *type, const std::string &name, const std::string &label) {
    return std::shared_ptr<symbol>(new symbol(
        scope::LOCAL,
        type,
        name,
        arglist_t(),
        false,
        false,
        label,
        0
    ));
}
std::shared_ptr<symbol> make_local_function(basic_type *type, const std::string &name, const arglist_t &argtypes, const std::string &label) {
    return std::shared_ptr<symbol>(new symbol(
        scope::LOCAL,
        type,
        name,
        argtypes,
        true,
        true,
        label,
        0
    ));
}

std::shared_ptr<symbol> make_public_variable(basic_type *type, const std::string &name, const std::string &label) {
    return std::shared_ptr<symbol>(new symbol(
        scope::PUBLIC,
        type,
        name,
        arglist_t(),
        false,
        false,
        label,
        0
    ));
}
std::shared_ptr<symbol> make_public_function(basic_type *type, const std::string &name, const arglist_t &argtypes, const std::string &label) {
    return std::shared_ptr<symbol>(new symbol(
        scope::PUBLIC,
        type,
        name,
        argtypes,
        true,
        true,
        label,
        0
    ));
}

std::shared_ptr<symbol> make_import_variable(basic_type *type, const std::string &name) {
    return std::shared_ptr<symbol>(new symbol(
        scope::IMPORT,
        type,
        name,
        arglist_t(),
        false,
        false,
        "",
        0
    ));
}

std::shared_ptr<symbol> make_import_function(basic_type *type, const std::string &name, const arglist_t &argtypes) {
    return std::shared_ptr<symbol>(new symbol(
        scope::IMPORT,
        type,
        name,
        argtypes,
        true,
        false,
        "",
        0
    ));
}

}