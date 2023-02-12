set(SOURCES 
./main.cpp
./GreenhouseManager.cpp
./ClientStatusIndicator)

# Include for common code; need to be reconsidered
idf_component_register(SRCS ${SOURCES})

