EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x06 J4
U 1 1 60AF5F34
P 7150 2600
F 0 "J4" H 7068 2075 50  0000 C CNN
F 1 "POWER" H 7068 2166 50  0000 C CNN
F 2 "kk_369_right_angle:Molex_KK-396_Right_Angle" H 7150 2600 50  0001 C CNN
F 3 "~" H 7150 2600 50  0001 C CNN
	1    7150 2600
	-1   0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATmega:ATmega168-20AU U3
U 1 1 629C4B67
P 2050 2600
F 0 "U3" H 2200 1050 50  0000 L CNN
F 1 "ATmega168-20AU" H 2200 950 50  0000 L CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 2050 2600 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2545-8-bit-AVR-Microcontroller-ATmega48-88-168_Datasheet.pdf" H 2050 2600 50  0001 C CNN
	1    2050 2600
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0132
U 1 1 609B7335
P 2100 1000
F 0 "#PWR0132" H 2100 850 50  0001 C CNN
F 1 "+5V" H 2115 1173 50  0000 C CNN
F 2 "" H 2100 1000 50  0001 C CNN
F 3 "" H 2100 1000 50  0001 C CNN
	1    2100 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 1100 2050 1050
Wire Wire Line
	2050 1050 2100 1050
Wire Wire Line
	2100 1050 2100 1000
Wire Wire Line
	2100 1050 2150 1050
Wire Wire Line
	2150 1050 2150 1100
Connection ~ 2100 1050
$Comp
L power:GNDD #PWR0133
U 1 1 60A30B61
P 2050 4150
F 0 "#PWR0133" H 2050 3900 50  0001 C CNN
F 1 "GNDD" H 2054 3995 50  0000 C CNN
F 2 "" H 2050 4150 50  0001 C CNN
F 3 "" H 2050 4150 50  0001 C CNN
	1    2050 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 4150 2050 4100
Wire Wire Line
	2650 3300 3250 3300
Wire Wire Line
	2650 2400 3050 2400
Text Label 3050 2400 2    50   ~ 0
LOCK
Wire Wire Line
	2650 3400 3250 3400
Wire Wire Line
	2650 1500 3050 1500
Text Label 3050 1500 2    50   ~ 0
DIR
Wire Wire Line
	2650 1600 3050 1600
Text Label 3050 1600 2    50   ~ 0
SPEED
Wire Wire Line
	2650 2600 3050 2600
Text Label 3050 2600 2    50   ~ 0
REF
$Comp
L Device:C C12
U 1 1 60B9F084
P 1000 1750
F 0 "C12" H 1115 1796 50  0000 L CNN
F 1 "100n" H 1115 1705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1038 1600 50  0001 C CNN
F 3 "~" H 1000 1750 50  0001 C CNN
	1    1000 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 1500 1000 1600
Wire Wire Line
	1000 1900 1000 2000
$Comp
L power:+5V #PWR0134
U 1 1 60BC41B1
P 1000 1500
F 0 "#PWR0134" H 1000 1350 50  0001 C CNN
F 1 "+5V" H 1015 1673 50  0000 C CNN
F 2 "" H 1000 1500 50  0001 C CNN
F 3 "" H 1000 1500 50  0001 C CNN
	1    1000 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0135
U 1 1 60BC6414
P 1000 2000
F 0 "#PWR0135" H 1000 1750 50  0001 C CNN
F 1 "GNDD" H 1004 1845 50  0000 C CNN
F 2 "" H 1000 2000 50  0001 C CNN
F 3 "" H 1000 2000 50  0001 C CNN
	1    1000 2000
	1    0    0    -1  
$EndComp
$Comp
L Connector:AVR-ISP-6 J6
U 1 1 60BCE6CB
P 5200 1900
F 0 "J6" H 4870 1996 50  0000 R CNN
F 1 "AVR-ISP-6" H 4870 1905 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" V 4950 1950 50  0001 C CNN
F 3 " ~" H 3925 1350 50  0001 C CNN
	1    5200 1900
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4800 1700 3100 1700
Wire Wire Line
	3100 1700 3000 1800
Wire Wire Line
	3000 1800 2650 1800
$Comp
L power:GNDD #PWR0136
U 1 1 60C31455
P 5300 2300
F 0 "#PWR0136" H 5300 2050 50  0001 C CNN
F 1 "GNDD" H 5304 2145 50  0000 C CNN
F 2 "" H 5300 2300 50  0001 C CNN
F 3 "" H 5300 2300 50  0001 C CNN
	1    5300 2300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0137
