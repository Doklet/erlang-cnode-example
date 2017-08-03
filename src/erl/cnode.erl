-module(cnode).

-export([call/1]).

call(Msg) ->
    {ok, Hostname} = inet:gethostname(),
    {any, list_to_atom(lists:append(["c1@", Hostname]))} ! {call, self(), Msg},
    receive
	{cnode, Result} ->
	    Result
    end.