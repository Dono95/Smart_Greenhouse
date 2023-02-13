#ifndef OBSERVER_INTERFACE_H
#define OBSERVER_INTERFACE_H

/* Common components includes  */
#include "Publisher/BasePublisherDefinitions.hpp"
#include "Publisher/EventData.hpp"

namespace Component
{
    namespace Observer
    {
        namespace Interface
        {
            class ObserverInterface
            {
            public:
                /* Typedef for observed event*/
                using Event_T = Component::Publisher::Events;

                /**
                 * @brief Class constructor
                 *
                 * @param event : Event to observer
                 */
                ObserverInterface(Event_T event) : mObserverEvent(event) {}

                /**
                 * @brief Virtual class destructor
                 */
                virtual ~ObserverInterface() {}

                /**
                 * @brief Get observer event type
                 */
                Event_T GetObservedEvent() const
                {
                    return mObserverEvent;
                }

                /**
                 * @brief Pure virtual method to update all observers with new event data
                 */
                virtual void Update(Component::Publisher::EventData *eventData) = 0;

            private:
                // Event type
                Event_T mObserverEvent;
            };
        } // namespace Interface

    } // namespace Observer
} // namespace Component

#endif // OBSERVER_INTERFACE_H