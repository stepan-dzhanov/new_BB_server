#makefile
#LIBS = -lpthread

#app: main.c out.c
#	g++ -Wall -o app  main.c $(LIBS)
CC=g++ -std=c++11
CFLAGS=-c -Wall 
LDFLAGS=-lpthread
SOURCES=main.cpp ComPort.cpp NRF905Gate.cpp Sensor.cpp ScenarioEngine.cpp ThingSpeakClient.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=app

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< $(LIBS) -o $@


clean:
	rm *.o
	rm $(EXECUTABLE)
