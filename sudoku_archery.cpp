#include <GL/glut.h>
#include <stdio.h>


int MAX_X = 500;
int MAX_Y = 600;
double VERTICAL_SPEED_OF_ARROWS = 0.4;
double HORIZONTAL_SPEED_OF_ARROWS = 2;
double TARGET_VELOCITY = 5;

double TARGET_X_LOWER_BOUND = 300;
double TARGET_X_UPPER_BOUND = 450;
double TARGET_Y_LOWER_BOUND = 100;
double TARGET_Y_UPPER_BOUND = 400;

int count = 0;
int view = 0;
double x = 25, y = 50;
char str[10];
void id1();
void id();
void draw_target();
void display_string(double, double, char*, int);

// 0 means target is not hit
int target_state[10];
// take random positions for the targets within x 300 to 450 and y 100 to 500
double target_x[10];
double target_y[10];
// every target has a direction up or down
int target_direction[10];

/*based on count display no of arrows and result of game*/
void counting()
{
    sprintf(str, "No of Arrows:%d", count);
    display_string(40, 40, str, 2);
    int win = 1;
    for (int i = 0; i < 10; i++)
    {
        if (target_state[i] == 0)
        {
            win = 0;
            break;
        }
    }

    if (win)
    {
        display_string(5, 300, "CONGRATULATION YOU WON", 1);

        glutIdleFunc(NULL);
    }
    else if (count >= 30)
    {
        display_string(5, 300, "GAME OVER! YOU LOST", 1);
        display_string(5, 100, "PRESS n TO PLAY AGAIN", 2);
        display_string(5, 80, "PRESS q TO EXIT", 2);

        glutIdleFunc(NULL);
    }
}

/*to move arrow up*/
void id()
{
    if (view == 1)
    {
        y += VERTICAL_SPEED_OF_ARROWS;
        if (y > MAX_Y)
        {
            y = 0;
            count++;
        }
    }
    glutPostRedisplay();
}

/*to display bitmap char (strings). preview page instruction resizing, call it from display1() function*/
void display_string(double x, double y, char* string, int font)
{
    int len, i;
    glColor3f(0.8, 0.52, 1.0);
    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        if (font == 1)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
        if (font == 2)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
        if (font == 3)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
        if (font == 4)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
    }
}

// show different instruction on archery preview page
void display1(void)
{

    glClearColor(0.0, 0.5, 0.5, 1.0);
    display_string(100, 300, "ARCHERY GAME", 1);
    display_string(100, 250, "MOUSE", 2);
    display_string(150, 230, "PRESS RIGHT BUTTON FOR MENU", 3);
    display_string(100, 210, "KEYBOARD", 2);
    display_string(150, 190, "PRESS 'r' TO RELEASE ARROWS", 3);
    display_string(150, 180, " PRESS 'q' TO EXIT", 3);
    display_string(100, 150, "PRESS SPACE BAR TO START GAME", 2);

    // Update the display
    glutPostRedisplay();
    glutSwapBuffers();
}

/*to clear screen & set projection mode*/
void init()
{
    glClearColor(0, 0.5, 0.5, 1);
    glColor3f(1, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glMatrixMode(GL_MODELVIEW);
}

/*to draw the arrow*/
void disp()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // view=0 means initial preview page of the archery game 
    if (view == 0)
    {
        init();
        display1();
    }

    else
    {
        glLoadIdentity();
        glColor3f(1, 1, 0);
        display_string(150, 450, "ARCHERY GAME", 1);
        counting();
        // Drawing of arrow
        glColor3f(0, 1, 1);
        glBegin(GL_LINES);
        glVertex2d(x, y);
        glVertex2d(x + 100, y);
        glEnd();
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2d(x, y + 2);
        glVertex2d(x + 100, y + 2);
        glEnd();
        glBegin(GL_LINES);
        glVertex2d(x, y - 2);
        glVertex2d(x + 100, y - 2);
        glEnd();

        //drawing arrow head (triangle)
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); // color of tip
        glVertex2d(x + 100, y + 3);
        glVertex2d(x + 110, y);
        glVertex2d(x + 100, y - 3);
        glEnd();

        //drawing arrow foot (quads)
        glBegin(GL_QUADS);
        glVertex2d(x, y + 3);
        glVertex2d(x, y - 3);
        glVertex2d(x - 10, y - 5);
        glVertex2d(x - 10, y + 5);
        glEnd();

        // Drawing of target
        draw_target();

        glFlush();
        glutSwapBuffers();
    }
}

