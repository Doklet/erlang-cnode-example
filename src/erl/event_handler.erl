-module(event_handler).

-export([ping/0]).

ping() ->
	io:format("Howdy\n"),
	666.