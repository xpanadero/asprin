#ifndef _ASPRINSH_H_
#define _ASPRINSH_H_


/* Defines */
#define ASPRINSH_VERSION "2.0.0"

#define ASPRINSH_VERSION_NUM 0x20000

#define ASPRINSH_VERSION_MAJOR 2
#define ASPRINSH_VERSION_MINOR 0
#define ASPRINSH_VERSION_MICRO 0

#define ASPRINSH_SH  1

#define BUFSIZE  4096
#define PORT     80
#define HNAME    "asprin"
#define HVALUE   "headache"
#define ROWS     25
#define COLS     80
#define METHOD   "GET"
#define PATH     "/"
#define PROTOCOL "HTTP/1.0"


/* System includes */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>


/* Functions */
int runshell(int server,char *hname,char *hvalue,char *cmd);
void options(char *name);

#endif
