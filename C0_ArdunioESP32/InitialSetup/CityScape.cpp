#include "CityScape.h"
#include <iostream>


using namespace std;

//  Global variables
const int NUM_PANELS = 4;

int main()
{
    //  Variable (single) declaration
    int termNum = 0;
    int runState = 1;
    int styleGpioPin = 36;
    int styleProfile = 0;

    //  Variable (array) declaration
    //  https://docs.espressif.com/projects/esp-idf/en/v5.1/esp32/_images/esp32-devkitC-v4-pinout.png
    //  All pins run NE to NW on board orientation
    char* cityNames[NUM_PANELS];
    int ledGpioPins[NUM_PANELS][3] =
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
    led_init(panels, cityNames, ledGpioPins, ledColors);
    debug_print_panel(panels);

    //  Main loop
    while (runState != termNum) {

    }

    return 0;
}


//  Address the LED Red, Green, and Blue color values individually
void change_led_color_single(struct led_panel &panel, char color, int colorValue) {
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
void change_led_color_group(struct led_panel &panel, int &r, int &g, int &b) {
    char rgb[3] = {'r', 'g', 'b'};
    int ledColors[] = { r, g, b };
    for (int i = 0; i < 3; ++i) {
        change_led_color_single(panel, rgb[i], ledColors[i]);
    }
    return;
}

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
void change_led_power(struct led_panel *panel, int power) {

}


//  Initialize LED values
void led_init(struct led_panel panels[], char *names[], int ledGpioPins[][3], int ledColors[][3]) {
    int arrLevel = 0;
    for (int i = 0; i < NUM_PANELS; ++i) {
        //  Simple data copy
        panels[i].name = names[i];
        panels[i].power = (int *)100;

        //  Pass in the assigned GPIO pins
        panels[i].redPin = (int *)ledGpioPins[i][0];
        panels[i].greenPin = (int *)ledGpioPins[i][1];
        panels[i].bluePin = (int *)ledGpioPins[i][2];

        //  Assign colors by group
        change_led_color_group(panels[i], ledColors[i][0], ledColors[i][1], ledColors[i][2]);
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