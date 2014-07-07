EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:demeter-cache
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Demeter"
Date "02 jul 2014"
Rev "1.0-alpha"
Comp "Sebastián Zaffarano"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ATMEGA328-P IC1
U 1 1 53B4A8D6
P 4290 3650
F 0 "IC1" H 3540 4900 40  0000 L BNN
F 1 "ATMEGA328-P" H 4690 2250 40  0000 L BNN
F 2 "DIL28" H 4290 3650 30  0000 C CIN
F 3 "" H 4290 3650 60  0000 C CNN
	1    4290 3650
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 53B4ACE7
P 3280 2260
F 0 "C1" H 3280 2360 40  0000 L CNN
F 1 "C" H 3286 2175 40  0000 L CNN
F 2 "" H 3318 2110 30  0000 C CNN
F 3 "" H 3280 2260 60  0000 C CNN
	1    3280 2260
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 53B4AEBF
P 3280 2030
F 0 "#PWR01" H 3280 2030 30  0001 C CNN
F 1 "GND" H 3280 1960 30  0001 C CNN
F 2 "" H 3280 2030 60  0000 C CNN
F 3 "" H 3280 2030 60  0000 C CNN
	1    3280 2030
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR02
U 1 1 53B4AFAA
P 3350 4930
F 0 "#PWR02" H 3350 4930 30  0001 C CNN
F 1 "GND" H 3350 4860 30  0001 C CNN
F 2 "" H 3350 4930 60  0000 C CNN
F 3 "" H 3350 4930 60  0000 C CNN
	1    3350 4930
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P1
U 1 1 53B4B248
P 2890 3820
F 0 "P1" V 2840 3820 40  0000 C CNN
F 1 "POWER" V 2940 3820 40  0000 C CNN
F 2 "" H 2890 3820 60  0000 C CNN
F 3 "" H 2890 3820 60  0000 C CNN
	1    2890 3820
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 53B4B336
P 2460 3720
F 0 "#PWR03" H 2460 3720 30  0001 C CNN
F 1 "GND" H 2460 3650 30  0001 C CNN
F 2 "" H 2460 3720 60  0000 C CNN
F 3 "" H 2460 3720 60  0000 C CNN
	1    2460 3720
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR04
U 1 1 53B4B34D
P 2460 3920
F 0 "#PWR04" H 2460 4020 30  0001 C CNN
F 1 "VCC" H 2460 4020 30  0000 C CNN
F 2 "" H 2460 3920 60  0000 C CNN
F 3 "" H 2460 3920 60  0000 C CNN
	1    2460 3920
	0    -1   -1   0   
$EndComp
$Comp
L PWR_FLAG #FLG05
U 1 1 53B4B383
P 2540 3720
F 0 "#FLG05" H 2540 3815 30  0001 C CNN
F 1 "PWR_FLAG" H 2540 3900 30  0000 C CNN
F 2 "" H 2540 3720 60  0000 C CNN
F 3 "" H 2540 3720 60  0000 C CNN
	1    2540 3720
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG06
U 1 1 53B4B39A
P 2540 3920
F 0 "#FLG06" H 2540 4015 30  0001 C CNN
F 1 "PWR_FLAG" H 2540 4100 30  0000 C CNN
F 2 "" H 2540 3920 60  0000 C CNN
F 3 "" H 2540 3920 60  0000 C CNN
	1    2540 3920
	-1   0    0    1   
$EndComp
NoConn ~ 5290 2650
NoConn ~ 5290 3500
NoConn ~ 5290 3600
NoConn ~ 5290 3700
NoConn ~ 5290 4350
NoConn ~ 5290 4450
NoConn ~ 5290 4650
NoConn ~ 5290 4750
$Comp
L VCC #PWR07
U 1 1 53B4B5F9
P 3140 2550
F 0 "#PWR07" H 3140 2650 30  0001 C CNN
F 1 "VCC" H 3140 2650 30  0000 C CNN
F 2 "" H 3140 2550 60  0000 C CNN
F 3 "" H 3140 2550 60  0000 C CNN
	1    3140 2550
	0    -1   -1   0   
$EndComp
NoConn ~ 3390 3150
$Comp
L LED D1
U 1 1 53B4B6FF
P 5580 2550
F 0 "D1" H 5480 2490 50  0000 C CNN
F 1 "YELLOW" H 5570 2650 50  0000 C CNN
F 2 "" H 5580 2550 60  0000 C CNN
F 3 "" H 5580 2550 60  0000 C CNN
	1    5580 2550
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 53B4B74B
P 6100 2550
F 0 "R1" V 6180 2550 40  0000 C CNN
F 1 "330R" V 6107 2551 40  0000 C CNN
F 2 "" V 6030 2550 30  0000 C CNN
F 3 "" H 6100 2550 30  0000 C CNN
	1    6100 2550
	0    1    1    0   
