#include <unity.h>
#include "./include/utils/test_init.h"

void setUp(void) {}

void tearDown(void) {}

int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_init_serial_communication);
    RUN_TEST(test_init_pin_layout);

    return UNITY_END();
}
