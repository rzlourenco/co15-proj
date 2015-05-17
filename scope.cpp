#include "scope.h"

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
    }
  return os;
}

