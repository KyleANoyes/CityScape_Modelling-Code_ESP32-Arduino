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

int change_led_power(struct led_panel* panel, int newPower);
char* add_city(char* newName);
void led_init(struct led_panel* panels, char* name, int* gpioPins, int* ledColors[3]);
void debug_print_panel(struct led_panel* panel[]);