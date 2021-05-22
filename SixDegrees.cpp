/* SixDegrees.cpp
* 
* Comp 15 project 2: Six Degrees of Separation 
* 
* by Marshall Wilson 
* 11/23/2020
* 
* Purpose: The implementation of the DatumStack class. Allows the user to 
*           populate a graph of artists and songs from a file. The user can 
*           interact with the graph by inputting commands through cin or from a
*           file and can decide whether the results output goes to the terminal
*           or an output file. Finally, the class contains/will contain a 
*           number of algorithms for finding paths between artists using the
*           collaborations between artists.
*
*/

#include "SixDegrees.h"
#include "CollabGraph.h"
#include <iostream>

using namespace std;

/*Default/Nullary Constructor 
*   Purpose: creates an empty SixDegrees object  
*/
SixDegrees::SixDegrees() {
    
}


/*Data + Commands + Output Constructor 
*   Purpose: creates a SixDegrees object, populates the graph from a provide
*               list of artists and songs, and initializes streams that read
*               commands and print outputs
*/
SixDegrees::SixDegrees(ifstream &data, istream &commands, ostream &output) {
    
    populateGraph(data);
    
    readCommands(commands, output);

}


/*populateGraph
*   Purpose: uses the data stream to fill the collab graph with artists and 
*               song collaborations 
*   Parameters: ifstream &data- the stream containing the artist data 
*   Returns: none 
*/
void SixDegrees::populateGraph(ifstream &data) {
    
    //get the data from the dataStream into a vector 
    vector<Artist>artistList = getData(data);
    
    //once all the veritces are loaded in, create the edges
    createVertices(artistList);
    createEdges(artistList);
}


/*getData 
*   Purpose: reads and parses the data from the dataStream into a vector of 
*               artists and their discography
*   Parameters: none 
*   Returns: vector<Artist>- The list of artists contained in the data file 
*/
vector<Artist> SixDegrees::getData(ifstream &data) {
    
    //create temporary artist holders
    Artist tempArtist;
    vector<Artist> artistList;
    
    //start looping through lines in the data file 
    string input;
    getline(data, input);
    while (not data.fail()) {
        
        // "*" causes Artist to push and reset 
        if (input == "*") {
            artistList.push_back(tempArtist);
            tempArtist = Artist();
            
        // if the artist's name is empty, this must be the name! 
        } else if (tempArtist.get_name() == "") {
            tempArtist.set_name(input);
            
        // everything else is songs 
        } else {
            tempArtist.add_song(input);
            
        }
        getline(data, input);
        
    }
    
    return artistList;
    
}


/* createVertices
*   Purpose: populates the graph with a vertex for each non-duplicate artist in
*               a provided list 
*   Parameters: vector<Artist> artistList- the artists to be stored in vertices
*   Returns: none 
*/
void SixDegrees::createVertices(vector<Artist> artistList) {
    
    for (Artist artist : artistList) {
        
        graph.insert_vertex(artist);
        
    }
    
}


/*createEdges
*   Purpose: Generates the edges from a graph with only vertices 
*   Parameters: vector<Artist> artistList- The list of artists that was used to
*               create the vertices  
*   Returns: none 
*/
void SixDegrees::createEdges(vector<Artist> artistList) {
    
    //loop through all pairs of vertices looking for/creating edges 
    for (Artist artist1 : artistList) {
        
        for (Artist artist2 : artistList) {
            
            string collaboration = artist1.get_collaboration(artist2);
            if (collaboration != "" and artist1 != artist2) {
                graph.insert_edge(artist1, artist2, collaboration);
            
            }
            
        }
        
    }
    
}


/*readCommands
*   Purpose: reads in commands and parses them
*   Parameters: istream &commands- the input stream to read commands from 
*               ostream &output- the stream to send outputs to
*   Returns: none 
*/
void SixDegrees::readCommands(istream &commands, ostream &output) {
    
    string input = "";
    getline(commands, input);
    
    while (not commands.fail()) {
        
        if (input == "dfs") {
            dfs_container(commands, output);
            
        } else if (input == "bfs") {
            bfs_container(commands, output);
            
        } else if (input == "not") {
            bfs_not(commands, output);
            
        } else if (input == "incl") {
            incl_container(commands, output);
            
        } else if (input == "quit") {
            return; 
            
        } else {
            output << input << " is not a command. Please try again.\n";
            
        }
        
        getline(commands, input);
        
    }

}


