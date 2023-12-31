struct Button_State {
    bool is_down;
    bool changed;
};

enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_JUMP,
    BUTTON_JUMP2,
    BUTTON_ENTER,

    BUTTON_COUNT,
};

struct Input {
    Button_State buttons[BUTTON_COUNT];
};

enum {
    left,
    right,
};
