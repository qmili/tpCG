#include "SOIL/SOIL.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <time.h>

#define LARGURA_DO_MUNDO 600
#define ALTURA_DO_MUNDO 600

struct{
    float x;
    float y;
}cordPlataforma[4][4];

struct{
    float movPlataforma1;
    float movPlataforma2;
    float movPlataforma3;
    float movPlataforma4;
    float movPlataforma5;
    float movPlataforma6;
}movimento;

struct{
    float movx;
    float movy;
    float x1;
    float y1;
    float x2;
    float y2;
    float rotate;
    int libera;
}torpedo;

struct{
    float movx;
    float movy;
    float x1;
    float y1;
    float x2;
    float y2;
    float xper; //centro do personagem
    float yper; //centro do personagem
    int libera;
}projetil;

struct{
    float movx;
    float movy;
    float x1;
    float y1;
    float x2;
    float y2;
    int ataque;
    int libera;
    float xper;
    float yper;
}abelha;

struct{
    float movx;
    float movy;
    float x1;
    float x2;
    float y1;
    float y2;
}ataqueAbelha;



int points = 0;

int primo(int num){
    for(int i=2; i<num/2; i++){
        if((num%i)==0){
            return 0;
        }
    }
    return 1;
}

void pontos(int periodo){
    points++;
    glutTimerFunc(periodo, pontos, periodo);
}

void fase2(){//mudar de fase, timerFunc ta setada para durar 3 min glutTimerFunc(180000, fase2, 0);. Repetir para trocar para fase 3.
    exit(1);
}

void analisaTorpedo(int periodo){
    if((torpedo.rotate == 0) && ((torpedo.movx+5) < 570)){
        torpedo.movx +=4;
    }
    if((torpedo.rotate == 0) && ((torpedo.movx+5) >= 570)){
        torpedo.rotate = 90;
    }
    if((torpedo.rotate == 90) && ((torpedo.movx+5) >= 570) && (torpedo.movy<70)){
        torpedo.movy +=4;
    }
    if((torpedo.rotate == 90) && ((torpedo.movx+5) >= 570) && (torpedo.movy>=70) && (torpedo.movy<140)){
        torpedo.rotate = 180;
    }
    if((torpedo.rotate == 180) && ((torpedo.movx+5) > 30)){
        torpedo.movx -=4;
    }
    if((torpedo.rotate == 180) && ((torpedo.movx+5) <= 30)){
        torpedo.rotate = 90;
    }
    if((torpedo.rotate == 90) && ((torpedo.movx+5) <= 30) && (torpedo.movy>=70) && (torpedo.movy<140)){
        torpedo.movy +=4;
    }
    if((torpedo.rotate == 90) && ((torpedo.movx+5) <= 30) && (torpedo.movy>=140) && (torpedo.movy<210)){
        torpedo.rotate = 0;
    }

    if((torpedo.rotate == 90) && ((torpedo.movx+5) >= 570) && (torpedo.movy>=140) && (torpedo.movy<210)){
        torpedo.movy +=4;
    }
    if((torpedo.rotate == 90) && ((torpedo.movx+5) >= 570) && (torpedo.movy>=210) && (torpedo.movy<280)){
        torpedo.rotate = 180;
    }

    if((torpedo.rotate == 90) && ((torpedo.movx+5) <= 30) && (torpedo.movy>=210) && (torpedo.movy<280)){
        torpedo.movy +=4;
    }

    if((torpedo.rotate == 90) && ((torpedo.movx+5) <= 30) && (torpedo.movy>=280) && (torpedo.movy<350)){
        torpedo.rotate = 0;
    }

    if((torpedo.rotate == 90) && ((torpedo.movx+5) >= 570) && (torpedo.movy>=280) && (torpedo.movy<350)){
        torpedo.movy +=4;
    }

    if((torpedo.rotate == 90) && ((torpedo.movx+5) >= 570) && (torpedo.movy>=350) && (torpedo.movy<420)){
        torpedo.rotate = 180;
    }

    if((torpedo.rotate == 90) && ((torpedo.movx+5) <= 30) && (torpedo.movy>=350) && (torpedo.movy<420)){
        torpedo.movy +=4;
    }
    //  
    if((torpedo.rotate == 90) && ((torpedo.movx+5) <= 30) && (torpedo.movy>=420) && (torpedo.movy<490)){
        torpedo.rotate = 0;
    }
    if((torpedo.rotate == 90) && ((torpedo.movx+5) >= 570) && (torpedo.movy>=420) && (torpedo.movy<490)){
        torpedo.movy +=4;
    }
    if((torpedo.rotate == 90) && ((torpedo.movx+5) >= 570) && (torpedo.movy>=490) && (torpedo.movy<560)){
        torpedo.rotate = 180;
    }
    //
    if((torpedo.rotate == 90) && ((torpedo.movx+5) <= 30) && (torpedo.movy>=490) && (torpedo.movy<560)){
        torpedo.movy +=4;
    }
    if((torpedo.rotate == 90) && ((torpedo.movx+5) <= 30) && (torpedo.movy>=560) && (torpedo.movy<590)){
        torpedo.rotate = 0;
    }
    //
    if((torpedo.rotate == 90) && ((torpedo.movx+5) >= 570) && (torpedo.movy>=560) && (torpedo.movy<620)){
        torpedo.movy +=4;
    }
    if(torpedo.movy>=620){
        torpedo.libera = 0;
    }

    if(torpedo.libera == 1){
     glutTimerFunc(periodo, analisaTorpedo, periodo);
    }
}

