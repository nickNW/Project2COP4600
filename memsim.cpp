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
int LRU(vector<pageEntry> memFrame, vector<pageEntry> trace, int frameNum);
void SFIFO(vector<pageEntry> trace, int percentage, int frameNum);

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
    
    LRU(memFrame, trace, frameNum);

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
}

int LRU(vector<pageEntry> memFrame,vector<pageEntry> trace, int frameNum){
    for (int i =0;i < trace.size(); i++ ){
        pageEntry temp = trace[i];
        if (find(memFrame.begin(), memFrame.end(), temp) != memFrame.end()){ //found in frame
                int tempIndex;
                if (temp.readWrite == 'W'){
                    for(tempIndex = 0; tempIndex < memFrame.size()-1; tempIndex++){
                        if (memFrame[tempIndex].memAddress == trace[i].memAddress)
                            break;

                    }
                    memFrame[tempIndex].readWrite = 'W';

                    pageEntry tempEntry = memFrame[tempIndex];
                    memFrame.erase(memFrame.begin()+tempIndex);
                    memFrame.push_back(tempEntry);
                    
                    
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
}

void SFIFO(vector<pageEntry> trace, int percentage, int frameNum){
    int primaryBufferCount = frameNum * (percentage/100);
    int secondaryBufferCount = frameNum - primaryBufferCount;
    vector<pageEntry> primaryBuffer;
    vector<pageEntry> secondaryBuffer;

    for (int i =0;i < trace.size(); i++ ){
        int foundIn = 0; //1 is primary 2 is secondary 
        pageEntry temp = trace[i];
        if (find(primaryBuffer.begin(), primaryBuffer.end(), temp) != primaryBuffer.end()){ //found in frame
                int tempIndex;
                if (temp.readWrite == 'W'){
                    for(tempIndex = 0; tempIndex < primaryBuffer.size(); tempIndex++){
                        if (primaryBuffer[tempIndex].memAddress == trace[i].memAddress)
                            break;

                    }
                    primaryBuffer[tempIndex].readWrite = 'W';
                }
                foundIn = 1;
        }
        if (find(secondaryBuffer.begin(), secondaryBuffer.end(), temp) != secondaryBuffer.end()){ //found in frame
                int tempIndex;
                if (temp.readWrite == 'W'){
                    for(tempIndex = 0; tempIndex < secondaryBuffer.size(); tempIndex++){
                        if (secondaryBuffer[tempIndex].memAddress == trace[i].memAddress)
                            break;

                    }
                    secondaryBuffer[tempIndex].readWrite = 'W';

                    pageEntry tempEntry = secondaryBuffer[tempIndex];
                    secondaryBuffer.erase(secondaryBuffer.begin()+tempIndex);
                    secondaryBuffer.push_back(tempEntry);
                }
                foundIn = 2;
        }
        else if (foundIn == 0) { //not found in primary frame or secondary 
            if (primaryBuffer.size() < primaryBufferCount){ //primary frame is not full 
                readCount++;
                primaryBuffer.push_back(trace[i]);
            }
            else if (primaryBuffer.size() >= primaryBufferCount && secondaryBuffer.size() < secondaryBufferCount ) {
                // primary frame is full and secondary frame is not full
                //move from primary to secondary and insert new value into primary 
                secondaryBuffer.push_back(primaryBuffer[0]);
                primaryBuffer.erase(primaryBuffer.begin());
                primaryBuffer.push_back(trace[i]);
                readCount++; 
            }
            else if (primaryBuffer.size() >= primaryBufferCount && secondaryBuffer.size() >= secondaryBufferCount ) {
                //primary frame is full and secondary frame is full
                ////move from primary to secondary and insert new value into primary remove from secondary 
                secondaryBuffer.erase(secondaryBuffer.begin());
                secondaryBuffer.push_back(primaryBuffer[0]);
                primaryBuffer.erase(primaryBuffer.begin());
                primaryBuffer.push_back(trace[i]);
                readCount++; 
            }
        }

        else if(foundIn == 1){ //found in the primary frame
             int tempIndex;
                if (temp.readWrite == 'W'){
                    for(tempIndex = 0; tempIndex < primaryBuffer.size(); tempIndex++){
                        if (primaryBuffer[tempIndex].memAddress == trace[i].memAddress)
                            break;

                    }
                    primaryBuffer[tempIndex].readWrite = 'W';
                }
        }

        else if (foundIn == 2){//found in the secondary frame

        } 
        

    }
}
