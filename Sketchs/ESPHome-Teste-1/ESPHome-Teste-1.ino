<<<<<<< HEAD
esphome:
  name: living_room_nodemcu_temphum_sensor
  platform: ESP32
  board: nodemcuv3

esphome:
  name: sensor_node1
  platform: ESP32
  board: nodemcuv3

wifi:
  ssid: 'MOTOROLA-A7CE9'
  password: '3104e43ece65b0885fa9'
  manual_ip:
    static_ip: 192.168.0.20  change this IP and remove this message
    gateway: 192.168.0.1
    subnet: 255.255.255.0
    dns1: 8.8.8.8
    dns2: 8.8.4.4

captive_portal:

# Enable logging
logger:

# Enable Home Assistant API
api:
  password: "mypass"

ota:
  password: "mypass"

sensor:
  - platform: dht
    pin: 4
    temperature:
      name: "Living Room Temperature"
    humidity:
      name: "Living Room Humidity"
    update_interval: 30s
    model: DHT22
=======
esphome:
  name: living_room_nodemcu_temphum_sensor
  platform: ESP32
  board: nodemcuv3

esphome:
  name: sensor_node1
  platform: ESP32
  board: nodemcuv3

wifi:
  ssid: 'MOTOROLA-A7CE9'
  password: '3104e43ece65b0885fa9'
  manual_ip:
    static_ip: 192.168.0.20  change this IP and remove this message
    gateway: 192.168.0.1
    subnet: 255.255.255.0
    dns1: 8.8.8.8
    dns2: 8.8.4.4

captive_portal:

# Enable logging
logger:

# Enable Home Assistant API
api:
  password: "mypass"

ota:
  password: "mypass"

sensor:
  - platform: dht
    pin: 4
    temperature:
      name: "Living Room Temperature"
    humidity:
      name: "Living Room Humidity"
    update_interval: 30s
    model: DHT22
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