void analisaProjetil(int periodo){
    projetil.movx += (projetil.xper - projetil.movx)/100.0f;
    projetil.movy += (projetil.yper - projetil.movy)/100.0f;

    if((projetil.movx >= projetil.xper-15) && (projetil.movx <= projetil.xper+15)){
        projetil.libera =0;
    }

    if(projetil.libera == 1){
    glutTimerFunc(periodo, analisaProjetil, periodo);
    }
}

void analisaAtaqueAbelha(int periodo){
    if(ataqueAbelha.movy>-10){
        ataqueAbelha.movy-=5;
    }else{
        abelha.ataque = 0;
    }
    if(abelha.ataque == 1){
    glutTimerFunc(periodo, analisaAtaqueAbelha, periodo);
    }
}

void analisaAbelha(int periodo){
    if(abelha.movx > -20){
        abelha.movx-=0.5;
    }else{
        abelha.libera = 0;
    }

    if(((abelha.movx >= abelha.xper-5) && (abelha.movx <=abelha.xper+5)) && (abelha.ataque == 0)){ // colocar no lugar de abelha.xper a posição x do personagem
        ataqueAbelha.movx = abelha.xper;
        ataqueAbelha.movy = abelha.movy;
        abelha.ataque = 1;
        glutTimerFunc(0, analisaAtaqueAbelha,16);
    }
    if(abelha.libera == 1){
    glutTimerFunc(periodo, analisaAbelha, periodo);
    }
}

void enemyRandom(int periodo){ //randomizar o surgimento de inimigos, ideia com time.h usando números primos e resto de divisão
    int controle = rand()%201;
    if((primo(controle) == 1) && abelha.libera == 0){ //acrescentar a variavel de mudança de fase no if. Esse inimigo so aparece na fase 2.
        abelha.movx = 620; // colocoar no enemyRandom
        abelha.movy = 560; // colocoar no enemyRandom
        /*colocar para setar a posição do boneco aqui
        EX: abelha.xper = personagem.x*/
        abelha.libera = 1;
        glutTimerFunc(0, analisaAbelha, 16);
        
    }

    if(((controle>=142) && (controle<=200)) && projetil.libera == 0){
        if((controle%2) == 0){
            projetil.movx = (600+(rand()%21));
            projetil.movy = rand()%601;
        }
        if((controle%2)!=0){
            projetil.movx = (-(rand()%21));
            projetil.movy = rand()%601;
        }
        projetil.xper = 300; //randomizar na enemyRandom
        projetil.yper = 300; //randomizar na enemyRandom
        projetil.libera = 1;
        glutTimerFunc(0, analisaProjetil, 16);
    }

    if(((controle%2) == 0) && torpedo.libera == 0){
        torpedo.movx = 20;
        torpedo.movy = 20;
        torpedo.rotate = 0;
        torpedo.libera = 1;
        glutTimerFunc(0, analisaTorpedo, 16);
    }

    glutTimerFunc(periodo, enemyRandom, periodo);
}

