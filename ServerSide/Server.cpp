/*
    File: server.cpp

    Modified Server code from CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "NetSocket.h"
#include "Evaluator.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* VARIABLES */
/*--------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- THE PROCESS REQUEST LOOP */
/*--------------------------------------------------------------------------*/

void *handle_process_loop(int* arg)//loop to handle requests
{

  int sfd = (int)((long)arg);//get file descriptor
  cout << "Client Connected\n";
  Evaluator e(sfd,"p");//create evaluator with password p

  for(;;)//run loop
  {

	e.eval();//run evaluator

	if(e.exit)//if exit flag
		break;
  }
    cout << "Client Disconnected\n";
	close(sfd);//close file descriptor
	usleep(1000000);// give the other end a bit of time.
	pthread_exit(NULL);
	return(void*) 1;
}



/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[])
{
	srand(time(NULL));
	unsigned int P = 10000+rand()%1000; //default port number
  	unsigned int B = 10; //size of default backlog


	opterr = 0;
	int option =0;

	//parse command line args
	while((option = getopt(argc,argv,"p:b:")) != -1){
		switch(option){
		case 'p': P = atoi(optarg); //parse port number
			if (P == 0) //if conversion fails
			{
				fprintf(stderr, "Non-valid argument for -p <port number for data server>"); //error message
				return 2;  //fail with status code 2
			}
			break;
		case 'b': //parse backlog
			B = atoi(optarg);
			if (B == 0) //if conversion fails
			{
				fprintf(stderr, "Non-valid argument for -b <backlog of the server socket>"); //error message
				return 3; //fail with status code 3
			}
			break;
		case '?': //unknown option
			if (optopt == 'p' || optopt == 'b')
				fprintf(stderr, "Unknown Option:%c", optopt); //error message
			return 1; //fail with status code 1
		default:
			abort(); //exit
		}
	}
  cout << "Data server pending requests on port " << P << " ... " << flush;
  NetSocket control_channel(P,handle_process_loop,B);
  cout << "done.\n" << flush;

}
