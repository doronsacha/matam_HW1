#include <stdbool.h>
#include <stdlib.h>

#include "chessSystem.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "tournament.h"
#include "statistic.h"

//


void* copyGameKey(void* game_to_copy)
{
    if(game_to_copy==NULL)
    {
        return NULL;
    }
    int* new_id=malloc(sizeof (*new_id));
    if(new_id==NULL)
    {
        return NULL;
    }
    *new_id=*(int*)game_to_copy;
    return (void*)new_id;
}

void* copyGameData (void* game_data_to_copy)
{
    if(game_data_to_copy==NULL)
    {
        return NULL;
    }
    Game_data new_data=malloc(sizeof(*new_data));
    if(new_data==NULL)
    {
        return NULL;
    }
    new_data->player1_id=((Game_data)game_data_to_copy)->player1_id;
    new_data->player2_id=((Game_data)game_data_to_copy)->player2_id;
    new_data->result=((Game_data)game_data_to_copy)->result;
    new_data->time=((Game_data)game_data_to_copy)->time;
    return (void*)new_data;
}

void freeGameKey(void* game_id_to_free)
{

    free(game_id_to_free);
}

void freeGameData (void* game_data_to_free)
{
    Game_data to_free=(Game_data)game_data_to_free;
    free(to_free);
}


int compareTwoGames (void* game_id1, void* game_id2)
{
    if(*(int*)game_id1>*(int*)game_id2)
    {
        return POSITIVE_NUMBER;
    }

    if(*(int*)game_id1<*(int*)game_id2)
    {
        return NEGATIVE_NUMBER;
    }

    return NEUTRAL_NUMBER;
}


bool gameIdIsValid (int game_id)
{
    if(game_id<=0)
    {
        return false;
    }
    return true;
}

bool playerIdIsValid (int player_id)
{
    if(player_id<=0)
    {
        return false;

    }
    return true;
}

bool timeIsValid(int time)
{
    if(time<=0)
    {
        return false;
    }
    return true;
}

bool playedInAGame (Game_data game_data, int player_id)
{
    if(game_data==NULL || playerIdIsValid(player_id)==false)
    {
        return false;
    }
    if(player_id==game_data->player1_id || player_id==game_data->player2_id)
    {
        return true;
    }
    return false;
}

bool isValidMaxGame(int max_game_per_player)
{
    if(max_game_per_player<=0)
    {
        return false;
    }
    return true;
}

bool isValidWinner(Winner game_winner)
{
    if(game_winner== FIRST_PLAYER || game_winner==SECOND_PLAYER || game_winner==DRAW)
    {
        return true;
    }
    return false;
}


bool twoPlayedInAGame(Game_data game_data,int first_player_id,int second_player_id)
{
    if(playedInAGame(game_data,first_player_id)==true && playedInAGame(game_data,second_player_id)==true)
    {
        return true;
    }
    return false;
}


bool gameAlreadyExist(Tournament_data tournament_data,int first_player_id,int second_player_id)
{
    if(tournament_data==NULL)
    {
        return false;
    }
    MAP_FOREACH(int*,key,tournament_data->map_game)
    {
        Game_data new=mapGet(tournament_data->map_game,key);
        if(twoPlayedInAGame(new,first_player_id,second_player_id)==true)
        {
            if(mapContains(tournament_data->map_player, (void*)&first_player_id)==false || mapContains(tournament_data->map_player, (void*)&second_player_id)==false)
            {
                free(key);
                return false;
            }
            free(key);
            return true;
        }
        free(key);
    }
    return false;
}

int determinateGameId(Map game_map)
{
    if(game_map==NULL)
    {
        return -1;
    }
    int number_of_games= mapGetSize(game_map)+1;

    return number_of_games;
}

Map createGameMap()
{
    Map game_map=mapCreate(copyGameData,copyGameKey,freeGameData,freeGameKey,compareTwoGames);
    return game_map;
}

int returnWinnerIdOfGame(Game_data game_data)
{
    if(game_data->result==FIRST_PLAYER)
    {
        return game_data->player1_id;
    }
    else if(game_data->result==SECOND_PLAYER)
    {
        return game_data->player2_id;
    }
    return -1;
}

