#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <thread>
#include <chrono>

#include "Server.h"

#include "erl_interface.h"
#include "ei.h"

#define BUFSIZE 1000

#pragma GCC diagnostic ignored "-Wwrite-strings"

Server::Server(CommandHandler* commandHandler)
{
  assert(commandHandler != NULL);

  this->mCommandHandler = commandHandler;

  this->mLoop = true;
}

Server::~Server()
{
}

bool Server::Open(int port)
{
  std::cout << "[Server] begin open and publish port: " << port << std::endl;

	this->mPort = port;

	erl_init(NULL, 0);

	if (erl_connect_init(1, "secretcookie", 0) == -1)
  	erl_err_quit("[Server] erl_connect_init");

	/* Make a listen socket */
	if ((this->mListen = this->SetupSocket(this->mPort)) <= 0)
  	erl_err_quit("[Server] SetupSocket");

	if (erl_publish(this->mPort) == -1)
  	erl_err_quit("[Server] erl_publish");

  std::cout << "[Server] open completed " << std::endl;

  return true;
}

void Server::Close()
{

}

void Server::Listen()
{
	int got;
	unsigned char buf[BUFSIZE];              /* Buffer for incoming message */
  ErlMessage emsg;                         /* Incoming message */
  int res;

  ETERM *fromp, *tuplep, *fnp, *argp, *resp;

  std::cout << "[Server] start listen for connections" << std::endl;

  while (this->mLoop) {
	  if ((this->mFD = erl_accept(this->mListen, &this->mConn)) == ERL_ERROR)
	    erl_err_quit("[Server] erl_accept");

	  std::cout << "[Server] connected to " << this->mConn.nodename << std::endl;


    got = erl_receive_msg(this->mFD, buf, BUFSIZE, &emsg);
    if (got == ERL_TICK) {
      /* ignore */
    } else if (got == ERL_ERROR) {
      std::cout << "[Server] error received, listening now" << std::endl;
      //loop = 0;
    } else {

      if (emsg.type == ERL_REG_SEND) {
      	fromp = erl_element(2, emsg.msg);
      	tuplep = erl_element(3, emsg.msg);
      	fnp = erl_element(1, tuplep);
      	argp = erl_element(2, tuplep);

        if (strncmp(ERL_ATOM_PTR(fnp), "record", 3) == 0) {

          char path[BUFSIZE];
          int argplen = ERL_BIN_SIZE(argp);
          memcpy(path, (char *) ERL_BIN_PTR(argp), argplen);
          path[argplen + 1] = '\0';

          std::cout << path <<std::endl;

          Command* cmd = mCommandHandler->CreateRecordCommand(path);

          CommandResult* result = NULL;
          while (result == NULL) {
            result = this->mCommandHandler->GetNextResult();

            std::this_thread::sleep_for (std::chrono::milliseconds(30));
          }

          if (result->command == cmd) {
            resp = erl_format("{cnode, ~b}", result->result.c_str());
          } else {
            // TODO need to handle this better should probarbly just close the socket without response?
            resp = erl_format("{cnode, ~b}", "FATAL ERROR COMMAND RESULT MISSMATCH");
          }

          // Delete cause a Segmentation fault

          // mCommandHandler->DeleteResult(result);

        } if (strncmp(ERL_ATOM_PTR(fnp), "predict", 3) == 0) {

          char path[BUFSIZE];
          int argplen = ERL_BIN_SIZE(argp);
          memcpy(path, (char *) ERL_BIN_PTR(argp), argplen);
          path[argplen + 1] = '\0';

          std::cout << path;
          
          Command* cmd = mCommandHandler->CreatePredictCommand(path);

          CommandResult* result = NULL;
          while (result == NULL) {
            result = this->mCommandHandler->GetNextResult();

            std::this_thread::sleep_for (std::chrono::seconds(1));
          }

          if (result->command == cmd) {
            resp = erl_format("{cnode, ~b}", result->result.c_str());
          } else {
            // TODO need to handle this better should probarbly just close the socket without response?
            resp = erl_format("{cnode, ~b}", "FATAL ERROR COMMAND RESULT MISSMATCH");
          }

          // Segmentation fault
          // mCommandHandler->DeleteResult(result);

        } else if (strncmp(ERL_ATOM_PTR(fnp), "foo", 3) == 0) {

      	  // res = foo(ERL_INT_VALUE(argp));
          resp = erl_format("{cnode, ~i}", res);
      	} else if (strncmp(ERL_ATOM_PTR(fnp), "bar", 3) == 0) {
      	  // res = bar(ERL_INT_VALUE(argp));
          resp = erl_format("{cnode, ~i}", res);
      	}

      	erl_send(this->mFD, fromp, resp);

      	erl_free_term(emsg.from); erl_free_term(emsg.msg);
      	erl_free_term(fromp); erl_free_term(tuplep);
      	erl_free_term(fnp); erl_free_term(argp);
      	erl_free_term(resp);
        erl_close_connection(this->mFD);
      }
    }
  } /* while */

  std::cout << "[Server] Server shutting down" << std::endl;
}

int Server::SetupSocket(int port) {
  int listen_fd;
  struct sockaddr_in addr;
  int on = 1;

  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return (-1);

  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  //memset((void*) &addr, 0, (size_t) sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listen_fd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    return (-1);

  listen(listen_fd, 5);
  return listen_fd;
}
