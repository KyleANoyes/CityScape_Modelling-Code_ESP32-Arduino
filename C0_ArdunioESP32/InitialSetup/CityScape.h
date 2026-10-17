struct led_panel {
    char* name;
    int* power;
    int* redPin;
    int* redValue;
    int* bluePin;
    int* blueValue;
    int* greenPin;
    int* greenValue;
    double* sinewaveRadial;
};

void set_led_color_single(struct led_panel *panel, char color, int colorValue);
void set_led_color_group(struct led_panel &panel, int r, int g, int b);
void set_led_power(struct led_panel &panel, int *newPower);
void set_led_power_sinwave(struct led_panel& panel);
void incrament_sinwave(struct led_panel& panel);
void write_led_rgb(struct led_panel panels[]);
char* add_city(const char *newName);
void led_init(struct led_panel panels[], char *names[], int gpioLedPins[][3], int ledColors[][3]);
void debug_print_panel(struct led_panel *panel);
void debug_analogWrite(int pin, int value);