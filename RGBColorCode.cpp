/*  Copyright (C) <2014>  <Joon Kim  a.k.a  rudfol>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you are interesting contact me: 33jhkim@gmail.com        :')
*/


#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <iostream>

void InitMemoryGL();
void InitWindowGL(int xsize, int ysize);

void DrawPixels_movePoint( int pixelColumns, int pixelRows,
     int rgbRed, int rgbGreen,     int rgbBlue,    int rgbAlpha);

void DrawRectangle( double ax, double ay,
                    double bx, double by);

void DrawLineH( double x, double y, double width);
void DrawLineV( double x, double y, double height);


// forgive me for the Global Variables :'(
constexpr double pixelLength{1.7};
constexpr double objectGap{20};


int main(int argc, char *argv[])
{
    /*********************
     *  Initialization.  *
     *********************/

    SDL_Init(SDL_INIT_VIDEO);

    InitMemoryGL();
    SDL_WM_SetCaption( "RGB Color Code | rudfol", NULL);

    constexpr int wWidth{800}, wHeight{600};
    InitWindowGL( wWidth, wHeight);




    /****************
     *  Variables.  *
     ****************/

    constexpr int rgbMax{255};

    constexpr double pixelScreenSize{pixelLength * rgbMax};

    constexpr double pixelScreenLeft{objectGap};
    constexpr double pixelScreenRight{objectGap + pixelScreenSize};
    constexpr double pixelScreenTop{objectGap};
    constexpr double pixelScreenBottom{objectGap + pixelScreenSize};


    double xLineH = objectGap;
    double yLineH = objectGap;

    double xLineV = objectGap;
    double yLineV = objectGap;

    double xCross = xLineV + ( pixelScreenSize /2);
    double yCross = yLineH + ( pixelScreenSize /2);

    bool            wKey{false};            //    ^

    bool aKey{false},       dKey{false};    // <     >

    bool            sKey{false};            //    ▾


    constexpr double crossVelocityValue{pixelLength /2};

    double xCrossVelocity{crossVelocityValue};
    double yCrossVelocity{crossVelocityValue};


    bool accelKey{false};
    constexpr double velocityAccel{pixelLength};


    bool leftKey{false}, rightKey{false};
    int rgbBlueVelocity{1};

    int rgbRed  {0};
    int rgbGreen{0};
    int rgbBlue {0};
    constexpr int rgbAlpha{rgbMax};




    /****************
     *  Game loop.  *
     ****************/

    SDL_Event event;
    bool isRunning{true};

    std::cout<< "Game loop starting...\n";

    while(isRunning) {

        /*********************
         *  Event handling.  *
         *********************/

        while( SDL_PollEvent(&event) ) {

            // quit method
            if (event.type == SDL_QUIT)

                isRunning = false;

            else if (event.type == SDL_KEYUP &&
                 event.key.keysym.sym == SDLK_ESCAPE)

                isRunning = false;

            // control-w to close window does not work
            /*
            else if (event.type == SDL_KEYUP &&
                     event.key.keysym.sym == SDLK_LCTRL) {

                 if (event.key.keysym.sym == SDLK_w)
                    isRunning = false;
            }
            */



            if(event.type == SDL_KEYDOWN) {

                if (event.key.keysym.sym == SDLK_a)
                    aKey = true;

                else if (event.key.keysym.sym == SDLK_d)
                    dKey = true;


                if (event.key.keysym.sym == SDLK_w)
                    wKey = true;

                else if (event.key.keysym.sym == SDLK_s)
                    sKey = true;


                if (event.key.keysym.sym == SDLK_LSHIFT ||
                    event.key.keysym.sym == SDLK_RSHIFT )
                    accelKey = true;


                if (event.key.keysym.sym == SDLK_LEFT)
                    leftKey = true;

                else if (event.key.keysym.sym == SDLK_RIGHT)
                    rightKey = true;

            } // key down


            else if (event.type == SDL_KEYUP) {

                if (event.key.keysym.sym == SDLK_a)
                    aKey = false;

                else if (event.key.keysym.sym == SDLK_d)
                    dKey = false;


                if (event.key.keysym.sym == SDLK_w)
                    wKey = false;

                else if (event.key.keysym.sym == SDLK_s)
                    sKey = false;


                if (event.key.keysym.sym == SDLK_LSHIFT ||
                    event.key.keysym.sym == SDLK_RSHIFT )
                    accelKey = false;


                if (event.key.keysym.sym == SDLK_LEFT)
                    leftKey = false;

                else if (event.key.keysym.sym == SDLK_RIGHT)
                    rightKey = false;

            } // key up

        } // event loop


        /*********************
         *  Logic handling.  *
         *********************/

        if (aKey)
            xCross -= xCrossVelocity;

        else if (dKey)
            xCross += xCrossVelocity;

        if (wKey)
            yCross -= yCrossVelocity;

        else if (sKey)
            yCross += yCrossVelocity;


        if (leftKey)
            rgbBlue -= rgbBlueVelocity;

        else if (rightKey)
            rgbBlue += rgbBlueVelocity;


        if (accelKey) {
            xCrossVelocity += velocityAccel;
            yCrossVelocity += velocityAccel;

            rgbBlueVelocity += velocityAccel;

        } else if (!accelKey) {
            xCrossVelocity = crossVelocityValue;
            yCrossVelocity = crossVelocityValue;

            rgbBlueVelocity = 1;
        }




        if (xCross < pixelScreenLeft)
            xCross = pixelScreenLeft + 1;

        else if (xCross > pixelScreenRight)
            xCross = pixelScreenRight - 1;

        if (yCross < pixelScreenTop)
            yCross = pixelScreenTop + 1;

        else if (yCross > pixelScreenBottom)
            yCross = pixelScreenBottom - 1;


        if (rgbBlue < 0)
            rgbBlue = 0;

        else if (rgbBlue > rgbMax)
            rgbBlue = rgbMax;




        /******************
         *  Draw Matrix.  *
         ******************/

        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();
        glOrtho(0, wWidth, wHeight, 0, -1, 1);


        for (int pixelRows = 0; pixelRows < rgbMax; ++pixelRows) {

            for (int pixelColumns = 0;
                    pixelColumns < rgbMax; ++pixelColumns) {

                rgbRed = pixelRows + 1;
                rgbGreen = pixelColumns + 1;

                DrawPixels_movePoint(pixelColumns, pixelRows,
                            rgbRed, rgbGreen, rgbBlue, rgbAlpha);
            }
        }


        // integers truncates towards 0
        int xCrossColor = (xCross + 1) /2;
        int yCrossColor = (yCross + 1) /2;

        glColor4ub(yCrossColor, xCrossColor, rgbBlue, 255);

        DrawLineH(xLineH, yCross, pixelScreenSize);
        DrawLineV(xCross, yLineV, pixelScreenSize);


        DrawRectangle(pixelScreenRight, pixelScreenBottom,
                       wWidth - objectGap, wHeight-objectGap);

        glPopMatrix();


        SDL_GL_SwapBuffers();
        SDL_Delay(10);

    } // game loop


    SDL_Quit();
    return 0;

} // main