$EndComp
$Comp
L GND #PWR08
U 1 1 53B4B7FA
P 6410 2550
F 0 "#PWR08" H 6410 2550 30  0001 C CNN
F 1 "GND" H 6410 2480 30  0001 C CNN
F 2 "" H 6410 2550 60  0000 C CNN
F 3 "" H 6410 2550 60  0000 C CNN
	1    6410 2550
	0    -1   -1   0   
$EndComp
$Comp
L SW_PUSH SW1
U 1 1 53B4BA8E
P 5540 5240
F 0 "SW1" H 5690 5350 50  0000 C CNN
F 1 "PUSH" H 5540 5160 50  0000 C CNN
F 2 "" H 5540 5240 60  0000 C CNN
F 3 "" H 5540 5240 60  0000 C CNN
	1    5540 5240
	0    1    1    0   
$EndComp
$Comp
L GND #PWR09
U 1 1 53B4BC15
P 5540 5580
F 0 "#PWR09" H 5540 5580 30  0001 C CNN
F 1 "GND" H 5540 5510 30  0001 C CNN
F 2 "" H 5540 5580 60  0000 C CNN
F 3 "" H 5540 5580 60  0000 C CNN
	1    5540 5580
	1    0    0    -1  
$EndComp
$Comp
L CONN_4 P3
U 1 1 53B4BD43
P 6590 2990
F 0 "P3" V 6540 2990 50  0000 C CNN
F 1 "RELAYS" V 6640 2990 50  0000 C CNN
F 2 "" H 6590 2990 60  0000 C CNN
F 3 "" H 6590 2990 60  0000 C CNN
	1    6590 2990
	-1   0    0    1   
$EndComp
Text Label 5520 3250 0    60   ~ 0
RELAY2
Text Label 5520 3150 0    60   ~ 0
RELAY1
$Comp
L GND #PWR010
U 1 1 53B4C0B5
P 6980 3140
F 0 "#PWR010" H 6980 3140 30  0001 C CNN
F 1 "GND" H 6980 3070 30  0001 C CNN
F 2 "" H 6980 3140 60  0000 C CNN
F 3 "" H 6980 3140 60  0000 C CNN
	1    6980 3140
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR011
U 1 1 53B4C19B
P 6980 2840
F 0 "#PWR011" H 6980 2940 30  0001 C CNN
F 1 "VCC" H 6980 2940 30  0000 C CNN
F 2 "" H 6980 2840 60  0000 C CNN
F 3 "" H 6980 2840 60  0000 C CNN
	1    6980 2840
	0    1    1    0   
$EndComp
Text Label 7130 3040 0    60   ~ 0
RELAY1
Text Label 7130 2940 0    60   ~ 0
RELAY2
Text Label 5420 4550 0    60   ~ 0
DHT22
$Comp
L CONN_3 K2
U 1 1 53B4C3F1
P 6590 5050
F 0 "K2" V 6540 5050 50  0000 C CNN
F 1 "DHT22" V 6640 5050 40  0000 C CNN
F 2 "" H 6590 5050 60  0000 C CNN
F 3 "" H 6590 5050 60  0000 C CNN
	1    6590 5050
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR012
U 1 1 53B4C6A1
P 7010 4950
F 0 "#PWR012" H 7010 4950 30  0001 C CNN
F 1 "GND" H 7010 4880 30  0001 C CNN
F 2 "" H 7010 4950 60  0000 C CNN
F 3 "" H 7010 4950 60  0000 C CNN
	1    7010 4950
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR013
U 1 1 53B4C6BD
P 7010 5150
F 0 "#PWR013" H 7010 5250 30  0001 C CNN
F 1 "VCC" H 7010 5250 30  0000 C CNN
F 2 "" H 7010 5150 60  0000 C CNN
F 3 "" H 7010 5150 60  0000 C CNN
	1    7010 5150
	0    1    1    0   
$EndComp
Text Label 7370 5050 0    60   ~ 0
DHT22
$Comp
L R R3
U 1 1 53B4C757
P 7250 5420
F 0 "R3" V 7330 5420 40  0000 C CNN
F 1 "10K" V 7257 5421 40  0000 C CNN
F 2 "" V 7180 5420 30  0000 C CNN
F 3 "" H 7250 5420 30  0000 C CNN
	1    7250 5420
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR014
U 1 1 53B4C8B9
P 7250 5730
F 0 "#PWR014" H 7250 5830 30  0001 C CNN
F 1 "VCC" H 7250 5830 30  0000 C CNN
F 2 "" H 7250 5730 60  0000 C CNN
F 3 "" H 7250 5730 60  0000 C CNN
	1    7250 5730
	-1   0    0    1   
