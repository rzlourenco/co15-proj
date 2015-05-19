// -*- vim: sw=2 sts=2 ts=2 expandtab ft=cpp
// $Id: postfix_writer.cpp,v 1.6 2015/05/18 08:46:25 ist176133 Exp $ -*- c++ -*-
#include <cassert>
#include <string>
#include <sstream>
#include <memory>
#include "targets/frame_size_calculator.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated


void pwn::postfix_writer::declare_rts_function(const std::string &s) {
  if(_declared_functions.count(s) == 0) { 
    _pf.TEXT();
    _pf.ALIGN(); 
    _pf.EXTERN(s);
    _declared_functions.insert(s);
  }
}

//---------------------------------------------------------------------------
//     THIS IS THE VISITOR'S DEFINITION
//---------------------------------------------------------------------------

void pwn::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//------------------- Literals  ---------------------------------------------

void pwn::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  _pf.INT(node->value()); // push an integer
}

void pwn::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  auto lbl = mklbl();

  _pf.RODATA();
  _pf.ALIGN();
  _pf.LABEL(lbl);
  _pf.DOUBLE(node->value());

  _pf.TEXT();
  _pf.ALIGN();
  _pf.ADDR(lbl);
  _pf.DLOAD();
}

void pwn::postfix_writer::do_noob_node(pwn::noob_node * const node, int lvl) {
  _pf.INT(0);
}

void pwn::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  auto lbl = mklbl();

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(lbl); // give the string a name
  _pf.STR(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  _pf.ALIGN();
  _pf.ADDR(lbl); // the string to be printed
}

void pwn::postfix_writer::do_identifier_node(pwn::identifierrr_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  const std::string &id = node->identifier();
  auto sym = _symtab.find(id);

  assert(sym != nullptr && "symbol is nullptr");

  // eww
  if (sym->scope() == scope::BLOCK) {
    _pf.LOCAL(sym->offset());
  } else {
    _pf.ADDR(sym->label());
  }
}

//--------------- Unary functions -------------------------------------------

void pwn::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  if (is_int(node->type())) {
    _pf.NEG(); // 2-complement
    return;
  }

  if (is_double(node->type())) {
    _pf.DNEG(); // 2-complement
    return;
  }

  assert(false && "typechecker failed");
}

// FIXME
void pwn::postfix_writer::do_alloc_node(pwn::alloc_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  // $ val
  node->accept(this, lvl);
  auto type = std::unique_ptr<basic_type>(make_type(basic_type::TYPE_DOUBLE));
  // $ val sz
  _pf.INT(type->size());
  // $ (val*sz)
  _pf.MUL();
  // $ #(val*sz)
  _pf.ALLOC();
  // $ #(val*sz) sp
  _pf.SP();

  //_pf.INT(val)
  _pf.INT(type->size());
  _pf.MUL();
  _pf.ADD();
}

void pwn::postfix_writer::do_not_node(pwn::not_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  auto label = mklbl();
  auto after = mklbl();
  node->argument()->accept(this, lvl+2);
  _pf.JZ(label);
  _pf.INT(0);
  _pf.JMP(after);
  _pf.LABEL(label);
  _pf.INT(1);
  _pf.LABEL(after);
}

void pwn::postfix_writer::do_addressof_node(pwn::addressof_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->argument()->accept(this, lvl+2);
}

void pwn::postfix_writer::do_identity_node(pwn::identity_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->argument()->accept(this, lvl+2);
}

//--------------------- Binary functions ------------------------------------

