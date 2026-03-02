CXX = g++
CXXFLAGS = -Wall -Werror

all: loadbalancer

loadbalancer: main.o webserver.o loadbalancer.o switch.o
	$(CXX) $(CXXFLAGS) -o loadbalancer main.o webserver.o loadbalancer.o switch.o

main.o: main.cpp loadbalancer.h request.h switch.h
	$(CXX) $(CXXFLAGS) -c main.cpp

webserver.o: webserver.cpp webserver.h request.h
	$(CXX) $(CXXFLAGS) -c webserver.cpp

loadbalancer.o: loadbalancer.cpp loadbalancer.h webserver.h request.h
	$(CXX) $(CXXFLAGS) -c loadbalancer.cpp

switch.o: switch.cpp switch.h loadbalancer.h request.h
	$(CXX) $(CXXFLAGS) -c switch.cpp

clean:
	rm -f loadbalancer *.o