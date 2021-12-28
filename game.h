#ifndef UNTITLED47_GAME_H
#define UNTITLED47_GAME_H

#define POSITIVE_NUMBER 2
#define NEGATIVE_NUMBER -2
#define NEUTRAL_NUMBER 0

#include "chessSystem.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "tournament.h"
#include "statistic.h"
//
typedef struct Game_node *Game;

typedef struct game_data *Game_data;

struct game_data
{
    int player1_id;
    int player2_id;
    Winner result;
    int time;
};

struct Game_node
{
    int* game_id;
    struct game_data* game_data;
    struct Game_node* next;
};


/**
 * ok
 * copy a game key
 * @param game_to_copy
 * @return
 */
void* copyGameKey(void* game_to_copy);


/**
 * ok
 * copy the game data (all the informations of the game)
 * @param game_to_copy
 * @return
 */
void* copyGameData (void* game_to_copy);


/**
 * ok
 *free the game id
 * @param game_to_free
 */
void freeGameKey(void* game_to_free);


/**
 * ok
 * free the game data
 * @param game_to_free
 */
void freeGameData (void* game_to_free);

/**
 * ok
 * compare two games by their id
 * @param game_id1
 * @param game_id2
 * @return
 */
int compareTwoGames (void* game_id1, void* game_id2);


/**
 * ok
 * @param game_id
 * @return
 */
bool gameIdIsValid (int game_id);

/**
 * ok
 * @param player_id
 * @return
 */
bool playerIdIsValid (int player_id);

/**
 * ok
 *verify that the time is a valid number
 * @param time
 * @return>>>>>>>>>>>>>>>>>>>>timeIsValid(jeu->game_data->time);
 */
bool timeIsValid(int time);


/**
 * ok
 *return true if a player (defined by his id) played in a game
 * @param game_data
 * @param player_id
 * @return
 */
bool playedInAGame(Game_data game_data, int player_id);

/**
 * ok
 * @param game_winner
 * @return
 */
bool isValidWinner(Winner game_winner);

/**
 * ok
 * @param max_game_per_player
 * @return
 */
bool isValidMaxGame(int max_game_per_player);

/**
 * ok
 * @param game_data
 * @param first_player_id
 * @param second_player_id
 * @return
 */
bool twoPlayedInAGame(Game_data game_data,int first_player_id,int second_player_id);


/**
 * ok>>>>> pb de if copy aloors on prend le meme id??? sinon top
 * >>>>>>>>determinateGameId(game_map); ne pas oublier de le mettre dans un pointeur
 *     int* dat;
    *dat=determinateGameId(game);
    * et ensuite de faire mapPut(game,dat,jeu_copy->game_data);
    * car mapPut recois un pointeur vers un int(int*)
 * @param game_map
 * @return
 */
int determinateGameId(Map game_map);

/**
 * okkkkk
 * @return
 */
Map createGameMap();

/**
 * ok - faire plus de bdikot
 * @param game_map
 * @param first_player_id
 * @param second_player_id
 * @return
 */
bool gameAlreadyExist(Tournament_data tournament_data,int first_player_id,int second_player_id);

int returnWinnerIdOfGame(Game_data game_data);
#endif //UNTITLED47_GAME_H
