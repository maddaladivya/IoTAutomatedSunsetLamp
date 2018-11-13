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
const char* host = "192.168.43.156";
String ip(){        //get IP Address
  HTTPClient http;
  String h = "http://api.ipify.org";
  http.begin(h);
  int response = http.GET();
  if(response>0){
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
String timings(String a , String c)             //Get current time
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
      Serial.println(ans);
   }
   return ans;
}
String gettime()
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
String time_24(String str)
{
    String t24 ="";
    int h1 = (int)str[1] - '0';
    int h2 = (int)str[0] - '0';
    int hh = (h2 * 10 + h1 % 10);
    if (str[9] == 'A')
    {
        if (hh == 12)
        {
          t24 = t24+"00";
          for (int i=2; i <= 7; i++)
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
int minutes(String s)
{
  int a = (int)s[3] -'0';
  a =a*10 + (int)s[4] - '0';
  //Serial.println(a);
  return a;
}
int hrs(String s)
{
  int a = (int)s[0] -48;
  a =a*10 + (int)s[1] - 48;
  return a;
}

void setup()
{
  Serial.begin(9600);
  Serial.print("Connecting");
  WiFi.begin("divya", "winteriscoming");  
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!");
  pinMode(D7, OUTPUT);
  //ipadd = (WiFi.localIP()).toString();
  //Serial.println(ipadd);
}

void loop()
{
  if(WiFi.status() == WL_CONNECTED )
  {
          /*ipadd = ip();
          Serial.println(ipadd);
          pos = coordinates(ipadd);
          if(pos!="0")
          {
              Serial.println(pos);
              int ind = pos.indexOf(",");
              lat = pos.substring(0,ind);
              lon = pos.substring(ind+1, pos.length()-1);*/
              WiFiClient client;
              Serial.println("Connected!");
              if (client.connect(host, 8080)){
                while(client.connected()){
                    if(client.available()){
                      Serial.println("Give Input");
                      String pos = client.readStringUntil('\n');
                      Serial.println(pos);
                      int ind = pos.indexOf(",");
                      lat = pos.substring(0,ind);
                      lon = pos.substring(ind+1, pos.length()-1);
                      if(lat!=""){
                        client.stop();
                        }
                      }
                  }
                }
              String tim = timings(lat,lon);
              if(tim!="0")
              {
                  int ind =tim.indexOf("M");
                  rise = tim.substring(0,ind+1);
                  rise = time_24(rise);
                  sunset = tim.substring(ind+2,tim.length());
                  sunset = time_24(sunset);
                  Serial.println(rise);
                  Serial.println(sunset);
                  tnow =  gettime();
                  int x = minutes(tnow);
                  int y =minutes(sunset);
                  if(tnow!="0")
                  {
                      Serial.println(tnow);
                      Serial.println(hrs(tnow));
                      Serial.println(hrs(sunset));
                      if(hrs(tnow) < hrs(sunset))
                      {
                        Serial.println("1");
                        long long int x = 3600000;
                        delay(x);
                      }
                      else if((hrs(tnow) == hrs(sunset)) && (x < y))
                      {
                        Serial.println("2");
                        delay(600000);
                      }
                      else
                      {
                        int f = hrs(tnow);
                        int g = hrs(rise);
                        int k = hrs(sunset);
                        x = minutes(tnow);
                        y =minutes(rise);
                        Serial.println(f);
                        Serial.println(g);
                        while((f>g) || (f==g && x<=y))
                        {
                          Serial.println("Blow");
                          digitalWrite(D7, HIGH);
                          if(f<g)
                          {
                            delay(3600000);
                           
                          }
                          else
                          {
                            delay(600000);
                          }
                          tnow = gettime();
                         
                        }
                        digitalWrite(D7,LOW);
                      }
                  }
                  else
                  {
                    Serial.println("Error on sending request to clock api");
                  }
              /*}
              else
              {
                Serial.println("Error on sending request to sunrise and sunset api");
              }*/
          }
          else
          {
            Serial.println("Error on sending request to Longitude and latitude api");
          }
    
  }
  else
  {
    Serial.println("No proper wifi connection");
  }
}
