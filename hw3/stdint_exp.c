#include <stdint.h>
#include <stdio.h>

int main() {
    printf(
        "%lu %lu %lu %lu %lu %lu %lu %lu"
        , sizeof(int8_t),  sizeof(int16_t),  sizeof(int32_t),  sizeof(int64_t)
        , sizeof(uint8_t), sizeof(uint16_t), sizeof(uint32_t), sizeof(uint64_t)
    );
}