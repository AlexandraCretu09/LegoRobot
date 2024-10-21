/*
 * brickpi_types.h
 *
 *  Created on: Mar 8, 2024
 *      Author: valy
 */

#ifndef BRICKPI_TYPES_H_
#define BRICKPI_TYPES_H_

#include <stdint.h>
#include <stdbool.h>

#define FIRMWARE_VERSION_REQUIRED "1.4." // Firmware version needs to start with this

#define LONGEST_SPI_TRANSFER 29 // longest possible message for configuring for an I2C sensor
#define LONGEST_I2C_TRANSFER 16 // longest possible I2C read/write

#define SPI_TARGET_SPEED 500000 // SPI target speed of 500kbps

#define SPIDEV_FILE_NAME "/dev/spidev0.1" // File name of SPI

// Error values
#define ERROR_NONE                  0
#define ERROR_SPI_FILE             -1
#define ERROR_SPI_RESPONSE         -2
#define ERROR_WRONG_MANUFACTURER   -3
#define ERROR_WRONG_DEVICE         -4
#define ERROR_FIRMWARE_MISMATCH    -5
#define ERROR_SENSOR_TYPE_MISMATCH -6

// SPI message type
enum BPSPI_MESSAGE_TYPE{
  BPSPI_MESSAGE_NONE,

  BPSPI_MESSAGE_GET_MANUFACTURER,      // 1
  BPSPI_MESSAGE_GET_NAME,
  BPSPI_MESSAGE_GET_HARDWARE_VERSION,
  BPSPI_MESSAGE_GET_FIRMWARE_VERSION,
  BPSPI_MESSAGE_GET_ID,
  BPSPI_MESSAGE_SET_LED,
  BPSPI_MESSAGE_GET_VOLTAGE_3V3,
  BPSPI_MESSAGE_GET_VOLTAGE_5V,
  BPSPI_MESSAGE_GET_VOLTAGE_9V,
  BPSPI_MESSAGE_GET_VOLTAGE_VCC,
  BPSPI_MESSAGE_SET_ADDRESS,           // 11

  BPSPI_MESSAGE_SET_SENSOR_TYPE,       // 12

  BPSPI_MESSAGE_GET_SENSOR_1,          // 13
  BPSPI_MESSAGE_GET_SENSOR_2,
  BPSPI_MESSAGE_GET_SENSOR_3,
  BPSPI_MESSAGE_GET_SENSOR_4,

  BPSPI_MESSAGE_I2C_TRANSACT_1,        // 17
  BPSPI_MESSAGE_I2C_TRANSACT_2,
  BPSPI_MESSAGE_I2C_TRANSACT_3,
  BPSPI_MESSAGE_I2C_TRANSACT_4,

  BPSPI_MESSAGE_SET_MOTOR_POWER,

  BPSPI_MESSAGE_SET_MOTOR_POSITION,

  BPSPI_MESSAGE_SET_MOTOR_POSITION_KP,

  BPSPI_MESSAGE_SET_MOTOR_POSITION_KD, // 24

  BPSPI_MESSAGE_SET_MOTOR_DPS,         // 25

  BPSPI_MESSAGE_SET_MOTOR_DPS_KP,

  BPSPI_MESSAGE_SET_MOTOR_DPS_KD,

  BPSPI_MESSAGE_SET_MOTOR_LIMITS,

  BPSPI_MESSAGE_OFFSET_MOTOR_ENCODER,  // 29

  BPSPI_MESSAGE_GET_MOTOR_A_ENCODER,   // 30
  BPSPI_MESSAGE_GET_MOTOR_B_ENCODER,
  BPSPI_MESSAGE_GET_MOTOR_C_ENCODER,
  BPSPI_MESSAGE_GET_MOTOR_D_ENCODER,

  BPSPI_MESSAGE_GET_MOTOR_A_STATUS,    // 34
  BPSPI_MESSAGE_GET_MOTOR_B_STATUS,
  BPSPI_MESSAGE_GET_MOTOR_C_STATUS,
  BPSPI_MESSAGE_GET_MOTOR_D_STATUS
};

// Sensor type
enum SENSOR_TYPE{
  SENSOR_TYPE_NONE = 1, // Not configured for any sensor type
  SENSOR_TYPE_I2C,
  SENSOR_TYPE_CUSTOM,   // Choose 9v pullup, pin 5 and 6 configuration, and what to read back (ADC 1 and/or ADC 6 (always reports digital 5 and 6)).

  SENSOR_TYPE_TOUCH,    // Touch sensor. When this mode is selected, automatically configure for NXT/EV3 as necessary.
  SENSOR_TYPE_TOUCH_NXT,
  SENSOR_TYPE_TOUCH_EV3,

  SENSOR_TYPE_NXT_LIGHT_ON,
  SENSOR_TYPE_NXT_LIGHT_OFF,

