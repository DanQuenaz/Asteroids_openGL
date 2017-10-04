/*
Autores:
Alexandre S. Nogueira
Dan Quenaz M. S. Pimentel
*/

#include <iostream>
#include <thread>
#include "environment.cpp"
#include "rgb.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <pthread.h>
#include "tga.cpp"


#include <GL/glut.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>


using namespace std;
//#include "position.cpp"
#define MAX 41
#define win 500.0f


GLint view_w, view_h;

Environment *env;// Ambiente
Pos *dim; // dimensao x -> w, y -> h

pthread_t thread1, thread2, thread3;

int argc1; 
char** argv1;

int sound_select;
int cont_anim = 9;
int mov_L, mov_A;  // sentido das movimentacoes linear e angular
int mov_Acel; //Aceleração e desaceleração
double input_delay=1.5;
double time_global=0, time_A=-input_delay, time_L=-input_delay;



void *toca_audio( void *ptr )
{
     char *message;
     message = (char *) ptr;
     while(sound_select ){
     	if(sound_select == 32){
     		system("canberra-gtk-play -f Sounds/tiro_nave.ogg");
     		sound_select =1;
     	}
     	
     }
}

void *sound_track( void *ptr )
{
     char *message;
     message = (char *) ptr;
     while(true){
     	system("canberra-gtk-play -f Sounds/musica1.ogg");
     	
     }
}

void draw_texto24(string text, RGB c, Pos p) 
{  
    glColor3f(c.r, c.g, c.b);
    glPushMatrix();
        // Posição no universo onde o texto será colocado          
        glRasterPos2f(p.x, p.y); 
        // Exibe caracter a caracter
            for(int k=0; k<text.size(); ++k)
            	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,text[k]);
  glPopMatrix();
}

void draw_texto10(string text, RGB c, Pos p) 
{  
    glColor3f(c.r, c.g, c.b);
    glPushMatrix();
        // Posição no universo onde o texto será colocado          
        glRasterPos2f(p.x, p.y); 
        // Exibe caracter a caracter
        for(int k=0; k<text.size(); ++k)
             glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[k]); 
  glPopMatrix();
}

void draw_texto18(string text, RGB c, Pos p) 
{  
    glColor3f(c.r, c.g, c.b);
    glPushMatrix();
        // Posição no universo onde o texto será colocado          
        glRasterPos2f(p.x, p.y); 
        // Exibe caracter a caracter
        for(int k=0; k<text.size(); ++k)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[k]); 
  glPopMatrix();
}

void draw_upMenu(){
	string title = "BUILD STATS";
	string pts = "P - Increment power shot";
	string hp = "H - Increment max HP";

	RGB *color = new RGB(1.0f, 0.0f, 0.0f);
	Pos *pos = new Pos(0.0f, 200.0f);
	draw_texto24(title, *color, *pos);
	delete color; delete pos;

	
	pos = new Pos(-60.0f, 180.0f); color = new RGB(0.0f, 1.0f, 0.0f);
	draw_texto24(pts, *color, *pos);
	delete color; delete pos;

	pos = new Pos(-60.0f, 160.0f); color = new RGB(0.0f, 1.0f, 0.0f);
	draw_texto24(hp, *color, *pos);
	delete color; delete pos;

}

