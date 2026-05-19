#include "xil_printf.h"
#include <stdlib.h>

#define FILTER_GAIN 148 

void execute_bms_application_layer(u32* TxBuffer, u32* RxBuffer, float* actual_cell_voltage) {
    *actual_cell_voltage -= 0.05; 
    int is_accelerating = (rand() % 100 > 90); 
    int voltage_sag = is_accelerating ? 250 : 0; 
    int base_voltage = (int)(*actual_cell_voltage) - voltage_sag;

    for (int i = 0; i < 64; i++) {
        int inverter_noise = (rand() % 300) - 150; 
        TxBuffer[i] = base_voltage + inverter_noise; 
    }

    for (int i = 21; i < 64; i++) { 
        
        int32_t signed_rx = (int32_t)RxBuffer[i]; 
        if (signed_rx & 0x00800000) signed_rx |= 0xFF000000;  
        int32_t clean_val = signed_rx / FILTER_GAIN;

        int soc_pct = ((clean_val - 3000) * 100) / 1200;
        if (soc_pct > 100) soc_pct = 100;
        if (soc_pct < 0) soc_pct = 0;

        char* bms_status = "SYSTEM OK";
        if (clean_val >= 4250) {
            bms_status = "CRITICAL: OVER-VOLTAGE TRIP!";
        } else if (clean_val <= 2900) {
            bms_status = "CRITICAL: UNDER-VOLTAGE TRIP!";
        } else if (is_accelerating) {
            bms_status = "LOAD: HEAVY ACCELERATION";
        }
    }
}
