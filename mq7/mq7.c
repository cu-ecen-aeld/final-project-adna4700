#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

//#define MQ7_PIN 17 // Connect MQ7 digital output to Raspberry Pi GPIO 17


// Define MCP3008 SPI channel and clock speed
#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

// Define sensor channel on MCP3008
#define ADC_CHANNEL 0

// Define delay between readings
#define DELAY 1000


//float getMQ7COConcentration(float rs_ro_ratio) {
//    return 2.3*pow(rs_ro_ratio, -1.27); // Calculate CO concentration
//}

int read_adc(int channel, struct gpiod_line *mosi, struct gpiod_line *miso, struct gpiod_line *clk, struct gpiod_line *cs) {
  unsigned char buffer[3];
  int adc;

  buffer[0] = 1;              // Start bit
  buffer[1] = 0x80 | (channel << 4); // Single ended, channel selection
  buffer[2] = 0;              // Don't care

  struct gpiod_chip *chip = gpiod_chip_open("/dev/gpiochip0");
  gpiod_line_set_value(cs, 0); // Set chip select line low to enable MCP3008
  for (int i = 0; i < 3; i++) {
    for (int j = 7; j >= 0; j--) {
      gpiod_line_set_value(mosi, (buffer[i] >> j) & 1);
      gpiod_line_set_value(clk, 1);
      gpiod_line_set_value(clk, 0);
      buffer[i] <<= 1;
    }
  }
  for (int i = 0; i < 3; i++) {
    buffer[i] = 0;
    for (int j = 7; j >= 0; j--) {
      gpiod_line_set_value(clk, 1);
      buffer[i] = (buffer[i] << 1) | gpiod_line_get_value(miso);
      gpiod_line_set_value(clk, 0);
    }
  }
  gpiod_line_set_value(cs, 1); // Set chip select line high to disable MCP3008
  gpiod_chip_close(chip);

  adc = ((buffer[1] & 3) << 8) + buffer[2]; // Convert data to integer value

  return adc;
}


int main(void) 
{
    int sensor_value;


  struct gpiod_chip *chip = gpiod_chip_open("/dev/gpiochip0");
  struct gpiod_line *mosi = gpiod_chip_get_line(chip, 9); // BCM 9
  struct gpiod_line *miso = gpiod_chip_get_line(chip, 10); // BCM 10
  struct gpiod_line *clk = gpiod_chip_get_line(chip, 11); // BCM 11
  struct gpiod_line *cs = gpiod_chip_get_line(chip, 8); // BCM 8
  gpiod_line_request_output(cs, "cs", 0); // Request chip select line as output, initially low

  while (1) {
    // Read MQ-7 sensor value
    sensor_value = read_adc(ADC_CHANNEL, mosi, miso, clk, cs);
    printf("MQ-7 Sensor Value: %d\n", sensor_value);
    usleep(DELAY * 1000);
  }

  gpiod_line_release(mosi);
  gpiod_line_release(miso);
  gpiod_line_release(clk);
  gpiod_line_release(cs);
  gpiod_chip_close(chip);


   //struct gpiod_chip *chip;
   // struct gpiod_line *mq7_line;
   // int mq7_value = 0;
   // float mq7_rs_ro_ratio = 0;
   // float mq7_ro = 10.0; // Resistance of the MQ7 sensor's RO (clean air)
   // float mq7_co_concentration = 0;

  //  chip = gpiod_chip_open("/dev/gpiochip0"); // Open GPIO chip
  //  mq7_line = gpiod_chip_get_line(chip, MQ7_PIN); // Get GPIO line
  //  gpiod_line_request_input(mq7_line, "mq7"); // Set GPIO line as input

    // Continuously read data
  //  while (1) {
 //       mq7_value = gpiod_line_get_value(mq7_line); // Read GPIO value
  //      mq7_rs_ro_ratio = (float)mq7_value / (1 - (float)mq7_value); // Calculate Rs/Ro
 //       mq7_co_concentration = getMQ7COConcentration(mq7_rs_ro_ratio); // Calculate CO ////concentration

    //    printf("MQ7 Value: %d\n", mq7_value);
    //    printf("Rs/Ro: %.3f\n", mq7_rs_ro_ratio);
    //    printf("CO Concentration: %.3f ppm\n", mq7_co_concentration);
//
    //    usleep(100000); // Wait for 100 milliseconds
   // }

  //  gpiod_line_release(mq7_line); // Release GPIO line
  //  gpiod_chip_close(chip); // Close GPIO chip

    return 0;
}