$EndComp
Text Label 5520 3400 0    60   ~ 0
LDR
$Comp
L CONN_2 P4
U 1 1 53B4D113
P 6590 3410
F 0 "P4" V 6540 3410 40  0000 C CNN
F 1 "LDR" V 6640 3410 40  0000 C CNN
F 2 "" H 6590 3410 60  0000 C CNN
F 3 "" H 6590 3410 60  0000 C CNN
	1    6590 3410
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR015
U 1 1 53B4D248
P 6980 3310
F 0 "#PWR015" H 6980 3410 30  0001 C CNN
F 1 "VCC" H 6980 3410 30  0000 C CNN
F 2 "" H 6980 3310 60  0000 C CNN
F 3 "" H 6980 3310 60  0000 C CNN
	1    6980 3310
	0    1    1    0   
$EndComp
Text Label 7130 3510 0    60   ~ 0
LDR
$Comp
L R R2
U 1 1 53B4D572
P 7030 3800
F 0 "R2" V 7110 3800 40  0000 C CNN
F 1 "10K" V 7037 3801 40  0000 C CNN
F 2 "" V 6960 3800 30  0000 C CNN
F 3 "" H 7030 3800 30  0000 C CNN
	1    7030 3800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 53B4D6C4
P 7030 4090
F 0 "#PWR016" H 7030 4090 30  0001 C CNN
F 1 "GND" H 7030 4020 30  0001 C CNN
F 2 "" H 7030 4090 60  0000 C CNN
F 3 "" H 7030 4090 60  0000 C CNN
	1    7030 4090
	1    0    0    -1  
$EndComp
Text Label 5420 3800 0    60   ~ 0
SDA-RTC
Text Label 5420 3900 0    60   ~ 0
SCL-RTC
Text Label 5520 2750 0    60   ~ 0
SS-SD
Text Label 5520 3050 0    60   ~ 0
SCK-SD
Text Label 5520 2950 0    60   ~ 0
MISO-SD
Text Label 5520 2850 0    60   ~ 0
MOSI-SD
$Comp
L CONN_5X2 P2
U 1 1 53B4DC46
P 5030 1880
F 0 "P2" H 5030 2180 60  0000 C CNN
F 1 "SD_RTC" V 5030 1880 50  0000 C CNN
F 2 "" H 5030 1880 60  0000 C CNN
F 3 "" H 5030 1880 60  0000 C CNN
	1    5030 1880
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR017
U 1 1 53B4E100
P 5470 1680
F 0 "#PWR017" H 5470 1780 30  0001 C CNN
F 1 "VCC" H 5470 1780 30  0000 C CNN
F 2 "" H 5470 1680 60  0000 C CNN
F 3 "" H 5470 1680 60  0000 C CNN
	1    5470 1680
	0    1    1    0   
$EndComp
$Comp
L GND #PWR018
U 1 1 53B4E2BD
P 5590 2080
F 0 "#PWR018" H 5590 2080 30  0001 C CNN
F 1 "GND" H 5590 2010 30  0001 C CNN
F 2 "" H 5590 2080 60  0000 C CNN
F 3 "" H 5590 2080 60  0000 C CNN
	1    5590 2080
	0    -1   -1   0   
$EndComp
Text Label 5420 4150 0    60   ~ 0
TX-BT
Text Label 5420 4250 0    60   ~ 0
RX-BT
Text Label 4480 1680 2    60   ~ 0
MOSI-SD
Text Label 4480 1980 2    60   ~ 0
SCK-SD
Text Label 4480 2080 2    60   ~ 0
MISO-SD
$Comp
L CONN_3 K1
U 1 1 53B75D0B
P 3510 1880
F 0 "K1" V 3460 1880 50  0000 C CNN
F 1 "SW_IPS" V 3560 1880 40  0000 C CNN
F 2 "" H 3510 1880 60  0000 C CNN
F 3 "" H 3510 1880 60  0000 C CNN
	1    3510 1880
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR019
U 1 1 53B76291
P 3870 1780
F 0 "#PWR019" H 3870 1880 30  0001 C CNN
F 1 "VCC" H 3870 1880 30  0000 C CNN
F 2 "" H 3870 1780 60  0000 C CNN
F 3 "" H 3870 1780 60  0000 C CNN
	1    3870 1780
	0    1    1    0   
$EndComp
Text Label 5420 4000 0    60   ~ 0
RST
Text Label 5590 1780 0    60   ~ 0
SCL-RTC
Text Label 5590 1880 0    60   ~ 0
SDA-RTC
Text Label 4480 1780 2    60   ~ 0
SS-SD
Text Label 3850 1980 0    60   ~ 0
RST
Wire Wire Line
	3280 2060 3280 2030
