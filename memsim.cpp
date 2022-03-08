#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

struct pageEntry{
    unsigned memAddress;
    char readWrite;

    bool operator==(const pageEntry& other) const
    {
    return (memAddress == other.memAddress);
    }
};


int fifo(vector<pageEntry> memFrame,vector<pageEntry> trace);

int main(int argc, char** argv)
{
    vector<pageEntry> trace;   
    vector<pageEntry> memFrame;
    FILE * fp;
    fp = fopen("bizip.trace", "r");
    if (fp == NULL) {
        cout << "error fp is NULL" << endl;
        return 1;
    }

    unsigned int addr;
    unsigned int pageNum; 
    char readWrite; 
    while (fscanf(fp, "%x %c", &addr, &readWrite) != EOF) {
        struct pageEntry entry;
        pageNum = addr >> 12; 
        entry.memAddress = pageNum; 
        entry.readWrite = readWrite;
        trace.push_back(entry);
    }
    
    fifo(memFrame, trace);



    
    return 0;
}


int fifo(vector<pageEntry> memFrame,vector<pageEntry> trace){
    for (int i =0;i < trace.size(); i++ ){
        if (memFrame.size() < 4096){
            if (trace[i].readWrite == 'R')
                memFrame.push_back(trace[i]);
        }
        else {
            pageEntry temp = trace[i];
            if (find(memFrame.begin(), memFrame.end(), temp) != memFrame.end()){
                return 0;
            }
        }
        
        
    }

//test


}