void draw_infoBar(){
	string buffer;
	string pts = "PONTOS: ";
	string hp = "HP: ";
	string ps = "POWER SHOT: ";
	string st = "STAGE: ";

	RGB *color = new RGB(1.0f, 0.0f, 0.0f);
	Pos *pos = new Pos(-220.0f, -220.0f);
	draw_texto10(hp, *color, *pos);
	delete color; delete pos;

	//itoa(env->player->life, buffer, 10);
	buffer = to_string(env->player->life);
	pos = new Pos(-200.0f, -220.0f); color = new RGB(0.0f, 1.0f, 0.0f);
	draw_texto10(buffer, *color, *pos);
	delete color; delete pos;

	pos = new Pos(-130.0f, -220.0f); color = new RGB(1.0f, 0.0f, 0.0f);
	draw_texto10(ps, *color, *pos);
	delete color; delete pos;

	buffer = to_string(env->player->power_shot);
	pos = new Pos(-60.0f, -220.0f); color = new RGB(0.0f, 1.0f, 0.0f);
	draw_texto10(buffer, *color, *pos);
	delete color; delete pos;

	pos = new Pos(-40.0f, -220.0f); color = new RGB(1.0f, 0.0f, 0.0f);
	draw_texto10(st, *color, *pos);
	delete color; delete pos;

	buffer = to_string(env->stage);
	pos = new Pos(0.0f, -220.0f); color = new RGB(0.0f, 1.0f, 0.0f);
	draw_texto10(buffer, *color, *pos);
	delete color; delete pos;

	buffer = to_string(env->die_enemies);
	pos = new Pos(190.0f, -220.0f); color = new RGB(0.2f, 1.0f, 0.85f);
	draw_texto18(buffer, *color, *pos);
	delete color; delete pos;

	pos = new Pos(210.0f, -220.0f); color = new RGB(0.2f, 1.0f, 0.85f);
	draw_texto18("/", *color, *pos);
	delete color; delete pos;

	buffer = to_string(env->stage*10);
	pos = new Pos(230.0f, -220.0f); color = new RGB(0.2f, 1.0f, 0.85f);
	draw_texto18(buffer, *color, *pos);
	delete color; delete pos;

}

void draw_menuStage(){
	string buffer;

	RGB *color = new RGB(0.0f, 0.0f, 1.0f);
	Pos *pos = new Pos(-30.0f, 30.0f);
	draw_texto24("STAGE: ", *color, *pos);
	delete color; delete pos;

	buffer = to_string(env->stage);
	pos = new Pos(40.0f, 30.0f); color = new RGB(0.0f, 0.0f, 1.0f);
	draw_texto24(buffer, *color, *pos);
	delete color; delete pos;

	pos = new Pos(-60.0f, -30.0f); color = new RGB(0.0f, 1.0f, 0.0f);
	draw_texto24("Press ENTER to start!", *color, *pos);
	delete color; delete pos;
}

void draw_gameOver(){
	string buffer;

	RGB *color = new RGB(0.0f, 0.0f, 1.0f);
	Pos *pos = new Pos(-30.0f, 30.0f);
	draw_texto24("GAME OVER", *color, *pos);
	delete color; delete pos;

	pos = new Pos(-60.0f, -30.0f); color = new RGB(0.0f, 1.0f, 0.0f);
	draw_texto24("Press ENTER to continue!", *color, *pos);
	delete color; delete pos;
}

void draw_gameWin(){
	string buffer;

	RGB *color = new RGB(0.0f, 0.0f, 1.0f);
	Pos *pos = new Pos(-30.0f, 30.0f);
	draw_texto24("CONGRATULATIONS!!!", *color, *pos);
	delete color; delete pos;

	pos = new Pos(-60.0f, -30.0f); color = new RGB(0.0f, 1.0f, 0.0f);
	draw_texto24("Press ENTER to continue!", *color, *pos);
	delete color; delete pos;
}


void draw_menu1(){
	RGB *color = new RGB(0.0f, 0.0f, 1.0f);
	Pos *pos = new Pos(-70.0f, 150.0f);
	draw_texto24("ASTEROIDS INVADERS", *color, *pos);
	delete color; delete pos;
	pos = new Pos(-50.0f, 100.0f); color = new RGB(0.0f, 1.0f, 0.0f);
	draw_texto24("N  - New Game", *color, *pos);
	delete color; delete pos;
	pos = new Pos(-50.0f, 80.0f); color = new RGB(0.0f, 1.0f, 0.0f);
	draw_texto24("H - How to play", *color, *pos);
	delete color; delete pos;
	pos = new Pos(-50.0f, 60.0f); color = new RGB(0.0f, 1.0f, 0.0f);
	draw_texto24("ESC - Exit Game", *color, *pos);
	delete color; delete pos;
}

void draw_Boss(Enemy_Boss *b){
	int n=b->n_points;
    Pos *points[MAX];
    forI(n,points[i]=Pos::add(*(b->pos),*(b->points[i])););

	if(!b->hit){
		float alfa=0.5f-(float)b->life/((float)b->lifemax*2.0);
		glColor3f(alfa+0.5f, 0.5f, 0.5f);
	}else{
		b->hit=false;
		glColor3f(1.0f, 01.0f, 0.8f);
	}
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 6); 
    glBegin(GL_POLYGON);
		glTexCoord2f (0.5f,1.0f);glVertex2f(points[0]->x,points[0]->y);
        glTexCoord2f (1.0f,0.75f);glVertex2f(points[1]->x,points[1]->y);
        glTexCoord2f (1.0f,0.25f);glVertex2f(points[2]->x,points[2]->y);
        glTexCoord2f (0.5f,0.0f);glVertex2f(points[3]->x,points[3]->y);
        glTexCoord2f (0.0f,0.25f);glVertex2f(points[4]->x,points[4]->y);
        glTexCoord2f (0.0f,0.75f);glVertex2f(points[5]->x,points[5]->y);
	glEnd();

