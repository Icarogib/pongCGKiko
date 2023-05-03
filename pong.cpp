#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

/*
    Grupo: João Matheus Falcão de Oliveira
           Arnaldo Antônio Inácio Silva
           ícaro Dutra Gibson Da Silva
*/

// SE FOR WINDOWS USA AQUI EM BAIXO
// Comenta o de cima e usa esse.

/*
#include "stdafx.h"

#include <string>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <sstream>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "GL/freeglut.h"
#pragma comment(lib, "OpenGL32.lib")
*/

bool scored_left = false;
bool scored_right = false;
bool pausado = false;

// window size and update rate (60 fps)
int width = 1000;
int height = 1000;
int interval = 1000 / 60;

// rackets in general
int racket_width = 10;
int racket_height = 100;
int racket_speed = 15;

// left racket position
float racket_left_x = 10.0f;
float racket_left_y = 500;

// right racket position
float racket_right_x = width - racket_width - 10;
float racket_right_y = 500;

float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 8;
int ball_speed = 2;

// score
int score_left = 0;
int score_right = 0;

void enable2D(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

std::string int2str(int x)
{
    // converts int to string
    std::stringstream ss;
    ss << x;
    return ss.str();
}

void drawText(float x, float y, std::string text)
{
    glRasterPos2f(x, y);
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *)text.c_str());
}

void drawRect(float x, float y, float width, float height)
{
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void draw()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // desenha bola
    drawRect(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size, ball_size);

    // desenha raquetes
    drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
    drawRect(racket_right_x, racket_right_y, racket_width, racket_height);

    // desenha score
    if (score_right < 5 && score_left < 5)
    {
        drawText(width / 2 - 10, height - 15,
                 int2str(score_left) + ":" + int2str(score_right));
    }
    else
    {
        drawText(width / 4, height - 15,
                 "Deseja reiniciar ou encerrar? (y/n)");
    }

    if (pausado)
    {
        drawText(width / 4, height - 15,
                 "Jogo Pausado");
    }

    // swap buffers (has to be done at the end)
    glutSwapBuffers();
}

void keyboard(GLubyte key, GLint x, GLint y)
{
    // raquete esquerda
    if (!pausado)
    {
        if (key == 'w')
            racket_left_y += racket_speed;
        if (key == 's')
            racket_left_y -= racket_speed;
    }

    if (key == 13)
    {
        ball_speed = 2;
        scored_right = false;
        scored_left = false;
    }
    if (key == 'y')
    {
        score_left = 0;
        score_right = 0;
        scored_right = false;
        scored_left = false;
        ball_speed = 2;
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = -1.0f;
        ball_dir_y = 0.0f;
        // ball_pos_x += ball_dir_x * ball_speed;
        // ball_pos_y += ball_dir_y * ball_speed;
    }
    if (key == 'n')
    {
        exit(0);
    }

    if (key == ' ')
    {
        pausado = !pausado;
    }
}

void eventoTecladoEspecial(GLint key, GLint x, GLint y)
{

    // raquete direita
    if (!pausado)
    {
        if (key == GLUT_KEY_UP)
            racket_right_y += racket_speed;
        if (key == GLUT_KEY_DOWN)
            racket_right_y -= racket_speed;
    }
}

void vec2_norm(float &x, float &y)
{
    // sets a vectors length to 1 (which means that x + y == 1)
    float length = sqrt((x * x) + (y * y));
    if (length != 0.0f)
    {
        length = 1.0f / length;
        x *= length;
        y *= length;
    }
}

void pausaJogo()
{
    drawText(width / 4, height - 15,
             "Jogo Pausado");
}

void updateBall()
{
    // fly a bit
    if (scored_right && score_right < 5 && score_left < 5)
    {
        ball_pos_x = racket_right_x - 10;
        ball_pos_y = racket_right_y + (racket_height / 2);
        // scored_right = false;
    }
    else if (scored_left && score_right < 5 && score_left < 5)
    {
        ball_pos_x = racket_left_x + 15;
        ball_pos_y = racket_left_y + (racket_height / 2);
    }
    else
    {
        ball_pos_x += ball_dir_x * ball_speed;
        ball_pos_y += ball_dir_y * ball_speed;
    }

    // encostou na raquete da esquerda
    if (ball_pos_x < racket_left_x + racket_width &&
        ball_pos_x > racket_left_x &&
        ball_pos_y < racket_left_y + racket_height &&
        ball_pos_y > racket_left_y)
    {
        // dependendo onde bateu a raquete, mudar a direção
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
        ball_dir_x = fabs(ball_dir_x); // force it to be positive
        ball_dir_y = t;
        if (ball_speed < 12)
            ball_speed++;
    }

    // encostou na raquete da direita
    if (ball_pos_x > racket_right_x &&
        ball_pos_x < racket_right_x + racket_width &&
        ball_pos_y < racket_right_y + racket_height &&
        ball_pos_y > racket_right_y)
    {
        // dependendo onde bateu a raquete, mudar a direção
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
        ball_dir_x = -fabs(ball_dir_x);
        ball_dir_y = t;
        if (ball_speed < 12)
            ball_speed++;
    }

    // parede esquerda
    if (ball_pos_x < 0)
    {
        ++score_right;
        // ball_pos_x = width / 2;
        // ball_pos_y = height / 2;
        ball_dir_x = -fabs(ball_dir_x);
        ball_dir_y = 0;
        scored_right = true;
        ball_speed = 0;
    }

    // parede direita
    if (ball_pos_x > width)
    {
        ++score_left;
        // ball_pos_x = width / 2;
        // ball_pos_y = height / 2;
        ball_dir_x = fabs(ball_dir_x); // force it to be negative
        ball_dir_y = 0;
        scored_left = true;
        ball_speed = 0;
    }

    // parede cima
    if (ball_pos_y > height)
    {
        ball_dir_y = -fabs(ball_dir_y); // force it to be negative
    }

    // parede baixo
    if (ball_pos_y < 0)
    {
        ball_dir_y = fabs(ball_dir_y); // force it to be positive
    }

    // make sure that length of dir stays at 1
    vec2_norm(ball_dir_x, ball_dir_y);
}

void update(int value)
{

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(eventoTecladoEspecial);

    if (!pausado)
    {
        updateBall();
    }

    glutTimerFunc(interval, update, 0);

    glutPostRedisplay();
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Pong");

    glutDisplayFunc(draw);
    glutTimerFunc(interval, update, 0);

    // cena em 2d e cor branca
    enable2D(width, height);
    glColor3f(1.0f, 1.0f, 1.0f);

    glutMainLoop();
    return 0;
}