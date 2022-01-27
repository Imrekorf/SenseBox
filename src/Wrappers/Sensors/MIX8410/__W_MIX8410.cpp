#include "__W_MIX8410.h"
#include <Arduino.h>
#include "../../../Logger/Logger.h"

#define ADCMax 4095.0

float __W_MIX8410::readO2Vout(){
    long sum = 0;
    for(int i=0; i<32; i++)
    {
        sum += analogRead(pinAdc);
    }
 
    sum >>= 5;
 
    float MeasuredVout = sum * (VRefer / ADCMax);
    return MeasuredVout;
}
 
float __W_MIX8410::readConcentration(){
    // Vout samples are with reference to 3.3V
    float MeasuredVout = readO2Vout();
 
    //float Concentration = FmultiMap(MeasuredVout, VoutArray,O2ConArray, 6);
    //when its output voltage is 2.0V,
    float Concentration = MeasuredVout * 0.21 / 2.0;
    float Concentration_Percentage=Concentration*100;
    return Concentration_Percentage;
}