/*
	glColor3f(1.0f, 0.1f, 0.1f);
	glBegin(GL_POLYGON);
		glVertex2f(points[4]->x,points[4]->y);
        glVertex2f(points[5]->x,points[5]->y);
        glVertex2f(points[6]->x,points[6]->y);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(points[7]->x,points[7]->y);
        glVertex2f(points[8]->x,points[8]->y);
        glVertex2f(points[9]->x,points[9]->y);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(points[10]->x,points[10]->y);
        glVertex2f(points[11]->x,points[11]->y);
        glVertex2f(points[12]->x,points[12]->y);
        glVertex2f(points[13]->x,points[13]->y);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(points[14]->x,points[14]->y);
        glVertex2f(points[15]->x,points[15]->y);
        glVertex2f(points[16]->x,points[16]->y);
        glVertex2f(points[17]->x,points[17]->y);
	glEnd();*/

	forI(n,delete points[i];);
	glDisable (GL_TEXTURE_2D);

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
		glVertex2f(-win/2.0f, 230.0f);
        glVertex2f(-win/2.0f, 250.0f);
        glVertex2f((win/2.0f)-(500.0f - env->boss->life), 250.0f);
        glVertex2f((win/2.0f)-(500.0f - env->boss->life), 230.0f);
	glEnd();
}

void draw_bossBullets(vector<Boss_Bullet *> b){
	glColor3f(0.95f, 0.95f, 1.0f);
	int n;
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 7); 
	for(int k=0; k<b.size(); ++k){
		n = b[k]->n_points;
		Pos *points[n];
		forI(n,points[i]=Pos::add(*(b[k]->pos), *(b[k]->points[i])););
		glBegin(GL_POLYGON);
				glTexCoord2f (0.5f,1.0f);glVertex2f(points[0]->x,points[0]->y);
                glTexCoord2f (1.0f,0.75f);glVertex2f(points[1]->x,points[1]->y);
                glTexCoord2f (1.0f,0.25f);glVertex2f(points[2]->x,points[2]->y);
                glTexCoord2f (0.5f,0.0f);glVertex2f(points[3]->x,points[3]->y);
                glTexCoord2f (0.0f,0.25f);glVertex2f(points[4]->x,points[4]->y);
                glTexCoord2f (0.0f,0.75f);glVertex2f(points[5]->x,points[5]->y);
		glEnd();
		forI(n,delete points[i];);
	}
	glDisable (GL_TEXTURE_2D);

}

void draw_bullets(vector<Bullet *> b){
	int n;
	for(int k=0; k<env->bullets.size(); ++k){
		if(b[k]->type_id==1)
			glColor3f(0.8f, 0.2f, 0.0f);
		else
			glColor3f(0.0f, 0.8f, 0.8f);

		n = b[k]->n_points;
		Pos *points[n];
		forI(n,points[i]=Pos::add(*(b[k]->position), *(b[k]->points[i])););
		glBegin(GL_POLYGON);
				glVertex2f(points[0]->x,points[0]->y);
                glVertex2f(points[1]->x,points[1]->y);
                glVertex2f(points[2]->x,points[2]->y);
                glVertex2f(points[3]->x,points[3]->y);
		glEnd();
		forI(n,delete points[i];);
	}

}

