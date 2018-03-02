//
//  main.c
//  
//
//  Created by Murat Ekşi on 2018-02-04.
//

#include "main.h"
#include <pic32mx.h>
#include <stdint.h>
#include "Project.h"
#include "display.h"
#include "game_logic.h"
#include "objects.h"
#include <stdlib.h>






int buttons;
const int resethigh [3] = {0};

main(void){
    
    init();
    game_status = -1;
    intro();
    game_status = 0;
    slide_switch = getsw();
    read_highscores(high_scores);
    while(1){
        
        while(game_status == 0){
            while(menu_status == 0){
                slide_switch = getsw();
                if((slide_switch >> 3)  != 0){
                    write_highscores(resethigh);
                    read_highscores(high_scores);
                }
                clear_textbuffer();
                clear_screen();
                display_string(2, "     !PLAY!    ");
                display_game(0, GAME);
                delay(10);
                buttons = getbtns();
                if(buttons == 1){
                    delay(50);
                    while(1){
                        if(DIFFICULTY == 2){
                            display_string(2, "     EASY    ");
                        }
                        if(DIFFICULTY == 3){
                            display_string(2, "     MEDIUM    ");
                        }
                        if(DIFFICULTY == 4){
                            display_string(2, "     HARD    ");
                        }
                        buttons = getbtns();
                        delay(20);
                        if(buttons == 4 && DIFFICULTY > 2){
                            DIFFICULTY = DIFFICULTY - 1;
                        }
                        if(buttons == 2 && DIFFICULTY < 4){
                            DIFFICULTY = DIFFICULTY + 1;
                        }
                        if(buttons == 1 ){
                            break;
                        }
                        display_game(0, GAME);
                        delay(10);
                    }
                    clear_textbuffer();
                    menu_status = -1;
                    game_status = 1;
                    led_setup();
                }if(buttons == 2){
                    menu_status = 1;
                }
            }
            while(menu_status == 1){
                clear_screen();
                clear_textbuffer();
                int i;
                display_string(0, "   HIGH SCORES  ");
                for(i = 0; i < 3; i++){
                    print_highscore(i + 1, high_scores[i]);
                }
                display_game(0, GAME);
                delay(10);
                buttons = getbtns();
                if(buttons == 4){
                    menu_status = 0;
                }
            }
        }
        
        while(game_status == 1 & main_ship_health > 0){
            while(slide_switch >> 3 != 1 & main_ship_health > 0){
                slide_switch = getsw();
                buttons = getbtns();
                move(buttons);
                print_ship(main_ship_x,main_ship_y,main_ship_data);
                
                enemy_spawn();
                print_bullet();
                ship_check();
                
                display_game(0, GAME);
                delay(10);
                led_update();
                if(slide_switch >> 3 != 1){
                    clear_screen();
                }
                hit = 0;
                
                while(slide_switch >> 3 == 1){
                    slide_switch = getsw();
                    hit = 1;
                    display_game(0, GAME);
                    led_update();
                }
            }
            if(main_ship_health == 0){
                check_highscore();
                gameOver();
                delay(1000);
                DIFFICULTY = 2;
                game_status = 0;
                menu_status = 0;
                hit = 0;
                main_ship_x = 5;
                main_ship_y = 0;
                main_ship_health = 8;
                clear_textbuffer();
                clear_screen();
                display_string(1, "   !GAME OVER!");
                print_highscore(2, (unsigned int) high_score);
                display_game(0, GAME);
                delay(1000);
                high_score = 0;
            }
    }

    
    }
    }
    

