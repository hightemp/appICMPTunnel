.PHONY: all sniffer

all: sniffer

sniffer:
	g++ -std=c++11 sniffer.cpp -o sniffer

%: %.cpp
	g++ -std=c++11 $< -o $@

%: %.c
	gcc $< -o $@

