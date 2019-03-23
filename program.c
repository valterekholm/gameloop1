#include <stdio.h>
#include <stdlib.h>//för att använda rand()
#include <time.h>
#include <unistd.h>
#include <locale.h>
#include <ncurses.h>
#define ESC 27
#define BODYMAX 1000
#define BILLION 1000000000L
struct head{
	int x;
	int y;
	char direction;
};

struct body{
	int x;
	int y;
};

struct snake{
	struct head head;
	struct body body[BODYMAX];
	int length;
};


int kbhit (WINDOW * stdsrc)
{
	int ch = wgetch(stdsrc);

	if(ch != ERR){
		ungetch(ch);
		return 1;
	} else {
		return 0;
	}
}

void draw_wall(WINDOW * stdsrc){

	for(int i=0; i<COLS; i++){
		mvprintw(0,i, "\u2592");
		mvprintw(LINES-1,i, "\u2592");
	}
	for(int i=0; i<LINES; i++){
		mvprintw(i, 0, "\u2592");
		mvprintw(i, COLS-1, "\u2592");
	}
}

unsigned get_random_number_for_board(int width, int height, int border_width){
	unsigned tal = rand();
	//man ska skicka in bredd o höjd för spelplanen samt hur bred kanten är
	
	unsigned width_i = width - (border_width*2);
	unsigned height_i = height - (border_width*2);//inre höjden - inner height
	unsigned span = width_i * height_i;//antal möjliga positioner

	unsigned ret = tal % span;
	//mvprintw(19, 10, "Ska få en position i en yta om %dx%d, area: %d, fick %d   ", width_i, height_i, span, ret);
	//mvprintw(20, 10, "width_i: %d height_i: %d Tal: %d, Span: %d, Ret: %d   ", width_i, height_i, tal, span, ret);

	return ret;
	//jag prövar ... att ge ett tal från en "endimensionell array"
}

int pos_1D_to_x(int n, int width){
	return n % width;//här ska man få ut x resp. y från det erhållna talet
}

int pos_1D_to_y(int n, int width){
	int y = n/width;
	//mvprintw(21, 10, "pos_1D_to_y med n %d width %d, y %d", n, width, y);
	return y;//y blir för högt ibland
}

struct body get_new_position(int width, int height, int border_width){
	unsigned n = get_random_number_for_board(width, height, border_width);
	struct body new_pos;
	new_pos.x = pos_1D_to_x(n, width-border_width*2)+border_width;
	new_pos.y = pos_1D_to_y(n, width-border_width*2)+border_width;
	return new_pos;
}


void printw_message(char * message, int deci_sec){
	mvprintw(14, 10, "message");
	mvprintw(15, 10, "%s", message);
	refresh();
	int rest = deci_sec % 10;//10->0 11->1 7->7 21->1
	struct timespec tim1, tim2;
	tim1.tv_sec = deci_sec / 10;
	tim1.tv_nsec = rest*100000000;
	nanosleep(&tim1, &tim2);

}


int is_position_in_snake(struct snake snake, int x, int y, int margin){
	
	char * buffer = malloc(1000);

	//compare with head
	for(int x2=0;x2<margin*2+1; x2++){
		for(int y2=0;y2<margin*2+1; y2++){
			if(x == snake.head.x + x2-margin && y == snake.head.y + y2-margin){
				//sprintf(buffer, "Platsen %d, %d är tagen av huvud \t\t", x, y);
				//printw_message(buffer, 20);
				return 1;
			}
		}
	}

	for(int i=1; i<snake.length; i++){
		for(int x2=0;x2<margin*2+1; x2++){
			for(int y2=0;y2<margin*2+1; y2++){
				//check if x == snake.body[i-1].x + x-margin
				if(x == snake.body[i-1].x + x2-margin
				&& y == snake.body[i-1].y + y2-margin){
					//sprintf(buffer, "Platsen %d, %d är för kroppen \t\t", x, y);
					//printw_message(buffer, 20);
					return 1;
				}//dublettfelet fixat
			}
		}
	}
	//sprintf(buffer, "Platsen %d, %d är inte upptagen \t\t", x, y);
	//printw_message(buffer, 20);

	free(buffer);
	return 0;
}

struct snake init_snake(int board_width, int board_height){
	printw("init_snake\n");
	struct head h;

	h.x = board_width / 2;
	h.y = board_height / 2;
	h.direction = 'r';
	printw("Head x %d, y %d, direction %c \n", h.x, h.y, h.direction);
	struct snake s;
	s.length = 11;
	s.head = h;

	for(int i=1; i<s.length; i++){
		struct body b;

		if(h.direction == 'r'){

			b.x = h.x - i;
			b.y = h.y;

		}
		else{
			//Not implemented
			//exit(1);
		}


		s.body[i-1] = b;
		printw("Adding bodypart x y %d %d ", s.body[i-1].x, s.body[i-1].y);

	}
	return s;
}