void movPlat1(int periodo){
    if(movimento.movPlataforma1>=580){
        movimento.movPlataforma1 = 0;
    }
    if(movimento.movPlataforma1<580){
        movimento.movPlataforma1+=0.8;
    }

    if(movimento.movPlataforma2>=580){
        movimento.movPlataforma2 = 0;
    }
    if(movimento.movPlataforma2<580){
        movimento.movPlataforma2+=0.8;
    }

    if(movimento.movPlataforma3>=580){
        movimento.movPlataforma3 = 0;
    }
    if(movimento.movPlataforma3<580){
        movimento.movPlataforma3+=0.8;
    }

    if(movimento.movPlataforma4>=580){
        movimento.movPlataforma4 = 0;
    }
    if(movimento.movPlataforma4<580){
        movimento.movPlataforma4+=0.8;
    }

    if(movimento.movPlataforma5>=580){
        movimento.movPlataforma5 = 0;
    }
    if(movimento.movPlataforma5<580){
        movimento.movPlataforma5+=0.8;
    }

    if(movimento.movPlataforma6>=580){
        movimento.movPlataforma6 = 0;
    }
    if(movimento.movPlataforma6<580){
        movimento.movPlataforma6+=0.8;
    }

    glutPostRedisplay();

    glutTimerFunc(periodo, movPlat1, periodo);
}


void plataforma1(){
    glColor3f(1,0,0);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[0][0].x, cordPlataforma[0][0].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[0][1].x, cordPlataforma[0][1].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[0][2].x, cordPlataforma[0][2].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[0][3].x, cordPlataforma[0][3].y-movimento.movPlataforma1, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[1][0].x, cordPlataforma[1][0].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[1][1].x, cordPlataforma[1][1].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[1][2].x, cordPlataforma[1][2].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[1][3].x, cordPlataforma[1][3].y-movimento.movPlataforma1, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[2][0].x, cordPlataforma[2][0].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[2][1].x, cordPlataforma[2][1].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[2][2].x, cordPlataforma[2][2].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[2][3].x, cordPlataforma[2][3].y-movimento.movPlataforma1, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[3][0].x, cordPlataforma[3][0].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[3][1].x, cordPlataforma[3][1].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[3][2].x, cordPlataforma[3][2].y-movimento.movPlataforma1, 0);
        glVertex3f(cordPlataforma[3][3].x, cordPlataforma[3][3].y-movimento.movPlataforma1, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[0][0].x, cordPlataforma[0][0].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[0][1].x, cordPlataforma[0][1].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[0][2].x, cordPlataforma[0][2].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[0][3].x, cordPlataforma[0][3].y-movimento.movPlataforma2, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[1][0].x, cordPlataforma[1][0].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[1][1].x, cordPlataforma[1][1].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[1][2].x, cordPlataforma[1][2].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[1][3].x, cordPlataforma[1][3].y-movimento.movPlataforma2, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[2][0].x, cordPlataforma[2][0].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[2][1].x, cordPlataforma[2][1].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[2][2].x, cordPlataforma[2][2].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[2][3].x, cordPlataforma[2][3].y-movimento.movPlataforma2, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[3][0].x, cordPlataforma[3][0].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[3][1].x, cordPlataforma[3][1].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[3][2].x, cordPlataforma[3][2].y-movimento.movPlataforma2, 0);
        glVertex3f(cordPlataforma[3][3].x, cordPlataforma[3][3].y-movimento.movPlataforma2, 0);
    glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[0][0].x, cordPlataforma[0][0].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[0][1].x, cordPlataforma[0][1].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[0][2].x, cordPlataforma[0][2].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[0][3].x, cordPlataforma[0][3].y-movimento.movPlataforma3, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[1][0].x, cordPlataforma[1][0].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[1][1].x, cordPlataforma[1][1].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[1][2].x, cordPlataforma[1][2].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[1][3].x, cordPlataforma[1][3].y-movimento.movPlataforma3, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[2][0].x, cordPlataforma[2][0].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[2][1].x, cordPlataforma[2][1].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[2][2].x, cordPlataforma[2][2].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[2][3].x, cordPlataforma[2][3].y-movimento.movPlataforma3, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[3][0].x, cordPlataforma[3][0].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[3][1].x, cordPlataforma[3][1].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[3][2].x, cordPlataforma[3][2].y-movimento.movPlataforma3, 0);
        glVertex3f(cordPlataforma[3][3].x, cordPlataforma[3][3].y-movimento.movPlataforma3, 0);
    glEnd();  

        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[0][0].x, cordPlataforma[0][0].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[0][1].x, cordPlataforma[0][1].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[0][2].x, cordPlataforma[0][2].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[0][3].x, cordPlataforma[0][3].y-movimento.movPlataforma4, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[1][0].x, cordPlataforma[1][0].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[1][1].x, cordPlataforma[1][1].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[1][2].x, cordPlataforma[1][2].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[1][3].x, cordPlataforma[1][3].y-movimento.movPlataforma4, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[2][0].x, cordPlataforma[2][0].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[2][1].x, cordPlataforma[2][1].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[2][2].x, cordPlataforma[2][2].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[2][3].x, cordPlataforma[2][3].y-movimento.movPlataforma4, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[3][0].x, cordPlataforma[3][0].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[3][1].x, cordPlataforma[3][1].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[3][2].x, cordPlataforma[3][2].y-movimento.movPlataforma4, 0);
        glVertex3f(cordPlataforma[3][3].x, cordPlataforma[3][3].y-movimento.movPlataforma4, 0);
    glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[0][0].x, cordPlataforma[0][0].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[0][1].x, cordPlataforma[0][1].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[0][2].x, cordPlataforma[0][2].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[0][3].x, cordPlataforma[0][3].y-movimento.movPlataforma5, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[1][0].x, cordPlataforma[1][0].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[1][1].x, cordPlataforma[1][1].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[1][2].x, cordPlataforma[1][2].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[1][3].x, cordPlataforma[1][3].y-movimento.movPlataforma5, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[2][0].x, cordPlataforma[2][0].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[2][1].x, cordPlataforma[2][1].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[2][2].x, cordPlataforma[2][2].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[2][3].x, cordPlataforma[2][3].y-movimento.movPlataforma5, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[3][0].x, cordPlataforma[3][0].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[3][1].x, cordPlataforma[3][1].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[3][2].x, cordPlataforma[3][2].y-movimento.movPlataforma5, 0);
        glVertex3f(cordPlataforma[3][3].x, cordPlataforma[3][3].y-movimento.movPlataforma5, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[0][0].x, cordPlataforma[0][0].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[0][1].x, cordPlataforma[0][1].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[0][2].x, cordPlataforma[0][2].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[0][3].x, cordPlataforma[0][3].y-movimento.movPlataforma6, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[1][0].x, cordPlataforma[1][0].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[1][1].x, cordPlataforma[1][1].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[1][2].x, cordPlataforma[1][2].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[1][3].x, cordPlataforma[1][3].y-movimento.movPlataforma6, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[2][0].x, cordPlataforma[2][0].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[2][1].x, cordPlataforma[2][1].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[2][2].x, cordPlataforma[2][2].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[2][3].x, cordPlataforma[2][3].y-movimento.movPlataforma6, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cordPlataforma[3][0].x, cordPlataforma[3][0].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[3][1].x, cordPlataforma[3][1].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[3][2].x, cordPlataforma[3][2].y-movimento.movPlataforma6, 0);
        glVertex3f(cordPlataforma[3][3].x, cordPlataforma[3][3].y-movimento.movPlataforma6, 0);
    glEnd();

}

