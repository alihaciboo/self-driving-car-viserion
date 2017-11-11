/*
 * trigger_middle.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: Miroslav Grubic
 */

#include "trigger_middle.h"
#include "utilities.h"


/**
 * Since this function is called from 100Hz (10ms) task it will trigger
 * middle sensor every 150ms but not at the same time when left and right sensors.
 * Sensor is triggered by setting GPIO output pin on our board to high for about 25us
 * @count number that shows how many times periodic function period_100Hz() is called
 * @m_trigger object of GPIO class defining pin to trigger middle sensor
 */
void trigger_middle(uint32_t const count, GPIO &m_trigger){

    if(count % 15 == 5){

        m_trigger.setHigh();
        delay_us(25);
        m_trigger.setLow();
    }
}


