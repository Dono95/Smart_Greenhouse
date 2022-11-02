set(SOURCES 
./main.cpp
./GreenhouseManager.cpp)

#./Bluetooth/BluetoothController.cpp
#./Bluetooth/ServerBluetoothHandler.cpp
#./Drivers/Com_Interfaces/I2C_I.cpp
#./Drivers/Sensors/SHT4x.cpp

idf_component_register(SRCS ${SOURCES})

