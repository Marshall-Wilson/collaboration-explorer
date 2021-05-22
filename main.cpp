/* main.cpp 
* 
* Comp 15 project 2: Six Degrees of Separation 
* 
* by Marshall Wilson 
* 11/23/2020
* 
* Purpose: initializes a SixDegrees class instance from command line arguments 
*
*/

#include "SixDegrees.h"
#include <iostream>
#include <fstream>

using namespace std;

template<typename streamtype>
void openFile(streamtype &stream, std::string fileName);

int main(int argc, char *argv[])
{
    //variables to hold file stream(s) from argument(s)
    ifstream dataStream;
    ifstream commandStream;
    ofstream outStream; 
    
    //print usage error if wrong number of file names is provided
    if (argc < 2 or argc > 4) {
        cerr << "Usage: ./SixDegrees dataFile [commandFile] [outputFile]\n";
    
    //Depending on the number of arguments, open files and create a SixDegrees
    //instance with the streams
    } else if (argc == 2) {
        openFile(dataStream, argv[1]);
        SixDegrees sd(dataStream, cin, cout);
        
    } else if (argc == 3) {
        openFile(dataStream, argv[1]);
        openFile(commandStream, argv[2]);
        SixDegrees sd(dataStream, commandStream, cout);
        
    } else if (argc == 4) {
        openFile(dataStream, argv[1]); 
        openFile(commandStream, argv[2]);
        openFile(outStream, argv[3]);
        SixDegrees sd(dataStream, commandStream, outStream);
        
    }
    return 0;
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
        cerr << fileName + " cannot be opened.\n";
        exit(EXIT_FAILURE);
    }
    
}


