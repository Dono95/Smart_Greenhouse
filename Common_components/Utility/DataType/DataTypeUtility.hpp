#ifndef DATA_TYPE_UTILITY
#define DATA_TYPE_UTILITY

namespace Utility
{
    namespace DataType
    {
        template <class T>
        class Value
        {
        public:
            /**
             * @brief Class constructor
             *
             * @param[in] value : Value
             */
            explicit Value(T value) : mValue(value), mSet(true) {}

            /**
             * @brief Class constructor with no parameter
             */
            explicit Value() : mValue(0), mSet(false) {}

            /**
             * @brief Class destructor
             */
            ~Value() {}

            /**
             * @brief Set sensor value
             *
             * @param[in] value : Value
             */
            void Set(T value) { 
                mSet = true;
                mValue = value; 
            }

            /**
             * @brief Get sensor value
             *
             * @return float
             */
            T Get() const { return mValue; }

            /**
             * @brief Check if value is set
             *
             * @return bool     : true if value is set, false otherwise
             */
            bool IsValueSet() const { return mSet; }

        private:
            // Value
            T mValue;

            // Boolean if value is set
            bool mSet;
        };
    } // namespace DataType
} // namespace Utility

#endif