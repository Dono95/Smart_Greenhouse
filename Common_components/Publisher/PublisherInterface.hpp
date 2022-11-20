#ifndef PUBLISHER_INTERFACE_H
#define PUBLISHER_INTERFACE_H

#include "Observer/ObserverInterface.hpp"

namespace Component
{
    namespace Publisher
    {
        namespace Interface
        {
            class PublisherInterface
            {
            public:
                /**
                 * @brief Virtual class destructor
                 */
                virtual ~PublisherInterface() {}

                /**
                 * @brief Pure virtual method to subscribe observer to publisher
                 *
                 * @param[in] observer  : Pointer to observer object
                 */
                virtual void Subscribe(Observer::Interface::ObserverInterface *observer) = 0;

                /**
                 * @brief Pure virtual method to unsubscribe observer from publisher
                 *
                 * @param[in] observer  : Pointer to observer object
                 */
                virtual void Unsubscribe(Observer::Interface::ObserverInterface *observer) = 0;

                /**
                 * @brief Pure virtual method to notify observer about somethind based on specific publisher
                 */
                virtual void Notify() = 0;
            };
        } // namespace Interface

    } // namespace Publisher
} // namespace Component

#endif // PUBLISHER_INTERFACE_H