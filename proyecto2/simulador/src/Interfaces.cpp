#include "Interfaces.hpp"

std::map<BusMessageType, std::string> bus_message_type_map = {
    {BusMessageType::BusRd, "BusRd"},
    {BusMessageType::BusRdX, "BusRdX"},
    {BusMessageType::BusUpgr, "BusUpgr"},
    {BusMessageType::Flush, "Flush"},

};