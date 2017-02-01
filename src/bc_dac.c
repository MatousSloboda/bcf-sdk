#include "bc_dac.h"
#include "stm32l0xx.h"

#define BC_DAC_CHANNEL_DAC0_SET(_CODE_) (DAC1->DHR12L1 = _CODE_)
#define BC_DAC_CHANNEL_DAC1_SET(_CODE_) (DAC1->DHR12L2 = _CODE_)

// Approximate voltage to code constant
// #define BC_DAC_VOLTAGE_TO_CODE_CONSTANT 0.0000502626747

bc_dac_format_t bc_dac_setup[BC_DAC_CHANNEL_COUNT];

void bc_dac_init(bc_dac_channel_t channel, bc_dac_format_t format)
{
    /* store channel input format */
    bc_dac_setup[channel] = format;

    /* Enable peripheral code */
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;

    /* Software trigger, disable output buffer, enable DACs */
    DAC1->CR = DAC_CR_BOFF1 | DAC_CR_BOFF2 | DAC_CR_TSEL1 | DAC_CR_TSEL2 | DAC_CR_EN1 | DAC_CR_EN2;
}

void bc_dac_set_format(bc_dac_channel_t channel, bc_dac_format_t format)
{
    /* store channel input format */
    bc_dac_setup[channel] = format;
}

bc_dac_format_t bc_dac_get_format(bc_dac_channel_t channel)
{
    return bc_dac_setup[channel];
}

void bc_dac_set_output(bc_dac_channel_t channel, const void *output)
{
    uint16_t raw;

    /* Handle input format */
    switch (bc_dac_setup[channel])
    {
    case BC_DAC_FORMAT_8_BIT:
        raw = (*(uint16_t *) output << 8);
        break;
    case BC_DAC_FORMAT_16_BIT:
        raw = *(uint16_t *) output;
        break;
    case BC_DAC_FORMAT_24_BIT:
        raw = ((*(uint32_t *) output >> 8) & 0xffff);
        break;
    case BC_DAC_FORMAT_32_BIT:
        raw = ((*(uint32_t *) output >> 16) & 0xffff);
        break;
    default:
        for (;;);
        break;
    }

    /* Write raw on channel */
    switch (channel)
    {
    case BC_DAC_CHANNEL_DAC0:
        BC_DAC_CHANNEL_DAC0_SET(raw);
        break;
    case BC_DAC_CHANNEL_DAC1:
        BC_DAC_CHANNEL_DAC1_SET(raw);
        break;
    case BC_DAC_CHANNEL_COUNT:
    default:
        for (;;);
        break;
    }
}