void desenhaLava(){
    glColor3f(1, 0.64, 0);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 1);
        glVertex3f(600, 0, 1);
        glVertex3f(600, 40, 1);
        glVertex3f(0, 40, 1);
    glEnd();
}


void desenhaTorpedo(){
    glPushMatrix();
        glTranslatef(torpedo.movx, torpedo.movy, 0);
        glRotatef(torpedo.rotate,0,0,1);
        glColor3f(0,0,1);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(torpedo.x1, torpedo.y1, 0);
            glVertex3f(torpedo.x2, torpedo.y1, 0);
            glVertex3f(torpedo.x2, torpedo.y2, 0);
            glVertex3f(torpedo.x1, torpedo.y2, 0);
        glEnd();
    glPopMatrix();
}

void desenhaProjetil(){
    glPushMatrix();
        glTranslatef(projetil.movx, projetil.movy, 0);
        glColor3f(0,1,0);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(projetil.x1, projetil.y1, 0);
            glVertex3f(projetil.x2, projetil.y1, 0);
            glVertex3f(projetil.x2, projetil.y2, 0);
            glVertex3f(projetil.x1, projetil.y2, 0);
        glEnd();
    glPopMatrix();
}

void desenhaAbelha(){
    glPushMatrix();
        glTranslatef(abelha.movx, abelha.movy, 0);
        glColor3f(0,1,1);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(abelha.x1, abelha.y1, 0);
            glVertex3f(abelha.x2, abelha.y1, 0);
            glVertex3f(abelha.x2, abelha.y2, 0);
            glVertex3f(abelha.x1, abelha.y2, 0);
        glEnd();
    glPopMatrix();
}

