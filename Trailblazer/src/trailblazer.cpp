// Homework 7 :Trailblazer
//Xiangyu Peng (xypeng5) and Mingcheng yi (mingchyi)
//This assignment focuses on graphs, specifically on searching for paths in a graph


#include "trailblazer.h"
#include "pqueue.h"
#include "queue.h"
#include "vector.h"
#include "set.h"
#include "map.h"
#include "basicgraph.h"


using namespace std;
bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& chosen, Vector<Vertex*>& path, Set<Vertex*>& marked);
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end);
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end);
bool findWeightedPath(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& successPath, string type);

/* This function uses depth first search to find a path from the vertex start to the vertex end.
 * The function accepts a basicGraph, a vertex to start and a vertex of destination.
 * The function returns the path consisting of vertices.
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    start -> setColor(GREEN);
    if (start == end){                                                                     //Exit early if start = end
        Vector<Vertex*> oneVertex;
        oneVertex.add(start);
        return oneVertex;
    }else{
        Vector<Vertex*> path;
        path.add(start);
        Set<Vertex*> marked;
        marked.add(start);
        if (depthFirstSearchHelper(graph, start, end, path, path, marked)){                //Call Helper Function
            return path;
        }else{
            path.clear();
            start -> setColor(GRAY);
            return path;
        }
    }
}

/* This is the helper function for DepthFirstSearch.
 * The function returns true if there is a path from the Vertex start to the Vertex end.
 * The function also records the path.
 */
bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& chosen, Vector<Vertex*>& path, Set<Vertex*>& marked){
    if (start == end){                                                      //Base Case: Path found
        path = chosen;
        return true;
    }else{
        for(Vertex* eachNeighbor : graph.getNeighbors(start)){              //Recursive Backtracking
            if (!marked.contains(eachNeighbor)){
                chosen.add(eachNeighbor);                                   //choose
                eachNeighbor -> setColor(GREEN);
                marked.add(eachNeighbor);
                if (depthFirstSearchHelper(graph, eachNeighbor, end, chosen, path, marked)){
                    return true;                                            //explore
                }else{
                    chosen.remove(chosen.size() - 1);
                    eachNeighbor -> setColor(GRAY);                         //unchoose
                    marked.remove(eachNeighbor);
                }
            }
        }
        return false;
    }
}

/* This function uses breadth first search to find a path from the vertex start to the vertex end.
 * The function accepts a basicGraph, a vertex to start and a vertex of destination.
 * The function returns the path consisting of vertices.
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    start -> setColor(YELLOW);
    Vector<Vertex*> oneVertex;
    oneVertex.add(start);
    if (start == end){
        start -> setColor(GREEN);                                  //Exit early if start = end
        return oneVertex;
    }else{
        Vector<Vertex*> path;
        path.add(start);
        Set<Vertex*> marked;
        marked.add(start);
        Queue <Vector<Vertex*>> toDoList;                           //create a queue of vertices to visit
        toDoList.enqueue(oneVertex);
        while (!toDoList.isEmpty()){
            Vector<Vertex*> currPath = toDoList.dequeue();          //Visit vertices in the queue
            Vertex* currVertex = currPath[currPath.size() - 1];
            currVertex -> setColor(GREEN);
            for (Vertex* eachNeighbor : graph.getNeighbors(currVertex)){
                if (!marked.contains(eachNeighbor)){
                    marked.add(eachNeighbor);
                    if (eachNeighbor == end){                       //end found
                        currPath.add(eachNeighbor);
                        eachNeighbor -> setColor(GREEN);
                        return currPath;
                    }else{
                        currPath.add(eachNeighbor);                 //continue searching
                        eachNeighbor -> setColor(YELLOW);
                        toDoList.enqueue(currPath);
                        currPath.remove(currPath.size() - 1);
                    }
                }

            }
        }
        path.clear();           //return empty vector if not found
        return path;
    }
}

/* This function uses Dijkstras' algorithm to find a path from the vertex start to the vertex end.
 * The function accepts a basicGraph, a vertex to start and a vertex of destination.
 * The function returns the path consisting of vertices.
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> oneVertex;
    oneVertex.add(start);
    if (start == end){
        start-> setColor(GREEN);
        return oneVertex;
    }else{
        Vector<Vertex*> successPath;
        string type = "dijkstrasAlgorithm";
        findWeightedPath(graph, start, end, successPath, type); //Call the helper to find the path
        if (!successPath.isEmpty()){
            return successPath;
        } else{
            oneVertex.clear();  //return empty vector if path not found
            return oneVertex;
        }
    }
}

/* This function uses A star algorithm to find a path from the vertex start to the vertex end.
 * The function accepts a basicGraph, a vertex to start and a vertex of destination.
 * In this method, there is a heuristics function which guesses the distance of the
 * current node and the destination. Thus, the priority should be distance(s,u) + h(u,t)
 * This method will always find the shortest path and return it.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> oneVertex;
    oneVertex.add(start);
    if (start == end){
        start-> setColor(GREEN);
        return oneVertex;
    }else{
        Vector<Vertex*> successPath;
        string type = "A*";
        findWeightedPath(graph, start, end, successPath, type); //Call the helper to find the path
        if (!successPath.isEmpty()){
            return successPath;
        } else{
            oneVertex.clear();                                  //return empty vector if path not found
            return oneVertex;
        }
    }
}

/* This function is helper function for both two method
 * We use type to distinguish the two method
 */