void pwn::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  if (is_int(node->type())) {
    node->left()->accept(this, lvl + 2);
    node->right()->accept(this, lvl + 2);
    _pf.ADD();
    return;
  }

  if (is_pointer(node->type())) {
    cdk::expression_node *ptr =
      is_pointer(node->left()->type())
      ? node->left()
      : node->right();
    cdk::expression_node *incr =
      is_int(node->left()->type())
      ? node->left()
      : node->right();

    ptr->accept(this, lvl+2);
    incr->accept(this, lvl+2);

    auto type = std::unique_ptr<basic_type>(make_type(basic_type::TYPE_DOUBLE));
    _pf.INT(type->size());
    _pf.MUL();
    _pf.ADD();
    return;
  }

  if (is_double(node->type())) {
    node->left()->accept(this, lvl+2);
    is_int(node->left()->type()) ? (_pf.I2D(), 0) : 0;

    node->right()->accept(this, lvl+2);
    is_int(node->right()->type()) ? (_pf.I2D(), 0) : 0;

    _pf.DADD();
    return;
  }

  assert(false && "type checker is badly done");
}

void pwn::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  if (is_int(node->type()) && is_int(node->left()->type())) {
    //both are integers
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.SUB();
    return;
  }

  if (is_int(node->type())) {
    //both are pointers
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.SUB();

    auto type = std::unique_ptr<basic_type>(make_type(basic_type::TYPE_DOUBLE));
    _pf.INT(type->size());
    _pf.DIV();
    return;
  }

  if (is_double(node->type())) {
    node->left()->accept(this, lvl+2);
    is_int(node->left()->type()) ? (_pf.I2D(), 0) : 0;

    node->right()->accept(this, lvl+2);
    is_int(node->right()->type()) ? (_pf.I2D(), 0) : 0;

    _pf.DSUB();
  }

  if (is_pointer(node->type())) {
    node->left()->accept(this, lvl+2);
    node->right()->accept(this, lvl+2);

    // We only have pointers to double
    auto type = std::unique_ptr<basic_type>(make_type(basic_type::TYPE_DOUBLE));
    _pf.INT(type->size());
    _pf.MUL();
    _pf.SUB();
    return;
  }

  assert(false && "typechecker has failed us");
}

void pwn::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  if (is_int(node->type())) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.MUL();
    return;
  }

  if (is_double(node->type())) {
    node->left()->accept(this, lvl+2);
    is_int(node->left()->type()) ? (_pf.I2D(), 0) : 0;

    node->right()->accept(this, lvl+2);
    is_int(node->right()->type()) ? (_pf.I2D(), 0) : 0;
    _pf.MUL();
    return;
  }

  assert(false && "typechecker has failed us");
}

void pwn::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  if (is_double(node->type())) {
    node->left()->accept(this, lvl+2);
    is_int(node->left()->type()) ? (_pf.I2D(), 0) : 0;

    node->right()->accept(this, lvl+2);
    is_int(node->right()->type()) ? (_pf.I2D(), 0) : 0;

    _pf.DDIV();
  }

  assert(is_int(node->type()) && "div node is neither int nor double");

  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}

void pwn::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}

void pwn::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_ordering_node([&]() { _pf.LT(); }, node, lvl);
}
void pwn::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_ordering_node([&]() { _pf.LE(); }, node, lvl);
}
void pwn::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_ordering_node([&]() { _pf.GE(); }, node, lvl);
}
void pwn::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_ordering_node([&]() { _pf.GT(); }, node, lvl);
}

void pwn::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_equality_node([&]() { _pf.NE(); }, node, lvl);
}
void pwn::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_equality_node([&]() { _pf.EQ(); }, node, lvl);
}

void pwn::postfix_writer::do_or_node(pwn::or_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  auto lbl = mklbl();

  node->left()->accept(this, lvl+2);
  _pf.JNZ(lbl);
  _pf.TRASH(node->type()->size());
  node->right()->accept(this, lvl+2);
  _pf.LABEL(lbl);
}

void pwn::postfix_writer::do_and_node(pwn::and_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  auto lbl = mklbl();

  node->left()->accept(this, lvl+2);
  _pf.JZ(lbl);
  _pf.TRASH(node->type()->size());
  node->right()->accept(this, lvl+2);
  _pf.LABEL(lbl);
}