void draw_enemies(vector<Enemy_1 *> e1, vector<Enemy_2 *> e2, vector<Enemy_3 *> e3){
	int n;
	glEnable (GL_TEXTURE_2D); 
	for(int k=0; k<env->enemies1.size(); ++k){
		if(!e1[k]->hit){
			float alfa=0.5f-(float)e1[k]->life/((float)e1[k]->lifemax*2.0);
			glColor3f(alfa+0.5f, 0.5f, 0.5f);
		}else{
			e1[k]->hit=false;
			glColor3f(1.0f, 01.0f, 0.8f);
		}
		glBindTexture (GL_TEXTURE_2D, e1[k]->texture_id);
		n = e1[k]->n_points;
		Pos *points[n];
		forI(n,points[i]=Pos::add(*(e1[k]->pos), *(e1[k]->points[i])););
		glBegin(GL_POLYGON);
				glTexCoord2f (0.5f,1.0f);glVertex2f(points[0]->x,points[0]->y);
                glTexCoord2f (1.0f,0.75f);glVertex2f(points[1]->x,points[1]->y);
                glTexCoord2f (1.0f,0.25f);glVertex2f(points[2]->x,points[2]->y);
                glTexCoord2f (0.5f,0.0f);glVertex2f(points[3]->x,points[3]->y);
                glTexCoord2f (0.0f,0.25f);glVertex2f(points[4]->x,points[4]->y);
                glTexCoord2f (0.0f,0.75f);glVertex2f(points[5]->x,points[5]->y);
		glEnd();

		forI(n,delete points[i];);
		
	}

	for(int k=0; k<env->enemies2.size(); ++k){
		if(!e2[k]->hit){
			float alfa=0.5f-(float)e2[k]->life/((float)e2[k]->lifemax*2.0);
			glColor3f(alfa+0.5f, 0.5f, 0.5f);
		}else{
			e2[k]->hit=false;
			glColor3f(1.0f, 01.0f, 0.8f);
		}
		glBindTexture (GL_TEXTURE_2D, e2[k]->texture_id);
		n = e2[k]->n_points;
		Pos *points[n];
		forI(n,points[i]=Pos::add(*(e2[k]->pos), *(e2[k]->points[i])););
		glBegin(GL_POLYGON);
				glTexCoord2f (0.5f,1.0f);glVertex2f(points[0]->x,points[0]->y);
                glTexCoord2f (1.0f,0.75f);glVertex2f(points[1]->x,points[1]->y);
                glTexCoord2f (1.0f,0.25f);glVertex2f(points[2]->x,points[2]->y);
                glTexCoord2f (0.5f,0.0f);glVertex2f(points[3]->x,points[3]->y);
                glTexCoord2f (0.0f,0.25f);glVertex2f(points[4]->x,points[4]->y);
                glTexCoord2f (0.0f,0.75f);glVertex2f(points[5]->x,points[5]->y);
		glEnd();

		forI(n,delete points[i];);
		
	}

	
	for(int k=0; k<env->enemies3.size(); ++k){
		if(!e3[k]->hit){
			float alfa=0.5f-(float)e3[k]->life/((float)e3[k]->lifemax*2.0);
			glColor3f(alfa+0.5f, 0.5f, 0.5f);
		}else{
			e3[k]->hit=false;
			glColor3f(1.0f, 01.0f, 0.8f);
		}
		glBindTexture (GL_TEXTURE_2D, e3[k]->texture_id);
		n = e3[k]->n_points;
		Pos *points[n];
		forI(n,points[i]=Pos::add(*(e3[k]->pos), *(e3[k]->points[i])););
		glBegin(GL_POLYGON);
				glTexCoord2f (0.5f,1.0f);glVertex2f(points[0]->x,points[0]->y);
                glTexCoord2f (1.0f,0.75f);glVertex2f(points[1]->x,points[1]->y);
                glTexCoord2f (1.0f,0.25f);glVertex2f(points[2]->x,points[2]->y);
                glTexCoord2f (0.5f,0.0f);glVertex2f(points[3]->x,points[3]->y);
                glTexCoord2f (0.0f,0.25f);glVertex2f(points[4]->x,points[4]->y);
                glTexCoord2f (0.0f,0.75f);glVertex2f(points[5]->x,points[5]->y);
		glEnd();

		forI(n,delete points[i];);
		
	}

	glDisable (GL_TEXTURE_2D); /* disable texture mapping */
}