/*dfs_container
*   Purpose: reads in the artist names for a depth first search, runs the 
*               recursive search function, and prints the result 
*   Parameters: istream &commands- the stream to read the artist names from 
*               ostream &output- the stream to print the result to
*   Returns: None 
*   Note: assumes that all vertices have visited = false and 
*           predecessor = nullptr 
*/
void SixDegrees::dfs_container(istream &commands, ostream &output) {
    
    //read in artist names, make artists, and make sure they're in the graph
    vector<Artist> artistList = getArtists(commands, 2);
    bool artistsKnown = in_dataset(artistList, output);
    
    if (artistsKnown) {
    
        //print the result if a path is found, otherwise print "path not found"
        if (dfs_recursive(artistList[0], artistList[1])) {
            print_path(graph.report_path(artistList[0], artistList[1]), output);
            
        } else {
            output << "A path does not exist between \"" 
                    << artistList[0].get_name() << "\" and \"" 
                    << artistList[1].get_name() << "\".\n";
        }
    }
        
    graph.clear_metadata();
    
}


/*dfs_recursive
*   Purpose: recursive component of the depth first search function.  
*   Parameters: Artist &start- the artist the current iteration of the function
*                               is starting from 
*               Artist &end- the destination artist of the overall search 
*   Returns: bool- true if a path to the end was found.
*/
bool SixDegrees::dfs_recursive(Artist &start, Artist &end) {
    
    graph.mark_vertex(start);
    
    //recurse through vertex neighbors 
    for (Artist neighbor: graph.get_vertex_neighbors(start)) {
        
        //unvisited nodes get an updated predecessor and get recursed over 
        if (not graph.is_marked(neighbor)) {
            
            graph.set_predecessor(neighbor, start);
            
            if (neighbor == end) {
                return true; 
                        
            //passes a found path boolean back up the chain 
        } else if (dfs_recursive(neighbor, end)) {
                return true; 
            }
        }
    }
    
    //function only reaches here if path wasn't found 
    return false; 
    
}


/*in_dataset
*   Purpose: checks if each artist in a list is in a graph and prints an error 
*               for any missing artists 
*   Parameters: vector<Artist> artistList- the artists to check for in the graph
*               ostream &output- the output stream to print errors to 
*   Returns: bool- whether all the provided artists are in the graph 
*/
bool SixDegrees::in_dataset(vector<Artist> artistList, ostream &output) {
    
    bool allInDataset = true;
    
    for (Artist artist: artistList) {
        
        if (not graph.is_vertex(artist)) {

            output << "\"" << artist.get_name() 
                        << "\" was not found in the dataset :(\n";
            allInDataset = false; 
        }
    }
    return allInDataset;
}


/*print_path
*   Purpose: prints the collaborations on the path between two artists 
*   Parameters: stack<Artist> artistStack- A stack containing every artist on 
*                   the path between the first and last artist in order 
*               ostream &output- the output stream to print to 
*   Returns: none
*/
void SixDegrees::print_path(stack<Artist> artistStack, ostream &output) {
    
    Artist currArtist;
    
    //keeps looping until reaching the last pair 
    while (artistStack.size() > 1) {
        
        currArtist = artistStack.top();
        artistStack.pop();
        output << "\"" << currArtist << "\" collaborated with \"" 
                << artistStack.top() << "\" in \"" 
                << graph.get_edge(currArtist, artistStack.top()) << "\".\n";
        
    }
    output << "***\n";
}


/*bfs_container
*   Purpose: reads in the parameters for a bredth first search, performs the 
*               search, prints the results, and cleans up the metadata 
*   Parameters: istream &commands- the stream to read the artist names from 
*               ostream &output- the stream to print the result to
*   Returns: None 
*   Note: assumes that all vertices have visited = false and 
*           predecessor = nullptr. Returns vertices to this state when finished
*/
void SixDegrees::bfs_container(istream &commands, ostream &output) {
    
    //read in artist names, make artists, and make sure they're in the graph
    vector<Artist> artistList = getArtists(commands, 2);
    bool artistsKnown = in_dataset(artistList, output);
    
    if (artistsKnown) {
    
        //print the result if a path is found, otherwise print "path not found"
        if (bfs(artistList[0], artistList[1])) {
            print_path(graph.report_path(artistList[0], artistList[1]), output);
            
        } else {
            output << "A path does not exist between \"" 
                    << artistList[0].get_name() << "\" and \"" 
                    << artistList[1].get_name() << "\".\n";
        }
    }
        
    graph.clear_metadata();
}


