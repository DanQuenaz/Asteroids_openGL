#include <ctime>


class Time{
public:
	Time(){;}

	static float getTime(){
		clock_t aux = clock();
		return (float) (aux *(1000.0f/CLOCKS_PER_SEC));
	}
};