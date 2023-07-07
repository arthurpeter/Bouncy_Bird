

void render_background() {
    unsigned int* pixel = (unsigned int*)buffer_memory;
    for (int y = 0 ; y < buffer_heigth ; y++) {
        for (int x = 0 ; x < buffer_width ; x++) {
            *pixel++ = 0xACEADB;
        }
    }
}

void clear_screen(unsigned int color) {
    unsigned int* pixel = (unsigned int*)buffer_memory;
    for (int y = 0 ; y < buffer_heigth ; y++) {
        for (int x = 0 ; x < buffer_width ; x++) {
            *pixel++ = color;
        }
    }
}

void flash() {
    clear_screen(0xFFFFFF);
}

int clamp(int min, int value, int max) {
    if(value < min) return min;
    if(value > max) return max;
    return value;
}

void draw_rect_in_pixels(int x0, int y0, int x1, int y1, unsigned int color) {
    x0 = clamp(0, x0, buffer_width);
    x1 = clamp(0, x1, buffer_width);
    y0 = clamp(0, y0, buffer_heigth);
    y1 = clamp(0, y1, buffer_heigth);

   // unsigned int* pixel = (unsigned int*)buffer_memory;
    for (int y = y0 ; y < y1 ; y++) {
        unsigned int* pixel = (unsigned int*)buffer_memory + x0 + y * buffer_width;
        for (int x = x0 ; x < x1 ; x++) {
            *pixel++ = color;
        }
    }
}

void draw_rect(float x, float y, float half_size_x, float half_size_y, unsigned int color) {
    x *= buffer_heigth;
    y *= buffer_heigth;
    half_size_x *= buffer_heigth;
    half_size_y *= buffer_heigth;

    x += buffer_width / 2.f;
    y += buffer_heigth / 2.f;

    int x0 = x - half_size_x;
    int x1 = x + half_size_x;
    int y0 = y - half_size_y;
    int y1 = y + half_size_y;

    draw_rect_in_pixels(x0, y0, x1, y1, color);
}

void draw_character_right(float x, float y, float half_size_x, float half_size_y) {
    draw_rect(x, y, half_size_x, half_size_y, 0xF8F402);
    draw_rect(x, y + 0.031, half_size_x * 1.1, half_size_y * 0.07, 0x000000);
    draw_rect(x, y - 0.031, half_size_x * 1.1, half_size_y * 0.07, 0x000000);
    draw_rect(x + 0.031, y, half_size_x * 0.07, half_size_y * 1.1, 0x000000);
    draw_rect(x - 0.031, y, half_size_x * 0.07, half_size_y * 1.1, 0x000000);
    draw_rect(x + 0.03, y + 0.005, half_size_x * 0.7, half_size_y * 0.07, 0x000000);
    draw_rect(x + 0.03, y - 0.015, half_size_x * 0.7, half_size_y * 0.07, 0x000000);
    draw_rect(x + 0.05, y - 0.0052, half_size_x * 0.07, half_size_y * 0.36, 0x000000);
    draw_rect(x + 0.01, y - 0.0052, half_size_x * 0.07, half_size_y * 0.36, 0x000000);
    draw_rect(x + 0.03, y - 0.0052, half_size_x * 0.63, half_size_y * 0.32, 0xFF6C3D);
    draw_rect(x - 0.001, y + 0.015, half_size_x * 0.07, half_size_y * 0.5, 0x000000);
    draw_rect(x + 0.015, y, half_size_x * 0.58, half_size_y * 0.07, 0x000000);
    draw_rect(x + 0.031, y + 0.0052, half_size_x * 0.07, half_size_y * 0.1, 0x000000);
    draw_rect(x + 0.0145, y + 0.016, half_size_x * 0.5, half_size_y * 0.47, 0xFFFFFF);
    draw_rect(x + 0.0145, y + 0.016, half_size_x * 0.15, half_size_y * 0.2, 0x000000);
    draw_rect(x + 0.035, y - 0.007, half_size_x * 0.5, half_size_y * 0.07, 0x000000);
    draw_rect(x - 0.03, y + 0.005, half_size_x * 0.6, half_size_y * 0.5, 0x000000);
    draw_rect(x - 0.03, y + 0.005, half_size_x * 0.5, half_size_y * 0.4, 0xFFFDBE);
}

