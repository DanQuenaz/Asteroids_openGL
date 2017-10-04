#include "position.cpp"
#include "enemies.cpp"
#include "bullet.cpp"
#define MAX 41
#define forI(limit, proc) for(int i=0; i < limit; i++){proc}

class Player{
	public:
	double ANG_VEL = 0.0560999*4.0;
	double ang_shot;
	float life, life_base, l; 
	bool hit;
	int n_points;
	int power_shot;
	Pos *pos, *direction, *d_shot;
	Pos *r;
	Pos *points[MAX], *bounds;
	Pos *pointsLife[4];
	float a, lim_a;
	Player(Pos* pos, Pos *bounds){
		this->life_base=100;
		this->life = this->life_base;
		this->power_shot = 5;
		hit=false;
		direction=Pos::create(0.0f, 2.0f);
		d_shot=Pos::create(0.0f, 3.0f);
		ang_shot = 0.0f;
		this->pos=pos;
		this->bounds=bounds;
		l=7.5;
		a = 1.0f;
		n_points=4;
		lim_a=8.0f;

		this->points[0] = Pos::create(-5.0f*l, -5.0f*l);
		this->points[1] = Pos::create(-5.0f*l, 5.0f*l);
		this->points[2] = Pos::create(5.0f*l, 5.0f*l);
		this->points[3] = Pos::create(5.0f*l, -5.0f*l);
		/*/ponta 1
		this->points[0] = Pos::create(-3.0f*l, 5.0f*l);
		this->points[1] = Pos::create(-3.0f*l, 3.0f*l);
		this->points[2] = Pos::create(-4.0f*l, 3.0f*l);
		//ponta 2
		this->points[3] = Pos::create(3.0f*l, 5.0f*l);
		this->points[4] = Pos::create(3.0f*l, 3.0f*l);
		this->points[5] = Pos::create(4.0f*l, 3.0f*l);
		//ponta 3
		this->points[6] = Pos::create(-3.0f*l, -2.0f*l);
		this->points[7] = Pos::create(-3.0f*l, -1.0f*l);
		this->points[8] = Pos::create(-4.0f*l, -1.0f*l);
		//ponta 4
		this->points[9] = Pos::create(3.0f*l, -2.0f*l);
		this->points[10] = Pos::create(3.0f*l, -1.0f*l);
		this->points[11] = Pos::create(4.0f*l, -1.0f*l);
		//asa esquerda
		this->points[12] = Pos::create(-3.0f*l, 3.0f*l);
		this->points[13] = Pos::create(-4.0f*l, 3.0f*l);
		this->points[14] = Pos::create(-5.0f*l, 0.0f*l);
		this->points[15] = Pos::create(-4.0f*l, -1.0f*l);
		this->points[16] = Pos::create(-3.0f*l, -1.0f*l);
		this->points[17] = Pos::create(-3.0f*l, 0.0f*l);
		this->points[18] = Pos::create(-1.0f*l, 0.0f*l);
		this->points[19] = Pos::create(-1.0f*l, 1.0f*l);
		this->points[20] = Pos::create(-3.0f*l, 1.0f*l);
		//asa direita
		this->points[21] = Pos::create(3.0f*l, 3.0f*l);
		this->points[22] = Pos::create(4.0f*l, 3.0f*l);
		this->points[23] = Pos::create(5.0f*l, 0.0f*l);
		this->points[24] = Pos::create(4.0f*l, -1.0f*l);
		this->points[25] = Pos::create(3.0f*l, -1.0f*l);
		this->points[26] = Pos::create(3.0f*l, 0.0f*l);
		this->points[27] = Pos::create(1.0f*l, 0.0f*l);
		this->points[28] = Pos::create(1.0f*l, 1.0f*l);
		this->points[29] = Pos::create(3.0f*l, 1.0f*l);
		//corpo da nave
		this->points[30] = Pos::create(-1.0f*l, 1.0f*l);
		this->points[31] = Pos::create(1.0f*l, 1.0f*l);
		this->points[32] = Pos::create(1.0f*l, -1.0f*l);
		this->points[33] = Pos::create(-1.0f*l, -1.0f*l);
		// frente da nave
		this->points[34] = Pos::create(0.0f*l, 2.0f*l);
		this->points[35] = Pos::create(-1.0f*l, 1.0f*l);
		this->points[36] = Pos::create(1.0f*l, 1.0f*l);
		//  fundo da nave
		this->points[37] = Pos::create(-1.0f*l, -1.0f*l);
		this->points[38] = Pos::create(-0.5f*l, -2.0f*l);
		this->points[39] = Pos::create(0.5f*l, -2.0f*l);
		this->points[40] = Pos::create(1.0f*l, -1.0f*l);
		*/
		
		
	}
	~Player(){
		delete direction;
		delete pos;
	}

