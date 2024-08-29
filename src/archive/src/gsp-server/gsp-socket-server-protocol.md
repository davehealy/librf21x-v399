# General Serial Port Server: Usage and Protocol

## Instruction
General Serial Port Server, aka gsp-server, is a serial port redirect server for librf21x.
We have implemented gsp-server on multiple platform, such as Windows, Linux with x86 cpu, Linux With Mips cpu. With the help of gsp-server, user can access rf21x device by tcp/ip (wired network or wireless network) instead of plug in rf21x device in your target device.

### gsp-server in Windows and Linux on PC
The release of librf21x include gsp-server, so just use it by runing it.

### gsp-server in TP-Link Wireless Router (TP-Link WR703n)
TP-Link WR703n is based on a Mips chip. What's more, WR703n have support of OpenWRT.
So we use OpenWRT to put gsp-server in TP-Link WR703n. What you need is to connect to the router, the user can access by router's IP adderss with port 9000.
gsp-server in WR703n only support hid interface device.

## Usage
The Usage is very simple. Just run gsp-server, then it will serve at default 9000. You can change default by command line parameter like "gsp-server 8900".
After running gsp-server, you can access rf21x device by librf21x API.

## Low Level Procotol on TCP/IP

### Procotol Steps
- connect to server socket.
- server will read the first data packeg from client, so client need to send connection command at the this time, as follow:
  - client to server: "open <port> <baud>"
      - example 1: "open hid:// 28800"
      - example 2: "open hid:// 115200"
      - example 3: "open hid:// 256000"
  - server got command, then:
    1. check command format.
    2. Old connection in gsp-server will be close. Only one connection can exist at same time.
    3. try to open "port" by "badu", if sucess, an "OK" will be returned to client.
 - after send command and get "OK", you can write and read data by socket withoout any other process.