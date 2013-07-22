Test Suite
==========

This is a self contained test of all the functions of the VM167 card

Required Hardware
-----------------

Setup a circuit according to these specifications

      Digital
     1 --R-- 5
     2 --R-- 6
     3 --R-- 7
     4 --R-- 8
     
       Analog/PWM
     A1 ---R--- GND
     A2 ---R--- +5V
    +5V -R-+-R- GND
           A3
     
     A4 --R+R+-- PWM1
           C-C
     GND---+-+	
     
     A5 --R+R+-- PWM2
           C-C
     GND---+-+

Resistor values are not crucial as long as they are not extremely large or small. What matters is to be consistent.

