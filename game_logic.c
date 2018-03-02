//
//  game_logic.c
//  
//
//  Created by Murat Ekşi on 2018-02-05.
//

#include "game_logic.h"
#include <pic32mx.h>
#include <stdint.h>
#include "Project.h"
#include "objects.h"
#include <stdbool.h>





uint8_t GAME [128*32] = {0};
char textbuffer[4][16];
unsigned int high_scores[3];
unsigned const int MAX_X = 128;
unsigned const int MAX_Y = 32;
unsigned int position_y = 0;
unsigned int DIFFICULTY = 2;
unsigned int shoot_timer = 1;
unsigned int high_score = 0;
uint8_t score;
unsigned int timeoutcount = 0;
volatile int* E = (volatile int*) 0xbf886100;
volatile int* PortE = (volatile int*) 0xbf886110;
int slide_switch;
int hit = 0;
int k;
int PortE_cache;
int cache_update = 0;
int game_status = 0;
int menu_status = 0;
int page_status = 0;
char c;

void print_highscore(unsigned int line , unsigned int n){
    if(line < 0 || line >= 4)
        return;
    
    int count = 1;
    int number = n;
    int num_char;
    int i = 0;
    
    while(number > 0){
        number = number / 10;
        count = count + 1;
    }
    
    char char_buff[count];
    textbuffer[line][count - 1] = 0;
    
    for (i = 7 + (count - 1)/2; i >= 7 - (count - 1)/2; i--){
        if(n > 0){
        num_char = n % 10;
        num_char = num_char + 0x30;
        n = n / 10;
        textbuffer[line][i] = num_char;
        }
    }
}

int getsw( void ){
    return (0x00000000 | ((PORTD & (0xF00)) >> 8 ));
}

uint8_t ship_x = 0;
uint8_t ship_y = 0;
uint8_t random = 0;
void delay(int k){
    int j = 0;
    while (k > 0){
        for (j = 0; j < 7500; j++){
        }
        k--;
    }
}
void put_ship(int i){
    int j;
    random += 5;
    j = (uint8_t) (128 +(32 * i)) + ((TMR2 * 128) +TMR2 * random) % 32;
    SHIPS[i][0] = 2;
    SHIPS[i][1] = j < 128 ? j : MAX_X + j;
    SHIPS[i][2] = (uint8_t) (((TMR2 * 32 ) % 20) * TMR2) % 20;
    
}

void intro(void){
    INTRO_SHIPS[0][0] = -15;
    INTRO_SHIPS[0][1] = 2;
    INTRO_SHIPS[1][0] = -8;
    INTRO_SHIPS[1][1] = 14;
    INTRO_SHIPS[2][0] = -17;
    INTRO_SHIPS[2][1] = 22;
    hit = 1;
    int i;
    int count = 0;
    while(count < 125){
        for(i = 0; i < 3; i++){
            print_ship(INTRO_SHIPS[i][0],INTRO_SHIPS[i][1],Batman);
            INTRO_SHIPS[i][0]++;
        }
        
        display_game(0, GAME);
        delay(10);
        clear_screen();
        count++;
    }
    game_status = 0;
    display_string(2, "!WHALE INVADERS!");
    display_game(0, GAME);
    delay(5000);
    hit = 0;
    
}

// DONT MESS WITH THIS PART... || WORK IN PROGRESS... || SOMEHOW...
void user_isr( void ){
    if(shoot_timer == 0){
    if(IFS(2) > 0  ){
        timeoutcount++;
        //TMR2 = 0x0;
        IFSCLR(0) = 0x00000100;
    }
    if(timeoutcount == 20){
        shoot_timer = 1;
        timeoutcount = 0;
    }
    }
}

void ship_check(void){
    int i;
    for (i = 1; i < DIFFICULTY + 1; i++ ){
        if ((bullets[0][1] != 0) & (bullets[0][1] + 4 > SHIPS[i][1] & bullets[0][1] < SHIPS[i][1] + 18)&
            (bullets[0][2] + 4 > SHIPS[i][2] - 2 & bullets[0][2] - 2< SHIPS[i][2] + 12)){
            SHIPS[i][0] -= 2;
            SHIPS[i][1] = 255;
            SHIPS[i][2] = 255;
            bullets[0][0] = 0;
            bullets[0][1] = 0;
            bullets[0][2] = 0;
            shoot_timer = 1;
            delay(1);
            high_score += 2;
        }
        if (((main_ship_x + 18  >= SHIPS[i][1] ) & (main_ship_y  >= SHIPS[i][2] & main_ship_y + 16 < SHIPS[i][2] + 18))
            ||
            ((main_ship_x + 18  >= SHIPS[i][1] ) & (main_ship_y + 4 >= SHIPS[i][2] & main_ship_y < SHIPS[i][2] + 4 ))){
            hit = 1;
            SHIPS[i][0] -= 2;
            SHIPS[i][1] = 255;
            SHIPS[i][2] = 255;
            main_ship_health--;
            if (cache_update == 0){
                *PortE = *PortE >> 1;
            }else if (cache_update == 1){
                PortE_cache = PortE_cache >> 1;
            }
            delay(1);
        }
    }
}
void led_update (void){
    slide_switch = getsw();
    if ((slide_switch & 0x1) == 1){
        if (cache_update == 0){
            PortE_cache = *PortE;
        }
        *PortE = 0;
        *PortE = high_score;
        cache_update = 1;
    }
    if (((slide_switch & 0x1) != 1) & cache_update == 1){
        *PortE = 0;
        *PortE = PortE_cache;
        cache_update = 0;
    }
}