// initializing the properties of the targets
void init_targets()
{
    for (int i = 0; i < 10; i++)
    {
        //  set to random values within upper and lower bounds
        target_x[i] = rand() % (int)(TARGET_X_UPPER_BOUND - TARGET_X_LOWER_BOUND) + TARGET_X_LOWER_BOUND;
        target_y[i] = rand() % (int)(TARGET_Y_UPPER_BOUND - TARGET_Y_LOWER_BOUND) + TARGET_Y_LOWER_BOUND;
        target_state[i] = 0;
        // initial direction of movement for the target (0 or 1)
        target_direction[i] = rand() % 2;
    }
}

/*to draw the target inside line loop*/
void draw_target()
{
    for (int i = 0; i < 10; i++)
    {
        //show both white points and outside magenta line loop if target_state = 0 means target is not hitted
        if (target_state[i] == 0)
        {
            glColor3f(1, 0, 1);
            glPointSize(30);
            //show the inside white points of the target
            glBegin(GL_POINTS);
            glVertex2f(target_x[i], target_y[i]);
            glEnd();

            //draw a line loop here a outside rectangle of target
            glBegin(GL_LINE_LOOP);
            glVertex2f(target_x[i] - 15, target_y[i] + 15);
            glVertex2f(target_x[i] + 15, target_y[i] + 15);
            glVertex2f(target_x[i] + 15, target_y[i] - 15);
            glVertex2f(target_x[i] - 15, target_y[i] - 15);
            glEnd();
        }

        //if target hits, show only whitle points of target
        else
        {
            glColor3f(1, 1, 1);
            glPointSize(20);
            glBegin(GL_POINTS);
            glVertex2f(target_x[i], target_y[i]);
            glEnd();
        }
    }

    glFlush();
}

