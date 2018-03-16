/*
 * CS 106B/X Trailblazer
 * This file contains implementations of the members of the World class.
 * See World.h for declarations and documentation of each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, Keith Schwarz, et al
 * @version 2017/11/16
 * - 17au version; fixed minor compiler warnings
 * @version 2014/11/19
 * - initial version for 14au
 */

#include "World.h"

static World* currentWorld = nullptr;

void setCurrentWorld(World* world) {
    currentWorld = world;
}

/*
 * Returns a 'heuristic' value, or rough estimation, of the distance between
 * this vertex and the given other vertex.
 * The heuristic function is guaranteed to be an 'admissible heuristic',
 * meaning that it is never an overestimation of the distance.
 * (The word 'extern' means this function is defined elsewhere.
 *  You do not need to worry about it.)
 */
double heuristicFunction(Vertex* from, Vertex* to) {
    if (!currentWorld) {
        return 0.0;
    } else {
        return currentWorld->heuristic(from, to);
    }
}
