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
void drawstring(float x,float y, float z ,char *string);
void front(void);
void initiate_board();
void draw_board();
void init_mines(int num,int dim);
void init_mines_adjacent(int num, int dim);
void calc_adjacent_mines(int i,int j,int dim);
void uncover_cell(int i, int j);
void uncover_area(int i, int j);
void uncover_area_check_cell(int k, int l);
void left_click(int i,int j);
void game_over(int result);

void draw_square(int x,int y,int color);
void user_input(int button,int state,int x,int y);
void show_content(int i,int j);

void myKeyboardFunc( unsigned char key, int x, int y );
void gl_init();
void display();
void mydisplay(void);
void printMessage(char *msg);
void printString(char *s);
void makeRasterFont(void);

/**************************main************************/
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
    initiate_board();
    
    glutMainLoop();
    return 1;
}






void mydisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    if(flag==0)
        front();
    if(flag==1)
        display();
    
}




/*********init_mines********/
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

/********init_mine_adjacent*********/

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

/********calc_adjacent_mines*********/
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


/**********initiate_board***********/
void initiate_board()
{
    
    init_mines(mines,dim);
    init_mines_adjacent(mines,dim);
}





/***************uncover_cell*******************/
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

/**********uncover_area_check_cell******************/
void uncover_area_check_cell(int k, int l)
{
    if(board[k][l].status==COVER)
    {
        if(board[k][l].content!=MINE)
        {
            board[k][l].status=UNCOVER;
            game_stats.covered--;
            game_stats.uncovered++;
            draw_square(k,l,UNCOVER);
            show_content(k,l);
            if(board[k][l].content==BLANK)
                uncover_area(k,l);
        }
        
    }
}

/**********uncover_area******************/
void uncover_area(int i, int j)
{
    
    int k=i,l=j;
    
    if(i-1>=0)
        uncover_area_check_cell(i-1,j);
    if(i+1<dim)
        uncover_area_check_cell(i+1,j);
    if(j-1>=0)
        uncover_area_check_cell(i,j-1);
    if(j+1<dim)
        uncover_area_check_cell(i,j+1);
}

/************left_click*****************/
void left_click(int i,int j)
{
    if(board[i][j].status==COVER)
    {
        if(UNKNOWN!=game_stats.check())
        {
            game_over(game_stats.check());
            return;
        }
        
        board[i][j].status=UNCOVER;
        game_stats.covered--;
        game_stats.uncovered++;
        draw_square(i,j,UNCOVER);
        uncover_cell(i,j);
        if(UNKNOWN!=game_stats.check())
        {
            game_over(game_stats.check());
            return;
        }
    }
}


/*************game_over*******************/
void game_over(int result)
{
    
    if(result!=UNKNOWN)
    {
        glutMouseFunc(NULL);
        
        if(result==WIN)
            printMessage("YOU WIN");
        else
            printMessage("YOU LOSE");
    }
    
}



/**************************************************
 Graphic routines
 *********************************************/



/**********user_input***********/

void user_input(int button,int state,int x,int y)
{
    int square_x=x/120;
    int square_y=(650-y)/65;
    
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&flag==1)
    {
        left_click(square_x,square_y);
    }
}


/***********************draw_board**********************/
void draw_board()
{
    
    int x_coord=0,y_coord=0;
    
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINES);
    for(x_coord=0;x_coord<=max_x;x_coord+=xint)
    {
        
        glVertex3i(x_coord,0,0);
        glVertex3i(x_coord,max_y,0);
    }
    for(y_coord=0;y_coord<=max_y;y_coord+=yint)
    {
        glVertex3i(0,y_coord,0);
        glVertex3i(max_x,y_coord,0);
    }
    for(int i=0;i<dim;i++)
    {
        for(int j=0;j<dim;j++)
        {
            
            draw_square(i,j,board[i][j].status);
            if(board[i][j].status==UNCOVER)
                show_content(i,j);
            
        }
    }
    
    glEnd();
    glFlush();
    
}
/**********draw_square************/
void draw_square(int i,int j,int color)
{
    int x,y;
    if(color==COVER)
        glColor3f(0.89,0.0,0.0);
    else if(color==UNCOVER)
        glColor3f(0,1,0);
    x=i*xint;
    y=j*yint;
    glBegin(GL_POLYGON);
    glVertex3i(x+1,y+1,0);
    glVertex3i(x+1,y+yint-1,0);
    glVertex3i(x+xint-1,y+yint-1,0);
    glVertex3i(x+xint-1,y+1,0);
    
    
    glEnd();
    glFlush();
    
}
/************************print_message********************/
void printMessage(char *msg)
{
    glColor3f(1.0,0.0,0.0);
    glRasterPos2i(xint*4,yint*4);
    printString(msg);
    glFlush();
}

