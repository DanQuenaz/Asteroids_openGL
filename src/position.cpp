#ifndef POSITION_C					
#define POSITION_C

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;
#define PI 3.1415926

class Pos{
	public:
		double x, y;
		Pos(double x, double y){
			this->x=x;
			this->y=y;
		}
		double modulo(){
			return sqrt(pow(x,2.0)+pow(y,2.0));
		}
		Pos* rot(double teta){
			return new Pos(x*cos(teta)-y*sin(teta),
				           x*sin(teta)+y*cos(teta));
		}
		static Pos* create(double x, double y){
			return new Pos(x,y);
		}
		static Pos* zero(){
			return new Pos(0.0,0.0);
		}
		static Pos* random(Pos A, Pos B){
			//srand(seed);
			int xo=fmin(A.x,B.x), xdif=fmax(A.x,B.x)-xo;
			int yo=fmin(A.y,B.y), ydif=fmax(A.y,B.y)-yo;

			return new Pos((double)xo+(double)(rand()%(xdif*10))/10.0,(double)yo+(double)(rand()%(ydif*10))/10.0);
		}
		static Pos* add(Pos A, Pos B){
			return new Pos(A.x+B.x,A.y+B.y);
		}
		static Pos* sub(Pos A, Pos B){
			return new Pos(A.x-B.x,A.y-B.y);
		}
		static Pos* mult(Pos P, double m){
			return new Pos(P.x*m,P.y*m);
		}
		static double distance(Pos A, Pos B){
			Pos *sub = Pos::sub(A,B);
			double m = sub->modulo(); 
			delete sub;
			return m;
		}

		static Pos* verificaPos(Pos pos, Pos bounds){
			double x=pos.x, y=pos.y;

			if(pos.x > (bounds.x/2.0)){
				x = pos.x - bounds.x;
			}
			else if(pos.x < (-(bounds.x/2.0))){
				x = pos.x + bounds.x;
			}

			if(pos.y > (bounds.y/2.0)){
				y = pos.y - bounds.y;
			}
			else if(pos.y < (-(bounds.y/2.0))){
				y = pos.y + bounds.y;
			}
			
			return Pos::create(x,y);
		}

		static int verificaBordas(Pos pos, Pos bounds){
			if(pos.x > (bounds.x/2.0)){
				return 1;
			}
			else if(pos.x < (-(bounds.x/2.0))){
				return 2;
			}

			if(pos.y > (bounds.y/2.0)){
				return 0;
			}
			else if(pos.y < (-(bounds.y/2.0))){
				return 0;
			}

			return 0;
		}
		


};

#endif
