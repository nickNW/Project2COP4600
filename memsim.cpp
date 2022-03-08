#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int fifo(vector<string> memFrame,vector<string> trace);

  
int main(int argc, char** argv)
{
    vector<string> trace;   
    vector<string> memFrame;
    string line;
    ifstream myfile ("bzip.trace");
    if (myfile.is_open())
    {
        while ( getline(myfile, line) )
        {
            trace.push_back(line);
        }
        myfile.close();
    }
    else cout << "Unable to open file"; 
    
    fifo(memFrame, trace);



    
    return 0;
}
struct pageEntry{
    string memAddress;
    int readWrite;
};

int fifo(vector<string> memFrame,vector<string> trace){
    for (int i =0;i < trace.size(); i++ ){
        struct pageEntry entry;
        entry.memAddress = trace[i].substr(0, 7);
        entry.readWrite = (trace[i][9]=='R'? 1 : 0);// Read == 1 Write == 0

        
        
    }



}