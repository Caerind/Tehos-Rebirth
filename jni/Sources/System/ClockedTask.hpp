#ifndef KE_CLOCKEDTASK_HPP
#define KE_CLOCKEDTASK_HPP

#include <functional>
#include <SFML/System/Clock.hpp>

namespace ke
{

class ClockedTask
{
    public:
        ClockedTask(std::function<void()> function = [](){});

        void setTask(std::function<void()> function);

        sf::Time execute();

    protected:
        std::function<void()> mFunction;
};

} // namespace ke

#endif // KE_CLOCKEDTASK_HPP