U 1 1 60C5326F
P 5300 1400
F 0 "#PWR0137" H 5300 1250 50  0001 C CNN
F 1 "+5V" H 5315 1573 50  0000 C CNN
F 2 "" H 5300 1400 50  0001 C CNN
F 3 "" H 5300 1400 50  0001 C CNN
	1    5300 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 2900 4700 2900
Wire Wire Line
	4700 2900 4700 2000
Wire Wire Line
	4700 2000 4800 2000
$Comp
L Device:R R42
U 1 1 60C960DC
P 4700 1350
F 0 "R42" H 4630 1304 50  0000 R CNN
F 1 "10k" H 4630 1395 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4630 1350 50  0001 C CNN
F 3 "~" H 4700 1350 50  0001 C CNN
	1    4700 1350
	-1   0    0    1   
$EndComp
Wire Wire Line
	4700 1500 4700 2000
Connection ~ 4700 2000
$Comp
L power:+5V #PWR0138
U 1 1 60CB7E68
P 4700 1200
F 0 "#PWR0138" H 4700 1050 50  0001 C CNN
F 1 "+5V" H 4715 1373 50  0000 C CNN
F 2 "" H 4700 1200 50  0001 C CNN
F 3 "" H 4700 1200 50  0001 C CNN
	1    4700 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 1900 4800 1900
Wire Wire Line
	4800 1800 3100 1800
Wire Wire Line
	3100 1800 3000 1700
Wire Wire Line
	2650 1700 3000 1700
$Comp
L power:GNDD #PWR0139
U 1 1 60D50F52
P 3500 2650
F 0 "#PWR0139" H 3500 2400 50  0001 C CNN
F 1 "GNDD" H 3504 2495 50  0000 C CNN
F 2 "" H 3500 2650 50  0001 C CNN
F 3 "" H 3500 2650 50  0001 C CNN
	1    3500 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2250 4000 2350
Wire Wire Line
	3900 2250 4000 2250
Wire Wire Line
	3500 2250 3600 2250
Wire Wire Line
	3500 2350 3500 2250
$Comp
L Device:C C13
U 1 1 60D0D7FF
P 3500 2500
F 0 "C13" H 3615 2546 50  0000 L CNN
F 1 "27p" H 3615 2455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3538 2350 50  0001 C CNN
F 3 "~" H 3500 2500 50  0001 C CNN
	1    3500 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 60CE08E4
P 3750 2250
F 0 "Y1" H 3600 2450 50  0000 C CNN
F 1 "16MHz" H 3850 2450 50  0000 C CNN
F 2 "Crystal:Crystal_SMD_HC49-SD" H 3750 2250 50  0001 C CNN
F 3 "~" H 3750 2250 50  0001 C CNN
	1    3750 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C14
U 1 1 60D0DF55
P 4000 2500
F 0 "C14" H 4115 2546 50  0000 L CNN
F 1 "27p" H 4115 2455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4038 2350 50  0001 C CNN
F 3 "~" H 4000 2500 50  0001 C CNN
	1    4000 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0140
U 1 1 60D7571E
P 4000 2650
F 0 "#PWR0140" H 4000 2400 50  0001 C CNN
F 1 "GNDD" H 4004 2495 50  0000 C CNN
F 2 "" H 4000 2650 50  0001 C CNN
F 3 "" H 4000 2650 50  0001 C CNN
	1    4000 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2250 4000 2000
Wire Wire Line
	4000 2000 2650 2000
Connection ~ 4000 2250
Wire Wire Line
	2650 2100 3500 2100
Wire Wire Line
	3500 2100 3500 2250
Connection ~ 3500 2250
$Comp
L Device:LED D6
U 1 1 61233BB7
P 5250 3450
F 0 "D6" V 5289 3332 50  0000 R CNN
F 1 "LOCK" V 5198 3332 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 5250 3450 50  0001 C CNN
F 3 "~" H 5250 3450 50  0001 C CNN
	1    5250 3450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R43
U 1 1 6123443D
P 5250 3750
F 0 "R43" H 5320 3796 50  0000 L CNN
F 1 "330*" H 5320 3705 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5180 3750 50  0001 C CNN
F 3 "~" H 5250 3750 50  0001 C CNN
	1    5250 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0141
