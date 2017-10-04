#include <vector>
#include "time.cpp"
#include "player.cpp"
#include "bullet.cpp"
#include "stars.cpp"
//#include "bullet.cpp"
//#include "position.cpp"

using namespace std;

#define forI(limit, proc) for(int i=0; i < limit; i++){proc}

class Environment{
	public:
		int MAX_ENEMIES;
		static const int MAX_STARS = 20;
		int fun, die_enemies, stage;
		float t0, speed_shot;
		Pos *bounds;
		Player *player;
		Enemy_Boss *boss;
		vector<Enemy_1 *> enemies1;
		vector<Enemy_2 *> enemies2;
		vector<Enemy_3 *> enemies3;
		vector<Bullet *> bullets;
		vector<Boss_Bullet *> boss_bullets;
		vector<Star *> stars;
		Environment(){
			this->MAX_ENEMIES = 3;
			this->fun = 0;
			this->die_enemies = 0;
			this->stage = 1;
			this->t0 = 0.0f;
			this->speed_shot = 500.0f;
		}
		static Environment* create(Pos *bounds){
			Environment *env = new Environment();
			env->player = new Player(Pos::zero(), bounds);
			env->boss = new Enemy_Boss(Pos::create(0.0f, 170.0f), bounds);
			env->bounds = bounds;
			return env;
		}

		void add_enemy(){
			while((enemies1.size()+enemies2.size())<MAX_ENEMIES){
				Pos *pos;

				while(true){
					Pos *a1 = Pos::mult(*bounds, 0.5f);
					Pos *a2 = Pos::mult(*bounds, -0.5f);
					pos = Pos::random(*a1, *a2);
					if(Pos::distance(*pos,*(player->pos)) < bounds->modulo()/25.0f){
						delete pos;
						delete a1;
						delete a2;
						continue;
					}
					else{
						break;
					}
				}
				int choice = (rand()%3)+1;
				if( choice == 1){
					enemies1.push_back( new Enemy_1(pos, bounds) );
				}
				else if(choice == 2){
					enemies2.push_back( new Enemy_2(pos, bounds) );
				}
				else{
					enemies3.push_back( new Enemy_3(pos, bounds) );
				}
			}
		}

		void add_stars(){
			while(stars.size() < MAX_STARS){
				Pos *pos;
				Pos *a1 = Pos::mult(*bounds, 0.5f);
				Pos *a2 = Pos::mult(*bounds, -0.5f);
				pos = Pos::random(*a1, *a2);
				stars.push_back( new Star(pos, bounds) );
				delete a1;
				delete a2;	
			}
		}

		void add_bullet(){
			Pos *position = Pos::create(player->pos->x, player->pos->y);
			Pos *direction = Pos::create(player->d_shot->x, player->d_shot->y);

			Pos *perpendicular=Pos::mult(*player->d_shot->rot(3.141592/2.0),player->l*1.65);
			Pos *lateral_1=Pos::add(*position,*perpendicular);
			Pos *lateral_2=Pos::sub(*position,*perpendicular);

			if(this->player->a > 1.0f){
				Pos *trash = direction;
				direction = Pos::mult(*direction, this->player->a);
				delete trash;
			}
			//this->bullets.push_back(new Bullet(position, direction, player->ang_shot));
			this->bullets.push_back(new Bullet(lateral_1,direction, player->ang_shot,1));
			this->bullets.push_back(new Bullet(lateral_2,direction, player->ang_shot,2));
		}

		void add_bossBullet(){
			if(this->t0 == 0){
				Pos *position = Pos::create(this->boss->pos->x, this->boss->pos->y);
			 	this->boss_bullets.push_back(new Boss_Bullet(position));
			 	t0 = Time::getTime();	

			}
			else if((Time::getTime()-t0) >= this->speed_shot){
				Pos *position = Pos::create(this->boss->pos->x, this->boss->pos->y);
			 	this->boss_bullets.push_back(new Boss_Bullet(position));
			 	t0 = Time::getTime();
			 }
		}

