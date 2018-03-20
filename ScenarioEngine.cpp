/*
 * ScenarioEngine.cpp
 *
 *  Created on: Mar 18, 2018
 *      Author: stepan
 */

#include "ScenarioEngine.hpp"
#include "Sensor.hpp"

ScenarioEngine::ScenarioEngine(key_t queue_key)	{

	if ((msqid = msgget(queue_key, O_RDWR | IPC_CREAT )) < 0) {
	    	    perror("ScenarioEngine:can not connect to message queue");
	    	    exit(-1);
	}
}

bool ScenarioEngine::attach_sensor(Sensor* sns)	{

	if(sensor_pool.size()<=SENSOR_POOL_LIMIT)	{
		sensor_pool.push_back(*sns);
        return true;
	}
	else return false;
}

void ScenarioEngine::wait_event_form_gate()	{

	if(msgrcv(msqid, &rbuf, QUEUES_MESSAGE_SIZE, MESSAGE_TYPE_EVENT,0)>0){
		memcpy(event_str,rbuf.mtext,QUEUES_MESSAGE_SIZE);

		std::cout<<"Data form sensor:"<<event_str<<std::endl;

	}

}

bool ScenarioEngine::check_scenario()	{
	t_sensor_command command_from_fifo;
	int buf_length;
	const char *door_str = "door";
	const char *bt_str = "bt";
	const char *button_str = "button";
	active_sensor_addr = 0;
	active_sensor_event =0;

	active_sensor_addr = event_str[3];

	std::cout<<"Sensor`s data recived, address:"<<(active_sensor_addr)<<std::endl;

	if(!memcmp(door_str,&event_str[5],4)) active_sensor_event = ALARM_EVENT;
    if(!memcmp(button_str,&event_str[5],6))	active_sensor_event = ALARM_EVENT;
    if(!memcmp(bt_str,&event_str[6],2))active_sensor_event = BATTERY_MONITOR_EVENT;

	std::cout<<"Sensor event:"<<active_sensor_event<<std::endl;

	std::cout<<"Pool size"<<sensor_pool.size()<<std::endl;

    for(unsigned int i=0; i<sensor_pool.size(); i++)	{
    	std::cout<<"Checking sensor"<<sensor_pool[i].get_address()<<std::endl;
    	if(sensor_pool[i].settings.address == active_sensor_addr)	{
    	memcpy(sensor_pool[i].last_packet, event_str,QUEUES_MESSAGE_SIZE);// Copy data from gate to sensor object
    		std::cout<<"Found a sensor in pool"<<std::endl;
    		std::cout<<"Sensor battery:"<<event_str[5]<<std::endl;
    		//if(active_sensor_event == BATTERY_MONITOR_EVENT)
    			if(sensor_pool[i].get_queue_size() == 0)	{         // if sensor`s queue is empty
    				rbuf.mtype = active_sensor_addr;
    			    buf_length = sizeof(message_buf_t) - sizeof(long);
    				sprintf(rbuf.mtext,"%ctst\n\r",active_sensor_addr);
    				std::cout<<"Sending command to sensor:"<<active_sensor_addr<<std::endl;
    				if (msgsnd(msqid, &rbuf, buf_length, 0) < 0) {
    					perror("ScenarioEngine:can not sent message");
    				}
    			}
    			else {                                               // sensor`s queue is NOT empty
    				sensor_pool[i].pop_command(&command_from_fifo);  // reading command from sensor fifo
    				std::cout<<"Pop command from sensor:"<<command_from_fifo.command<<std::endl;
    				rbuf.mtype = active_sensor_addr;
    				buf_length = sizeof(message_buf_t) - sizeof(long);
    				memcpy(rbuf.mtext,command_from_fifo.command,SENSOR_PACKET_LENGTH);
    				std::cout<<"Sending command to sensor from fifo:"<<rbuf.mtext<<std::endl;
    				if (msgsnd(msqid, &rbuf, buf_length, 0) < 0) {   // sending command from fifo to sensor
    					perror("ScenarioEngine:can not sent message");
    				}
    			}


    	}

    }


}

void ScenarioEngine::run_scenario()	{

	for(unsigned int i=0; i<sensor_pool.size(); i++)	{
		if(sensor_pool[i].settings.temperature) scenario_temperature(&sensor_pool[i]);
		if(sensor_pool[i].settings.battery_monitor) scenario_battery(&sensor_pool[i]);
		if(sensor_pool[i].settings.alarm) scenario_alarm(&sensor_pool[i]);
	}

}


bool ScenarioEngine::scenario_temperature(Sensor* sens)	{
	std::cout<<"Scenario TEMPERATURE run, sensor:"<<sens->settings.address<<std::endl;
	return true;
}
bool ScenarioEngine::scenario_battery(Sensor* sens)	{
	std::cout<<"Scenario BATTERY run, sensor:"<<sens->settings.address<<std::endl;
	return true;
}
bool ScenarioEngine::scenario_alarm(Sensor* sens)	{
	std::cout<<"Scenario ALARM run, sensor:"<<sens->settings.address<<std::endl;
	return true;
}

