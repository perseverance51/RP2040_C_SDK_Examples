https://github.com/bablokb/pico-st7735
https://github.com/bablokb/pico-st7735-demo

st7735 Pin:
复位（RES）、片选（CS）、数据/命令（D/C）、MOSI(SDA)（主输出/从输入）、SCK（串行时钟），BLK(背光) (可以不用)

RP2040 SPI1:sck=Pin(10),mosi=Pin(11),miso=Pin(12))
SPI0:sck=Pin(14),mosi=Pin(15),miso=Pin(12))

# library configuration (override if necessary, see pico-st7735 for a complete list)
set(SPI_TFT_PORT "spi1" CACHE STRING "TFT spi port-name")
set(SPI_TFT_CS   "9"    CACHE STRING "TFT CS  pin number")  #9引脚
set(SPI_TFT_DC   "10"   CACHE STRING "TFT DC  pin number")  #10引脚
set(SPI_TFT_RST  "11"   CACHE STRING "TFT RST pin number")  #11引脚
set(TFT_OPTIONS TFT_ENABLE_BLACK TFT_ENABLE_RESET TFT_ENABLE_TEXT TFT_ENABLE_SHAPES
                TFT_ENABLE_ROTATE TFT_ENABLE_SCROLL
            CACHE STRING "TFT options/functions")
#program configuration
set(SPI_PORT "spi1" CACHE STRING "spi port-name")
set(SPI_RX   "12"   CACHE STRING "RX  pin number")
set(SPI_TX   "15"   CACHE STRING "TX  pin number")  #SDA引脚 15引脚
set(SPI_SCK  "14"   CACHE STRING "SCK pin number")  #SCL引脚 14引脚

///SPI0
set(SPI_TFT_PORT "spi0" CACHE STRING "TFT spi port-name")
set(SPI_TFT_CS   "9"    CACHE STRING "TFT CS  pin number")  #9引脚
set(SPI_TFT_DC   "10"   CACHE STRING "TFT DC  pin number")  #10引脚
set(SPI_TFT_RST  "11"   CACHE STRING "TFT RST pin number")  #11引脚
set(TFT_OPTIONS TFT_ENABLE_BLACK TFT_ENABLE_RESET TFT_ENABLE_TEXT TFT_ENABLE_SHAPES
                TFT_ENABLE_ROTATE TFT_ENABLE_SCROLL
            CACHE STRING "TFT options/functions")
#program configuration
set(SPI_PORT "spi0" CACHE STRING "spi port-name")
set(SPI_RX   "8"   CACHE STRING "RX  pin number")
set(SPI_TX   "7"   CACHE STRING "TX  pin number")  #SDA引脚 15引脚
set(SPI_SCK  "6"   CACHE STRING "SCK pin number")  #SCL引脚 14引脚
