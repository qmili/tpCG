#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <SOIL/SOIL.h>
#define QUADRADO_SIZE 150
#define MAX_QUADRADOS 5
#define LARGURA_MUNDO 600
#define ALTURA_MUNDO 600
#define MAX_SQUARES 300//define uma quantidade máxima de tiros

float rectPosX[MAX_QUADRADOS];
float rectPosY[MAX_QUADRADOS];
int numQuadrados = 0;


int  colisaoSize=QUADRADO_SIZE;


//float rectPosX = 0.0f;
//float rectPosY = 0.0f;
float targetX = 0.0f;
float targetY = 0.0f;
bool targetReached = true;
typedef struct {
    float posX, posY, deslocamentoemX, deslocamentoemY;
    float spriteX, spriteY;
    int tecla_w, tecla_a, tecla_d, tecla_s;
    int largura, altura;
    GLuint textura;
    GLuint Enemy;
    float rotacao;
    int spriteIndex;
    float velocidade;
      GLuint estrelinha;
    float TranslateX;
    float TranslateY;
    float TranslateEnemyX;
    float TranslateEnemyY;
} personagem;

float characterX = 0.0f;        // Posição X do personagem
float characterY = 0.0f;        // Posição Y do personagem
float velocityY = 0.0f;         // Velocidade vertical do personagem
bool isJumping = false;         // Indica se o personagem está pulando
float velocityX = 0.0f;         // Velocidade horizontal do personagem
float rotation = 0.0f;          // Ângulo de rotação do personagem
float rotationSpeed = 10.0f;  
// Estrutura para representar um quadrado
typedef struct {
    float x;        // Posição x do quadrado
    float y;        // Posição y do quadrado
    float speed;    // Velocidade do quadrado
    bool active;    // Indica se o quadrado está ativo ou não
} Square;

Square* squares[MAX_SQUARES];           // Vetor de ponteiros para quadrados
float destinations[MAX_SQUARES][2];     // Matriz para armazenar as posições de destino dos cliques do mouse
int numSquares = 0;    

float angle = 0.0f; 

bool keyPressed = false;
float velocidade = 100.0f;
int direcaoX = 0; // -1 para esquerda, 1 para direita, 0 para nenhum
int direcaoY = 0; // -1 para baixo, 1 para cima, 0 para nenhum

int spriteIndex = 0;
int frameInterval = 120; // Intervalo de 120ms para 8 frames por segundo
float texCoordX = 0.0f;
float texCoordY = 0.8f;

int invertAnimationDirection = 0; // Variável para controlar a direção da animação
int lineIndex = 3; // Variável para controlar a linha dos quadradinhos


float mouseX = 0.0f; // Posição X do mouse
float mouseY = 0.0f; // Posição Y do mouse


int frameCount = 0;
int ataqueEnemy=0;
int ataquePrincipal=0;// acertar o inimigo que é o meu quadrado


personagem personagem_Boss;



void destinoPersonagem(){
 if (numSquares < MAX_SQUARES) {
            int x=characterX;
            //subtrair um pouco mais para conseguir colidair exatamente;
            int y=-((ALTURA_MUNDO-characterY)-characterY);// achar o zero
            Square* newSquare = (Square*)malloc(sizeof(Square));
            newSquare->x = 0.0f;
            newSquare->y = 0.0f;
            newSquare->speed = 0.01f;
            newSquare->active = true;

            // Calcula a direção para o novo quadrado com base na posição do clique do mouse
            float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
            float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            float centerX = windowWidth / 2.0f;
            float centerY = windowHeight / 2.0f;
            float dx = x - centerX;
            float dy = centerY - y; // Inverte a direção y, pois o sistema de coordenadas OpenGL tem origem no canto inferior esquerdo
            float magnitude = sqrt(dx * dx + dy * dy);
            newSquare->x += dx / magnitude * newSquare->speed;
            newSquare->y += dy / magnitude * newSquare->speed;

            squares[numSquares++] = newSquare;
            destinations[numSquares - 1][0] = x;
            destinations[numSquares - 1][1] = y;
            x-=50;
            y-=50;
        }
}

