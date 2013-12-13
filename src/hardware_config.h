#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

#define BUTTON_LOW_DRIVE_PIN    11
#define BUTTON_INPUT_PIN        12
#define LED_PIN                 13

#define PIXELS_PER_EYE              16
#define NUM_PIXELS                  PIXELS_PER_EYE * 2

#ifdef UNO_HARDWARE
    #define NEO_OUTPUT_PIN              0
#elif defined PRO_MINI_8MHZ_HARDWARE
    #define NEO_OUTPUT_PIN              10
#else
    #error "Unknown hardware"
#endif

#define TOP_RIGHT                   12
#define TOP_LEFT                    20

#endif // HARDWARE_CONFIG_H
