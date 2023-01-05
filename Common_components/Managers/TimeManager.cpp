/* Project specific includes */
#include "TimeManager.hpp"

/* SDK library*/
#include <ctime>

/* SDK config */
#include "sdkconfig.h"

/* ESP log library */
#include <esp_log.h>

using namespace Component::Manager;

TimeManager *TimeManager::mManager{nullptr};
std::mutex TimeManager::mManagerMutex;

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Class constructor
 */
TimeManager::TimeManager()
{
    // Set operation mode
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

    // Set synchronization mode
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);

    RegisterTimeSyncNotificationCallback(TimeManager::TimeSync_Notification);

// Set default NTP server from config file
#ifdef CONFIG_STANDALONE_SERVER
    SetServerName(0, CONFIG_SERVER_HOST);
#elif CONFIG_MULTI_SERVER
    SetServerName(0, CONFIG_NTP_HOST);
#endif

#ifdef CONFIG_TIME_ZONE
    setenv("TZ", CONFIG_TIME_ZONE, 1);
    tzset();
#endif
}

/**
 * @brief Class destructor
 */
TimeManager::~TimeManager()
{
}

/**
 * @brief Method will be call when time synchronization arrive
 *
 * @param[in] time : Data structure returned from gettimeofday(2) system call
 */
void TimeManager::TimeSync_Notification(struct timeval *time)
{
    struct tm timeinfo = {0};

    // Convert epoch timestamp to struct tm
    localtime_r(&time->tv_sec, &timeinfo);

    // Date
    char date_str[50];
    strftime(date_str, 50, "Synchronized date set to: %d %B, %Y", &timeinfo);

    char time_str[50];
    strftime(time_str, 50, "Synchronized time set to: %T", &timeinfo);

    ESP_LOGI(TIME_MANAGER_TAG, "%s", date_str);
    ESP_LOGI(TIME_MANAGER_TAG, "%s", time_str);
}

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Static method to get instance of TimeManager
 */
TimeManager *TimeManager::GetInstance()
{
    std::lock_guard<std::mutex> lock(mManagerMutex);
    if (!mManager)
        mManager = new TimeManager();

    return mManager;
}

/**
 * @brief Initialize SNTP module and immediately send request to synchronize time
 */
void TimeManager::Initialize()
{
    sntp_init();
}

/**
 * Get actual synchronization status
 */
sntp_sync_status_t TimeManager::GetStatus() const
{
    return sntp_get_sync_status();
}

/**
 * @brief Set NTP server name
 */
void TimeManager::SetServerName(uint8_t id, const char *server)
{
    sntp_setservername(id, server);
}

/**
 * @brief Register callback for notification when time is synchronized
 */
void TimeManager::RegisterTimeSyncNotificationCallback(sntp_sync_time_cb_t callback)
{
    sntp_set_time_sync_notification_cb(callback);
}