void desenhaAtaqueAbelha(){
    glPushMatrix();
        glTranslatef(ataqueAbelha.movx, ataqueAbelha.movy, 0);
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(ataqueAbelha.x1, ataqueAbelha.y1, 0);
            glVertex3f(ataqueAbelha.x2, ataqueAbelha.y1, 0);
            glVertex3f(ataqueAbelha.x2, ataqueAbelha.y2, 0);
            glVertex3f(ataqueAbelha.x1, ataqueAbelha.y2, 0);
        glEnd();
    glPopMatrix();
}
// Rotina de desenho
void desenhaMinhaCena(){
    glClear(GL_COLOR_BUFFER_BIT);
    plataforma1();
    
    if(torpedo.libera == 1){
        desenhaTorpedo();
    }
    if(projetil.libera == 1){
        desenhaProjetil();
    }
    if(abelha.libera == 1){//acrescentrar no if a variavel de mudança de fase. Esse inimigo pertense a fase 2.
        desenhaAbelha();
        if(abelha.ataque == 1){
            desenhaAtaqueAbelha();
        }
    }

    desenhaLava();

    glutSwapBuffers();
}

void setup() {
    glClearColor(1, 1, 1, 0); // branco
    cordPlataforma[0][0].x=30;
    cordPlataforma[0][0].y=570;
    cordPlataforma[0][1].x=110;
    cordPlataforma[0][1].y=570;
    cordPlataforma[0][2].x=110;
    cordPlataforma[0][2].y=580;
    cordPlataforma[0][3].x=30;
    cordPlataforma[0][3].y=580;

    cordPlataforma[1][0].x=180;
    cordPlataforma[1][0].y=570;
    cordPlataforma[1][1].x=260;
    cordPlataforma[1][1].y=570;
    cordPlataforma[1][2].x=260;
    cordPlataforma[1][2].y=580;
    cordPlataforma[1][3].x=180;
    cordPlataforma[1][3].y=580;

    cordPlataforma[2][0].x=330;
    cordPlataforma[2][0].y=570;
    cordPlataforma[2][1].x=410;
    cordPlataforma[2][1].y=570;
    cordPlataforma[2][2].x=410;
    cordPlataforma[2][2].y=580;
    cordPlataforma[2][3].x=330;
    cordPlataforma[2][3].y=580;

    cordPlataforma[3][0].x=480;
    cordPlataforma[3][0].y=570;
    cordPlataforma[3][1].x=560;
    cordPlataforma[3][1].y=570;
    cordPlataforma[3][2].x=560;
    cordPlataforma[3][2].y=580;
    cordPlataforma[3][3].x=480;
    cordPlataforma[3][3].y=580;

    movimento.movPlataforma1 = 0;
    movimento.movPlataforma2 = 100;
    movimento.movPlataforma3 = 200;
    movimento.movPlataforma4 = 300;
    movimento.movPlataforma5 = 400;
    movimento.movPlataforma6 = 500;

    torpedo.x1 = -30;
    torpedo.x2 = 30;
    torpedo.y1 = -15;
    torpedo.y2 = 15;
    torpedo.libera =0;

    
    projetil.x1 = -10;
    projetil.x2 = 10;
    projetil.y1 = -10;
    projetil.y2 = 10;
    projetil.libera =0;

    abelha.x1 = -20;
    abelha.x2 = 20;
    abelha.y1 = -15;
    abelha.y2 = 15;
    
    abelha.libera = 0;
    abelha.xper = 300;
    abelha.ataque = 0;

    ataqueAbelha.x1 = -10;
    ataqueAbelha.x2 = 10;
    ataqueAbelha.y1 = -10;
    ataqueAbelha.y2 = 10;
}

void redimensionada(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARGURA_DO_MUNDO, 0, ALTURA_DO_MUNDO, -1, 1);

    float razaoAspectoJanela = ((float)width)/height;
    float razaoAspectoMundo = ((float) LARGURA_DO_MUNDO)/ ALTURA_DO_MUNDO;
    if (razaoAspectoJanela < razaoAspectoMundo) {
        float hViewport = width / razaoAspectoMundo;
        float yViewport = (height - hViewport)/2;
        glViewport(0, yViewport, width, hViewport);
    }
    else if (razaoAspectoJanela > razaoAspectoMundo) {
        float wViewport = ((float)height) * razaoAspectoMundo;
        float xViewport = (width - wViewport)/2;
        glViewport(xViewport, 0, wViewport, height);
    } else {
        glViewport(0, 0, width, height);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void teclaPressionada(unsigned char key, int x, int y)
{
    if (key == 27) {
      exit(0);
    }
}

// Função principal
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(700,200);

    glutCreateWindow("Fase 1");

    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);
    glutTimerFunc(0, movPlat1, 16); // idem ao debaixo
    glutTimerFunc(1000, enemyRandom, 1000); // iniciar assim que clicar no start e entrar na fase 1
    glutTimerFunc(180000, fase2, 0); // idem ao de cima
    glutTimerFunc(0, pontos, 1000); // idem também

    setup();

    glutMainLoop();
    return 0;
}