//Constants Definition of Pin Numbers and other constants

#define ROT1_RIGHT_PIN    GPIO_NUM_25
#define ROT1_LEFT_PIN     GPIO_NUM_26
#define ROT2_RIGHT_PIN    GPIO_NUM_27
#define ROT2_LEFT_PIN     GPIO_NUM_9
#define ROT3_RIGHT_PIN    GPIO_NUM_10
#define ROT3_LEFT_PIN     GPIO_NUM_13
#define PIXEL_PIN         GPIO_NUM_5
#define WAKEUP_TOUCH_PIN  T0
#define VCC_LEVEL_PIN     GPIO_NUM_15

#define PIXEL_COUNT       6
#define colorSaturation   128


#define FIREBEETLE_ADC_LEVELS 4096
#define TOUCH_TRESHOLD 40

#define PERIOD_MILLIS 1000
#define IDDLE_SECONDS 60


//Now define WIFI constants 
const char* ssid = "NaisDelClot";
const char* pwd = "xmp13051985";
const IPAddress ip(192, 168, 1, 201);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);
const char* host = "192.168.1.131";
const int recv_port = 10000;
const int send_port = 12000;