void enemy_spawn(void){
        int i = 1;
            for (i = 1; i < DIFFICULTY + 1; i++){
                random+= 2;
                if (SHIPS[i][0] == 0){
                    put_ship(i);
                }else{
                    SHIPS[i][1] = SHIPS[i][1] - 1;
                    SHIPS[i][2] = SHIPS[i][2];
                }
                if (SHIPS[i][1] < -5){
                    SHIPS[i][0] = 0;
                    SHIPS[i][1] = -20;
                    SHIPS[i][2] = -20;
                }else{
                    print_ship(SHIPS[i][1],SHIPS[i][2],Enemy);
                }
            }
    
}
void put_bullet(uint8_t X, uint8_t Y){
    int i;
    for (i = 0; i < 2; i++){
        if (bullets [i][0] == 0){
            bullets [i][0] = 1;
            bullets [i][1] = X + 16;
            bullets [i][2] = Y + 2;
            break;
        }
    }
}
void print_bullet(void){
    int i = 0;
    for (i = 0; i < 2; i++){
        
        if (bullets [i][0] == 1){
            bullets [i][1]++;
            if (bullets[i][1] > 120){
                bullets[i][0] = 0;
                bullets[i][1] = 0;
                bullets[i][2] = 0;
            }
            else {
            display_bullet(bullets[i][1],bullets[i][2],Bullet);
                
            }
        }
    }
}

int getbtns(void){
    
    return (0x00000000 | ((PORTD & 0xE0) >> 5));
}

uint8_t power(uint8_t n, unsigned int k){
    while(k > 0){
        n *= n;
        k--;
    }
    return (n - 1);
}
void led_setup (){
    k = power(2, main_ship_health);
    *PortE = k;
}
move(int buttons){
    if ((DIFFICULTY < 5)){
        if ((DIFFICULTY < 3) & high_score >= 10){
            DIFFICULTY = 3;
        }
        if ((DIFFICULTY < 4) & high_score >= 100){
            DIFFICULTY = 4;
        }
        if ((DIFFICULTY < 5) & high_score >= 1000){
            DIFFICULTY = 5;
        }
    }
    
    timeoutcount++;
    AD1CON1 |= (0x1 << 1);
    while(!(AD1CON1 & (0x1 << 1)));
    while(!(AD1CON1 & 0x1));
    int i = 0;
    if (timeoutcount == 10){
        shoot_timer = 1;
        timeoutcount = 0;
    }
    main_ship_y = (ADC1BUF0 / 1023.0) * 24;
    
    if(buttons != 0 ){
        if (buttons  == 2 & main_ship_x < MAX_X - 16) {
            main_ship_x++;
        }
        if (buttons  == 4 & main_ship_x > 0) {
            main_ship_x--;
        }
        if (buttons  == 1 & shoot_timer == 1) {
            put_bullet(main_ship_x, main_ship_y);
            shoot_timer = 0;
        }
    }
}

void init(void){

    
    /* Buttons from 2 to 4 */
    TRISD = (TRISD | 0x1FC0);
    
    /* LED */
    *E = *E & 0xffffff00;
    *PortE = *PortE & 0xffffff00;

    
    spi_init();
    
    /* Analog input */
    AD1PCFG = ~(1 << 2);
    TRISBSET = (1 << 2);
    AD1CHS = (0x2 << 16);
    AD1CON1 = (0x4 << 8) | (0x7 << 5);
    AD1CON2 = 0x0;
    AD1CON3 |= (0x1 << 15);
    ODCE = 0x0;
    TRISECLR = 0xFF;
    
    /* Turn on ADC */
    AD1CON1 |= (0x1 << 15);
    /* Timer */
    T2CON = 0x0;
    TMR2 = 0x0;
    PR2 = 32150;
    IPCSET(2) = 0x0000000C;
    IPCSET(2) = 0x00000001;
    IFSCLR(0) = 0x00000100;
    IECSET(0) = 0x00000100;
    T2CONSET = 0x8070;
    
    /* Set up i2c */
    I2C1CON = 0x0;
    /* I2C Baud rate should be less than 400 kHz, is generated by dividing
     the 40 MHz peripheral bus clock down */
    I2C1BRG = 0x0C2;
    I2C1STAT = 0x0;
    I2C1CONSET = 1 << 13; //SIDL = 1
    I2C1CONSET = 1 << 15; // ON = 1
    score = I2C1RCV;
    
    
    spi_send_recv();
    display_init();
    //enable_interrupt();
    
}

