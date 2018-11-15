#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

String ipadd="";
String pos="";
String lon="";
String lat="";
String rise="";
String sunset="";
String tnow="";

String ip(){        //For IP Address
  HTTPClient http;
  ipadd="0";
  String h = "http://api.ipify.org";
  http.begin(h);
  int response = http.GET();
  if(response>0)
  {
    ipadd = http.getString();
    Serial.println(ipadd);
  }
  return ipadd;
}

String coordinates(String a)     //get GeoLocation(Latitude, longitude)
{
  HTTPClient http;
  DynamicJsonBuffer jsonBuffer;
  String ans ="0";
  String result="";
  result = "http://ipinfo.io/"+a+"/geo";
  http.begin(result);
  http.addHeader("Content-Type", "text/json");
  int response = http.GET();
  if(response > 0)
  {
     ans = http.getString();
     JsonObject& root = jsonBuffer.parseObject(ans);
     if(root.success())
     {
       const char*  pos;
       pos=root["loc"];
       ans = pos;
     }
  }
   http.end();
  return ans;
}
String timings(String a , String c)             //Get Sunrise and sunset time
{
   HTTPClient http;
   DynamicJsonBuffer jsonBuffer;
   String ans="0";
   String b = "";
   String times = "http://api.sunrise-sunset.org/json?lat="+a+"&lng="+c; 
   http.begin(times);
   http.addHeader("Content-Type","text/json");
   int response = http.GET();
   if(response >0)
   {
      times = http.getString();
      JsonObject& root = jsonBuffer.parseObject(times);
      if(root.success())
      {
          const char* sun;
          sun = root["results"]["sunrise"];
          ans = sun;
          sun = root["results"]["sunset"];
          b = sun;
          ans = ans+" "+b;
      }
   }
   //Serial.println(ans);
   return ans;
}
String gettime()//Get current time
{
  HTTPClient http;
  DynamicJsonBuffer jsonBuffer;
  String ans="0";
  String gtime = "http://worldclockapi.com/api/json/utc/now";
  http.begin(gtime);
  http.addHeader("Content-Type","application/json");
  int response = http.GET();
  if(response > 0)
  {
        String r = http.getString();
        JsonObject& root = jsonBuffer.parseObject(r);
        if(root.success())
        {
          const char* data;
          data = root["currentDateTime"];
          r= data;
        }
        int splitT = r.indexOf("T");
        ans = r.substring(splitT+1, r.length()-1);
  }
  return ans;
}
String time_24(String str) //Changes12hr format  to 24 hr format
{ 
    String t24 ="";
    int i;
    if(str.length() == 10)
      str = "0"+str;
    int h2 = (int)str[0] - '0'; 
    int h1 = (int)str[1] - '0'; 
    int hh = (h2 * 10 + h1 % 10); 
    if (str[9] == 'A') 
    { 
        if (hh == 12) 
        { 
          t24 = t24+"00";
          for (i=2; i <= 7; i++) 
              t24 = t24+str[i]; 
        } 
        else
        { 
            for (int i=0; i <= 7; i++) 
               t24=t24+str[i]; 
        } 
    } 
    else
    { 
        if (hh == 12) 
        { 
            t24 = t24 + "12";
            for (int i=2; i <= 7; i++) 
                t24=t24+str[i];
        } 
        else
        { 
            hh = hh + 12; 
            t24 = t24+hh;
            for (int i=2; i <= 7; i++) 
               t24=t24+str[i];
        } 
    }
    return t24; 
} 

int removeColon(String s) 
{   
  int x = (s[0]-'0')*1000+(s[1] -'0')*100+(s[3]-'0')*10+(s[4]-'0');
  return x;
} 

int diff(String s1, String s2) 
{ 
  int time1 = removeColon(s1); 
  int time2 = removeColon(s2); 
  int hourDiff = time2 / 100 - time1 / 100 - 1; 
  int minDiff = time2 % 100 + (60 - time1 % 100); 
  if (minDiff >= 60) 
  { 
    hourDiff++; 
    minDiff = minDiff - 60; 
  }
  long int h = 60*hourDiff + minDiff;
  if(h<0)
    h= -1;
  //Serial.print("Gap:");
  //Serial.println(h); 
  return h; 
} 

