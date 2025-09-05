/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/flash.h"
#include "hardware/flash.h"
#include <string.h>
// We're going to erase and reprogram a region 256k from the start of flash.
// Once done, we can access this at XIP_BASE + 256k.
#define FLASH_TARGET_OFFSET (25 * 1024)
#define FLASH_DATA32_OFFSET  (26 * 1024)
#define FLASH_FLOAT_OFFSET  (27 * 1024)
#define FLASH_STRUCT_OFFSET  (28 * 1024)

const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
const uint8_t *flash_data32_contents = (const uint8_t *) (XIP_BASE + FLASH_DATA32_OFFSET);
const uint8_t *flash_float_contents = (const uint8_t *) (XIP_BASE + FLASH_FLOAT_OFFSET);
const uint8_t *flash_struct_contents = (const uint8_t *) (XIP_BASE + FLASH_STRUCT_OFFSET);

void print_buf(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x", buf[i]);
        if (i % 16 == 15)
            printf("\n");
        else
            printf(" ");
    }
}

// This function will be called when it's safe to call flash_range_erase
static void call_flash_range_erase(void *param) {
    uint32_t offset = (uint32_t)param;
    flash_range_erase(offset, FLASH_SECTOR_SIZE);
}

// This function will be called when it's safe to call flash_range_program
static void call_flash_range_program(void *param) {
    uint32_t offset = ((uintptr_t*)param)[0];
    const uint8_t *data = (const uint8_t *)((uintptr_t*)param)[1];
    flash_range_program(offset, data, FLASH_PAGE_SIZE);
}

