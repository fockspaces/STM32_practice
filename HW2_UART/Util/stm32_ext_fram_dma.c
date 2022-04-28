#include "stm32_ext_fram_dma.h"

uint8_t completed_tx;
uint8_t completed_rx;

void FRAM_DMA_Init(){
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
    uint8_t buf[2];
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
    buf[0] = 0x06; //CMD_WREN
    #if (USE_SPI_1 == 1)
    HAL_SPI_Transmit(&hspi1, buf, 1, HAL_MAX_DELAY);
    #endif
    #if (USE_SPI_2 == 1)
    HAL_SPI_Transmit(&hspi2, buf, 1, HAL_MAX_DELAY);
    #endif

    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
    buf[0] = 0x01; //CMD_WRSR
    buf[1] = 0xc0;
    #if (USE_SPI_1 == 1)
    HAL_SPI_Transmit(&hspi1, buf, 2, HAL_MAX_DELAY);
    #endif
    #if (USE_SPI_2 == 1)
    HAL_SPI_Transmit(&hspi2, buf, 2, HAL_MAX_DELAY);
    #endif

    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);

}

void FRAM_DMA_Read(uint32_t addr, uint8_t *dst, unsigned long len){
    uint8_t buf[4];
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
    buf[0] = 0x03; //CMD_READ;
    buf[1] = (addr & 0xFF0000U) >> 16;
    buf[2] = (addr & 0x00FF00U) >> 8;
    buf[3] = (addr & 0x0000FFU);
    #if (USE_SPI_1 == 1)
    HAL_SPI_Transmit(&hspi1, buf, 4, HAL_MAX_DELAY);
    #endif
    #if (USE_SPI_2 == 1)
    HAL_SPI_Transmit(&hspi2, buf, 4, HAL_MAX_DELAY);
    #endif
    completed_rx = 0;
    #if (USE_SPI_1 == 1)
    HAL_SPI_Receive_DMA(&hspi1, dst, len);
    #endif
    #if (USE_SPI_2 == 1)
    HAL_SPI_Receive_DMA(&hspi2, dst, len);
    #endif
    while(completed_rx == 0){;}
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
}

void FRAM_DMA_Write(uint32_t addr, uint8_t *src, unsigned long len){
    uint8_t buf[4];
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
    buf[0] = 0x06; //CMD_WREN
    #if (USE_SPI_1 == 1)
    HAL_SPI_Transmit(&hspi1, buf, 1, HAL_MAX_DELAY);
    #endif
    #if (USE_SPI_2 == 1)
    HAL_SPI_Transmit(&hspi2, buf, 1, HAL_MAX_DELAY);
    #endif

    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

    buf[0] = 0x02; //CMD_WRITE;
    buf[1] = (addr & 0xFF0000U) >> 16;
    buf[2] = (addr & 0x00FF00U) >> 8;
    buf[3] = (addr & 0x0000FFU);
    #if (USE_SPI_1 == 1)
    HAL_SPI_Transmit(&hspi1, buf, 4, HAL_MAX_DELAY);
    #endif
    #if (USE_SPI_2 == 1)
    HAL_SPI_Transmit(&hspi2, buf, 4, HAL_MAX_DELAY);
    #endif

    completed_tx = 0;
    #if (USE_SPI_1 == 1)
    HAL_SPI_Transmit_DMA(&hspi1, src, len);
    #endif
    #if (USE_SPI_2 == 1)
    HAL_SPI_Transmit_DMA(&hspi2, src, len);
    #endif

    while(completed_tx == 0){;}
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi)
{
    completed_tx = 1;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef * hspi)
{
    completed_rx = 1;
}
