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
             */
            explicit Value(T value = 0) : mValue(value) {}

            /**
             * @brief Class destructor
             */
            ~Value() {}

            /**
             * @brief Set sensor value
             *
             * @param[in] value : Value
             */
            void Set(T value) { mValue = value; }

            /**
             * @brief Get sensor value
             *
             * @return float
             */
            T Get() const { return mValue; }

        private:
            T mValue;
        };
    } // namespace DataType
} // namespace Utility

#endif