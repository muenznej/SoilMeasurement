EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
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
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L -BATT #PWR?
U 1 1 5A42E706
P 2850 2700
F 0 "#PWR?" H 2850 2550 50  0001 C CNN
F 1 "-BATT" H 2850 2840 50  0000 C CNN
F 2 "" H 2850 2700 50  0001 C CNN
F 3 "" H 2850 2700 50  0001 C CNN
	1    2850 2700
	1    0    0    -1  
$EndComp
$Comp
L Battery_Cell BT?
U 1 1 5A42E795
P 3500 2850
F 0 "BT?" H 3600 2950 50  0000 L CNN
F 1 "Battery_Cell" H 3600 2850 50  0000 L CNN
F 2 "" V 3500 2910 50  0001 C CNN
F 3 "" V 3500 2910 50  0001 C CNN
	1    3500 2850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5A42E7C8
P 2750 3500
F 0 "#PWR?" H 2750 3250 50  0001 C CNN
F 1 "GND" H 2750 3350 50  0000 C CNN
F 2 "" H 2750 3500 50  0001 C CNN
F 3 "" H 2750 3500 50  0001 C CNN
	1    2750 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 2950 3500 3500
Wire Wire Line
	3500 3500 2750 3500
Connection ~ 3500 3500
Text Label 3200 3500 0    60   ~ 0
JHH
$Sheet
S 5450 3100 850  1200
U 5A42E803
F0 "Sheet5A42E802" 60
F1 "file5A42E802.sch" 60
F2 "JH" I L 5450 3250 60 
F3 "JK" B L 5450 3700 60 
$EndSheet
$EndSCHEMATC
