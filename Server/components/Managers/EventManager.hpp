#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

/* Common components includes */
#include "Publisher/PublisherInterface.hpp"

/* STL includes */
#include <mutex>
#include <unordered_set>
#include <map>

#define EVENT_MANAGER_TAG "EventManager"

namespace Greenhouse
{
    namespace Manager
    {
        class EventManager : public Component::Publisher::Interface::PublisherInterface
        {
        public:
            // typedef for observer component interface
            using Observer_T = Component::Observer::Interface::ObserverInterface;

            // typed for component publisher event
            using Event_T = Component::Publisher::Events;

            /**
             * @brief Static method to get singleton instance of Event manager
             *
             * @return EventManager : Pointer to singleton instance
             */
            static EventManager *GetInstance();

            /**
             * @brief Method to subscribe observer to EventManager
             *
             * @param[in] observer  : Pointer to observer object
             */
            void Subscribe(Event_T event, Observer_T *observer) override;

            /**
             * @brief Method to unsubscribe observer from EventManager
             *
             * @param[in] observer  : Pointer to observer object
             */
            void Unsubscribe(Event_T event, Observer_T *observer) override;

            /**
             * @brief Method to notify observer about new event from E
             */
            void Notify(Event_T event, Component::Publisher::EventData *eventData) override;

        private:
            /* Map consist of pointer to observers follow specific event*/
            using ObserversEventMap = std::map<Event_T, std::unordered_set<Observer_T *>>;

            /**
             * @brief Class constructor
             */
            explicit EventManager();

            /**
             * @brief Class destructor
             */
            ~EventManager();

            /* Singleton instance of Event manager */
            static EventManager *mManagerInstance;

            /* Singleton mutex to protect instance from multithread */
            static std::mutex mManagerMutex;

            /* Observers events map */
            ObserversEventMap mObservers;
        };
    } // namespace Manager
} // namepsace Greenhouse

#endif // EVENT_MANAGER_H