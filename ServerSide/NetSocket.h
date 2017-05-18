/*
    File: netsocket.h

    Modified socket code from CSCE 313
*/


#ifndef _NetSocket_H_                   // include file only once
#define _NetSocket_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   R e q u e s t C h a n n e l */
/*--------------------------------------------------------------------------*/

class NetSocket {

private:

  /*  The current implementation uses sockets. */ 

  int fd = -1; //master file descriptor
  void handle_request(void* (*connection_handler)(int*));//function to create slave sockets
  struct sockaddr_in conn; //connection

public:

  /* -- CONSTRUCTOR/DESTRUCTOR */
  
  NetSocket(const string _server_host_name, const unsigned short _port_no);
  /* Creates a CLIENT-SIDE local copy of the channel. The channel is connected
  to the given port number at the given server host.
  THIS CONSTRUCTOR IS CALLED BY THE CLIENT. */
  
  NetSocket(const unsigned short _port_no, void* (*connection_handler)(int*));
  //server without backlog, default 10
  
  NetSocket(const unsigned short _port_no, void* (*connection_handler)(int*),const unsigned int backlog);
  /* Creates a SERVER-SIDE local copy of the channel that is accepting connections
  at the given port number.
  NOTE that multiple clients can be connected to the same server-side end of the
  request channel. Whenever a new connection comes in, it is accepted by the
  the server, and the given connection handler is invoked. The parameter to
  the connection handler is the file descriptor of the slave socket returned
  by the accept call. */
  
  ~NetSocket();
  /* Destructor of the local copy of the channel. */
  
  int getFD();

};


#endif


