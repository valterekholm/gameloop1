#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define BILL 1E9

int is_timespec_b_greater_than_a(struct timespec a, struct timespec b){

	if(b.tv_sec > a.tv_sec){
		return 1;
	}
	else if(b.tv_sec == a.tv_sec){
		if(b.tv_nsec > a.tv_nsec){
			return 1;
		}
	}
	return 0;


}

long timespec_to_ms(struct timespec tim){
	return (long)(tim.tv_sec * 1000 + (tim.tv_nsec / 1000000L));
}

int timespec_to_deci(struct timespec tim){
	return tim.tv_sec * 100 + (tim.tv_nsec / 10000000);
}

long get_clock_ms(){
	struct timespec temp;

	if (clock_gettime(CLOCK_REALTIME, &temp) == -1) return -1;
	return timespec_to_ms(temp);
}

long get_clock_deci(){//100-fractions of seconds
	struct timespec temp;
	
	if (clock_gettime(CLOCK_REALTIME, &temp) == -1) return -1;
	return timespec_to_deci(temp);
}



struct timespec add_ms(struct timespec tim, int milliseconds){

	int sec = tim.tv_sec;
	int ms1 = tim.tv_nsec / 1000000;

	int add_sec = milliseconds / 1000; //if above 1000, > 0
	milliseconds = milliseconds % 1000; //limit to 0-999

	if(ms1 + milliseconds >= 1000){
		sec++;
	}

	sec += add_sec;

	ms1 = (ms1 + milliseconds) % 1000;

	struct timespec new;

	new.tv_sec = sec;
	new.tv_nsec = ms1 * 1000000;

	return new;

}

int kbhit ()
{
	int ch = getch();

	if(ch != ERR){
		ungetch(ch);
		return 1;
	} else {
		return 0;
	}
} 
int main(){
	struct timespec start, end, klockan, s_f;
	double accum, comp;
	int count = 0, diff=0, c1, c2, interval_ms = 50, interval_deci = 5;
	long clock, lasttime;

	//printf("Game loop 1 start, %ld\n", get_clock_ms());
	//sleep(1);
	WINDOW * stds = initscr();
	//timeout(100);

	/*for non-breaking key input*/
	cbreak();
	noecho();
	nodelay(stds, TRUE);
	clear();
	clock_gettime(CLOCK_REALTIME, &start);

	if (clock_gettime(CLOCK_REALTIME, &s_f)== -1) printf("error1");

	lasttime = get_clock_deci();


	while(1){
		clock = get_clock_deci();
		printw("%ld ", clock);

		if(clock > lasttime + interval_deci){
			count++;
			lasttime = clock;
			refresh();
		}

		if(kbhit()){
			printw("                              ");
			break;
		}

	}

	clock_gettime(CLOCK_REALTIME, &end);

	accum = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILL;

	endwin();
	printf("\n   %lf %d\t%d %ld\t%ld   \n", accum, count, interval_ms, lasttime, clock);

	//printf("Game loop 1 end, %ld\n", get_clock_ms());
	return 0;
}
