
OTPROOT=$(wildcard /usr/local/lib/erlang/lib/erl_interface-*)

# all:	bin/cnodeserver bin/cnodeclient bin/complex3.beam bin/cnode.beam bin/recording.beam bin/predicting.beam
all:	compile bin/complex3.beam bin/cnode.beam bin/recording.beam bin/predicting.beam

bin/%.beam:	src/erl/%.erl
	erlc -o bin $<

# bin/%:	src/c/%.c
# 	mkdir -p bin
# 	gcc -o $@ -I$(OTPROOT)/include -L$(OTPROOT)/lib src/c/complex.c src/c/recording.c src/c/predicting.c $< -lerl_interface -lei -lpthread

compile:
	mkdir -p bin
	g++ -std=c++11 -o bin/cppnodeserver -I$(OTPROOT)/include -L$(OTPROOT)/lib src/cpp/CommHandler.cpp src/cpp/CommandHandler.cpp src/cpp/main.cpp $< -lerl_interface -lei -lpthread

clean:
	rm -rf bin


start_server:
	epmd -daemon
	bin/cppnodeserver 3456

start_client:
	echo "run 'complex3:foo(4).' via erlang shell"
	erl -sname e1 -setcookie secretcookie -pa bin
