#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

struct pageEntry{
    unsigned memAddress;
    char readWrite;
    bool isDirty;

    bool operator==(const pageEntry& other) const
    {
    return (memAddress == other.memAddress);
    }
};
int readCount = 0 ;
int writeCount = 0 ;
int hitCount =0;

int fifo(vector<pageEntry> memFrame, vector<pageEntry> trace, int frameNum);

int main(int argc, char** argv)
{
    vector<pageEntry> trace;   
    vector<pageEntry> memFrame;
    FILE * fp;
    fp = fopen("bzip.trace", "r");
    if (fp == NULL) {
        cout << "error fp is NULL" << endl;
        return 1;
    }

    unsigned int addr;
    unsigned int pageNum; 
    char readWrite; 
    int frameNum = 64;
    while (fscanf(fp, "%x %c", &addr, &readWrite) != EOF) {
        struct pageEntry entry;
        pageNum = addr >> 12; 
        entry.memAddress = pageNum; 
        entry.readWrite = readWrite;
        trace.push_back(entry);
    }
    
    fifo(memFrame, trace, frameNum);

    cout<< "read " << readCount <<"\n";
    cout<< "write " << writeCount <<"\n";
    


    return 0;
}


int fifo(vector<pageEntry> memFrame,vector<pageEntry> trace, int frameNum){
    for (int i =0;i < trace.size(); i++ ){

        pageEntry temp = trace[i];
        if (find(memFrame.begin(), memFrame.end(), temp) != memFrame.end()){ //found in frame
                int tempIndex;
                if (temp.readWrite == 'W'){
                    for(tempIndex = 0; tempIndex < memFrame.size(); tempIndex++){
                        if (memFrame[tempIndex].memAddress == trace[i].memAddress)
                            break;

                    }
                    memFrame[tempIndex].readWrite = 'W';
                    
                }
        }
        else{
            if (memFrame.size() < frameNum){ //frame is not full 

            readCount++;
            memFrame.push_back(trace[i]);
            }
            else {
                //pop if the poped value has W incriment write 
                if (memFrame[0].readWrite == 'W' )
                    writeCount++;
                
                memFrame.erase(memFrame.begin());
                memFrame.push_back(trace[i]);
                readCount++;
            }
        }
        
        
    }

//test


}