void pwn::postfix_writer::do_index_node(pwn::index_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  // We only have pointers to double
  auto type = std::unique_ptr<basic_type>(make_type(basic_type::TYPE_DOUBLE));

  node->pointer()->accept(this, lvl+2);
  node->index()->accept(this, lvl+2);
  _pf.INT(type->size());
  _pf.MUL();
  _pf.ADD();

  // An index is a left-value, so we must leave the address on the stack
}

void pwn::postfix_writer::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  auto type = node->rvalue()->type();

  node->rvalue()->accept(this, lvl+2);

  switch (type->size()) {
  case 4:
    _pf.DUP();
    node->lvalue()->accept(this, lvl+2);
    _pf.STORE();
    break;
  case 8:
    _pf.DDUP();
    node->lvalue()->accept(this, lvl+2);
    _pf.DSTORE();
    break;
  default:
    assert(false && "type has size not in {4, 8}");
    break;
  }
}

// ------------------------ N-ary functions ------------------------------------

void pwn::postfix_writer::do_function_call_node(pwn::function_call_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  auto symb = _symtab.find("." + node->function());
  assert(symb != nullptr);

  size_t clean_size = 0;

  // Arguments are pushed onto the stack in reverse order (cdecl)
  const auto &argtypes = symb->argument_types();
  const auto &args = node->arguments() ? node->arguments()->nodes() : std::vector<cdk::basic_node *>();

  if (argtypes.size() != args.size()) {
    throw "argument list differs in size from previously declared function " + node->function();
  }

  for (ptrdiff_t ix = argtypes.size() - 1; ix >= 0; --ix) {
    auto arg = dynamic_cast<cdk::expression_node *>(args[ix]);
    assert(arg != nullptr && "syntax is allowing non-expression nodes in function call");

    if (!is_same_raw_type(argtypes[ix], arg->type())) {
      throw "argument " + std::to_string(ix + 1) + " differs in function call to " + node->function();
    }

    clean_size += arg->type()->size();
    arg->accept(this, lvl);
  }

  _pf.CALL(node->function());
  _pf.TRASH(clean_size);

  // FIXME
  if (is_same_raw_type(symb->type(), basic_type::TYPE_VOID))
    return;

  switch (symb->type()->size()) {
  case 4:
    _pf.PUSH();
    break;
  case 8:
    _pf.DPUSH();
    break;
  default:
    assert(false && "return type size is not 4 or 8 bytes");
  }
}

// ------------------------ Declarations ---------------------------------------

void pwn::postfix_writer::do_variable_import(pwn::variable_node *const node, std::shared_ptr<symbol> sym) {
    _pf.GLOBAL(sym->label(), _pf.OBJ());
}

void pwn::postfix_writer::do_variable_block(pwn::variable_node *const node, std::shared_ptr<symbol> sym) {
    if(node->initializer() == nullptr) {
      return;
    }
    _pf.TEXT();
    _pf.ALIGN();
    node->initializer()->accept(this, -1); /*meaningless -1: forgot to add lvl to the function parameters, won't change now*/

    switch (node->type()->size()) {
    case 4:
       _pf.LOCA(sym->offset());
      break;
    case 8:
      _pf.LOCAL(sym->offset());
      _pf.DSTORE();
      break;
    default:
      assert(false && "type size not 4 or 8");
    }
}

void pwn::postfix_writer::do_variable_public(pwn::variable_node *const node, std::shared_ptr<symbol> sym) {
    _pf.GLOBAL(sym->label(), _pf.OBJ());
    do_variable_local(node, sym);
}

void pwn::postfix_writer::do_variable_local(pwn::variable_node *const node, std::shared_ptr<symbol> sym) {
  if (node->initializer() == nullptr) {
    do_variable_local_without_init(node, sym);
  } else if ( dynamic_cast<cdk::string_node *>(node->initializer())) {
    do_variable_local_with_init_string(node, sym);
  } else {
    do_variable_local_with_init_non_string(node, sym);
  }
}


