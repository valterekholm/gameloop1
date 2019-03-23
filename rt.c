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
	return tim.tv_sec * 1000 + (tim.tv_nsec / 1000000L);
}

long get_clock_ms(){
	struct timespec temp;
	if (clock_gettime(CLOCK_REALTIME, &temp) == -1) return -1;

	return timespec_to_ms(temp);
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

int kbhit (WINDOW * stds)
{
	int ch = wgetch(stds);

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
	int count = 0, diff=0, c1, c2, interval_ms = 50;
	long clock, lasttime;

	//printf("Game loop 1 start, %ld\n", get_clock_ms());
	//sleep(1);
	WINDOW * stds = initscr();
	clear();
	clock_gettime(CLOCK_REALTIME, &start);

	if (clock_gettime(CLOCK_REALTIME, &s_f)== -1) printf("error1");

	lasttime = get_clock_ms();


	while(1){
		clock = lasttime;
		printw("%ld ", clock);
		refresh();

		if(clock > lasttime + interval_ms){
			count++;
			lasttime = clock;
		}

		if(kbhit(stds)){
			break;
		}

	}

	clock_gettime(CLOCK_REALTIME, &end);

	accum = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILL;

	endwin();
	printf("%lf %d\t%d %ld\t%ld\n", accum, count, interval_ms, lasttime, clock);

	//printf("Game loop 1 end, %ld\n", get_clock_ms());
	return 0;
}
