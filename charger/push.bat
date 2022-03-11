taskkill /F /IM putty.exe
arduino-cli compile --clean -b arduino:avr:nano:cpu=atmega328old && arduino-cli upload -b arduino:avr:nano:cpu=atmega328old -p COM8 cli-blink && putty /COM8 -serial -sercfg 9600,8,n,1,N &
