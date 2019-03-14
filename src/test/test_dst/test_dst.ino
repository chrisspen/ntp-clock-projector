#include <ESP8266WiFi.h>
#include "pass.h" /* ssid=; pass=; */
#include "time.h"

time_t this_second = 0;
time_t last_second = 0;
int dst=3600;

void setup() {
     //Serial.begin(19200);
     Serial.begin(115200);
     WiFi.mode(WIFI_STA);
     WiFi.begin(SSID, SSIDPWD);
     while (WiFi.status() != WL_CONNECTED)
     {
         Serial.print(".");
         delay(500);
     }
     /* let us set: timezone in sec, daylightOffset in sec, server_name1, server_name2, server_name3 */
     configTime(7200, 3600, "time.nist.gov", "time.windows.com", "de.pool.ntp.org");
     // wait until network has actual time
     while(!this_second)
     {
         time(&this_second);
         Serial.print("-");
         delay(100);
     }
     Serial.println();
}

void loop() {
     time(&this_second); // until time is set, it remains 0
     if (this_second != last_second)
     {
         last_second = this_second;
         if ((this_second % 60) == 0)
         {
             Serial.print(ctime(&this_second));
         }
     }
     delay(500);
}