void update_targets(int value)
{
    for (int i = 0; i < 10; i++)
    {
        if (target_state[i] == 0)
        {
            if (target_direction[i] == 1)
            {
                target_y[i] += TARGET_VELOCITY;
                if (target_y[i] >= TARGET_Y_UPPER_BOUND)
                {
                    target_direction[i] = 0;
                }
            }
            else
            {
                target_y[i] -= TARGET_VELOCITY;
                if (target_y[i] <= TARGET_Y_LOWER_BOUND)
                {
                    target_direction[i] = 1;
                }
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, update_targets, 0);
}

/* to move the arrow right when 'r' pressed*/
void id1()
{
    x += HORIZONTAL_SPEED_OF_ARROWS;
    // disa();
    for (int i = 0; i < 10; i++)
    {
        if (target_state[i] == 0 && x >= target_x[i] - 15 && x <= target_x[i] + 15 && y >= target_y[i] - 15 && y <= target_y[i] + 15)
        {
            target_state[i] = 1;

            x = 25;
            y = 0;
            count++;

            glutIdleFunc(id);
            break;
        }
    }
    if (x >= MAX_X)
    {
        x = 25;
        y = 0;
        count++;
        glutIdleFunc(id);
    }
    glutPostRedisplay();
}
/*set key to perform desired operation*/
void keys(unsigned char k, int x, int y)
{
    switch (k)
    {

    case ' ':
        disp();
        view = 1;
        init_targets();
        break;
    case 'r':
        glutIdleFunc(id1);
        break;
    case 'n': /* new game */
        count = 0;
        view = 1;
        x = 25, y = 50;
        init_targets();

        disp();
        glutIdleFunc(id);

        break;
    case 'q': /* quit game */
        exit(0);
    }
}
/*sub menu to display instructions*/
void demo_menu(int i)
{

    switch (i)
    {
    case 5:
    case 6:
    case 7:
    case 8:
        break;
    }
}
/*sub menu to display designer names*/
void demo(int i)
{
    switch (i)
    {
    case 9:
    case 11:
        break;
    }
}
void game(int id)
{
    switch (id)
    {
    }
}














// start sudoku game
int checkTest = 0;
const int xMax = 9, xMin = 0, yMax = 9, yMin = 0;
bool checkSolve = true, checkPressNumber = true, checkDemo = true;
unsigned char s[9][9], test_s[9][9], keyboardPress;
int posClick_x, posClick_y;

void Initialize();
void drawSquare(GLint x1, GLint y1, GLint x2, GLint y2, GLint x3, GLint y3, GLint x4, GLint y4);
void keyboard(unsigned char key, int x, int y);
void mouseClick(int button, int state, int x, int y);
void preDraw();
void test();
void clearBoard();
void display();
void printText(int x, int y, char* str);
void printText2(int x, int y, char* str);
void printNumber(unsigned char s, GLint x, GLint y);
void printNumber2(unsigned char s, GLint x, GLint y);
void inputBoard();
void solveBoard(int x, int y);
bool checkBoard(int x, int y, unsigned char check);

void Initialize()
{
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 0, 600);
    for (int i = 0; i < xMax; i++)
        for (int j = 0; j < yMax; j++)
        {
            s[i][j] = '0';
            test_s[i][j] = '0';
        }

}

void drawSquare(GLint x1, GLint y1, GLint x2, GLint y2, GLint x3, GLint y3, GLint x4, GLint y4)
{
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glVertex2i(x3, y3);
    glVertex2i(x4, y4);
    glEnd();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
    {
        keyboardPress = key;
        break;
    }
    case '2':
    {
        keyboardPress = key;
        break;
    }
    case '3':
    {
        keyboardPress = key;
        break;
    }
    case '4':
    {
        keyboardPress = key;
        break;
    }
    case '5':
    {
        keyboardPress = key;
        break;
    }
    case '6':
    {
        keyboardPress = key;
        break;
    }
    case '7':
    {
        keyboardPress = key;
        break;
    }
    case '8':
    {
        keyboardPress = key;
        break;
    }
    case '9':
    {
        keyboardPress = key;
        break;
    }
    default:
    {
        break;
    }
    }
}

void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        posClick_x = x;
        posClick_y = 600 - y;
    }
    glutPostRedisplay();
}

