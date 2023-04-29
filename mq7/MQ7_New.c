#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MQ7_PIN 27 // Connect MQ7 digital output to Raspberry Pi GPIO 1

int main(void) 
{
  int sensor_value;

  struct gpiod_chip *chip = gpiod_chip_open("/dev/gpiochip0");
  struct gpiod_line *mq7_line;
  int mq7_value = 0;
	
    char *buffer_n = "CO is in the normal range 69 ppm to 80 ppm\r\n";
    char *buffer_g = "HARMFULL!!! CO level above 100 ppm\r\n";

    chip = gpiod_chip_open("/dev/gpiochip0"); // Open GPIO chip
    mq7_line = gpiod_chip_get_line(chip, MQ7_PIN); // Get GPIO line
    gpiod_line_request_input(mq7_line, "mq7"); // Set GPIO line as input

    // Continuously read data
    while (1) 
   {
        mq7_value = gpiod_line_get_value(mq7_line); // Read GPIO value

        if(mq7_value == 1)
        {
		printf("%s", buffer_n);
	}
	else
	{
		printf("%s", buffer_g);
	}

        usleep(100000); // Wait for 100 milliseconds
    }


  
  gpiod_line_release(mq7_line); // Release GPIO line
  gpiod_chip_close(chip);


  

    return 0;
}