U 1 1 612605FD
P 5250 3900
F 0 "#PWR0141" H 5250 3650 50  0001 C CNN
F 1 "GNDD" H 5254 3745 50  0000 C CNN
F 2 "" H 5250 3900 50  0001 C CNN
F 3 "" H 5250 3900 50  0001 C CNN
	1    5250 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3300 5250 3250
Wire Wire Line
	5250 3250 4950 3250
Text Label 4950 3250 0    50   ~ 0
LOCK
$Comp
L adm485:ADM485AR U4
U 1 1 609A3A40
P 7750 4200
F 0 "U4" H 7750 4575 50  0000 C CNN
F 1 "ADM485AR" H 7750 4484 50  0000 C CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 7750 4200 50  0001 C CNN
F 3 "" H 7750 4200 50  0001 C CNN
	1    7750 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 4050 6850 4050
Text Label 6850 4050 0    50   ~ 0
RXD
Wire Wire Line
	7300 4350 6850 4350
Text Label 6850 4350 0    50   ~ 0
TXD
Wire Wire Line
	7300 4150 6850 4150
$Comp
L power:+5V #PWR0142
U 1 1 60B53369
P 8250 3950
F 0 "#PWR0142" H 8250 3800 50  0001 C CNN
F 1 "+5V" H 8265 4123 50  0000 C CNN
F 2 "" H 8250 3950 50  0001 C CNN
F 3 "" H 8250 3950 50  0001 C CNN
	1    8250 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 4050 8250 4050
Wire Wire Line
	8250 4050 8250 3950
$Comp
L power:GNDD #PWR0145
U 1 1 60BF4729
P 8250 4450
F 0 "#PWR0145" H 8250 4200 50  0001 C CNN
F 1 "GNDD" H 8254 4295 50  0000 C CNN
F 2 "" H 8250 4450 50  0001 C CNN
F 3 "" H 8250 4450 50  0001 C CNN
	1    8250 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 4350 8250 4350
Wire Wire Line
	8250 4350 8250 4450
$Comp
L Device:R R47
U 1 1 60C38457
P 8600 4200
F 0 "R47" H 8531 4154 50  0000 R CNN
F 1 "120*" H 8531 4245 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8530 4200 50  0001 C CNN
F 3 "~" H 8600 4200 50  0001 C CNN
	1    8600 4200
	1    0    0    1   
$EndComp
Wire Wire Line
	8200 4150 8300 4150
Wire Wire Line
	8300 4150 8300 4000
Wire Wire Line
	8300 4000 8600 4000
Wire Wire Line
	8600 4000 8600 4050
Wire Wire Line
	8200 4250 8300 4250
Wire Wire Line
	8300 4250 8300 4400
Wire Wire Line
	8300 4400 8600 4400
Wire Wire Line
	8600 4400 8600 4350
$Comp
L Connector_Generic:Conn_01x03 J7
U 1 1 60D93692
P 9250 4250
F 0 "J7" H 9330 4292 50  0000 L CNN
F 1 "NSM E-004 Controller RS485" H 9330 4201 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 9250 4250 50  0001 C CNN
F 3 "~" H 9250 4250 50  0001 C CNN
	1    9250 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 4000 8850 4000
Wire Wire Line
	8850 4000 8850 4150
Wire Wire Line
	8850 4150 9050 4150
Connection ~ 8600 4000
Wire Wire Line
	9050 4250 8850 4250
Wire Wire Line
	8850 4250 8850 4400
Wire Wire Line
	8850 4400 8600 4400
Connection ~ 8600 4400
$Comp
L power:GNDD #PWR0146
U 1 1 60DF9552
P 8950 4450
F 0 "#PWR0146" H 8950 4200 50  0001 C CNN
F 1 "GNDD" H 8954 4295 50  0000 C CNN
F 2 "" H 8950 4450 50  0001 C CNN
F 3 "" H 8950 4450 50  0001 C CNN
	1    8950 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 4450 8950 4350
Wire Wire Line
	8950 4350 9050 4350
Text Label 9000 4150 2    50   ~ 0
B
Text Label 9000 4250 2    50   ~ 0
A
Text Label 6850 4250 0    50   ~ 0
DE
Wire Wire Line
	2650 3200 3050 3200
Text Label 3050 3200 2    50   ~ 0
TXD
Wire Wire Line
	2650 3100 3050 3100
