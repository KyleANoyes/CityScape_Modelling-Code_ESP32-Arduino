#include "CityScape.h"
#include <iostream>


using namespace std;

//  Global variables
const int NUM_PANELS = 4;
const int NUM_STYLES = 9;
const int NUM_SOLID_COLORS = 5;

//  GPIO Panel LED Declarations
#define P0_PIN_R 15
#define P0_PIN_G 2
#define P0_PIN_B 4
#define P1_PIN_R 16
#define P1_PIN_G 17
#define P1_PIN_B 5
#define P2_PIN_R 18
#define P2_PIN_G 19
#define P2_PIN_B 21
#define P3_PIN_R 3
#define P3_PIN_G 1
#define P3_PIN_B 22

//  GPIO Button Declarations
#define STYLE_PIN 32
#define BRIGHTNESS_PIN 33

int main()
{
    //  Variable (single) declaration
    int termCase = 1;
    int runState = 0;
    int gpioStylePin = 36;
    int gpioLedPowerScale = 39;
    int styleProfile = 0;
    int caseGroupIndex = 0;
    bool styleButtonArmed = true;

    //  Variable (array) declaration
    //  https://docs.espressif.com/projects/esp-idf/en/v5.1/esp32/_images/esp32-devkitC-v4-pinout.png
    //  All pins run NE to NW on board orientation
    char* cityNames[NUM_PANELS];
    int gpioLedPins[NUM_PANELS][3] =
    {
        {P0_PIN_R, P0_PIN_G, P0_PIN_B},
        {P1_PIN_R, P1_PIN_G, P1_PIN_B},
        {P2_PIN_R, P2_PIN_G, P2_PIN_B},
        {P3_PIN_R, P3_PIN_G, P3_PIN_B}
    };
    int ledDefaultColors[NUM_PANELS][3] =
    {
        {206, 170, 122},
        {205, 173, 138},
        {134, 81, 73},
        {110, 147, 42},
    };
    int solidColorPresets[NUM_SOLID_COLORS][3]{
        {250, 250, 250},
        {255, 237, 168},
        {187, 161, 46},
        {141, 200, 242},
        {189, 222, 171}
    };

    //  Program Initialization
    struct led_panel panels[NUM_PANELS];
    cityNames[0] = add_city("auerbach");
    cityNames[1] = add_city("salina");
    cityNames[2] = add_city("dundee");
    cityNames[3] = add_city("vancouver");
    led_init(panels, cityNames, gpioLedPins, ledDefaultColors);
    

    //  Main loop
    while (runState != termCase) {
        //  Reset group index to 0
        //  SWITCH: Color Style Selecion
        //      Sets LED colors to appropriate style
        switch(styleProfile){
            // Individual panel city colors
            case(0):
                reset_panels_colors(panels, ledDefaultColors);
                break;
            //  Fallthrough case to setting group color
            case (1):   //  Auerbach
                caseGroupIndex = 1;
                [[fallthrough]];
            case (2):   //  Salina
                [[fallthrough]];
            case (3):   //  Dundee
                [[fallthrough]];
            case (4):   //  Vancouver
                group_landscape_color(panels, ledDefaultColors, (styleProfile - caseGroupIndex));
                break;
            //  Fallthrough case to setting group solid color
            case (5):   //  White
                caseGroupIndex = 5;
                [[fallthrough]];
            case (6):   //  Yellow - Butter
                [[fallthrough]];
            case (7):   //  Yellow - Dark
                [[fallthrough]];
            case (8):   //  Blue
                [[fallthrough]];
            case (9):   //  Green
                group_solid_color(panels, solidColorPresets, (styleProfile - caseGroupIndex));
                break;
            default:
                cout << "ERROR: Switch 'Color Style Selecion' default reached" << endl;
        }

        debug_print_panel(panels);
        write_led_rgb(panels);
        styleProfile = debug_incrament_styleProfile(styleProfile);

        //  This code drives the arduino button state check and sets armed status
        /*
        //  Check if we can arm the style button
        if (styleButtonArmed == false) {
            if (digitalRead(STYLE_PIN) == LOW) {
                styleButtonArmed = true;
            }
        }

        //  If the style button is armed, check if a high press is active
        if (styleButtonArmed == true && digitalRead(STYLE_PIN) == HIGH) {
            styleButtonArmed = false;
            styleProfile = incramentStyleCase(styleProfile);
        }
        */
        
    }

    return runState;
}


