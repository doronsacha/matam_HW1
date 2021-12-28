//
#include "chessSystem.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "tournament.h"
#include "statistic.h"
#include "chessDeclaration.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define NOT_ENDED_YET -1




static bool playerExists (ChessSystem chess, int player_id);


ChessSystem chessCreate()
{
    ChessSystem new_chess=malloc(sizeof (*new_chess));
    if(new_chess==NULL)
    {
        return NULL;
    }
    new_chess->tournament_map=createTournamentMap();
    if(new_chess->tournament_map==NULL)
    {
        return NULL;
    }
    new_chess->total_games=0;
    return new_chess;
}


void chessDestroy(ChessSystem chess)
{
    if(chess==NULL)
    {
        return ;
    }
    mapDestroy(chess->tournament_map);
    free(chess);
}

ChessResult chessAddTournament (ChessSystem chess, int tournament_id,int max_games_per_player, const char* tournament_location)
{
    if(chess==NULL || tournament_location==NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournamentIdIsValid(tournament_id)==false)
    {
        return CHESS_INVALID_ID;
    }
    if(mapContains(chess->tournament_map,&(tournament_id))==true)
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    if(isValidLocation(tournament_location)==false)
    {
        return CHESS_INVALID_LOCATION;
    }
    if(isValidMaxGame(max_games_per_player)==false)
    {
        return CHESS_INVALID_MAX_GAMES;
    }
    Tournament_data new_data=malloc(sizeof(*new_data));
    if(new_data==NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    new_data->tournament_location=malloc(sizeof(char)*(strlen(tournament_location)+1));
    if(new_data->tournament_location==NULL)
    {
        free(new_data);
        return CHESS_OUT_OF_MEMORY;
    }
    strcpy(new_data->tournament_location, tournament_location);

    new_data->max_games_per_player=max_games_per_player;
    new_data->winner_id=NOT_ENDED_YET;
    new_data->map_game=createGameMap();
    if(new_data->map_game==NULL)
    {
        free(new_data->tournament_location);
        free(new_data);
        return CHESS_OUT_OF_MEMORY;
    }

    new_data->map_player=createPlayerMap();
    if(new_data->map_player==NULL)
    {
        mapDestroy(new_data->map_game);
        free(new_data->tournament_location);
        free(new_data);
        return CHESS_OUT_OF_MEMORY;
    }
    //   new_data->game_iterator=map_;
    // new_data->player_iterator=NULL;


    MapResult add_tournament=mapPut(chess->tournament_map,(void*)&tournament_id,(void*)new_data);
    if(add_tournament==MAP_OUT_OF_MEMORY)
    {
        freeTournamentData(new_data);
        return CHESS_OUT_OF_MEMORY;
    }
    freeTournamentData(new_data);
    return CHESS_SUCCESS;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,int second_player, Winner winner, int play_time)
{
    if(chess==NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournamentIdIsValid(tournament_id)==false || playerIdIsValid(first_player)==false || playerIdIsValid(second_player)==false || isValidWinner(winner)==false || first_player==second_player)
    {
        return CHESS_INVALID_ID;
    }
    if(mapContains(chess->tournament_map,(void*)&tournament_id)==false)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if(timeIsValid(play_time)==false)
    {
        return CHESS_INVALID_PLAY_TIME;
    }
    Tournament_data temp=mapGet(chess->tournament_map,(void*)&tournament_id);

    if(gameAlreadyExist(temp,first_player,second_player)==true)
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }
    if(notExceededGamePlayer(temp,first_player)==false || notExceededGamePlayer(temp, second_player)==false)
    {
        return CHESS_EXCEEDED_GAMES;
    }

    if(temp->winner_id!=NOT_ENDED_YET)
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    Game_data game_data_temp=malloc(sizeof(*game_data_temp));
    game_data_temp->player1_id=first_player;
    game_data_temp->player2_id=second_player;
    game_data_temp->result=winner;
    game_data_temp->time=play_time;

    int temp_game_id=determinateGameId(temp->map_game);
    if(temp_game_id==0)
    {
        free(game_data_temp);
        return CHESS_NULL_ARGUMENT;
    }
    MapResult temp_result=mapPut(temp->map_game,(void*)&temp_game_id,(void*)game_data_temp);
    if(temp_result==MAP_OUT_OF_MEMORY)
    {
        free(game_data_temp);
        return CHESS_OUT_OF_MEMORY;
    }
    free(game_data_temp);
    updatePlayersStatisticsAfterGame(temp,first_player,second_player,winner);
    chess->total_games++;

    return CHESS_SUCCESS;
}


ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id)
{
    if(chess==NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournamentIdIsValid(tournament_id)==false)
    {
        return CHESS_INVALID_ID;
    }
    if(mapContains(chess->tournament_map,(void*)&tournament_id)==false)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    Tournament_data temp=mapGet(chess->tournament_map,(void*)&tournament_id);

    int number_of_games_in_the_tournament= mapGetSize(temp->map_game);

    mapRemove(chess->tournament_map, (void*)&tournament_id);

    chess->total_games=chess->total_games-number_of_games_in_the_tournament;

    return CHESS_SUCCESS;
}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if(chess==NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if(playerIdIsValid(player_id)==false)
    {
        return CHESS_INVALID_ID;
    }

    if(playerExists(chess, player_id)==false)
    {
        return CHESS_PLAYER_NOT_EXIST;
    }

    MAP_FOREACH(int*,key,chess->tournament_map)
    {
        Tournament_data tournament_data_temp=mapGet(chess->tournament_map, key);
        if(mapContains(tournament_data_temp->map_player, (void*)&player_id)==true)
        {
            if(tournament_data_temp->winner_id==NOT_ENDED_YET)
            {
                MAP_FOREACH(int*, jey, tournament_data_temp->map_game)
                {
                    Game_data game_data_temp=mapGet(tournament_data_temp->map_game, jey);

                    if(playedInAGame(game_data_temp, player_id)==true)
                    {
                        Player_data first_player_data=mapGet(tournament_data_temp->map_player, (void*)&(game_data_temp->player1_id));
                        Player_data second_player_data=mapGet(tournament_data_temp->map_player, (void*)&(game_data_temp->player2_id));

                        if(game_data_temp->result!=DRAW)
                        {
                            changeWinnerOfAGame(game_data_temp, first_player_data,second_player_data, player_id);
                        }
                        else
                        {
                            changeWinnerOfGameIfDraw(game_data_temp, first_player_data, second_player_data, player_id);
                        }
                    }
                    free(jey);
                }
                mapRemove(tournament_data_temp->map_player, (void*)&player_id);
            }
            mapRemove(tournament_data_temp->map_player, (void*)&player_id);
        }
        free(key);
    }
    return CHESS_SUCCESS;
}


