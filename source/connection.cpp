
#include <PA9.h>
#include "connection.h"
#include "term.h"
#include <fcntl.h>


static int set_nonblock_flag (int desc, int value)
{
  int oldflags = fcntl (desc, F_GETFL, 0);
  /* If reading the flags failed, return error indication now. */
  if (oldflags == -1)
    return -1;
  /* Set just the flag we want to set. */
  if (value != 0)
    oldflags |= O_NONBLOCK;
  else
    oldflags &= ~O_NONBLOCK;
  /* Store modified flag word in the descriptor. */
  return fcntl (desc, F_SETFL, oldflags);
}


static bool init_sockaddr (struct sockaddr_in *name,
			   const char *hostname,
			   int port, Term* t)
{
 // struct hostent *hostinfo;
  u8 i;
  name->sin_family = AF_INET;
  name->sin_port = htons (port);
  char  message[50] = "";	
  
  //strange bug i should exec gesthostbyname many times to obtain good ip
		for (i=0; i <  5; i++){
			name->sin_addr.s_addr = *(unsigned long *)gethostbyname(hostname)->h_addr_list[0];
			PA_WaitForVBL();
		}
		
	
 
 /* hostinfo = gethostbyname (hostname);
 if (hostinfo == NULL) {
    PA_OutputText(1,0,5,"Unknown host %s", hostname);
    return 0;
  }
  name->sin_addr.s_addr = *((unsigned long *) hostinfo->h_addr_list[0]);
*/
 // PA_OutputText(1,10,0,"%s",inet_ntoa(name->sin_addr));
    sprintf(message,"Connecting  %s : %s",inet_ntoa(name->sin_addr), (char *) port);
	t->add(message);
  return 1;
}

Connection::Connection()
{
	
	sock = fdin = fdout = -1;
}


Connection::~Connection()
{
  close();
}


bool Connection::open( char*s, int p, Term* t)
{
  sockaddr_in name;
	int error = 0;
	char *debug;
	
	int nbMode = 1;
	int bMode = 0; 


  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
	 t->add("DBG : Disigo : Cannot create socket !");
    return 0;
  }
  
    //set_nonblock_flag(sock, 0);
	ioctl(sock,FIONBIO,&nbMode); 

  if (!init_sockaddr (&name, s, p, t)) {
  	
  //	 PA_16cText(1, 15, 10, 255, 192,  "DBG : Problem init socket !\n", 1, 1, 100);	
 
  	return 0;
	}
	PA_WaitForVBL();
	error = connect(sock, (sockaddr*) &name, sizeof(name));
	PA_WaitForVBL();
  if (0 > error ) {
 	 sprintf(debug,"Connect socket error : %d",error);
  	t->add(debug);
  		PA_WaitForVBL();
   // close();
    return 0;
  }

  fdin = sock;
  fdout = sock;
  t->add("Connection succeed");
  return 1;
}

void Connection::close()
{
  if (sock > 0)
    ::closesocket(sock);
  sock = 0;
}

int Connection::read(void* buf, int sz)
{
  return ::recv(fdin, buf, sz,0);
}

int Connection::write(const void* buf, int sz)
{
  return ::send(fdout, buf, sz,0);
}

