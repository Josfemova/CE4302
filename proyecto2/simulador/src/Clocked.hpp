#ifndef CE4302_CLOCK_H
#define CE4302_CLOCK_H
#include <atomic>

class Clocked{
private:
    std::atomic_bool ticked;
protected:
    /**
     * @brief Esta es la función que llama cualquier componente clocked para
     * saber si puede dar un paso de tiempo 
     */
    void step();
public:
    Clocked();
    /**
     * @brief Utilizado por un controlador de tiempo para decirle al clock que 
     * puede dar un paso
     */
    void tick();
};

#endif