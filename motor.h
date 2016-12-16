/*
 * motor.h
 *
 */

#ifndef MOTOR_H_
#define MOTOR_H_

void configure_timer(void); //timer configuration for motor movement
void configure_pins(void);
void start_alt_motor(void);
void stop_alt_motor(void);
void start_azi_motor(void);
void stop_azi_motor(void);
void toggle_laser(void);



#endif /* MOTOR_H_ */
