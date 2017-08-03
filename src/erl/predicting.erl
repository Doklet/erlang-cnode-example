-module(predicting).

-export([
	predict/1,
	predict_test/0
]).

predict(FilePath) when is_list(FilePath) ->
	Msg = {predict, unicode:characters_to_binary(FilePath)},
	cnode:call(Msg).

predict_test() ->
	predict("SomePath/MyFile.jpg").