void draw_character_left(float x, float y, float half_size_x, float half_size_y) {
    draw_rect(x, y, half_size_x, half_size_y, 0xF8F402);
    draw_rect(x, y + 0.031, half_size_x * 1.1, half_size_y * 0.07, 0x000000);
    draw_rect(x, y - 0.031, half_size_x * 1.1, half_size_y * 0.07, 0x000000);
    draw_rect(x - 0.031, y, half_size_x * 0.07, half_size_y * 1.1, 0x000000);
    draw_rect(x + 0.031, y, half_size_x * 0.07, half_size_y * 1.1, 0x000000);
    draw_rect(x - 0.03, y + 0.005, half_size_x * 0.7, half_size_y * 0.07, 0x000000);
    draw_rect(x - 0.03, y - 0.015, half_size_x * 0.7, half_size_y * 0.07, 0x000000);
    draw_rect(x - 0.05, y - 0.0052, half_size_x * 0.07, half_size_y * 0.36, 0x000000);
    draw_rect(x - 0.01, y - 0.0052, half_size_x * 0.07, half_size_y * 0.36, 0x000000);
    draw_rect(x - 0.03, y - 0.0052, half_size_x * 0.63, half_size_y * 0.32, 0xFF6C3D);
    draw_rect(x + 0.001, y + 0.015, half_size_x * 0.07, half_size_y * 0.5, 0x000000);
    draw_rect(x - 0.015, y, half_size_x * 0.58, half_size_y * 0.07, 0x000000);
    draw_rect(x - 0.031, y + 0.0052, half_size_x * 0.07, half_size_y * 0.1, 0x000000);
    draw_rect(x - 0.0145, y + 0.016, half_size_x * 0.5, half_size_y * 0.47, 0xFFFFFF);
    draw_rect(x - 0.0145, y + 0.016, half_size_x * 0.15, half_size_y * 0.2, 0x000000);
    draw_rect(x - 0.035, y - 0.007, half_size_x * 0.5, half_size_y * 0.07, 0x000000);
    draw_rect(x + 0.03, y + 0.005, half_size_x * 0.6, half_size_y * 0.5, 0x000000);
    draw_rect(x + 0.03, y + 0.005, half_size_x * 0.5, half_size_y * 0.4, 0xFFFDBE);
}

void draw_spike_down(float x, float y, float half_size_x, float half_size_y, unsigned int color) {
    x *= buffer_heigth;
    y *= buffer_heigth;
    half_size_x *= buffer_heigth;
    half_size_y *= buffer_heigth;

    x += buffer_width / 2.f;
    y += buffer_heigth / 2.f;

    int x0 = x - half_size_x;
    int x1 = x + half_size_x;
    int y0 = y - half_size_y;
    int y1 = y + half_size_y;

    x0 = clamp(0, x0, buffer_width);
    x1 = clamp(0, x1, buffer_width);
    y0 = clamp(0, y0, buffer_heigth);
    y1 = clamp(0, y1, buffer_heigth);

    for (int y = y0 ; y < y1 ; y++) {
        unsigned int* pixel = (unsigned int*)buffer_memory + x0 + y * buffer_width;
        for (int x = x0 ; x < x1 ; x++) {
            if(x >= x0 + (y - y0) && x < x1 - (y - y0)) *pixel = color;
            *pixel++;
        }
    }
}

void draw_spike_up(float x, float y, float half_size_x, float half_size_y, unsigned int color) {
    x *= buffer_heigth;
    y *= buffer_heigth;
    half_size_x *= buffer_heigth;
    half_size_y *= buffer_heigth;

    x += buffer_width / 2.f;
    y += buffer_heigth / 2.f;

    int x0 = x - half_size_x;
    int x1 = x + half_size_x;
    int y0 = y - half_size_y;
    int y1 = y + half_size_y;

    x0 = clamp(0, x0, buffer_width);
    x1 = clamp(0, x1, buffer_width);
    y0 = clamp(0, y0, buffer_heigth);
    y1 = clamp(0, y1, buffer_heigth);

    for (int y = y0 ; y < y1 ; y++) {
        unsigned int* pixel = (unsigned int*)buffer_memory + x0 + y * buffer_width;
        for (int x = x0 ; x < x1 ; x++) {
            if(x >= x0 + (y1 - y) && x <= x1 - (y1 - y)) *pixel = color;
            *pixel++;
        }
    }
}

void draw_spike_left(float x, float y, float half_size_x, float half_size_y, unsigned int color) {
    x *= buffer_heigth;
    y *= buffer_heigth;
    half_size_x *= buffer_heigth;
    half_size_y *= buffer_heigth;

    x += buffer_width / 2.f;
    y += buffer_heigth / 2.f;

    int x0 = x - half_size_x;
    int x1 = x + half_size_x;
    int y0 = y - half_size_y;
    int y1 = y + half_size_y;

    x0 = clamp(0, x0, buffer_width);
    x1 = clamp(0, x1, buffer_width);
    y0 = clamp(0, y0, buffer_heigth);
    y1 = clamp(0, y1, buffer_heigth);

    for (int y = y0 ; y < y1 ; y++) {
        unsigned int* pixel = (unsigned int*)buffer_memory + x0 + y * buffer_width;
        for (int x = x0 ; x < x1 ; x++) {
            if(y > y0 + (x - x0) && y < y1 - (x - x0)) *pixel = color;
            else if (y == y0 + (x - x0) && y == y1 - (x - x0)) *pixel = 0x000000;
            *pixel++;
        }
    }
}

