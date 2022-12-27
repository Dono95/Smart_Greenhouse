#ifndef PUBLISHER_INTERFACE_H
#define PUBLISHER_INTERFACE_H

/* Common components includes */
#include "BasePublisherDefinitions.hpp"
#include "Observer/ObserverInterface.hpp"
#include "EventData.hpp"

namespace Component
{
    namespace Publisher
    {
        namespace Interface
        {
            class PublisherInterface
            {
            public:
                /* Typedef for Publisher events */
                using Event_T = Component::Publisher::Events;

                /* Typedef for Observer pointer */
                using Observer_T = Component::Observer::Interface::ObserverInterface;

                /**
                 * @brief Virtual class destructor
                 */
                virtual ~PublisherInterface() {}

                /**
                 * @brief Pure virtual method to subscribe observer to publisher
                 *
                 * @param[in] observer  : Pointer to observer object
                 */
                virtual void Subscribe(Event_T event, Observer_T *observer) = 0;

                /**
                 * @brief Pure virtual method to unsubscribe observer from publisher
                 *
                 * @param[in] observer  : Pointer to observer object
                 */
                virtual void Unsubscribe(Event_T event, Observer_T *observer) = 0;

                /**
                 * @brief Pure virtual method to notify observer about somethind based on specific publisher
                 */
                virtual void Notify(Event_T event, EventData *eventData) = 0;
            };
        } // namespace Interface

    } // namespace Publisher
} // namespace Component

#endif // PUBLISHER_INTERFACE_H