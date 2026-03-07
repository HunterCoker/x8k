#include "drivers/communication/serial/spi.h"

#include <M480.h>
#include <spi.h>
#include <gpio.h>

#define FREQ_10MHZ  10000000UL

namespace communication::serial
{

void Spi::initialize()
{
    SYS_UnlockReg();

    CLK_EnableModuleClock(SPI2_MODULE);

    CLK_SetModuleClock(SPI2_MODULE, CLK_CLKSEL2_SPI2SEL_PCLK1, MODULE_NoMsk);

    /* Setup PA.8 through PA.10 as multi-function pins for SPI MOSI, MISO, CLK */
    SYS->GPA_MFPH &= ~(SYS_GPA_MFPH_PA8MFP_Msk  |
                       SYS_GPA_MFPH_PA9MFP_Msk  |
                       SYS_GPA_MFPH_PA10MFP_Msk |
                       SYS_GPA_MFPH_PA11MFP_Msk);
    SYS->GPA_MFPH |=  (SYS_GPA_MFPH_PA8MFP_SPI2_MOSI |
                       SYS_GPA_MFPH_PA9MFP_SPI2_MISO |
                       SYS_GPA_MFPH_PA10MFP_SPI2_CLK);

    // Can run at up to 96MHz? Maybe change value if sensor can handle it...
    SPI_Open(SPI2, SPI_MASTER, SPI_MODE_3, 8, FREQ_10MHZ);
    SPI_DisableAutoSS(SPI2);

    SYS_LockReg();
}

uint8_t Spi::transmitByte(const uint8_t tx) const
{
    SPI_ClearRxFIFO(SPI2);
    
    SPI_WRITE_TX(SPI2, tx);
    while (SPI_IS_BUSY(SPI2));
    while (SPI_GET_RX_FIFO_COUNT(SPI2) == 0);
    return static_cast<uint8_t>(SPI_READ_RX(SPI2));
}

void Spi::transfer(const uint8_t* tx, uint8_t* rx, size_t n) const
{
    if (n == 0)
        return;

    SPI_ClearRxFIFO(SPI2);

    if (tx == nullptr)  // tx == nullptr -> send dummy bytes (0x00)
    {
        if (rx == nullptr)  // rx == nullptr -> don't save response bytes
        {
            for (size_t i = 0; i < n; ++i)
            {
                SPI_WRITE_TX(SPI2, 0x00);
                while (SPI_IS_BUSY(SPI2));
                while (SPI_GET_RX_FIFO_COUNT(SPI2) == 0);
                UNUSED(SPI_READ_RX(SPI2));
            }
        }
        else
        {
            for (size_t i = 0; i < n; ++i)
            {
                SPI_WRITE_TX(SPI2, 0x00);
                while (SPI_IS_BUSY(SPI2));
                while (SPI_GET_RX_FIFO_COUNT(SPI2) == 0);
                rx[i] = static_cast<uint8_t>(SPI_READ_RX(SPI2));
            }
        }
    }
    else
    {
        if (rx == nullptr)
        {
            for (size_t i = 0; i < n; ++i)
            {
                SPI_WRITE_TX(SPI2, tx[i]);
                while (SPI_IS_BUSY(SPI2));
                while (SPI_GET_RX_FIFO_COUNT(SPI2) == 0);
                UNUSED(SPI_READ_RX(SPI2));
            }
        }
        else
        {

            for (size_t i = 0; i < n; ++i)
            {
                SPI_WRITE_TX(SPI2, tx[i]);
                while (SPI_IS_BUSY(SPI2));
                while (SPI_GET_RX_FIFO_COUNT(SPI2) == 0);
                rx[i] = static_cast<uint8_t>(SPI_READ_RX(SPI2));
            }
        }
    }
}

}  // namespace communication::serial