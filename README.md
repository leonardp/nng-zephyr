```
git clone https://github.com/leonardp/nng-zephyr
cd nng-zephyr
git clone -b zephyr https://github.com/leonardp/nng
cd ..
west build -b native_posix_64 -s nng-zephyr -p
west build -b native_posix_64 -s nng-zephyr -t run

*** Booting Zephyr OS build v2.6.0-rc1-320-g374629af906a  ***
starting threads
starting pong
starting ping
pong: received PING
ping: received PONG
pong: received PING
ping: received PONG
pong: received PING
ping: received PONG
pong: received PING
ping: received PONG
pong: received PING
ping: received PONG
pong: received PING
ping: received PONG
pong: received PING
ping: received PONG
pong: received PING
ping: received PONG
pong: received PING
ping: received PONG
pong: received PING
pong: last error: Hunky dory
ping: received PONG
ping: last error: Timed out
```