int main() {
        uint flash_size_bytes;
#ifndef PICO_FLASH_SIZE_BYTES
#warning PICO_FLASH_SIZE_BYTES not set, assuming 16M
    flash_size_bytes = 16 * 1024 * 1024;
#else
    flash_size_bytes = PICO_FLASH_SIZE_BYTES;
#endif
    stdio_init_all();
    uint8_t test_data[FLASH_PAGE_SIZE];
    for (uint i = 0; i < FLASH_PAGE_SIZE; ++i)
        test_data[i] = i;

    printf("Generated  data:\n");
    print_buf(test_data, FLASH_PAGE_SIZE);

    // Note that a whole number of sectors must be erased at a time.
    printf("\nErasing target region...\n");

    // Flash is "execute in place" and so will be in use when any code that is stored in flash runs, e.g. an interrupt handler
    // or code running on a different core.
    // Calling flash_range_erase or flash_range_program at the same time as flash is running code would cause a crash.
    // flash_safe_execute disables interrupts and tries to cooperate with the other core to ensure flash is not in use
    // See the documentation for flash_safe_execute and its assumptions and limitations
    int rc = flash_safe_execute(call_flash_range_erase, (void*)FLASH_TARGET_OFFSET, UINT32_MAX);//擦除
    hard_assert(rc == PICO_OK);

    printf("Done. Read back target region:\n");
    print_buf(flash_target_contents, FLASH_PAGE_SIZE);

    printf("\nProgramming target region...\n");
    uintptr_t params[] = { FLASH_TARGET_OFFSET, (uintptr_t)test_data};
    rc = flash_safe_execute(call_flash_range_program, params, UINT32_MAX);//编程
    hard_assert(rc == PICO_OK);
    printf("Done. Read back target region:\n");
    print_buf(flash_target_contents, FLASH_PAGE_SIZE);

    bool mismatch = false;
    for (uint i = 0; i < FLASH_PAGE_SIZE; ++i) {
        if (test_data[i] != flash_target_contents[i])
            mismatch = true;
    }
    if (mismatch)
        printf("Programming failed!\n");
    else
        printf("Programming successful!\n");

    // 写入 uint32_t 数据
    uint32_t values[] = {0x12345678, 0xABCDEF00, 0xDEADBEEF};

    // 确保数据对齐 (4字节对齐)
    static_assert(sizeof(values) % 4 == 0, "uint32_t array must be 4-byte aligned");

    // 拷贝数据到flash缓冲区
    memcpy(test_data, values, sizeof(values));
    // 剩余空间可以填充其他数据或0
    for (uint i = sizeof(values); i < FLASH_PAGE_SIZE; ++i) {
        test_data[i] = 0x00;
    }
 rc = flash_safe_execute(call_flash_range_erase, (void*)FLASH_DATA32_OFFSET, UINT32_MAX);//擦除
    hard_assert(rc == PICO_OK);

    printf("\nProgramming target region...\n");
    uintptr_t params2[] = { FLASH_DATA32_OFFSET, (uintptr_t)test_data};
    rc = flash_safe_execute(call_flash_range_program, params2, UINT32_MAX);//编程
    hard_assert(rc == PICO_OK);

        // 读取uint32_t数据
uint32_t *uint32_data = (uint32_t *)(XIP_BASE + FLASH_DATA32_OFFSET);
printf("First uint32: 0x%08x\n", uint32_data[0]);
printf("Second uint32: 0x%08x\n", uint32_data[1]);
printf("Third uint32: 0x%08x\n", uint32_data[2]);

    // 写入 float 数据
    float sensor_readings[] = {3.14159f, 2.71828f, 1.41421f, 0.0f};

    // 确保数据对齐 (4字节对齐)
    static_assert(sizeof(sensor_readings) % 4 == 0, "float array must be 4-byte aligned");

    // 拷贝数据到flash缓冲区
    memcpy(test_data, sensor_readings, sizeof(sensor_readings));

    // 剩余空间填充
    for (uint i = sizeof(sensor_readings); i < FLASH_PAGE_SIZE; ++i) {
        test_data[i] = 0xFF;  // 填充0xFF
    }
 rc = flash_safe_execute(call_flash_range_erase, (void*)FLASH_FLOAT_OFFSET, UINT32_MAX);//擦除
    hard_assert(rc == PICO_OK);

    printf("\nProgramming target region...\n");
    uintptr_t params0[] = { FLASH_FLOAT_OFFSET, (uintptr_t)test_data};
    rc = flash_safe_execute(call_flash_range_program, params0, UINT32_MAX);//编程
    hard_assert(rc == PICO_OK);

// 读取float数据
float *float_data = (float *)(XIP_BASE + FLASH_FLOAT_OFFSET);
printf("First float: %f\n", float_data[0]);
printf("Second float: %f\n", float_data[1]);
printf("Third float: %f\n", float_data[2]);

    // 定义要存储的数据结构
typedef struct __attribute__((packed)) {
    uint32_t magic;        // 4字节
    float temperature;     // 4字节（单精度浮点数）
    float humidity;        // 4字节（单精度浮点数）
    uint32_t timestamp;    // 4字节
    uint8_t status;        // 1字节
    char device_id[16];    // 16字节
    uint32_t checksum;     // 4字节
} sensor_data_t;


    // 创建并初始化结构体数据
    sensor_data_t sensor_data = {
        .magic = 0x53544F52,  // 'STOR' in hex
        .temperature = 25.6f,   // 单精度浮点数
        .humidity = 65.2f,   // 单精度浮点数，存在精度丢失问题，%f结果：65.199997
        .timestamp = 1704067200,  // 2024-01-01 00:00:00
        .status = 0x01,
        .device_id = "PICO_SENSOR_01",
        .checksum = 0  // 先设为0，后面计算
    };

    // 计算校验和（简单示例）
    sensor_data.checksum = sensor_data.magic +
                          (uint32_t)(sensor_data.temperature * 100) +
                          (uint32_t)(sensor_data.humidity * 100) +
                          sensor_data.timestamp +
                          sensor_data.status;

    // 拷贝结构体数据到flash缓冲区
    memcpy(test_data, &sensor_data, sizeof(sensor_data));

    // 剩余空间可以存储其他数据或填充
    // for (uint i = sizeof(sensor_data); i < FLASH_PAGE_SIZE; ++i) {
    //     test_data[i] = 0xAA;  // 填充模式
    // }
 rc = flash_safe_execute(call_flash_range_erase, (void*)FLASH_STRUCT_OFFSET, UINT32_MAX);//擦除
    hard_assert(rc == PICO_OK);

    printf("\nProgramming target region...\n");
    uintptr_t params3[] = { FLASH_STRUCT_OFFSET, (uintptr_t)test_data};
    rc = flash_safe_execute(call_flash_range_program, params3, UINT32_MAX);//编程
    hard_assert(rc == PICO_OK);




// 读取结构体数据
// 读取结构体数据
    sensor_data_t *struct_data = (sensor_data_t *)(XIP_BASE + FLASH_STRUCT_OFFSET);
    printf("\nReading struct data from flash:\n");
    printf("Magic: 0x%08X\n", struct_data->magic);
    printf("Temperature: %.1f\n", struct_data->temperature);
    printf("Humidity: %.1f\n", struct_data->humidity);
    printf("Timestamp: %u\n", struct_data->timestamp);
    printf("Status: 0x%02X\n", struct_data->status);
    printf("Device ID: %s\n", struct_data->device_id);
    printf("Checksum: 0x%08X\n", struct_data->checksum);

    // 验证结构体数据
    uint32_t calculated_checksum = struct_data->magic +
                                  (uint32_t)(struct_data->temperature * 100) +
                                  (uint32_t)(struct_data->humidity * 100) +
                                  struct_data->timestamp +
                                  struct_data->status;

    if (calculated_checksum == struct_data->checksum) {
        printf("Checksum verification: PASSED\n");
    } else {
        printf("Checksum verification: FAILED (expected: %u, got: %u)\n",
               calculated_checksum, struct_data->checksum);
    }

}
