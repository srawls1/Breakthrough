/*
    File: netsocket.cpp

    Modified socket code from CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "NetSocket.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

//pthread_t thread[200];//array to hold thread objects
//int thread_count = 0;

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

//const int MAX_MESSAGE = 255;//max message for buffer

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* PRIVATE METHODS FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

void NetSocket::handle_request(void* (*connection_handler)(int*))
{
	pthread_t th;//thread object
	pthread_attr_t ta;//thread attribute
	pthread_attr_init(&ta); //initialize attribute
	pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);//set attribute options
	int ssock;//socket number
	//int osock = 0; //count open sockets
	//int msock = getdtablesize()-4;//max sockets

	while(1)
	{
		//if(osock++ > msock);//check if too many sockets open
		//{
		//	cerr << "too many open sockets\n";
		//	break;
		//}

		int acclen = sizeof(conn); //size of conn used bellow
		ssock = accept(fd,(struct sockaddr*)&conn, (socklen_t*)&acclen);//accept socket

		if (ssock < 0) {//catch failure
			if (errno == EINTR)//catch EINTR signal
				continue;
			else
				cerr << "accept failed: " << errno << endl;//print failure message
		}

		pthread_create(&th,&ta,(void* (*)(void*))connection_handler,(void*)((long)ssock));//create new thread
	}
}

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

NetSocket::NetSocket(const string _server_host_name, const unsigned short _port_no)
{
	struct hostent* hostname;//socket hostname address
	memset(&conn,0,sizeof(conn));//zero out connection
	
	if((hostname = gethostbyname(_server_host_name.c_str())) == NULL)//resolve hostname
		cerr << "Network Request Channel failed to resolve host name: " << _server_host_name;//print error message
	else
		memcpy((char*)&conn.sin_addr.s_addr,hostname->h_addr_list[0],hostname->h_length);//copy hostname address into conn
	
	
	fd = socket(AF_INET, SOCK_STREAM, 0);//create socket to connect to server
	
	if (fd < 0)//if failed to create socket
		cerr << "can't create socket: " << errno << endl;//print error
	
	conn.sin_family = AF_INET;//set conn settings
	conn.sin_port=htons(_port_no);//set conn port
	
	if(connect(fd,(struct sockaddr*)&conn,sizeof(conn))  < 0)//connect to server and check for failure
	{
		cerr << "can't connect to socket: " << errno << endl;//print error
		fd = -1;
	}
}

NetSocket::NetSocket(const unsigned short _port_no, void* (*connection_handler)(int*))
{
	NetSocket(_port_no, connection_handler, 10);
}

NetSocket::NetSocket(const unsigned short _port_no, void* (*connection_handler)(int*),const unsigned int backlog)
{
	memset(&conn,0,sizeof(conn));//zero out connection
	conn.sin_family = AF_INET;//set conn settings
	conn.sin_addr.s_addr = INADDR_ANY;//set conn settings
	conn.sin_port=htons(_port_no);//set conn port
	fd = socket(AF_INET, SOCK_STREAM, 0);//create master socket

	if (fd < 0)//check if failed to create master socket
		cerr << "can't create socket: " << errno << endl;//print error

	if (bind(fd,(sockaddr*)&conn,sizeof(conn)) < 0)//bind to master socket
		cerr << "can't bind to socket: " << errno << endl;//print error

	if (listen(fd,backlog) < 0)//listen for requst on master socket
		cerr << "can't listen to socket: " << errno << endl;//print error

	handle_request(connection_handler);//pass control to handle request which will create slave sockets
}

NetSocket::~NetSocket() {
  //cout << "close requests channel " << my_name << endl;
  close(fd);
}

int NetSocket::getFD() {
  return fd;
}