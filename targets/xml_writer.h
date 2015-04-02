// $Id: xml_writer.h,v 1.1 2015/02/21 20:27:42 ist13500 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_XMLWRITER_H__
#define __PWN_SEMANTICS_XMLWRITER_H__

#include <string>
#include <iostream>
#include <cdk/ast/basic_node.h>
#include <cdk/symbol_table.h>
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"

namespace pwn {

  /**
   * Print nodes as XML elements to the output stream.
   */
  class xml_writer: public basic_ast_visitor {
    cdk::symbol_table<pwn::symbol> &_symtab;

  public:
    xml_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<pwn::symbol> &symtab) :
        basic_ast_visitor(compiler), _symtab(symtab) {
        os() << std::boolalpha;
    }

  public:
    ~xml_writer() {
      os().flush();
    }

  private:

    std::string indent(int level) {
        return std::string(level, ' ');
    }

    inline void write_attributes() {
        os() << ">" << std::endl;
    } 

    template <typename Arg1, typename... Args>
    inline void write_attributes(Arg1 arg1, Args... args) {
        os() << " " << arg1.first << "=\"" << arg1.second << "\"";
        write_attributes(args...);
    }

    template <typename... Args>
    inline void open_element(const std::string &tag, int lvl, Args... args) {
        os() << indent(lvl) << "<" << tag;
        write_attributes(args...);
    }

    template <typename... Args>
    inline void open_element(cdk::basic_node * const node, int lvl, Args... args) {
        open_element(node->name(), lvl, args...);
    }

    inline void open_element(const std::string &tag, int lvl) {
      os() << std::string(lvl, ' ') + "<" + tag + ">" << std::endl;
    }

    inline void open_element(const cdk::basic_node *node, int lvl) {
      open_element(node->name(), lvl);
    }

    inline void close_element(const std::string &tag, int lvl) {
      os() << std::string(lvl, ' ') + "</" + tag + ">" << std::endl;
    }
    inline void close_element(const cdk::basic_node *node, int lvl) {
      close_element(node->name(), lvl);
    }

    inline void write_children(int lvl) { (void)lvl; }

    template <typename Arg1, typename... Args>
    inline void write_children(int lvl, Arg1 arg1, Args... args) {
        open_element(arg1.first, lvl+2);
        if (arg1.second != nullptr) {
            arg1.second->accept(this, lvl+4);
        }
        
        close_element(arg1.first, lvl+2);
        write_children(lvl, args...);
    }

    // Shamelessly taken from Stack Overflow
    template <int...>
    struct seq { };

    template <int N, int... S>
    struct gens : gens<N-1, N-1, S...> { };

    template <int... S>
    struct gens <0, S...> {
        typedef seq<S...> type;
    };

    template <typename AttrType, int... S>
    inline void call_write_attributes(const std::string &tag, int lvl, AttrType attrs, seq<S...>) {
        open_element(tag, lvl, std::get<S>(attrs)...);
    }
    template <typename ChildType, int... S>
    inline void call_write_children(int lvl, ChildType children, seq<S...>) {
        write_children(lvl, std::get<S>(children)...);
    }

    template <typename AttrType, typename ChildType>
    inline void write_element(cdk::basic_node * const node, int lvl, AttrType attrs, ChildType children) {
        call_write_attributes(node->name(), lvl, attrs,
                typename gens<std::tuple_size<AttrType>::value>::type());

        call_write_children(lvl, children,
                typename gens<std::tuple_size<ChildType>::value>::type());

        close_element(node->name(), lvl);
    }

    inline void write_element(cdk::basic_node * const node, int lvl) {
        write_element(node, lvl,
                std::make_tuple(),
                std::make_tuple());
    }

    template<typename T>
    void write_simple(cdk::simple_value_node<T> * const node, int lvl) {
        os() << std::string(lvl, ' ') << "<" << node->name() << ">" << node->value() << "</" << node->name() << ">" << std::endl;
    }

  public:
    void do_sequence_node(cdk::sequence_node * const node, int lvl);

  protected:

  public:
    void do_integer_node(cdk::integer_node * const node, int lvl);
    void do_string_node(cdk::string_node * const node, int lvl);

  protected:
    void process_unary_expression(cdk::unary_expression_node * const node, int lvl);

  public:
    void do_neg_node(cdk::neg_node * const node, int lvl);

  protected:
    void process_binary_expression(cdk::binary_expression_node * const node, int lvl);

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
    void do_repeat_node(pwn::repeat_node * const node, int lvl);
    void do_addressof_node(pwn::addressof_node * const node, int lvl);
    void do_return_node(pwn::return_node * const node, int lvl);
    void do_next_node(pwn::next_node * const node, int lvl);
    void do_stop_node(pwn::stop_node * const node, int lvl);
    void do_index_node(pwn::index_node * const node, int lvl);
    void do_identifier_node(pwn::identifierrr_node * const node, int lvl);
    void do_noob_node(pwn::noob_node * const node, int lvl);

  public:
    void do_if_node(cdk::if_node * const node, int lvl);
    void do_if_else_node(cdk::if_else_node * const node, int lvl);

  public:
    void do_lvalue_node(pwn::lvalue_node * const node, int lvl);
    void do_rvalue_node(pwn::rvalue_node * const node, int lvl);

  public:
    void do_evaluation_node(pwn::evaluation_node * const node, int lvl);
    void do_print_node(pwn::print_node * const node, int lvl);
    void do_read_node(pwn::read_node * const node, int lvl);
    void do_assignment_node(pwn::assignment_node * const node, int lvl);
    void do_function_decl_node(pwn::function_decl_node * const node, int lvl);
    void do_function_def_node(pwn::function_def_node * const node, int lvl);
  };

} // pwn

#endif
