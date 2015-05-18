#include "pwn_type.h"

std::ostream& operator<<(std::ostream& os, const basic_type& typ) {
 /*doesn't work because  basic_type::name isn't a const function :(
  basic_type::type t = typ.name();
  */
  basic_type::type t = typ._name;

  if (t & pwn_type::TYPE_CONST) {
    os << "const ";
  }

  /*Remove TYPE_CONST bit if it's set*/
  t = t & (~ pwn_type::TYPE_CONST); 

  switch(t) {
    case(basic_type::TYPE_INT):
      os << "int";
      break;
    case(basic_type::TYPE_DOUBLE):
      os << "double";
      break;
    case(basic_type::TYPE_STRING):
      os << "string";
      break;
    case(basic_type::TYPE_POINTER):
      os << "pointer";
      break;
    case(basic_type::TYPE_VOID):
      os << "void";
      break;
    case(basic_type::TYPE_UNSPEC):
      os << "unset";
      break;
    default:
      os << "error";
      break;
  }
  return os;
}