void draw_player(Player* p){
		static int frame_c=0;
		int n=p->n_points;
		Pos *points[MAX];
		forI(n,points[i]=Pos::add(*(p->pos),*(p->points[i])););

		glColor3f(1.0f, 1.0f, 1.0);
		glEnable (GL_TEXTURE_2D);
		glBindTexture (GL_TEXTURE_2D, 9+frame_c/8);frame_c=(frame_c+1)%32; 
		glBegin(GL_POLYGON);
			glTexCoord2f (1.0f,1.0f);glVertex2f(points[0]->x,points[0]->y);
			glTexCoord2f (1.0f,0.0f);glVertex2f(points[1]->x,points[1]->y);
			glTexCoord2f (0.0f,0.0f);glVertex2f(points[2]->x,points[2]->y);
			glTexCoord2f (0.0f,1.0f);glVertex2f(points[3]->x,points[3]->y);
		glEnd();
			
		glDisable(GL_TEXTURE_2D);
		

      /*
      glColor3f(0.5f, 0.5f, 0.5f);
      glBegin(GL_POLYGON);
                glVertex2f(points[0]->x,points[0]->y);
                glVertex2f(points[1]->x,points[1]->y);
                glVertex2f(points[2]->x,points[2]->y);      
      glEnd();
      glBegin(GL_POLYGON);
                glVertex2f(points[3]->x,points[3]->y);
                glVertex2f(points[4]->x,points[4]->y);
                glVertex2f(points[5]->x,points[5]->y);      
      glEnd();
      glBegin(GL_POLYGON);
                glVertex2f(points[6]->x,points[6]->y);
                glVertex2f(points[7]->x,points[7]->y);
                glVertex2f(points[8]->x,points[8]->y);      
      glEnd();
      glBegin(GL_POLYGON);
                glVertex2f(points[9]->x,points[9]->y);
                glVertex2f(points[10]->x,points[10]->y);
                glVertex2f(points[11]->x,points[11]->y);      
      glEnd();

		if(!p->hit){
      		glColor3f(0.2f, 0.25f, 1.0f);
		}else{
			p->hit=false;
			glColor3f(1.0f, 1.0f, 0.8f);
		}
      glBegin(GL_POLYGON);
                glVertex2f(points[12]->x,points[12]->y);
                glVertex2f(points[13]->x,points[13]->y);
                glVertex2f(points[14]->x,points[14]->y);
                glVertex2f(points[15]->x,points[15]->y);
                glVertex2f(points[16]->x,points[16]->y);
                glVertex2f(points[17]->x,points[17]->y);
                glVertex2f(points[18]->x,points[18]->y);
                glVertex2f(points[19]->x,points[19]->y);
                glVertex2f(points[20]->x,points[20]->y);      
      glEnd();

      glBegin(GL_POLYGON);
                glVertex2f(points[21]->x,points[21]->y);
                glVertex2f(points[22]->x,points[22]->y);
                glVertex2f(points[23]->x,points[23]->y);
                glVertex2f(points[24]->x,points[24]->y);
                glVertex2f(points[25]->x,points[25]->y);
                glVertex2f(points[26]->x,points[26]->y);
                glVertex2f(points[27]->x,points[27]->y);
                glVertex2f(points[28]->x,points[28]->y);
                glVertex2f(points[29]->x,points[29]->y);      
      glEnd();

      glBegin(GL_POLYGON);
                glVertex2f(points[30]->x,points[30]->y);
                glVertex2f(points[31]->x,points[31]->y);
                glVertex2f(points[32]->x,points[32]->y);
                glVertex2f(points[33]->x,points[33]->y);     
      glEnd();

      glColor3f(0.5f, 0.5f, 0.5f);
      glBegin(GL_POLYGON);
                glVertex2f(points[34]->x,points[34]->y);
                glVertex2f(points[35]->x,points[35]->y);
                glVertex2f(points[36]->x,points[36]->y);      
      glEnd();

      glColor3f((float)fmax(fmin(input_delay-time_global+time_L,1.0f),0.0), 0.15f,0.0f);
      glBegin(GL_POLYGON);
                glVertex2f(points[37]->x,points[37]->y);
                glVertex2f(points[38]->x,points[38]->y);
                glVertex2f(points[39]->x,points[39]->y);
                glVertex2f(points[40]->x,points[40]->y);
                    
      glEnd();
      */

      

      

      //cout<<p->pos->x<<", "<<p->pos->y<<", "<< atan2(p->direction->y,p->direction->x)*180.0/PI<<endl<<p->life<<endl;
      forI(n,delete points[i];);
}