/*bfs
*   Purpose: performs a bredth first search to find the shortest path between
*               two artists in the graph  
*   Parameters: Artist &start- the artist to start the search from 
*               Artist &end- the artist to find a path to
*   Returns: bool- whether the search was successful 
*/
bool SixDegrees::bfs(Artist &start, Artist &end) {
    
    queue<Artist> artistQueue;
    artistQueue.push(start);
    Artist currentArtist;
    
    while (not artistQueue.empty()) {
        
        currentArtist = artistQueue.front();
        artistQueue.pop();
        graph.mark_vertex(currentArtist);
        
        //for any unvisited artists, add their neighbors to the queue 
        for (Artist neighbor: graph.get_vertex_neighbors(currentArtist)) {
            
            if (not graph.is_marked(neighbor)) {
                graph.set_predecessor(neighbor, currentArtist);
                
                if (neighbor == end) {
                    return true;
                }
                artistQueue.push(neighbor);

            }
        }    
    }
    
    return false;
}

/*getArtists (with number)
*   Purpose: reads a given number of artist names from the input stream, 
*               creates artist instances for each name, and returns them in a 
*               list 
*   Parameters: istrem &input- the steram to read artist names from 
*               int numArtists- the number of artists to read in 
*   Returns: vector<Artist>- a list of artists in the order they were read in 
*   Note: if reading from a file, if more artists are requested than there are
*           lines remaining in the file, only the remaining lines will be 
*           added to the list. 
*/
vector<Artist> SixDegrees::getArtists(istream &input, int numArtists){
    
    string artistName;
    vector<Artist> artistList;
    
    //read in the required number of artists 
    for (int i = 1; i <= numArtists; i++) {
        
        getline(input, artistName);
        
        //Don't add artists if the read fails 
        if (not input.fail()) {
            artistList.push_back(Artist(artistName));
        }
    }
    
    return artistList;
    
}


/*getArtists (with delimiter )
*   Purpose: reads artist names from the input stream until a given delimiter
*               is found, creates artist instances for each name, and returns 
*               them in a list 
*   Parameters: istrem &input- the steram to read artist names from 
*               string delimiter- the character that marks the end of the 
*                                   artists
*   Returns: vector<Artist>- a list of artists in the order they were read in 
*   Note: If reading from a file that doesn't include the delimiter as its own
*           line, will read in all lines until eof
*/
vector<Artist> SixDegrees::getArtists(istream &input, string delimiter) {

    string artistName;
    vector<Artist> artistList;
    getline(input, artistName);

    //add artists until the delimiter is found or eof is reached 
    while(artistName != delimiter and not input.fail()) {
        
        artistList.push_back(Artist(artistName));
        getline(input, artistName);
        
    }

    return artistList;

}

/*bfs_not
*   Purpose: Reads a series of artists and runs a BFS to find the shortest path
*               between the first two artists that doesn't include any of the 
*               other artists and reports the result. 
*   Parameters: istream &commands- the stream to read the artist names from 
*               ostream &output- the stream to print the result to
*   Returns: None 
*   Note: assumes that all vertices have visited = false and 
*           predecessor = nullptr. Returns vertices to this state when finished
*/
void SixDegrees::bfs_not(istream &commands, ostream &output) {
    
    //read in artists until * or eof 
    vector<Artist> artistList = getArtists(commands, "*");
    
    //check that artists are in the graph 
    bool artistsKnown = in_dataset(artistList, output);
    
    //if every artist is in the graph, mark the "nots" and run bfs 
    if (artistsKnown) {
        
        //mark each "not" artist as already visited 
        for (size_t i = 2; i < artistList.size(); i++) {
            
            graph.mark_vertex(artistList[i]);
        } 
        
        //print the result if a path is found, otherwise print "path not found"
        if (bfs(artistList[0], artistList[1])) {
            print_path(graph.report_path(artistList[0], artistList[1]), output);
            
        } else {
            output << "A path does not exist between \"" 
                    << artistList[0].get_name() << "\" and \"" 
                    << artistList[1].get_name() << "\".\n";
        }
    }
    
    graph.clear_metadata();    
}


