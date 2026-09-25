#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "CityScape.h"


int main()
{
    // variable declaration
    int numPanels = 4;
    char* cityNames[4];
    int* gpioPins[4][3] = {
        {2, 3, 4},
        {5, 6, 7},
        {14, 15, 16},
        {17, 18, 19}
    };
    //  Arbitruary color values. Will change later
    int* ledColors[4][3] = {
        {124, 4, 6},
        {55, 94, 177},
        {84, 4, 212},
        {20, 244, 10}
    };

    //  Program Initialization
    cityNames[0] = add_city("auerbach");
    cityNames[1] = add_city("salina");
    cityNames[2] = add_city("dundee");
    cityNames[3] = add_city("vancouver");

    // Create collection of leds
    struct led_panel panels[4];
    led_init(panels, cityNames, gpioPins, ledColors);

    //  All data is initialized and now we can run the main program

    //  We need to figure out how to control the LED strip now. The
    //  primary control needed for the strips are:
    //      - Power Level
    //      - Red Color
    //      - Green Color
    //      - Blue Color
    //
    //  The ability to control power dynamically will allow the
    //  desired breathing effect and individual panel assignemnts


    debug_print_panel(&panels, numPanels);

    int termNum = 0;
    int runState = 1;

    change_led_power(&panels[0], 'R', 255);
    change_led_power(&panels[1], 'G', 255);
    change_led_power(&panels[2], 'B', 255);

    debug_print_panel(&panels, numPanels);

    while (runState != termNum) {

    }



    return 0;
}

void change_led_power(struct led_panel* panel, char color, int newPower) {
    switch (tolower(color)) {
    case 'r':
        panel->redValue = newPower;
        return;
    case 'g':
        panel->greenValue = newPower;
        return;
    case 'b':
        panel->blueValue = newPower;
        return;
    }
    printf("ERROR: Unexpected character provided. Char: %d", color);
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

void led_init(struct led_panel* panels, char** names[], int* gpioPins[3], int* ledColors[3]) {
    int arrLevel = 0;
    for (int i = 0; i < sizeof(panels); ++i) {
        //  Simple data copy
        panels[i].name = names[i];
        panels[i].power = 0;

        arrLevel = i * 3;
        panels[i].redPin = *&gpioPins[arrLevel + 0];
        panels[i].redValue = *&ledColors[arrLevel + 0];
        panels[i].greenPin = *&gpioPins[arrLevel + 1];
        panels[i].greenValue = *&ledColors[arrLevel + 1];
        panels[i].bluePin = *&gpioPins[arrLevel + 2];
        panels[i].blueValue = *&ledColors[arrLevel + 2];
    }
}

void debug_print_panel(struct led_panel panel[], int* arrSize) {
    for (int i = 0; i < arrSize; ++i) {
        printf("Panel name: %s\n", panel[i].name);
        printf("Power lvl:  %d\n", panel[i].power);
        printf("GPIO Red:   %d\n", panel[i].redPin);
        printf("GPIO Pwr:   %d\n", panel[i].redValue);
        printf("GPIO Grn:   %d\n", panel[i].greenPin);
        printf("GPIO Pwr:   %d\n", panel[i].greenValue);
        printf("GPIO Blu:   %d\n", panel[i].bluePin);
        printf("GPIO Pwr:   %d\n", panel[i].blueValue);
        printf("- - - - - - - - - - -\n");

    }
}