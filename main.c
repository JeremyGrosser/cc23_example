#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"

void main(void) {
    while(1) {
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_LED_ON);
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_LED_OFF);
    }
}
