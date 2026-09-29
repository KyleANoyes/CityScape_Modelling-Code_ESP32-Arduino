struct led_panel {
    char* name;
    int* power;
    int* redPin;
    int* redValue;
    int* bluePin;
    int* blueValue;
    int* greenPin;
    int* greenValue;
};

void change_led_color_single(struct led_panel* panel, char color, int colorValue);
void change_led_color_group(struct led_panel* panel, int* r, int* g, int* b);
void change_led_power(struct led_panel* panel, int power);
char* add_city(char* newName);
void led_init(struct led_panel* panels, char** names[], int (*ledGpioPins)[3], int(*ledColors)[3]);
void debug_print_panel(struct led_panel panel[], int* arrSize);