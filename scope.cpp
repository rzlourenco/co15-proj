#include "scope.h"
#include <cassert>

std::ostream& operator<<(std::ostream& os, const pwn::scope& scp) {
  switch(scp) {
    case pwn::scope::PUBLIC:
      os << "public";
      break;
    case pwn::scope::IMPORT:
      os << "import";
      break;
    case pwn::scope::LOCAL:
      os << "local";
      break;
    case pwn::scope::BLOCK:
      os << "block";
      break;
    case pwn::scope::DEFAULT:
      assert(false && "should never use an undetermined(DEFAULT) scope");
      break;
    }
  return os;
}