Text Label 3050 3100 2    50   ~ 0
RXD
Wire Wire Line
	2650 2700 3050 2700
Text Label 3050 2700 2    50   ~ 0
~RE
$Comp
L Connector_Generic:Conn_01x05 J8
U 1 1 60FDA9A5
P 9250 3300
F 0 "J8" H 9330 3342 50  0000 L CNN
F 1 "UART" H 9330 3251 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B5B-XH-A_1x05_P2.50mm_Vertical" H 9250 3300 50  0001 C CNN
F 3 "~" H 9250 3300 50  0001 C CNN
	1    9250 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R48
U 1 1 61026ED7
P 8750 3200
F 0 "R48" V 8543 3200 50  0000 C CNN
F 1 "220" V 8634 3200 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8680 3200 50  0001 C CNN
F 3 "~" H 8750 3200 50  0001 C CNN
	1    8750 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	8600 3200 8300 3200
Wire Wire Line
	8300 3300 9050 3300
Wire Wire Line
	8900 3200 9050 3200
Text Label 8300 3200 0    50   ~ 0
RXD
Text Label 8300 3300 0    50   ~ 0
TXD
Wire Wire Line
	9050 3400 8950 3400
Wire Wire Line
	8950 3400 8950 3500
Wire Wire Line
	9050 3500 8950 3500
Connection ~ 8950 3500
Wire Wire Line
	8950 3500 8950 3550
$Comp
L power:GNDD #PWR0147
U 1 1 611271E0
P 8950 3550
F 0 "#PWR0147" H 8950 3300 50  0001 C CNN
F 1 "GNDD" H 8954 3395 50  0000 C CNN
F 2 "" H 8950 3550 50  0001 C CNN
F 3 "" H 8950 3550 50  0001 C CNN
	1    8950 3550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0148
U 1 1 6114E02E
P 8950 3050
F 0 "#PWR0148" H 8950 2900 50  0001 C CNN
F 1 "+5V" H 8965 3223 50  0000 C CNN
F 2 "" H 8950 3050 50  0001 C CNN
F 3 "" H 8950 3050 50  0001 C CNN
	1    8950 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 3050 8950 3100
Wire Wire Line
	8950 3100 9050 3100
$Comp
L Connector_Generic:Conn_01x03 J9
U 1 1 611C4C7E
P 9250 1600
F 0 "J9" H 9330 1642 50  0000 L CNN
F 1 "Speed/reverse" H 9330 1551 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 9250 1600 50  0001 C CNN
F 3 "~" H 9250 1600 50  0001 C CNN
	1    9250 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0149
U 1 1 611C9891
P 8950 2500
F 0 "#PWR0149" H 8950 2250 50  0001 C CNN
F 1 "GNDD" H 8954 2345 50  0000 C CNN
F 2 "" H 8950 2500 50  0001 C CNN
F 3 "" H 8950 2500 50  0001 C CNN
	1    8950 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 1600 8950 1600
Wire Wire Line
	8200 1700 7800 1700
Wire Wire Line
	8200 1500 7800 1500
Text Label 7800 1500 0    50   ~ 0
DIR
Text Label 7800 1700 0    50   ~ 0
SPEED
Wire Wire Line
	6850 4250 7300 4250
Text Label 6850 4150 0    50   ~ 0
~RE
Wire Wire Line
	2650 2800 3050 2800
Text Label 3050 2800 2    50   ~ 0
DE
Wire Wire Line
	2650 2300 3050 2300
Text Label 3050 2300 2    50   ~ 0
485\~UART
$Comp
L Device:Jumper JP2
U 1 1 60D2629D
P 3900 4100
F 0 "JP2" H 3900 4364 50  0000 C CNN
F 1 "RS485 or UART/232 select" H 3900 4273 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 3900 4100 50  0001 C CNN
F 3 "~" H 3900 4100 50  0001 C CNN
	1    3900 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 4100 3200 4100
Text Label 3200 4100 0    50   ~ 0
485\~UART
$Comp
L power:GNDD #PWR0155
U 1 1 60D8147C
P 4250 4150
F 0 "#PWR0155" H 4250 3900 50  0001 C CNN
F 1 "GNDD" H 4254 3995 50  0000 C CNN
F 2 "" H 4250 4150 50  0001 C CNN
F 3 "" H 4250 4150 50  0001 C CNN
	1    4250 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 4100 4250 4100
Wire Wire Line
	4250 4100 4250 4150
