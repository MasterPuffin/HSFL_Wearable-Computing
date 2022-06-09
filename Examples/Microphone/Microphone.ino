int sound_digital = 0;
int sound_analog = 4;
int maxsteps = 1000;
int steps = maxsteps;
int avgval = 0;
int addedval = 0;

void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(sound_digital, INPUT);  
}

void loop(){
  int val_digital = digitalRead(sound_digital);
  int val_analog = analogRead(sound_analog);

  // calibrate by taking first 1000 values for envoronment - average
  if(steps > 0){
    addedval = addedval + val_analog;
    steps = steps - 1;

    // calculate average after getting values
    if (steps == 0){
      avgval = int(addedval/maxsteps);
    }
    delay(10);
  }else{

    // check if value is louder than environment + correction value
    if (val_analog > int(avgval * 1.14))
    {
        Serial.println("Stufe 2");
        digitalWrite(LED_BUILTIN, LOW);
        delay(200);
    }
    if (val_analog > int(avgval * 1.12))
    {
        Serial.println("Stufe 1");
        digitalWrite(LED_BUILTIN, LOW);
        delay(200);
    }
    else if (val_analog > int(avgval * 1.1))
    {
        Serial.println("Stufe 0");
        digitalWrite(LED_BUILTIN, LOW);
        delay(200);
    }
    
    // else don't show something
    else
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    delay(10);
  }
}
