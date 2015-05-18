// $Id: symbol.h,v 1.1 2015/02/21 20:27:35 ist13500 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_SYMBOL_H__
#define __PWN_SEMANTICS_SYMBOL_H__

#include <cstddef>
#include <string>
#include <cdk/basic_type.h>
#include <memory>
#include "type_utils.h"
#include "scope.h"

namespace pwn {

  typedef std::vector<type_t> arglist_t;

  class symbol {
    pwn::scope _scope;
    basic_type *_type;
    std::string _name;
    const arglist_t _argtypes;
    bool _function;
    bool _definition;
    std::string _label;
    ptrdiff_t _offset;

    inline symbol(pwn::scope scp
                 , basic_type *type
                 , const std::string &name
                 , const arglist_t &argtypes
                 , bool function
                 , bool definition
                 , const std::string &label
                 , ptrdiff_t offset)
        : _scope(scp)
        , _type(type)
        , _name(name)
        , _argtypes(argtypes)
        , _function(function)
        , _definition(definition)
        , _label(label)
        , _offset(offset) { }

  public:
    pwn::scope scope() const { return _scope; }
    bool definition() const { return _definition; }
    void definition(bool newdef) { _definition = newdef; }
    bool function() const { return _function; }
    bool variable() const { return !function(); }
    const arglist_t &argument_types() const { return _argtypes; }
    basic_type *type() const { return _type; }
    const std::string &name() const { return _name; }
    ptrdiff_t offset() const { return _offset; }
    const std::string &label() const { return _label;}

  public:
    friend std::shared_ptr<symbol> make_block_variable(
        basic_type *type,
        const std::string &name,
        ptrdiff_t offset);

    friend std::shared_ptr<symbol> make_local_variable(
        basic_type *type,
        const std::string &name,
        const std::string &label);
    friend std::shared_ptr<symbol> make_local_function(
        basic_type *type,
        const std::string &name,
        const arglist_t &argtypes,
        const std::string &label);

    friend std::shared_ptr<symbol> make_public_variable(
        basic_type *type,
        const std::string &name,
        const std::string &label);
    friend std::shared_ptr<symbol> make_public_function(
        basic_type *type,
        const std::string &name,
        const arglist_t &argtypes,
        const std::string &label);

    friend std::shared_ptr<symbol> make_import_variable(
        basic_type *type,
        const std::string &name);
    friend std::shared_ptr<symbol> make_import_function(
        basic_type *type,
        const std::string &name,
        const arglist_t &argtypes);

  public:
    virtual ~symbol() { delete _type; }
  };

  std::shared_ptr<symbol> make_block_variable(
      basic_type *type,
      const std::string &name,
      ptrdiff_t offset);

  std::shared_ptr<symbol> make_local_variable(
      basic_type *type,
      const std::string &name,
      const std::string &label);
  std::shared_ptr<symbol> make_local_function(
        basic_type *type,
        const std::string &name,
        const arglist_t &argtypes,
        const std::string &label);

  std::shared_ptr<symbol> make_public_variable(
      basic_type *type,
      const std::string &name,
      const std::string &label);
  std::shared_ptr<symbol> make_public_function(
      basic_type *type,
      const std::string &name,
      const arglist_t &argtypes,
      const std::string &label);

  std::shared_ptr<symbol> make_import_variable(
      basic_type *type,
      const std::string &name);
  std::shared_ptr<symbol> make_import_function(
      basic_type *type,
      const std::string &name,
      const arglist_t &argtypes);

} // pwn

#endif