	void upPower(){
		this->power_shot = this->power_shot + 5;
	}

	void upHP(){
		this->life_base = this->life_base + 30;
	}
	
	void acelera(){
		direction = Pos::mult(*direction, 1/a);
		a = a+0.5f;
		if(a>lim_a)a=lim_a;
		direction = Pos::mult(*direction, a);
	
	}
	
	void desacelera(){
		if(a-0.2f>0){
			direction = Pos::mult(*direction, 1/a);
			a = a-0.2f;
			direction = Pos::mult(*direction, a);
		}
	}
	
	void move_forward(){
		Pos *trash;
		trash=pos;
		pos = Pos::add(*pos,*direction);
		delete trash;
		trash = pos;
		pos = Pos::verificaPos(*pos,*bounds);
		delete trash;
	}
	void move_backwards(){
		Pos *trash, *i_direction;
		trash=pos;
		i_direction=Pos::mult(*direction,-1.0);
		pos = Pos::add(*pos,*i_direction);
		delete trash;
		delete i_direction;
		trash = pos;
		pos = Pos::verificaPos(*pos,*bounds);
		delete trash;
	}
	void rotate_left(){
		Pos *trash;
		trash = direction;
		ang_shot = (ang_shot-ANG_VEL);
		direction = direction->rot(-1.0*ANG_VEL);
		delete trash;

		trash = d_shot;
		d_shot = d_shot->rot(-1.0*ANG_VEL);
		delete trash;

		forI(n_points,
		trash = points[i];
		points[i] = points[i]->rot(-1.0*ANG_VEL);
		delete trash;
		);
	}
	void rotate_right(){
		Pos *trash;
		trash = direction;
		ang_shot = (ang_shot+ANG_VEL);
		direction = direction->rot(ANG_VEL);
		delete trash;

		trash = d_shot;
		d_shot = d_shot->rot(ANG_VEL);
		delete trash;

		forI(n_points,
		trash = points[i];
		points[i] = points[i]->rot(ANG_VEL);
		delete trash;
		);
	}
	void checaColisao(vector<Enemy_1 *> enemies1, vector<Enemy_2 *> enemies2, vector<Enemy_3 *> enemies3){
		forI(enemies1.size(),
				if(Pos::distance(*pos,*(enemies1[i]->pos))<(0.7*enemies1[i]->l+l*1.0) ){
					life=life-0.5; 
					hit=true;
				}
			);

		forI(enemies2.size(),
				if(Pos::distance(*pos,*(enemies2[i]->pos))<(0.7*enemies2[i]->l+l*1.0) ){
					life=life-0.7; 
					hit=true;
				}
			);
		forI(enemies3.size(),
			if(Pos::distance(*pos,*(enemies3[i]->pos))<(0.7*enemies3[i]->l+l*1.0) ){
				life=life-0.7; 
					hit=true;
			}
		);
	}
	
	int checaColisaoBoss(Pos pb, vector<Boss_Bullet *> boss_bullets, float r){
		if(Pos::distance(*pos, pb)<(0.7*r+l*1.0) ){
			life=life-5.0f;
			hit=true;
			return -30; 
		}
		
		forI(boss_bullets.size(),
			if(Pos::distance(*pos,*(boss_bullets[i]->pos))<(0.7*boss_bullets[i]->l+l*1.0) ){
				life=life-10.0;
				hit=true;
				return i; 
			}
		);
		return -10;
	}
};
