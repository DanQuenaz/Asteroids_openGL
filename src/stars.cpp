#include "position.cpp"

class Star{

public:
	Pos *pos, *direction, *points[4];
	Pos *bounds;
	int n_points;
	float l;

	Star(Pos *position, Pos *bnds){
		n_points = 4;
		l = 3.5;
		this->bounds = bnds;
		this->pos = position;
		this->direction = Pos::mult(*direction, 2.0f);
		this->points[0]=Pos::create(0.0f, 0.4f*l);
		this->points[1]=Pos::create(0.3f*l, 0.0f);
		this->points[2]=Pos::create(0.0f, -0.4f*l);
		this->points[3]=Pos::create(-0.3f*l, 0.0f);
	}

	void move_forward(Pos direction){
		Pos *trash, *trash2;
		trash = this->pos;
		trash2 = Pos::mult(direction, -1.0f);
		this->pos = Pos::add(*this->pos, *trash2);
		Pos *trash3 = this->pos;
		this->pos = Pos::verificaPos(*this->pos, *bounds);
		delete trash;
		delete trash2;
		delete trash3;
	}
};