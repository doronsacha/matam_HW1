#include "chessSystem.h"
#include "player.h"

#include <stdlib.h>
#include "chessDeclaration.h"

//
#define NOT_ENDED_YET -1



bool CheckAreTournamentsEnd (ChessSystem chess)
{
    MAP_FOREACH(int*, key, chess->tournament_map)
    {
        Tournament_data data_temp=mapGet(chess->tournament_map, key);
        if(data_temp->winner_id!=NOT_ENDED_YET)
        {
            free(key);
            return true;
        }
        free(key);
    }
    return false;
}

void addPlayerLevel(ChessSystem chess)
{
    MAP_FOREACH(int*,key,chess->tournament_map)
    {
        Tournament_data tournament=mapGet(chess->tournament_map,key);
        MAP_FOREACH(int*, id, tournament->map_player)
        {
            double level = calculateLevelById(chess, *id);
            Player_data player = mapGet(tournament->map_player, id);
            player->level_of_player = level;
            player->final=ON_GAME;
            free(id);
        }
        free(key);
    }
}

double returnTheHighestLevel(ChessSystem chess)
{
    double max=INT_MIN;
    MAP_FOREACH(int*,key,chess->tournament_map)
    {
        Tournament_data  temp=mapGet(chess->tournament_map,key);
        MAP_FOREACH(int*,jey,temp->map_player)
        {
            Player_data player=mapGet(temp->map_player,jey);
            if(player->final==ON_GAME && player->level_of_player>=max)
            {
                max=player->level_of_player;
            }
            free(jey);
        }
        free(key);
    }
    return max;
}

int returnTheIdOfTheNext(ChessSystem chess)
{
    double max_level=returnTheHighestLevel(chess);
    int minimum_id=INT_MAX;
    MAP_FOREACH(int*,key,chess->tournament_map)
    {
        Tournament_data  tournament=mapGet(chess->tournament_map,key);
        MAP_FOREACH(int*,jey,tournament->map_player)
        {
            Player_data player=mapGet(tournament->map_player,jey);
            if(player->level_of_player==max_level && player->final==ON_GAME)
            {
                if(*jey<minimum_id)
                {
                    minimum_id=*jey;
                }
            }
            free(jey);
        }
        free(key);
    }
    return minimum_id;
}


void initializeStatsOfThisPlayer(ChessSystem chess,int player_id)
{
    MAP_FOREACH(int*, key, chess->tournament_map)
    {
        Tournament_data tournament = mapGet(chess->tournament_map, key);
        MAP_FOREACH(int*, jey, tournament->map_player)
        {
            Player_data player = mapGet(tournament->map_player, jey);
            if (*jey == player_id)
            {
                player->final = FINISHED;
                player->level_of_player=0;
            }
            free(jey);
        }
        free(key);
    }
}

double returnTheBaddestLevel(ChessSystem chess)
{
    double min=(double)INT_MAX;
    MAP_FOREACH(int*,key,chess->tournament_map)
    {
        Tournament_data  temp=mapGet(chess->tournament_map,key);
        MAP_FOREACH(int*,jey,temp->map_player)
        {
            Player_data player=mapGet(temp->map_player,jey);
            if(player->level_of_player<=min)
            {
                min=player->level_of_player;
            }
            free(jey);
        }
        free(key);
    }
    return min;
}

int returnIdOfTheLastPlayer (ChessSystem chess)
{
    double min_level=returnTheBaddestLevel(chess);
    int maximum_id=NEGATIVE_NUMBER;
    MAP_FOREACH(int*,key,chess->tournament_map)
    {
        Tournament_data  tournament=mapGet(chess->tournament_map,key);
        MAP_FOREACH(int*,jey,tournament->map_player)
        {
            Player_data player=mapGet(tournament->map_player,jey);
            if(player->level_of_player==min_level)
            {
                if(*jey>maximum_id)
                {
                    maximum_id=*jey;
                }
            }
            free(jey);
        }
        free(key);
    }
    return maximum_id;
}
