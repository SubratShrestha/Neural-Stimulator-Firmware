# THIS FILE IS AUTOMATICALLY GENERATED
# Project: D:\Projects\NS\Neural-Stimulator-Firmware\new\PSOC6\CustomService.cydsn\CustomService.cyprj
# Date: Mon, 04 Oct 2021 13:42:25 GMT
#set_units -time ns
create_clock -name {CyWCO} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/wco}]]
create_clock -name {CyClk_LF} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyFLL} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/fll}]]
create_clock -name {CyClk_HF0} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/hfclk_0}]]
create_clock -name {CyClk_Fast} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/fastclk}]]
create_clock -name {CyClk_Peri} -period 20 -waveform {0 10} [list [get_pins {ClockBlock/periclk}]]
create_generated_clock -name {CyClk_Slow} -source [get_pins {ClockBlock/periclk}] -edges {1 2 3} [list [get_pins {ClockBlock/slowclk}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/periclk}] -edges {1 51 101} [list [get_pins {ClockBlock/ff_div_11}]]
create_generated_clock -name {Clock_2} -source [get_pins {ClockBlock/periclk}] -edges {1 51 101} [list [get_pins {ClockBlock/ff_div_12}]]
create_generated_clock -name {UART_1_SCBCLK} -source [get_pins {ClockBlock/periclk}] -edges {1 37 73} [list [get_pins {ClockBlock/ff_div_5}]]
create_clock -name {CyPeriClk_App} -period 20 -waveform {0 10} [list [get_pins {ClockBlock/periclk_App}]]
create_clock -name {CyExtClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/ext}]]
create_clock -name {CyIMO} -period 125 -waveform {0 62.5} [list [get_pins {ClockBlock/imo}]]


# Component constraints for D:\Projects\NS\Neural-Stimulator-Firmware\new\PSOC6\CustomService.cydsn\TopDesign\TopDesign.cysch
# Project: D:\Projects\NS\Neural-Stimulator-Firmware\new\PSOC6\CustomService.cydsn\CustomService.cyprj
# Date: Mon, 04 Oct 2021 13:42:08 GMT
