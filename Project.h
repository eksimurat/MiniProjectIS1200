//
//  Project.h
//  
//
//  Created by Murat Ekşi on 2018-02-04.
//

#ifndef Project_h
#define Project_h


#endif /* Project_h */

#define EEPROM_ADDR 0x50
extern const uint8_t const WhaleRight[58];
extern const uint8_t const WhaleLeft[58];
extern const uint8_t const GFont[32];
extern const uint8_t const AFont[32];
extern const uint8_t const MFont[32];
extern const uint8_t const EFont[32];
extern const uint8_t const OFont[32];
extern const uint8_t const VFont[32];
extern const uint8_t const RFont[32];
extern const uint8_t const Mask[1];
void game_over_print(int xPos, int yPos, const uint8_t *data, int tag);
void mask_print(int xPos, int yPos, const uint8_t *data);
void gameOver(void);
void clear_textbuffer(void);
void enable_interrupt(void);
void enemy_spawn(void);
void display_bullet(int x,int y,const uint8_t *data);
void print_bullet(void);
void put_bullet(const uint8_t X, const uint8_t Y);
void ship_check(void);
void led_update(void);
void display_string(int line, char *s);
void intro(void);
int getsw(void);
int getbtns();
void check_highscore();
void print_highscore(unsigned int line , unsigned int n);
extern int page_status;
extern char textbuffer[4][16];
extern char c;
extern int game_status;
extern int PortE_cache;
extern int cache_update;
extern unsigned int high_score;
extern volatile int* PortE;
extern rand();
extern int hit;
extern unsigned int high_scores[3];
extern int menu_status;
extern unsigned const int MAX_Y;
extern unsigned const int MAX_X;
extern int slide_switch;
extern unsigned int DIFFICULTY;

extern int main_ship_x;
extern int main_ship_y;
extern int main_ship_health;
extern const uint8_t* main_ship_data;

extern uint8_t SHIPS [6][3];
extern uint8_t INTRO_SHIPS [3][2];
extern uint8_t bullets [1][3];

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
extern uint8_t GAME[128*32];
/* Declare bitmap array containing icon */
extern const uint8_t const Batman[16];
extern const uint8_t const Enemy[16];
extern const uint8_t const Bullet[10];
/* Declare text buffer for display output */
extern char textbuffer[4][16];
