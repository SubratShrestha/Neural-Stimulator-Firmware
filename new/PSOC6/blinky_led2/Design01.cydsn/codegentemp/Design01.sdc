# THIS FILE IS AUTOMATICALLY GENERATED
# Project: D:\Projects\NS\Firmware\NeruoStim\PSOC6\blinky_led2\Design01.cydsn\Design01.cyprj
# Date: Fri, 11 Jun 2021 01:25:13 GMT
#set_units -time ns
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyClk_LF} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyFLL} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/fll}]]
create_clock -name {CyClk_HF0} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/hfclk_0}]]
create_clock -name {CyClk_Fast} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/fastclk}]]
create_clock -name {CyClk_Peri} -period 20 -waveform {0 10} [list [get_pins {ClockBlock/periclk}]]
create_generated_clock -name {CyClk_Slow} -source [get_pins {ClockBlock/periclk}] -edges {1 2 3} [list [get_pins {ClockBlock/slowclk}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/periclk}] -edges {1 50001 100001} [list [get_pins {ClockBlock/ff_div_12}]]
create_clock -name {CyPeriClk_App} -period 20 -waveform {0 10} [list [get_pins {ClockBlock/periclk_App}]]
create_clock -name {CyIMO} -period 125 -waveform {0 62.5} [list [get_pins {ClockBlock/imo}]]


# Component constraints for D:\Projects\NS\Firmware\NeruoStim\PSOC6\blinky_led2\Design01.cydsn\TopDesign\TopDesign.cysch
# Project: D:\Projects\NS\Firmware\NeruoStim\PSOC6\blinky_led2\Design01.cydsn\Design01.cyprj
# Date: Fri, 11 Jun 2021 01:24:47 GMT