void draw_stars(vector<Star *> s){
	glColor3f(0.95f, 0.95f, 1.0f);
	int n;
	for(int k=0; k<s.size(); ++k){
		n = s[k]->n_points;
		Pos *points[n];
		forI(n,points[i]=Pos::add(*(s[k]->pos), *(s[k]->points[i])););
		glBegin(GL_POLYGON);
				glVertex2f(points[0]->x,points[0]->y);
                glVertex2f(points[1]->x,points[1]->y);
                glVertex2f(points[2]->x,points[2]->y);
                glVertex2f(points[3]->x,points[3]->y);
		glEnd();

		forI(n,delete points[i];);
	}
}

// Função callback chamada para fazer o desenho
void Desenha(void){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);

	if(env->fun == 0){
		draw_menu1();
	}
	else if(env->fun == 1){
		draw_stars(env->stars);
		draw_player(env->player);
		draw_enemies(env->enemies1, env->enemies2, env->enemies3);
		draw_bullets(env->bullets);
		draw_infoBar();

	}
	else if(env->fun==2){
		draw_upMenu();
	}
	else if(env->fun == 3){
		draw_menuStage();
	}
	else if(env->fun == 4){
		draw_stars(env->stars);
		draw_player(env->player);
		draw_Boss(env->boss);
		draw_bullets(env->bullets);
		draw_bossBullets(env->boss_bullets);
		draw_infoBar();
	}
	else if(env->fun==5){
    	draw_gameOver();
    }
    else if(env->fun==6){
    	draw_gameWin();
    }

    glFlush();

    glutSwapBuffers();
	
}

// Inicializa parâmetros de rendering
void Inicializa (void){   
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.00f, 0.0f, 0.0f, 1.0f);
    
    glEnable (GL_DEPTH_TEST);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    if (!loadTGA ("Textures/texture1.tga", 1))
        printf ("texture.tga not found!\n");

    if (!loadTGA ("Textures/texture2.tga", 2))
        printf ("texture.tga not found!\n");

    if (!loadTGA ("Textures/texture3.tga", 3))
        printf ("texture.tga not found!\n");

    if (!loadTGA ("Textures/texture4.tga", 4))
        printf ("texture.tga not found!\n");

    if (!loadTGA ("Textures/texture5.tga", 5))
        printf ("texture.tga not found!\n");

    if (!loadTGA ("Textures/textureBoss.tga", 6))
        printf ("texture.tga not found!\n");

    if (!loadTGA ("Textures/textureBoss2.tga", 7))
        printf ("texture.tga not found!\n");

    if (!loadTGA ("Textures/textureNave.tga", 8))
        printf ("texture.tga not found!\n");

    if (!loadTGA ("Textures/nave0.tga", 9))
        printf ("texture.tga not found!\n");
    if (!loadTGA ("Textures/nave1.tga", 10))
        printf ("texture.tga not found!\n");
    if (!loadTGA ("Textures/nave2.tga", 11))
        printf ("texture.tga not found!\n");
    if (!loadTGA ("Textures/nave3.tga", 12))
        printf ("texture.tga not found!\n");

    dim = Pos::create((double)win,(double)win);
    env = Environment::create(dim);
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{ 
    // Especifica as dimensões da Viewport

    glViewport(0, 0, w, h);

    view_w = w;
    view_h = h;                   

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D((GLfloat)-dim->x/2.0f, (GLfloat)dim->x/2.0f, (GLfloat)-dim->y/2.0f, (GLfloat)dim->y/2.0f);
}

