#include <cstdlib> 
#include <ctime>
#include <stdio.h>
#include "MMSystem.h"

#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)
#define player_died(p_y)\
((p_y > (random_number + 2) * 2 * spike_half_size_y - 0.45  && p_y > (random_number2 + 2) * 2 * spike_half_size_y - 0.45) ||\
(p_y > (random_number + 2) * 2 * spike_half_size_y - 0.45  && p_y < random_number2 * 2 * spike_half_size_y - 0.45) ||\
(p_y < random_number * 2 * spike_half_size_y - 0.45  && p_y > (random_number2 + 2) * 2 * spike_half_size_y - 0.45) ||\
(p_y < random_number * 2 * spike_half_size_y - 0.45  && p_y < random_number2 * 2 * spike_half_size_y - 0.45))

float player_p_y = 0.f;
float player_p_x = 0.f;
float player_dp_x;
float player_dp_y;

float player2_p_y = 0.f;
float player2_p_x = 0.f;
float player2_dp_x;
float player2_dp_y;

float arena_half_size_x = 0.85, arena_half_size_y = 0.49;
float player_half_size_x = 0.03, player_half_size_y = 0.03;
float player_speed = 0.5;
float speed_increment = 1.05;
float spike_half_size_x = 0.025, spike_half_size_y = 0.05;
unsigned int random_number = 5;
unsigned int random_number2 = 7;

unsigned int player_score, player_score_fin;
float flash_time = 0;

enum GameMode {
    GM_MENU,
    GM_GAMEPLAY,
    GM_GAME_OVER,
};

GameMode current_gamemode;
int hot_button_menu, hot_button_game_over;
bool multiplayer = false;

enum GameState {
    PLAYER1_WON,
    PLAYER2_WON,
    TIE,
};

GameState current_gamestate;

bool music_started = false;

