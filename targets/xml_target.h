// $Id: xml_target.h,v 1.2 2015/05/19 19:07:56 ist176133 Exp $
#ifndef __PWN_SEMANTICS_XML_EVALUATOR_H__
#define __PWN_SEMANTICS_XML_EVALUATOR_H__

#include <cdk/basic_target.h>
#include <cdk/compiler.h>
#include "targets/default_scope_visitor.h"
#include "targets/xml_writer.h"

namespace pwn {

  class xml_target: public cdk::basic_target {
    static xml_target _self;

  private:
    inline xml_target() :
        cdk::basic_target("xml") {
    }

  public:
    bool evaluate(std::shared_ptr<cdk::compiler> compiler) {
      // this symbol table will be used to check identifiers
      // an exception will be thrown if identifiers are used before declaration
      cdk::symbol_table<pwn::symbol> symtab;

      default_scope_visitor dsv(compiler);
      compiler->ast()->accept(&dsv, 0);

      xml_writer writer(compiler, symtab);
      compiler->ast()->accept(&writer, 0);
      return true;
    }

  };

} // pwn

#endif