int main(int argc, char * argv[]){
	int key_hit, points = 0, count_moves = 0;
	struct timespec tim1, tim2, tim3, tim4, start, end, s_f, klockan;
	double i_m_f = 0.05;// intervallet i sekunder...
	double accum;//users.pja.edu.pl/~jms/qnx

	//get start time
	if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}//Här ingår också error-kontroll

	setlocale(LC_ALL, "");
	srand(time(NULL));//För att initiera rand-funktionen
	WINDOW * stdsrc = initscr();
	cbreak();
	noecho();
	nodelay(stdsrc, TRUE);

	keypad(stdsrc, TRUE);//testar
 
	tim1.tv_sec = 0;
	tim1.tv_nsec = 50000000L;

	tim3.tv_sec = 3;
	tim3.tv_nsec = 0;

	tim4.tv_sec = 0;
	tim4.tv_nsec = 200000000;

	clock_gettime( CLOCK_REALTIME, &s_f);
	
	struct snake snake = init_snake(COLS, LINES);

	struct body temp;
	do{
		temp = get_new_position(COLS, LINES, 2);
	} while(is_position_in_snake(snake, temp.x, temp.y, 20));

		
	struct body food_position = temp;

	curs_set(0);
	clear();
	draw_wall(stdsrc);
	
	int x,y,oldx,oldy, has_eaten = 0;//Ormen ska växa varvet efter den ätit
	while(1){

		clock_gettime( CLOCK_REALTIME, &klockan);
		
		if(kbhit(stdsrc)){//segar ej ner
			key_hit = getch();
			mvprintw(20, 10, "%d", key_hit);
			refresh();
		}
		switch (key_hit){
			case 259://upp
			if(snake.head.direction != 'd')
				snake.head.direction = 'u';
			break;
			case 261://höger
			if(snake.head.direction != 'l')
				snake.head.direction = 'r';
			break;
			case 258: //ner
			if(snake.head.direction != 'u')
				snake.head.direction = 'd';
			break;
			case 260: //vänster
			if(snake.head.direction != 'r')
				snake.head.direction = 'l';
			break;
		}

		if(has_eaten == 1){
			
			snake.length++;
			has_eaten = 0;//återställ
		}
		accum = (double)( klockan.tv_sec - s_f.tv_sec ) + (double)( klockan.tv_nsec - s_f.tv_nsec ) / BILLION;

		if(accum > i_m_f){
			//här börjar uppdatering
			
			clear();//I spelloopen sker nu en rensning av hela skärmen varje "varv"
			draw_wall(stdsrc);//kanterna på planen måste därför ritas om också
			mvprintw(snake.head.y, snake.head.x, "\u2588");
			for(int i=0; i<snake.length-1; i++){
				mvprintw(snake.body[i].y, snake.body[i].x, "\u2588");
			}

			//rita mat
			mvprintw(food_position.y, food_position.x, "\u2588");// %d %d  ", food_position.x, food_position.y);

			//kolla om orm fick mat
			if(food_position.x == snake.head.x && food_position.y == snake.head.y){
				points++;
				has_eaten = 1;
				//food_position = get_new_position(COLS, LINES, 2);
				do{
					temp = get_new_position(COLS, LINES, 2);
				} while(is_position_in_snake(snake, temp.x, temp.y, 20));
				food_position = temp;
			}

			x = snake.head.x;
			y = snake.head.y;//förkortning av kod
			oldx=x;//gamla x, y sparas
			oldy=y;

			switch(snake.head.direction){
				case 'r':
					x++;
					break;
				case 'd':
					y++;
					break;
				case 'l':
					x--;
					break;
				case 'u':
					y--;
					break;
				default: printw("Unknown direction value");
					 //nanosleep(&tim3, &tim2);
					 endwin();
					 return 1;
			}//beroende på riktning ändras x eller y
			if(x == 0 || x == COLS-1 || y == 0 || y == LINES-1){
				//krash
				mvprintw(LINES/2,COLS/2," Krash ");
				refresh();
				//nanosleep(&tim3, &tim2);
				break;//här avbryts spelloopen
			}//krock med väggarna kontrolleras
			if(is_position_in_snake(snake, x, y, 0)){
				mvprintw(LINES/2,COLS/2," Krash ");
				refresh();
				break;
			}

			snake.head.x = x;
			snake.head.y = y;//här återgår man till fulla variablelnamnen

			if(snake.length>2)
			{
				for(int i=snake.length-2; i>0; i--)
				{
					snake.body[i] = snake.body[i-1];
				}//bara om det finns en kropp om minst 2 bitar...
			}
			snake.body[0].x = oldx;//första kropps-biten får huvudets gamla position
			snake.body[0].y = oldy;	
			//Här slutar uppdatering...

			s_f = klockan;//Här sparas denna flytts tidpunkt

			refresh();
			count_moves++;
		}
	}



	endwin();

	//get end time
	if( clock_gettime( CLOCK_REALTIME, &end) == -1) {
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}

	accum = (double)( end.tv_sec - start.tv_sec ) + (double)( end.tv_nsec - start.tv_nsec ) / BILLION;
	printf("Points: %d\n", points);
	printf("Time elapsed: %lf\n ", accum);
	printf("moves: %d\n", count_moves);
	return 0;
}
