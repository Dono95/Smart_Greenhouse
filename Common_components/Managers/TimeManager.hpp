#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

/* STD library */
#include <mutex>
#include <string>

/* ESP SNTP library */
#include <esp_sntp.h>

#define TIME_MANAGER_TAG "Time manager"

namespace Component
{
    namespace Manager
    {
        class TimeManager
        {
        public:
            /**
             * @brief Static method to get instance of TimeManager
             *
             * @return TimeManager    : Pointer to Time manager
             */
            static TimeManager *GetInstance();

            /**
             * @brief Initialize SNTP module and immediately send request to synchronize time
             */
            void Initialize();

            /**
             * Get actual synchronization status
             *
             * @return sntp_sync_status_t : Synchronization status
             */
            sntp_sync_status_t GetStatus() const;

            /**
             * @brief Get Unix timestamp
             *
             * @return time_t : POSIX time
             */
            time_t GetRawTime() const;

            /**
             * @brief Get time structure
             *
             * @return struct tm : Time structure
             */
            struct tm GetTime(const time_t *rawTime) const;

            /**
             * @brief Get formatted time HH:MM:SS
             *
             * @return std::string : Formatted time
             */
            std::string GetTime_String(time_t *rawTime = nullptr) const;

            /**
             * @brief Get formatted date DD MONTH, YYYY
             *
             * @return std::string : Formatted date
             */
            std::string GetDate_String(time_t *rawTime = nullptr) const;

            /**
             * @brief Set NTP server name
             *
             * @param[in] id        : Server ID
             * @param[in] server    : Server name
             */
            void SetServerName(uint8_t id, const char *server);

            /**
             * @brief Register callback for notification when time is synchronized
             *
             * @param[in] callback - Callback function
             */
            void RegisterTimeSyncNotificationCallback(sntp_sync_time_cb_t callback);

        private:
            /**
             * @brief Class constructor
             */
            explicit TimeManager();

            /**
             * @brief Class destructor
             */
            ~TimeManager();

            /**
             * @brief Method will be call when time synchronization arrive
             *
             * @param[in] time : Data structure returned from gettimeofday(2) system call
             */
            static void TimeSync_Notification(struct timeval *time);

            /* Singleton instance of Time manager*/
            static TimeManager *mManager;

            /* Singleton mutex to protect instance from multithread*/
            static std::mutex mManagerMutex;
        };
    } // Manager
} // Component

#endif // TIME_MANAGER_H