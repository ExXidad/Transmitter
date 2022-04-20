//
// Created by Ivan Kalesnikau on 11.04.2022.
//

#ifndef TRANSMITTER_MANCHESTERENCODE_H
#define TRANSMITTER_MANCHESTERENCODE_H

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <cstring>

#define MANCH_BYTES_NUM 82
#define MANCH_SYNC_BYTES_NUM 2
#define MANCH_DATA_BYTES_NUM MANCH_BYTES_NUM - MANCH_SYNC_BYTES_NUM
#define MANCH_SYNC_FIELD 0xAA55

#define MANCH_ENCODE_TIM_CNT_PER_BIT 5

class ManchesterEncode
{
private:
    typedef enum DecodeState
    {
        NOT_SYNC,
        BIT_SYNC,
        DATA_SYNC,
        DATA_READY
    };

    typedef enum DecodeEdge
    {
        NONE,
        RAISING_EDGE,
        FALLING_EDGE
    };

private:
    uint16_t signalPin;
    GPIO_TypeDef *signalPort;
    uint8_t data[MANCH_BYTES_NUM];
    uint16_t bitStream;
    uint16_t byteIdx;
    uint16_t bytesNum;
    uint8_t bitIdx;
    bool active;
    DecodeEdge curEdge, prevEdge;
    DecodeState decodeState = NOT_SYNC;
    uint32_t timer_period_us, bit_time_us;
    uint32_t timer_max, timer_threshold;
    uint16_t timer_counter;
    bool clk;
    TIM_HandleTypeDef *tim;
public:
private:
    uint8_t getInputSignal();

    uint8_t getDataBit();

    void dataReadyCallback();

    void setOutput(const uint8_t &state);

public:
    ManchesterEncode(GPIO_TypeDef *signalPort, const uint16_t &signalPin, TIM_HandleTypeDef *tim);

    void pasteThisToTIMCallback(TIM_HandleTypeDef *htim);

    void startEncoding(uint8_t* data, uint8_t size);
};


#endif //TRANSMITTER_MANCHESTERENCODE_H