void trocarSprite(int value) {
    int spriteIndexX = spriteIndex % 5; // Índice da coluna
    int spriteIndexY = spriteIndex / 5; // Índice da linha

    texCoordX = spriteIndexX * 0.2f;
    texCoordY = spriteIndexY * 0.2f;

    if (lineIndex == 1 && spriteIndexY == 0 && spriteIndexX == 4) {
        // Se estiver na última sprite da linha 1, mude para a linha 2
        lineIndex = 2;
        spriteIndexY = 1;
        spriteIndexX = 0;
    } else if (lineIndex == 2 && spriteIndexY == 1 && spriteIndexX == 4) {
        // Se estiver na última sprite da linha 2, mude para a linha 3
        lineIndex = 3;
        spriteIndexY = 2;
        spriteIndexX = 0;
    } else {
        spriteIndex = (spriteIndex + 1) % 5; // Altera o índice do sprite (de 0 a 4)

        if (spriteIndex == 0) {
            // Inverte a direção da animação ao chegar na última sprite
            invertAnimationDirection = !invertAnimationDirection;
        }

        if (invertAnimationDirection) {
            spriteIndex = 4 - spriteIndex; // Inverte o índice para animação reversa
        }

        spriteIndexX = spriteIndex % 5; // Atualiza o índice da coluna
        spriteIndexY = spriteIndex / 5; // Atualiza o índice da linha
    }

    if (lineIndex == 2) {
        // Se lineIndex for igual a 2, ajusta as coordenadas de textura para a linha 2
        texCoordY = 0.6f;
    } else if (lineIndex == 3) {
        // Se lineIndex for igual a 3, ajusta as coordenadas de textura para a linha 3
        texCoordY = 0.4f;
    } else {
        // Caso contrário, mantém as coordenadas de textura para a linha 1 (valor padrão)
        texCoordY = 0.8f;
    }

    if(ataquePrincipal==1){
    destinoPersonagem();
    }
        
    glutPostRedisplay();
       
    glutTimerFunc(frameInterval, trocarSprite, 0);
}


GLuint carregaTextura(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
        arquivo,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}

void inicializa() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    personagem_Boss.altura = personagem_Boss.largura = 150;
    personagem_Boss.posX = LARGURA_MUNDO / 2;
    personagem_Boss.posY = personagem_Boss.altura;
    personagem_Boss.textura = carregaTextura("abelha.png");
    personagem_Boss.estrelinha = carregaTextura("textura.png");
    personagem_Boss.Enemy = carregaTextura("texturascup.png");
    personagem_Boss.rotacao = 0.0f;
    personagem_Boss.spriteX = 0.0f; // Inicializar a coordenada X da textura do personagem
    personagem_Boss.spriteY = 0.0f; 
    personagem_Boss.velocidade = velocidade;

}
      

