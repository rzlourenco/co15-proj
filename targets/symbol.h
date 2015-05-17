// $Id: symbol.h,v 1.1 2015/02/21 20:27:35 ist13500 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_SYMBOL_H__
#define __PWN_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>
#include "type_utils.h"

namespace pwn {

    class symbol {
      const basic_type *_type;
      std::string _name;
      const std::vector<type_t> _argtypes;
      bool _function;
      bool _import {false};
      bool _definition {false};

      inline symbol(const basic_type *type, const std::string &name, const std::vector<type_t> &argtypes, bool function)
          : _type(type), _name(name), _argtypes(argtypes), _function(true) { }

    public:
      inline symbol(const basic_type *type, const std::string &name)
          : symbol(type, name, std::vector<type_t>(), false) { }

      inline symbol(const basic_type *type, const std::string &name, const std::vector<type_t> &argtypes)
          : symbol(type, name, argtypes, true) { }

      bool import() const { return _import; }
      void import(bool newimport) { _import = newimport; }
      bool definition() const { return _definition; }
      void definition(bool newdef) { _definition = newdef; }
      bool function() const { return _function; }

      const std::vector<type_t> &argument_types() const { return _argtypes; }

      virtual ~symbol() { delete _type; }

      inline const basic_type *type() const { return _type; }
      inline const std::string &name() const { return _name; }
    };

} // pwn

#endif