void pwn::postfix_writer::do_variable_local_without_init(pwn::variable_node *const node, std::shared_ptr<symbol> sym) {
    _pf.BSS();
    _pf.ALIGN();
    _pf.LABEL(sym->label());
    _pf.BYTE(sym->type()->size());
    return;
}

void pwn::postfix_writer::do_variable_local_with_init_non_string(pwn::variable_node *const node, std::shared_ptr<symbol> sym) {
  is_const_type(node->type()) ? _pf.RODATA() : _pf.DATA();
  _pf.ALIGN();
  _pf.LABEL(sym->label());
  if (dynamic_cast<cdk::integer_node *>(node->initializer())) {
    _pf.CONST(dynamic_cast<cdk::integer_node *>(node->initializer())->value());
  } else if (dynamic_cast<noob_node *>(node->initializer())) {
    _pf.CONST(0);
  }else if (dynamic_cast<cdk::double_node *>(node->initializer())) {
    _pf.DOUBLE(dynamic_cast<cdk::double_node *>(node->initializer())->value());
  } else {
    assert(false && "unknown initializer type");
  }
  
}

void pwn::postfix_writer::do_variable_local_with_init_string(pwn::variable_node *const node, std::shared_ptr<symbol> sym) {
  auto str_data_lbl = mklbl();

  /*str data*/
  _pf.RODATA();
  _pf.ALIGN(); 
  _pf.LABEL(str_data_lbl);
  _pf.STR(dynamic_cast<cdk::string_node *>(node->initializer())->value());

  /* reference label*/
  is_const_type(node->type()) ? _pf.RODATA() : _pf.DATA();
  _pf.ALIGN();
  _pf.LABEL(sym->label());
  _pf.ID(str_data_lbl);
}


void pwn::postfix_writer::do_variable_node(pwn::variable_node * const node, int lvl) {

  //add variable to symbol table
  const std::string &id = node->identifier();
  std::shared_ptr<symbol> symb = _symtab.find_local(id);
  if (symb == nullptr) {
    switch (node->scp()) {
      case scope::PUBLIC:
        symb = make_public_variable(node->type(), id, id);
        break;
      case scope::LOCAL:
        symb = make_local_variable(node->type(), id, id);
        break;
      case scope::IMPORT:
        symb = make_import_variable(node->type(), id);
        break;
      case scope::BLOCK:
        _last_var_addr -= node->type()->size();
        symb = make_block_variable(node->type(), id, _last_var_addr);
        break;
      case scope::DEFAULT:
        assert(false && "undefined scope");
    }
    _symtab.insert(id, symb);
  }  else {
    throw std::string("duplicate variable declaration: " + id);
  }

  CHECK_TYPES(_compiler, _symtab, node);

  switch(node->scp()) {
  case scope::IMPORT:
    do_variable_import(node, symb);
    break;
  case scope::BLOCK:
    do_variable_block(node, symb);
    break;
  case scope::PUBLIC:
    do_variable_public(node, symb);
    break;
  case scope::LOCAL:
    do_variable_local(node, symb);
    break;
  default:
    assert(false);
  }

}

const std::string calculate_function_label(const std::string &s) {
  if (s == "pwn") {
    return std::string("_main");
  } else if (s == "_main") {
    return std::string("._main");
  } else {
    return s;
  }
}

void pwn::postfix_writer::do_function_decl_symtab(pwn::function_decl_node *const node) {
  const std::string &id = "." + node->function();
  auto symb = _symtab.find(id);
  if( symb != nullptr) {
    return;
  } 
  switch(node->scp()) {
    case pwn::scope::PUBLIC:
      // Label must be compatible with C
      symb = make_public_function(node->return_type(), id, get_argument_types(node), node->function());
      break;
    case pwn::scope::LOCAL:
      symb = make_local_function(node->return_type(), id, get_argument_types(node), id);
      break;
    case pwn::scope::IMPORT:
      symb = make_import_function(node->return_type(), id, get_argument_types(node));
      break;
    default:
      assert(false && "functions must be defined in outer level");
  }

  _symtab.insert(id, symb);
}