void drawSquare(float x, float y, float size) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(personagem_Boss.TranslateX, personagem_Boss.TranslateY, 0);
    glScalef(0.1f, 0.1f, 0.1f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, personagem_Boss.estrelinha);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(LARGURA_MUNDO / 2, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(LARGURA_MUNDO / 2, ALTURA_MUNDO / 2);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0.0f, ALTURA_MUNDO / 2);
    
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void drawEnemy() {
    glPushMatrix();

    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(personagem_Boss.TranslateEnemyX, personagem_Boss.TranslateEnemyY, 0);
    glScalef(0.5f, 0.5f, 0.5f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, personagem_Boss.Enemy);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
   
    
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void desenhaTiros(){
       for (int i = 0; i < numSquares; i++) {
        if (squares[i]->active) {
           if(ataquePrincipal==1){
             drawSquare(squares[i]->x, squares[i]->y, 2.0f); 
           }
        
            // Move o quadrado em direção à posição de destino
            float dx = squares[i]->speed * (destinations[i][0] - squares[i]->x);
            float dy = squares[i]->speed * (destinations[i][1] - squares[i]->y);
            squares[i]->x += dx;
            squares[i]->y += dy;

            // Verifica se o quadrado atingiu a posição de destino
            if (dx * dx + dy * dy < squares[i]->speed * squares[i]->speed) {
                squares[i]->active = false;
            }
        }
    }
}
// Função de callback para o evento de desenho
void RenderizaBoss() {
    glDisable(GL_TEXTURE_2D); 
    glPushMatrix();
    glTranslatef(personagem_Boss.TranslateX, personagem_Boss.TranslateY, 0);// if inde
    glRotatef(personagem_Boss.rotacao, 0, 0, 1); // Rotaciona em relação ao eixo z
    glScalef(1.0f, 1.0f, 1.0f); // Aplica a escala de 0.5

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, personagem_Boss.textura);

    glBegin(GL_QUADS);

    // Coordenadas de textura para os cinco quadrados
    texCoordX = spriteIndex * 0.2f;

    glTexCoord2f(texCoordX, texCoordY);
    glVertex2f(-personagem_Boss.largura / 2, -personagem_Boss.altura / 2);

    glTexCoord2f(texCoordX + 0.2f, texCoordY);
    glVertex2f(personagem_Boss.largura / 2, -personagem_Boss.altura / 2);

    glTexCoord2f(texCoordX + 0.2f, texCoordY + 0.15f);
    glVertex2f(personagem_Boss.largura / 2, personagem_Boss.altura / 2);

    glTexCoord2f(texCoordX, texCoordY + 0.2f);
    glVertex2f(-personagem_Boss.largura / 2, personagem_Boss.altura / 2);

    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void  desenhaProtagonista(){

    glTranslatef(characterX, characterY, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);

    glColor3f(1.0f, 0.0f, 0.0f); // Cor vermelha
    glBegin(GL_QUADS);
    glVertex2f(-25.0f, -25.0f);
    glVertex2f(25.0f, -25.0f);
    glVertex2f(25.0f, 25.0f);
    glVertex2f(-25.0f, 25.0f);
    glEnd();
 
    
}


void desenhaTiroProtagonista() {
    glPushMatrix();
    glScalef(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, personagem_Boss.Enemy);
    
    for (int i = 0; i < numQuadrados; i++) {
        glBegin(GL_QUADS);
        
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(rectPosX[i] - QUADRADO_SIZE, rectPosY[i] - QUADRADO_SIZE, 0.0f);
        
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(rectPosX[i] + QUADRADO_SIZE, rectPosY[i] - QUADRADO_SIZE, 0.0f);
        
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(rectPosX[i] + QUADRADO_SIZE, rectPosY[i] + QUADRADO_SIZE, 0.0f);
        
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(rectPosX[i] - QUADRADO_SIZE, rectPosY[i] + QUADRADO_SIZE, 0.0f);
        
        glEnd();
    }
    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

bool checarColisaoEntreProIni1(){

}
bool checarColisaoEntreProIni2(){

}
bool checarColisaoEntreProIni3(){

}
bool checarColisaoEntreProIni4(){

}
bool checarColisaoEntreProTiroBoss() {
    for (int i = 0; i < 5; i++) {
        float distance = sqrt(pow(rectPosX[i] - personagem_Boss.TranslateX, 2) + pow(rectPosY[i] - personagem_Boss.TranslateY, 2));
        if (distance <= 150 * 2) {
            return true; // Se houver colisão, retorna verdadeiro
        }
    }
    // Fora do loop, se nenhuma colisão for encontrada, retorna falso
    return false;
}



void desenhaInimigo1(float x, float y){
  //  glColor3f(0,0, 1.0);
   // glBegin(GL_QUADS);
   // glVertex2f(x - squareSize, y - squareSize);
   // glVertex2f(x + squareSize, y - squareSize);
   // glVertex2f(x + squareSize, y + squareSize);
   // glVertex2f(x - squareSize, y + squareSize);
    //glEnd();
}


void desenhaInimigo2(){

}
void desenhaInimigo3(){

}
void desenhaInimigo4(){

}

void desenhaInimigos(){
   // desenhaInimigo1();
    desenhaInimigo2();
    desenhaInimigo3();
    desenhaInimigo4();

}
void drawScene() {

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0); // Define a cor de fundo como branco (RGB: 1, 1, 1)
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,LARGURA_MUNDO,0, ALTURA_MUNDO,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    RenderizaBoss();

    frameCount++;
    if (frameCount >= 25 && lineIndex == 3) { // Troca de sprite a cada 25 frames (aproximadamente 1 segundo)
        frameCount = 0;
        spriteIndex = (spriteIndex + 1) % 5; // Altera o índice do sprite (de 0 a 4)
        glutPostRedisplay(); // Solicita o redesenho da cena
    } 
    if(lineIndex==3){

    desenhaTiros(); 
    personagem_Boss.TranslateX=70;
    personagem_Boss.TranslateY=300;
    if (!targetReached) {
          glPushMatrix();
          desenhaTiroProtagonista();       
          glPopMatrix();
    }
      glColor3f(1.0f, 1.0f, 1.0f);   
    }
    if(lineIndex==0){
      personagem_Boss.TranslateX=300;
      personagem_Boss.TranslateY=350;
    }

    if(lineIndex==2){

    }
     // Verifica a colisão e muda a cor se necessário
    if ( checarColisaoEntreProTiroBoss()) {
        printf("Ocorreu uma colisão!");
    }
       
    desenhaInimigos();
    desenhaProtagonista();

    glFlush();
    glutSwapBuffers();
}

// Função de callback para o evento de redimensionamento da janela
void reshape(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARGURA_MUNDO, 0, ALTURA_MUNDO, -1, 1);

    float razaoAspectoJanela = ((float)width) / height;
    float razaoAspectoMundo = ((float)LARGURA_MUNDO) / ALTURA_MUNDO;

    if (razaoAspectoJanela < razaoAspectoMundo) {
        float hViewport = width / razaoAspectoMundo;
        float yViewport = (height - hViewport) / 2;
        glViewport(0, yViewport, width, hViewport);
    } else {
        if (razaoAspectoJanela > razaoAspectoMundo) {
            float wViewport = ((float)height) * razaoAspectoMundo;
            float xViewport = (width - wViewport) / 2;
            glViewport(xViewport, 0, wViewport, height);
        } else {
            glViewport(0, 0, width, height);
        }
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Função de inicialização do OpenGL
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void update(int value) {
  characterX += velocityX;

    if (isJumping)
    {
        characterY += velocityY;

        // Diminui gradualmente a velocidade vertical para simular a gravidade
        velocityY -= 0.2f;

        // Verifica se o personagem voltou ao solo (ou próximo a ele)
        if (characterY <= 0.0f)
        {
            // Define o personagem como não pulando e redefine a posição e velocidade vertical
            isJumping = false;
            characterY = 0.0f;
            velocityY = 0.0f;
        }
    }

    // Incrementa o ângulo de rotação enquanto a tecla de espaço estiver pressionada
    if (isJumping)
    {
        rotation += rotationSpeed;
    }
     if (!targetReached) {
        for (int i = 0; i < numQuadrados; i++) {
            float dx = targetX - rectPosX[i];
            float dy = targetY - rectPosY[i];
            float distance = sqrt(dx * dx + dy * dy);
            
            dx /= distance;
            dy /= distance;
            
            rectPosX[i] += dx * 5.0f;
            rectPosY[i] += dy * 5.0f;
            
            if (distance < 5.0f) {
                targetReached = true;
            }
        }
        
        if (targetReached && numQuadrados == MAX_QUADRADOS) {
            // Todos os quadrados atingiram o destino, resetar o vetor
            numQuadrados = 0;
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


void keyboard(unsigned char key, int x, int y)
{ targetReached = false;  // Defi
    // Pressionar a barra de espaço faz o personagem pular
    if (key == ' ' && !isJumping)
    {
        isJumping = true;
        velocityY = 10.0f;  // Aumenta a velocidade vertical para 10.0f para um salto mais alto
      
    }
    
    // Movimento para cima
    else if (key == 'w')
    {
        velocityY = 5.0f;
    }
    // Movimento para a direita
    else if (key == 'd')
    {
        velocityX = 5.0f;
    }
    // Movimento para baixo
    else if (key == 's')
    {
        velocityY = -5.0f;
    }
    // Movimento para a esquerda
    else if (key == 'a')
    {
        velocityX = -5.0f;
    }
    if(key=='h'){
    ataqueEnemy=1;
    keyPressed = true;
    targetX=personagem_Boss.TranslateX;
    targetY=personagem_Boss.TranslateY;
      if (numQuadrados < MAX_QUADRADOS) {
                rectPosX[numQuadrados] =characterX;
                rectPosY[numQuadrados] = characterY;
                numQuadrados++;
            }

    }
  
}

void keyboardUp(unsigned char key, int x, int y)
{
    // Liberar a barra de espaço faz o personagem retornar à origem
    if (key == ' ' && isJumping)
    {
        characterY = 0.0f;
        velocityY = 0.0f;
        rotation = 0.0f;  // Reseta o ângulo de rotação ao soltar a tecla de espaço
    }
    // Parar o movimento vertical quando a tecla correspondente for solta
    else if (key == 'w' && velocityY > 0.0f)
    {
        velocityY = 0.0f;
    }
    else if (key == 's' && velocityY < 0.0f)
    {
        velocityY = 0.0f;
    }
    // Parar o movimento horizontal quando a tecla correspondente for solta
    else if (key == 'd' && velocityX > 0.0f)
    {
        velocityX = 0.0f;
    }
    else if (key == 'a' && velocityX < 0.0f)
    {
        velocityX = 0.0f;
    }
   
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Quadrados em direção ao clique do mouse");

    init();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
  
    glutIdleFunc(drawScene);
    glutTimerFunc(frameInterval, trocarSprite, 0);
    glutTimerFunc(0, update, 0); 
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
   // glutTimerFunc(0, updatePuloEnemy, 0);

    inicializa();
    glutMainLoop();

    // Libera a memória alocada pelos quadrados
    for (int i = 0; i < numSquares; i++) {
        free(squares[i]);
    }

    return 0;
}