void preDraw()
{

    glColor3f(255, 255, 255);
    glClearColor(0.294f, 0.0f, 0.510f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    GLint x, y;
    for (y = 510; y >= 30; y -= 60)
    {
        for (x = 510; x >= 30; x -= 60)
        {
            drawSquare(x, y + 60, x + 60, y + 60, x + 60, y, x, y);
        }
    }
    glLineWidth(5);
    glPushAttrib(GL_CURRENT_BIT);
    glColor3d(255, 255, 0);

    //draw square of "Load"
    glBegin(GL_LINE_LOOP);
    glVertex2i(765, 570);
    glVertex2i(765, 470);
    glVertex2i(600, 470);
    glVertex2i(600, 570);
    glEnd();

    //draw square of "Check"
    glBegin(GL_LINE_LOOP);
    glVertex2i(765, 430);
    glVertex2i(765, 330);
    glVertex2i(600, 330);
    glVertex2i(600, 430);
    glEnd();

    //draw square of "Solve"
    glBegin(GL_LINE_LOOP);
    glVertex2i(765, 280);
    glVertex2i(765, 180);
    glVertex2i(600, 180);
    glVertex2i(600, 280);
    glEnd();

    //draw square of "Clear Board"
    glBegin(GL_LINE_LOOP);
    glVertex2i(765, 130);
    glVertex2i(765, 30);
    glVertex2i(600, 30);
    glVertex2i(600, 130);
    glEnd();


    glPopAttrib();

    printText(650, 515, "LOAD");
    if (checkTest == 0)
        printText2(125, 583, "Click on \"Load\" to open Sudoku board or input number from keyboard");
    else if (checkTest == 1)
    {
        printText2(225, 583, "Congratulations! You did it");
        int sub_menu, submenu;
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(900, 900);
        glutCreateWindow("ARCHERY GAME");
        sub_menu = glutCreateMenu(demo_menu);
        glutAddMenuEntry("r to release arrow", 5);
        glutAddMenuEntry("50 arrows and 10 blocks present", 6);
        glutAddMenuEntry("lost if arrow count exceeds blocks", 7);
        glutAddMenuEntry("otherwise win", 8);
        submenu = glutCreateMenu(demo);
        glutAddMenuEntry("Tanvir Raihan Tias", 9);
        glutCreateMenu(game);
        glutAddSubMenu("INSTRUCTION", sub_menu);
        glutAddSubMenu("ABOUT", submenu);
        glutAddMenuEntry("QUIT", 2);
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        glutDisplayFunc(disp);
        glutIdleFunc(id);

        glutTimerFunc(0, update_targets, 0);

        glutKeyboardFunc(keys);
        glEnable(GL_DEPTH_TEST);



        // Set the display function

        glutMainLoop();
    }
    else if (checkTest == -1)
        printText2(225, 583, "Try again. That's wrong!!!");
    else
        printText2(225, 583, "Get number from your keyboard");
    printText(640, 370, "CHECK");
    printText(642, 220, "SOLVE");
    printText(640, 70, "CLEAR");

    //Slipt Sudoku Board into 9 parts
    glPushAttrib(GL_CURRENT_BIT);
    glColor3d(255, 255, 0);
    glLineWidth(10);

    glBegin(GL_LINE_LOOP);
    glVertex2i(30, 570);
    glVertex2i(570, 570);
    glVertex2i(570, 30);
    glVertex2i(30, 30);
    glEnd();

    glBegin(GL_LINES);
    glVertex2i(210, 572);
    glVertex2i(210, 29);
    glEnd();

    glBegin(GL_LINES);
    glVertex2i(390, 572);
    glVertex2i(390, 29);
    glEnd();

    glBegin(GL_LINES);
    glVertex2i(30, 390);
    glVertex2i(570, 390);
    glEnd();

    glBegin(GL_LINES);
    glVertex2i(30, 210);
    glVertex2i(570, 210);
    glEnd();

    glPopAttrib();
}






void test()
{
    //set all value of s[9][9] to equal 0 to get input from keyboard and we also can solve it
    if (checkPressNumber == true)
    {
        for (int i = 0; i < xMax; i++)
            for (int j = 0; j < yMax; j++)
                if (checkBoard(i, j, s[i][j]) == false)
                {
                    checkTest = -1;
                    return;
                }
    }
    checkTest = 1;
}

void clearBoard()
{
    checkTest = 0;
    //if s[i][j] = 0, it isn't display on the screen, so we set all value to 0
    for (int i = 0; i < xMax; i++)
        for (int j = 0; j < yMax; j++)
        {
            s[i][j] = '0';
            test_s[i][j] = '0';
        }
}

void display()
{
    preDraw();
    //Click on "Load"
    if (posClick_x >= 600 && posClick_x <= 765 && posClick_y <= 570 && posClick_y >= 470)
    {
        inputBoard();
        checkPressNumber = true;
        checkSolve = true;
        checkTest = -2;
    }

    //Click on "Solve"
    if (posClick_x >= 600 && posClick_x <= 765 && posClick_y <= 280 && posClick_y >= 180)
    {
        solveBoard(0, 0);
        checkPressNumber = false;
        checkDemo = false;
        for (int i = 0; i < xMax; i++)
            for (int j = 0; j < yMax; j++)
                if (s[i][j] == '0')
                {
                    printText2(620, 200, "Can't solve. Try again!");
                    break;
                }
    }

    //Click on "Test"
    if (posClick_x >= 600 && posClick_x <= 765 && posClick_y <= 430 && posClick_y >= 330)
    {
        checkSolve = true;
        test();
        checkDemo = false;
    }

    //Click on "Clear Board"
    if (posClick_x >= 600 && posClick_x <= 765 && posClick_y <= 130 && posClick_y >= 30)
    {
        clearBoard();
        checkSolve = true;
        checkPressNumber = true;
    }

    if (30 <= posClick_x && 570 >= posClick_x && 30 <= posClick_y && 570 >= posClick_y)
    {
        checkTest = -2;
        int j = 8 - (posClick_x - 30) / 60;
        int i = (posClick_y - 30) / 60;
        if (test_s[i][j] == '0')
            s[i][j] = keyboardPress;
        keyboardPress = test_s[i][j];
        posClick_x = 0;
        posClick_y = 0;
    }

    GLint x, y;
    for (y = 510; y >= 30; y -= 60)
    {
        for (x = 510; x >= 30; x -= 60)
        {
            if (s[y / 60][8 - x / 60] != '0')
            {
                glPushAttrib(GL_CURRENT_BIT);
                if (test_s[y / 60][8 - x / 60] == '0')
                    printNumber(s[y / 60][8 - x / 60], x + 25, y + 20);
                else
                    printNumber2(s[y / 60][8 - x / 60], x + 25, y + 20);
                glPopAttrib();
            }
        }
    }
    glFlush();
}

void printText(int x, int y, char* str)
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(229, 236, 234);
    glRasterPos2f(x, y);
    int n = (int)strlen(str);
    for (int i = 0; i < n; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }
    glPopAttrib();
}

