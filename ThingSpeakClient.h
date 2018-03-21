/*
 * ThingSpeakClient.h
 *
 *  Created on: 22.03.2016
 *      Author: stepan
 */

#ifndef THINGSPEAKCLIENT_H_
#define THINGSPEAKCLIENT_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>


#include <netdb.h>          /* hostent struct, gethostbyname() */
#include <arpa/inet.h>      /* inet_ntoa() to format IP address */
#include <netinet/in.h>
#include <cstring>
#include <string>
#include <iostream>




















class ThingSpeakClient {
public:
	ThingSpeakClient();
	int GetField(char *s_field, char *e_field);
	void PutDataToChannel(char data, char channel);
	virtual ~ThingSpeakClient();


private:
	int sock;
	struct sockaddr_in addr;
	int pos;
	int start_pos;
	int end_pos;
	char result[3];
	int humidity;
	char buf[16000];
};

#endif /* THINGSPEAKCLIENT_H_ */
