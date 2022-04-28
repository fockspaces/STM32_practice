#include <stdint.h>

#define USE_SPI_1 0
#define USE_SPI_2 1

/* Define SPI_CS Pin config */
#ifdef STM32WB55xx
    #include "stm32wbxx_hal.h"
    #define SPI_CS_Pin GPIO_PIN_4
    #define SPI_CS_GPIO_Port GPIOA
#endif

#ifdef STM32L496xx
    #include "stm32l4xx_hal.h"
    #if (USE_SPI_1 == 1)
        #define SPI_CS_Pin GPIO_PIN_15
        #define SPI_CS_GPIO_Port GPIOA
    #endif
    #if (USE_SPI_2 == 1)
        #define SPI_CS_Pin GPIO_PIN_11
        #define SPI_CS_GPIO_Port GPIOG
    #endif
#endif

#if (USE_SPI_1 == 1)
extern SPI_HandleTypeDef hspi1;
#endif
#if (USE_SPI_2 == 1)
extern SPI_HandleTypeDef hspi2;
#endif

void FRAM_DMA_Init();
void FRAM_DMA_Read(uint32_t addr, uint8_t *dst, unsigned long len);
void FRAM_DMA_Write(uint32_t addr, uint8_t *src, unsigned long len);