Wire Wire Line
	3280 2850 3390 2850
Wire Wire Line
	3280 2460 3280 2850
Wire Wire Line
	3350 4750 3350 4930
Wire Wire Line
	3350 4750 3390 4750
Wire Wire Line
	3350 4850 3390 4850
Connection ~ 3350 4850
Wire Wire Line
	3140 2550 3390 2550
Connection ~ 3280 2550
Wire Wire Line
	2460 3920 2540 3920
Wire Wire Line
	2540 3720 2460 3720
Wire Wire Line
	5290 2550 5380 2550
Wire Wire Line
	5780 2550 5850 2550
Wire Wire Line
	6350 2550 6410 2550
Wire Wire Line
	5540 4940 5540 4850
Wire Wire Line
	5540 4850 5290 4850
Wire Wire Line
	5540 5540 5540 5580
Wire Wire Line
	5290 3150 5520 3150
Wire Wire Line
	5290 3250 5520 3250
Wire Wire Line
	6940 3140 6980 3140
Wire Wire Line
	6940 2840 6980 2840
Wire Wire Line
	6940 2940 7130 2940
Wire Wire Line
	6940 3040 7130 3040
Wire Wire Line
	5290 4550 5420 4550
Wire Wire Line
	6940 5150 7010 5150
Wire Wire Line
	6940 4950 7010 4950
Wire Wire Line
	6940 5050 7370 5050
Wire Wire Line
	7250 5170 7250 5050
Connection ~ 7250 5050
Wire Wire Line
	7250 5670 7250 5730
Wire Wire Line
	5290 4150 5420 4150
Wire Wire Line
	5290 4250 5420 4250
Wire Wire Line
	5290 3400 5520 3400
Wire Wire Line
	6980 3310 6940 3310
Wire Wire Line
	6940 3510 7130 3510
Wire Wire Line
	7030 4050 7030 4090
Wire Wire Line
	5290 3800 5420 3800
Wire Wire Line
	5290 3900 5420 3900
Wire Wire Line
	5290 2750 5520 2750
Wire Wire Line
	5290 2850 5520 2850
Wire Wire Line
	5290 2950 5520 2950
Wire Wire Line
	5290 3050 5520 3050
Wire Wire Line
	5430 1780 5590 1780
Wire Wire Line
	5430 1880 5590 1880
Wire Wire Line
	5430 2080 5590 2080
Wire Wire Line
	4630 1980 4480 1980
Wire Wire Line
	4630 2080 4480 2080
Wire Wire Line
	7030 3550 7030 3510
Connection ~ 7030 3510
Wire Wire Line
	4630 1680 4480 1680
Wire Wire Line
	5430 1980 5520 1980
Wire Wire Line
	3860 1880 4630 1880
Wire Wire Line
	5290 4000 5420 4000
Wire Wire Line
	5520 1980 5520 2080
Connection ~ 5520 2080
Wire Wire Line
	3870 1780 3860 1780
Wire Wire Line
	3860 1980 3850 1980
Wire Wire Line
	4630 1780 4480 1780
Wire Wire Line
	5430 1680 5470 1680
$Comp
L CONN_6 P5
U 1 1 53B77D64
P 6590 4510
F 0 "P5" V 6540 4510 60  0000 C CNN
F 1 "RS232" V 6640 4510 60  0000 C CNN
F 2 "" H 6590 4510 60  0000 C CNN
F 3 "" H 6590 4510 60  0000 C CNN
	1    6590 4510
	-1   0    0    -1  
$EndComp
NoConn ~ 6940 4260
NoConn ~ 6940 4660
Wire Wire Line
	6940 4360 7060 4360
Wire Wire Line
	6940 4460 7060 4460
Wire Wire Line
	6940 4560 7060 4560
Wire Wire Line
	6940 4760 7060 4760
Text Label 7060 4360 0    60   ~ 0
RX-BT
Text Label 7060 4460 0    60   ~ 0
TX-BT
$Comp
L VCC #PWR020
U 1 1 53B77F85
P 7060 4560
F 0 "#PWR020" H 7060 4660 30  0001 C CNN
F 1 "VCC" H 7060 4660 30  0000 C CNN
F 2 "" H 7060 4560 60  0000 C CNN
F 3 "" H 7060 4560 60  0000 C CNN
	1    7060 4560
	0    1    1    0   
$EndComp
$Comp
L GND #PWR021
U 1 1 53B77FA8
P 7060 4760
F 0 "#PWR021" H 7060 4760 30  0001 C CNN
F 1 "GND" H 7060 4690 30  0001 C CNN
F 2 "" H 7060 4760 60  0000 C CNN
F 3 "" H 7060 4760 60  0000 C CNN
	1    7060 4760
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
