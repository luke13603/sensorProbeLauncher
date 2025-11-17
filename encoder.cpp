#include <ezButton.h>

#define CLK_PIN 2
#define DT_PIN 3
#define SW_PIN 4

#define DIRECTION_CW 1
#define DIRECTION_CCW -1

bool motorForward = true;   // set this from your motor function
int Multiplier = (motorForward ? 1 : -1);

// Your encoder + gear setup:
static const float gearRatio = 55.0 / 12.0;
static const float countsPerRev = 30.0;
static const float turretCountsPerRev = countsPerRev * gearRatio;
static const float degreesPerCount = 360.0 / turretCountsPerRev;

static int counter = 0;
static int direction = DIRECTION_CW;
static int prev_CLK_state = 0;

static ezButton button(SW_PIN);

void encoderInit() {
    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);

    button.setDebounceTime(50);

    prev_CLK_state = digitalRead(CLK_PIN);
}

void encoderUpdate() {
    button.loop();

    int CLK_state = digitalRead(CLK_PIN);

    // capture rising edge
    if (CLK_state != prev_CLK_state && CLK_state == HIGH) {

        if (digitalRead(DT_PIN) == HIGH) {
            counter--;
            direction = DIRECTION_CCW;
        } else {
            counter++;
            direction = DIRECTION_CW;
        }
    }

    prev_CLK_state = CLK_state;

    if (button.isPressed()) {
        counter = 0;
    }
}

float getTurretAngle() {
    float angle = counter * degreesPerCount;

    // wrap 0–360
    while (angle >= 360) angle -= 360;
    while (angle < 0) angle += 360;

    return angle;
}

long getTurretCount() {
    return counter;
}

int getTurretDirection() {
    return direction;
}

void resetTurretAngle() {
    counter = 0;
}
