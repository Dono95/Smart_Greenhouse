#include "MQTT_Client.hpp"

using namespace Utility::Network;

/**
 * @brief Class constructor
 */
MQTT_Client::MQTT_Client(const std::string &uri, const std::string &clientName)
{
  // Create clear config structure
  mConfig = (esp_mqtt_client_config_t *)calloc(1, sizeof(esp_mqtt_client_config_t));

  // Allocate memory and set URI to config structure
  mConfig->uri = (const char *)malloc(uri.size());
  mConfig->uri = uri.c_str();

  // Allocate memory and set client name
  mConfig->client_id = (const char *)malloc(clientName.size());
  mConfig->client_id = clientName.c_str();

  // Allocate memory and set mqtt username
  mConfig->username = (const char *)malloc(strlen(CONFIG_MQTT_USERNAME));
  mConfig->username = CONFIG_MQTT_USERNAME;

  // Allocate memory and set mqtt password
  mConfig->password = (const char *)malloc(strlen(CONFIG_MQTT_PASSWORD));
  mConfig->password = CONFIG_MQTT_PASSWORD;

  mClient = esp_mqtt_client_init(mConfig);
}

/**
 * @brief Class destructor
 */
MQTT_Client::~MQTT_Client()
{
  // Free memory
  free((char *)mConfig->uri);
  free((char *)mConfig->client_id);

  free((char *)mConfig->username);
  free((char *)mConfig->password);

  free(mConfig);

  esp_mqtt_client_destroy(mClient);
}

/**
 * @brief Register MQTT event
 */
esp_err_t MQTT_Client::RegisterEventHandler(esp_mqtt_event_id_t event,
                                            esp_event_handler_t eventHandler,
                                            void *eventHandlerArg) const
{
  return esp_mqtt_client_register_event(mClient, event, eventHandler, eventHandlerArg);
}

/**
 * @brief Start MQTT client
 */
esp_err_t MQTT_Client::Start() const
{
  return esp_mqtt_client_start(mClient);
}

/**
 * @brief Stop MQTT client
 */
esp_err_t MQTT_Client::Stop() const
{
  return esp_mqtt_client_stop(mClient);
}

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
int MQTT_Client::Publish(const std::string &topic, const std::string &data, int QoS, bool retain)
{
  return esp_mqtt_client_publish(mClient, topic.c_str(), data.c_str(), data.size(), QoS, retain);
}

/**
 * @brief Client subscribe defined topic
 */
int MQTT_Client::Subscribe(const std::string &topic, int QoS)
{
  return esp_mqtt_client_subscribe(mClient, topic.c_str(), QoS);
}