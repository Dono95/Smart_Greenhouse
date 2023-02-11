#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

/* STD library */
#include <string>

/* ESP MQTT library */
#include <mqtt_client.h>

/* SDK config file */
#include "sdkconfig.h"

#define MQTT_CLIENT_TAG "MQTT client"

namespace Utility
{
  namespace Network
  {
    class MQTT_Client
    {
    public:
      /**
       * @brief Class constructor
       */
      explicit MQTT_Client(const std::string &uri, const std::string &clientName = CONFIG_MQTT_CLIENT_NAME);

      /**
       * @brief Class destructor
       */
      ~MQTT_Client();

      /**
       * @brief Register MQTT event
       *
       * @param[in] event             : MQTT event type
       * @param[in] eventHandler      : Handler callback
       * @param[in] eventHandlerArg   : Handler context data
       *
       * @return esp_err_t    ESP_OK              : Success
       *                      ESP_ERR_INVALID_ARG : Wrong inicialization
       *                      ESP_ERR_NO_MEM      : Failed to allocate
       */
      esp_err_t RegisterEventHandler(esp_mqtt_event_id_t event, esp_event_handler_t eventHandler, void *eventHandlerArg) const;

      /**
       * @brief Start MQTT client
       *
       * @return esp_err_t    ESP_OK              : Success
       *                      ESP_ERR_INVALID_ARG : Wrong inicialization
       *                      ESP_FAIL            : Client is in invalid state
       */
      esp_err_t Start() const;

      /**
       * @brief Stop MQTT client
       *
       * @return esp_err_t    ESP_OK              : Success
       *                      ESP_ERR_INVALID_ARG : Wrong inicialization
       *                      ESP_FAIL            : Client is in invalid state
       */
      esp_err_t Stop() const;

      /**
       * @brief Client publish message to MQTT broker
       *
       * @param[in] topic  : MQTT topic
       * @param[in] data   : Data
       * @param[in] QoS    : Quality of Service
       * @param[in] retain : Retain flag (Default false)
       *
       * @return int  : Message ID
       */
      int Publish(const std::string &topic, const std::string &data, int QoS, bool retain = false);

      /**
       * @brief Client subscribe defined topic
       *
       * @param[in] topic  : Topic to subscribe
       * @param[in] QoS    : Quality of Service
       */
      int Subscribe(const std::string &topic, int QoS);

    private:
      // Pointer to client MQTT configuration file
      esp_mqtt_client_config_t *mConfig;

      // MQTT client
      esp_mqtt_client_handle_t mClient;
    };
  } // namespace Network
} // namespace Utility

#endif