/*
 *  https://www.dexterindustries.com/BrickPi/
 *  https://github.com/DexterInd/BrickPi3
 *
 *  Copyright (c) 2017 Dexter Industries
 *  Released under the MIT license (http://choosealicense.com/licenses/mit/).
 *  For more information see https://github.com/DexterInd/BrickPi3/blob/master/LICENSE.md
 *
 *  C++ drivers for the BrickPi3
 */

#ifndef BrickPi3_h_
#define BrickPi3_h_


#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>            // for printf
#include <string.h>           // for strstr

#include "brickpi_types.h"
#include "spi.h"



//#include <linux/types.h>
//#include <getopt.h>
//#include <unistd.h>





// Set up SPI. Open the file, and define the configuration.



class BrickPi3{
  public:
  // Default to address 1, but the BrickPi3 address could have been changed.
    BrickPi3(uint8_t addr = 1);

  // Confirm that the BrickPi3 is connected and up-to-date
    int     detect(bool critical = true);

  // Get the manufacturer (should be "Dexter Industries")
    int     get_manufacturer(char *str);
  // Get the board name (should be "BrickPi3")
    int     get_board(char *str);
  // Get the hardware version number
    int     get_version_hardware(char *str);
  // Get the firmware version number
    int     get_version_firmware(char *str);
  // Get the serial number ID that is unique to each BrickPi3
    int     get_id(char *str);
  // Control the LED
    int     set_led(uint8_t value);

  // Get the voltages of the four power rails
    // Get the voltage and return as floating point voltage
    float   get_voltage_3v3    ();
    float   get_voltage_5v     ();
    float   get_voltage_9v     ();
    float   get_voltage_battery();
    // Pass the pass-by-reference float variable where the voltage will be stored. Returns the error code.
    int     get_voltage_3v3    (float &voltage);
    int     get_voltage_5v     (float &voltage);
    int     get_voltage_9v     (float &voltage);
    int     get_voltage_battery(float &voltage);

  // Configure a sensor
    // Pass the port(s), sensor type, and optionally extra sensor configurations (flags and I2C information).
    int     set_sensor_type(uint8_t port, uint8_t type, uint16_t flags = 0, i2c_struct_t *i2c_struct = NULL);
  // Configure and trigger an I2C transaction
    int     transact_i2c(uint8_t port, i2c_struct_t *i2c_struct);
  // Get sensor value(s)
    int     get_sensor(uint8_t port, void *value_ptr);

  // Set the motor PWM power
    int     set_motor_power(uint8_t port, int8_t power);
  // Set the motor target position to run to
    // Set the absolute position to run to (go to the specified position)
    int     set_motor_position(uint8_t port, int32_t position);
    // Set the relative position to run to (go to the current position plus the specified position)
    int     set_motor_position_relative(uint8_t port, int32_t position);
  // Set the motor speed in degrees per second.
    int     set_motor_dps(uint8_t port, int16_t dps);
  // Set the motor limits.
  // Use the power limit to limit the motor speed/torque in speed or position control modes.
  // Use dps limit to limit the motor speed in position control mode.
    int     set_motor_limits(uint8_t port, uint8_t power, uint16_t dps);
  // Get the motor status. State, PWM power, encoder position, and speed (in degrees per second)
    int     get_motor_status(uint8_t port, uint8_t &state, int8_t &power, int32_t &position, int16_t &dps);
  // Offset the encoder position. By setting the offset to the current position, it effectively resets the encoder value.
    int     offset_motor_encoder(uint8_t port, int32_t position);
  // Reset the encoder.
    // Pass the port and pass-by-reference variable where the old encoder value will be stored. Returns the error code.
    int     reset_motor_encoder(uint8_t port, int32_t &value);
    // Pass the port. Returns the error code.
    int     reset_motor_encoder(uint8_t port);
  // Set the encoder position.
    // Pass the port and the new encoder position. Returns the error code.
    int     set_motor_encoder(uint8_t port, int32_t value);
  // Get the encoder position
    // Pass the port and pass-by-reference variable where the encoder value will be stored. Returns the error code.
    int     get_motor_encoder(uint8_t port, int32_t &value);
    // Pass the port. Returns the encoder value.
    int32_t get_motor_encoder(uint8_t port);

  // Reset the sensors (unconfigure), motors (float with no limits), and LED (return control to the firmware).
    int     reset_all();

  private:
    // BrickPi3 SPI address
    uint8_t Address;

    uint8_t SensorType[4];
    uint8_t I2CInBytes[4];

    int spi_write_8(uint8_t msg_type, uint8_t value);
    int spi_read_16(uint8_t msg_type, uint16_t &value);
    int spi_read_32(uint8_t msg_type, uint32_t &value);
    int spi_read_string(uint8_t msg_type, char *str, uint8_t chars = 20);
};

#endif
