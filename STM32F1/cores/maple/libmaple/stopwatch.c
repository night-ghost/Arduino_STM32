#include <libmaple/stopwatch.h>
#include <stdbool.h>
#include <boards.h>

/* 
 * Provides a micro-second granular delay using the CPU cycle counter.
 */

/* cycles per microsecond */
uint32_t us_ticks;

void stopwatch_init(void)
{
	
	us_ticks = CYCLES_PER_MICROSECOND;
		
	/* turn on access to the DWT registers */
	DEMCR |= DEMCR_TRCENA; 
	/* enable the CPU cycle counter */
	DWT_CTRL |= CYCCNTENA;	

        stopwatch_reset();
}


void stopwatch_delay_us(uint32_t us){
//	stopwatch_reset(); we can't do that because any delay() in interrupt will reset main counter. It should be free running
    uint32_t ts = stopwatch_getticks(); // start time in ticks
    uint32_t dly = us * us_ticks;       // delay in ticks
    while(1) {
        uint32_t dt;
        uint32_t now = stopwatch_getticks(); // current time in ticks

//        if (now > ts) {
            dt = now - ts;
//        }else { // overflow
//            dt = now + (0xffffffffU - ts) + 1;
//        }
	if (dt >= dly)
		break;
    }
}