void simulate_game(Input * input, float dt, unsigned int *direction) {
    if (flash_time > 0) {
        flash();
        flash_time -= dt;
    }else {
        // background
        clear_screen(0x9E9E9B);
        // arena
        draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xACEADB);

        /*for (float i = -0.8; i < 0.8 ; i += 0.08) {
            draw_spike_down(i, -0.45, 0.04, 0.02, 0x9E9E9B);
            draw_spike_up(i, 0.45, 0.04, 0.02, 0x9E9E9B);
        }*/

        if(current_gamemode == GM_GAMEPLAY) {
            if(!multiplayer) {
                int spike_number = 0;
                for (float i = -0.45 ; i < 0.45 ; i += spike_half_size_y * 2) {
                    if(spike_number != random_number && spike_number != random_number + 1 && spike_number != random_number2 && spike_number != random_number2 + 1) {
                        draw_spike_right(0.83, i, spike_half_size_x, spike_half_size_y, 0x9E9E9B);
                        draw_spike_left(-0.83, i, spike_half_size_x, spike_half_size_y, 0x9E9E9B);
                    }
                    spike_number++;
                }
                float player_ddp_x = 0.f;
                float player_ddp_y = 0.f;
                
                if(*direction == right) player_p_x += player_speed * dt;
                if(*direction == left) player_p_x -= player_speed * dt;
                if(pressed(BUTTON_JUMP)) {
                    player_dp_y += 1.5;
                    PlaySound(TEXT("toy-button-105724.wav"), NULL, SND_FILENAME | SND_ASYNC);
                }

                // gravity
                player_ddp_y -= 2.5;

                player_ddp_x -= player_dp_x * 0.5;
                player_ddp_y -= player_dp_y * 0.5;
                player_p_x += player_dp_x * dt+ player_ddp_x * dt * dt * 0.5f;
                player_p_y += player_dp_y * dt+ player_ddp_y * dt * dt * 0.5f;
                player_dp_x += player_ddp_x * dt;
                player_dp_y += player_ddp_y * dt;

                if(player_p_x + player_half_size_x > arena_half_size_x) {
                    if(player_died(player_p_y)) {
                            flash_time = 0.1;
                            player_p_x = 0;
                            player_p_y = 0;
                            player_score_fin = player_score;
                            player_speed = 0.5;
                            speed_increment = 1.05;
                            current_gamemode = GM_GAME_OVER;
                    } else {
                        player_p_x = arena_half_size_x - player_half_size_x;
                        player_dp_x *= -0.5;
                        *direction = left;
                        if(speed_increment > 1){
                            player_speed *= speed_increment;
                            speed_increment *= 0.999;
                        }
                        player_score++;
                    }
                    srand((unsigned)time(0));
                    random_number = (rand() % 9);
                    random_number2 = (rand() % 9);
                }
                if(player_p_x - player_half_size_x < -arena_half_size_x) {
                    if(player_died(player_p_y)) {
                            flash_time = 0.1;
                            player_p_x = 0;
                            player_p_y = 0;
                            player_score_fin = player_score;
                            player_speed = 0.5;
                            speed_increment = 1.05;
                            current_gamemode = GM_GAME_OVER;
                    } else {
                        player_p_x = -arena_half_size_x + player_half_size_x;
                        player_dp_x *= -0.5;
                        *direction = right;
                        if(speed_increment > 1){
                            player_speed *= speed_increment;
                            speed_increment *= 0.999;
                        }
                        player_score++;
                    }
                    srand((unsigned)time(0));
                    random_number = (rand() % 9);
                    random_number2 = (rand() % 9);
                }
                if(player_p_y + player_half_size_y > arena_half_size_y) {
                    player_p_y = arena_half_size_y - player_half_size_y;
                    player_dp_y *= -0.3;
                }
                if(player_p_y - player_half_size_y < -arena_half_size_y) {
                    player_p_y = -arena_half_size_y + player_half_size_y;
                    player_dp_y *= -1;
                }

                draw_number(player_score, 0, 0.4, 0.01, 0xFF0000);

                // Rendering
                
                if(*direction == left) 
                    draw_character_left(player_p_x, player_p_y, player_half_size_x, player_half_size_y, 0xF8F402);
                else if(*direction == right)
                    draw_character_right(player_p_x, player_p_y, player_half_size_x, player_half_size_y, 0xF8F402);

            } else {
                
                int spike_number = 0;
                for (float i = -0.45 ; i < 0.45 ; i += spike_half_size_y * 2) {
                    if(spike_number != random_number && spike_number != random_number + 1 && spike_number != random_number2 && spike_number != random_number2 + 1) {
                        draw_spike_right(0.83, i, spike_half_size_x, spike_half_size_y, 0x9E9E9B);
                        draw_spike_left(-0.83, i, spike_half_size_x, spike_half_size_y, 0x9E9E9B);
                    }
                    spike_number++;
                }
                float player_ddp_x = 0.f;
                float player_ddp_y = 0.f;
                float player2_ddp_x = 0.f;
                float player2_ddp_y = 0.f;
                
                if(*direction == right) {
                    player_p_x += player_speed * dt;
                    player2_p_x -= player_speed * dt;
                }
                if(*direction == left) {
                    player_p_x -= player_speed * dt;
                    player2_p_x += player_speed * dt;
                }
                if(pressed(BUTTON_JUMP)) {
                    player_dp_y += 1.5;
                    PlaySound(TEXT("toy-button-105724.wav"), NULL, SND_FILENAME | SND_ASYNC);
                }
                if(pressed(BUTTON_JUMP2)) {
                    player2_dp_y += 1.5;
                    PlaySound(TEXT("toy-button-105724.wav"), NULL, SND_FILENAME | SND_ASYNC);
                }

                // gravity
                player_ddp_y -= 2.5;
                player2_ddp_y -= 2.5;

                player_ddp_x -= player_dp_x * 0.5;
                player_ddp_y -= player_dp_y * 0.5;
                player_p_x += player_dp_x * dt+ player_ddp_x * dt * dt * 0.5f;
                player_p_y += player_dp_y * dt+ player_ddp_y * dt * dt * 0.5f;
                player_dp_x += player_ddp_x * dt;
                player_dp_y += player_ddp_y * dt;

                player2_ddp_x -= player2_dp_x * 0.5;
                player2_ddp_y -= player2_dp_y * 0.5;
                player2_p_x += player2_dp_x * dt+ player2_ddp_x * dt * dt * 0.5f;
                player2_p_y += player2_dp_y * dt+ player2_ddp_y * dt * dt * 0.5f;
                player2_dp_x += player2_ddp_x * dt;
                player2_dp_y += player2_ddp_y * dt;

                if(player_p_x + player_half_size_x > arena_half_size_x) {
                    if(player_died(player_p_y) || player_died(player2_p_y)) {
                            if(player_died(player_p_y) && player_died(player2_p_y)) {
                                current_gamestate = TIE;
                            } else if (player_died(player_p_y)) {
                                current_gamestate = PLAYER2_WON;
                            }else if (player_died(player2_p_y)) {
                                current_gamestate = PLAYER1_WON;
                            }
                            flash_time = 0.1;
                            player_p_x = 0;
                            player_p_y = 0;
                            player2_p_x = 0;
                            player2_p_y = 0;
                            //player_score = 0;
                            player_speed = 0.5;
                            speed_increment = 1.05;
                            current_gamemode = GM_GAME_OVER;

                    } else {
                        player_p_x = arena_half_size_x - player_half_size_x;
                        player_dp_x *= -0.5;
                        player2_p_x = -arena_half_size_x + player_half_size_x;
                        player2_dp_x *= -0.5;
                        *direction = left;
                        if(speed_increment > 1){
                            player_speed *= speed_increment;
                            speed_increment *= 0.999;
                        }
                        player_score++;
                    }
                    srand((unsigned)time(0));
                    random_number = (rand() % 9);
                    random_number2 = (rand() % 9);
                }
                if(player_p_x - player_half_size_x < -arena_half_size_x) {
                    if(player_died(player_p_y) || player_died(player2_p_y)) {
                        
                        if(player_died(player_p_y) && player_died(player2_p_y)) {
                                current_gamestate = TIE;
                            } else if (player_died(player_p_y)) {
                                current_gamestate = PLAYER2_WON;
                            }else if (player_died(player2_p_y)) {
                                current_gamestate = PLAYER1_WON;
                            }
                            flash_time = 0.1;
                            player_p_x = 0;
                            player_p_y = 0;
                            player2_p_x = 0;
                            player2_p_y = 0;
                            //player_score = 0;
                            player_speed = 0.5;
                            speed_increment = 1.05;
                            current_gamemode = GM_GAME_OVER;

                    } else {
                        player_p_x = -arena_half_size_x + player_half_size_x;
                        player_dp_x *= -0.5;
                        player2_p_x = arena_half_size_x - player_half_size_x;
                        player2_dp_x *= -0.5;
                        *direction = right;
                        if(speed_increment > 1){
                            player_speed *= speed_increment;
                            speed_increment *= 0.999;
                        }
                        player_score++;
                    }
                    srand((unsigned)time(0));
                    random_number = (rand() % 9);
                    random_number2 = (rand() % 9);
                }
                if(player_p_y + player_half_size_y > arena_half_size_y) {
                    player_p_y = arena_half_size_y - player_half_size_y;
                    player_dp_y *= -0.3;
                }
                if(player_p_y - player_half_size_y < -arena_half_size_y) {
                    player_p_y = -arena_half_size_y + player_half_size_y;
                    player_dp_y *= -1;
                }
                if(player2_p_y + player_half_size_y > arena_half_size_y) {
                    player2_p_y = arena_half_size_y - player_half_size_y;
                    player2_dp_y *= -0.3;
                }
                if(player2_p_y - player_half_size_y < -arena_half_size_y) {
                    player2_p_y = -arena_half_size_y + player_half_size_y;
                    player2_dp_y *= -1;
                }
                draw_number(player_score, 0, 0.4, 0.01, 0xFF0000);

                // Rendering
                
                if(*direction == left) {
                    draw_character_left(player_p_x, player_p_y, player_half_size_x, player_half_size_y, 0xF8F402);
                    draw_character_right(player2_p_x, player2_p_y, player_half_size_x, player_half_size_y, 0x001BFF);
                } else if(*direction == right){
                    draw_character_right(player_p_x, player_p_y, player_half_size_x, player_half_size_y, 0xF8F402);
                    draw_character_left(player2_p_x, player2_p_y, player_half_size_x, player_half_size_y, 0x001BFF);
                }
                
            }

        }else if (current_gamemode == GM_MENU) {
            if(!music_started) {
                PlaySound(TEXT("puzzle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                music_started = true;
            }

            if(pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
                hot_button_menu = !hot_button_menu;
            }

            if(hot_button_menu == 0) {
                draw_text("SINGLE PLAYER", -0.65, -0.2, 0.008, 0xff0000);
                draw_text("MULTIPLAYER", 0.15, -0.2, 0.008, 0x9E9E9B);
                multiplayer = false;
            } else {
                draw_text("SINGLE PLAYER", -0.65, -0.2, 0.008, 0x9E9E9B);
                draw_text("MULTIPLAYER", 0.15, -0.2, 0.008, 0xff0000);
                multiplayer = true;
            }
            draw_text("BOUNCY BIRD", -0.53, 0.3, 0.017, 0xFF6800);
            if(pressed(BUTTON_ENTER)) {
                current_gamemode = GM_GAMEPLAY;
                music_started = false;
                PlaySound(TEXT("toy-button-105724.wav"), NULL, SND_FILENAME | SND_ASYNC);
            }
        } else if(current_gamemode = GM_GAME_OVER) {
            if(music_started == false) {
                PlaySound(TEXT("puzzle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                music_started = true;
            }

            if(pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
                hot_button_game_over = !hot_button_game_over;
            }

            if(hot_button_game_over == 0) {
                draw_text("PLAY AGAIN", -0.7, -0.2, 0.01, 0xff0000);
                draw_text("MAIN MENU", 0.15, -0.2, 0.01, 0x9E9E9B);
            } else {
                draw_text("PLAY AGAIN", -0.7, -0.2, 0.01, 0x9E9E9B);
                draw_text("MAIN MENU", 0.15, -0.2, 0.01, 0xff0000);
            }
            draw_text("GAME OVER", -0.53, 0.3, 0.02, 0x000000);
            if(!multiplayer) {
                draw_text("SCORE:", -0.5, 0.05, 0.02, 0xFF000C);
                draw_number(player_score_fin, 0.35, -0.008, 0.028, 0xFF000C);
            } else {
                if(current_gamestate == TIE) {
                    draw_text("TIE", -0.15, 0.05, 0.02, 0xFF000C);
                } else {
                    draw_text("PLAYER", -0.65, 0.05, 0.02, 0xFF000C);
                    if(current_gamestate == PLAYER1_WON) {
                        draw_number(1, 0.15, -0.008, 0.028, 0xFF000C);
                    } else {
                        draw_number(2, 0.15, -0.008, 0.028, 0xFF000C);
                    }
                    draw_text("WON", 0.3, 0.05, 0.02, 0xFF000C);
                }
            }

            player_score = 0;
            if(pressed(BUTTON_ENTER)) {
                if(hot_button_game_over == 0) {
                    current_gamemode = GM_GAMEPLAY;
                    music_started = false;
                    PlaySound(TEXT("toy-button-105724.wav"), NULL, SND_FILENAME | SND_ASYNC);
                } else {
                    current_gamemode = GM_MENU;
                }
            }
        }

    }
    
}