		void setNextStage(){
			this->die_enemies = 0;
			this->player->life = this->player->life_base;
			this->stage++;
			if(this->stage==5){
				this->fun = 4;
			}
			this->MAX_ENEMIES = this->MAX_ENEMIES+2;
		}

		void upPlayer(int op){
			if(op ==1){
				this->player->upPower();
			}
			else{
				this->player->upHP();
			}
		}

		void reset(){
			this->player->life_base = 100.0f;
			this->player->life = 100.0f;
			this->player->power_shot = 5.0f;
			this->enemies1.clear();
			this->enemies2.clear();
			this->enemies3.clear();
			this->bullets.clear();
			this->boss_bullets.clear();
			this->boss->life = 500.0f;
			this->MAX_ENEMIES = 5;
			this->fun = 0;
			this->die_enemies = 0;
			this->stage = 1;

		}

		bool verificaDie(){
			return (this->player->life <= 0);
		}

		void run_game(int mov_L, int mov_A, int mov_Acel){
			if(fun == 0){
				add_stars();
			    
			}
			else if(fun == 1){

				if(this->verificaDie()){
					this->fun = 5;
				}
				if(mov_L==1)player->move_forward();
			    if(mov_L==-1)player->move_forward();
			    if(mov_A==1)player->rotate_left();
			    if(mov_A==-1)player->rotate_right();
			    if(mov_Acel==1)player->acelera();
			    if(mov_Acel==-1)player->desacelera();
					//forI(bullets.size(), bullets[i]->move());
					//forI(enemies.size(), enemies[i]->move());
				add_enemy();
				forI(enemies1.size(), enemies1[i]->move_forward(););
				forI(enemies2.size(), enemies2[i]->move_forward(););
				forI(enemies3.size(), enemies3[i]->move_forward(););
				forI(bullets.size(), bullets[i]->move_forward(););
				forI(stars.size(), stars[i]->move_forward(*player->direction););

				player->checaColisao(enemies1, enemies2, enemies3);

				for(int k =0; k<bullets.size(); ++k){
					int check = bullets[k]->checaColisao(this->player->power_shot, enemies1, enemies2, enemies3, *bounds);
					if(check == 2 || check == 1){
						bullets.erase(bullets.begin()+k);
					}
					else if(check == 3){
						bullets.erase(bullets.begin()+k);
						this->die_enemies++;
					}
				}

				if(this->die_enemies >= this->stage*10){
					this->fun = 2;
				}
			}

			else if(fun == 4){

				if(this->verificaDie()){
					this->fun = 5;
				}

				if(mov_L==1)player->move_forward();
			    if(mov_L==-1)player->move_forward();
			    if(mov_A==1)player->rotate_left();
			    if(mov_A==-1)player->rotate_right();
			    if(mov_Acel==1)player->acelera();
			    if(mov_Acel==-1)player->desacelera();

			    add_bossBullet();
			    
			    int check = this->player->checaColisaoBoss(*this->boss->pos, boss_bullets, this->boss->l);
			    
			    if(check == -30){
			    	;
			    }
			    else if(check >= 0){
			    	boss_bullets.erase(boss_bullets.begin()+check);
			    }
			    else{;}

			    for(int k =0; k<bullets.size(); ++k){
					int check = bullets[k]->checaColisaoBoss(this->player->power_shot,  *this->boss->pos, this->boss_bullets, *bounds, this->boss->l);
					if(check == 1 || check == 3){
						bullets.erase(bullets.begin()+k);
					}
					else if(check == 2){
						bullets.erase(bullets.begin()+k);
						this->boss->life = this->boss->life - this->player->power_shot;
						this->boss->hit=true;
					}

				}
			    
			    forI(bullets.size(), bullets[i]->move_forward(););
			    forI(boss_bullets.size(), boss_bullets[i]->move_forward(*this->player->pos););
			    forI(stars.size(), stars[i]->move_forward(*player->direction););

			    this->boss->move_forward();

			    if(this->boss->life <= 0.0f){
			    	this->fun = 6;
			    }

			}
		}
		
		
		
};