void pwn::postfix_writer::do_function_decl(pwn::function_decl_node *const node) {
  if(_declared_functions.count(node->function()) > 0) {
    /*already declared*/
    return;
  }

  _pf.TEXT();
  _pf.ALIGN(); 
  switch(node->scp()) {
    case scope::PUBLIC:
      _pf.GLOBAL(calculate_function_label(node->function()), _pf.FUNC()); 
      break;
    case scope::IMPORT:
      _pf.EXTERN(calculate_function_label(node->function()));
      break;
    case scope::LOCAL:
      break;
    default:
      assert(false && "trying to declare unsupported functions ");
  }

  _declared_functions.insert(node->function());
}

void pwn::postfix_writer::do_function_def_node(pwn::function_def_node * const node, int lvl) {
  /*start of symtab accouting, type checking and what not*/

  do_function_decl_symtab(node);

  CHECK_TYPES(_compiler, _symtab, node);

  size_t reserved_bytes = 0;
  {
    auto calc = std::unique_ptr<frame_size_calculator>(new frame_size_calculator(_compiler));
    calc->do_function_def_node(node, lvl);
    reserved_bytes = calc->total_need();
  }
  reserved_bytes += (node->return_type() == nullptr ? 0 : node->return_type()->size());

  auto symb = _symtab.find("." + node->function());
  assert(symb != nullptr);

  _symtab.push();
  if (node->parameters()) {
    size_t base_offset = 8;
    for (auto *p : node->parameters()->nodes()) {
      variable_node *param = dynamic_cast<variable_node *>(p);
      _symtab.insert(param->identifier(), make_block_variable(param->type(), param->identifier(), base_offset));
      base_offset += param->type()->size();
    }
  }

  if (!is_same_raw_type(node->return_type(), basic_type::TYPE_VOID)) {
    _last_var_addr = -node->return_type()->size();
    _symtab.insert(node->function(), make_block_variable(node->return_type(), node->function(), _last_var_addr));
  } else {
    _last_var_addr = 0;
  }

  do_function_decl(node);
  _endfunction_label = mklbl();

  _pf.TEXT();
  _pf.ALIGN();
  const std::string &function_label = calculate_function_label(node->function());
  _pf.LABEL(function_label);
  _pf.ENTER(reserved_bytes);

  if (node->default_return() && !is_void(node->default_return()->type())) {
    node->default_return()->accept(this, lvl);
    _pf.LOCA(_last_var_addr);
  }

  node->body()->accept(this, lvl+2);

  _pf.LABEL(_endfunction_label);

  if (!is_void(node->return_type())) {
    switch (node->return_type()->size()) {
    case 8:
      _pf.LOCV(-node->return_type()->size());
      _pf.DPOP();
      break;
    case 4:
      _pf.LOCV(-node->return_type()->size());
      _pf.POP();
      break;
    default:
      assert(false && "function return not void and size is not 4 or 8");
    }
  }

  _pf.LEAVE();
  _pf.RET();

  _symtab.pop();
}

void pwn::postfix_writer::do_function_decl_node(pwn::function_decl_node * const node, int lvl) {
  
  do_function_decl_symtab(node);

  CHECK_TYPES(_compiler, _symtab, node);

  do_function_decl(node);
}

