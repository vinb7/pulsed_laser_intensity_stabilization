#include <vector>
#include <algorithm>
#include <array>



double control = 2046;
double p_gain = -10;
double i_gain = 0; // -0.000001;
double d_gain = 0;
int threshold_ = 10;
double intError, rateError;  
unsigned long elapsedTime;
//int error[1000] = {0}; //le.ngth of array sould be the max_setpoint duration of a pulse in microseconds
int control_DIGITALPIN = 10;
int calibration_DIGITALPIN = 12; // Usage has all been commented
int setpoint;
int lastError = 0;
unsigned long currentTime;
unsigned long previousTime = -100000;
int min_setpoint = 0;
int max_setpoint = 4095;
// std::vector<int> adc_readings;
int ARRAY_SIZE = 100;
// DO NOT GO BEYOND 20000 element
double DUTY_CYCLE = 10;                                              ; // in percentage
int element;
String sep_string = "\n ------------------------------------------------";

const byte numChars = 10;
char receivedChars[numChars];
boolean newData = false;
boolean on = true; 
boolean calibration = false; 

// serial communication
double ADC_avg = 0;
double DAC_beforePulse = 0;
double DAC_afterPulse = 0;
unsigned long Time_beforePulse = 0;
unsigned long Time_afterPulse = 0;
double Error_signal = 0;
double pulse_duration = 0;
int N_count = 0;
double correction = 0;
double i_correction = 0;
double p_correction = 0;
double d_correction = 0;

boolean above_flag = false;
boolean beTTL_has_light_flag = false;

int TTL_has_light = 0; // When TTL is HIGH and there is light, set this to 1;
                      // When TTL is TTL_has_light and there is light, set this to 0;

boolean serial_flag = true;
String serial_flag_on = "messenger_on";
String serial_flag_off = "messenger_off";

int sampling_window;

boolean stabilization_on = true;




void setup() {
  Serial.begin(115200);
  randomSeed(16);
  pinMode(control_DIGITALPIN, INPUT);
  pinMode(calibration_DIGITALPIN, INPUT);
  pinMode(A2, INPUT); // This pin monitors the output of DAC1
  ADC->ADC_MR |= 0x80; // these lines set free running mode 
  ADC->ADC_CR = 2;
  ADC->ADC_CHER = 0xA0; // set free running mode on pin A0 and A2. 0xA0 in binary is 10100000 
                        // (for A0 alone, use 0x80, which in binary is 10000000).
                        // pin A0 correspond to channel 7, CH6: A1, CH5: A2, CH4: A3 and so on 

  analogWriteResolution(12);
  analogWrite(DAC1, control);

  setpoint = 400;
  Serial.print(String(micros()));
}

String join(int arr[], int size){
  String output;
  int count = 0;
  for (int i=0; i<size; i++){
    output += String(arr[i]);
    output += "  ";
    count++;
    if (count == 10) {output += "\n"; count = 0;}
  }
  return output;
}


int read_adc() {
  while ((ADC->ADC_ISR & 0x80) == 0); // wait for conversion
  int a0 = ADC->ADC_CDR[7];    // read data
// Serial.println(a0);
 return a0;
}

int read_DAC1() { // DAC1 is connected to A2
  while ((ADC->ADC_ISR & 0x20) == 0); // wait for conversion
  int a2 = ADC->ADC_CDR[5];    // read data
 return a2;
}

void writeString(String stringData) { // Used to serially push out a String with Serial.write()

  for (int i = 0; i < stringData.length(); i++)
  {
    Serial.write(stringData[i]);   
  }
}