void setup() 
{
  Serial.begin(9600);
  Serial.print("Connecting");
  WiFi.begin("Need", "lotus1666");  //Wifi username and password... 
  while(WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!");
  pinMode(D7, OUTPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  if(WiFi.status() == WL_CONNECTED ) 
  {
      ipadd = ip();
      if(ipadd!="0")
      {
          Serial.print("IPAddress: ");
          Serial.println(ipadd);
          pos = coordinates(ipadd);
          if(pos!="0")
          {
             // lat=-41.68;
              //lon= 146.60;
              int ind = pos.indexOf(",");
              Serial.print("Latitude :");
              lat = pos.substring(0,ind);
              Serial.println(lat);
              Serial.print("Longitude :");
              lon = pos.substring(ind+1, pos.length()-1);
              Serial.println(lon);
              String tim = timings(lat,lon);
              if(tim!="0")
              {
                  int ind =tim.indexOf("M");
                  rise = tim.substring(0,ind+1);
                  rise = time_24(rise);
                  sunset = tim.substring(ind+2,tim.length());
                  sunset = time_24(sunset);
                  Serial.print("Sunrise :");
                  Serial.println(rise);
                  Serial.print("Sunset :");
                  Serial.println(sunset);
                  tnow =  gettime();
                  if(tnow!="0")
                  {
                    Serial.print("Time :");
                    Serial.println(tnow);
                    if(diff(rise.substring(0,5),sunset.substring(0,5)) >0)
                    {
                  
                       int x = diff(tnow.substring(0,5),rise.substring(0,5)); //Function return time gap between the 2 time intervals in minutes
                       int f = diff(tnow.substring(0,5),sunset.substring(0,5));
                       if(x > 0)
                       {
                         //digitalWrite(LED_BUILTIN,LOW);//On the led
                         digitalWrite(D7,LOW);
                         Serial.print("Lamp off after minutes");
                         Serial.println(x);
                         long j = x*60*1000;
                         delay(j);
                         //digitalWrite(LED_BUILTIN,HIGH);//off the led
                         digitalWrite(D7,HIGH);
                       }
                       else
                       {
                         if((x <= 0) && (f>=0))
                         {
                           //digitalWrite(LED_BUILTIN,HIGH);
                           digitalWrite(D7,HIGH);
                           long j = f*60*1000;
                           delay(j);
                         }
                         else
                         {
                           //digitalWrite(LED_BUILTIN,LOW);
                           digitalWrite(D7,LOW);
                           f = diff(tnow.substring(0,5),"23:59");
                           long j = (f+2)*60*1000;
                           delay(j);
                           //digitalWrite(LED_BUILTIN,HIGH);
                           digitalWrite(D7,HIGH);
                          }
                        }
                     }
                     else
                     {
                        int x = diff(tnow.substring(0,5),sunset.substring(0,5));
                        int f = diff(tnow.substring(0,5),rise.substring(0,5));
                        if(x <= 0 && (f>=0))
                        {
                          //digitalWrite(LED_BUILTIN,LOW);
                          digitalWrite(D7,LOW);                             
                              long j = f*60*1000;
                              delay(j);
                           //digitalWrite(LED_BUILTIN,HIGH);
                           digitalWrite(D7,HIGH);
                         }
                         else
                         {
                           if((x > 0))
                           {
                             //digitalWrite(LED_BUILTIN,HIGH);
                             digitalWrite(D7,HIGH);
                             long j = x*60*1000;
                             delay(j);
                            }
                            else if( f <0)
                            {
                              //digitalWrite(LED_BUILTIN,HIGH);
                              digitalWrite(D7,HIGH);
                              f = diff(tnow.substring(0,5),"23:59");
                              long j = (f+2)*60*1000;
                              delay(j);
                               
                             }
                           }
                            
                         }  
                  }
                  else
                  {
                    Serial.println("Error on sending request to clock api");
                  }
              }
              else
              {
                Serial.println("Error on sending request to sunrise and sunset api");
              }
          }
          else
          {
            Serial.println("Error on sending request to Longitude and latitude api");
          }
      }
      else
      {
        Serial.println("Error on sending request to ipaddress api");
      }
  } 
  else 
  {
    Serial.println("No proper wifi connection");
  } 
}
