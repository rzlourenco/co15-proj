// $Id: postfix_writer.h,v 1.6 2015/05/18 08:46:25 ist176133 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_PF_WRITER_H__
#define __PWN_SEMANTICS_PF_WRITER_H__

#include <cassert>
#include <string>
#include <set>
#include <iostream>
#include <cdk/ast/binary_expression_node.h>
#include <cdk/symbol_table.h>
#include <cdk/emitters/basic_postfix_emitter.h>
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"
#include "targets/type_checker.h"

namespace pwn {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<pwn::symbol> &_symtab;
    cdk::basic_postfix_emitter &_pf;
    std::vector<std::string> _endrepeat_labels;
    std::vector<std::string> _increment_labels;
    std::string _endfunction_label;
    std::set<std::string> _declared_functions;
    size_t _lbl;
    ptrdiff_t _last_var_addr;

  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<pwn::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0) {
    }

  public:

    void declare_rts_function(const std::string &s);
    void do_function_decl(function_decl_node *const node);


    ~postfix_writer() {
      os().flush();
    }

  private:
    inline std::string mklbl() {
      return "L" + std::to_string(_lbl++);
    }

    template <typename Func>
    void do_ordering_node(Func f, cdk::binary_expression_node *node, int lvl) {
      CHECK_TYPES(_compiler, _symtab, node);

      if (is_int(node->left()->type()) && is_int(node->right()->type())) {
       node->left()->accept(this, lvl);
       node->right()->accept(this, lvl);
       f();
       return;
      }

      node->left()->accept(this, lvl+2);
      is_int(node->left()->type()) ? (_pf.I2D(), 0) : 0;

      node->right()->accept(this, lvl+2);
      is_int(node->right()->type()) ? (_pf.I2D(), 0) : 0;

      _pf.DCMP();
      _pf.INT(0);
      f();
      return;
    }

    template <typename Func>
    void do_equality_node(Func f, cdk::binary_expression_node *node, int lvl) {
      CHECK_TYPES(_compiler, _symtab, node);

      if (is_same_raw_type(node->left()->type(), node->right()->type())
          && (is_int(node->left()->type()) || is_pointer(node->left()->type()))) {
        node->left()->accept(this, lvl+2);
        node->right()->accept(this, lvl+2);

        f();
        return;
      }

      node->left()->accept(this, lvl+2);
      is_int(node->left()->type()) ? (_pf.I2D(), 0) : 0;

      node->right()->accept(this, lvl+2);
      is_int(node->right()->type()) ? (_pf.I2D(), 0) : 0;

      _pf.DCMP();
      _pf.INT(0);
      f();
    }

  public:
    void do_sequence_node(cdk::sequence_node * const node, int lvl);

  public:
    void do_integer_node(cdk::integer_node * const node, int lvl);
    void do_string_node(cdk::string_node * const node, int lvl);
    void do_double_node(cdk::double_node * const node, int lvl);
    void do_noob_node(pwn::noob_node * const node, int lvl);

  public:
    void do_neg_node(cdk::neg_node * const node, int lvl);
    void do_identity_node(pwn::identity_node * const node, int lvl);

  public:
    void do_add_node(cdk::add_node * const node, int lvl);
    void do_sub_node(cdk::sub_node * const node, int lvl);
    void do_mul_node(cdk::mul_node * const node, int lvl);
    void do_div_node(cdk::div_node * const node, int lvl);
    void do_mod_node(cdk::mod_node * const node, int lvl);
    void do_lt_node(cdk::lt_node * const node, int lvl);
    void do_le_node(cdk::le_node * const node, int lvl);
    void do_ge_node(cdk::ge_node * const node, int lvl);
    void do_gt_node(cdk::gt_node * const node, int lvl);
    void do_ne_node(cdk::ne_node * const node, int lvl);
    void do_eq_node(cdk::eq_node * const node, int lvl);
    void do_or_node(pwn::or_node * const node, int lvl);
    void do_and_node(pwn::and_node * const node, int lvl);
    void do_not_node(pwn::not_node * const node, int lvl);
    void do_alloc_node(pwn::alloc_node * const node, int lvl);
    void do_variable_node(pwn::variable_node * const node, int lvl);
    void do_function_call_node(pwn::function_call_node * const node, int lvl);
    void do_function_def_node(pwn::function_def_node * const node, int lvl);
    void do_function_decl_node(pwn::function_decl_node * const node, int lvl);
    void do_repeat_node(pwn::repeat_node * const node, int lvl);
    void do_addressof_node(pwn::addressof_node * const node, int lvl);
    void do_return_node(pwn::return_node * const node, int lvl);
    void do_next_node(pwn::next_node * const node, int lvl);
    void do_stop_node(pwn::stop_node * const node, int lvl);
    void do_index_node(pwn::index_node * const node, int lvl);
    void do_identifier_node(pwn::identifierrr_node * const node, int lvl);

  public:
    void do_while_node(cdk::while_node * const node, int lvl) {
      assert(false && "do_while_node was called!");
    }
    void do_if_node(cdk::if_node * const node, int lvl);
    void do_if_else_node(cdk::if_else_node * const node, int lvl);
    void do_block_node(pwn::block_node * const node, int lvl);

  public:
    void do_rvalue_node(pwn::rvalue_node * const node, int lvl);

  public:
    void do_evaluation_node(pwn::evaluation_node * const node, int lvl);
    void do_print_node(pwn::print_node * const node, int lvl);
    void do_read_node(pwn::read_node * const node, int lvl);
    void do_assignment_node(pwn::assignment_node * const node, int lvl);

  };

} // pwn

#endif
