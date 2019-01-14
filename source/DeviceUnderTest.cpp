#include "ComplianceInterface.h"

#ifdef DEVICE_UNDER_TEST

int test_one()
{
    set_tx_rx_gpio(1);
}

int device_main()
{
    test_one();
}

#endif