void printText2(int x, int y, char* str)
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(229, 236, 234);
    // Draw your text
    glRasterPos2f(x, y);
    int n = (int)strlen(str);
    for (int i = 0; i < n; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
    }
    glPopAttrib();
}

void printNumber(unsigned char s, GLint x, GLint y)
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(229, 236, 234);
    glRasterPos2i(x, y);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s);
    glPopAttrib();
}

void printNumber2(unsigned char s, GLint x, GLint y)
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(0, 255, 255);
    glRasterPos2i(x, y);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s);
    glPopAttrib();
}

void inputBoard()
{
    for (int i = xMax - 1; i >= xMin; i--)
    {
        for (int j = yMax - 1; j >= yMin; j--)
        {
            if (i == 1 && j == 1)
                s[i][j] = '3';
            else if (i == 2 && j == 2)
                s[i][j] = '9';
            else if (i == 0 && j == 0)
                s[i][j] = '1';
            else if (i == 1 && j == 3)
                s[i][j] = '8';
            else if (i == 2 && j == 3)
                s[i][j] = '1';
            else if (i == 2 && j == 5)
                s[i][j] = '4';
            else if (i == 2 && j == 4)
                s[i][j] = '2';
            else if (i == 1 && j == 5)
                s[i][j] = '9';
            else if (i == 1 && j == 6)
                s[i][j] = '1';
            else if (i == 1 && j == 7)
                s[i][j] = '4';
            else if (i == 2 && j == 8)
                s[i][j] = '6';


            else if (i == 4 && j == 1)
                s[i][j] = '1';
            else if (i == 4 && j == 4)
                s[i][j] = '9';
            else if (i == 4 && j == 7)
                s[i][j] = '6';
            else if (i == 5 && j == 1)
                s[i][j] = '9';
            else if (i == 5 && j == 4)
                s[i][j] = '3';
            else if (i == 5 && j == 7)
                s[i][j] = '1';
            else if (i == 4 && j == 2)
                s[i][j] = '7';
            else if (i == 4 && j == 6)
                s[i][j] = '3';
            else if (i == 4 && j == 8)
                s[i][j] = '5';
            else if (i == 3 && j == 2)
                s[i][j] = '3';
            else if (i == 3 && j == 3)
                s[i][j] = '4';
            else if (i == 5 && j == 8)
                s[i][j] = '4';

            else if (i == 7 && j == 1)
                s[i][j] = '2';
            else if (i == 7 && j == 4)
                s[i][j] = '8';
            else if (i == 7 && j == 7)
                s[i][j] = '5';
            else if (i == 6 && j == 2)
                s[i][j] = '5';
            else if (i == 6 && j == 3)
                s[i][j] = '9';
            else if (i == 6 && j == 8)
                s[i][j] = '8';
            else if (i == 8 && j == 8)
                s[i][j] = '3';
            else if (i == 8 && j == 7)
                s[i][j] = '2';
            else if (i == 8 && j == 3)
                s[i][j] = '6';
            else if (i == 8 && j == 0)
                s[i][j] = '8';
        }
    }
}


