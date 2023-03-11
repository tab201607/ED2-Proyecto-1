#include <AdafruitIO.h>
#include <AdafruitIO_Dashboard.h>
#include <AdafruitIO_Data.h>
#include <AdafruitIO_Definitions.h>
#include <AdafruitIO_Feed.h>
#include <AdafruitIO_Group.h>
#include <AdafruitIO_MQTT.h>
#include <AdafruitIO_Time.h>
#include <AdafruitIO_WiFi.h>

/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME "tab201607"
#define IO_KEY "aio_zVpR00YfuRYKcMjTc56AXqEn2qfZ"

/******************************* WIFI **************************************/

//#define WIFI_SSID "Jacob iPhone"
//#define WIFI_PASS "244466666"
#define WIFI_SSID "The Promised LAN"
#define WIFI_PASS "1a2b3c4d5e6X"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
