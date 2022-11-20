#ifndef BLUETOOTH_DATA_PUBLISHER_H
#define BLUETOOTH_DATA_PUBLISHER_H

/* Common components includes */
#include "Publisher/PublisherInterface.hpp"

/* STL includes */
#include <mutex>
#include <vector>

#define BLUETOOTH_DATA_PUBLISHER_TAG "Bluetooth data publisher"

namespace Greenhouse
{
    namespace Publisher
    {
        class BluetoothDataPublisher : public Component::Publisher::Interface::PublisherInterface
        {
        public:
            // typedef for observer component interface
            using Observer_I = Component::Observer::Interface::ObserverInterface;

            /**
             * @brief Static method to get singleton instance of Bluetooth data publisher
             *
             * @return BluetoothDataPublisher : Pointer to singleton instance
             */
            static BluetoothDataPublisher *GetInstance();

            /**
             * @brief Method to subscribe observer to Bluetooth data publisher
             *
             * @param[in] observer  : Pointer to observer object
             */
            void Subscribe(Observer_I *observer) override;

            /**
             * @brief Method to subscribe observer from Bluetooth data publisher
             *
             * @param[in] observer  : Pointer to observer object
             */
            void Unsubscribe(Observer_I *observer) override;

            /**
             * @brief Method to notify observer about new data from bluetooth handler
             */
            void Notify() override;

        private:
            /**
             * @brief Class constructor
             */
            explicit BluetoothDataPublisher();

            /**
             * @brief Class destructor
             */
            ~BluetoothDataPublisher();

            /* Singleton instance of Bluetooth data pupblisher */
            static BluetoothDataPublisher *mPublisherInstance;

            /* Singleton mutex to protect instance from multithread*/
            static std::mutex mPublisherMutex;

            /* Vector of observers*/
            std::vector<Observer_I *> mObservers;
        };

    } // namespace Publisher
} // namepsace Greenhouse

#endif