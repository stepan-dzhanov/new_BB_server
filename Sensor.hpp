/*
 * Sensor.hpp
 *
 *  Created on: Mar 4, 2018
 *      Author: stepan
 */

#ifndef SENSOR_HPP_
#define SENSOR_HPP_
#include <ctime>
#include <queue>
#include <cstdlib>
#include <cstdio>
#include <iostream>



#define SENSOR_PACKET_LENGTH 32
#define MAX_QUEUE_SENSOR_SIZE 10

struct t_sensor_settings {
	char address;
	char power_type;
	bool alarm;
	bool out_control;
	bool temperature;
	bool cookihg;
	bool battery_monitor;
	bool adc_control;
};

typedef struct  {
	char address;
	char command[SENSOR_PACKET_LENGTH-1];

}t_sensor_command;



class Sensor {

private:

	std::queue<t_sensor_command> command_queue;
	time_t last_time;


public:
	char last_packet[SENSOR_PACKET_LENGTH];
	t_sensor_settings settings;
	Sensor(t_sensor_settings sset);
	bool push_command(t_sensor_command set_command);
	bool  pop_command(t_sensor_command* comand);
	void set_last_time(time_t time);
	void reset_last_packet();
	time_t get_last_time();
	int get_queue_size();
	char get_address();




};





#endif /* SENSOR_HPP_ */
