#include "CityScape.h"
#include <iostream>

using namespace std;

//  Global variables
const int NUM_PANELS = 4;

//  GPIO Panel LED Declarations
#define P0_PIN_R 15
#define P0_PIN_G 2
#define P0_PIN_B 0
#define P1_PIN_R 4
#define P1_PIN_G 16
#define P1_PIN_B 17
#define P2_PIN_R 5
#define P2_PIN_G 18
#define P2_PIN_B 19
#define P3_PIN_R 21
#define P3_PIN_G 3
#define P3_PIN_B 1

//  GPIO Button Declarations
#define STYLE_PIN = 39;
#define POWER_SCALE_PIN = 36;

void setup() {
    pinMode(P0_PIN_R, OUTPUT);
    pinMode(P0_PIN_G, OUTPUT);
    pinMode(P0_PIN_B, OUTPUT);
    pinMode(P1_PIN_R, OUTPUT);
    pinMode(P1_PIN_G, OUTPUT);
    pinMode(P1_PIN_B, OUTPUT);
    pinMode(P2_PIN_R, OUTPUT);
    pinMode(P2_PIN_G, OUTPUT);
    pinMode(P2_PIN_B, OUTPUT);
    pinMode(P3_PIN_R, OUTPUT);
    pinMode(P3_PIN_G, OUTPUT);
    pinMode(P3_PIN_B, OUTPUT);
}

void loop()
{
    //  Variable (single) declaration
    int termNum = 0;
    int runState = 1;
    int gpioStylePin = 36;
    int gpioLedPowerScale = 39;
    int styleProfile = 0;

    //  Variable (array) declaration
    //  https://docs.espressif.com/projects/esp-idf/en/v5.1/esp32/_images/esp32-devkitC-v4-pinout.png
    //  All pins run NE to NW on board orientation
    char* cityNames[NUM_PANELS];
    int gpioLedPins[NUM_PANELS][3] =
    {
        {15, 2, 0},
        {4, 16, 17},
        {5, 18, 19},
        {21, 3, 1}
    };
    int ledColors[NUM_PANELS][3] =
    {
        {124, 4, 6},
        {55, 94, 177},
        {84, 4, 212},
        {20, 244, 10}
    };

    //  Program Initialization
    struct led_panel panels[NUM_PANELS];
    cityNames[0] = add_city("auerbach");
    cityNames[1] = add_city("salina");
    cityNames[2] = add_city("dundee");
    cityNames[3] = add_city("vancouver");
    led_init(panels, cityNames, gpioLedPins, ledColors);

    //  Main loop
    while (runState != termNum) {
        write_led_rgb(panels);

    }
}

//  Assign gpio pins to output of LED
void write_led_rgb(struct led_panel panels[]) {
    for (int i = 0; i < NUM_PANELS; ++i) {
        //  Uncomment analogWrite in Arduino IDE
        analogWrite((int)panels[i].redPin, (int)panels[i].redValue);
        analogWrite((int)panels[i].greenPin, (int)panels[i].greenValue);
        analogWrite((int)panels[i].bluePin, (int)panels[i].blueValue);
    }
}

//  Address the LED Red, Green, and Blue color values individually
void set_led_color_single(struct led_panel &panel, char color, int colorValue) {
    switch (tolower(color)) {
    case 'r':
        panel.redValue = (int*)colorValue;
        return;
    case 'g':
        panel.greenValue = (int*)colorValue;
        return;
    case 'b':
        panel.blueValue = (int*)colorValue;
        return;
    }
    printf("ERROR: Unexpected character provided. Char: %d", color);
    return;
}

//  Address the LED Reg, Green, and Blue color values all at once
void set_led_color_group(struct led_panel &panel, int &r, int &g, int &b) {
    char rgb[3] = {'r', 'g', 'b'};
    int ledColors[] = { r, g, b };
    for (int i = 0; i < 3; ++i) {
        set_led_color_single(panel, rgb[i], ledColors[i]);
    }
    return;
}

//  Allocate city name into memory
char* add_city(const char *newName) {
    int nameLen = sizeof(newName) / sizeof(newName[0]);
    char* newCity;

    newCity = new char[nameLen];
    for (int i = 0; i < nameLen; ++i) {
        newCity[i] = newName[i];
    }
    newCity[nameLen] = '\0';

    return newCity;
}


//  Change a panels LED power output

void set_led_power(struct led_panel &panel, int *newPower, int *ledPowerScale) {
    //  Scale power into double then pass into panel
    double scaledPower = *newPower * *ledPowerScale;
    panel.power = (int*)(int)round(scaledPower);
}


//  Initialize LED values
void led_init(struct led_panel panels[], char *names[], int gpioLedPins[][3], int ledColors[][3]) {
    int arrLevel = 0;
    for (int i = 0; i < NUM_PANELS; ++i) {
        //  Simple data copy
        panels[i].name = names[i];
        panels[i].power = (int *)100;

        //  Pass in the assigned GPIO pins
        panels[i].redPin = (int*)gpioLedPins[i][0];
        panels[i].greenPin = (int*)gpioLedPins[i][1];
        panels[i].bluePin = (int*)gpioLedPins[i][2];

        //  Assign colors by group
        set_led_color_group(panels[i], ledColors[i][0], ledColors[i][1], ledColors[i][2]);
    }
}


void debug_print_panel(struct led_panel panels[]) {
    for (int i = 0; i < NUM_PANELS; ++i) {
        cout << "Panel name:    " << panels[i].name << endl;
        cout << "Power Level:   " << (int)panels[i].power << endl;
        cout << "GPIO Red:      " << (int)panels[i].redPin << endl;
        cout << "GPIO Red Val:  " << (int)panels[i].redValue << endl;
        cout << "GPIO Grn:      " << (int)panels[i].greenPin << endl;
        cout << "GPIO Grn Val:  " << (int)panels[i].greenValue << endl;
        cout << "GPIO Blu:      " << (int)panels[i].bluePin << endl;
        cout << "GPIO Blu Val:  " << (int)panels[i].blueValue << endl;
        cout << "- - - - - - - - - - - - -" << endl;
    }
}