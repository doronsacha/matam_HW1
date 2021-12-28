#ifndef UNTITLED47_TOURNAMENT_H
#define UNTITLED47_TOURNAMENT_H

//#include "chessSystem.h"
//#include "player.h"
//#include "game.h"
#include "map.h"
//#include "statistic.h"

#define FIRST_MAJUSCULE 'A'
#define LAST_MAJUSCULE 'Z'
#define FIRST_MINUSCULE 'a'
#define LAST_MINUSCULE 'z'

#define POSITIVE_NUMBER 2
#define NEGATIVE_NUMBER -2
#define NEUTRAL_NUMBER 0

//

typedef struct tournament_data *Tournament_data;

typedef struct Tournament_node *Tournament;

struct tournament_data
{
    char *tournament_location;
    int winner_id;
    int max_games_per_player;
    Map map_game;
    struct Game_node* game_iterator;
    Map map_player;
    struct player_stats* player_iterator;
};

struct Tournament_node
{
    int* id_tournament;
    Tournament_data data;
    Tournament next;
};

/**
 * copy the id tournament
 * @param tournament_id_to_copy
 * @return a pointer to a copy of the id
 */
void* copyTournamentKey (void*  tournament_id_to_copy);

/**
 * copy the data tournament
 * @param tournament_data_to_copy
 * @return a pointer to the copy of the data
 */
void*  copyTournamentData(void* tournament_data_to_copy);

/**
 * free the key tournament
 * @param tournament_id_to_free
 */
void freeTournamentKey (void* tournament_id_to_free);

/**
 * free the data tournament
 *
 * probleme pour max game per player sinon okk
 * @param tournament_data_to_free
 */
void freeTournamentData(void* tournament_data_to_free);

/**
 * compare two tournaments by their key (tournament id)
 * ok>>>>>>>>>>>>>>>>>
 * @param first_tournament_id
 * @param second_tournament_id
 * @return
 */
int compareTwoTournaments (void* first_tournament_id, void* second_tournament_id);

/**
 * create a map of tournaments
 * ok>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 * @return
 */
Map createTournamentMap();

/**
 * verify that the id is a valid number
 * ok>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 * @param tournament_id
 * @return
 */
bool tournamentIdIsValid(int tournament_id);

/**
 * verify that the id is a valid number
 *
 *                                              not yet
 * @param location
 * @return
 */
bool isValidLocation(const char *location);

/**
 * return true if a player (defined by his id is in the tournament)
 * @param tournament
 * @param playerId
 * @return
 */
bool tournamentPlayerContains(Tournament tournament,int playerId);//d

/**
 * return the number of games that a player played in a tournament
 * @param tournament
 * @param player_id
 * @return
 */
int numberOfGameByIdInATournament(Tournament tournament,int player_id);//a

/**
 * return true is the player played less that the max of games that he can play in a tournament
 * @param tournament
 * @param player_id
 * @return
 */
bool isValidNUmberOfGame(Tournament tournament,int player_id, int max_game);//a

/**
 * //return true if the player played in a tournament
 */
bool playedInATournament (Tournament_data data_tournament, int player_id);//d

/**
 * return true if a game (defined by is id is in a tournament
 */
bool isAGameInATournament (Tournament tournament, int game_id);//a

int returnWinnerId(Tournament_data tournament_data);

int findHighestGameTime (Tournament_data tournament_data);

double findAveragePlayTime (Tournament_data tournament_data);

char* returnLocation (Tournament_data tournament_data);

int returnNumberOfPlayers (Tournament_data tournament_data);

int returnNumberOfGames (Tournament_data tournament_data);

void printTournamentsStatistics (Tournament_data tournament_data, FILE* file);



#endif //UNTITLED47_TOURNAMENT_H