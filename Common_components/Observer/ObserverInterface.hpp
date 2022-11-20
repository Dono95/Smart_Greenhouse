#ifndef OBSERVER_INTERFACE
#define OBSERVER_INTERFACE

namespace Component
{
    namespace Observer
    {
        namespace Interface
        {
            class ObserverInterface
            {
            public:
                /**
                 * @brief Virtual class destructor
                 */
                virtual ~ObserverInterface();
            };
        } // namespace Interface

    } // namespace Observer
} // namespace Component

#endif // OBSERVER_INTERFACE_H