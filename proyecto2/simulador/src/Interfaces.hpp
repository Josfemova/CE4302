#ifndef CE4302_INTERFACES_H
#define CE4302_INTERFACES_H

#include "types.hpp"
#include "Clocked.hpp"


class BusMaster;
class MemorySlave;

enum class BusMessageType{
    /// @brief Read común, si otro cache tiene el bloque se espera que lo provea
    BusRd,
    /// @brief Read exclusive. Indica que el core local no tiene el bloque pero
    /// lo quiere escribir. El valor lo puede dar otro caché
    BusRdX,
    /// @brief Similar a RdX, pero en este caso no hay necesidad de proveer el 
    /// valor, solo se invalidan las copias en otros cachés. En este caso el 
    /// cache que realiza el request ya tiene una copia (transición S->M)
    BusUpgr,
    /// @brief Cuando hay un reemplazo de bloque de caché. Indica que se escribe
    /// el bloque reemplazado a memoria principal 
    Flush,
    /// FlusOpt no se necesita porque se administra con el "handle bus request"
};

typedef struct{
    BusMessageType type;
    int64_t address;
    int64_t (& data)[4];
    int master_id;
    /// @brief solo cambia en los casos de transición M->I y M->S. Implica que 
    /// los datos de "data" deben ser escritos a memoria principal 
    //! Deprecado!, es más fácil simplemente no completar el request y cambiar 
    //! y que el caché solo cambie el tipo de mensaje a "flush" 
    // bool must_flush_to_main_memory;
    
    /// @brief indica si un bloque es exclusivo al terminar una operación
    bool exclusive;
    bool completed;
} BusMessage_t;

typedef struct {
    MemorySlave* ptr;
    int64_t start_addr;
    int64_t end_addr; 
} MemorySlaveCard;

typedef struct {
    BusMaster* ptr;
    int read_requests;
    int write_requests; 
} BusMasterCard;


class MemorySlave{
public: 
    virtual int64_t read_request(int64_t addr)=0; 
    virtual void write_request(int64_t addr, int64_t value)=0; 
};

// similar a memory slave pero las operaciones son algo distintas
class Bus{
public:
    virtual void bus_request(BusMessage_t& request)=0; 
};

class BusMaster{
public:
    virtual int get_id()=0;
    virtual void handle_bus_message(BusMessage_t& msg)=0;
};

#endif 

