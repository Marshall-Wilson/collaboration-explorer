/* TestSixDegrees.cpp 
* 
* Comp 15 project 2: Six Degrees of Separation 
* 
* by Marshall Wilson 
* 11/23/2020
* 
* Purpose: Tests the member functions of the SixDegrees class.
*
*/

#include "SixDegrees.h"
#include <iostream>

using namespace std;

void testConstructors();
void testNullaryConstructor();
void testTripleConstructor();
void testPopulateGraph();
void testPrintPath();
void testDFS();
void testGetArtists();
void printVector(vector<Artist>);

template<typename streamtype>
void openFile(streamtype &stream, std::string fileName);


int main() {
    
    testConstructors();
    testPopulateGraph();
    testPrintPath();
    testDFS();
    testGetArtists();

    
    
    return 0;
}


/*testConstructors
*   Purpose: tests the nullary and 3 file constructors 
*   Parameters: none 
*   Returns: none 
*/
void testConstructors() {
    
    testNullaryConstructor();
    testTripleConstructor();
}


/*testNullaryConstructor
*   Purpose: Makes sure the nullary constructor doesn't cause any errors 
*   Parameters: none 
*   Returns: none 
*/
void testNullaryConstructor() {
    
    cout << "\nTesting Nullary Constructor: \n";
    
    cout << "Should produce no valgrind memory errors:\n";
    SixDegrees s1;
    
}


/*testTripleConstructor
*   Purpose: Makes sure that the constructor successfully initializes with 
*               a variety of inputs 
*   Parameters: none 
*   Returns: none 
*/
void testTripleConstructor() {
    
    cout << "\nTesting Triple Argument Constructor: \n";
    
    ifstream dataStream;
    openFile(dataStream, "artists.txt");
    ifstream commandStream;
    openFile(commandStream, "testCommands.txt");
    ofstream outStream;
    openFile(outStream, "testOut.txt");
    
    cout << "Should have the path between Adele and Bruno Mars:\n";
    SixDegrees s1(dataStream, commandStream, outStream);
    SixDegrees s2(dataStream, commandStream, cout);
    SixDegrees s3(datastream, cin, cout);
    
}


/*testPopulateGraph
*   Purpose: Makes sure that a graph can successfully make vertices and edges
*               from an input data stream 
*   Parameters: none 
*   Returns: none 
*   Note: testArtist2.txt has duplicates both where the songs are duplicated 
*           and where the songs are unique 
*/
void testPopulateGraph() {
    
    cout << "\nTesting Populate Graph: \n";
    
    ifstream dataStream;
    openFile(dataStream, "testArtists2.txt");
    
    //create an empty SixDegrees 
    SixDegrees s1;
    
    //populate the graph 
    s1.populateGraph(dataStream);
    
    cout << "Should print a graph with Artist 1 connected to 2 and 3\n";
    s1.graph.print_graph(cout);
     
}


/*testPrintPath
*   Purpose: makes sure that print path prints a path in the appropriate format
*   Parameters: none 
*   Returns: none 
*   Note: This function only really tests a basic case of printPath and doesn't
*           even print song names because I was too lazy to input them into the
*           test stack. More thorough testing of print_path happens in the dfs 
*           test function. 
*/
void testPrintPath() {
    
    cout << "\nTesting Print Path: \n";
    
    ifstream dataStream;
    openFile(dataStream, "testArtists.txt");
    
    SixDegrees s1; 
    s1.populateGraph(dataStream);
    
    stack<Artist> pathStack;
    pathStack.push(Artist("Artist 2"));
    pathStack.push(Artist("Artist 1"));
    pathStack.push(Artist("Artist 3"));
    
    cout << "Should print path from Artist 3 to Artist 2 without song names:\n";
    s1.print_path(pathStack, cout);
    
}


/*testDFS
*   Purpose: Makes sure that DFS finds a valid path between two nodes and prints
*               it appropriately, and also handles errors. 
*   Parameters: none 
*   Returns: none 
*/
void testDFS(){
    
    cout << "\nTesting Depth First Search: \n";
    
    ifstream dataStream;
    openFile(dataStream, "testArtists.txt");
    ifstream commandStream;
    openFile(commandStream, "testCommands2.txt");
    
    cout << "Order of outputs: \n 1. Artist 0 doesn't exist\n" 
         << "2. Artist 0 doesn't exist, Artist 7 doesn't exist\n"
         << "3. No path between Artist 1 and Artist 5\n"
         << "4. prints a path from Artist 1 to Artist 3\n";
    SixDegrees s1(dataStream, commandStream);
    
    
}


/*openFile
*   Purpose: opens a file into a provided stream and makes sure it opened 
*               successfully  
*   Parameters: streamtype &stream- the stream for the file to be opened into 
*               std::string fileName- the name of the file to be opened 
*   Returns: none 
*/
template<typename streamtype>
void openFile(streamtype &stream, std::string fileName) {
    
    stream.open(fileName);
    
    if (not stream.is_open()){
        cerr << fileName + " cannot be opened\n";
        exit(EXIT_FAILURE);
    }
    
}


/*testGetArtists
*   Purpose: makes sure that the getArtists function works as intended on a 
*               variety of edge cases 
*   Parameters: none 
*   Returns: none 
*/
void testGetArtists() {
    
    cout << "\nTesting getArtists: \n";
    
    SixDegrees s1;
    
    //test with numbers 
    cout << "Should collect and print 0, 1, and 3 artist names\n";
    vector<Artist> list0 = s1.getArtists(cin, 0);
    printVector(list0);
    vector<Artist> list1 = s1.getArtists(cin, 1);
    printVector(list1);
    vector<Artist> list2 = s1.getArtists(cin, 3);
    printVector(list2);
    
    //test with delimiter
    cout << "Should collect artists until '*' is input\n";
    vector<Artist> list3 = s1.getArtists(cin, "*");
    printVector(list3);
    
    //test from file 
    //no * in file 
    cout << "Should print content of all 4 lines in file\n";
    ifstream commandStream;    
    openFile(commandStream, "testCommands.txt");
    vector<Artist> list4 = s1.getArtists(commandStream, "*");
    printVector(list4);
    
    //number requested is > number of lines in file 
    cout << "Should print 4 words/names even though 6 were requested\n";
    ifstream commandStream2;    
    openFile(commandStream2, "testCommands.txt");
    vector<Artist> list5 = s1.getArtists(commandStream2, 6);
    printVector(list5);
    
}


/*printVector
*   Purpose: prints the names of the aritsts in a list of artists 
*   Parameters: vector<Artist> artistList- the list to print 
*   Returns: none 
*/
void printVector(vector<Artist> artistList) {
    
    cout << "printing: ";
    
    for (size_t i = 0; i < artistList.size(); i++) {
        
        cout << artistList[i].get_name() << ", ";
        
    }
    cout << endl;
    
}
