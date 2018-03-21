/*
 * Sensor.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: stepan
 */

#include "Sensor.hpp"


Sensor::Sensor(t_sensor_settings sset)	{
	settings.address = sset.address;
	settings.power_type = sset.power_type;
	settings.alarm = sset.alarm;
	settings.out_control = sset.out_control;
	settings.temperature = sset.temperature;
	settings.cookihg = sset.battery_monitor;
	settings.battery_monitor = sset.battery_monitor;
	settings.adc_control = sset.adc_control;
	reset_last_packet();


}

bool Sensor::push_command(t_sensor_command set_command) {
	if(command_queue.size() <= MAX_QUEUE_SENSOR_SIZE )	{
		command_queue.push(set_command);
		return true;
	}
	else return false;
}

int Sensor::get_queue_size() {
	return command_queue.size();
}

bool Sensor::pop_command(t_sensor_command* comand)	{
	if(!command_queue.empty())	{
		*comand = command_queue.front();
		command_queue.pop();
		return true;
	}
	else return false;
}

void Sensor::set_last_time(time_t time)	{

	last_time = time;
}


time_t Sensor::get_last_time()	{
	return last_time;

}
char Sensor::get_address()	{
	return settings.address;
}

void Sensor::reset_last_packet()	{
	for(unsigned char i=0; i<(SENSOR_PACKET_LENGTH-1); i++) last_packet[i]=0;
}