int bool_to_int(boolean state){
  if (state == true){
    return 1;
  }
  else{
    return 0;
  }
}
void rec_serial_data() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void changeValues(int array[], int array2[]) {
    if (newData == true) {
        if (receivedChars[0] == 'P' && receivedChars[1] == '=') {
           String temp1 = String(receivedChars);
           String temp2 = temp1.substring(2);
           p_gain = temp2.toDouble();
        }

        else if (receivedChars[0] == 'I' && receivedChars[1] == '=') {
           String temp1 = String(receivedChars);
           String temp2 = temp1.substring(2);
           i_gain = temp2.toDouble();

        }

        else if (receivedChars[0] == 'D' && receivedChars[1] == '=') {
           Serial.println(rateError);
           String temp1 = String(receivedChars);
           String temp2 = temp1.substring(2);
           d_gain = temp2.toDouble();
        }

        else if (receivedChars[0] == 'S' && receivedChars[1] == 'T') {
           stabilization_on = true;
        }
        else if (receivedChars[0] == 'N' && receivedChars[1] == 'S' && receivedChars[2] == 'T') {
           stabilization_on = false;
        }

        else if (receivedChars[0] == 'W' && receivedChars[1] == '?') {
           String temp1 = String(sampling_window);
           String toWrite = "sampling_window: " + temp1;
           writeString(toWrite);
        }

        else if (receivedChars[0] == 'W' && receivedChars[1] == '=') {
          String temp1 = String(receivedChars);
          String temp2 = temp1.substring(2);
          ARRAY_SIZE = temp2.toInt();
        }

        else if (receivedChars[0] == 'D' && receivedChars[1] == 'C' && receivedChars[2] == '=') {
          String temp1 = String(receivedChars);
          String temp2 = temp1.substring(3);
          DUTY_CYCLE = temp2.toDouble();
        }

        else if (receivedChars[0] == 'T' && receivedChars[1] == '=') {
           String temp1 = String(receivedChars);
           String temp2 = temp1.substring(2);
           control = temp2.toDouble();
        }
        
        else if (receivedChars[0] == 'P' && receivedChars[1] == '?') {
          String p_temp = String(p_gain);
          String p_out = p_temp + "\n";
          writeString(p_out);
        }

        else if (receivedChars[0] == 'I' && receivedChars[1] == '?') {
          String i_temp = String(i_gain);
          String i_out = i_temp + "\n";
          writeString(i_out);
        }

        else if (receivedChars[0] == 'D' && receivedChars[1] == '?') {
          String d_temp = String(d_gain);
          String d_out = d_temp + "\n";
          writeString(d_out);
        }

        else if (receivedChars[0] == 'N' && receivedChars[1] == '?') {
          String N_count_temp = String(N_count);
          String N_out = N_count_temp + "\n";
          writeString(N_out);
        }

         else if (receivedChars[0] == 'P' && receivedChars[1] == 'D'){
           String pulse_duration_temp = String(pulse_duration) + "\n";
           writeString(pulse_duration_temp);
        }
        

        else if (receivedChars[0] == 'B') {
          on = true;
        }        

        else if (receivedChars[0] == 'E') {
          on = false;
        }

        else if (receivedChars[0] == 'S'){ //receive new setpoint 
           String temp1 = String(receivedChars);
           String temp2 = temp1.substring(1);
           setpoint = temp2.toInt();     
        }

        else if (receivedChars[0] == 'C') {
           calibration = true;
//           String _min = String(min_setpoint);
//           String _max = String(max_setpoint);
//           String min_max = _min + "," + _max + "\n" + "setpoint: " + String(setpoint) + "\n";
//           writeString(min_max);
        }    

        // This gives you the ADC channel's reading, remember there are 4096 points for [0, 3.3]
        else if (receivedChars[0] == 'A' && receivedChars[1] == 'D' && receivedChars[2] == 'C'){
           String adc_avg = String("Max: ") + String(ADC_avg) + "\n";
           String a0_value = String("A0 Reading: ") + String(float(read_adc())) + "\n";
           writeString(a0_value + adc_avg);
        }

        // This gives you the DAC channel's reading, remember there are 4096 points for [0.5, 2.7]
        else if (receivedChars[0] == 'D' && receivedChars[1] == 'A' && receivedChars[2] == 'C'){
           String a2_value = String("A2 Reading: ") + String((float(read_DAC1())/4096.0*3.3 - 0.5)*4096/2.2) + "\n";
           String control_value = String("Control Value: ") + String(float(control)) + "\n";
           String toWrite = a2_value + control_value;
           writeString(toWrite);
        }
        
        else if (receivedChars[0] == 'D'){ // set the DAC1 output (int from 550 to 2075) 
           String temp1 = String(receivedChars);
           String temp2 = temp1.substring(1);
           int dac = temp2.toInt();
           analogWrite(DAC1, dac);
        }
        
        else if (receivedChars[0] == 'R') { //return new setpoint
           String s_temp = String(setpoint);
           String s_out = s_temp + "\n"; 
           writeString(s_out);
        }

        else if (receivedChars[0] == 'M'){ //return min_setpoint and max_setpoint
           String _min = String(min_setpoint);
           String _max = String(max_setpoint);
           String min_max = _min + "," + _max + "\n";
           writeString(min_max);
        }

/*
        else if (receivedChars[0] = '?') {
          int condition = bool_to_int(on);
          String state = String(condition);
          writeString(state);
//          if (on == true){
//            String _true = "Stop";
//            String state = _true + "\n";
//            writeString(state);
//          }
//          else {
//            String _false = "Start";
//            String state = _false + "\n";
//            writeString(state);
//          }
        }
*/

        else if (String(receivedChars).equals(serial_flag_on)) {
          serial_flag = true;
        } 
        
        else if (String(receivedChars).equals(serial_flag_off)) {
          serial_flag = false;
        }   



        else if (receivedChars[0] == '?') {
          String s_temp = String(setpoint);
          String setpoint_out = "setpoint: " + s_temp;
          
          String adc_avg = String("adc_avg: ") + String(ADC_avg);
          String a0_value = String("current A0 Reading: ") + String(float(read_adc()));
          String adc_out = adc_avg + "   " + a0_value;

          String a2_value = String("current A2 Reading: ") + String((float(read_DAC1())/4096.0*3.3 - 0.5)*4096/2.2);
          String control_value = String("Control Value: ") + String(float(control));
          String dac_out = a2_value + "   " + control_value;

          String errSig = String("Error Signal: ") + String(Error_signal);
          String corr = String("Correction: ") + String(correction);
          String intErr = String("Int Error: ") + String(intError);
          String ratErr = String("Rate Error: ") + String(rateError);
          String p_cor = String("p_correction: ") + String(p_correction);
          String i_cor = String("i_correction: ") + String(i_correction);
          String d_cor = String("d_correction: ") + String(d_correction);

          String toWrite = sep_string + setpoint_out + "\n" + adc_out + "\n" + dac_out + 
          "\n" + corr + "   " + errSig + "   " + intErr + "   " + ratErr + "\n" + p_cor + "\n"
          + i_cor + "\n" + d_cor + "\n";

          writeString(toWrite);
          }
          
          else if (receivedChars[0] == '.' && receivedChars[1] == '?') {
            String adc_readings_string = sep_string +"\n adc_readings: \n" + join(array, ARRAY_SIZE);
            String adc_readings_max_string = sep_string + "\n adc_readings_max: \n" + join(array2, element); 
            String toWrite = adc_readings_string + adc_readings_max_string;
            writeString(toWrite);
          }
          
          // else if (receivedChars[0]
          
        newData = false;
    }
}