/*************************show_content****************/
void show_content(int i,int j)
{
    char temp=board[i][j].content+48;
    if(board[i][j].content==MINE)
        temp=77;
    if(board[i][j].content==BLANK)
        temp=32;
    
    glColor3f(0.0,0.0,0.0);
    glRasterPos2i(i*xint+2,j*yint+2);
    printString(&temp);
    glFlush();
    
}



/**********gl-init*************/
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

/**********************display *************************/
void display()
{
    static int i=0;
    draw_board();
    
    game_over(game_stats.check());
}

/*************************Font_display_list*************************/
GLubyte space[] =
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
GLubyte letters[][13] = {
    {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
    {0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
    {0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
    {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
    {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
    {0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
    {0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
    {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
    {0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},
    {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},
    {0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
    {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
    {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff}
};
GLubyte digits[][13] = {
    {0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff},
    {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80},
    {0xff, 0x80, 0x80, 0x80, 0x80, 0x80, 0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff},
    {0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81},
    {0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0x80, 0x80, 0x80, 0x80, 0x80, 0xff},
    {0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0x80, 0x80, 0x80, 0x80, 0x80, 0xff},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff},
    {0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff}
};
GLuint fontOffset;

void makeRasterFont(void)
{
    GLuint i, j;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    fontOffset = glGenLists (128);
    for (i = 0,j = 'A'; i < 26; i++,j++) {
        glNewList(fontOffset + j, GL_COMPILE);
        glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, letters[i]);
        glEndList();
    }
    for (i = 0,j = '0'; i < 10; i++,j++) {
        glNewList(fontOffset + j, GL_COMPILE);
        glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, digits[i]);
        glEndList();
    }
    glNewList(fontOffset + ' ', GL_COMPILE);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, space);
    glEndList();
}
void printString(char *s)
{
    glPushAttrib (GL_LIST_BIT);
    glListBase(fontOffset);
    glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
    glPopAttrib ();
}








void front(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0,0,1);
    drawstring(20.0,90.0,0.0,"COLLEGE OF ENGINEERING");
    glColor3f(0.7,0,1);
    drawstring(21,82,0.0,"DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING");
    glColor3f(1,0.5,0);
    drawstring(38,70,0.0,"A MINI PROJECT ON");
    glColor3f(1,0,0);
    drawstring(40,60,0.0,"MINESWEEPER");
    glColor3f(1,0.5,0);
    drawstring(20,50,0.0,"By:");
    glColor3f(0.5,0,0.5);
    drawstring(10,40,0.0,"Some Name");
    
    drawstring(10,34,0.0,"Some Name");
    glColor3f(1,0.5,0);
    drawstring(68,50,0.0,"Guides:");
    glColor3f(0.5,0.2,0.2);
    drawstring(63,40,0.0,"Mr. Some Name");
    
    drawstring(63,34,0.0,"Mr.Some Name");
    glColor3f(1,0.1,1);
    drawstring(32,10,0.0," Press Enter to start MINESWEEPER");
    glFlush();
    glutSwapBuffers();
}

void drawstring(float x,float y, float z ,char *string)
{
    char *c;
    glRasterPos3i(x,y,z);
    
    for(c=string;*c!='\0';c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*c);
    }
}

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch(key)
    {
            
        case 13:if(flag==0)
            flag=1;
            break;
        case 27 :exit(0);
            
    }
    
    mydisplay();
}
