/*
 * ScenarioEngine.hpp
 *
 *  Created on: Mar 18, 2018
 *      Author: stepan
 */

#ifndef SCENARIOENGINE_HPP_
#define SCENARIOENGINE_HPP_

#include <ctime>
#include <queue>
#include <cstdlib>
#include <cstdio>
#include <iostream>

 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/msg.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "Sensor.hpp"
#include "protocol.h"

enum event { TEMPERAUTRE_EVENT, ALARM_EVENT, OUT_CONTROL_EVENT, COOKING_EVENT, BATTERY_MONITOR_EVENT,ADC_CONTROL_EVENT };

#define SENSOR_POOL_LIMIT 10

class ScenarioEngine {

public:
	ScenarioEngine(key_t queue_key);
	bool attach_sensor(Sensor* sns);
	void wait_event_form_gate();
	bool check_scenario();
	void run_scenario();


private:
	std::vector <Sensor> sensor_pool;
	char event_str[SENSOR_PACKET_LENGTH];
	message_buf_t command_to_gate, rbuf;
	key_t key;
	int msqid;
	char active_sensor_addr;
	char active_sensor_event;

	bool scenario_temperature(Sensor* sens);
	bool scenario_battery(Sensor* sens);
	bool scenario_alarm(Sensor* sens);






};



#endif /* SCENARIOENGINE_HPP_ */
