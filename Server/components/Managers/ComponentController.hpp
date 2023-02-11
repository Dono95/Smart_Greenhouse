#ifndef COMPONENT_CONTROLLER_h
#define COMPONENT_CONTROLLER_h

/* Commmon conponents */
#include "Common_components/Drivers/Motor/StepMotor.hpp"
#include "Common_components/Drivers/Active/WaterPump.hpp"

/* STD library */
#include <mutex>

#define COMPONENT_CONTROLLER_TAG "ComponentController"

namespace Greenhouse
{
  namespace Manager
  {
    class ComponentController
    {
    public:
      /**
       * @brief Static method to get singleton instance of component controller
       *
       * @return ComponentController : POinter to singleton instance
       */
      static ComponentController *GetInstance();

      /**
       * @brief Open window method
       */
      void OpenWindow();

      /**
       * @brief Open window
       *
       * @param[in] percentage : openness in percentage (50 -> the window will open halfway)
       */
      void OpenWindow(uint8_t percentage);

      /**
       * @brief Close window method
       */
      void CloseWindow();

      /**
       * @brief Method to turn on irrigation
       */
      void TurnOnIrrigation();

      /**
       * @brief Method to turn off irrigation
       */
      void TurnOffIrrigation();

    private:
      /**
       * @brief Class constructor
       */
      explicit ComponentController();

      /**
       * @brief Class destructor
       */
      ~ComponentController();

      /* Singleton instance of component controller */
      static ComponentController *mControllerInstance;

      /* Mutex to protect instance from multithread */
      static std::mutex mControllerMutex;

      /* Pointer to step motor driver for window */
      Component::Driver::Motor::StepMotor *mWindowMotor;

      /* Pointer to water pump */
      Component::Driver::Active::WaterPump *mWaterPump;
    };
  } // namespace Manager
} // namespace Greenhouse

#endif