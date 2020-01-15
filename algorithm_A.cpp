#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"
#include "../include/rules.h"

using namespace std;
#define ROWS 5
#define COLS 6
/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/

//BoardEva, chooseorb, gametree;

struct node{
    int score;
    Board B;
};

class gametree{
    private:
    Board board;
    Player *player, *oppo;
    public:
    gametree(Board board, Player *player);
    int chooseorb();
    int Evaluate(Board B, Board after, int x, int y);
    int minmax(int score[30], int min_max);
};

gametree::gametree(Board board, Player *player){
    this->player = player;
    this->board = board;
    char colorPlayer = player->get_color();
    char colorOpponent = 'v'; // virtual player
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            if(board.get_cell_color(i,j)!='w' && board.get_cell_color(i,j)!=colorPlayer)
                colorOpponent = board.get_cell_color(i,j);
        }
    }
    this->oppo = new Player(colorOpponent);
}
//30+30*30*30
//2*2*2*2
//x,y,B * 4;
int gametree::chooseorb(){
    node temp[4];
    int scoresum[4][ROWS*COLS], maxscorenum;
    int colorplayer = player->get_color();
    int coloroppo = oppo->get_color();

    maxscorenum = 0;
    temp[0].B = this->board;
    for(int r = 0; r < ROWS*COLS; r++){
        scoresum[0][r] = -999;
    }
    for(int i = 0; i < ROWS*COLS;i++){
        for(int r = 0; r < ROWS*COLS; r++){
            scoresum[1][r] = 999;
        }
        if(board.get_cell_color(i/6,i%6) == colorplayer || board.get_cell_color(i/6,i%6) == 'w'){
            temp[0].B = this->board;            
            (temp[0].B).place_orb(i/6, i%6, this->player);
            if((temp[0].B).win_the_game(*player)) return i;
            temp[0].score = Evaluate(board, temp[0].B,i/6,i%6);
            for(int i2 = 0; i2 < ROWS*COLS;i2++){
                for(int r = 0; r < ROWS*COLS; r++){
                    scoresum[2][r] = -999;        
                }
                if(board.get_cell_color(i2/6,i2%6) == coloroppo || board.get_cell_color(i2/6,i2%6) == 'w'){
                    temp[1].B = temp[0].B;
                    (temp[1].B).place_orb(i2/6, i2%6, this->oppo);
                    temp[1].score = Evaluate(temp[0].B,temp[1].B,i2/6,i2%6);
                    for(int i3 = 0; i3 < ROWS*COLS;i3++){
                        for(int r = 0; r < ROWS*COLS; r++){
                            scoresum[3][r] = 999;        
                        }
                        if(board.get_cell_color(i3/6,i3%6) == colorplayer || board.get_cell_color(i3/6,i3%6) == 'w'){
                            temp[2].B = temp[1].B;
                            (temp[2].B).place_orb(i3/6, i3%6, this->player);
                            temp[2].score = Evaluate(temp[1].B,temp[2].B,i3/6,i3%6);
                            for(int i4 = 0; i4 < ROWS*COLS;i4++){
                                if(board.get_cell_color(i4/6,i4%6) == coloroppo || board.get_cell_color(i4/6,i4%6) == 'w'){
                                    temp[3].B = temp[2].B;
                                    (temp[3].B).place_orb(i4/6, i4%6, this->oppo);
                                    scoresum[3][i4] = Evaluate(temp[2].B,temp[3].B,i4/6,i4%6);
                                }
                            }
                            scoresum[2][i3] = minmax(scoresum[3],0);
                            //cout << "" << scoresum[2][i3];
                        }
                    }
                    scoresum[1][i2] = minmax(scoresum[2],1);
                    //cout << "*" << scoresum[1][i2];
                }
            }
            scoresum[0][i] = minmax(scoresum[1],0);
            //cout << "scoresum[0][i]: "  << i << " "<< scoresum[0][i] << endl;
            if(scoresum[0][i] > scoresum[0][maxscorenum]) maxscorenum = i;
            else if(scoresum[0][i] == scoresum[0][maxscorenum] && board.get_cell_color(i/6,i%6) == 'w' 
            && board.get_capacity(i/6,i%6) < board.get_capacity(maxscorenum/6,maxscorenum%6)) maxscorenum = i;
            else if(scoresum[0][i] == scoresum[0][maxscorenum] && rand()%4 == 0) maxscorenum = i;
        }
    }
    return maxscorenum;
}
int gametree::minmax(int score[30], int min_max){
    int min, max;
    min = max = score[0];
    for(int i = 0; i < ROWS*COLS; i++){
        if(score[i] > max){
            max = score[i];
        }
        if(score[i] < min){
            min  = score[i];
        }
    }
    if (min_max == 1) return max;
    else return min;
}

