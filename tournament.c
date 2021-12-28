
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "chessSystem.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "tournament.h"
#include "statistic.h"
//
void* copyTournamentKey (void*  tournament_id_to_copy)
{
    if(tournament_id_to_copy==NULL)
    {
        return NULL;
    }

    int* id_copied = malloc(sizeof(*id_copied));
    if(id_copied==NULL)
    {
        return NULL;
    }
    *id_copied=*(int*)tournament_id_to_copy;
    return (void*) id_copied;
}

void*  copyTournamentData(void* tournament_data_to_copy)
{
    if (tournament_data_to_copy == NULL)
    {
        return NULL;
    }
    Tournament_data new_tournament_data = malloc(sizeof(*new_tournament_data));
    if (new_tournament_data == NULL)
    {
        return NULL;
    }

    //copy the location
    unsigned int number_of_letters=strlen(((Tournament_data)tournament_data_to_copy)->tournament_location);
    new_tournament_data->tournament_location=malloc(sizeof(char)*(number_of_letters+1));
    strcpy(new_tournament_data->tournament_location, ((Tournament_data)tournament_data_to_copy)->tournament_location);

    //copy max_games
    new_tournament_data->max_games_per_player=((Tournament_data)tournament_data_to_copy)->max_games_per_player;

    //copy result
    new_tournament_data->winner_id = ((Tournament_data)tournament_data_to_copy)->winner_id;


    //copy the map of games
    new_tournament_data->map_game=mapCopy(((Tournament_data)tournament_data_to_copy)->map_game);
    if(new_tournament_data->map_game==NULL)
    {
        free(new_tournament_data);
        return NULL;
    }
    //copy all the map of players
    new_tournament_data->map_player=mapCopy(((Tournament_data)tournament_data_to_copy)->map_player);
    if(new_tournament_data->map_player==NULL)
    {
        mapDestroy(new_tournament_data->map_game);
        free(new_tournament_data);
        return NULL;
    }
    return (void*)new_tournament_data;
}


void freeTournamentKey (void* tournament_id_to_free)
{
    free(tournament_id_to_free);
}


void freeTournamentData(void* tournament_data_to_free)
{
    if (tournament_data_to_free == NULL)
    {
        return;
    }

    Tournament_data to_free=(Tournament_data) tournament_data_to_free;
    //free the location
    free(to_free->tournament_location);
    //free the map of games
    mapDestroy(to_free->map_game);
    //free the map of players
    mapDestroy(to_free->map_player);
    free(to_free);
    // free(tournament_data_to_free);
}

int compareTwoTournaments(void* first_tournament_id, void* second_tournament_id)
{
    if(first_tournament_id==NULL || second_tournament_id==NULL)
    {
        return NEUTRAL_NUMBER;
    }
    int first_id=*(int*)first_tournament_id;
    int second_id=*(int*)second_tournament_id;

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

Map createTournamentMap()
{
    Map tournament_map=mapCreate(copyTournamentData,copyTournamentKey,freeTournamentData,freeTournamentKey,compareTwoTournaments);

    return tournament_map;
}


bool tournamentIdIsValid(int tournament_id)
{

    if(tournament_id<=0)
    {
        return false;
    }
    return true;
}

bool isValidLocation(const char *location)
{
    if(*location<FIRST_MAJUSCULE || *location>LAST_MAJUSCULE )
    {
        return false;
    }
    for(int i=1; i<strlen(location);i++)
    {
        if(*(location+i)==' ')
        {
            continue;
        }
        if (*(location+i)<FIRST_MINUSCULE || *(location+i)>LAST_MINUSCULE)
        {
            return false;
        }
    }
    return true;
}


bool tournamentPlayerContains(Tournament tournament,int playerId)
{
    if (tournament==NULL)
    {
        return false;
    }
    if(mapContains(tournament->data->map_player, (void*)&playerId)==false)
    {
        return false;
    }

    return true;
}

int numberOfGameByIdInATournament(Tournament tournament,int player_id)
{
    if(tournament==NULL || playerIdIsValid(player_id)==false)
    {
        return 0;
    }
    if(mapContains(tournament->data->map_player, (void*)&player_id)==false)
    {
        return 0;
    }

    int counter=0;

    MAP_FOREACH(int*, key, tournament->data->map_game)
    {
        Game_data temp_game_data=mapGet(tournament->data->map_game, key);
        if(playedInAGame(temp_game_data,player_id))
        {
            counter++;
        }
    }
    return counter;
}

bool isValidNUmberOfGame(Tournament tournament,int player_id, int max_game)
{
    if(tournament==NULL || playerIdIsValid(player_id)==false || max_game <=0)
    {
        return false;
    }
    if(numberOfGameByIdInATournament(tournament,player_id)>max_game)
    {
        return false;
    }
    return true;
}


bool playedInATournament (Tournament_data data_tournament, int player_id)
{
    if(data_tournament==NULL || tournamentIdIsValid(player_id)==false)
    {
        return false;
    }

    if(mapContains(data_tournament->map_player, (void*)&player_id)==true)
    {
        return true;
    }
    return false;
}

bool isAGameInATournament (Tournament tournament, int game_id)
{
    if(tournament==NULL || gameIdIsValid(game_id)==false)
    {
        return false;
    }

    if(mapContains(tournament->data->map_game, (void*)&game_id)==true)
    {
        return true;
    }
    return false;
}

int returnWinnerId(Tournament_data tournament_data)
{
    int winner=tournament_data->winner_id;
    return winner;
}

int findHighestGameTime (Tournament_data tournament_data)
{
    int max_time=0;
    MAP_FOREACH(int*, key, tournament_data->map_game)
    {
        Game_data temp_game_data=mapGet(tournament_data->map_game, key);
        if(temp_game_data->time>=max_time)
        {
            max_time=temp_game_data->time;
        }
        free(key);
    }
    return max_time;
}

double findAveragePlayTime (Tournament_data tournament_data)
{
    double total_time=0;
    double number_of_games=0;
    MAP_FOREACH(int*, key, tournament_data->map_game)
    {
        Game_data temp_game_data=mapGet(tournament_data->map_game, key);
        total_time+=temp_game_data->time;
        number_of_games++;
        free(key);
    }
    double average=total_time/number_of_games;
    return average;
}


int returnNumberOfPlayers (Tournament_data tournament_data)
{
    int number_of_players=mapGetSize(tournament_data->map_player);
    return number_of_players;
}

int returnNumberOfGames (Tournament_data tournament_data)
{
    int number_of_games=mapGetSize(tournament_data->map_game);
    return number_of_games;
}

