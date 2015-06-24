/*
 * Some of source code is ripped from 
 * Tiny SHell by Christophe Devine <devine@cr0.n;
 */

#include "shell.h"




int runshell( int client,char *term,int wx,int wy,char *cmd) {

  fd_set rd;
  struct winsize ws;
  char *slave, *temp, *shell,*cterm;
  int ret, len, pid, pty, tty, n;
  unsigned char message[BUFSIZE + 1];

  /* request a pseudo-terminal */

#if defined LINUX || defined FREEBSD || defined OPENBSD || defined OSF

  if( openpty( &pty, &tty, NULL, NULL, NULL ) < 0 ) {
    return( SHERR_OPENPTY );
  }

  slave = ttyname( tty );

  if( slave == NULL ) {
    return( SHERR_TTYNAME );
  }

#else
#if defined IRIX

  slave = _getpty( &pty, O_RDWR, 0622, 0 );

  if( slave == NULL ) {
    return( SHERR_OPENPTY );
  }

  tty = open( slave, O_RDWR | O_NOCTTY );

  if( tty < 0 ) {
    return( SHERR_TTYNAME );
  }

#else
#if defined CYGWIN || defined SUNOS || defined HPUX

  pty = open( "/dev/ptmx", O_RDWR | O_NOCTTY );

  if( pty < 0 ) {
    return( SHERR_OPENPTY );
  }

  if( grantpt( pty ) < 0 ) {
    return( SHERR_GRANTPT );
  }

  if( unlockpt( pty ) < 0 ) {
    return( SHERR_UNLOCKPT );
  }

  slave = ptsname( pty );

  if( slave == NULL ) {
    return( SHERR_TTYNAME );
  }

  tty = open( slave, O_RDWR | O_NOCTTY );

  if( tty < 0 ) {
    return( SHERR_OPENPTY );
  }

#if defined SUNOS || defined HPUX

  if( ioctl( tty, I_PUSH, "ptem" ) < 0 ) {
    return( SHERR_PTY );
  }

  if( ioctl( tty, I_PUSH, "ldterm" ) < 0 ) {
    return( SHERR_PTY );
  }

#if defined SUNOS

  if( ioctl( tty, I_PUSH, "ttcompat" ) < 0 ) {
    return( SHERR_PTY );
  }

#endif
#endif
#endif
#endif
#endif

  /* just in case bash is run, kill the history file */

  putenv( SHELL_HISTFILE );

  /* put the TERM environment variable */

  len= strlen(SHELL_TERM) + strlen(term)+1;
  cterm = (char *) malloc(sizeof(char)*len);

  if( cterm == NULL ) {
    return( 38 );
  }

  snprintf(cterm,len,"%s%s",SHELL_TERM,term);

  putenv( cterm );
  
  /*
   Search where i must free this variable??
   free(cterm); 
  */


  /* set the window size */

  ws.ws_row = wy;
  ws.ws_col = wx;


  ws.ws_xpixel = 0;
  ws.ws_ypixel = 0;

  if( ioctl( pty, TIOCSWINSZ, &ws ) < 0 ) {
    return( 40 );
  }

  /* get the system command */


  /* fork to spawn a shell */

  pid = fork();

  if( pid < 0 ) {
    return( 43 );
  }

  if( pid == 0 ) {
    /* close the client socket and the pty (master side) */

    close( client );
    close( pty );

    /* create a new session */

    if( setsid() < 0 ) {
      return( 44 );
    }

    /* set controlling tty, to have job control */

#if defined LINUX || defined FREEBSD || defined OPENBSD || defined OSF

    if( ioctl( tty, TIOCSCTTY, NULL ) < 0 ) {
      return( 45 );
    }

#else
#if defined CYGWIN || defined SUNOS || defined IRIX || defined HPUX

    {
      int fd;

      fd = open( slave, O_RDWR );

      if( fd < 0 ) {
        return( 46 );
      }

      close( tty );

      tty = fd;
    }

#endif
#endif

    /* tty becomes stdin, stdout, stderr */

    dup2( tty, 0 );
    dup2( tty, 1 );
    dup2( tty, 2 );

    if( tty > 2 ) {
      close( tty );
    }

    /* fire up the shell */

    if(cmd==NULL) {
      //write(client,"NULL\n",5);
      execl( SHELL_COMMAND, SHELL_COMMAND + 5, "-c", "exec bash --login", (char *) 0 );
    } else {
      execl( SHELL_COMMAND, SHELL_COMMAND + 5, "-c", cmd, (char *) 0 );
    } 

    /* This shouldn't happen */
 
    return( 48 );
  
  } else {
    /* tty (slave side) not needed anymore */

    close( tty );

    /* let's forward the data back and forth */

    while( 1 ) {
      FD_ZERO( &rd );
      FD_SET( client, &rd );
      FD_SET( pty, &rd );

      n = ( pty > client ) ? pty : client;

      if( select( n + 1, &rd, NULL, NULL, NULL ) < 0 ) {
        return( 49 );
      }

      if( FD_ISSET( client, &rd ) ) {
        
        len=recv(client,message,BUFSIZE,0);

        if( len == 0 ) break;

        if( len < 0 ) {
          return( 52 );
        }

        
        if( write( pty, message, len ) != len ) {
          return( 51 );
        }
      }

      if( FD_ISSET( pty, &rd ) ) {
        len = read( pty, message, BUFSIZE );

        if( len == 0 ) break;

        if( len < 0 ) {
          return( 52 );
        }
        
        if( send( client, message, len, 0 ) != len ) {
          return( 51 );
        }
        
      }
    }

    return( 54 );
  }

  /* not reached */
  return( 55 );
}