void InitMemoryGL()
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,  8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,  8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    std::cout<< "Memory Allocation successful.\n";
}


void InitWindowGL(int xsize, int ysize)
{
    SDL_SetVideoMode(xsize, ysize, 32, SDL_OPENGL);

    glClearColor(1, 1, 1, 1);
    glViewport(0, 0, xsize, ysize);

    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    std::cout<< "OpenGL successfully created a window.\n";
}


void DrawPixels_movePoint(int pixelColumns, int pixelRows,
        int rgbRed, int rgbGreen,    int rgbBlue,    int rgbAlpha)
{
    // move the starting coordinate to the next column
    // on every increment of pixelColumns.
    double x{objectGap};
    x += pixelLength * pixelColumns;

    double y{objectGap};
    y += pixelLength * pixelRows;


    glBegin(GL_QUADS);
        glColor4ub(rgbRed, rgbGreen, rgbBlue, rgbAlpha);

        glVertex2f(x, y);
        glVertex2f(x + pixelLength, y);
        glVertex2f(x + pixelLength, y+ pixelLength);
        glVertex2f(x, y + pixelLength);
    glEnd();
}


void DrawLineH(double x, double y, double width)
{
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
    glEnd();
}


void DrawLineV(double x, double y, double height)
{
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x, y + height);
    glEnd();
}


void DrawRectangle(double ax, double ay,
                    double bx, double by)
{
    glBegin(GL_QUADS);
        glVertex2f(ax, ay);
        glVertex2f(bx, ay);
        glVertex2f(bx, by);
        glVertex2f(ax, by);
    glEnd();
}
