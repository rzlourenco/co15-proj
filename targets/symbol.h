// $Id: symbol.h,v 1.1 2015/02/21 20:27:35 ist13500 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_SYMBOL_H__
#define __PWN_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>
#include "type_utils.h"
#include "scope.h"

namespace pwn {

    class symbol {
      scope _scope;
      basic_type *_type;
      std::string _name;
      const std::vector<type_t> _argtypes;
      bool _function;
      bool _function_definition;

      inline symbol(scope scp, basic_type *type, const std::string &name, const std::vector<type_t> &argtypes, bool function, bool function_definition)
          : _scope(scp), _type(type), _name(name), _argtypes(argtypes), _function(function), _function_definition(function_definition) { }

    public:
      inline symbol(scope scp, basic_type *type, const std::string &name)
          : symbol(scp, type, name, std::vector<type_t>(), false, false) { }

      inline symbol(scope scp, basic_type *type, const std::string &name, const std::vector<type_t> &argtypes, bool function_definition)
          : symbol(scp, type, name, argtypes, true, function_definition) { }

      scope scp() const { return _scope; }
      bool function_definition() const { return _function_definition; }
      void make_definition() { _function_definition = true; }
      bool function() const { return _function; }

      const std::vector<type_t> &argument_types() const { return _argtypes; }

      virtual ~symbol() { delete _type; }

      inline basic_type *type() const { return _type; }
      inline const std::string &name() const { return _name; }
    };

} // pwn

#endif
