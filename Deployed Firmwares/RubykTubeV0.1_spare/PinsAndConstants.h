//Constants Definition of Pin Numbers and other constants

#define N_ROTARIES        3              //BITMASKs for PINs
#define ROT1_RIGHT_PIN    GPIO_NUM_26    // 0x2000000         
#define ROT1_LEFT_PIN     GPIO_NUM_25    // 0x4000000
#define ROT2_RIGHT_PIN    GPIO_NUM_13    // 0x3D0900
#define ROT2_LEFT_PIN     GPIO_NUM_10
#define ROT3_RIGHT_PIN    GPIO_NUM_9
#define ROT3_LEFT_PIN     GPIO_NUM_27    // 0x2000
#define GPIO_MASK ( 0x2000000 | 0x4000000 | 0x3D0900 | 0x2000 )
#define PIXEL_PIN         GPIO_NUM_5
#define WAKEUP_TOUCH_PIN  T0
#define VCC_LEVEL_PIN     A0

#define PIXEL_COUNT       6
#define colorSaturation   128

#define FIREBEETLE_ADC_LEVELS 4096
#define TOUCH_TRESHOLD 40

#define PERIOD_MILLIS 1000
#define IDDLE_SECONDS 60


#define BLINK_LENGTH 50

//Now define WIFI constants
const char* ssid = "etron room";
const char* pwd = "etron2018"; 
const IPAddress ip(192, 168, 1, 39);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);
const char* host = "192.168.1.10";  //192.168.1.143   
const int recv_port = 8889;
const int send_port = 8888;
