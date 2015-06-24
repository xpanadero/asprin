#ifndef _SHELL_H_
#define _SHELL_H_


/* Define error codes */
#define SHERR_OPENPTY   -10
#define SHERR_TTYNAME   -11
#define SHERR_GRANTPT   -12
#define SHERR_UNLOCKPT  -13
#define SHERR_PTY	-14


#define BUFSIZE 4096    /* maximum message length */
#define SHELL_HISTFILE "HISTFILE=\0" 
#define SHELL_TERM     "TERM=\0"
#define SHELL_COMMAND  "/bin/sh"

/* System includes */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>

/* PTY support requires system-specific #include */

#if defined LINUX || defined OSF
  #include <pty.h>
#else
#if defined FREEBSD
  #include <libutil.h>
#else
#if defined OPENBSD
  #include <util.h>
#else
#if defined SUNOS || defined HPUX
  #include <sys/stropts.h>
#else
#if ! defined CYGWIN && ! defined IRIX
  #error Undefined host system
#endif
#endif
#endif
#endif
#endif


/* Shell Functions */

int runshell( int client,char *term,int wx,int wy,char *cmd);

#endif