/* incl_container 
*   Purpose: reads in three artists and finds a path that goes from the first 
*               artist to the second artist that includes the third artist. 
*   Parameters: istream &commands- the stream to read the artist names from 
*               ostream &output- the stream to print the result to
*   Returns: None 
*   Notes: assumes that all vertices have visited = false and 
*           predecessor = nullptr. Returns vertices to this state when finished
*   Note on Algorithm: 
* If a shortest path from artist A to artist B that includes artist C without 
* any repetition exists, then it must include the shortest path from 
* A to C or the shortest path from B to C. I don't know why, but it seems to
* work.   
*/
void SixDegrees::incl_container(istream &commands, ostream &output) {
    
    //reads in artists and checks that they are in the graph  
    vector<Artist> artistList = getArtists(commands, 3);
    if (not in_dataset(artistList, output)) {
        return;
    }
    
    if (incl(artistList[0], artistList[1], artistList[2])) {
        
        print_path(graph.report_path(artistList[0], artistList[1]), output);
        return;
        
    } 
    graph.clear_metadata();

    if (incl(artistList[1], artistList[0], artistList[2])) {
        
        print_path(graph.report_path(artistList[1], artistList[0]), output);
        return;    
        
    }
    graph.clear_metadata();
    
    //only reaches error message if neither previous step works     
    output << "A path does not exist between \"" 
            << artistList[0].get_name() << "\" and \"" 
            << artistList[1].get_name() << "\".\n";
    
}


/* incl 
*   Purpose: Attemts to find a path from A to B that includes the shortest 
*               path from A to C
*   Parameters: Artist artistA- the starting artist 
*               Artist artistB- the ending artist 
*               Artist artistC- the middle artist (must be included) 
*   Returns: bool- whether the search was sucessful 
*   Note: fails to find a path even when one exists in graphs like the following
*          because the first BFS will find the shortest path from A-C which 
*           cuts off any path from B-C 
*
*   B - X ------- A
*       |        /
*       |       Y
*       |      /
*       C --- Z 
*/
bool SixDegrees::incl(Artist artistA, Artist artistB, Artist artistC) {
    
    vector<Artist> artistList;
    artistList.push_back(artistA);
    artistList.push_back(artistB);
    artistList.push_back(artistC);

    if (bfs(artistList[0], artistList[2])) {

        stack<Artist> pathStack = 
                            graph.report_path(artistList[0], artistList[2]);
        
        graph.clear_metadata();
        
        //mark the path from A-(C-1) as visited and save C's predecessor 
        Artist predC = markPathToEnd(pathStack, artistList[2]);
        
        //find a path from from C-B (will avoid the marked path from A-C)
        if (bfs(artistList[2], artistList[1])) {
            //remember to update the predecessor of C 
            graph.set_predecessor(artistList[2], predC);
            return true; 
            
        } else {
            return false;
        }    
    }
    
    return false; 
    
}




/*markPathToEnd 
*   Purpose: marks the artists in a stack as visited and sets their 
*               predecessors. Stops at the node before a designated node. 
*   Parameters: stack<Artist> pathStack- the stack of artists to be marked 
*               Artist end- the artist that the path should end at 
*   Returns: Artist- the artist right before the end artist/predecessor of the
*                       end artist in the path 
*   Notes: Requires that "end" is in the stack 
*          Doesn't mark the end artist so that it can be used in searches
*/
Artist SixDegrees::markPathToEnd(stack<Artist> pathStack, Artist end) {
    
    Artist currArtist = pathStack.top();
    graph.mark_vertex(currArtist);
    pathStack.pop();
    Artist prevArtist = currArtist;
    
    //go through all but the last artist marking and setting predecessors 
    while (pathStack.top() != end){
        currArtist = pathStack.top();
        graph.mark_vertex(currArtist);
        graph.set_predecessor(currArtist, prevArtist);
        prevArtist = currArtist; 
        pathStack.pop();
    }

    //prevArtist at this point is the predecessor of the end artist 
    return prevArtist;
}
