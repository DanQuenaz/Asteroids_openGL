#include "position.cpp"
#define MAX_E 30
#define forI(limit, proc) for(int i=0; i < limit; i++){proc}


class Enemy{
	public:
	float life, lifemax; 
	bool hit;
	int n_points;
	int enemie_t;
	int texture_id;
	Pos *pos;
	Pos *points[MAX_E];

	Pos *bounds;

	Enemy(Pos *bnds){
		bounds = bnds;
		this->texture_id = rand()%3+3;
		hit=false;
	}

	~Enemy(){
		delete pos;
	}	
};

class Enemy_1 : public Enemy{
	public:
	float t,l;
	Enemy_1(Pos *ep, Pos *bnds) : Enemy(bnds){
		this->life = this->lifemax = 30;
		this->n_points = 6;
		l = (rand()%20)+15;
		this->pos = ep;
		this->points[0]=Pos::create(0.0f, 0.5*l);
		this->points[1]=Pos::create(0.5*l, 0.3*l);
		this->points[2]=Pos::create(0.5*l, -0.3*l);
		this->points[3]=Pos::create(0.0f, -0.5*l);
		this->points[4]=Pos::create(-0.5*l, -0.3*l);
		this->points[5]=Pos::create(-0.5*l, 0.3*l);
		t = 0.0f;
	}
	
	void move_forward(){

		float x1 = 1.5f;
		float y1 = sin(t);
		
		Pos *trash, *aux;
		aux = new Pos(x1, y1);
		trash=pos;
		this->pos = Pos::add(*pos,*aux);
		this->pos = Pos::verificaPos(*pos, *(this->bounds));
		delete trash;

		t = t + 0.01f;	

		if(t >= 10000.0f) t =0.0f;

		for(int k=0; k<this->n_points; k++){
			trash = this->points[k];
			this->points[k] = this->points[k]->rot(0.13);
			delete trash;
		}
	}
};

class Enemy_2 : public Enemy{
	public:
	float t,l;
	Enemy_2(Pos *ep, Pos *bnds) : Enemy(bnds){
		this->life = this->lifemax = 40;
		this->n_points = 6;
		l = (rand()%20)+15;
		this->pos = ep;
		this->points[0]=Pos::create(0.0f, 0.5*l);
		this->points[1]=Pos::create(0.5*l, 0.3*l);
		this->points[2]=Pos::create(0.5*l, -0.3*l);
		this->points[3]=Pos::create(0.0f, -0.5*l);
		this->points[4]=Pos::create(-0.5*l, -0.3*l);
		this->points[5]=Pos::create(-0.5*l, 0.3*l);
		t = 0.0f;
	}
	
	void move_forward(){
		
		float x1 = -1.0f;
		float y1 = 5.0f*sin(t);

		Pos *trash, *aux;
		aux = new Pos(x1, y1);
		trash=pos;
		this->pos = Pos::add(*pos,*aux);
		this->pos = Pos::verificaPos(*pos, *(this->bounds));
		delete trash;

		t = t + 0.01f;	

		if(t >= 10000.0f) t =0.0f;

		for(int k=0; k<this->n_points; k++){
			trash = this->points[k];
			this->points[k] = this->points[k]->rot(0.13);
			delete trash;
		}
	}
};

class Enemy_3 : public Enemy{
	public:
	float t,l;
	Enemy_3(Pos *ep, Pos *bnds) : Enemy(bnds){
		this->life = this->lifemax = 20;
		this->n_points = 6;
		l = (rand()%20)+15;
		this->pos = ep;
		this->points[0]=Pos::create(0.0f, 0.5*l);
		this->points[1]=Pos::create(0.5*l, 0.3*l);
		this->points[2]=Pos::create(0.5*l, -0.3*l);
		this->points[3]=Pos::create(0.0f, -0.5*l);
		this->points[4]=Pos::create(-0.5*l, -0.3*l);
		this->points[5]=Pos::create(-0.5*l, 0.3*l);
		t = 0.0f;
	}
	
	void move_forward(){
		
		float y1 = 1.0f;
		float x1 = 3.0f*cos(t);

		Pos *trash, *aux;
		aux = new Pos(x1, y1);
		trash=pos;
		this->pos = Pos::add(*pos,*aux);
		this->pos = Pos::verificaPos(*pos, *(this->bounds));
		delete trash;

		t = t + 0.01f;	

		if(t >= 10000.0f) t =0.0f;

		for(int k=0; k<this->n_points; k++){
			trash = this->points[k];
			this->points[k] = this->points[k]->rot(0.13);
			delete trash;
		}

	}
};

class Enemy_Boss : public Enemy{
	public:
	float t,l, r;
	Pos *mov;
	Enemy_Boss(Pos *ep, Pos *bnds) : Enemy(bnds){
		this->mov = new Pos(-1.0f, 0.0f);
		this->life = this->lifemax = 500.0f;
		this->n_points = 6;
		this->l = 35;
		this->r = 0.05;
		this->pos = ep;
		//corpo do boss
		this->points[0]=Pos::create(0.0f, 1.0*l);
		this->points[1]=Pos::create(1.0*l, 0.5*l);
		this->points[2]=Pos::create(1.0*l, -0.5*l);
		this->points[3]=Pos::create(0.0f, -1.0*l);
		this->points[4]=Pos::create(-1.0*l, -0.5*l);
		this->points[5]=Pos::create(-1.0*l, 0.5*l);
		//rosto do boss
		/*this->points[4]=Pos::create(-18.5f, 20.0f);
		this->points[5]=Pos::create(-11.5f, 20.0f);
		this->points[6]=Pos::create(-15.5f, 10.0f);

		this->points[7]=Pos::create(18.5f, 20.0f);
		this->points[8]=Pos::create(11.5, 20.0f);
		this->points[9]=Pos::create(15.5f, 10.0f);

		this->points[10]=Pos::create(0.0f, 2.5f);
		this->points[11]=Pos::create(2.5f, 0.0f);
		this->points[12]=Pos::create(0.0f, -2.5f);
		this->points[13]=Pos::create(-2.5f, 0.0f);

		this->points[14]=Pos::create(-10.0f, -6.0f);
		this->points[15]=Pos::create(10.0f, -6.0f);
		this->points[16]=Pos::create(10.0f, -11.0f);
		this->points[17]=Pos::create(-10.0f, -11.0f);*/
		

		//
		t = 0.0f;
	}
	
	void move_forward(){
		this->mov->y = 0.2f*cos(t);
		Pos *trash;
		if(Pos::verificaBordas(*(this->pos), *(this->bounds)) == 1){
			this->mov->x = -1.0f;
			this->r = -this->r;
		}
		else if(Pos::verificaBordas(*(this->pos), *(this->bounds)) == 2){
			this->mov->x = 1.0f;
			this->r = -this->r;
		}

		trash=pos;
		this->pos = Pos::add(*this->pos,*this->mov);
		delete trash;

		for(int k=0; k<6; k++){
			trash = this->points[k];
			this->points[k] = this->points[k]->rot(this->r);
			delete trash;
		}

		t = t + 0.01f;	
		if(t >= 10000.0f) t =0.0f;
	}
};
