# WEBSERVER

  LWIP RAW MODE
  Start PHY autonegotiation
  Waiting for PHY to complete autonegotiation.
  autonegotiation complete
  link speed for phy address 0: 1000
  
# CONFIGURING
  
  default IP of 192.168.1.10
  BOARD ZEDBOARD
  Board IP:       192.168.1.10
  Netmask :       255.255.255.0
  Gateway :       192.168.1.1

              Server   Port Connect With..
-------------------- ------ --------------------
         echo server      7 $ telnet <board_ip> 7
       rxperf server   5001 $ iperf -c <board ip> -i 5 -t 100
       txperf client    N/A $ iperf -s -i 5 -w 64k (on host with IP 192.168.1.100)
         tftp server     69 $ tftp -i 192.168.1.10 PUT <source-file>
         http server     80 Point your web browser to http://192.168.1.10