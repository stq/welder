# About
Software for arduino controller for 3.3-15v supercaps impulse welder.
Output current up to 2000A (but probably about 1500A) due to connectors.

#Basic device info
Primary use: impulse welder for think sheet metals
Secondary use: portable power source (for example for powerful lamp or car starter)
Tertiary use: solar battery buffer (increases life of lead solar battery)
Forth use: low power solar energy harvester (with built in solar batter)

Max power: about 30kJ in 4 seconds
Example power: 100Wt 12v lamp gives light for a few minutes

#Hardware (draft)
    6 LSUC capacitors 3000f * 2.7v
    6 on-capacitor overcharge protectors (2A dischargers)
    Arduino nano
    Bare 16x2 lcd display
    2 encoders (one with button)
    PSU 15v 3a
    PSU 5v 1a
    Current Limiter module ( example https://www.aliexpress.com/item/32902533475.html LTC3780EG LTC3780 DC-DC 5-32V to 1V-30V 10A Automatic Step Up Down Regulator Charging Module Power supply module Boost)
    Arduino current sensor module
    Arduino voltage sensor module
    Automatic impulse sensor - usev oltage divider with 4.3 zener diode for protection 

Hint:
If dont have current limiter, use 1-2 powerful resistors (with optional mosfets or relays to turn then off when charge current is becoming low enough).
You need to limit current because when capacitors are discharged - they will act as short circuit.
Another option is to use old linear PSU (with heavy transformer, rectifier bridge and few caps) -
they work way better with shortcircuits, just make sure transformer is strong enough.
Also, if you plan to use stronger chargers (up to 200A), it's better to use 500W+ resistors
like 0.5-1 Ohm (use Ohm law to calculate required resistance and power), it'll be cheaper then finding proper current limiter.

 

#Setup

run setup.bat

#Build

run build.bat

#Other

Detect COM port by `arduino-cli board list`

#Issues/Todos
1. Add support for case when charger is not connected
