EESchema Schematic File Version 4
EELAYER 30 0
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
L RF_Module:ESP-12E U?
U 1 1 5FD3F954
P 5550 3350
F 0 "U?" H 5550 4331 50  0000 C CNN
F 1 "ESP-12E" H 5550 4240 50  0000 C CNN
F 2 "RF_Module:ESP-12E" H 5550 3350 50  0001 C CNN
F 3 "http://wiki.ai-thinker.com/_media/esp8266/esp8266_series_modules_user_manual_v1.1.pdf" H 5200 3450 50  0001 C CNN
	1    5550 3350
	1    0    0    -1  
$EndComp
$Comp
L Sensor_Temperature:LM35-LP U?
U 1 1 5FD415AF
P 6350 4400
F 0 "U?" H 6021 4446 50  0000 R CNN
F 1 "LM35-LP" H 6021 4355 50  0000 R CNN
F 2 "" H 6400 4150 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm35.pdf" H 6350 4400 50  0001 C CNN
	1    6350 4400
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5FD44B7B
P 5150 5150
F 0 "J?" V 5022 5230 50  0000 L CNN
F 1 "Screw_Terminal_01x02" V 5113 5230 50  0000 L CNN
F 2 "" H 5150 5150 50  0001 C CNN
F 3 "~" H 5150 5150 50  0001 C CNN
	1    5150 5150
	0    1    1    0   
$EndComp
$Comp
L Sensor_Motion:MPU-6050 U?
U 1 1 5FD4083C
P 3250 3250
F 0 "U?" H 3250 2461 50  0000 C CNN
F 1 "MPU-6050" H 3250 2370 50  0000 C CNN
F 2 "Sensor_Motion:InvenSense_QFN-24_4x4mm_P0.5mm" H 3250 2450 50  0001 C CNN
F 3 "https://store.invensense.com/datasheets/invensense/MPU-6050_DataSheet_V3%204.pdf" H 3250 3100 50  0001 C CNN
	1    3250 3250
	-1   0    0    1   
$EndComp
$Comp
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 5FD426BE
P 4300 4200
F 0 "U?" H 4300 4442 50  0000 C CNN
F 1 "AMS1117-3.3" H 4300 4351 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 4300 4400 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 4400 3950 50  0001 C CNN
	1    4300 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 4200 4600 2550
Wire Wire Line
	4600 2550 5550 2550
Wire Wire Line
	5550 4050 5550 4700
Wire Wire Line
	5550 4950 5150 4950
Wire Wire Line
	4300 4500 5150 4500
Wire Wire Line
	5150 4500 5150 4750
Connection ~ 5150 4950
Wire Wire Line
	5050 4950 4000 4950
Wire Wire Line
	4000 4950 4000 4650
Wire Wire Line
	3150 3950 3150 4200
Wire Wire Line
	3150 4200 4000 4200
Connection ~ 4000 4200
Wire Wire Line
	3250 2550 1950 2550
Wire Wire Line
	1950 2550 1950 4750
Wire Wire Line
	1950 4750 5150 4750
Connection ~ 5150 4750
Wire Wire Line
	5150 4750 5150 4950
Wire Wire Line
	4950 3750 3950 3750
Wire Wire Line
	3950 3750 3950 3550
Wire Wire Line
	3950 3450 4750 3450
Wire Wire Line
	4750 3450 4750 3650
Wire Wire Line
	4750 3650 4950 3650
Wire Wire Line
	6350 4700 5550 4700
Connection ~ 5550 4700
Wire Wire Line
	5550 4700 5550 4950
Wire Wire Line
	6350 4100 4950 4100
Wire Wire Line
	4950 4100 4950 4650
Wire Wire Line
	4950 4650 4000 4650
Connection ~ 4000 4650
Wire Wire Line
	4000 4650 4000 4200
Wire Wire Line
	6750 4400 6750 4000
Wire Wire Line
	6750 4000 4800 4000
Wire Wire Line
	4800 4000 4800 3150
Wire Wire Line
	4800 3150 4950 3150
$EndSCHEMATC