void Timer(int value){
    env->run_game(mov_L, mov_A, mov_Acel);
    //cout<<env->enemies.size();
    mov_A=0; mov_Acel=0;
    glutPostRedisplay();
    time_global+=0.016;
    //if(time_global-time_A>0.1)mov_A=0;
    //if(time_global-time_L>input_delay)mov_L=0;
    glutTimerFunc(16,Timer, 1);
}
// Função callback chamada para gerenciar eventos de teclado
void GerenciaTeclado(unsigned char key, int x, int y)
{
	if(env->fun==1 || env->fun==4){
	    if(key == 'd'){
	        if(mov_A!=1)time_A=time_global;
	        mov_A=1;
	    }
	    else if(key == 'a'){
	        if(mov_A!=-1)time_A=time_global;
	        mov_A=-1;
	    }
	    else if(key == 'w'){
	        mov_L=1;
	        mov_Acel = 1;
	        time_L=time_global;
	    }
	    else if(key == 's'){
	        mov_L=-1;
	        mov_Acel = -1;
	        time_L=time_global;
	    }
	    else if(key == 'e'){
	        mov_L=mov_A=0;
	    }
	    else if(key == 27){
        	//pthread_cancel(thread1);
        	//pthread_cancel(thread2);
        	pthread_cancel(thread3);
        	exit(0);
    	}
	}

	else if(env->fun == 0){
	    if(key == 'n'){
	          env->fun = 1;
	    }
	    else if(key == 27){
        	//pthread_cancel(thread1);
        	//pthread_cancel(thread2);
        	pthread_cancel(thread3);
        	exit(0);
    	}
	}
	else if(env->fun == 2){
	    if(key == 'p'){
	          env->fun = 3;
	          env->upPlayer(1);
	          env->setNextStage();
	    }
	    else if(key == 'h'){
	          env->fun = 3;
	          env->upPlayer(2);
	          env->setNextStage();
	    }
	    else if(key == 27){
        	//pthread_cancel(thread1);
        	//pthread_cancel(thread2);
        	pthread_cancel(thread3);
        	exit(0);
    	}
	}
	else if(env->fun==3){
	    if(key == 13 && env->fun == 3){
	          env->fun = 1;
	    }
	    else if(key == 27){
        	//pthread_cancel(thread1);
        	//pthread_cancel(thread2);
        	pthread_cancel(thread3);
        	exit(0);
    	}
	}

	else if(env->fun==5 || env->fun==6){
	    if(key == 13){
	          env->reset();
	    }
	    else if(key == 27){
        	//pthread_cancel(thread1);
        	//pthread_cancel(thread2);
        	pthread_cancel(thread3);
        	exit(0);
    	}
	}	

    else{}
    
}
           
// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
    
    if (button == GLUT_LEFT_BUTTON)
         if (state == GLUT_DOWN) {
            env->add_bullet();
            sound_select = 32;
                  //xf = ( (2 * win * x) / view_w) - win;
                  //yf = ( ( (2 * win) * (y-view_h) ) / -view_h) - win;
         }
    
}

// Função callback chamada para gerenciar eventos do teclado   
// para teclas especiais, tais como F1, PgDn e Home
void TeclasEspeciais(int key, int x, int y)
{
    /*
    if(key == GLUT_KEY_UP) {
           win -= 20;
           glMatrixMode(GL_PROJECTION);
           glLoadIdentity();
           gluOrtho2D (-win, win, -win, win);
    }
    if(key == GLUT_KEY_DOWN) {
           win += 20;
           glMatrixMode(GL_PROJECTION);
           glLoadIdentity();
           gluOrtho2D (-win, win, -win, win);
    }
    glutPostRedisplay();
    */
}
     
// Programa Principal 

void* startGame(void *ptr){
	srand(time(NULL));
    glutInit(&argc1,argv1);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(win*1.5,win*1.5);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Asteroids Invaders");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(GerenciaTeclado);
    glutMouseFunc(GerenciaMouse);
    //glutSpecialFunc(TeclasEspeciais); 
    glutTimerFunc(16, Timer, 1);    
    Inicializa();
    glutMainLoop();
}

int main(int argc, char** argv)
{	
	
	const char *message1 = "Thread 1";
	const char *message2 = "Thread 2";
	const char *message3 = "Thread 3";
	int  iret1, iret2, iret3;

	sound_select = 1;

	argc = argc;
	argv1 = argv;

	iret1 = pthread_create( &thread1, NULL, startGame, (void*)message1);
	if(iret1){
		fprintf(stderr,"Error ‐ pthread_create() return code: %d\n",iret1);
		exit(EXIT_FAILURE);
	}
	printf("pthread_create() for thread 1 returns: %d\n",iret1);

	iret2 = pthread_create( &thread2, NULL, toca_audio, (void*)message2);
	if(iret2){
		fprintf(stderr,"Error ‐ pthread_create() return code: %d\n",iret2);
		exit(EXIT_FAILURE);
	}
	printf("pthread_create() for thread 1 returns: %d\n",iret2);

	iret3 = pthread_create( &thread3, NULL, sound_track, (void*)message3);
	if(iret3){
		fprintf(stderr,"Error ‐ pthread_create() return code: %d\n",iret3);
		exit(EXIT_FAILURE);
	}
	printf("pthread_create() for thread 1 returns: %d\n",iret3);


	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	pthread_join( thread3, NULL);

	
	
}
