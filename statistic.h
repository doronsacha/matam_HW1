#ifndef STATISTICS_H
#define STATISTICS_H
#include "chessSystem.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "tournament.h"
#include "statistic.h"

//
typedef enum {
    FINISHED,
    ON_GAME
} Situation;


bool CheckAreTournamentsEnd (ChessSystem chess);



/**
 * change the level of all players in all tournaments
 * @param chess
 */
void addPlayerLevel(ChessSystem chess);


/**
 * return the highest level in the whole map
 * @param chess
 * @return
 */
double returnTheHighestLevel(ChessSystem chess);

/**
 * returns the id of the first element that we will enter in the file
 * @param chess
 * @return
 */
int returnTheIdOfTheNext(ChessSystem chess);

/**
 * initializes for a player Id specific the player->final value to NoT YET ENDED
 * @param chess
 * @param player_id
 */
void initializeStatsOfThisPlayer(ChessSystem chess,int player_id);

double returnTheBaddestLevel(ChessSystem chess);

int returnIdOfTheLastPlayer (ChessSystem chess);

#endif //STATISTICS_H