  SENSOR_TYPE_NXT_COLOR_RED,
  SENSOR_TYPE_NXT_COLOR_GREEN,
  SENSOR_TYPE_NXT_COLOR_BLUE,
  SENSOR_TYPE_NXT_COLOR_FULL,
  SENSOR_TYPE_NXT_COLOR_OFF,

  SENSOR_TYPE_NXT_ULTRASONIC,

  SENSOR_TYPE_EV3_GYRO_ABS,
  SENSOR_TYPE_EV3_GYRO_DPS,
  SENSOR_TYPE_EV3_GYRO_ABS_DPS,

  SENSOR_TYPE_EV3_COLOR_REFLECTED,
  SENSOR_TYPE_EV3_COLOR_AMBIENT,
  SENSOR_TYPE_EV3_COLOR_COLOR,
  SENSOR_TYPE_EV3_COLOR_RAW_REFLECTED,
  SENSOR_TYPE_EV3_COLOR_COLOR_COMPONENTS,

  SENSOR_TYPE_EV3_ULTRASONIC_CM,
  SENSOR_TYPE_EV3_ULTRASONIC_INCHES,
  SENSOR_TYPE_EV3_ULTRASONIC_LISTEN,

  SENSOR_TYPE_EV3_INFRARED_PROXIMITY,
  SENSOR_TYPE_EV3_INFRARED_SEEK,
  SENSOR_TYPE_EV3_INFRARED_REMOTE
};

// Sensor states/error values
enum SENSOR_STATE{
  SENSOR_STATE_VALID_DATA,
  SENSOR_STATE_NOT_CONFIGURED,
  SENSOR_STATE_CONFIGURING,
  SENSOR_STATE_NO_DATA,
  SENSOR_STATE_I2C_ERROR        // Such as no ACK, SCL stretched too long, etc.
};

// Flags for configuring custom and I2C sensors
enum SENSOR_CONFIG_FLAGS{
  SENSOR_CONFIG_I2C_MID_CLOCK = 0x0001, // I2C. Send a clock pulse between reading and writing. Required by the NXT US sensor.
  SENSOR_CONFIG_PIN_1_PULL    = 0x0002, // I2C or custom. Enable 9v pullup on pin 1
  SENSOR_CONFIG_I2C_REPEAT    = 0x0004, // I2C. Keep performing the same transaction e.g. keep polling a sensor
  SENSOR_CONFIG_PIN_5_DIR     = 0x0010, // Custom. Set pin 5 output
  SENSOR_CONFIG_PIN_5_STATE   = 0x0020, // Custom. Set pin 5 high
  SENSOR_CONFIG_PIN_6_DIR     = 0x0100, // Custom. Set pin 6 output
  SENSOR_CONFIG_PIN_6_STATE   = 0x0200, // Custom. Set pin 6 high
  SENSOR_CONFIG_REPORT_1_ADC  = 0x1000, // Custom. Read pin 1 ADC
  SENSOR_CONFIG_REPORT_6_ADC  = 0x4000  // Custom. Read pin 6 ADC
};

// Sensor ports
#define PORT_1 0x01
#define PORT_2 0x02
#define PORT_3 0x04
#define PORT_4 0x08

// Motor ports
#define PORT_A 0x01
#define PORT_B 0x02
#define PORT_C 0x04
#define PORT_D 0x08

// Motor Float. Value to pass to set_motor_power to make a motor float.
#define MOTOR_FLOAT -128

// EV3 infrared remote button bit masks
#define REMOTE_BIT_RED_UP    0x01
#define REMOTE_BIT_RED_DOWN  0x02
#define REMOTE_BIT_BLUE_UP   0x04
#define REMOTE_BIT_BLUE_DOWN 0x08
#define REMOTE_BIT_BROADCAST 0x10

// structure for I2C
struct i2c_struct_t{
  uint8_t speed;
  uint8_t delay;
  uint8_t address;
  uint8_t length_write;
  uint8_t buffer_write[LONGEST_I2C_TRANSFER];
  uint8_t length_read;
  uint8_t buffer_read[LONGEST_I2C_TRANSFER];
};

// structure for custom sensors
struct sensor_custom_t{
  uint16_t adc1;
  uint16_t adc6;
  bool     pin5;
  bool     pin6;
};

// structure for touch sensors
struct sensor_touch_t{
  bool     pressed;
};

// structure for light sensor
struct sensor_light_t{
  int16_t  ambient;
  int16_t  reflected;
};

// structure for color sensors
struct sensor_color_t{
  int8_t   color;
  int16_t  reflected_red;
  int16_t  reflected_green;
  int16_t  reflected_blue;
  int16_t  ambient;
};

// structure for ultrasonic sensors
struct sensor_ultrasonic_t{
  float    cm;
  float    inch;
  bool     presence;
};

// structure for gyro sensor
struct sensor_gyro_t{
  int16_t  abs;
  int16_t  dps;
};

// structure for infrared sensor
struct sensor_infrared_t{
  uint8_t  proximity;
  int8_t   distance[4];
  int8_t   heading[4];
  uint8_t  remote[4];
};



#endif /* BRICKPI_TYPES_H_ */
