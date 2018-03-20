/*
 * ComPort.hpp
 *
 *  Created on: 29.10.2011
 *      Author: stepan
 */

#ifndef COMPORT_HPP_
#define COMPORT_HPP_

typedef int HANDLE;

class ComPort	{

public:

	ComPort(const char *name, unsigned int baud_rate);
	~ComPort();
	int ReadDataPort(  void *buff, unsigned int size );
	int WriteDataPort( const void *buff, unsigned int size );

private:

	void Sleep( unsigned int n );
	int SetPortBaudrate(  unsigned int baudRate );
	void ClosePort();
	HANDLE OpenPort( const char *name );
    HANDLE port;


};



#endif /* COMPORT_HPP_ */