$Comp
L Device:Jumper JP3
U 1 1 60DC6E55
P 2800 7000
F 0 "JP3" H 2800 7264 50  0000 C CNN
F 1 "OPTIONAL GND JOIN" H 2800 7173 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 2800 7000 50  0001 C CNN
F 3 "~" H 2800 7000 50  0001 C CNN
	1    2800 7000
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0156
U 1 1 60DCC2BB
P 3150 7050
F 0 "#PWR0156" H 3150 6800 50  0001 C CNN
F 1 "GNDD" H 3154 6895 50  0000 C CNN
F 2 "" H 3150 7050 50  0001 C CNN
F 3 "" H 3150 7050 50  0001 C CNN
	1    3150 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 7000 3150 7000
Wire Wire Line
	3150 7000 3150 7050
$Comp
L power:GNDA #PWR0157
U 1 1 60E26C46
P 2450 7050
F 0 "#PWR0157" H 2450 6800 50  0001 C CNN
F 1 "GNDA" H 2455 6877 50  0000 C CNN
F 2 "" H 2450 7050 50  0001 C CNN
F 3 "" H 2450 7050 50  0001 C CNN
	1    2450 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 7050 2450 7000
Wire Wire Line
	2450 7000 2500 7000
$Comp
L Device:R R53
U 1 1 60F592CB
P 8350 1500
F 0 "R53" V 8143 1500 50  0000 C CNN
F 1 "200" V 8234 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8280 1500 50  0001 C CNN
F 3 "~" H 8350 1500 50  0001 C CNN
	1    8350 1500
	0    1    1    0   
$EndComp
$Comp
L Device:R R54
U 1 1 60F5D8F6
P 8350 1700
F 0 "R54" V 8143 1700 50  0000 C CNN
F 1 "200" V 8234 1700 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8280 1700 50  0001 C CNN
F 3 "~" H 8350 1700 50  0001 C CNN
	1    8350 1700
	0    1    -1   0   
$EndComp
Wire Wire Line
	8500 1500 8750 1500
Wire Wire Line
	8500 1700 9050 1700
$Comp
L Device:Jumper TP1
U 1 1 61126A46
P 3650 5400
F 0 "TP1" H 3650 5664 50  0000 C CNN
F 1 "TEST POINT DGND" H 3650 5573 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x01_P2.54mm_Vertical" H 3650 5400 50  0001 C CNN
F 3 "~" H 3650 5400 50  0001 C CNN
	1    3650 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0109
U 1 1 61128AFA
P 3950 5400
F 0 "#PWR0109" H 3950 5150 50  0001 C CNN
F 1 "GNDD" H 3954 5245 50  0000 C CNN
F 2 "" H 3950 5400 50  0001 C CNN
F 3 "" H 3950 5400 50  0001 C CNN
	1    3950 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0158
U 1 1 6115F435
P 3350 5400
F 0 "#PWR0158" H 3350 5150 50  0001 C CNN
F 1 "GNDD" H 3354 5245 50  0000 C CNN
F 2 "" H 3350 5400 50  0001 C CNN
F 3 "" H 3350 5400 50  0001 C CNN
	1    3350 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper TP2
U 1 1 61193221
P 3650 5950
F 0 "TP2" H 3650 6214 50  0000 C CNN
F 1 "TEST POINT AGND" H 3650 6123 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x01_P2.54mm_Vertical" H 3650 5950 50  0001 C CNN
F 3 "~" H 3650 5950 50  0001 C CNN
	1    3650 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR0159
U 1 1 611CF547
P 3350 5950
F 0 "#PWR0159" H 3350 5700 50  0001 C CNN
F 1 "GNDA" H 3355 5777 50  0000 C CNN
F 2 "" H 3350 5950 50  0001 C CNN
F 3 "" H 3350 5950 50  0001 C CNN
	1    3350 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR0160
U 1 1 611F9480
P 3950 5950
F 0 "#PWR0160" H 3950 5700 50  0001 C CNN
F 1 "GNDA" H 3955 5777 50  0000 C CNN
F 2 "" H 3950 5950 50  0001 C CNN
F 3 "" H 3950 5950 50  0001 C CNN
	1    3950 5950
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper TP3
U 1 1 61347F05
P 3650 6500
F 0 "TP3" H 3650 6764 50  0000 C CNN
F 1 "TEST POINT TACHO" H 3650 6673 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x01_P2.54mm_Vertical" H 3650 6500 50  0001 C CNN
F 3 "~" H 3650 6500 50  0001 C CNN
	1    3650 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 6500 3350 6650
