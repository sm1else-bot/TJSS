//Tested OK 12/08/2020 Impotant see below:
//This program written for use with Atmega328P for smooth control
//of robot.
//This robot working fine with manual configuration of wifi credentials
//Connect to wifi in SmartConfig mode tried & working fine
//For this "EspTouch: SmartConfig for ESP32" Android app
//by "Espressif" to be used to connect the ESP32 
//to WiFi network of your choice.
//MAC Address of this ESP32 is: 24:6F:28:B0:D7:CC 
//Camera tilted for straitening the view & found OK
//Storing the Wifi Credentials is to be tried

/*This program works for the following
 * Mouse over: Forward, Reverse, Left, Right & Stop
 * Also Mouse click on: Forward, Reverse & Stop
 * Mouse Down: Stop
 * Mouse Up: Stop
 */

#include "esp_camera.h"
#include <WiFi.h>

//Select camera model
#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

extern int ForwardControl =  2; // Left Motor(Orange)
extern int ReverseControl =  4; //(Gray) 
extern int LeftControl = 14; // Right Motor(Brown) 
extern int RightControl = 13; //( Green)

extern String Camerafeed ="";

void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  pinMode(ForwardControl, OUTPUT); 
  pinMode(ReverseControl, OUTPUT); 
  pinMode(LeftControl, OUTPUT); 
  pinMode(RightControl, OUTPUT); 
 
  //initialize
  digitalWrite(ForwardControl, LOW);
  digitalWrite(ReverseControl, LOW);
  digitalWrite(LeftControl, LOW);
  digitalWrite(RightControl, LOW);
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
  Serial.printf("Camera init failed with error 0x%x", err);
  return;
  }

  //drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);

  //Enter here for smart config
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  startCameraServer();
  
  Serial.println("");
  Serial.print("Camera Ready! Use 'http://");
  Serial.println(WiFi.localIP());
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  WiFi.printDiag(Serial);//This statement shows the WiFi Credentials.
  //Serial.print("   ");
  //Serial.println(WiFi.PASS:);
  Serial.print("ESP32 Board's MAC Address: ");
  Serial.println(WiFi.macAddress());

  Camerafeed = WiFi.localIP().toString();
}

void loop() {
  // put your main code here, to run repeatedly:
}
