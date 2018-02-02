//Name: Xianyu Peng; SUID: 06187373
//This game of life program implements a simplified version of Conway's simulation.
//The program shows a basic user interface for watching the bacteria grow over time.

#include <iostream>
#include <fstream>
#include "console.h"
#include "filelib.h"
#include "strlib.h"
#include "vector.h"
#include "grid.h"
#include "simpio.h"
#include "lifegui.h"

using namespace std;
void intro();
void findneigbors(Grid<string> matrix,Grid<string>& matrixnew);
void fileread(Vector<string>& words);
void filegrid(const Vector<string> words, Grid<string>&bacteria);
void animate(Grid<string>& matrix,Grid<string> matrixnew);


int main(){
    //The GUI first appears when the program's console pops up.
    LifeGUI::initialize();
    intro();    
    int point=1;
    while (point<2){        
        LifeGUI::repaint();
        Vector <string> words {};
        fileread(words);
        int row=stringToInteger(words[0]);
        int column=stringToInteger(words[1]);
        LifeGUI::resize(row, column);
        Grid<string> bacteria(row,column);        
        filegrid(words,bacteria);
        Grid<string> bacterianew(row,column);
        bacterianew=bacteria;

       // Prompt the user for entering an action and check the valid input.
        while (1) {
            string order=getLine("a)nimate, t)ick, q)uit?");

            //When the user enter a or A, prompt the user for entering an integer as the number of frames.
            //Then check the valid input and begin the animation.
            if (order =="a"||order =="A" ){
                animate(bacteria,bacterianew);

            //When the user enter t or T or '\n', forward the bacteria simulation by one generation.
            } else if (order.size()==0||order =="T" || order =="t"){
                findneigbors(bacteria,bacterianew);
                bacteria=bacterianew;

            //Quit the program.
            } else if (order =="Q" || order =="q"){
                string order2=getLine("Load another file? (y/n)");
                if (order2=="n"||order2 =="N"){
                    point=2;
                    break;
                }else if (order=="y"||order!="Y"){
                    break;
                }else{
                    cout<<"Please type a word that begins with 'Y' or 'N'."<<endl;
                }
            } else {
                cout<<"Invalid choice; please try again."<<endl;
            }

        }

    }

    cout << "Have a nice Life!" << endl;
    return 0;
}


// A welcome message to introduce the game to the user.
void intro(){
    cout<<"Welcome to the CS 106B/X Game of Life!"<<endl;
    cout<<"This program simulates the lifecycle of a bacterial colony."<<endl;
    cout<<"Cells (X) live and die by the following rules:"<<endl;
    cout<<"* A cell with 1 or fewer neighbors dies."<<endl;
    cout<<"* Locations with 2 neighbors remain stable."<<endl;
    cout<<"* Locations with 3 neighbors will create life"<<endl;
    cout<<"* A cell with 4 or more neighbors dies."<<endl;
}

// Prompt the user for a file name and check for valid user input.
void fileread(Vector<string>& words){
    ifstream input;
    while (1) {
        string name=getLine("Grid input file name?");
        if (fileExists(name)){
            input.open(name);
            break;
        } else{
            cout << "Unable to open that file.  Try again." << endl;
        }
    }
    string line;

    while (getline(input,line)){
        words+=line;
    }
}

// Use the file's contents to set the initial state of the bacterial colony grid.
void filegrid(const Vector<string> words, Grid<string>&bacteria){
    int row=stringToInteger(words[0]);
    int column=stringToInteger(words[1]);
    for (int i = 2; i < row+2; i++) {
        for (int j = 0; j < column; j++) {
            bacteria[i-2][j]=words[i][j];
            cout<<bacteria[i-2][j];
            if (bacteria[i-2][j]=="X"){
                LifeGUI::fillCell(i-2, j);
            }
        }
        cout<<endl;
    }
    LifeGUI::repaint();
}

// Perform animation.
void animate(Grid<string>& matrix,Grid<string> matrixnew){
    int frames=getInteger("How many frames?");
    int no=1;
    while (no<frames+1){
        clearConsole();
        findneigbors(matrix,matrixnew);
        matrix=matrixnew;
        no+=1;
        pause(100.00);
    }
}

// Forward the bacteria simulation by one generation.
void findneigbors(Grid<string> matrix,Grid<string>& matrixnew){
    Grid<int> counts(matrix.numRows(),matrix.numCols());
    matrixnew=matrix;
    int rmin;
    int rplus;
    int cmin;
    int cplus;
    LifeGUI::clear();
    for (int r=0; r<matrix.numRows();r++){
        for (int c=0; c<matrix.numCols();c++){
            int count=0;

            // Define the neighbors.
            if (r==0){
                    rmin=matrix.numRows()-1;
                    rplus=r+1;
            } else if (r==matrix.numRows()-1){
                    rmin=r-1;
                    rplus=0;
            } else{
                    rmin=r-1;
                    rplus=r+1;
            }
            if (c==0){
                    cmin=matrix.numCols()-1;
                    cplus=c+1;
            } else if (c==matrix.numCols()-1){
                    cmin=c-1;
                    cplus=0;
            } else{
                    cmin=c-1;
                    cplus=c+1;
            }

            //Calculate the number of neighbors.
            if (matrix[rmin][cmin]=="X"){
                count+=1;
            }
            if (matrix[rmin][c]=="X"){
                count+=1;
            }
            if (matrix[rmin][cplus]=="X"){
                count+=1;
            }
            if (matrix[r][cmin]=="X"){
                count+=1;
            }
            if (matrix[r][cplus]=="X"){
                count+=1;
            }
            if (matrix[rplus][cmin]=="X"){
                count+=1;
            }
            if (matrix[rplus][c]=="X"){
                count+=1;
            }
            if (matrix[rplus][cplus]=="X"){
                count+=1;
            }

            //Determine the status of next state.
            if (count<2 || count>3){
                matrixnew[r][c]="-";
            } else if (count==3){
                matrixnew[r][c]="X";
            }

            counts[r][c]=count;
            cout<<matrixnew[r][c];
            if (matrixnew[r][c]=="X"){
                LifeGUI::fillCell(r, c);
            }
        }
        cout<<endl;
    }
    LifeGUI::repaint();
}

