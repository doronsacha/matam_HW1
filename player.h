
#ifndef UNTITLED47_PLAYER_H
#define UNTITLED47_PLAYER_H

#include "chessSystem.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "tournament.h"
#include "statistic.h"

#include <limits.h>


//
typedef struct player_stats *Player_stats;

typedef struct player_data *Player_data;

struct player_data
{
    int number_of_wins;
    int number_of_losses;
    int number_of_equals;
    double level_of_player;
    int score_of_player;
    int place_of_player;
    Situation final;
};

struct player_stats
{
    int* player_id;
    struct player_data* data_of_player;
    struct player_stats* next;
};

/**
 * ok
 * return a pointer to a copy of the player key (id)
 * @param player_id_to_copy-the id that we want to copy.
 * @return- a new pointer that have the same value of the player_id_to_copy
 */
void* copyPlayerId (void* player_id_to_copy);

/**
 * ok
 * return a pointer to a copy of the data player
 * @param player_data_to_copy.
 * @return
 */
void* copyDataPlayer (void* player_data_to_copy);

/**
 * ok
 * free the player key (id)
 * @param player_id_to_free.
 */
void freePlayerId (void* player_id_to_free);

/**
 * ok
 * free the data player
 * @param player_data_to_free
 */
void freeDataPlayer(void* player_data_to_free);


/**
 * ok
 * compare two players by their id
 * @param first_player_id
 * @param second_player_id
 * @return
 */
int compareTwoPlayers(void* first_player_id, void* second_player_id);

/**
 * ok
 * create a map of player
 * @return the map
 */
Map createPlayerMap();

/**
 * ok
 *calculate the score of a player
 * @param player
 * @return his score.
 */

int calculatePoints (Player_data player);

double calculateLevelById (ChessSystem chess, int player_id);

/**
 *ok
 * @param the data of the player
 * @return the number of games that a player did
 */
int numberOfGameByPlayer(Tournament_data tournament_data, int player_id);

/**
 * a tester
 * verify if a player didnt play more games tham what is permitted
 * @param tournament_to_max
 * @param player_id
 * @return true or false
 */
bool notExceededGamePlayer(Tournament_data tournament_to_max,int player_id);

/** a tester
 *update the statistics of the two players that played in a game after they played. update in the map player
 * @param tournament_data
 * @param player_id1
 * @param player_id2
 * @param result
 */

void addNewPlayer (Map map_player, int player_id);


void updatePlayersStatisticsAfterGame (Tournament_data tournament_data, int player_id1, int player_id2, Winner result);

/** a tester
 * change the winner of a game and update the statitics.
 * @param game_data
 * @param first_data_player
 * @param second_data_player
 */
void changeWinnerOfAGame (Game_data game_data, Player_data first_data_player, Player_data second_data_player, int player_id);

void changeWinnerOfGameIfDraw (Game_data game_data, Player_data first_data_player, Player_data second_data_player, int player_id);

int calculatePointsForOnePlayer (Player_data player);

void updatePointsOfAllThePlayers (Map map_player);

int findMaxScore(Map player_map);

int findMinimumNumberOfLosses(Map player_map, int max_score);

int findMaximumOfWins (Map player_map, int minimum_losses);

int findMinimumID(Map player_map, int maximum_wins);

/** a tester
 * find in a tournament who is the winner by using the 4 fonctions of conditions
 * @param player_map
 * @return
 */
int findWinnerID (Map player_map);


#endif //UNTITLED47_PLAYER_H

