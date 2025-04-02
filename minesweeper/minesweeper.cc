#include <iostream>
#include <cassert>
using namespace std;

//returns true if there are any number in the 8 squares around (x,y)
bool checkSurroundings(char **board, int size, int x, int y) {
    for (int dx=-1;dx<=1;dx++) {
        for (int dy=-1;dy<=1;dy++) {
            if (x+dx>=0&&x+dx<size) {
                if (y+dy>=0&&y+dy<size) {
                    if (board[x+dx][y+dy]>='0'&&board[x+dx][y+dy]<='8') {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


//add change to the number of surrounding open spaces counter of any number tile
//in the 8 spaces around (x,y)
void updateSpace(int **space, int size, int x, int y, int change) {
    for (int dx=-1;dx<=1;dx++) {
        for (int dy=-1;dy<=1;dy++) {
            if (x+dx>=0&&x+dx<size) {
                if (y+dy>=0&&y+dy<size) {
                    if (space[x+dx][y+dy]!=-1) {
                        space[x+dx][y+dy]+=change;
                    }
                    //when (dx,dy)=(0,0), space(x,y)==-1
                }
            }
        }
    }
}

//add change to the counter storing the number of addtional mines needed counter of each number tile
//in the 8 spaces around (x,y)
//return false if any number tile needs a negative number of mines after this process
bool updateMinesNeeded(int **minesNeeded, int size, int x, int y, int change) {
    bool result=true;
    for (int dx=-1;dx<=1;dx++) {
        for (int dy=-1;dy<=1;dy++) {
            if (x+dx>=0&&x+dx<size) {
                if (y+dy>=0&&y+dy<size) {
                    if (minesNeeded[x+dx][y+dy]!=42) {
                        minesNeeded[x+dx][y+dy]+=change;
                        if (minesNeeded[x+dx][y+dy]<0) {
                            result = false;
                        }
                    }
                }
            }
        }
    }
    return result;
}

//return false if any of the surrounding number tiles need more mines than there are open spaces around them
bool checkSpaceVSMines(int **space, int **minesNeeded, int size, int x, int y) {
    for (int dx=-1;dx<=1;dx++) {
        for (int dy=-1;dy<=1;dy++) {
            if (x+dx>=0&&x+dx<size) {
                if (y+dy>=0&&y+dy<size) {
                    if (minesNeeded[x+dx][y+dy]!=42) {
                        if (minesNeeded[x+dx][y+dy]>space[x+dx][y+dy]) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

//counts the number of surrounding unknown spaces and mines for a new number tile being inserted
void countSpaceAndMines(char **grid, int **space, int **minesNeeded, int size, int x, int y) {
    for (int dx=-1;dx<=1;dx++) {
        for (int dy=-1;dy<=1;dy++) {
            if (dx==0&&dy==0) continue;
            if (x+dx>=0&&x+dx<size) {
                if (y+dy>=0&&y+dy<size) {
                    if (grid[x+dx][y+dy]=='?') {
                        space[x][y]+=1;
                    }
                    if (grid[x+dx][y+dy]=='f') {
                        minesNeeded[x][y]-=1;
                    }
                }
            }
        }
    }
}

//generates a sizexsize array storing the number of valid boards where there is no mines in each given square
//also returns the total number of valid boards
//note that squares we have no information about are not factored in to this number (each square could be safe or a mine but
//that factor of two is ignored)
//this function assumes all flags placed are valid for efficiency
int generateHeatMap(int **heatMap, char **board, int **space, int **minesNeeded, int size, int x, int y) {
    if (x==size) {
        x=0;
        y+=1;
    }
    if (y==size) {
        return 1;
    }
    if (board[x][y]=='?'&&checkSurroundings(board,size,x,y)) {
        if (heatMap[x][y]==-1) {
            heatMap[x][y]=0;
        }
        int sum=0;
        updateSpace(space,size,x,y,-1);
        if (checkSpaceVSMines(space,minesNeeded,size,x,y)) {
            sum+=generateHeatMap(heatMap,board,space,minesNeeded,size,x+1,y);
            heatMap[x][y]+=sum;
        }
        if(updateMinesNeeded(minesNeeded,size,x,y,-1)) {
            sum+=generateHeatMap(heatMap,board,space,minesNeeded,size,x+1,y);
        }
        updateSpace(space,size,x,y,1);
        updateMinesNeeded(minesNeeded,size,x,y,1);
        return sum;
    }
    else {
        return generateHeatMap(heatMap,board,space,minesNeeded,size,x+1,y);
    }
}

int main() {
    char **board;
    int **heatMap;
    int **space;
    int **minesNeeded;
    int size;
    cin>>size;
    board=new char*[size];
    heatMap=new int*[size];
    space=new int*[size];
    minesNeeded=new int*[size];
    for (int i=0;i<size;i++) {
        board[i]=new char[size];
        heatMap[i]=new int[size];
        space[i]=new int[size];
        minesNeeded[i]=new int[size];
        for (int j=0;j<size;j++) {
            board[i][j]='?';
            space[i][j]=-1;
            minesNeeded[i][j]=42;
        }
    }
    string input;
    while (cin>>input&&input!="quit") {
        if (input == "update"||input == "enter") {
            //enter board data i.e. flag, number
            int x,y;
            cin>>x>>y;
            assert(x>=0&&y>=0);
            assert(x<size&&y<size);
            char value;
            cin>>value;
            assert(value=='f'||(value>='0'&&value<='8'));
            board[x][y]=value;
            updateSpace(space,size,x,y,-1);
            if (value>='0'&&value<='8') {
                minesNeeded[x][y]=value-'0';
                space[x][y]=0;
                countSpaceAndMines(board,space,minesNeeded,size,x,y);
            }
            if (value=='f') {
                updateMinesNeeded(minesNeeded,size,x,y,-1);
            }
        }
        else if (input == "solve") {
            //the program will generate a list of the squares that are either guaranteed to have a mine or guarnteed to be safe
            //note that solve does not modify the board
            for (int i=0;i<size;i++) {
                for (int j=0;j<size;j++) {
                    heatMap[i][j]=-1;
                }
            }
            int options=generateHeatMap(heatMap,board,space,minesNeeded,size,0,0);
            //cout<<"opt: "<<options<<endl;
            for (int i=0;i<size;i++) {
                for (int j=0;j<size;j++) {
                    //cout<<heatMap[i][j]<<endl;
                    if (heatMap[i][j]==0) {
                        cout<<"mine at "<<i<<' '<<j<<endl;
                    }
                    else if (heatMap[i][j]==options) {
                        cout<<"no mine at "<<i<<' '<<j<<endl;
                    }
                }
            }
        }
        else if (input == "print") {
            //print the current board state
            for (int i=0;i<size;i++) {
                for (int j=0;j<size;j++) {
                    cout<<board[i][j];
                    //cout<<'/'<<space[i][j]<<'/'<<minesNeeded[i][j];
                    cout<<' ';
                }
                cout<<endl;
            }
        }
    }
    for (int i=0;i<size;i++) {
        delete [] board[i];
        delete [] heatMap[i];
        delete [] space[i];
        delete [] minesNeeded[i];
    }
    delete [] board;
    delete [] heatMap;
    delete [] space;
    delete [] minesNeeded;

    return 0;
}