#ifndef __PWNTYPE_H__
#define __PWNTYPE_H__

#include <cdk/basic_type.h>
#include <iostream>

struct pwn_type {
  typedef basic_type::type type;
  static const type TYPE_CONST = 1UL<<29;
};

std::ostream& operator<<(std::ostream& os, const basic_type& typ);

#endif 