void pwn::postfix_writer::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  auto condition = mklbl(), increment = mklbl(), endrepeat = mklbl();

  _increment_labels.push_back(increment);
  _endrepeat_labels.push_back(endrepeat);

  if (node->initializer()) {
    node->initializer()->accept(this, lvl);
  }

  _pf.LABEL(condition);
  if (node->condition()) {
    node->condition()->accept(this, lvl);
  } else {
    _pf.INT(1);
  }
  _pf.JZ(endrepeat);

  node->body()->accept(this, lvl+2);

  _pf.LABEL(increment);
  if (node->increment()) {
    node->increment()->accept(this, lvl);
  }

  _pf.JMP(condition);
  _pf.LABEL(endrepeat);

  _increment_labels.pop_back();
  _endrepeat_labels.pop_back();
}
void pwn::postfix_writer::do_return_node(pwn::return_node * const node, int lvl) {
  _pf.JMP(_endfunction_label);
}
void pwn::postfix_writer::do_next_node(pwn::next_node * const node, int lvl) {
  auto jmp = node->next() - 1;
  if (jmp > _increment_labels.size()) {
    throw std::string("too many next levels") + std::to_string(jmp+1);
  }

  _pf.JMP(_increment_labels.rbegin()[jmp]);
}
void pwn::postfix_writer::do_stop_node(pwn::stop_node * const node, int lvl) {
  auto jmp = node->stop() - 1;
  if (jmp > _endrepeat_labels.size()) {
    throw std::string("too many stop levels") + std::to_string(jmp+1);
  }

  _pf.JMP(_endrepeat_labels.rbegin()[jmp]);
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->lvalue()->accept(this, lvl);

  switch (node->type()->size()) {
    case 8:
      _pf.DLOAD();
      break;
    case 4:
      _pf.LOAD();
      break;
    default:
      assert(false && "type size not 4 or 8");
  }
}

//---------------------------------------------------------------------------

#if 0
void pwn::postfix_writer::do_program_node(pwn::program_node * const node, int lvl) {
  // Note that Simple doesn't have functions. Thus, it doesn't need
  // a function node. However, it must start in the main function.
  // The ProgramNode (representing the whole program) doubles as a
  // main function node.

  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");
  _pf.ENTER(0);  // Simple doesn't implement local variables

  node->statements()->accept(this, lvl);

  // end the main function
  _pf.INT(0);
  _pf.POP();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}
#endif

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value

  _pf.TRASH(node->argument()->type()->size());
}

void pwn::postfix_writer::do_print_node(pwn::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value to print

  if (is_int(node->argument()->type())) {
    declare_rts_function("printi");
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (is_string(node->argument()->type())) {
    declare_rts_function("prints");
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else if (is_double(node->argument()->type())) {
    declare_rts_function("printd");
    _pf.CALL("printd");
    _pf.TRASH(8); // dele the printed value
  }
  else {
    assert(false && "Print not of type int, double or string");
  }

  if (node->new_line()) {
    declare_rts_function("println");
    _pf.CALL("println"); // print a newline
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_read_node(pwn::read_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  if (is_int(node->type())) {
    declare_rts_function("readi");
    _pf.CALL("readi");
    _pf.PUSH();
    return;
  }
  if (is_double(node->type())) {
    declare_rts_function("readd");
    _pf.CALL("readd");
    _pf.DPUSH();
    return;
  }

  assert(false && "read is not of int or double");
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_if_node(cdk::if_node * const node, int lvl) {
  auto endif = mklbl();

  node->condition()->accept(this, lvl);
  _pf.JZ(endif);
  node->block()->accept(this, lvl+2);
  _pf.LABEL(endif);
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  auto elseblock = mklbl(), endif = mklbl();

  node->condition()->accept(this, lvl);
  _pf.JZ(elseblock);
  node->thenblock()->accept(this, lvl+2);
  _pf.JMP(endif);
  _pf.LABEL(elseblock);
  node->elseblock()->accept(this, lvl+2);
  _pf.LABEL(endif);
}

//---------------------------------------------------------------------------
void pwn::postfix_writer::do_block_node(pwn::block_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  _symtab.push();
  node->decls()->accept(this, lvl+2);
  node->stmts()->accept(this, lvl+2);
  _symtab.pop();
}