void solveBoard(int x, int y)
{
    if (y == yMax)
    {
        if (x == xMax - 1)
        {
            checkSolve = false;
            return;
        }
        else
            solveBoard(x + 1, yMin);
    }
    else if (s[x][y] == '0')
    {
        for (int check = 1; check <= yMax; check++)
        {
            if (checkBoard(x, y, check + '0') == true)
            {
                s[x][y] = check + '0';
                solveBoard(x, y + 1);
                if (checkSolve == true)
                    s[x][y] = '0';
            }
        }
    }
    else
        solveBoard(x, y + 1);
}

bool checkBoard(int x, int y, unsigned char c_check)
{
    for (int i = 0; i < yMax; i++)
    {
        if (s[x][i] == c_check)
            return false;
    }
    for (int i = 0; i < xMax; i++)
    {
        if (s[i][y] == c_check)
            return false;
    }
    int a = x / 3, b = y / 3;
    for (int i = 3 * a; i < 3 * a + 3; i++)
        for (int j = 3 * b; j < 3 * b + 3; j++)
            if (s[i][j] == c_check)
                return false;
    return true;
}



int windowWidth = 900;
int windowHeight = 650;

void displayf1()
{
    glClearColor(0.5, 0.0, 1.0, 1.0); // Set background color (deep violet)

    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

    // Set text color (white)
    glColor3f(1.0, 1.0, 1.0);

    // Set the stylish font
    void* font = GLUT_BITMAP_TIMES_ROMAN_24;


    // Display project name
    glRasterPos2f(-0.2, 0.7); // Adjust the position as needed
    const char* projectName = "Sudoku Solver";
    for (int i = 0; projectName[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, projectName[i]);
    }


    // Display project description
    glRasterPos2f(-0.9, -0.3); // Adjust the position as needed
    const char* description = "Sudoku is a 9x9 grid puzzle where each row, column, and 3x3 block must be filled with numbers 1-9 without repetition.";
    for (int i = 0; description[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, description[i]);
    }


    // Draw the play button (rectangular button)
    glColor3f(0.0, 0.5, 0.0); // Green color for the button
    glBegin(GL_POLYGON);
    glVertex2f(-0.1, -0.6); // Bottom-left corner
    glVertex2f(0.1, -0.6); // Bottom-right corner
    glVertex2f(0.1, -0.8); // Top-right corner
    glVertex2f(-0.1, -0.8); // Top-left corner
    glEnd();

    // Set text color (white) for the button text
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-0.05, -0.7); // Adjust the position as needed
    const char* buttonText = "Play";
    for (int i = 0; buttonText[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, buttonText[i]);
    }

    glFlush(); // Flush the OpenGL pipeline
}

