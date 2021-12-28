#include <stdio.h>
#include <stdlib.h>

#include "chessSystem.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "tournament.h"
#include "statistic.h"
#include "chessDeclaration.h"

#define NO_CALCULATED_YET -3
//

void* copyPlayerId (void* player_id_to_copy)
{
    if(player_id_to_copy==NULL)
    {
        return NULL;
    }
    int* id_copied = malloc(sizeof(*id_copied));
    if(id_copied==NULL)
    {
        return NULL;
    }
    *id_copied=*(int*)player_id_to_copy;
    return (void*) id_copied;
}

void* copyDataPlayer (void* player_data_to_copy)
{
    if(player_data_to_copy==NULL)
    {
        return NULL;
    }
    Player_data new_copy=malloc(sizeof(*new_copy));
    if(new_copy==NULL)
    {
        return NULL;
    }
    new_copy->number_of_wins=((Player_data)player_data_to_copy)->number_of_wins;
    new_copy->number_of_equals=((Player_data)player_data_to_copy)->number_of_equals;
    new_copy->number_of_losses=((Player_data)player_data_to_copy)->number_of_losses;
    new_copy->level_of_player=((Player_data)player_data_to_copy)->level_of_player;
    new_copy->place_of_player=((Player_data)player_data_to_copy)->place_of_player;
    new_copy->final=((Player_data)player_data_to_copy)->final;
    new_copy->score_of_player=((Player_data)player_data_to_copy)->score_of_player;

    return (void*) new_copy;
}



void freePlayerId (void* player_id_to_free)
{
    free(player_id_to_free);
}

void freeDataPlayer(void* player_data_to_free)
{
    Player_data to_free=(Player_data) player_data_to_free;
    free(to_free);
}

int compareTwoPlayers(void* first_player_id, void* second_player_id)
{
    if(first_player_id==NULL || second_player_id==NULL)
    {
        return -1;
    }
    int first_id=*(int*)first_player_id;
    int second_id=*(int*)second_player_id;

    if(first_id>second_id)
    {
        return POSITIVE_NUMBER;
    }

    if(first_id<second_id)
    {
        return NEGATIVE_NUMBER;
    }
    return NEUTRAL_NUMBER;
}

Map createPlayerMap()
{
    Map player_map=mapCreate(copyDataPlayer,copyPlayerId,freeDataPlayer,freePlayerId,compareTwoPlayers);
    return player_map;
}

double calculateLevelById (ChessSystem chess, int player_id)
{
    double number_of_wins=0, number_of_draws=0, number_of_losses=0, number_of_games=0;

    MAP_FOREACH(int*, key, chess->tournament_map)
    {
        Tournament_data temp_tournament_data=mapGet(chess->tournament_map, key);
        if(mapContains(temp_tournament_data->map_player, (void*)&player_id)==true)
        {
            Player_data  temp_player_data=mapGet(temp_tournament_data->map_player, (void*)&player_id);
            number_of_wins+=temp_player_data->number_of_wins;
            number_of_losses+=temp_player_data->number_of_losses;
            number_of_draws+=temp_player_data->number_of_equals;
            number_of_games+=(temp_player_data->number_of_wins+temp_player_data->number_of_losses+temp_player_data->number_of_equals);
        }
        free(key);
    }
    double level= (6*number_of_wins-10*number_of_losses+2*number_of_draws)/number_of_games;
    return level;
}


int numberOfGameByPlayer(Tournament_data tournament_data, int player_id)
{
    if(tournament_data==NULL || mapContains(tournament_data->map_player, (void*)&player_id)==false)
    {
        return 0;
    }


    Player_data temp_player_data = mapGet(tournament_data->map_player, (void*)&player_id);

    int number_of_games= temp_player_data->number_of_wins+temp_player_data->number_of_losses+temp_player_data->number_of_equals;

    return number_of_games;
}

bool notExceededGamePlayer(Tournament_data tournament_to_max,int player_id)
{
    if(tournament_to_max==NULL)
    {
        return NULL;
    }

    if(numberOfGameByPlayer(tournament_to_max, player_id)>=tournament_to_max->max_games_per_player)
    {
        return false;
    }

    return true;
}

void addNewPlayer (Map map_player, int player_id)
{
    Player_data initial_data_player=malloc(sizeof(*initial_data_player));
    initial_data_player->number_of_wins=0;
    initial_data_player->number_of_losses=0;
    initial_data_player->number_of_equals=0;
    initial_data_player->level_of_player=NO_CALCULATED_YET;
    initial_data_player->final=ON_GAME;
    initial_data_player->score_of_player=NO_CALCULATED_YET;

    mapPut(map_player,(void*)&player_id, (void*)initial_data_player);
    free(initial_data_player);
}

void updatePlayersStatisticsAfterGame (Tournament_data tournament_data, int player_id1, int player_id2, Winner result)
{


    if(mapContains(tournament_data->map_player, (void*)&player_id1)==false)
    {
        addNewPlayer(tournament_data->map_player, player_id1);
    }

    if(mapContains(tournament_data->map_player, (void*)&player_id2)==false)
    {
        addNewPlayer(tournament_data->map_player, player_id2);
    }

    Player_data first_player_data= mapGet(tournament_data->map_player, (void*)&player_id1);
    Player_data second_player_data=mapGet(tournament_data->map_player, (void*)&player_id2);

    if(result==FIRST_PLAYER)
    {
        first_player_data->number_of_wins++;
        second_player_data->number_of_losses++;
    }
    else if(result==SECOND_PLAYER)
    {
        second_player_data->number_of_wins++;
        first_player_data->number_of_losses++;
    }
    else
    {
        first_player_data->number_of_equals++;
        second_player_data->number_of_equals++;
    }
}

