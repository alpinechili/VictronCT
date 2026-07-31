#include <stdint.h>
#include <stddef.h>

enum class DataType
{
    UINT16,
    INT16
};

struct RegisterDefinition
{
    uint16_t address;
    float* destination;
    float scale;
    DataType type;
};

extern RegisterDefinition systemRegisters[];
extern const size_t systemRegisterCount;