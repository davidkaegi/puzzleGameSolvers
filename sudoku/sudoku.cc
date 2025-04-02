#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

//if I want to optimize, then I would want ways to reduce number of candidates reasonably fast
//a human approach to this would be to: think 48 48, then we know 4 and 8 from this section must be in those 2 squares

//also if there exist only one place to get a given number

int x=0;
//lazy global variable to count the number of times to program made wrong guesses

struct Square {
    public:
    Square() {
        i=-1;
        j=-1;
    }
    int i;
    int j;
};

void updateCandidates(int i, int j, int num, int candidates[9][9][9], bool remove) {
    if (!num) {
        return;
    }
    int change=1;
    if (remove){
        change=-1;
    }
    for (int c=0;c<9;c++) {
        candidates[i][c][num-1]+=change;
        candidates[c][j][num-1]+=change;
        //note that this double updates cand[i][j]; this is fine since it will also be double undone
    }
    int a=i/3;
    int b=j/3;
    for (int ii=0;ii<3;ii++) {
        for (int jj=0;jj<3;jj++) {
            candidates[ii+3*a][jj+3*b][num-1]+=change;
        }
    }
    //add the grid stuff
}
bool isValid(int grid[9][9]) {
    //validation checking, not used in algorithm, I have no idea why I am
    //writing this; it was useful for testing in the end
    bool nums[9];
    for (int i=0;i<9;i++) {
        for (int c=0;c<9;c++) {
            nums[c]=false;
        }
        for (int j=0;j<9;j++) {
            if (grid[i][j]) {
                if (nums[grid[i][j]-1]) {
                    return false;
                }
                else {
                    nums[grid[i][j]-1]=true;
                }
            }
        }
    }
    for (int j=0;j<9;j++) {
        for (int c=0;c<9;c++) {
            nums[c]=false;
        }
        for (int i=0;i<9;i++) {
            if (grid[i][j]) {
                if (nums[grid[i][j]-1]) {
                    return false;
                }
                else {
                    nums[grid[i][j]-1]=true;
                }
            }
        }
    }
    for (int a=0;a<3;a++) {
        for (int b=0;b<3;b++) {
            for (int c=0;c<9;c++) {
                nums[c]=false;
            }
            for (int i=0;i<3;i++) {
                for (int j=0;j<3;j++) {
                    if (grid[i+3*a][j+3*b]) {
                        if (nums[grid[i+3*a][j+3*b]-1]) {
                            return false;
                        }
                        else {
                            nums[grid[i+3*a][j+3*b]-1]=true;
                        }
                    }
                }
            }
        }
    }
    return true;
}

Square findMinOptions(int grid[9][9], int candidates[9][9][9]) {
    Square retVal;
    //cout<<retVal.i<<retVal.j<<"!"<<endl;
    int minOpt=10;//impossible value
    int cur;
    for (int i=0;i<9;i++) {
        for (int j=0;j<9;j++) {
            if (!grid[i][j]) {
                //cout<<i<<j<<">:("<<endl;
                cur=9;
                for (int c=0;c<9;c++) {
                    if (candidates[i][j][c]) {
                        cur--;
                    }
                }
                if (cur<minOpt) {
                    //cout<<"$"<<i<<j<<endl;
                    minOpt=cur;
                    retVal.i=i;
                    retVal.j=j;
                }
            }
        }
    }
    //cout<<retVal.i<<retVal.j<<"!"<<endl;
    return retVal;
}

bool guess(int i, int j, int num, int grid[9][9], int candidates[9][9][9]) {
    //cout<<"Called "<<i<<j<<num<<endl;
    //cout<<grid[0][0]<<endl;
    grid[i][j]=num;
    updateCandidates(i,j,num,candidates,false);
    Square minOpt = findMinOptions(grid, candidates);
    if (minOpt.i==-1) {
        return true;
        //this implies that there is no unfilled square in the grid
        //otherwise it would be guaranteed to return a "valid" minOpt
    }
    //cout<<minOpt.i<<minOpt.j<<endl;
    for (int c=0;c<9;c++) {
        if (!candidates[minOpt.i][minOpt.j][c]) {
            bool result=guess(minOpt.i,minOpt.j,c+1,grid,candidates);
            if (result) {
                return true;
            }
            else {
                updateCandidates(minOpt.i,minOpt.j,c+1,candidates,true);
                grid[minOpt.i][minOpt.j]=0;
                x++;
            }
        }
    }
    //we only reach here if there is an empty square with 0 choices that lead to a valid sudoku
    //the base case occurs when a square is blocked on all 9 choices
    return false;
}

int main() {
    ifstream myCin("med.txt");//name of the file that contains the puzzle change this for
    int grid[9][9] = {0};
    int candidates[9][9][9] = {0};
    for (int i=0;i<9;i++) {
        for (int j=0;j<9;j++) {
            myCin>>grid[i][j];
            updateCandidates(i,j,grid[i][j],candidates,false);
        }
    }
    myCin.close();
    Square minOpt = findMinOptions(grid, candidates);
    //cout<<minOpt.i<<minOpt.j<<endl;
    for (int c=0;c<9;c++) {
        //cout<<c<<' '<<candidates[minOpt.i][minOpt.j][c]<<endl;
        if (!candidates[minOpt.i][minOpt.j][c]) {
            //cout<<c<<endl;
            bool result=guess(minOpt.i,minOpt.j,c+1,grid,candidates);
            if (result) {
                break;
            }
            else {
                updateCandidates(minOpt.i,minOpt.j,c+1,candidates,true);
                grid[minOpt.i][minOpt.j]=0;
                x++;
            }
        }
        if (c==8) {
            //none of the guess lead to a solved sudoku :(
            cout<<"The Sudoku given has no solution (or my code is buggy)"<<endl;
        }
    }
    for (int i=0;i<9;i++) {
        for (int j=0;j<9;j++) {
            cout<<grid[i][j]<<' ';
        }
        cout<<'\n'<<endl;
    }

    cout<<x<<" wrong guesses"<<endl;
    //the program does a sort of educated guess and check strategy to solve

    assert(isValid(grid));//to ensure a correct result

    return 0;
}