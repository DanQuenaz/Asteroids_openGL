#ifndef BULLET_C					
#define BULLET_C

#include "position.cpp"


class Boss_Bullet{

public:
	Pos *pos, *direction, *points[6];
	int n_points;
	float l;

	Boss_Bullet(Pos *position){
		this->pos = position;
		this->direction = Pos::create(0.0f, 0.0f);
		this->n_points = 6;
		this->l = 15.0f;
		this->points[0]=Pos::create(0.0f, 0.5*l);
		this->points[1]=Pos::create(0.5*l, 0.3*l);
		this->points[2]=Pos::create(0.5*l, -0.3*l);
		this->points[3]=Pos::create(0.0f, -0.5*l);
		this->points[4]=Pos::create(-0.5*l, -0.3*l);
		this->points[5]=Pos::create(-0.5*l, 0.3*l);
		
	}

	void move_forward(Pos dir){
		Pos *trash, *trash2, *dif;
		trash = this->pos;
		trash2 = this->direction;
		dif = Pos::sub(dir, *trash);
		float dist = dif->modulo();
		//cout<<dist<<endl;
		this->direction = Pos::mult(*dif, (1.0f/(1.0f+dist)));
		this->pos = Pos::add(*this->pos, *this->direction);
		delete trash;
		delete trash2;
		delete dif;

		for(int k=0; k<this->n_points; ++k){
			Pos *trash = this->points[k];
			this->points[k] = this->points[k]->rot(0.13f);
			delete trash;
		}
	}

	int checaColisao(int ps, vector<Enemy_1 *> &enemies1, vector<Enemy_2 *> &enemies2, vector<Enemy_3 *> &enemies3, Pos bounds){
				
	}
};

class Bullet{

public:
	Pos *position, *direction, *points[4];
	int n_points, type_id;
	float l;

	Bullet(Pos *position, Pos *direction, double ang_shot, int type_id){
		n_points = 4;
		l = 15;
		this->position = position;
		this->direction = Pos::mult(*direction, 1.6f);
		this->points[0]=Pos::create(0.0f, 0.4f*l);
		this->points[1]=Pos::create(0.2f*l, 0.0f);
		this->points[2]=Pos::create(0.0f, -0.2f*l);
		this->points[3]=Pos::create(-0.2f*l, 0.0f);
		this->type_id=type_id;

		for(int k=0; k<this->n_points; ++k){
			Pos *trash = this->points[k];
			this->points[k] = this->points[k]->rot(ang_shot);
			delete trash;
		}
	}

	void move_forward(){
		Pos *trash;
		trash = this->position;
		this->position = Pos::add(*this->position, *this->direction);
		delete trash;
	}

	int checaColisao(int ps, vector<Enemy_1 *> &enemies1, vector<Enemy_2 *> &enemies2, vector<Enemy_3 *> &enemies3, Pos bounds){
		if(this->position->x > bounds.x/2)return 1;
		if(this->position->x < -bounds.x/2) return 1;
		if(this->position->y > bounds.y/2) return 1;
		if(this->position->y < -bounds.y/2) return 1;

		
		forI(enemies1.size(),
			if(Pos::distance(*position,*(enemies1[i]->pos))<(0.7*enemies1[i]->l+l*0.6) ){
				enemies1[i]->life = enemies1[i]->life - ps;
				enemies1[i]->hit=true;
				if(enemies1[i]->life <= 0){
					enemies1.erase(enemies1.begin()+i);
					return 3;
				}
				return 2;
				//delete trash;
			}
		);
		
		forI(enemies2.size(),
			if(Pos::distance(*position,*(enemies2[i]->pos))<(0.7*enemies2[i]->l+l*0.6) ){
				enemies2[i]->life = enemies2[i]->life - ps;
				enemies2[i]->hit=true;
				if(enemies2[i]->life <= 0){
					enemies2.erase(enemies2.begin()+i);
					return 3;
				}
				return 2;
				//delete trash; 
			}
		);
		
		forI(enemies3.size(),
			if(Pos::distance(*position,*(enemies3[i]->pos))<(0.7*enemies3[i]->l+l*0.6) ){
				enemies3[i]->life = enemies3[i]->life - ps;
				enemies3[i]->hit=true;
				if(enemies3[i]->life <= 0){
					enemies3.erase(enemies3.begin()+i);
					return 3;
				}
				return 2;
				//delete trash; 
			}
		);
		
		return 4;		
	}

	int checaColisaoBoss(int ps, Pos bp, vector<Boss_Bullet *> &bb, Pos bounds, float r){
		if(this->position->x > bounds.x/2)return 1;
		if(this->position->x < -bounds.x/2) return 1;
		if(this->position->y > bounds.y/2) return 1;
		if(this->position->y < -bounds.y/2) return 1;

		if(Pos::distance(*this->position, bp) < (1.0f*r + this->l*1.0f))
			return 2;

		forI(bb.size(),
			if(Pos::distance(*this->position,*(bb[i]->pos))<(0.7*(bb[i]->l)+this->l*0.6) ){
				
				bb.erase(bb.begin()+i);
				return 3;
			}
		);
		
		return 4;		
	}
};



#endif
