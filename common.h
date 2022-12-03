#ifndef _COMMONT_H_
#define _COMMONT_H_

#define IRRev_Pin 14
#define Sensor_Pin 12
#define PWM_Pin 13
#define Fan_Pin 15
#define Relay_1_Pin 16
#define Relay_2_Pin 2

#define DEFAULT_BAUDRATE 115200

#define CH1_KEY 0xFFA25D
#define CH2_KEY 0xFF629D
#define CH3_KEY 0xFFE21D
#define PREV_KEY 0xFF22DD
#define NEXT_KEY 0xFF02FD
#define PLAY_KEY 0xFFC23D
#define VOL1_KEY 0xFFE01F
#define VOL2_KEY 0xFFA857
#define EQ_KEY 0xFF906F

#define START_BYTE 0x0A
#define STOP_BYTE 0x0B

#define CMD_TURN_OFF_ALL 0x01

#define CMD_SET_MOTOR_SPEED_1 0x02
#define CMD_SET_MOTOR_SPEED_2 0x03
#define CMD_SET_MOTOR_SPEED_3 0x04
#define CMD_SET_MOTOR_SPEED_4 0x05
#define CMD_TURN_OFF_ALL_MOTOR 0x06

#define PWM_OFF 0
#define PWM_LEVEL_1 50
#define PWM_LEVEL_2 100
#define PWM_LEVEL_3 150
#define PWM_LEVEL_4 250

#define CMD_TURN_ON_WARM_LIGHT 0x07
#define CMD_TURN_OFF_WARM_LIGHT 0x08

#define CMD_TURN_ON_FAN 0x09
#define CMD_TURN_OFF_FAN 0x10

#define CMD_REPORT_STATE 0x11

#define CMD_SET_TIME_OUT 0x12

#define DIRECTION_INC true
#define DIRECTION_DEC false

#define DELAY_OUTPUT_PWM 20
#define DELAY_READ_SENSOR 20

#define TIMES_TO_READ_SENSOR 10

#define PWM_FOR_MOTOR 0
#define PWM_FOR_FAN 1
#define PWM_FOR_NONE 2

#endif  // _COMMONT_H_
