#include <stdlib.h>

// 
static int GetIntArg (const char *p_arg, int min, int max, int *a_val, char *info32) {
  errno = 0; /* To distinguish success/failure after call */
  char *endPtr = NULL;
  long val;
  val = strtol(p_arg, &endPtr, 10);

#define m_INFO32CPY(p_src) {\
  strncpy(info32,p_src,32);\
  info32[31] = '\0';\
}

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
      (errno != 0 && val == 0)) {
    m_INFO32CPY("Invalid integer value")
    return 0;
  } // if

  if (endPtr == p_arg) {
    m_INFO32CPY("No digits were found")
    return 0;
  } // if

  if (*endPtr != '\0') { 
    m_INFO32CPY("Further characters after number");
    return 0;
  } // if

  if (val > INT_MAX || val < INT_MIN) {
    m_INFO32CPY("Too long integer value")
    return 0;
  } // if

  *a_val = (int)val;

  if (*a_val < min) {
    snprintf(info32,32,"< %d",min); 
    return 0;
  } // if

  if (*a_val > max) {
    snprintf(info32,32,"> %d",max); 
    return 0;
  } // if

  return 1;
} // GetIntArg