void changeWinnerOfAGame (Game_data game_data, Player_data first_data_player, Player_data second_data_player, int player_id)
{
    if(game_data->result==FIRST_PLAYER && game_data->player1_id==player_id)
    {
        game_data->result=SECOND_PLAYER;
        first_data_player->number_of_wins--;
        first_data_player->number_of_losses++;
        second_data_player->number_of_losses--;
        second_data_player->number_of_wins++;
    }

    if(game_data->result==SECOND_PLAYER && game_data->player2_id==player_id)
    {
        game_data->result=FIRST_PLAYER;
        second_data_player->number_of_wins--;
        second_data_player->number_of_losses++;
        first_data_player->number_of_losses--;
        first_data_player->number_of_wins++;
    }
}


void changeWinnerOfGameIfDraw (Game_data game_data, Player_data first_data_player, Player_data second_data_player, int player_id)
{

    if(game_data->player1_id==player_id)
    {
        first_data_player->number_of_losses++;
        first_data_player->number_of_equals--;
        second_data_player->number_of_wins++;
        second_data_player->number_of_equals--;
    }
    if(game_data->player2_id==player_id)
    {
        second_data_player->number_of_losses++;
        second_data_player->number_of_equals--;
        first_data_player->number_of_wins++;
        first_data_player->number_of_equals--;
    }
}

int calculatePointsForOnePlayer (Player_data player)
{
    int number_of_games=player->number_of_wins+player->number_of_equals+player->number_of_losses;
    int result= (2*player->number_of_wins + player->number_of_equals)/number_of_games;
    return result;
}

void updatePointsOfAllThePlayers (Map map_player)
{
    MAP_FOREACH(int*, key, map_player)
    {
        Player_data temp_player_data=mapGet(map_player,key);
        temp_player_data->score_of_player=calculatePointsForOnePlayer(temp_player_data);
        free(key);
    }
}

int findMaxScore(Map player_map)
{

    int max=NEUTRAL_NUMBER;

    MAP_FOREACH(int*, key, player_map)
    {
        Player_data temp_player_data=mapGet(player_map, key);
        int temp_score=temp_player_data->score_of_player;
        if(temp_score>=max)
        {
            max=temp_score;
        }
        free(key);
    }
    return max;
}

int findMinimumNumberOfLosses(Map player_map, int max_score)
{
    int minimum_losses=INT_MAX;
    MAP_FOREACH(int*, key, player_map)
    {
        Player_data temp_player_data=mapGet(player_map, key);
        if(temp_player_data->score_of_player==max_score)
        {
            if(temp_player_data->number_of_losses<=minimum_losses)
            {
                minimum_losses=temp_player_data->number_of_losses;
            }
        }
        free(key);
    }
    return minimum_losses;
}


int findMaximumOfWins (Map player_map, int minimum_losses)
{
    int maximum_wins=NEUTRAL_NUMBER;
    MAP_FOREACH(int*, key, player_map)
    {
        Player_data temp_player_data=mapGet(player_map, key);
        if(temp_player_data->number_of_losses==minimum_losses)
        {
            if(temp_player_data->number_of_wins>=maximum_wins)
            {
                maximum_wins=temp_player_data->number_of_wins;
            }
        }
        free(key);
    }
    return maximum_wins;
}


int findMinimumID(Map player_map, int maximum_wins)
{
    int minimum_id=INT_MAX;
    MAP_FOREACH(int*, key, player_map)
    {
        Player_data temp_player_data=mapGet(player_map, key);

        if(temp_player_data->number_of_wins==maximum_wins)
        {
            if(*key<=minimum_id)
            {
                minimum_id=*key;
            }
        }
        free(key);
    }
    return minimum_id;
}


int findWinnerID (Map player_map)
{

    int max_score=findMaxScore(player_map);
    int minimum_losses=findMinimumNumberOfLosses(player_map, max_score);
    int maximum_wins=findMaximumOfWins(player_map, minimum_losses);

    //By the score
    int counter=0;
    int player_id=0;
    MAP_FOREACH(int*, key, player_map)
    {
        Player_data temp_player_data=mapGet(player_map,key);
        if(temp_player_data->score_of_player==max_score)
        {
            counter++;
            player_id=*key;
        }
        free(key);
    }
    if(counter==1)
    {
        return player_id;
    }

    //By the number of losses
    counter=0;
    player_id=0;
    MAP_FOREACH(int*, key, player_map)
    {
        Player_data temp_player_data=mapGet(player_map,key);
        if(temp_player_data->number_of_losses==minimum_losses)
        {
            counter++;
            player_id=*key;
        }
        free(key);

    }
    if(counter==1)
    {
        return player_id;
    }

    //By the number of wins
    counter=0;
    player_id=0;
    MAP_FOREACH(int*, key, player_map)
    {
        Player_data temp_player_data=mapGet(player_map,key);
        if(temp_player_data->number_of_wins==maximum_wins)
        {
            counter++;
            player_id=*key;
        }
        free(key);
    }
    if(counter==1)
    {
        return player_id;
    }

    //else by the id
    player_id=findMinimumID(player_map,maximum_wins);
    return player_id;
}
