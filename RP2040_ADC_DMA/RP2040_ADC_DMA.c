
/*
 CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_ADC_DMA.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_ADC.elf verify reset exit"


*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25

// Channel 0 is GPIO26
#define CAPTURE_CHANNEL 0
#define SAMPLES 10
// Ideally the signal should be bandwidth limited to sample_frequency/2
#define SAMPLING_FREQUENCY 14000 // Sampling frequency in Hz

uint16_t sampleBuffer[SAMPLES];
uint16_t streamBuffer[SAMPLES]; // Scaled ADC sample working buffer

dma_channel_config cfg;
int dma_chan;

// const char src[] = "Hello, world! (from DMA)";
// char dst[count_of(src)];

static void measure_freqs(void);


void ADC_Reading(void)
{

    // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
        const float conversion_factor = 3.3f / (1 << 12);
          adc_select_input(0);//设置通道2
        uint16_t adc_0_raw = adc_read();//读取转换通道转换结果
        printf("Raw1 value:%d, voltage: %f V\n", adc_0_raw,adc_0_raw * conversion_factor);

}


int main()
{

    stdio_init_all();

    sleep_ms(2500);
    printf("adc DMA test!\n");
   // set_sys_clock_khz(133000, true); // 325us
                                     // GPIO initialisation.
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);

    adc_init();

    // Init GPIO for analogue use: hi-Z, no pulls, disable digital input buffer.
    adc_gpio_init(26 + CAPTURE_CHANNEL);
    //ADC_Reading();//1983
  //adc_set_round_robin(ADCopen==1 ? 1 : ADCopen==2 ? 3 : ADCopen==3 ? 7 : 15);
    adc_select_input(CAPTURE_CHANNEL);
    adc_fifo_setup(
        true,    // Write each completed conversion to the sample FIFO
        true,    // Enable DMA data request (DREQ)
        1,       // DREQ (and IRQ) asserted when at least 1 sample present
        false,   // We won't see the ERR bit because of 8 bit reads; disable.
        //Shift each sample to 8 bits when pushing to FIFO [true] Changed to false for 12bit values
        false     // Shift each sample to 8 bits when pushing to FIFO
    );

    // Divisor of 0 -> full speed. Free-running capture with the divider is
    // equivalent to pressing the ADC_CS_START_ONCE button once per `div + 1`
    // cycles (div not necessarily an integer). Each conversion takes 96
    // cycles, so
   // in general you want a divider of 0 (hold down the button
    // continuously) or > 95 (take samples less frequently than 96 cycle
    // intervals). This is all timed by the 48 MHz ADC clock.
    //adc_set_clkdiv((48000000/SAMPLING_FREQUENCY) - 1);
    adc_set_clkdiv(0);
  // Set up the DMA to start transferring data as soon as it appears in FIFO设置DMA，一旦数据出现在FIFO中就开始传输数据
  dma_chan = dma_claim_unused_channel(true);
  cfg = dma_channel_get_default_config(dma_chan);//获取给定通道的默认通道配置

  // Reading from constant address, writing to incrementing byte addresses
  channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
  channel_config_set_read_increment(&cfg, false);//因为就一个地址永远地写到同一个位置,目的是循环触发同一个DMA.
  channel_config_set_write_increment(&cfg, true);//FIFO的地址自增
//新增
   channel_config_set_irq_quiet(&cfg, true);//在QUIET模式下，通道不会在每个传输块结束时产生irq。
    channel_config_set_dreq(&cfg, DREQ_ADC); // pace data according to ADC
    channel_config_set_chain_to(&cfg, dma_chan);//外设作为传输源，即ADC->DMA
    channel_config_set_enable(&cfg, true);

//设置DMA通道的配置，包括源地址、目的地址、传输字节数、传输方向、中断等。
//这里设置了源地址为ADC的FIFO，目的地址为streamBuffer，传输字节数为SAMPLES，传输方向为从ADC到streamBuffer，中断为DREQ_ADC。

  // Pace transfers based on availability of ADC samples
 // channel_config_set_dreq(&cfg, DREQ_ADC);

  dma_channel_configure(dma_chan, &cfg,
                        sampleBuffer,   // dst (uint16_t*)数据存储到目标缓冲区
                        &adc_hw->fifo,  // src
                        SAMPLES,        // transfer count 传输数量，即采样点数
                        true            // start immediately
                       );
// Everything is ready to go. Tell the control channel to load the first
    // control block. Everything is automatic from here.
    dma_start_channel_mask(1u << dma_chan);// 开始传输

    printf("Starting capture\n");
    adc_run(true);// Start capture

    while (true)
    {
        // Read the temperature from the onboard temperature sensor.
      //  float temperature = read_onboard_temperature(TEMPERATURE_UNITS);
     //   printf("Onboard temperature = %.02f %c\n", temperature, TEMPERATURE_UNITS);
        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        // tight_loop_contents();
        measure_freqs();
        //ADC_Reading();
        // Wait for DMA to finish (may have already)
  dma_channel_wait_for_finish_blocking(dma_chan);

  // Stop and clean out FIFO
  adc_run(false);
  adc_fifo_drain();
  // Copy samples into buffer for approxFFT SAMPLES
  for (int i = 0; i < SAMPLES; i++) {
   // streamBuffer[i] = sampleBuffer[i]/4 - 512 + 49; // The +49 compensated for a slight DC offset from VCC/2
   // streamBuffer[i] = sampleBuffer[i];
   // streamBuffer[i] = (uint16_t)((sampleBuffer[i]&0x0f)<<8)+(uint16_t)sampleBuffer[i+1] ;

    printf("%-3d ",sampleBuffer[i]);
     if (i % 10 == 9)
            printf("\n");
   // sleep_ms(100);

  }
  // SUM_VALUE/=SAMPLES;
  // printf("SUM_VALUE = %d\n",SUM_VALUE);
  // SUM_VALUE = 0;
  sleep_ms(1000);
 // dma_channel_set_trans_count(DREQ_ADC, 10, true);//设置DMA传输的字节数，这里是10个字节，即10个采样点。
  //Now we have a copy of the samples we can start capture again
  dma_channel_configure(dma_chan, &cfg,
                        (uint16_t*)sampleBuffer,   // dst
                        &adc_hw->fifo,  // src
                        SAMPLES,        // transfer count
                        true            // start immediately
                       );

  // Restart the ADC capture
  adc_run(true);

    }

    return 0;
}

static void measure_freqs(void) {
    uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
    uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
    uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
    uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);

    printf("pll_sys  = %dkHz\n", f_pll_sys);
    printf("pll_usb  = %dkHz\n", f_pll_usb);
    printf("rosc     = %dkHz\n", f_rosc);
    printf("clk_sys  = %dkHz\n", f_clk_sys);
    printf("clk_peri = %dkHz\n", f_clk_peri);
    printf("clk_usb  = %dkHz\n", f_clk_usb);
    printf("clk_adc  = %dkHz\n", f_clk_adc);
    printf("clk_rtc  = %dkHz\n", f_clk_rtc);

    // Can't measure clk_ref / xosc as it is the ref
}