Wire Wire Line
	3350 6650 3950 6650
Wire Wire Line
	3950 6650 3950 6500
Text Label 3400 6650 0    50   ~ 0
TACHO
$Comp
L Device:Jumper TP4
U 1 1 613CD486
P 3650 7000
F 0 "TP4" H 3650 7264 50  0000 C CNN
F 1 "TEST POINT LOCK" H 3650 7173 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x01_P2.54mm_Vertical" H 3650 7000 50  0001 C CNN
F 3 "~" H 3650 7000 50  0001 C CNN
	1    3650 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 7000 3350 7150
Wire Wire Line
	3350 7150 3950 7150
Wire Wire Line
	3950 7150 3950 7000
Text Label 3400 7150 0    50   ~ 0
LOCK
$Comp
L Device:Jumper TP5
U 1 1 614014B6
P 4450 6500
F 0 "TP5" H 4450 6764 50  0000 C CNN
F 1 "TEST POINT REF" H 4450 6673 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x01_P2.54mm_Vertical" H 4450 6500 50  0001 C CNN
F 3 "~" H 4450 6500 50  0001 C CNN
	1    4450 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 6500 4150 6650
Wire Wire Line
	4150 6650 4750 6650
Wire Wire Line
	4750 6650 4750 6500
Text Label 5300 6650 2    50   ~ 0
REF
$Comp
L Device:R R55
U 1 1 61458608
P 4950 6650
F 0 "R55" V 4743 6650 50  0000 C CNN
F 1 "200" V 4834 6650 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4880 6650 50  0001 C CNN
F 3 "~" H 4950 6650 50  0001 C CNN
	1    4950 6650
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 6650 4750 6650
Connection ~ 4750 6650
Wire Wire Line
	5100 6650 5300 6650
$Comp
L Connector_Generic:Conn_01x05 J11
U 1 1 615AEC52
P 9250 2200
F 0 "J11" H 9330 2242 50  0000 L CNN
F 1 "NSM E-004 Counter power & RS485" H 9330 2151 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B5B-XH-A_1x05_P2.50mm_Vertical" H 9250 2200 50  0001 C CNN
F 3 "~" H 9250 2200 50  0001 C CNN
	1    9250 2200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0162
U 1 1 6160737C
P 8850 900
F 0 "#PWR0162" H 8850 750 50  0001 C CNN
F 1 "+5V" H 8865 1073 50  0000 C CNN
F 2 "" H 8850 900 50  0001 C CNN
F 3 "" H 8850 900 50  0001 C CNN
	1    8850 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 2000 9050 2000
Wire Wire Line
	9050 2200 8650 2200
Wire Wire Line
	9050 2300 8650 2300
Wire Wire Line
	9050 2400 8950 2400
Wire Wire Line
	8950 2400 8950 2500
Text Label 8650 2200 0    50   ~ 0
A
Text Label 8650 2300 0    50   ~ 0
B
Wire Wire Line
	8850 900  8850 1150
Wire Wire Line
	8950 1600 8950 2400
Connection ~ 8950 2400
$Comp
L Connector_Generic:Conn_01x03 J10
U 1 1 618D3D6D
P 9250 1150
F 0 "J10" H 9330 1192 50  0000 L CNN
F 1 "NSM E-004 Counter power/speed select" H 9330 1101 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 9250 1150 50  0001 C CNN
F 3 "~" H 9250 1150 50  0001 C CNN
	1    9250 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 1150 8850 1150
Connection ~ 8850 1150
Wire Wire Line
	8850 1150 8850 2000
Wire Wire Line
	8950 1600 8950 1250
Wire Wire Line
	8950 1250 9050 1250
Connection ~ 8950 1600
Wire Wire Line
	9050 1050 8750 1050
Wire Wire Line
	8750 1050 8750 1500
Connection ~ 8750 1500
Wire Wire Line
	8750 1500 9050 1500
$Sheet
S 3250 3200 850  300 
U 61F382E3
F0 "analog section" 50
F1 "analog.sch" 50
F2 "Tacho" O R 4100 3300 50 
F3 "PWM" I L 3250 3400 50 
F4 "Reverse" I L 3250 3300 50 
$EndSheet
Wire Wire Line
	4400 1400 4400 3300
