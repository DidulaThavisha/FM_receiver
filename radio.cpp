#include <RDA5807.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 Oled =Adafruit_SSD1306(128,64,&Wire);

RDA5807 rx; 
char buffer[120];
char val;
const int buttonPin = 9; 
int up = 10;
int buttonState = 0;
int channel_up=0;
int vol; 
void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(up, INPUT);
  pinMode(A2, INPUT);
  Serial.begin(9600);
  Oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  while(!Serial);
  
  delay(500);
  
  if (!checkI2C())
  {
      Serial.println("\nCheck your circuit!");
      while(1);
  }

  rx.setup();
  delay(200);
  showReceiverInfo();
  rx.setVolume(1);  
  delay(500);
  //****
  Serial.print("\nEstacao 106.5MHz");
  rx.setFrequency(10650); // Please, change it to your local FM station
  
  sprintf(buffer,"\nCurrent Channel: %d, Real Frequency: %d, RSSI: %d\n", rx.getRealChannel(), rx.getRealFrequency(), rx.getRssi());
  Serial.print(buffer);
  delay(500);
  
  // Mute test
  /**
  Serial.print("\nAfter 4s device will mute during 3s");
  delay(4000);
  rx.setMute(true);
  delay(3000);
  rx.setMute(false);
  Serial.print("\nMute test has finished.");
  **/
  Serial.print("\nStation 9270Hz\n");
  rx.setFrequency(9270); // Please, change it to another local FM station
  
  delay(100);
  /**
  for (int i=8760;i<=10780;i+=10){
    Serial.print("\nStation Hz\n");
    Serial.print(i);
    rx.setFrequency(i);
    delay(1000);
  }**/
  while(true){    
    if (Serial.available()){
      val = Serial.read();
      Serial.println(val);
    }



    Oled.clearDisplay();
    Oled.setTextColor(WHITE);
    Oled.setTextSize(2);
    Oled.setCursor(0,0),
    Oled.print("Didula");
    Oled.display();


    
    vol=analogRead(A2);
    Serial.println(analogRead(A2));
    if (vol<128){rx.setVolume(0);}
    else if (vol<256){rx.setVolume(1);}
    else if (vol<384){rx.setVolume(2);}
    else if (vol<512){rx.setVolume(3);}
    else if (vol<640){rx.setVolume(4);}
    else if (vol<768){rx.setVolume(5);}
    else if (vol<896){rx.setVolume(6);}
    else if (vol<1024){rx.setVolume(7);}
    
    
    buttonState = digitalRead(buttonPin);
    channel_up = digitalRead(up);
    if (channel_up==1){
      rx.setFrequency(rx.getRealFrequency()+10);
      }
    //Serial.print(buttonState);
    if (buttonState==1 || val =='1'){
      delay(100);
      rx.seek(1,1);
      Serial.println(rx.getRealFrequency());
      Serial.print("\nCaught");
      val=0;
      buttonState=0;
      delay(1000);
      }
    Serial.println(rx.getRealFrequency());
    //Serial.print("\n");
    }

  Serial.print("Setting volume to 0");
  rx.setVolume(0); 
  showReceiverInfo();
  delay(3000);
  rx.setVolume(1);
  rx.setMute(true);
  rx.setSoftmute(true);
  showReceiverInfo();
  delay(2000);
  rx.setMute(false);
  delay(2000);
  Serial.print("Setting audio output impedance to high");
  rx.setAudioOutputHighImpedance(true);
  showReceiverInfo();
  
  Serial.print("\nResetting the system in 5s\n");
  delay(5000);
  //rx.setup();
  showReceiverInfo();
  rx.setFrequency(8760);
  rx.setVolume(1);
  // Seek test
  Serial.print("\nSeek stations");
  
  /**
  for (int i = 0; i < 10; i++ ) { 
    rx.seek(1,1);
    Serial.print("\nReal Frequency.: ");
    Serial.println(rx.getRealFrequency());
    showReceiverInfo();
    delay(5000);

  }
  **/
}

void showReceiverInfo() {
  sprintf(buffer,"\nID: %x, RSSI: %d, Band Space: %d, Volume: %d, Muted: %d, HighZ: %d, Soft Mute: %d \n", rx.getDeviceId(), rx.getRssi(), rx.getSpace(), rx.getVolume(), rx.isMuted(), rx.isAudioOutputHighImpedance(), rx.isSoftmuted() );
  Serial.print(buffer);
}

void loop() {
  
}

/**
 * Returns true if device found
 */
bool checkI2C() {
  Wire.begin();
  byte error, address;
  int nDevices;
  Serial.println("I2C bus Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("\nI2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("\nUnknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
    return false;
  }
  else {
    Serial.println("done\n");
    return true;
  }
}
