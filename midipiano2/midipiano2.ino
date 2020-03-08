
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();
byte input_pins[] = {
  30, 34, 22, 26, 32, 38, 24, 28 
};

byte inital_pins[] = {
  30, 34, 22, 26 
};
byte final_pins2[] = {
  32, 38, 24, 28  
};
//22, 24, 26, 28, 30, 32, 34, 38
//initial 30, 34, 22, 26
//final 32, 38, 24, 28 
byte output_pins[] = {
  53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23, 52, 50, 48, 44, 42, 40
};
/*GPIO_pin_t input_pins2[]= {
  DP2, DP3, DP4, DP5, DP6, DP7, DP8, DP9
};
GPIO_pin_t output_pins2[]= {
  DP30, DP31, DP32, DP33, DP34, DP35, DP36, DP37, DP38, DP39, DP40, DP41, DP42, DP43, DP44, DP45, DP46
};*/
int keys[] = {
  21, 22, 23, 24, 121, 122, 123, 124,//00 53
  25, 26, 27, 28, 125, 126, 127, 128,//01 51
  29, 30, 31, 32, 129, 130, 131, 132,//02 49
  33, 34, 35, 36, 133, 134, 135, 136,//03 47
  37, 38, 39, 40, 137, 138, 139, 140,//04 45
  41, 42, 43, 44, 141, 142, 143, 144,//05 43
  45, 46, 47, 48, 145, 146, 147, 148,//06 41
  49, 50, 51, 52, 149, 150, 151, 152,//07 39
  53, 54, 55, 56, 153, 154, 155, 156,//08 37
  57, 58, 59, 60, 157, 158, 159, 160,//09 35
  61, 62, 63, 64, 161, 162, 163, 164,//10 33
  65, 66, 67, 68, 165, 166, 167, 168,//11 31
  69, 70, 71, 72, 169, 170, 171, 172,//12 29
  73, 74, 75, 76, 173, 174, 175, 176,//13 27
  77, 78, 79, 80, 177, 178, 179, 180,//14 25
  81, 82, 83, 84, 181, 182, 183, 184,//15 23
  85, 86, 87, 88, 185, 186, 187, 188,//16 52
  89, 90, 91, 92, 189, 190, 191, 192,//17 50
  93, 94, 95, 96, 193, 194, 195, 196,//18 48
  97, 98, 99, 100, 197, 198, 199, 200,//19 44
  101, 102, 103, 104, 201, 202, 203, 204,//20 42
  105, 106, 107, 108, 205, 206, 207, 208,//21 40
};
boolean signals[sizeof(input_pins) * sizeof(output_pins)];
boolean signals_old[sizeof(input_pins) * sizeof(output_pins)];
unsigned long timer[108 * 1];
unsigned long timeout = 500;
byte k;

//////////////////////////////////////////////
void setup() {
  
  for (byte pin = 0; pin < sizeof(input_pins); pin++)  {
    pinMode(input_pins[pin],INPUT_PULLUP);
  }
  for (byte pin = 0; pin < sizeof(output_pins); pin++) {
    pinMode(output_pins[pin],OUTPUT);
    digitalWrite(output_pins[pin],LOW);
  }
  for (byte s=0;s<sizeof(signals);s++){
    signals[s]=0;
  }
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  //Serial.begin(115200);
}

//////////////////////////////////////////////
void sendMIDI(byte key,byte velocity/*,byte channel*/) {
  MIDI.sendNoteOn(key, velocity, 1);
}

void sendMIDI2(byte key,byte velocity){
  MIDI.sendNoteOff(key, 0, 1);
}
/////////////////////////////////////////////
void loop() {
  for (byte j=0;j<sizeof(signals);j++){
    signals_old[j]=signals[j]; //set signals_old to the last value of signals
  }
  k=0;
  //read the pin states
  for (byte o=0;o<sizeof(output_pins);o++){ //for each output pin
    digitalWrite(output_pins[o],HIGH); //write the output pin high
    for (byte i=0;i<sizeof(input_pins);i++){ //for each input pin   
      signals[k]=digitalRead(input_pins[i]); //save the value for each input_pullup
      //Serial.print(signals[k]);
      k++;
    }
    digitalWrite(output_pins[o],LOW);
  }
  
  //check if there is a change in state, and if there is, send the MIDI message 
  for (byte j=0;j<sizeof(signals);j++){
    if(signals[j]!=signals_old[j]){
      if(signals[j]==1){ //if signal at index j in array = 1 then the note has been changed to pressed so find midi key at index of j and send note on
        
        if(keys[j] < 109){
          timer[keys[j]] = millis();
          //Serial.print(keys[j]);
        }
        if(keys[j] > 109){
          // && timer[keys[j]- 100] < 100
          if(millis()- (timer[keys[j]- 100]) > 0 && (millis()- (timer[keys[j]- 100]) < 100)){
            sendMIDI(keys[j] - 100, map(millis()- (timer[keys[j]- 100]), 3, 20, 127, 5));
            //sendMIDI(keys[j] - 100,  millis() - (timer[keys[j]- 100]));
            timer[keys[j]- 100] = 0;
            
          }
          else (timer[keys[j]-100]) = 0;
        }
        //sendMIDI(keys[j],127);
      }
      else{
        //if signal at index j in array = 1 then the note has been changed to pressed so find midi key at index of j and send note on
        if(keys[j] < 109){
          sendMIDI2(keys[j],0);
        }
      }
    }
  }
  /*for (byte j=0;j<sizeof(timer);j++){
    if((timer[j] - millis()) > timeout){
      timer[j] = 0;
    }
  }*/
}
