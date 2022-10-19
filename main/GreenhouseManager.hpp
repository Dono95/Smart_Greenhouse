/**
 * Definition of Greenhouse manager
 * 
 * @author Dominik Regec
 */
#ifndef GREENHOUSE_MANAGER
#define GREENHOUSE_MANAGER

namespace Greenhouse
{
    class GreenhouseManager
    {
    public:
        /**
         * @brief Static method to get instance of GreenhouseManager
         */
        static GreenhouseManager* GetInstance();

    private:
        /**
         * @brief Class constructor
         */
        explicit GreenhouseManager();

        /**
         * @brief Class destructor
         */ 
        ~GreenhouseManager();

        /* Singleton instance of GreenhouseManager object */
        static GreenhouseManager* mManagerInstance;
    };
} // namespace Greenhouse

#endif // GREENHOUSE_MANAGER