void calibrator() {

  if (calibration == true){
    // potential issues:
    // 1. the ADC input already gets out of bound
    // 2. the input signal is being pulsed; 
    // 3. be careful that if the pulse is too fast the calibrator may not have enough time to change the DAC and sample data from ADC
    boolean step1 = true;
    boolean step2 = true;
  
    analogWrite(DAC1, 0);
    delay(5);
    
    while (step1) {
      if (digitalRead(control_DIGITALPIN) == TTL_has_light) {
        min_setpoint = read_adc();
        if (digitalRead(control_DIGITALPIN) == TTL_has_light) {step1 = false;}  
      }
    }
  
    analogWrite(DAC1, 4095);
    delay(5);
  
    while (step2) {
      if (digitalRead(control_DIGITALPIN) == TTL_has_light) {
        max_setpoint = read_adc();
        if (digitalRead(control_DIGITALPIN) == TTL_has_light) {step2 = false;}  
      }
    }
    
    setpoint = (max_setpoint + min_setpoint)/2;
    calibration = false;
    Serial.println(String(setpoint));
  }
}



void stabilizer(int arr[], int arr2[]) {
    int n = 0;
    unsigned long t_start = micros();
    // unsigned long prisoner1, prisoner2, prisoner3;
    int prisoner;
    
    while(n < ARRAY_SIZE){
      arr[n] = read_adc();
      n++;
      // prisoner = random(4, 20);
      // delayMicroseconds(prisoner);
    }

    currentTime = micros();
    sampling_window = currentTime - t_start;
    elapsedTime = (double)(currentTime - previousTime);
    
    int temp_arr[ARRAY_SIZE];
    std::copy(arr, arr+ARRAY_SIZE, temp_arr);
    std::sort(temp_arr, temp_arr + ARRAY_SIZE, std::greater<int>());
    std::copy(temp_arr, temp_arr + element, arr2);

    for (int k=0; k<element; k++){ADC_avg += arr2[k];}
    ADC_avg = ADC_avg / element;
    if (ADC_avg < 0.005 * setpoint) {return;}  // Test if the stuck situation happes
    correction = (ADC_avg - setpoint) / ADC_avg;
// -------------------------------------
    intError += correction * elapsedTime / 1000000; // Be careful with our micros()
    rateError = (correction - lastError)/elapsedTime * 1000000;

    i_correction = i_gain*intError;
    p_correction = p_gain*correction;
    d_correction = d_gain*rateError;
    
    double sig = p_correction + i_correction + d_correction;
    //
    Error_signal = sig;
    control = control + sig;
    lastError = correction;
    previousTime = currentTime; 
    
    if (control >= 4095) 
      { control = 4095;
        }
    if (control <= 0) 
      { control = 0; 
        }

    if (stabilization_on == true) {analogWrite(DAC1, int(round(control)));}
    // analogWrite(DAC1, int(round(control)));
//    String sep_string = "-------------------------------------";
//    Serial.println(sep_string);
//    String adc_readings_string = "adcs: " + join(adc_readings);
//    Serial.println(adc_readings_string);

    
}

  

void loop(){
  calibrator();
  on = true; // Should comment this line out
  if (on == true){
    element = int(ARRAY_SIZE * DUTY_CYCLE / 100);
    int adc_readings[ARRAY_SIZE];
    int adc_readings_max[element];
    stabilizer(adc_readings, adc_readings_max); 
    rec_serial_data();
    changeValues(adc_readings, adc_readings_max);
  }
  // analogWrite(DAC1, 4095);
  // delay(5000);
  // analogWrite(DAC1, 0);
  // delay(5000);
}
