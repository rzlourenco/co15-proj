// $Id: postfix_writer.cpp,v 1.5 2015/04/08 10:23:35 ist176133 Exp $ -*- c++ -*-
#include <cassert>
#include <string>
#include <sstream>
#include <memory>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------
//     THIS IS THE VISITOR'S DEFINITION
//---------------------------------------------------------------------------

void pwn::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  _pf.INT(node->value()); // push an integer
}

void pwn::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  std::string lbl = mklbl();

  _pf.RODATA();
  _pf.ALIGN();
  _pf.LABEL(lbl);
  _pf.DOUBLE(node->value());

  _pf.TEXT();
  _pf.ALIGN();
  _pf.ADDR(lbl);
  _pf.DLOAD();
}

void pwn::postfix_writer::do_noob_node(cdk::noob_node * const node, int lvl) {
  _pf.INT(0);
}

void pwn::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.STR(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  _pf.ALIGN();
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  if(is_int(node->type())) {
    _pf.NEG(); // 2-complement
  } else {
    _pf.DNEG(); // 2-complement
  }
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  if(is_int(node->type())) {
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

  if(is_double(node->type())) {
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

  if (is_int(node->type()) {
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
  do_ordering_node([&]() { _pf.LT(); });
}
void pwn::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_ordering_node([&]() { _pf.LE(); });
}
void pwn::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_ordering_node([&]() { _pf.GE(); });
}
void pwn::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_ordering_node([&]() { _pf.GT(); });
}

void pwn::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_equality_node([&]() { _pf.NE(); });
}
void pwn::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_equality_node([&]() { _pf.EQ(); });
}

void pwn::postfix_writer::do_or_node(pwn::or_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->left()->accept(this, lvl+2);
  node->right()->accept(this, lvl+2);
  _pf.OR();
}
void pwn::postfix_writer::do_and_node(pwn::and_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->left()->accept(this, lvl+2);
  node->right()->accept(this, lvl+2);
  _pf.ADD();
}
void pwn::postfix_writer::do_not_node(pwn::not_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  node->argument()->accept(this, lvl+2);
  _pf.NOT();
}
void pwn::postfix_writer::do_alloc_node(pwn::alloc_node * const node, int lvl) {
  /* implement me*/
}
void pwn::postfix_writer::do_variable_node(pwn::variable_node * const node, int lvl) {
  /* implement me*/
}
void pwn::postfix_writer::do_function_call_node(pwn::function_call_node * const node, int lvl) {
  /* implement me*/
}
void pwn::postfix_writer::do_repeat_node(pwn::repeat_node * const node, int lvl) {
  /* implement me*/
}
void pwn::postfix_writer::do_addressof_node(pwn::addressof_node * const node, int lvl) {
  /* implement me*/
}
void pwn::postfix_writer::do_return_node(pwn::return_node * const node, int lvl) {
  /* implement me*/
}
void pwn::postfix_writer::do_next_node(pwn::next_node * const node, int lvl) {
  /* implement me*/
}
void pwn::postfix_writer::do_stop_node(pwn::stop_node * const node, int lvl) {
  /* implement me*/
}
void pwn::postfix_writer::do_index_node(pwn::index_node * const node, int lvl) {
  /* implement me*/
}
void pwn::postfix_writer::do_identifier_node(pwn::identifierrr_node * const node, int lvl) {
  /* implement me*/
}
void pwn::postfix_writer::do_noob_node(pwn::noob_node * const node, int lvl) {
  /* implement me*/
}


//---------------------------------------------------------------------------

void pwn::postfix_writer::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->lvalue()->accept(this, lvl);
  _pf.LOAD(); //FIXME: depends on type size
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  // DAVID: horrible hack! topkek
  // (this is caused by Simple not having explicit variable declarations)
    const std::string id;
  /*const std::string &id = node->lvalue()->value()*/;
  std::shared_ptr<pwn::symbol> symbol = _symtab.find(id);
  //if (symbol->value() == -1) {
    if(0) {
    _pf.DATA(); // variables are all global and live in DATA
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(id); // name variable location
    _pf.CONST(0); // initialize it to 0 (zero)
    _pf.TEXT(); // return to the TEXT segment
    //symbol->value(0);
  }

  node->rvalue()->accept(this, lvl); // determine the new value
  _pf.DUP();
  node->lvalue()->accept(this, lvl); // where to store the value
  _pf.STORE(); // store the value at address
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
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void pwn::postfix_writer::do_print_node(pwn::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  _pf.CALL("println"); // print a newline
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_read_node(pwn::read_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  _pf.CALL("readi");
  _pf.PUSH();
  //node->argument()->accept(this, lvl);
  _pf.STORE();
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_while_node(cdk::while_node * const node, int lvl) {
  int lbl1, lbl2;
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_if_node(cdk::if_node * const node, int lvl) {
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void pwn::postfix_writer::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------
void pwn::postfix_writer::do_block_node(pwn::block_node * const node, int lvl) {
  /* implement me*/
}
