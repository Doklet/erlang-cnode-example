
OTPROOT=$(wildcard /usr/local/lib/erlang/lib/erl_interface-*)

# all:	bin/cnodeserver bin/cnodeclient bin/complex3.beam bin/cnode.beam bin/recording.beam bin/predicting.beam
all: compile bin/complex3.beam bin/cnode.beam bin/recording.beam bin/predicting.beam bin/event_handler.beam

bin/%.beam:	src/erl/%.erl
	erlc -o bin $<

# bin/%:	src/c/%.c
# 	mkdir -p bin
# 	gcc -o $@ -I$(OTPROOT)/include -L$(OTPROOT)/lib src/c/complex.c src/c/recording.c src/c/predicting.c $< -lerl_interface -lei -lpthread

compile:
	mkdir -p bin
	g++ -std=c++11 -g -o bin/cppnodeserver -I$(OTPROOT)/include -L$(OTPROOT)/lib src/cpp/Server.cpp src/cpp/Client.cpp src/cpp/CommandHandler.cpp src/cpp/eventHandler.cpp src/cpp/main.cpp $< -lerl_interface -lei -lpthread

clean:
	rm -rf bin


start_server:
	epmd -daemon
	bin/cppnodeserver 3457

start_client:
	echo "run 'complex3:foo(4).' via erlang shell"
	erl -sname e1 -setcookie secretcookie -pa bin

start_c_client:
	bin/cnodeclient

#open_port({spawn, "bin/cppnodeserver 3457"}, [exit_status, {packet, 2}, {args, },{cd, "/Users/marcusnilsson/VBoxShared/test/erlang-cnode-example"}]).