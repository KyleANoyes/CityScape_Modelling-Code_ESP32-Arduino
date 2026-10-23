//  - - Structs and Classes - - 
//  Only one struct needed for this project.
//  - - - - - - - - -
//  led_panel tracks 
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

//  - - Setters - - 
//  
//  - - - - - - - - -
void set_led_color_single(struct led_panel *panel, char color, int colorValue);
void set_led_color_group(struct led_panel &panel, int r, int g, int b);
void set_led_power(struct led_panel &panel, int *newPower);
void set_led_color_sinwave(struct led_panel& panel);


//  - - Getters - - 
//  
//  - - - - - - - - -
double get_brightness_normalized();


//  - - Modifiers - -
//  
//  - - - - - - - - -
void scale_panels_led_power(struct led_panel panels[]);
void incrament_sinwave(struct led_panel& panel);
void write_led_rgb(struct led_panel panels[]);
char* add_city(const char *newName);
void led_init(struct led_panel panels[], char *names[], int gpioLedPins[][3], int ledColors[][3]);


//  - - Patterns - - 
//  All LED patten control functions
//  - - - - - - - - -
void reset_panels_colors(struct led_panel panels[], int ledColors[][3]);
void group_landscape_color(struct led_panel panels[], int ledColors[][3], int styleCase);
void group_solid_color(struct led_panel panels[], int ledColors[][3], int styleCase);
void group_sinewave_color(struct led_panel panels[]);


//  - - Debugging - - 
//  Used for internal testing and to dump data or
//      memory to the terminal when needed
//  - - - - - - - - -
void debug_print_panel(struct led_panel *panel);
void debug_analogWrite(int pin, int value);
int debug_incrament_styleProfile(int styleProfile);