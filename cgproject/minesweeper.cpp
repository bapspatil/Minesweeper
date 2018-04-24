/**
 Copyright 2018 Bapusaheb Patil
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include<GLUT/GLUT.h>
#include<iostream>
#include<stdlib.h>
#include<iomanip>

using namespace std;

enum {UNCOVER,COVER,MINE=-1,BLANK,ADJACENT,LOSE=-1,UNKNOWN,WIN,IN_PROGRESS,GAME_OVER};
const int dim=10,mines=10;
const int max_x=100,max_y=100,xint=max_x/dim,yint=max_y/dim;
int flag=0;
class cell
{
    public :
    int content ;
    //0 blank,1-8 adjacent mines,-1 mine
    int status;
    //0 uncovered ,1 covered
    cell()
    {
        content=BLANK;status=COVER;
    }
};
cell board[dim][dim];
class GAMEPLAY
{
    public:
        int flagged,uncovered,covered,status,result;
        int mine_pos[mines][2],mine_count;
        GAMEPLAY()
        {
            uncovered=0;
            covered=dim*dim;
            result=UNKNOWN;
            mine_count=0;
            
            for(int i=0;i<mines;i++)
                for(int j=0;j<2;j++)
                    mine_pos[i][j]=-1;                      //make two dimentional array to store the position of mines
            
        }
        void add_mine(int i,int j)
        {
            
            mine_pos[mine_count][0]=i;
            mine_pos[mine_count][1]=j;
            mine_count++;
        }
        int check()
        {
            if(result!=UNKNOWN)
                return result;
            
            if(covered==10)
            {
                status=GAME_OVER;
                result=WIN;
                return WIN;
            }
            
            
            for(int i=0;i<mines;i++)
            {
                if(UNCOVER==board[mine_pos[i][0]][mine_pos[i][1]].status)
                {
                    status=GAME_OVER;
                    result=LOSE;
                    return LOSE;
                }
                
            }
            return UNKNOWN;
        }
};

GAMEPLAY game_stats;

int main(int argc, char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(1200,650);
    glutInitWindowPosition(100,50);
    glutCreateWindow("Minesweeper");
    glutDisplayFunc(mydisplay);
    glutKeyboardFunc(myKeyboardFunc);
    glutMouseFunc(user_input);
    
    gl_init();
    
    glutMainLoop();
    return 0;
}

void mydisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    if(flag==0)
        front();
    if(flag==1)
        display();
}

void init_mines(int num,int dim)
{
    int i=0,j=0,count=0;
    while(count!=num)
    {
        
        i=rand()%dim;
        j=rand()%dim;
        
        if(board[i][j].content!=MINE)
        {
            board[i][j].content=MINE;
            count++;
            game_stats.add_mine(i,j);
        }
        
    }
    
}

void init_mines_adjacent(int num, int dim)
{
    
    int i,j,count;
    for(i=0;i<dim;i++)
        for(j=0;j<dim;j++)
        {
            
            if(board[i][j].content!=MINE)
            {calc_adjacent_mines(i,j,dim);
                count++;}
        }
}

void calc_adjacent_mines(int i,int j,int dim)
{
    //row above mines
    if(i-1>=0)
    {
        if(j-1>=0)
            if(board[i-1][j-1].content==MINE)
                board[i][j].content++;
        if(board[i-1][j].content==MINE)
            board[i][j].content++;
        
        if(j+1<dim)
            if(board[i-1][j+1].content==MINE)
                board[i][j].content++;
        
        
    }
    if(j-1>=0)
        if(board[i][j-1].content==MINE)
            board[i][j].content++;
    if(j+1<dim)
        if(board[i][j+1].content==MINE)
            board[i][j].content++;
    
    if(i+1<dim)
    {
        if(j-1>=0)
            if(board[i+1][j-1].content==MINE)
                board[i][j].content++;
        if(board[i+1][j].content==MINE)
            board[i][j].content++;
        
        if(j+1<dim)
            if(board[i+1][j+1].content==MINE)
                board[i][j].content++;
        
        
    }
    
    
}

void initiate_board()
{
    init_mines(mines,dim);
    init_mines_adjacent(mines,dim);
}

void uncover_cell(int i , int j)
{
    
    switch(board[i][j].content)
    {
        case MINE       :   show_content(i,j);
            break;
        case BLANK      :   show_content(i,j);
            uncover_area(i,j);
            break;
        case 1          :   show_content(i,j);
            break;
        case 2          :   show_content(i,j);
            break;
        case 3          :   show_content(i,j);
            break;
        case 4          :   show_content(i,j);
            break;
        case 5          :   show_content(i,j);
            break;
        case 6          :   show_content(i,j);
            break;
        case 7          :   show_content(i,j);
            break;
            
        case 8          :   show_content(i,j);
            break;
        default         :   break;
            
            
    }
}


void user_input(int button,int state,int x,int y)
{
    int square_x=x/120;
    int square_y=(650-y)/65;
    
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&flag==1)
    {
        left_click(square_x,square_y);
    }
}

void printMessage(char *msg)
{
    glColor3f(1.0,0.0,0.0);
    glRasterPos2i(xint*4,yint*4);
    printString(msg);
    glFlush();
}

void gl_init()
{
    glClearColor(0.0,0.0,0.0,0);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glOrtho(0.0,100.0,0.0,100.0,-100.0,100.0);
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glFlush();
    makeRasterFont();
    
}

