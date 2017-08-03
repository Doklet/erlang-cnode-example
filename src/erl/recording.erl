-module(recording).

-export([
	record/1,
	record_test/0
]).

record(DataSetCategoryPath) when is_list(DataSetCategoryPath) ->
	Msg = {record, unicode:characters_to_binary(DataSetCategoryPath)},
	cnode:call(Msg).

record_test() ->
	record("Record").