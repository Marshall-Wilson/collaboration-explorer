/* SixDegrees.h 
* 
* Comp 15 project 2: Six Degrees of Separation 
* 
* by Marshall Wilson 
* 11/23/2020
* 
* Purpose: The interface of the SixDegrees class. Contains all the public and 
*           private members of the class. 
*
*/

#ifndef _SIXDEGREES_
#define _SIXDEGREES_

#include "CollabGraph.h"
#include <iostream>
#include <cstdlib>
#include <fstream> 
#include <queue>
#include <stack>
#include <vector>

class SixDegrees {
    
public:
    SixDegrees();
    SixDegrees(std::ifstream &, std::istream &, std::ostream &);
    
    void populateGraph(std::ifstream &data);
    void readCommands(std::istream &commands, std::ostream &output);
    
private:
    
    std::vector<Artist> getData(std::ifstream &data);
    void createVertices(std::vector<Artist> artistList);
    void createEdges(std::vector<Artist> artistList);
    
    void dfs_container(std::istream &commands, std::ostream &output);
    bool dfs_recursive(Artist &start, Artist &end);
    
    void bfs_container(std::istream &commands, std::ostream &output);
    bool bfs(Artist &start, Artist &end);
    void bfs_not(std::istream &commands, std::ostream &output);
    void incl_container(std::istream &commands, std::ostream &output);
    bool incl(Artist artistA, Artist artistB, Artist artistC);
    
    bool in_dataset(std::vector<Artist> artistList, std::ostream &output);
    std::vector<Artist> getArtists(std::istream &input, int numArtists);
    std::vector<Artist> getArtists(std::istream &input, std::string delimiter);
    
    Artist markPathToEnd(std::stack<Artist> pathStack, Artist end);    
    void print_path(std::stack<Artist>, std::ostream &output);
                    
    CollabGraph graph;

};

#endif