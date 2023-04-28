#include <TinyGPS++.h>
#include <math.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

//Arah
#define LEFT 1
#define RIGHT -1

#define PIN_RUDDER  12
#define PIN_ESC     11

#define max_kiri        125  //sudut
#define max_kanan       50   //sudut
#define setpoint_jarak  0.5  //meter - jarak toleransi

TinyGPSPlus gps;
Servo esc;
Servo rudder;
HMC5883L compass;
LiquidCrystal_I2C lcd(0x27, 20, 4);

int speed_esc = 1100;
bool gps_valid = false;
int index_way = 0;
int max_way = 5;
double waypoint[5][2] = {
//  { -7.278868 , 112.793495}, //ppns1
//  { -7.278856 , 112.793663},
//  { -7.278990 , 112.793701},
//  { -7.278861 , 112.793800},
//  { -7.278740 , 112.793609}
    {-7.278899, 112.793570}, //ppns2
    {-7.278982, 112.793590},
    {-7.279010, 112.793750},
    {-7.278818, 112.793790},
    {-7.278737, 112.793550}
};
double pos_lat = 0.0;
double pos_lng = 0.0;
double degree = 0.0;


void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);

  rudder.attach(PIN_RUDDER);
  esc.attach(PIN_ESC);
  Serial.println("Init ESC");
  esc.writeMicroseconds(1000); //aktivasi esc

  lcd.begin(20,4);
  lcd.clear();

  // Initialize Initialize HMC5883L
  Serial.println("Initialize HMC5883L");
  while (!compass.begin()) {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }
  compass.setRange(HMC5883L_RANGE_1_3GA);
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
  compass.setDataRate(HMC5883L_DATARATE_30HZ);
  compass.setSamples(HMC5883L_SAMPLES_8);
  compass.setOffset(0, 0);
}

void loop() {

  //menunggu sensor gps aktif
  if ( !gps.location.isValid() ) {
    Serial.println("Waiting GPS Valid...");
    delay(500);
    return;
  }

  //pembacaan sensor gps
  pos_lat = gps.location.lat();
  pos_lng = gps.location.lng();
  degree = baca_kompas();

  //kapal berhenti ketika kapal sudah mencapai titik terakhir
  if (index_way == max_way) {
    esc.writeMicroseconds(1000);
    Serial.println("Sudah mencapai setpoint terakhir !!");
    sedot();
    delay(1000);
    return;
  }

  //pembacaan koordinat target,index way merupakan point berapa yang dituju
  double target_lat = waypoint[index_way][0];
  double target_lng = waypoint[index_way][1];
//  double target_jarak = hitung_jarak(pos_lat, pos_lng, target_lat, target_lng);
//  double target_sudut = hitung_sudut(pos_lat, pos_lng, target_lat, target_lng);

  double target_jarak =
    TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      target_lat,
      target_lng);

  double target_sudut =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      target_lat,
      target_lng);
      
  //ditambahkan 360-target karena gak nemu2 pointnya
  target_sudut = 360-target_sudut;

  //program posisi rudder berdasarkan target sudutnya
  double diff_degree = selisih_sudut((int)target_sudut, (int)degree);
  int arah_belok = arah_sudut((int)target_sudut, (int)degree);
  int rdata = 90;
  if (arah_belok == LEFT) {
    rdata = 90 + diff_degree;
    if (rdata > max_kiri) rdata = max_kiri;
  } else if (arah_belok == RIGHT) {
    rdata = 90 - diff_degree;
    if (rdata < max_kanan) rdata = max_kanan;
  }

  //program menjalankan motor dan ruddernya
  esc.writeMicroseconds(speed_esc);
  rudder.write(rdata);
  Serial.println("Kapal: Lat=" + String(pos_lat, 6) + " Lng=" + String(pos_lng, 6) + " Target: Lat=" + String(target_lat, 6) + " Lng=" + String(target_lng, 6) + " Jarak=" + String(target_jarak, 1) + "m SudutTarget=" + String(target_sudut) +" BedaSudut=" + String(diff_degree) + " Arah=" + ((arah_belok == LEFT) ? "KIRI" : "KANAN") );

  //program perpindahan waypoint 
  if (index_way == 0 && target_jarak <= setpoint_jarak) {
    Serial.println("Selesai !! Lanjut ke waypoint berikutnya");
    index_way=1;
  }
  if (index_way == 1 && target_jarak <= setpoint_jarak) {
    Serial.println("Selesai !! Lanjut ke waypoint berikutnya");
    index_way=2;
  }
  if (index_way == 2 && target_jarak <= setpoint_jarak) {
    Serial.println("Selesai !! Lanjut ke waypoint berikutnya");
    index_way=3;
  }
  if (index_way == 3 && target_jarak <= setpoint_jarak) {
    Serial.println("Selesai !! Lanjut ke waypoint berikutnya");
    index_way=4;
  }

  //program display lcd
  lcd.setCursor(0, 0); lcd.print("Posisi");
  lcd.setCursor(0, 1); lcd.print(String(degree, 1));
  lcd.setCursor(0, 2); lcd.print(String(target_sudut, 1));
  lcd.setCursor(0, 3); lcd.print(String(target_jarak, 1));

  lcd.setCursor(10, 0); lcd.print("Target");
  lcd.setCursor(10, 1); lcd.print(String(target_lat, 6));
  lcd.setCursor(10, 2); lcd.print(String(target_lng, 6));
  lcd.setCursor(10, 3); lcd.print(((arah_belok == LEFT) ? "KIRII" : "KANAN"));
  
  delay(200);
}

void serialEvent3() {
  while (Serial3.available()) {
    gps.encode(Serial3.read());
  }
}

double baca_kompas() {
  Vector norm = compass.readNormalize();

  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);

  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (0.0 + (48.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }

  // Convert to degrees
  float headingDegrees = heading * 180 / M_PI;

  // Output
  //  Serial.print(" Heading = ");
  //  Serial.print(heading);
  //  Serial.print(" Degress = ");
  //  Serial.print(headingDegrees);
  //  Serial.println();

  return headingDegrees;
}

void sedot() {

}

bool is_minus(double val) {
  return (val < 0) ? true : false;
}
