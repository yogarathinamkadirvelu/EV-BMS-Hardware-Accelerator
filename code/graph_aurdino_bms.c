#include <stdio.h>
#include <stdlib.h> 
#include "xil_printf.h"
#include "xaxidma.h"
#include "xparameters.h"
#include "xil_cache.h"
#include "sleep.h" 

#define DMA_BASE_ADDR XPAR_XAXIDMA_0_BASEADDR
#define DATA_SIZE 64    
#define FILTER_GAIN 148 

XAxiDma AxiDma;
u32 TxBuffer[DATA_SIZE]; 
u32 RxBuffer[DATA_SIZE]; 
float actual_cell_voltage = 4100.0; 

int init_dma() {
    XAxiDma_Config *CfgPtr = XAxiDma_LookupConfig(DMA_BASE_ADDR);
    if (!CfgPtr) return XST_FAILURE;
    if (XAxiDma_CfgInitialize(&AxiDma, CfgPtr) != XST_SUCCESS) return XST_FAILURE;
    XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
    XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
    return XST_SUCCESS;
}

int main() {
    Xil_ICacheEnable();
    Xil_DCacheEnable();

    if (init_dma() != XST_SUCCESS) return XST_FAILURE;

    int is_first_run = 1; 

    while (1) {
        actual_cell_voltage -= 0.05; 
        int is_accelerating = (rand() % 100 > 90); 
        int voltage_sag = is_accelerating ? 250 : 0; 
        
        int base_voltage = (int)actual_cell_voltage - voltage_sag;
        for (int i = 0; i < DATA_SIZE; i++) {
            int inverter_noise = (rand() % 300) - 150; 
            TxBuffer[i] = base_voltage + inverter_noise; 
            RxBuffer[i] = 0; 
        }

        Xil_DCacheFlushRange((UINTPTR)TxBuffer, DATA_SIZE * sizeof(u32));
        Xil_DCacheFlushRange((UINTPTR)RxBuffer, DATA_SIZE * sizeof(u32));
        XAxiDma_SimpleTransfer(&AxiDma, (UINTPTR)RxBuffer, DATA_SIZE * sizeof(u32), XAXIDMA_DEVICE_TO_DMA);
        XAxiDma_SimpleTransfer(&AxiDma, (UINTPTR)TxBuffer, DATA_SIZE * sizeof(u32), XAXIDMA_DMA_TO_DEVICE);
        while (XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE)) {}
        while (XAxiDma_Busy(&AxiDma, XAXIDMA_DEVICE_TO_DMA)) {}
        Xil_DCacheInvalidateRange((UINTPTR)RxBuffer, DATA_SIZE * sizeof(u32));
        for (int i = 0; i < DATA_SIZE; i++) {
            if (is_first_run && i < 21) continue; 
            
            u32 raw_rx = RxBuffer[i];
            int32_t signed_rx = (int32_t)raw_rx; 
            if (signed_rx & 0x00800000) signed_rx |= 0xFF000000;  
            int32_t clean_val = signed_rx / FILTER_GAIN;
            int raw_val = TxBuffer[i];
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
            xil_printf("Raw_Noise:%d,Clean_Telemetry:%d \t /* Battery: %d%% | Status: %s */ \r\n", 
                       raw_val, clean_val, soc_pct, bms_status);
            usleep(50000); 
        }
        is_first_run = 0; 
   }
   return 0;
}