Wire Wire Line
	4400 3300 4100 3300
Wire Wire Line
	2650 1400 4400 1400
$Comp
L power:+36V #PWR?
U 1 1 62246AF8
P 7750 2400
F 0 "#PWR?" H 7750 2250 50  0001 C CNN
F 1 "+36V" V 7765 2528 50  0000 L CNN
F 2 "" H 7750 2400 50  0001 C CNN
F 3 "" H 7750 2400 50  0001 C CNN
	1    7750 2400
	0    1    1    0   
$EndComp
$Comp
L power:GNDD #PWR?
U 1 1 622566E7
P 7750 2500
F 0 "#PWR?" H 7750 2250 50  0001 C CNN
F 1 "GNDD" V 7754 2390 50  0000 R CNN
F 2 "" H 7750 2500 50  0001 C CNN
F 3 "" H 7750 2500 50  0001 C CNN
	1    7750 2500
	0    -1   -1   0   
$EndComp
$Comp
L power:+15V #PWR?
U 1 1 6227E062
P 7750 2700
F 0 "#PWR?" H 7750 2550 50  0001 C CNN
F 1 "+15V" V 7765 2828 50  0000 L CNN
F 2 "" H 7750 2700 50  0001 C CNN
F 3 "" H 7750 2700 50  0001 C CNN
	1    7750 2700
	0    1    1    0   
$EndComp
$Comp
L power:-15V #PWR?
U 1 1 6227EF3D
P 7750 2900
F 0 "#PWR?" H 7750 3000 50  0001 C CNN
F 1 "-15V" V 7765 3028 50  0000 L CNN
F 2 "" H 7750 2900 50  0001 C CNN
F 3 "" H 7750 2900 50  0001 C CNN
	1    7750 2900
	0    1    1    0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 62288661
P 7750 2800
F 0 "#PWR?" H 7750 2550 50  0001 C CNN
F 1 "GNDA" V 7755 2672 50  0000 R CNN
F 2 "" H 7750 2800 50  0001 C CNN
F 3 "" H 7750 2800 50  0001 C CNN
	1    7750 2800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7350 2600 7750 2600
Wire Wire Line
	7350 2800 7750 2800
Wire Wire Line
	7350 2900 7750 2900
Wire Wire Line
	7750 2700 7350 2700
$Comp
L power:+5V #PWR?
U 1 1 622D5EFF
P 7750 2600
F 0 "#PWR?" H 7750 2450 50  0001 C CNN
F 1 "+5V" V 7765 2728 50  0000 L CNN
F 2 "" H 7750 2600 50  0001 C CNN
F 3 "" H 7750 2600 50  0001 C CNN
	1    7750 2600
	0    1    1    0   
$EndComp
Wire Wire Line
	7350 2500 7750 2500
Wire Wire Line
	7350 2400 7750 2400
$Comp
L power:+5V #PWR?
U 1 1 6231086A
P 8750 6150
AR Path="/61F382E3/6231086A" Ref="#PWR?"  Part="1" 
AR Path="/6231086A" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8750 6000 50  0001 C CNN
F 1 "+5V" H 8765 6323 50  0000 C CNN
F 2 "" H 8750 6150 50  0001 C CNN
F 3 "" H 8750 6150 50  0001 C CNN
	1    8750 6150
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG?
U 1 1 62310870
P 8750 6150
AR Path="/61F382E3/62310870" Ref="#FLG?"  Part="1" 
AR Path="/62310870" Ref="#FLG?"  Part="1" 
F 0 "#FLG?" H 8750 6225 50  0001 C CNN
F 1 "PWR_FLAG" H 8750 6323 50  0000 C CNN
F 2 "" H 8750 6150 50  0001 C CNN
F 3 "~" H 8750 6150 50  0001 C CNN
	1    8750 6150
	-1   0    0    1   
$EndComp
$Comp
L power:+15V #PWR?
U 1 1 62310876
P 10400 6150
AR Path="/61F382E3/62310876" Ref="#PWR?"  Part="1" 
AR Path="/62310876" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10400 6000 50  0001 C CNN
F 1 "+15V" H 10415 6323 50  0000 C CNN
F 2 "" H 10400 6150 50  0001 C CNN
F 3 "" H 10400 6150 50  0001 C CNN
	1    10400 6150
	1    0    0    -1  