bool findWeightedPath(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& successPath, string type){
    PriorityQueue<Vertex*> toDoList;                    //This is the main pqueue we are using to generate the optimized path
    Map <Vertex*, double> marked;                       //This map is to mark the lowest cost upon certain point
    Map <Vertex*, Vertex*> bestConnection;              //This map is to track the best routh
    int startPriority;
    if (type == "dijkstrasAlgorithm"){                  //Decide different starting point priority
        startPriority = 0;
    }else{
        startPriority = heuristicFunction(start,end);
    }
    toDoList.enqueue(start, startPriority);
    start -> setColor(YELLOW);
    for (Vertex* eachVet: graph.getVertexSet()){
        marked[eachVet] = POSITIVE_INFINITY;            //Mark all other locations costs as infinity
    }
    marked.put(start, 0);                               //Mark start point cost as 0
    while(!toDoList.isEmpty()){
        Vertex* currVertex = toDoList.dequeue();        //Dequeue a vertex
        if (currVertex == end){                         //End loop if the first element in the pqueue is the end point
            currVertex -> setColor(GREEN);
            for (Vertex* v = end; v != start; v=bestConnection[v]){
                successPath.insert(0,v);                //recover the path
            }
            successPath.insert(0,start);
            return true;
        }
        currVertex -> setColor(GREEN);
        for (Vertex* each : graph.getNeighbors(currVertex)){                                  //Explore each neighbor
            double newCost = graph.getEdge(currVertex, each)->cost + marked[currVertex];      //Calculate new cost
            if (newCost <= marked[each]){                                                     // Ignore if new cost is higher
                marked[each] = newCost;
                double newPriority;
                if (type == "dijkstrasAlgorithm"){                                            //Decide new priority
                    newPriority = newCost;
                }else{
                    newPriority = marked[each] + heuristicFunction(each, end);
                }
                try{                                                                //if the vertex is already in, change priority
                    toDoList.changePriority(each,newPriority);
                }
                catch(exception){
                    each -> setColor(YELLOW);
                    toDoList.enqueue(each, newPriority);                            //if not, enqueue it
                }
                bestConnection[each] = currVertex;                                  //update the best connection map
            }
        }

    }
    return false;
}

/* This function uses Kruskal's algorithm to find a minimal spinning tree (MST) for random-maze-generation.
 * The function accepts a basicGraph.
 * The function returns the set of edges of the MST.
 * We assume that the graph is undirected.
 * The idea is to search from the edge with smaller cost and accept an edge if and only if
 * the start and finish vertices of the edge are not connected with previous selected edges.
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    graph.resetData();
    Set<Edge*> mst;
    Map<Vertex*, int> vertexCluterNum;                  //This map is to record the cluster number
    Vector<Set<Vertex*>> clusterSets;                   //This is to record the vertices in each cluster
    for(Vertex* v : graph.getVertexSet()) {
        Set<Vertex*> cluster;
        cluster.add(v);
        clusterSets.add(cluster);
        vertexCluterNum.put(v, clusterSets.size() - 1);  //cluster the Vertex
    }
    PriorityQueue<Edge*> edgeQueue;                      // make the order of edges
    for(Edge* edge : graph.getEdgeSet()) {
        edgeQueue.enqueue(edge, edge->cost);
    }

    while(!edgeQueue.isEmpty()){
        Edge* curredge = edgeQueue.dequeue();
        Vertex* start = curredge -> start;
        Vertex* finish = curredge -> finish;
        if (vertexCluterNum[start] != vertexCluterNum[finish]){
            mst.add(curredge);                                                                  //Choose the edge
            clusterSets[vertexCluterNum[start]] += clusterSets[vertexCluterNum[finish]];        //connect both ends together
            clusterSets[vertexCluterNum[finish]] += clusterSets[vertexCluterNum[start]];
            for (Vertex* each : clusterSets[vertexCluterNum[finish]]){
                vertexCluterNum[each] = vertexCluterNum[start];                                 //update cluster numbers
            }
        }
    }
    return mst;
}
