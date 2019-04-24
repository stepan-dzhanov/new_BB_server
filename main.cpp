
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <time.h>
#include <ctime>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <pwd.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <mutex>

#include <string> 
#include <stdio.h>      /* printf */
#include <assert.h> 

#include <iostream>

#include "ComPort.hpp"
#include "NRF905Gate.h"
#include "protocol.h"
#include "Sensor.hpp"
#include "ScenarioEngine.hpp"




void *RF_Service(void *threadid)
{
	std::cout<<"Second Thread"<<std::endl;
	NRF905Gate *gate;
	gate = new NRF905Gate();
	while(1){
		gate->GetEvent();
	}
}






int main()
{


  key_t key =10;
  int msqid;


  message_buf_t  rbuf;

  pthread_t threads;
  
    
        
    
  
  
  
  
  int rc = pthread_create(&threads, NULL,RF_Service, 0);
    if (rc){
            std::cout << "Error:unable to create thread," << rc << std::endl;
            exit(-1);
    }
    if ((msqid = msgget(key, O_RDWR | IPC_CREAT )) < 0) {
    	    perror("msgget");
    	    exit(-1);
    	}


  sleep(1);

  t_sensor_settings sensor;
  t_sensor_command com, q;
 // command.address = 0x36;
 // sprintf(command.command,"test comand");
  sensor.address = 0x06;
  std::cout<<"Start - Sensor address:"<<sensor.address+0x30<<std::endl;
  sensor.alarm = false;
  sensor.battery_monitor = false;
  sensor.temperature = true;
  sprintf(com.command,"%ctst1\n\r",0x06);
  Sensor *alarm_sensor = new Sensor(sensor);
  //alarm_sensor->push_command(com);
  sprintf(com.command,"%ctst2\n\r",0x06);
 // alarm_sensor->push_command(com);
  sensor.address = 0x07;
  sensor.alarm = true;
  sensor.battery_monitor = true;
  sensor.temperature = false;
  Sensor *nalarm_sensor = new Sensor(sensor);
  sprintf(com.command,"%ctst72\n\r",0x07);
 /// nalarm_sensor->push_command(com);
 // std::cout<<"Sensor has been created, address:"<<nalarm_sensor->get_address()<<std::endl;
 // std::cout<<"size before "<<alarm_sensor->get_queue_size()<<std::endl;
 // bool a = alarm_sensor->push_command(command);
  //alarm_sensor->pop_command();
 // std::cout<<"push command  "<<a<<command.command<<std::endl;
 //alarm_sensor->pop_command(&q);
  //std::cout<<"pop comand  "<<q.command<<std::endl;
  //std::cout<<"size  "<<alarm_sensor->get_queue_size()<<std::endl;

  ScenarioEngine *scenario = new ScenarioEngine(10);
  scenario->attach_sensor(alarm_sensor);
  scenario->attach_sensor(nalarm_sensor);

  while(1) {
	 /* if(msgrcv(msqid, &rbuf, QUEUES_MESSAGE_SIZE, MESSAGE_TYPE_EVENT,0)>0){
		  std::cout<<rbuf.mtext<<std::endl;
		  rbuf.mtype = 6;
		  int buf_length = sizeof(message_buf_t) - sizeof(long);
		  sprintf(rbuf.mtext,"%ctst\n\r",0x06);
		  if (msgsnd(msqid, &rbuf, buf_length, 0) < 0) {
		  					perror("RF_gate_thead");
		  }

	  }
	  sleep(1);
	 		  std::cout<<"doooo"<<std::endl;*/
	  scenario->wait_event_form_gate();
	  scenario->check_scenario();
	  scenario->run_scenario();
  }

  return 0;
}