ChessResult chessEndTournament (ChessSystem chess, int tournament_id)
{
    if(chess==NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournamentIdIsValid(tournament_id)==false)
    {
        return CHESS_INVALID_ID;
    }

    if(mapContains(chess->tournament_map, (void*)&tournament_id)==false)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    Tournament_data tournament_to_end=mapGet(chess->tournament_map,(void*)&tournament_id);

    if(tournament_to_end->winner_id!=NOT_ENDED_YET)
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    updatePointsOfAllThePlayers(tournament_to_end->map_player);

    int id_of_the_winner= findWinnerID(tournament_to_end->map_player);

    tournament_to_end->winner_id=id_of_the_winner;
    return CHESS_SUCCESS;
}

double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result)
{
    if(chess==NULL)
    {
        *chess_result=CHESS_NULL_ARGUMENT;
        return 0;
    }

    if(playerIdIsValid(player_id)==false)
    {
        *chess_result=CHESS_INVALID_ID;
        return 0;
    }

    if(playerExists(chess, player_id)==false)
    {
        *chess_result=CHESS_PLAYER_NOT_EXIST;
        return 0;
    }
    double time_total=0;
    double number_total_games=0;
    MAP_FOREACH(int*,key,chess->tournament_map)
    {
        Tournament_data temp_tournament_data=mapGet(chess->tournament_map,key);
        if(playedInATournament(temp_tournament_data,player_id)==true)
        {
            MAP_FOREACH(int*,game_key,temp_tournament_data->map_game)
            {
                Game_data temp_game_data=mapGet(temp_tournament_data->map_game,game_key);
                if(playedInAGame(temp_game_data,player_id)==true)
                {
                    time_total+=temp_game_data->time;
                    number_total_games++;
                }
                free(game_key);
            }


        }
        free(key);
    }
    double average_time=(double)time_total/number_total_games;
    *chess_result=CHESS_SUCCESS;
    return average_time;
}

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    if(chess==NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if(CheckAreTournamentsEnd(chess)==false)
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }

    FILE* statistics_tournaments_file= fopen(path_file,"w");
    if(statistics_tournaments_file==NULL)
    {
        return CHESS_SAVE_FAILURE;
    }
    MAP_FOREACH(int*, key,chess->tournament_map)
    {
        Tournament_data temp_tournament_data=mapGet(chess->tournament_map, key);
        if(temp_tournament_data->winner_id!=NOT_ENDED_YET)
        {
            fprintf(statistics_tournaments_file,"%d\n",returnWinnerId(temp_tournament_data));
            fprintf(statistics_tournaments_file,"%d\n",findHighestGameTime(temp_tournament_data));
            fprintf(statistics_tournaments_file,"%.2f\n", findAveragePlayTime(temp_tournament_data));
            fprintf(statistics_tournaments_file,"%s\n",(temp_tournament_data->tournament_location));
            fprintf(statistics_tournaments_file,"%d\n",returnNumberOfGames(temp_tournament_data));
            fprintf(statistics_tournaments_file,"%d\n",returnNumberOfPlayers(temp_tournament_data));
        }
        free(key);
    }
    fclose(statistics_tournaments_file);
    return CHESS_SUCCESS;
}

ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)
{
    if (chess == NULL || file == NULL) {
        return CHESS_SAVE_FAILURE;
    }
    //file is already Open
    addPlayerLevel(chess);
    int id_of_the_next_to_print=NEGATIVE_NUMBER;
    int id_of_the_last_player_to_print= returnIdOfTheLastPlayer(chess);

    while(id_of_the_next_to_print!=id_of_the_last_player_to_print)
    {
        id_of_the_next_to_print = returnTheIdOfTheNext(chess);
        //  fprintf(file, "%d ", id_of_the_next_to_print);
        MAP_FOREACH(int*, key, chess->tournament_map)
        {
            Tournament_data temp_data=mapGet(chess->tournament_map, key);
            if(mapContains(temp_data->map_player, &id_of_the_next_to_print)==true)
            {
                Player_data temp_player=mapGet(temp_data->map_player, &id_of_the_next_to_print);
                double level_to_print=temp_player->level_of_player;
                fprintf(file,"%d %.2f\n",id_of_the_next_to_print, level_to_print);
                initializeStatsOfThisPlayer(chess, id_of_the_next_to_print);
            }
            freeTournamentKey(key);
            break;


            free(key);
        }
    }
    return CHESS_SUCCESS;
}


static bool playerExists (ChessSystem chess, int player_id)
{
    MAP_FOREACH(int*, key, chess->tournament_map)
    {
        Tournament_data temp_tournament_data=mapGet(chess->tournament_map, key);
        if(playedInATournament(temp_tournament_data,player_id)==true)
        {
            free(key);
            return true;
        }
        free(key);
    }
    return false;
}

