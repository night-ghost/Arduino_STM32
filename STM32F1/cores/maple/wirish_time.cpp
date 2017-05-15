/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 *  @brief Delay implementation.
 */

#include "wirish_time.h"
#include <Arduino.h>

#include <libmaple/libmaple_types.h>
#include <libmaple/delay.h>
#include <libmaple/stopwatch.h>

void delay(unsigned long ms) {
    uint32_t start = micros();
        
    while (ms > 0) {
            yield(1000);
            while ( ms > 0 && (micros() - start) >= 1000) {
                    ms--;
                    start += 1000;
            }
    }    

}

void delayMicroseconds(uint32 us) {
//    delay_us(us);

#ifdef SHED_PROF
    uint32_t t = _micros();
#endif

    uint32_t rtime = stopwatch_getticks(); // start ticks
    uint32_t dt    = us_ticks * us;  // delay time in ticks
  
    uint32_t ny = 5 * us_ticks; // 5 uS in ticks
    uint32_t tw;

    while ((tw = stopwatch_getticks() - rtime) < dt) { // tw - time waiting, in ticks
        if((dt - tw) > ny){ // No Yeld time - 5uS to end of wait 
            yield((dt - tw) / us_ticks); // in micros
        }
    }

#ifdef SHED_PROF
    us=_micros()-t; // real time

    if(_in_timer_proc)
        delay_int_time +=us;
    else
        delay_time     +=us;
#endif

}

/*
static void __empty() {
        // Empty
}
void yield(void) __attribute__ ((weak, alias("__empty")));
*/