//  Assign gpio pins to output of LED
void write_led_rgb(struct led_panel panels[]) {

    for (int i = 0; i < NUM_PANELS; ++i) {
        //  Scale panels RGB output to current brightness setting
        scale_panels_led_power(panels);

        //  Remove debug when copying into Arduino's IDE
        debug_analogWrite((int)panels[i].redPin, (int)(panels[i].redValue));
        debug_analogWrite((int)panels[i].greenPin, (int)(panels[i].greenValue));
        debug_analogWrite((int)panels[i].bluePin, (int)(panels[i].blueValue));
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
    cout << "ERROR: Unexpected character provided. Char: " << color << endl;
    return;
}


//  Address the LED Reg, Green, and Blue color values all at once
void set_led_color_group(struct led_panel &panel, int r, int g, int b) {
    char rgb[3] = {'r', 'g', 'b'};
    int ledColors[] = { r, g, b };
    for (int i = 0; i < 3; ++i) {
        set_led_color_single(panel, rgb[i], ledColors[i]);
    }
    return;
}


//  Adjust all panels LED's to brightness level based on signwave
void set_led_color_sinwave(struct led_panel& panel) {
    //  LOGIC WORKFLOW 
    //  1. Call sinwave radial from panel, scale LED based on that
    //  2. Scale the LED brightness based on radial calcualation
    
}


//  Get potentiometer value from analogInput
double get_brightness_normalized() {
    int inputVal = 0;
    double outputVal = 0.0;

    //  Debugging for VS
    //outputVal = inputVal / 4096;
    // Production for Arudino
    // inputVal = analogRead(BRIGHTNESS_PIN);
    //outputVal = map(inputVal, 0, 4096, 0, 100) / 100.0;
    return outputVal;
}


//  Return LED power based on potentiometer value
void scale_panels_led_power(struct led_panel panels[]) {
    for (int i = 0; i < NUM_PANELS; ++i) {
        int calculatedValue = 0;
        double scaleMultiple = get_brightness_normalized();

        calculatedValue = (int)panels[i].redValue * scaleMultiple;
        panels[i].redValue = (int*)calculatedValue;
        calculatedValue = (int)panels[i].blueValue * scaleMultiple;
        panels[i].blueValue = (int*)calculatedValue;
        calculatedValue = (int)panels[i].greenValue * scaleMultiple;
        panels[i].greenValue = (int*)calculatedValue;
    }
}


//  Allocate city name into memory
char* add_city(const char *newName) {
    char* newCity;
    int nameLen = 0;
    //  Parse length of char array to see if we find a null terminator
    for (int i = 0; i < 256; ++i) {
        if (newName[i] == '\0') {
            break;
        }
        nameLen += 1;
    }

    //  If we blew past 256 loops, assume bad data ingestion and use unknown
    if (nameLen == 256) {
        nameLen = 8;
        newName = "unknown";
    }

    //  Allocate heap memeory for needed size of char array
    newCity = new char[nameLen];
    for (int i = 0; i < nameLen; ++i) {
        newCity[i] = newName[i];
    }
    newCity[nameLen] = '\0';

    return newCity;
}


//  Change a panels LED power output
void set_led_power(struct led_panel &panel, int *newPower) {
    panel.power = (int*)(int)newPower;
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


//  - - Panel style setters - -
//  
void reset_panels_colors(struct led_panel panels[], int ledColors[][3]) {
    for (int i = 0; i < NUM_PANELS; ++i) {
        set_led_color_group(panels[i], ledColors[i][0], ledColors[i][1], ledColors[i][2]);
    }
}
//  Sets all panels to one of the defined colors
void group_landscape_color(struct led_panel panels[], int ledColors[][3], int styleCase) {
    for (int i = 0; i < NUM_PANELS; ++i) {
        set_led_color_group(panels[i], ledColors[styleCase][0], ledColors[styleCase][1], ledColors[styleCase][2]);
    }
}
//  Sets all panels to a series of solid color
void group_solid_color(struct led_panel panels[], int solidColors[][3], int styleCase) {
    for (int i = 0; i < NUM_PANELS; ++i) {
        set_led_color_group(panels[i], solidColors[styleCase][0], solidColors[styleCase][1], solidColors[styleCase][2]);
    }
}
//  Sets all panels to a sinewave fade
void group_sinewave_color(struct led_panel panels[]) {

}


//  Debug print all data related to panel struct
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
        cout << endl;
    }
}


//  Debug print a panels selected color pin value output
void debug_analogWrite(int pin, int value) {
    cout << "Pin: " << pin << endl;
    cout << "Value: " << value << endl;
    cout << "- - - - - - - - -" << endl;
    return;
}


//  Incrament the styleProfile by one and loop if needed
int debug_incrament_styleProfile(int styleProfile) {
    if (styleProfile == NUM_STYLES) {
        return 0;
    }

    return styleProfile += 1;
}