//
// Created by Ivan Kalesnikau on 11.04.2022.
//

#include "ManchesterEncode.h"

ManchesterEncode::ManchesterEncode(GPIO_TypeDef *signalPort, const uint16_t &signalPin, TIM_HandleTypeDef *tim)
{
    this->signalPort = signalPort;
    this->signalPin = signalPin;
    this->tim = tim;
}

void ManchesterEncode::setOutput(const uint8_t &state)
{
    HAL_GPIO_WritePin(signalPort, signalPin, (GPIO_PinState) state);
}

void ManchesterEncode::startEncoding(uint8_t *data, uint8_t size)
{
    bitIdx = 0;
    byteIdx = 0;
    if (size > MANCH_DATA_BYTES_NUM)
    {
        bytesNum = MANCH_BYTES_NUM;
    } else
    {
        bytesNum = size + MANCH_SYNC_BYTES_NUM;
    }
    memcpy(&this->data[MANCH_SYNC_BYTES_NUM], data, bytesNum - MANCH_SYNC_BYTES_NUM);
    this->data[0] = MANCH_SYNC_FIELD & 0xFF;
    this->data[1] = (MANCH_SYNC_FIELD & 0xFF00) >> 8;

    timer_counter = 0;
    clk = 1;
    active = true;
}

uint8_t ManchesterEncode::getDataBit()
{
    uint8_t res;

    uint8_t curByte = data[byteIdx];
    uint8_t curBitIdx = bitIdx;

    res = (curByte >> curBitIdx) & 0x01;

    return res;
}

void ManchesterEncode::pasteThisToTIMCallback(TIM_HandleTypeDef *htim)
{
    if (htim == tim)
    {
        // Encoding process
        if (active)
        {
            if ((timer_counter == (MANCH_ENCODE_TIM_CNT_PER_BIT / 2)) ||
                (timer_counter == MANCH_ENCODE_TIM_CNT_PER_BIT))
            {
                uint8_t currentBit = getDataBit();
                uint8_t outputBit = currentBit ^clk;
                setOutput(outputBit);
                clk ^= 1;
            }

            if (timer_counter == MANCH_ENCODE_TIM_CNT_PER_BIT)
            {
                ++bitIdx;

                if (bitIdx == 8)
                {
                    bitIdx = 0;

                    ++byteIdx;
                    if (byteIdx == bytesNum)
                    {
                        active = false;
                    }
                }

                timer_counter = 0;
            }

            timer_counter++;
        }
    }
}