#include "Client.h"

#include <string>
#include <iostream>
#include <assert.h>
#include <thread>
#include <chrono>

#include "erl_interface.h"
#include "ei.h"

#pragma GCC diagnostic ignored "-Wwrite-strings"

//#define HOST "e1@Marcuss-MacBook-Pro"
//#define COOKIE "secretcookie"
//#define HANDLER "event_handler"
//#define HANDLER_FUNCTION "ping"

#define HOST "skyraid@127.0.0.1"
#define COOKIE "skyraid"
#define HANDLER "skyraid_doclet_event_handler"
#define HANDLER_FUNCTION "process_event"

Client::Client(const char* deviceId, EventHandler* eventHandler) {

	assert(deviceId != NULL);
	assert(eventHandler != NULL);

	mDeviceId = deviceId;
	mEventHandler = eventHandler;
}

void Client::Connect() {

  	std::cout << "[Client] Connect: " << std::endl;

	erl_init(NULL, 0);

	if (erl_connect_init(1, COOKIE, 0) == -1)
  		erl_err_quit("[Client] erl_connect_init");


  	std::cout << "[Client] Connect completed " << std::endl;
}

void Client::StartListenForEvents() {

	std::cout << "[Client] start listen for events\n\r";

	while (true) {

		// This must be here for some reason
		// if (erl_connect_init(1, "secretcookie", 0) == -1)
			// erl_err_quit("[Client] erl_connect_init");

		Event* event = this->mEventHandler->GetNextEvent();

		if (event != NULL) {

			ETERM* response;

			std::cout << "[Client] rpc \n\r";

			if ((this->mFD = erl_connect(HOST)) < 0)
		  		erl_err_quit("[Client] erl_connect did YOU start the SKYRAID server!");

		  	if (event->type == SYSTEM_READY) {
		  		//{ready, "Camera1"}
		  		ETERM *arr[2], *arrl[1];
				arr[0] = erl_mk_atom("ready");
				arr[1] = erl_mk_string(mDeviceId.c_str());
				ETERM* tuple = erl_mk_tuple(arr, 2);
				arrl[0] = tuple;
				ETERM* args = erl_mk_list(arrl, 1);

			  	response = erl_rpc(this->mFD, HANDLER, HANDLER_FUNCTION, args);

			  	erl_free_term(arr[0]);
			  	erl_free_term(arr[1]);
			  	erl_free_term(tuple);
			  	erl_free_term(args);

		  	} else if (event->type == PRODUCT_ENTER) {
		  		//{ready, "Camera1"}
		  		ETERM *arr[2], *arrl[1];
				arr[0] = erl_mk_atom("enter");
				arr[1] = erl_mk_string(mDeviceId.c_str());
				ETERM* tuple = erl_mk_tuple(arr, 2);
				arrl[0] = tuple;
				ETERM* args = erl_mk_list(arrl, 1);

			  	response = erl_rpc(this->mFD, HANDLER, HANDLER_FUNCTION, args);

			  	erl_free_term(arr[0]);
			  	erl_free_term(arr[1]);
			  	erl_free_term(tuple);
			  	erl_free_term(args);

		  	} else if (event->type == PRODUCT_LEAVE) {
		  		//{ready, "Camera1"}
		  		ETERM *arr[2], *arrl[1];
				arr[0] = erl_mk_atom("leave");
				arr[1] = erl_mk_string(mDeviceId.c_str());
				ETERM* tuple = erl_mk_tuple(arr, 2);
				arrl[0] = tuple;
				ETERM* args = erl_mk_list(arrl, 1);

			  	response = erl_rpc(this->mFD, HANDLER, HANDLER_FUNCTION, args);

			  	erl_free_term(arr[0]);
			  	erl_free_term(arr[1]);
			  	erl_free_term(tuple);
			  	erl_free_term(args);

		  	} else if (event->type == CLASSIFICATION) {
				//{classification, "Camera1", 1, "good", 0.95}
				ETERM *arr[5], *arrl[1];
				arr[0] = erl_mk_atom("classification");
				arr[1] = erl_mk_string(mDeviceId.c_str());
				arr[2] = erl_mk_int(1);
				arr[3] = erl_mk_string("good");
				arr[4] = erl_mk_float(0.95);
				ETERM* tuple = erl_mk_tuple(arr, 5);
				arrl[0] = tuple;
				ETERM* args = erl_mk_list(arrl, 1);

			  	response = erl_rpc(this->mFD, HANDLER, HANDLER_FUNCTION, args);

			  	erl_free_term(arr[0]);
			  	erl_free_term(arr[1]);
			  	erl_free_term(arr[2]);
			  	erl_free_term(arr[3]);
			  	erl_free_term(arr[4]);
			  	erl_free_term(tuple);
			  	erl_free_term(args);
		  	}

		  	if (response != NULL) {
		  		if (ERL_IS_INTEGER(response)) {
		  			std::cout << "[Client] got a int: " << ERL_INT_VALUE(response) <<"\n\r";
		  		} else if (ERL_IS_ATOM(response)) {
		  			int atomSize = ERL_ATOM_SIZE(response);
		  			std::string atomValue(ERL_ATOM_PTR_UTF8(response), atomSize);
		  			std::cout << "[Client] got a atom: " << atomValue <<"\n\r";
		  		} else if (ERL_IS_BINARY(response)) {
		  			std::cout << "[Client] got a binary: "  << "\n\r";
		  		} else if (ERL_IS_TUPLE(response)) {
		  			std::cout << "[Client] got a tuple: "  << "\n\r";
		  		} else {
		  			std::cout << "[Client] got a response but now checked: "  << "\n\r";
		  		}

		  		erl_free_term(response);

			} else {
				std::cout << "[Client] got a invalid reponse \n\r";
			}

			this->mEventHandler->DeleteEvent(event);

			if (erl_close_connection(this->mFD) != 0)
				erl_err_quit("[Client] erl_close_connection");

		} else {

			std::this_thread::sleep_for (std::chrono::milliseconds(10));
		}
	}
}
