#include "c-ansi/magic-kitchen.h"

// Public function: see description in .h
g_MAGIC_VALUE_unsigned_long SimpleHash (const unsigned char *p_str) {
  g_MAGIC_VALUE_unsigned_long hash = 5381;
  int c;

  while ((c = *p_str++)) {
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  } // while

  return hash;
} // MagicHash

