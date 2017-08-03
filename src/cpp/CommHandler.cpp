#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <assert.h>
#include <string.h>

#include "CommHandler.h"

#include "erl_interface.h"
#include "ei.h"

#define BUFSIZE 1000

CommHandler::CommHandler(CommandHandler* commandHandler)
{
  assert(commandHandler != NULL);

  this->mCommandHandler = commandHandler;

  this->mLoop = true;
}

CommHandler::~CommHandler()
{
}

bool CommHandler::Open(int port)
{
  std::cout << "CommHandler open and publish port: " << port << std::endl;

	this->mPort = port;

	erl_init(NULL, 0);

	if (erl_connect_init(1, "secretcookie", 0) == -1)
  	erl_err_quit("erl_connect_init");

	/* Make a listen socket */
	if ((this->mListen = this->SetupSocket(this->mPort)) <= 0)
  	erl_err_quit("my_listen");

	if (erl_publish(this->mPort) == -1)
  	erl_err_quit("erl_publish");
}

void CommHandler::Close()
{

}

void CommHandler::Listen()
{
	int got;
	unsigned char buf[BUFSIZE];              /* Buffer for incoming message */
  ErlMessage emsg;                         /* Incoming message */
  int res;

  ETERM *fromp, *tuplep, *fnp, *argp, *resp;

  std::cout << "CommHandler start listen for connections" << std::endl;

  while (this->mLoop) {
	  if ((this->mFD = erl_accept(this->mListen, &this->mConn)) == ERL_ERROR)
	    erl_err_quit("erl_accept");

	  std::cout << "CommHandler connected to " << this->mConn.nodename << std::endl;


    got = erl_receive_msg(this->mFD, buf, BUFSIZE, &emsg);
    if (got == ERL_TICK) {
      /* ignore */
    } else if (got == ERL_ERROR) {
      std::cout << "error received, listening now" << std::endl;
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

          RecordCommand* recCmd = new RecordCommand();
          recCmd->recordPath = new std::string(path);


          this->mCommandHandler->AddCommand(cmd);

          resp = erl_format("{cnode, ~b}", path);

        } if (strncmp(ERL_ATOM_PTR(fnp), "predict", 3) == 0) {

          char path[BUFSIZE];
          int argplen = ERL_BIN_SIZE(argp);
          memcpy(path, (char *) ERL_BIN_PTR(argp), argplen);
          path[argplen + 1] = '\0';

          std::cout << path;
          
          PredictCommand* cmd = new PredictCommand();
          cmd->filePath = new std::string(path);

          this->mCommandHandler->AddCommand(cmd);

          resp = erl_format("{cnode, ~b}", path);
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

  std::cout << "CommHandler shutting down" << std::endl;
}

int CommHandler::SetupSocket(int port) {
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
