#ifndef OBSERVER_INTERFACE_H
#define OBSERVER_INTERFACE_H

/* Common components includes  */
#include "Publisher/BasePublisherDefinitions.hpp"

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
                 */
                ObserverInterface(Event_T event) : mObserverEvent(event) {}

                /**
                 * @brief Virtual class destructor
                 */
                virtual ~ObserverInterface() {}

                /**
                 * @brief
                 */
                Event_T GetObservedEvent() const
                {
                    return mObserverEvent;
                }

                Event_T mObserverEvent;
            };
        } // namespace Interface

    } // namespace Observer
} // namespace Component

#endif // OBSERVER_INTERFACE_H