int gametree::Evaluate(Board B, Board After, int x, int y){
    int orbplayer, orboppo, Score;
    int playercolor = player->get_color();
    int oppocolor = oppo->get_color();
    Score = orbplayer = orboppo = 0;
    for(int i=0;i<ROWS;i++){
        for(int j=0;j<COLS;j++){
            // 考慮座標(i,j)的orb數量以及四個方向的對手orb數量
            if(B.get_cell_color(i,j) == playercolor){
                orbplayer -= B.get_orbs_num(i,j);
            }
            else if(B.get_cell_color(i,j) == oppocolor){
                orboppo += B.get_orbs_num(i,j);
            }
            if(After.get_cell_color(i,j) == playercolor){
                orbplayer += B.get_orbs_num(i,j);
            }
            else if(After.get_cell_color(i,j) == oppocolor){
                orboppo -= B.get_orbs_num(i,j);
            }
        }
    }
    Score += (orbplayer-orboppo)*3/2;
    // 計算四個方向的opponent orb數量(越多代表這個cell越值得引爆)
    if(B.get_cell_color(x,y) == playercolor){
        if(index_range_illegal(x-1, y) == true){
            if(B.get_cell_color(x-1, y) == oppocolor){
                if(B.get_orbs_num(x-1,y) < B.get_orbs_num(x,y)){
                    Score += (B.get_orbs_num(x-1,y));
                }  
                else{
                    if(B.get_orbs_num(x-1,y) >= 2)
                    Score -= B.get_orbs_num(x-1,y)+(B.get_orbs_num(x,y))-1;
                } 
            }     
        }
        if(index_range_illegal(x+1,y)==true){
            if(B.get_cell_color(x+1, y) == oppocolor){
                if(B.get_orbs_num(x+1,y) < B.get_orbs_num(x,y)){
                    Score += (B.get_orbs_num(x+1,y));
                }  
                else{
                    if(B.get_orbs_num(x+1,y) >= 2)
                    Score -= B.get_orbs_num(x+1,y)+(B.get_orbs_num(x,y))-1;
                } 
            }     
        }
        if(index_range_illegal(x,y+1)==true){
            if(B.get_cell_color(x, y+1) == oppocolor){
                if(B.get_orbs_num(x,y+1) < B.get_orbs_num(x,y)){
                    Score += (B.get_orbs_num(x,y+1));
                }  
                else{
                    if(B.get_orbs_num(x,y+1) >= 2)
                    Score -= B.get_orbs_num(x,y+1)+(B.get_orbs_num(x,y))-1;
                } 
            }     
        }
        if(index_range_illegal(x,y-1)==true){
            if(B.get_cell_color(x, y-1) == oppocolor){
                if(B.get_orbs_num(x,y-1) < B.get_orbs_num(x,y)){
                    Score += (B.get_orbs_num(x,y-1));
                }  
                else{
                    if(B.get_orbs_num(x,y-1) >= 2)
                    Score -= B.get_orbs_num(x,y-1)+(B.get_orbs_num(x,y))-1;
                } 
            }     
        }
    }
    return Score;
}
 
void algorithm_A(Board board, Player player, int index[]){
    // //cout << board.get_capacity(0, 0) << endl;
    // //cout << board.get_orbs_num(0, 0) << endl;
    // //cout << board.get_cell_color(0, 0) << endl;
    // board.print_current_board(0, 0, 0);
    //////////// Random Algorithm ////////////
    // Here is the random algorithm for your reference, you can delete or comment it.
    int row, col;
    srand(time(NULL)*time(NULL));
    gametree T(board, &player);
    int node = T.chooseorb();
    //cout << "result:   " << node << endl;
    index[0] = node/6;
    index[1] = node%6;
}