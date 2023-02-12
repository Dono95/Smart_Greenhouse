#ifndef STEP_MOTOR_DRIVER_H
#define STEP_MOTOR_DRIVER_H

/* ESP library */
#include "driver/gpio.h"

/* STD library */
#include <vector>
#include <bitset>

#define FULL_CIRCLE_ANGLE 360
#define CLOCKWISE_STEPS 518
#define COUNTER_CLOCKWISE_STEPS 517

#define TIME_DELAY 1

namespace Component
{
  namespace Driver
  {
    namespace Motor
    {
      class StepMotor
      {
      public:
        /**
         * @brief Class constructor
         *
         * @param[in] coilA
         * @param[in] coilB
         * @param[in] coilC
         * @param[in] coilD
         */
        explicit StepMotor(gpio_num_t coilA, gpio_num_t coilB, gpio_num_t coilC, gpio_num_t coilD);

        /**
         * @brief Class constructor
         *
         * @param[in] coilA
         * @param[in] coilB
         * @param[in] coilC
         * @param[in] coilD
         */
        explicit StepMotor(int coilA, int coilB, int coilC, int coilD);

        /**
         * @brief Class destructor
         */
        ~StepMotor();

        /**
         * @brief Rotate motor in clockwise direction
         *
         * @param[in] rotateAngle - Rotation angle
         */
        void RotateClockwise(uint16_t rotateAngle = FULL_CIRCLE_ANGLE);

        /**
         * @brief Rotate motor in counter clockwise direction
         *
         * @param[in] rotateAngle - Rotation angle
         */
        void RotateCounterClockWise(uint16_t rotateAngle = FULL_CIRCLE_ANGLE);

      private:
        /**
         * @brief Do one step with motor with specific iterator
         *
         * @param[in] begin   : Beggining of movement
         * @param[in] end     : End of movement
         */
        template <typename Itr>
        void DoOneStep(const Itr begin, const Itr end);

        /**
         * @brief Turn off all coils
         */
        void TurnOffCoils();

        /**
         * @brief Inline method to calculate steps based on input rotation angle
         *
         * @param[in] fullCircleSteps : Steps for full circle
         * @param[in] rotateAngle     : Rotation angle
         *
         * @return uint16_t : Calculated steps
         */
        inline uint16_t CalculateSteps(uint16_t fullCircleSteps, uint16_t rotateAngle);

        /**
         * @brief Check operation condition
         *
         * @return bool
         */
        bool CheckOperationConditions();

        /* Vector of motor coils */
        std::vector<gpio_num_t> mCoils;
        /* Motor movement */
        std::vector<std::bitset<4>> mMovement = {8, 12, 4, 6, 2, 3, 1, 9};
      };
    } // namespace Motor

  } // namespace Driver
} // namespace Component

#endif
