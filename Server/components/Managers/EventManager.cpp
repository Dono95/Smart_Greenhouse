/* Project specific includes */
#include "EventManager.hpp"

/* ESP log library includes */
#include "esp_log.h"

/* STL library includes */
#include <algorithm>
#include <utility>

using namespace Greenhouse::Manager;

EventManager *EventManager::mManagerInstance{nullptr};
std::mutex EventManager::mManagerMutex;

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Class constructor
 */
EventManager::EventManager()
{
}

/**
 * @brief Class destructor
 */
EventManager::~EventManager()
{
}

/*********************************************
 *              PUBLIC API                   *
 ********************************************/
/**
 * @brief Static method to get singleton instance of Event manager
 */
EventManager *EventManager::GetInstance()
{
    std::lock_guard<std::mutex> lock(mManagerMutex);
    if (!mManagerInstance)
        mManagerInstance = new EventManager();

    return mManagerInstance;
}

/**
 * @brief Method to subscribe observer to EventManager
 *
 * @param[in] observer  : Pointer to observer object
 */
void EventManager::Subscribe(Event_T event, Observer_T *observer)
{
    if (!observer)
        return;

    auto eventObserversItr = mObservers.find(event);
    if (eventObserversItr == mObservers.end())
        mObservers.insert({event, {observer}});
    else
        eventObserversItr->second.emplace(observer);

    ESP_LOGD(EVENT_MANAGER_TAG, "Observer is now subscribing.");
}

/**
 * @brief Method to unsubscribe observer from EventManager
 *
 * @param[in] observer  : Pointer to observer object
 */
void EventManager::Unsubscribe(Event_T event, Observer_T *observer)
{
    if (!observer)
        return;

    auto observersMapItr = mObservers.find(event);
    if (observersMapItr == mObservers.end())
    {
        ESP_LOGD(EVENT_MANAGER_TAG, "Provided event not found in observers map.");
        return;
    }

    auto observersEventSetItr = observersMapItr->second.find(observer);
    if (observersEventSetItr == observersMapItr->second.end())
    {
        ESP_LOGD(EVENT_MANAGER_TAG, "Provided observer not found in observers map.");
        return;
    }

    observersMapItr->second.erase(observersEventSetItr);
    ESP_LOGD(EVENT_MANAGER_TAG, "Observer is no longer subscribe.");
}

/**
 * @brief Method to notify observer about new data from bluetooth handler
 */
void EventManager::Notify(Event_T event, Component::Publisher::EventData *eventData)
{
    ESP_LOGI(EVENT_MANAGER_TAG, "Notify about %s event.", Component::Publisher::EnumToString(event).c_str());

    auto eventObservers = mObservers.find(event);
    if (eventObservers == mObservers.end())
    {
        ESP_LOGE(EVENT_MANAGER_TAG, "Provided event not found in observers map.");
        return;
    }

    if (eventObservers->second.empty())
    {
        ESP_LOGW(EVENT_MANAGER_TAG, "No observers found to have interested about event");
        return;
    }

    for (const auto &observer : eventObservers->second)
        observer->Update(eventData);
}