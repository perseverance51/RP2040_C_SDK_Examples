/*
cd /RP2040_FLASH_RW/build
//# 编译时指定Flash大小，生成16MB pico_flash_region.ld文件（默认2MB）
cmake .. -G "Ninja" -DPICO_FLASH_SIZE_BYTES=16M
ninja

  CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_FLASH_RW.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_FLASH_RW.elf verify reset exit"


 */

#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/divider.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"

#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/flash.h"

#define LED_PIN 25

// 常见的Flash容量对应的JEDEC ID映射表
typedef struct {
    uint8_t manufacturer_id;
    uint8_t memory_type;
    uint8_t capacity_id;
    uint32_t size_bytes;
    const char *description;
} flash_id_map_t;

// 制造商映射表
const char *manufacturer_map[] = {
    [0xEF] = "Winbond",
    [0xC2] = "Macronix",
    [0x20] = "Micron"
};

// 容量映射表（1 << (id - 0x10) 计算结果单位MB）
const float capacity_map[] = {
    [0x17] = 16.0,  // 128Mbit
    [0x18] = 32.0,  // 256Mbit
    [0x19] = 64.0   // 512Mbit
};

void Flash_JEDEC_ID() {
// 读取JEDEC ID示例
    uint8_t tx[4] = {0x9F, 0x00, 0x00, 0x00}; // 9Fh指令
    uint8_t rx[4];
// 进入临界区，禁用中断以确保通信的原子性
    uint32_t interrupts = save_and_disable_interrupts();
    flash_do_cmd(tx, rx, 4);
// 退出临界区，恢复中断
    restore_interrupts(interrupts);

// printf("制造商ID: %02x\r\n", rx[1]);
// printf("内存类型: %02x\r\n", rx[2]);
// printf("容量ID: %02x\r\n", rx[3]);
    printf("制造商: %s (0x%02x)\r\n",
           manufacturer_map[rx[1]] ? manufacturer_map[rx[1]] : "Unknown",
           rx[1]);

    printf("存储容量: %.1f MB\r\n",
           capacity_map[rx[3]] ? capacity_map[rx[3]] : 0.0);

// 保留原始打印
    printf("原始ID检测 - 类型:0x%02x 容量:0x%02x\r\n", rx[2], rx[3]);

// 读取SFDP参数示例
    uint8_t sfdp_tx[8] = {0x5A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t sfdp_rx[8];
    flash_do_cmd(sfdp_tx, sfdp_rx, 8);

    printf("SFDP参数: ");
    for (int i = 0; i < 8; i++) {
        printf("%02x ", sfdp_rx[i]);
    }
    printf("\r\n");
}

// 状态寄存器读取指令
#define READ_STATUS_REG_1  0x05
#define READ_STATUS_REG_2  0x35
#define READ_STATUS_REG_3  0x15

uint8_t read_flash_status(uint8_t reg_num) {
    uint8_t tx[4], rx[4];

    switch (reg_num) {
    case 1:
        tx[0] = READ_STATUS_REG_1;
        break;
    case 2:
        tx[0] = READ_STATUS_REG_2;
        break;
    case 3:
        tx[0] = READ_STATUS_REG_3;
        break;
    default:
        return 0xFF;
    }

    flash_do_cmd(tx, rx, 2);  // 发送1字节指令，接收2字节响应
    return rx[1];  // 第二个字节为状态值
}

// 新增写状态寄存器函数
void write_flash_status(uint8_t reg_num, uint8_t value) {
    uint8_t tx[4];

    // 写使能指令
    uint8_t write_enable[] = {0x06};
    flash_do_cmd(write_enable, NULL, 1);

    switch (reg_num) {
    case 1:
        tx[0] = 0x01;  // WRITE_STATUS_REG_1
        break;
    case 2:
        tx[0] = 0x31;  // WRITE_STATUS_REG_2
        break;
    case 3:
        tx[0] = 0x11;  // WRITE_STATUS_REG_3
        break;
    default:
        return;
    }
    tx[1] = value;

    // 发送写指令
    flash_do_cmd(tx, NULL, 2);

    // 等待写入完成
    while (read_flash_status(1) & 0x01);
}



/**
 * 打印当前函数的编译信息
 *
 * 该函数打印当前函数的名称、编译日期、编译时间、文件名、行号、ANSI C标准和编译器版本。
 *
 * @return 无返回值
 */
void INFO_Func() {
    printf("RP2040 INFO Test\n");
    // 打印当前函数的名称
    printf("当前函数: %s\n", __func__);
    //   printf("函数名: %s\n", __FUNCTION__);
    // 打印编译日期
    printf("编译日期: %s\n", __DATE__);
    // 打印编译时间
    printf("编译时间: %s\n", __TIME__);
    // 打印文件名
    printf("文件名: %s\n", __FILE__);
    // 打印行号
    printf("行号: %d\n", __LINE__);

    // 打印ANSI C标准
    printf("ANSI C标准: %d\n", __STDC__);
    // 打印编译器版本
    printf("编译器版本: %s\n", __VERSION__);
    printf("开始获取Flash信息...\n");

    // 调用SDK函数获取Flash大小（字节为单位）
    // uint32_t flash_size_bytes = get_flash_size();
    uint8_t unique_id[8];
    flash_get_unique_id(unique_id); // 获取唯一ID
    printf("唯一ID: ");
    for (int i = 0; i < 8; i++) {
        printf("%02x", unique_id[i]);
    }
    printf("\r\n");

    // 转换为更易读的单位
    // printf("片外Flash总大小: %lu 字节\n", flash_size_bytes);
    // printf("片外Flash总大小: %lu KB\n", flash_size_bytes / 1024);
    // printf("片外Flash总大小: %lu MB\n", flash_size_bytes / (1024 * 1024));
    // 获取Flash设备信息


}

void measure_freqs(void) {
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

int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    // 使用示例：
    uint8_t status1 = read_flash_status(1);
    uint8_t status2 = read_flash_status(2);
    uint8_t status3 = read_flash_status(3);


    while (1)
    {
        // tight_loop_contents();
        // 翻转LED状态
        gpio_put(LED_PIN, !gpio_get(LED_PIN));
        INFO_Func();
        sleep_ms(1000);
        measure_freqs();
        // read_flash_size();
        Flash_JEDEC_ID();
        printf("状态寄存器1: 0x%02x\r\n", status1);
        printf("状态寄存器2: 0x%02x\r\n", status2);
        printf("状态寄存器3: 0x%02x\r\n", status3);
        sleep_ms(1000);
        tight_loop_contents();
        if (status3 == 0x60)
        {
            write_flash_status(3, 0x20);
            status3 = read_flash_status(3);
            printf("新状态寄存器3: 0x%02x\r\n", status3);
        }


    }


//    puts("Hello, world!");

    return 0;
}