void draw_spike_right(float x, float y, float half_size_x, float half_size_y, unsigned int color) {
    x *= buffer_heigth;
    y *= buffer_heigth;
    half_size_x *= buffer_heigth;
    half_size_y *= buffer_heigth;

    x += buffer_width / 2.f;
    y += buffer_heigth / 2.f;

    int x0 = x - half_size_x;
    int x1 = x + half_size_x;
    int y0 = y - half_size_y;
    int y1 = y + half_size_y;

    x0 = clamp(0, x0, buffer_width);
    x1 = clamp(0, x1, buffer_width);
    y0 = clamp(0, y0, buffer_heigth);
    y1 = clamp(0, y1, buffer_heigth);

    for (int y = y0 ; y < y1 ; y++) {
        unsigned int* pixel = (unsigned int*)buffer_memory + x0 + y * buffer_width;
        for (int x = x0 ; x < x1 ; x++) {
            if(y > y0 + (x1 - x) && y < y1 - (x1 - x)) *pixel = color;
            else if(y == y0 + (x1 - x) && y == y1 - (x1 - x)) *pixel = color;
            *pixel++;
        }
    }
}

void draw_number(unsigned int number, float x, float y, float size, unsigned int color) {
    float half_size = size * 0.5;
    bool drew_number = false;

    while (number || !drew_number) {
        drew_number = true;
        int digit = number % 10;
        number = number / 10;
        switch(digit) {
            case 0: {
                draw_rect(x - size, y, half_size, 2.5 * size, color);
                draw_rect(x + size, y, half_size, 2.5 * size, color);
                draw_rect(x, y + size * 2, half_size, half_size, color);
                draw_rect(x, y - size * 2, half_size, half_size, color);
                x -= size * 4;
            }break;
            case 1: {
                draw_rect(x, y, half_size, 2.5 * size, color);
                x -= size * 2;
            }break;
            case 2: {
                draw_rect(x, y + size * 2, 1.5 * size, half_size, color);
                draw_rect(x, y, 1.5 * size, half_size, color);
                draw_rect(x, y - size * 2, 1.5 * size, half_size, color);
                draw_rect(x + size, y + size, half_size, half_size, color);
                draw_rect(x - size, y - size, half_size, half_size, color);
                x -= size * 4;
            }break;
            case 3: {
                draw_rect(x - half_size, y + size * 2, size, half_size, color);
                draw_rect(x - half_size, y, size, half_size, color);
                draw_rect(x - half_size, y - size * 2, size, half_size, color);
                draw_rect(x + size, y, half_size, size * 2.5, color);
                x -= size * 4;
            }break;
            case 4: {
                draw_rect(x + size, y, half_size, size * 2.5, color);
                draw_rect(x - size, y + size, half_size, size * 1.5, color);
                draw_rect(x, y, half_size, half_size, color);
                x -= size * 4;
            }break;
            case 5: {
                draw_rect(x, y + size * 2, 1.5 * size, half_size, color);
                draw_rect(x, y, 1.5 * size, half_size, color);
                draw_rect(x, y - size * 2, 1.5 * size, half_size, color);
                draw_rect(x - size, y + size, half_size, half_size, color);
                draw_rect(x + size, y - size, half_size, half_size, color);
                x -= size * 4;
            }break;
            case 6: {
                draw_rect(x + half_size, y + size * 2, size, half_size, color);
                draw_rect(x + half_size, y, size, half_size, color);
                draw_rect(x + half_size, y - size * 2, size, half_size, color);
                draw_rect(x - size, y, half_size, size * 2.5, color);
                draw_rect(x + size, y - size, half_size, half_size, color);
                x -= size * 4;
            }break;
            case 7: {
                draw_rect(x + size, y, half_size, size * 2.5, color);
                draw_rect(x - half_size, y + size * 2, size, half_size, color);
                x -= size * 4;
            }break;
            case 8: {
                draw_rect(x - size, y, half_size, size * 2.5, color);
                draw_rect(x + size, y, half_size, size * 2.5, color);
                draw_rect(x, y + size * 2, half_size, half_size, color);
                draw_rect(x, y, half_size, half_size, color);
                draw_rect(x, y - size * 2, half_size, half_size, color);
                x -= size * 4;
            }break;
            case 9: {
                draw_rect(x - half_size, y + size * 2, size, half_size, color);
                draw_rect(x - half_size, y, size, half_size, color);
                draw_rect(x - half_size, y - size * 2, size, half_size, color);
                draw_rect(x + size, y, half_size, size * 2.5, color);
                draw_rect(x - size, y + size, half_size, half_size, color);
                x -= size * 4;
            }break;
        }
    }
}