/* Wait for I2C bus to become idle */
void i2c_idle() {
    while(I2C1CON & 0x1F || I2C1STAT & (1 << 14)); //TRSTAT
}

/* Send one byte on I2C bus, return ack/nack status of transaction */
bool i2c_send(uint8_t data) {
    i2c_idle();
    I2C1TRN = data;
    i2c_idle();
    return !(I2C1STAT & (1 << 15)); //ACKSTAT
}

/* Receive one byte from I2C bus */
uint8_t i2c_recv() {
    i2c_idle();
    I2C1CONSET = 1 << 3; //RCEN = 1
    i2c_idle();
    I2C1STATCLR = 1 << 6; //I2COV = 0
    return I2C1RCV;
}

/* Send acknowledge conditon on the bus */
void i2c_ack() {
    i2c_idle();
    I2C1CONCLR = 1 << 5; //ACKDT = 0
    I2C1CONSET = 1 << 4; //ACKEN = 1
}

/* Send not-acknowledge conditon on the bus */
void i2c_nack() {
    i2c_idle();
    I2C1CONSET = 1 << 5; //ACKDT = 1
    I2C1CONSET = 1 << 4; //ACKEN = 1
}

/* Send start conditon on the bus */
void i2c_start() {
    i2c_idle();
    I2C1CONSET = 1 << 0; //SEN
    i2c_idle();
}

/* Send restart conditon on the bus */
void i2c_restart() {
    i2c_idle();
    I2C1CONSET = 1 << 1; //RSEN
    i2c_idle();
}

/* Send stop conditon on the bus */
void i2c_stop() {
    i2c_idle();
    I2C1CONSET = 1 << 2; //PEN
    i2c_idle();
}


void write_highscores(unsigned int* data){
    int i, j;
    
    do {
            i2c_start();
    } while(!i2c_send((EEPROM_ADDR) << 1));
    
    //i2c_send((EEPROM_ADDR | 0x2));
    
    i2c_send((0x00));
    i2c_send((0x01));
    
        for(i = 0; i <3; i++){
            for(j = 0; j < 4; j++){
                i2c_send((*(data + i) & (0x0ff << j*8)));
                
            }
            }
    /* Send stop condition */
    i2c_stop();
        
}
                         
void read_highscores(unsigned int* data){
    int i, j;
                             
    do {
        i2c_start();
    } while(!i2c_send(((EEPROM_ADDR) << 1)));
    
    i2c_send((0x00));
    i2c_send((0x01));
    
    do {
        i2c_start();
    } while(!i2c_send(((EEPROM_ADDR) << 1) | 1) );
    
    for(i = 0; i <3; i++)  {
        for(j = 0; j < 4; j++){
          score = i2c_recv();
          *(data + i) |= (score << j*8);
            i2c_ack();
                     }
    }
    /* Send stop condition */
    i2c_nack();
    i2c_stop();
                                              
    }
        

void check_highscore(){
    //read_highscores(high_scores);
    int i;
    for(i = 0; i < 3; i++){
    if(high_score >= high_scores[i]){
        if(i != 2){
           high_scores[i + 1] = high_scores[i];
        }
        high_scores[i] = high_score;
        break;
        }
    }
    write_highscores(high_scores);
}

void gameOver(void)
{
    const uint8_t *gameOverTop[5] = {WhaleLeft, GFont, AFont, MFont, EFont}; //array of assets
    const uint8_t *gameOverBottom[5] = {OFont, VFont, EFont, RFont, WhaleRight}; //array of assets
    int numberPosTop[5] = {93, 62, 47, 31, 16}; //position to change, starting position
    int numberPosBottom[5] = {128, 143, 159, 174, 192}; //position to change, starting position
    int x = 0;
    int y = 0;
    int i = 0;
    
    for(x = 0; x < 80 + 16; x++)//112 //0-93
    {
        clear_screen();
        clear_textbuffer();
        for(y = 0; y < 5; y++)//top part
        {
            if(y > 0) //if letters
            {
                game_over_print(x - numberPosTop[y], 0, gameOverTop[y], 0);
            }
            else //if whale
            {
                game_over_print(x - numberPosTop[y], 0, gameOverTop[y], 1);
            }
        }
        
        for(i = 92; i > -1; i--)//62 95 //61 96 //mask width
        {
            if(x + i + 36 < 128)//starting position
            {
                mask_print(x + i + 36, 0, Mask); //top mask
            }
        }
        
        for(y = 0; y < 5; y++)//bottom part
        {
            if(y < 4)//if letters
            {
                game_over_print(numberPosBottom[y] - x , 2, gameOverBottom[y], 0);
            }
            else//if whale
            {
                game_over_print(numberPosBottom[y] - x , 2, gameOverBottom[y], 1);
            }
        }
        
        for(i = 92; i > -1; i--) // start 92 92 long//91 91
        {
            if(i - x > -1)
            {
                mask_print(i - x, 3, Mask); //bottom mask
            }
        }
        delay(5);
        display_game(0, GAME);
    }
    
}