// Function to handle mouse clicks
void mouse1(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Convert mouse coordinates to OpenGL coordinates
        float normX = (float)x / windowWidth * 2.0 - 1.0;
        float normY = (1.0 - (float)y / windowHeight) * 2.0 - 1.0;

        // Check if the cursor is within the play button's area
        if (normX >= -0.1 && normX <= 0.1 && normY >= -0.8 && normY <= -0.6)
        {
            // Add your game start logic here (e.g., open a new window for the game or start the game loop)
            // For now, let's just show an alert
            glutInitDisplayMode(GLUT_SINGLE);
            glutInitWindowPosition(200, 100);
            glutInitWindowSize(800, 600);
            glutCreateWindow("Solve Sudoku");
            Initialize();
            glutDisplayFunc(display);
            glutMouseFunc(mouseClick);
            glutKeyboardFunc(keyboard);
            glutMainLoop();

        }
    }
}
// Function to handle window resizing
void reshape1(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Set the orthographic view
}

// Function to display the content on the screen
void displayf()
{
    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

    // Set text color (white)
    glColor3f(1.0, 1.0, 1.0);

    // Set the stylish font
    void* font = GLUT_BITMAP_TIMES_ROMAN_24;

    // Display "Welcome"
    glRasterPos2f(-0.2, 0.7); // Adjust the position as needed
    const char* welcomeText = "Welcome!!";
    for (int i = 0; welcomeText[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, welcomeText[i]);
    }

    // Display project name
    glRasterPos2f(-0.5, 0.3); // Adjust the position as needed
    const char* projectName = "Mind Games Odyssey";
    for (int i = 0; projectName[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, projectName[i]);
    }

    // Display project member name
    glRasterPos2f(-0.4, 0.1); // Adjust the position as needed
    const char* projectMember = "Tanvir <> Raihan <> Tias";
    for (int i = 0; projectMember[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, projectMember[i]);
    }

    // Display project description
    glRasterPos2f(-0.9, -0.3); // Adjust the position as needed
    const char* description = "Welcome to the Mind Games Odyssey Project. This Project Showcases 2 Exciting Games: Archery Game & Sudoku Solver";
    for (int i = 0; description[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, description[i]);
    }


    // Draw the play button (rectangular button)
    glColor3f(0.0, 0.5, 0.0); // Green color for the button
    glBegin(GL_POLYGON);
    glVertex2f(-0.1, -0.6); // Bottom-left corner
    glVertex2f(0.1, -0.6); // Bottom-right corner
    glVertex2f(0.1, -0.8); // Top-right corner
    glVertex2f(-0.1, -0.8); // Top-left corner
    glEnd();

    // Set text color (white) for the button text
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-0.05, -0.7); // Adjust the position as needed
    const char* buttonText = "Start ";
    for (int i = 0; buttonText[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, buttonText[i]);
    }

    glFlush(); // Flush the OpenGL pipeline
}

// Function to handle mouse clicks
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Convert mouse coordinates to OpenGL coordinates
        float normX = (float)x / windowWidth * 2.0 - 1.0;
        float normY = (1.0 - (float)y / windowHeight) * 2.0 - 1.0;

        // Check if the cursor is within the play button's area
        if (normX >= -0.1 && normX <= 0.1 && normY >= -0.8 && normY <= -0.6)
        {
            // Add your game start logic here (e.g., open a new window for the game or start the game loop)
            // For now, let's just show an alert
            glutInitWindowSize(windowWidth, windowHeight); // Set the window size (800x600)
            glutCreateWindow("Game Preview"); // Create a window with a title

            glutDisplayFunc(displayf1); // Set the display function
            glutReshapeFunc(reshape1); // Set the reshape function
            glutMouseFunc(mouse1); // Set the mouse function for mouse clicks

            glutMainLoop(); // Start t

        }
    }
}
// Function to handle window resizing
void reshape(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Set the orthographic view
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv); // Initialize GLUT
    glutInitWindowSize(windowWidth, windowHeight); // Set the window size (800x600)
    glutCreateWindow("Game Preview"); // Create a window with a title

    glutDisplayFunc(displayf); // Set the display function
    glutReshapeFunc(reshape); // Set the reshape function
    glutMouseFunc(mouse); // Set the mouse function for mouse clicks

    glutMainLoop(); // Start the GLUT main loop

    return 0;
}
