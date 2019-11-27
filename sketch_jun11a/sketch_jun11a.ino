#include <Wire.h>
#include <FaBo3Axis_ADXL345.h>

FaBo3Axis fabo3axis;

//first calibrate on powering up the module
//a poscal function to calculate avg values of x , y , z and print them on serial
//turn on the LED on pin13 and turn it off after the calibration is completed

//in loop continually read the data and calculate total acceleration vector
//print total number of steps on monitor

float threshhold = 6.8;

float xval[100] = {0};
float yval[100] = {0};
float zval[100] = {0};

float xavg, yavg, zavg;

int steps, flag = 0;

void setup()

{

  Serial.begin(9600);
  /* Initialise the sensor */
  fabo3axis.configuration();
  fabo3axis.powerOn();

  calibrate();
}



void loop()
{
  int x;
  int y;
  int z;
  
  fabo3axis.readXYZ(&x,&y,&z);

  int acc = 0;
  float totvect[100] = {0};
  float totave[100] = {0};
  //float sum1,sum2,sum3=0;
  float xaccl[100] = {0};
  float yaccl[100] = {0};
  float zaccl[100] = {0};
  // float x,y,z; 


  for (int a = 0; a < 100; a++)
  {
    xaccl[a] = int(x);
    delay(1);


    //delay(100);
    //x=sum1/100.0;

    //Serial.println(xavg);


    yaccl[a] = int(y);
    delay(1);

    //sum2=yaccl[i]+sum2;

    //y=sum2/100.0;

    //Serial.println(yavg);
    //delay(100);

    zaccl[a] = int(z);
    delay(1);

    //sum3=zaccl[i]+sum3;
    //z=sum3/100;

    // inital acc readings compared to calibrated avg readings
    totvect[a] = sqrt(((xaccl[a] - xavg) * (xaccl[a] - xavg)) + ((yaccl[a] - yavg) * (yaccl[a] - yavg)) + ((zval[a] - zavg) * (zval[a] - zavg)));

    //cal of acceleration vector: http://physics.stackexchange.com/questions/41653/how-do-i-get-the-total-acceleration-from-3-axes
    totave[a] = (totvect[a] + totvect[a - 1]) / 2 ;

    //acc=acc+totave[i];
    Serial.println(totave[a]);
    delay(100);

    //cal steps
    if (totave[a] > threshhold && flag == 0)
    {
      steps = steps + 1;
      flag = 1;

    }
    else if (totave[a] > threshhold && flag == 1)
    {
      // nothing happens
    }
    if (totave[a] < threshhold   && flag == 1)
    {
      flag = 0;
    }

    if (steps < 0) {
      steps = 0;
    }
    Serial.println('\n');
    Serial.print("steps: ");
    Serial.println(steps - 1);

    delay(1000);

  }


  //float tim=acc/100;
  //Serial.println(tim);
  delay(1000);
  // stepcal(totave);


}



void calibrate()

{
  int x;
  int y;
  int z;
  
  fabo3axis.readXYZ(&x,&y,&z);
  float sum = 0;
  float sum1 = 0;
  float sum2 = 0;

  for (int i = 0; i < 100; i++) {

    xval[i] = int(x);
    sum = xval[i] + sum;

  }

  delay(100);
  xavg = sum / 100.0;

  Serial.println(xavg);

  for (int j = 0; j < 100; j++)
  {
    yval[j] = int(y);

    sum1 = yval[j] + sum1;
  }
  yavg = sum1 / 100.0;

  Serial.println(yavg);
  delay(100);
  for (int q = 0; q < 100; q++)
  {
    zval[q] = int(z);

    sum2 = zval[q] + sum2;
  }
  zavg = sum2 / 100.0;
  delay(100);
  Serial.println(zavg);
}
