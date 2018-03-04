//
//  objects.c
//  
//
//  Created by Murat Ekşi on 2018-02-05.
//

#include "objects.h"
#include "main.h"
#include <pic32mx.h>
#include <stdint.h>
#include "Project.h"
#include "display.h"
#include "game_logic.h"

// Main player ship
int main_ship_x = 5;
int main_ship_y = 0;
int main_ship_health = 8;
const uint8_t* main_ship_data = Batman;




// Enemy ships
/*
 First row: player ship
 Each row: ship
 Column 0 = health
 Column 1 = Position X
 Column 2 = Position Y
 */
uint8_t SHIPS [6][3] = {0};
uint8_t INTRO_SHIPS [3][2];


// Bullets
/*
 maximum 10 bullets
 Each row: Bullet
 Column 0 = Position X
 Column 1 = Position Y
 */

uint8_t bullets [1][3] = {0};



