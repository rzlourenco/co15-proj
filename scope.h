#ifndef __PWN_SCOPE_H__
#define __PWN_SCOPE_H__

#include <iostream>

namespace pwn {
  /*
   * DEFAULT is for indicating that it didn't have a scope qualifier(we have visitors that determine if it is IMPORT, GLOBAL, etc)
   * PUBLIC is for global declaration/definitions
   * IMPORT is for external ones
   * LOCAL is for private ones
   * BLOCK is for block private variables/parameters
   */
  enum class scope { DEFAULT, PUBLIC, IMPORT, LOCAL, BLOCK };
}


std::ostream& operator<<(std::ostream& os, const pwn::scope& scp);

#endif
