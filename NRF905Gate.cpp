/*
 * NRF905Gate.cpp
 *
 *  Created on: Sep 23, 2016
 *      Author: stepan
 */

#include "NRF905Gate.h"
#include "protocol.h"
using namespace std;



NRF905Gate::NRF905Gate()	{

	str_counter=0;
	gate_port = new ComPort("/dev/ttyS4", 9600);
	if (gate_port<=0)	{
		perror ("Error COM port");
		exit (-1);
	}
	key = QUEUES;
	if ((msqid = msgget(key, O_RDWR | IPC_CREAT )) < 0) {
	    perror("msgget");
	    exit(-1);
	}
	a =0;
	pointer_data_from_gate=0;
	fid = open("new_sensor_log.txt", O_RDWR | O_CREAT );
    if (fid<0){
	   	 perror("nRF905_fopen");
    }
    flg=0;



}

NRF905Gate::~NRF905Gate()	{
	;
}

bool NRF905Gate::GetEvent ()	{


	bytes_from_gate = gate_port->ReadDataPort( tmp_data , 64);
	if ((bytes_from_gate>0)){
		memcpy(&data_from_gate[pointer_data_from_gate],tmp_data,bytes_from_gate);
		pointer_data_from_gate += bytes_from_gate;
	}
	if (pointer_data_from_gate>=34){
		for (i=0; i<128; i++) { if ( (data_from_gate[i]==0x0D)&&(data_from_gate[i-1]==0x0A) ) break;}

		if (i<=33) {
			if (i==33){
				sprintf(command,"%ctst\n\r",data_from_gate[3]);
				AddSensorToLog(data_from_gate[3],data_from_gate[5]);// TBD full string from sensor
				rbuf.mtype = MESSAGE_TYPE_EVENT;
				memcpy(rbuf.mtext,data_from_gate, 32);
				//cout<<rbuf.mtext<<'\n';
				int buf_length = sizeof(message_buf_t) - sizeof(long);
				if (msgsnd(msqid, &rbuf, buf_length, 0) < 0) {
					perror("RF_gate_thead");
				}
				usleep(300000);
				if(msgrcv(msqid, &rbuf, QUEUES_MESSAGE_SIZE, data_from_gate[3], IPC_NOWAIT)>0)	{
					memcpy(command,rbuf.mtext,32);
				    gate_port->WriteDataPort(command,GetMessageLenght());


                    cout<<"QUEES "<<command<<'\n';
					usleep(500000);
					bytes_from_gate = gate_port->ReadDataPort( tmp_data , 64);

				}
			}
			memcpy(tmp_data,&data_from_gate[i+1],pointer_data_from_gate-(i+1));
			memcpy(data_from_gate,tmp_data,pointer_data_from_gate-i);
			pointer_data_from_gate = pointer_data_from_gate-i-1;


		}

		return 1;

	}
	return 0;

}

void NRF905Gate::AddSensorToLog(char _id, char _bat )	{

     char buff[64];
	 long int s_time;
	 struct tm m_time;
     s_time = time (NULL);
     localtime_r (&s_time, &m_time);

     str_counter++;
     if (str_counter>1200){
    	 str_counter=0;
    	 close(fid);
    	 fid = open("new_sensor_log.txt", O_RDWR | O_CREAT );
    	     if (fid<0){
    	 	   	 perror("nRF905_fopen");
    	 }


     }


     sprintf(buff,"Sensor-");
     write(fid, buff, strlen(buff));
     sprintf(buff,"%d",_id);
     write(fid, buff, strlen(buff));
     sprintf(buff,"Battery-");
     write(fid, buff, strlen(buff));

     write(fid, &_bat, 1);
     sprintf(buff," Time-");
     write(fid, buff, strlen(buff));
     sprintf(buff,"%d",(char)m_time.tm_mday);
     write(fid, buff, strlen(buff));
     sprintf(buff,".");
     write(fid, buff, strlen(buff));
     sprintf(buff,"%d",(char)m_time.tm_hour);
     write(fid, buff, strlen(buff));
     sprintf(buff,":");
     write(fid, buff, strlen(buff));
     sprintf(buff,"%d",(char)m_time.tm_min);
     write(fid, buff, strlen(buff));
     sprintf(buff,"\n");
     write(fid, buff, strlen(buff));



}
char NRF905Gate::GetMessageLenght()	{

	for(int i=0; i<32; i++){
		if ((command[i]==0x0D)&&(command[i-1]==0x0A)) return i+1;

	}
	return 0;




}
