#ifndef BYTE_CONVERTOR_H
#define BYTE_CONVERTOR_H

/* C library */
#include <cstdint>

/* STD library */
#include <vector>

namespace Component
{
    namespace Convertor
    {
        class ByteConvertor
        {
        public:
            /**
             * @brief Class constructor
             */
            explicit ByteConvertor() = default;

            /**
             * @brief Class destructor
             */
            ~ByteConvertor() = default;

            /**
             * @brief Template method to convert number to Type parts
             *
             * @param[in] number : Number to convert
             * @param[in] bytes  : Define valid byte's parts for number parameter
             *
             * @return std::vector<Type>
             */
            template <typename Type>
            std::vector<Type> ConvertTo(uint64_t number, const Type bytes)
            {
                // Create empty vector
                std::vector<Type> data;

                // Check convert range
                if (!(bytes >= 1 && bytes <= (8 / sizeof(Type))))
                    return data;

                data.reserve(bytes);
                number <<= (8 - bytes) * 8;

                // Calculate shift
                const auto shift = CalculateShift<decltype(number), Type>();
                // Create constant for AND operation
                const auto const_and = static_cast<decltype(number)>(0xFF) << shift;

                printf("Shift %llu\n", static_cast<long long>(shift));
                printf("Const %llu\n", static_cast<long long>(const_and));

                for (Type i = 0; i < bytes; ++i)
                {
                    data.emplace_back((number & const_and) >> shift);
                    number <<= sizeof(Type) * 8;
                }

                return data;
            }

        private:
            template <typename NumberType, typename Type>
            inline uint8_t CalculateShift()
            {
                return (sizeof(NumberType) * 8) - (8 * sizeof(Type));
            }

            template <typename ConstType, typename ByteType>
            inline ConstType CreateConstAND()
            {
                ConstType const_and{0};

                for (ByteType i = 0; i < sizeof(ByteType); ++i)
                {
                    const_and <<= 8;
                    const_and |= 0xFF;
                }

                return const_and;
            }
        };
    } // namespace Convertor
} // namespace Component

#endif