$EndComp
$Comp
L power:-15V #PWR?
U 1 1 6231087C
P 9150 6150
AR Path="/61F382E3/6231087C" Ref="#PWR?"  Part="1" 
AR Path="/6231087C" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9150 6250 50  0001 C CNN
F 1 "-15V" H 9165 6323 50  0000 C CNN
F 2 "" H 9150 6150 50  0001 C CNN
F 3 "" H 9150 6150 50  0001 C CNN
	1    9150 6150
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG?
U 1 1 62310882
P 10400 6150
AR Path="/61F382E3/62310882" Ref="#FLG?"  Part="1" 
AR Path="/62310882" Ref="#FLG?"  Part="1" 
F 0 "#FLG?" H 10400 6225 50  0001 C CNN
F 1 "PWR_FLAG" H 10400 6323 50  0000 C CNN
F 2 "" H 10400 6150 50  0001 C CNN
F 3 "~" H 10400 6150 50  0001 C CNN
	1    10400 6150
	1    0    0    1   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 62310888
P 10000 6150
AR Path="/61F382E3/62310888" Ref="#PWR?"  Part="1" 
AR Path="/62310888" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10000 5900 50  0001 C CNN
F 1 "GNDA" H 10005 5977 50  0000 C CNN
F 2 "" H 10000 6150 50  0001 C CNN
F 3 "" H 10000 6150 50  0001 C CNN
	1    10000 6150
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG?
U 1 1 6231088E
P 10000 6150
AR Path="/61F382E3/6231088E" Ref="#FLG?"  Part="1" 
AR Path="/6231088E" Ref="#FLG?"  Part="1" 
F 0 "#FLG?" H 10000 6225 50  0001 C CNN
F 1 "PWR_FLAG" H 10000 6323 50  0000 C CNN
F 2 "" H 10000 6150 50  0001 C CNN
F 3 "~" H 10000 6150 50  0001 C CNN
	1    10000 6150
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG?
U 1 1 62310894
P 9150 6150
AR Path="/61F382E3/62310894" Ref="#FLG?"  Part="1" 
AR Path="/62310894" Ref="#FLG?"  Part="1" 
F 0 "#FLG?" H 9150 6225 50  0001 C CNN
F 1 "PWR_FLAG" H 9150 6323 50  0000 C CNN
F 2 "" H 9150 6150 50  0001 C CNN
F 3 "~" H 9150 6150 50  0001 C CNN
	1    9150 6150
	-1   0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG?
U 1 1 6231089A
P 9600 6150
AR Path="/61F382E3/6231089A" Ref="#FLG?"  Part="1" 
AR Path="/6231089A" Ref="#FLG?"  Part="1" 
F 0 "#FLG?" H 9600 6225 50  0001 C CNN
F 1 "PWR_FLAG" H 9600 6323 50  0000 C CNN
F 2 "" H 9600 6150 50  0001 C CNN
F 3 "~" H 9600 6150 50  0001 C CNN
	1    9600 6150
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR?
U 1 1 623108A0
P 9600 6150
AR Path="/61F382E3/623108A0" Ref="#PWR?"  Part="1" 
AR Path="/623108A0" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9600 5900 50  0001 C CNN
F 1 "GNDD" H 9604 5995 50  0000 C CNN
F 2 "" H 9600 6150 50  0001 C CNN
F 3 "" H 9600 6150 50  0001 C CNN
	1    9600 6150
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG?
U 1 1 623108A6
P 10800 6150
AR Path="/61F382E3/623108A6" Ref="#FLG?"  Part="1" 
AR Path="/623108A6" Ref="#FLG?"  Part="1" 
F 0 "#FLG?" H 10800 6225 50  0001 C CNN
F 1 "PWR_FLAG" H 10800 6323 50  0000 C CNN
F 2 "" H 10800 6150 50  0001 C CNN
F 3 "~" H 10800 6150 50  0001 C CNN
	1    10800 6150
	1    0    0    1   
$EndComp
$Comp
L power:+36V #PWR?
U 1 1 623108AC
P 10800 6150
AR Path="/61F382E3/623108AC" Ref="#PWR?"  Part="1" 
AR Path="/623108AC" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10800 6000 50  0001 C CNN
F 1 "+36V" H 10815 6323 50  0000 C CNN
F 2 "" H 10800 6150 50  0001 C CNN
F 3 "" H 10800 6150 50  0001 C CNN
	1    10800 6150
	1    0    0    -1  
$EndComp
$EndSCHEMATC
