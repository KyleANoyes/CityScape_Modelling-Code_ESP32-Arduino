#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "CityScape.h"


int main()
{
    //  Variable (single) declaration
    int termNum = 0;
    int runState = 1;
    int numPanels = 4;
    int styleGpioPin = 36;
    int styleProfile = 0;

    //  Variable (array) declaration
    //  https://docs.espressif.com/projects/esp-idf/en/v5.1/esp32/_images/esp32-devkitC-v4-pinout.png
    //  All pins run NE to NW on board orientation
    char* cityNames[4];
    int ledGpioPins[4][3] =
    {
        {15, 2, 0},
        {4, 16, 17},
        {5, 18, 19},
        {21, 3, 1}
    };
    int ledColors[4][3] =
    {
        {124, 4, 6},
        {55, 94, 177},
        {84, 4, 212},
        {20, 244, 10}
    };

    //  Program Initialization
    struct led_panel *panels[4];
    cityNames[0] = add_city("auerbach");
    cityNames[1] = add_city("salina");
    cityNames[2] = add_city("dundee");
    cityNames[3] = add_city("vancouver");
    led_init(panels, cityNames, ledGpioPins, ledColors);
    debug_print_panel(panels, numPanels);

    //  Main loop
    while (runState != termNum) {

    }



    return 0;
}


//  Address the LED Red, Green, and Blue color values individually
void change_led_color_single(struct led_panel *panel, char color, int colorValue) {
    switch (tolower(color)) {
    case 'r':
        panel->redValue = colorValue;
        return;
    case 'g':
        panel->greenValue = colorValue;
        return;
    case 'b':
        panel->blueValue = colorValue;
        return;
    }
    printf("ERROR: Unexpected character provided. Char: %d", color);
    return;
}

//  Address the LED Reg, Green, and Blue color values all at once
void change_led_color_group(struct led_panel *panel, int *r, int *g, int *b) {
    char* rgb[] = { 'r', 'g', 'b' };
    int* ledColors[] = { r, g, b };
    for (int i = 0; i < 3; ++i) {
        change_led_color_single(&panel, rgb[i], ledColors[i]);
    }
    return;
}

char* add_city(char *newName) {
    int nameLen = strlen(*&newName);
    char* newCity;

    newCity = malloc((sizeof(newCity) * nameLen) + 1);
    for (int i = 0; i < nameLen; ++i) {
        newCity[i] = newName[i];
    }
    newCity[nameLen] = '\0';

    return newCity;
}


void change_led_power(struct led_panel *panel, int power) {

}


void led_init(struct led_panel *panels, char *names, int (*ledGpioPins)[3], int(*ledColors)[3]) {
    for (int i = 0; i < sizeof(panels); ++i) {
        //  Simple data copy
        panels[i].name = names[i];
        panels[i].power = 0;

        //  Pass in the assigned GPIO pins
        panels[i].redPin = ledGpioPins[i][0];
        panels[i].greenPin = ledGpioPins[i][1];
        panels[i].bluePin = ledGpioPins[i][2];

        //  Assign colors by group
        /*change_led_color_group(
            panels[i],
            ledColors[i][0],
            ledColors[i][1],
            ledColors[i][2]
        );*/
    }
}

void debug_print_panel(struct led_panel *panel[], int arrSize) {
    for (int i = 0; i < arrSize; ++i) {
        printf("Panel name: %s\n", panel[i]->name);
        printf("Power lvl:  %d\n", panel[i]->power);
        printf("GPIO Red:   %d\n", panel[i]->redPin);
        printf("GPIO Pwr:   %d\n", panel[i]->redValue);
        printf("GPIO Grn:   %d\n", panel[i]->greenPin);
        printf("GPIO Pwr:   %d\n", panel[i]->greenValue);
        printf("GPIO Blu:   %d\n", panel[i]->bluePin);
        printf("GPIO Pwr:   %d\n", panel[i]->blueValue);
        printf("- - - - - - - - - - -\n");

    }
}