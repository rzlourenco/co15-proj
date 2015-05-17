#ifndef __PWN_SCOPE_H__
#define __PWN_SCOPE_H__

#include <iostream>

namespace pwn {
  enum class scope { PUBLIC, IMPORT, LOCAL };
}


std::ostream& operator<<(std::ostream& os, const pwn::scope& scp);

#endif
