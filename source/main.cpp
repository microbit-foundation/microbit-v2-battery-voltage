#include "MicroBit.h"

MicroBit uBit;


// The ADC channel to be used for the battery voltage measurement
static int BATTERY_ADC_CHANNEL = -1;

/**
 * Measure the VDD voltage using the ADC channel for pin P0.
 *
 * @return int The VDD voltage in millivolts.
 */
static int get_vdd_millivolts() {
    if (BATTERY_ADC_CHANNEL == -1) {
        // Not initilised
        return 0;
    }
    // Second arg ensures channel is activated before we do an analog read,
    // so that it configures CH[n].CONFIG before we apply our changes
    (void)uBit.adc.getChannel(uBit.io.P0, true);

    // Configure CH[n].CONFIG with 0.6V internal reference and 1/6 gain
    // so max Vin detection is 0-3.6V
    NRF_SAADC->CH[BATTERY_ADC_CHANNEL].CONFIG =
        // Positive channel resistor control set to bypass resistor ladder
        (SAADC_CH_CONFIG_RESP_Bypass << SAADC_CH_CONFIG_RESP_Pos) |
        // Negative channel resistor control set to bypass resistor ladder
        (SAADC_CH_CONFIG_RESN_Bypass << SAADC_CH_CONFIG_RESN_Pos) |
        // Gain control set to 1/6
        (SAADC_CH_CONFIG_GAIN_Gain1_6 << SAADC_CH_CONFIG_GAIN_Pos) |
        // Reference control set to internal 0.6V reference
        (SAADC_CH_CONFIG_REFSEL_Internal << SAADC_CH_CONFIG_REFSEL_Pos) |
        // Acquisition time set to 3us
        (SAADC_CH_CONFIG_TACQ_3us << SAADC_CH_CONFIG_TACQ_Pos) |
        // Differential mode set to single ended
        (SAADC_CH_CONFIG_MODE_SE << SAADC_CH_CONFIG_MODE_Pos) |
        // Burst mode set to disabled
        (SAADC_CH_CONFIG_BURST_Disabled << SAADC_CH_CONFIG_BURST_Pos);

    // And set the positive input to VDD
    NRF_SAADC->CH[BATTERY_ADC_CHANNEL].PSELP = SAADC_CH_PSELP_PSELP_VDD << SAADC_CH_PSELP_PSELP_Pos;

    int vin_millivolts = (1000 * 0.6 * 6 * uBit.io.P0.getAnalogValue()) / 1024;

    return vin_millivolts;
}

/**
 * Initialize the ADC to be able to measure Vdd voltage.
 *
 * To do this it initialises and finds the ADC channel for pin P0,
 * which is hijacked by get_vdd_millivolts() to measure Vdd instead.
 */
static void vdd_adc_init() {
    // Ensure CODAL has configured the P0 ADC channel
    (void)uBit.io.P0.getAnalogValue();
    (void)uBit.adc.getChannel(uBit.io.P0, true);
    (void)uBit.io.P0.getAnalogValue();

    // Now look in the MCU ADC channels for a channel configured for P0 (P0.2 AIN0)
    for (size_t i = 0; i < NRF52_ADC_CHANNELS; i++) {
        if (NRF_SAADC->CH[i].PSELP == (SAADC_CH_PSELP_PSELP_AnalogInput0 << SAADC_CH_PSELP_PSELP_Pos)) {
            BATTERY_ADC_CHANNEL = i;
            break;
        }
    }

    // Do one measurement and throw away the result, as the first is always a bit off
    (void)get_vdd_millivolts();
}

int main() {
    uBit.init();

    vdd_adc_init();

    while (true) {
        ManagedString battery_mv = ManagedString(get_vdd_millivolts()) + " mV";

        uBit.serial.send(battery_mv + "\r\n");
        uBit.display.scroll(battery_mv);

        uBit.sleep(1000);
    }
}
