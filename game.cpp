#include <cstdlib> 
#include <ctime>

#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_p_y = 0.f;
float player_p_x = 0.f;
float player_dp_x;
float player_dp_y;
float arena_half_size_x = 0.85, arena_half_size_y = 0.49;
float player_half_size_x = 0.03, player_half_size_y = 0.03;
float player_speed = 0.002;
float speed_increment = 1.075;
float spike_half_size_x = 0.025, spike_half_size_y = 0.05;
unsigned int random_number = 5;
unsigned int random_number2 = 7;

unsigned int player_score;
float flash_time = 0;

void simulate_game(Input * input, float dt, unsigned int *direction) {
    if (flash_time > 0) {
        flash();
        flash_time -= dt;
    }else {
        clear_screen(0x9E9E9B);
        draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xACEADB);
        /*for (float i = -0.8; i < 0.8 ; i += 0.08) {
            draw_spike_down(i, -0.45, 0.04, 0.02, 0x9E9E9B);
            draw_spike_up(i, 0.45, 0.04, 0.02, 0x9E9E9B);
        }*/
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
        if(is_down(BUTTON_UP)) player_ddp_y += 20;
        if(is_down(BUTTON_DOWN)) player_ddp_y -= 20;
        //if(is_down(BUTTON_LEFT)) player_ddp_x -= 20;
        //if(is_down(BUTTON_RIGHT)) player_ddp_x += 20;
        if(*direction == right) player_p_x += player_speed;
        if(*direction == left) player_p_x -= player_speed;
        if(pressed(BUTTON_JUMP)) player_ddp_y += 10 * 20;
        player_ddp_y -= 2.5;

        player_ddp_x -= player_dp_x * 0.5;
        player_ddp_y -= player_dp_y * 0.5;
        player_p_x += player_dp_x * dt+ player_ddp_x * dt * dt * 0.5f;
        player_p_y += player_dp_y * dt+ player_ddp_y * dt * dt * 0.5f;
        player_dp_x += player_ddp_x * dt;
        player_dp_y += player_ddp_y * dt;

        if(player_p_x + player_half_size_x > arena_half_size_x) {
            if((player_p_y + 0.01 > (random_number + 2) * 2 * spike_half_size_y - 0.45  && player_p_y + 0.01 > (random_number2 + 2) * 2 * spike_half_size_y - 0.45) ||
            (player_p_y + 0.01 > (random_number + 2) * 2 * spike_half_size_y - 0.45  && player_p_y - 0.01 < random_number2 * 2 * spike_half_size_y - 0.45) ||
            (player_p_y - 0.01 < random_number * 2 * spike_half_size_y - 0.45  && player_p_y + 0.01 > (random_number2 + 2) * 2 * spike_half_size_y - 0.45) ||
            (player_p_y - 0.01 < random_number * 2 * spike_half_size_y - 0.45  && player_p_y - 0.01 < random_number2 * 2 * spike_half_size_y - 0.45)) {
                    flash_time = 0.1;
                    player_p_x = 0;
                    player_p_y = 0;
                    player_score = 0;
            } else {
                player_p_x = arena_half_size_x - player_half_size_x;
                player_dp_x *= -0.5;
                *direction = left;
                if(speed_increment > 1){
                    player_speed *= speed_increment;
                    speed_increment *= 0.997;
                }
                player_score++;
            }
            srand((unsigned)time(0));
            random_number = (rand() % 9);
            random_number2 = (rand() % 9);
        }
        if(player_p_x - player_half_size_x < -arena_half_size_x) {
            if((player_p_y + 0.01 > (random_number + 2) * 2 * spike_half_size_y - 0.45  && player_p_y + 0.01 > (random_number2 + 2) * 2 * spike_half_size_y - 0.45) ||
            (player_p_y + 0.01 > (random_number + 2) * 2 * spike_half_size_y - 0.45  && player_p_y - 0.01 < random_number2 * 2 * spike_half_size_y - 0.45) ||
            (player_p_y - 0.01 < random_number * 2 * spike_half_size_y - 0.45  && player_p_y + 0.01 > (random_number2 + 2) * 2 * spike_half_size_y - 0.45) ||
            (player_p_y - 0.01 < random_number * 2 * spike_half_size_y - 0.45  && player_p_y - 0.01 < random_number2 * 2 * spike_half_size_y - 0.45)) {
                    flash_time = 0.1;
                    player_p_x = 0;
                    player_p_y = 0;
                    player_score = 0;
            } else {
                player_p_x = -arena_half_size_x + player_half_size_x;
                player_dp_x *= -0.5;
                *direction = right;
                if(speed_increment > 1){
                    player_speed *= speed_increment;
                    speed_increment *= 0.997;
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
            draw_character_left(player_p_x, player_p_y, player_half_size_x, player_half_size_y);
        else if(*direction == right)
            draw_character_right(player_p_x, player_p_y, player_half_size_x, player_half_size_y);

    }
    
}