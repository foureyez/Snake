%% Author: smarr
%% Created: Dec 9, 2008
%% Description: This module initializes the Snake application and spawns all processes.
%%   The following active componets/processes/actors are started:
%%		Terminal: for interaction
%%		GameMaster:	enforcing game rules and executing the game itself
-module(main).

%%
%% Exported Functions
%%
-export([start/0]).

%%
%% API Functions
%%
start() ->
	board_view:init(),
    
    game:start(human),
    game:start(ai),
    game:start(ai),
    game:start(ai),
    game:start(ai),
    game:start(ai),
    game:start(ai),
    game:start(ai),



    board_view:start().