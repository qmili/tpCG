#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include "SOIL/SOIL.h"


#define LARGURA_MUNDO 600
#define ALTURA_MUNDO 600
int listaMenu;
int listaInstrucao;

struct
{
    int menu,gameOver,instrucoes;
}cena;
struct
{
    int emCimaNovoJogo,emCimaInstrucao;
    int cliqueNovoJogo,cliqueInstrucao;
}acoesMenu;

GLuint idTexturaNovoJogoMenu;

GLuint carregaTextura(const char*arquivo){
    GLuint idTextura = SOIL_load_OGL_texture(
                                            arquivo, 
                                            SOIL_LOAD_AUTO,
                                            SOIL_CREATE_NEW_ID,
                                            SOIL_FLAG_INVERT_Y
                                            );
    if(idTextura == 0)
        printf("Erro do SOIL: '%s'\n",SOIL_last_result());

    return idTextura;
}

void inicializa(){
    glClearColor(1.0,1.0,1.0,1.0);
    cena.menu=1;
    cena.gameOver=0;
    cena.instrucoes=0;

    acoesMenu.emCimaNovoJogo=0;
    acoesMenu.emCimaInstrucao=0;
    acoesMenu.cliqueNovoJogo=0;
    acoesMenu.cliqueInstrucao=0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaNovoJogoMenu = carregaTextura("novoJogo.png");
}
void alteraMenu(){

    glColor3f(1,1,1);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0);
        glVertex3f(600, 0, 0);
        glVertex3f(600, 600, 0);
        glVertex3f(0, 600, 0);
        glEnd();


        glColor3f(1.0,1.0,1.0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaNovoJogoMenu);
    //glColor3f(1,0,0);
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0,0);
        glVertex3f(100, 100, 0);
        glTexCoord2f(0,1);
        glVertex3f(100, 250, 0);
        glTexCoord2f(1,1);
        glVertex3f(500, 250, 0);
        glTexCoord2f(1,0);
        glVertex3f(500, 100, 0);
        glEnd();
    glDisable(GL_TEXTURE_2D);  
    
    glColor3f(0,0,1);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(100, 350, 0);
        glVertex3f(100, 500, 0);
        glVertex3f(500, 500, 0);
        glVertex3f(500, 350, 0);
        glEnd();
    if(acoesMenu.emCimaInstrucao==1){
         glColor3f(1,1,0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(100, 100, 0);
        glVertex3f(100, 250, 0);
        glVertex3f(500, 250, 0);
        glVertex3f(500, 100, 0);
        glEnd();
    }
    if(acoesMenu.emCimaNovoJogo==1){
         glColor3f(1,1,0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(100, 350, 0);
        glVertex3f(100, 500, 0);
        glVertex3f(500, 500, 0);
        glVertex3f(500, 350, 0);
        glEnd();
    }
    
    glutPostRedisplay();
}
void instrucao(){
    
    glColor3f(1,0,0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 1);
        glVertex3f(600, 0, 1);
        glVertex3f(600, 600, 1);
        glVertex3f(0, 600, 1);
        glEnd();
    
}

void desenhaMinhaCena(){
   
    glClearColor(0,0,0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    if(cena.menu==1){
        alteraMenu();
    }
    if(cena.instrucoes==1){
        instrucao();
        
    }
    glutSwapBuffers();
}
void redimensionada(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARGURA_MUNDO, 0, ALTURA_MUNDO, -1, 1);

    float razaoAspectoJanela = ((float)width)/height;
    float razaoAspectoMundo = ((float) LARGURA_MUNDO)/ ALTURA_MUNDO;
    // se a janela está menos larga do que o mundo (16:9)...
    if (razaoAspectoJanela < razaoAspectoMundo) {
        // vamos colocar barras verticais (acima e abaixo)
        float hViewport = width / razaoAspectoMundo;
        float yViewport = (height - hViewport)/2;
        glViewport(0, yViewport, width, hViewport);
    }
    // se a janela está mais larga (achatada) do que o mundo (16:9)...
    else if (razaoAspectoJanela > razaoAspectoMundo) {
        // vamos colocar barras horizontais (esquerda e direita)
        float wViewport = ((float)height) * razaoAspectoMundo;
        float xViewport = (width - wViewport)/2;
        glViewport(xViewport, 0, wViewport, height);
    } else {
        glViewport(0, 0, width, height);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MouseAndandoNaoPressionado (int x, int y)
{     // início da conversão de coordenadas da tela para do mundo
    // retirado de https://community.khronos.org/t/converting-
    //window-coordinates-to-world-coordinates/16029/8

    GLint viewport[4]; //var to hold the viewport info
    GLdouble modelview[16]; //var to hold the modelview info
    GLdouble projection[16]; //var to hold the projection matrix info
    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
    glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
    glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    winZ = 0;
    
    //get the world coordinates from the screen coordinates
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);

    // fim da conversão de coordenadas da tela para do mundo

    
    if(worldX>100&&worldX<500){
        if(worldY>100&&worldY<250){
            acoesMenu.emCimaInstrucao=1;
        }
        else if(worldY>350&&worldY<500){
            acoesMenu.emCimaNovoJogo=1;
        }
        else{
            acoesMenu.emCimaInstrucao=0;
            acoesMenu.emCimaNovoJogo=0;
        }
    }
    else{
        acoesMenu.emCimaInstrucao=0;
        acoesMenu.emCimaNovoJogo=0; 
    }
    glutPostRedisplay();
    
}
void mouse(int button, int state, int x, int y){
    /*Conversão das coordenadas da tela para as coordenadas do mundo
    A solução abaixo foi encontrada em: https://community.khronos.org/t/converting-window-coordinates-to-world-coordinates/16029/8
    Foi necessário para que o quadrado e o círculo continuassem centrados onde o mouse foi clicado
    */
    GLint viewport[4];
    GLdouble modelview[16]; //var to hold the modelview info
    GLdouble projection[16]; //var to hold the projection matrix info
    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info

	winX = (float)x;
    winY = (float)viewport[3] - (float)y;
	winZ = 0;

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
    // fim da conversão
    
    switch (button)
    {
        case GLUT_LEFT_BUTTON: printf("Botao esquerdo.");
                               break;
        case GLUT_RIGHT_BUTTON: printf("Botao direito.");
                               break;
        case GLUT_MIDDLE_BUTTON: printf("Botao do meio.");
                               break;
    }
    if (state == GLUT_DOWN){
        if(worldX>100&&worldX<500){
            if(worldY>100&&worldY<250){
                cena.instrucoes=1;
            }
            else if(worldY>350&&worldY<500){
                acoesMenu.emCimaNovoJogo=1;
            }
        }
        else{
            acoesMenu.emCimaInstrucao=0;
            acoesMenu.emCimaNovoJogo=0; 
        }
    }
    if(worldX>100&&worldX<500){
        if(worldY>100&&worldY<250){
            acoesMenu.emCimaInstrucao=1;
        }
        else if(worldY>350&&worldY<500){
            acoesMenu.emCimaNovoJogo=1;
        }
        else{
            acoesMenu.emCimaInstrucao=0;
            acoesMenu.emCimaNovoJogo=0;
        }
    }
    else{
        acoesMenu.emCimaInstrucao=0;
        acoesMenu.emCimaNovoJogo=0; 
    }
    if (state == GLUT_DOWN)
        printf("Pressionado na posição: ");
    if (state == GLUT_UP)
        printf("Liberado na posição: ");
    printf("(%d, %d)\n", (int)worldX,(int)worldY);

    
}
int main(int argc, char** argv){
    glutInit(&argc, argv); // acordando o GLUT

    //definindo a versão do OpenGL que vamos usar
    glutInitContextVersion(1,1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    //configuração inicial da janela GLUT
    glutInitDisplayMode(GLUT_DOUBLE); //QUANTIDADE DE BUFFERS -> GLUT_DOUBLE É DOIS
    glutInitWindowPosition(500,100); // posição inicial
    glutInitWindowSize(600,600);     // tamanho inicial

    glutCreateWindow("Menu Jogo");


    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);

    glutPassiveMotionFunc(MouseAndandoNaoPressionado);
    glutMouseFunc(mouse);

    inicializa();
    glutMainLoop();
    return 0;
}