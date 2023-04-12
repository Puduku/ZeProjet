// (*) In order NOT to worsen the portability trouble, we try to reproduce exactly the "standard"
//  behavior, even if subtle variations would have somewhat simplified the design of this tool.

// Supposed to Behave EXACTLY LIKE "dirname" command (*) 
static char *Dirname (const char *pathname) {
  static char buf[1024] ;
  strcpy(buf,pathname) ;
  int len = strlen(buf);
  while (len > 1 && buf[len-1] == '/') {
    buf[--len] = '\0'; 
  } // if
  while (len > 0 && buf[len-1] != '/') {
    buf[--len] = '\0'; 
  } // while
  while (len > 1 && buf[len-1] == '/') {
    buf[--len] = '\0'; 
  } // if
  if (buf[0] == '\0') {
    strcpy(buf,".") ;
  } // if
  return buf;  
} // Dirname


// Supposed to Behave EXACTLY LIKE "basename" command (*) 
static char *Basename (const char *pathname) {
  static char buf[1024] ;
  strcpy(buf,pathname) ;
  int len = strlen(buf);
  while (len > 1 && buf[len-1] == '/') {
    buf[--len] = '\0'; 
  } // if
  char *ptr = buf + len ;
  while (ptr > buf && *(ptr-1) != '/') {
    ptr--; 
  } // while
  return ptr;
} // Basename

