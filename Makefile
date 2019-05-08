.PHONY: all icmptunnel

all: icmptunnel

icmptunnel:
	g++ -std=c++11 icmptunnel.cpp -o icmptunnel

%: %.cpp
	g++ -std=c++11 $< -o $@

%: %.c
	gcc $< -o $@

