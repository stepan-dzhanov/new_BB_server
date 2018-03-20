






#include <ctype.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <iostream>

#include "ComPort.hpp"
using namespace std;
struct termios tio, oldtio;


ComPort::ComPort(const char *name, unsigned int baud_rate)	{

	port = OpenPort(name );
	SetPortBaudrate(baud_rate );
}

ComPort::~ComPort()	{

	ClosePort();
}



void ComPort:: Sleep( unsigned int n )	{
    struct timespec t;
    t.tv_sec = n / 1000;
    t.tv_nsec = (n % 1000) * 1000000;
    nanosleep(&t, 0);
   // millisleep( i );
}

HANDLE ComPort::OpenPort( const char *name )	{
    int fd;

    fd = open( name, O_RDWR|O_NONBLOCK, 0 );

    if( fd > 0 )
    {
        Sleep( 10 );
        fcntl( fd, F_SETFL, 0 );
    }

    if( fd < 0 ) 	{
    	cout << "Error port" << endl;
    				exit(1);
    }
	return fd;
}

int ComPort::SetPortBaudrate(  unsigned int baudRate )	{

    int baud = B9600;
    switch(baudRate)
    {
		case 4800:
			baud = B4800;
			break;
		case 9600:
				baud = B9600;
				break;
		case 19200:
				baud = B19200;
				break;
		case 38400:
				baud = B38400;
				break;
		case 57600:
				baud = B57600;
				break;
		case 115200:
				baud = B115200;
				break;
		default:
				baud = B9600;
				break;
    }




    tcgetattr( port, &oldtio );
    tio.c_iflag = IGNBRK | IGNPAR | IXOFF | 0 | 0;
    tio.c_cflag = CREAD | HUPCL | CLOCAL | CS8 | 0;
    tio.c_oflag = 0;
    tio.c_lflag = 0;
    tio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    tio.c_cc[VMIN]     = 0;
    cfsetispeed( &tio, baud );
    cfsetospeed( &tio, baud );

    if( tcsetattr( port, TCSAFLUSH, &tio ) == -1 ) return 0;
    else return 1;
}

void ComPort::ClosePort( )	{
	tcsetattr(port,TCSANOW,&oldtio);
	close( port );
}


int ComPort::ReadDataPort(  void *buff, unsigned int size )	{

        char *buffer = (char*) buff;
        int res = read( port, buffer, size );
        return res;
}

int ComPort::WriteDataPort(  const void *buff, unsigned int size )	{
	int res =0;
    res = write( port, buff, size );
    return res;
}
