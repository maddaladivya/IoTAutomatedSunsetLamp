# Sunset Lamp
This is an IoT project which turns on a lamp after the sunset and automatically gets switched off after the sunrise. The advantages of the project comes into picture if it is difficult to use daylight sensor. 

## Description
Consider the case of street lights and huge apartments. In the former case the lights are switched on and off manually at the time of sunset and sunrise respectively. If there is any delay in switching on the street lights may lead to accidents and if there is any delay in switching off the light may lead to huge amount of electricity comsumption which is again an economic constraint. The same is the case with the latter situation. This project works efficiently satisfiying all the constraints.

## Technical Description
Firstly, the [geolocation](http://ipinfo.io) of the lamp is fetch using the [IP Address](https://api.ipify.org). From the geolocation the [sunset and sunrise](http://api.sunrise-sunset.org) timings are fetched from internet. The [current time](http://worldclockapi.com/api/json/utc/now) are also fetched from internet. Comparing the current time with the sunset and sunrise timings the bulb will switch on or off accordingly.

## Contribution
1. Saranya Nukala (AM.EN.U4CSE16146)
   * Contribution: 
     * Handled the logic part of the code which involves the conversion, comparing the timings which is used to control the bulb.
     * Worked on API's.
   * Learnings:
     * Learned how geolocation is figured out from IP Address
     * Learned how to figure out corner cases of a logic.
2. Maddala Divya Santhi (AM.EN.U4CSE16137)
    * Contribution:
      * Handled the requests part of API's and processing the information fetched. Resolved all the issues and errors encountered.
      * Handled the communication between terminal and ESP8266. This was done for the purpose of demo as it is not possible to show if the bulb is working or not according to India timings. So latitude and longitude input is taken via terminal so we can show the output according to other countries.
    * Learnings:
      * Learned how API's work, client-server communication.
      * Learned about flask(there was a situation where I had to create an API but didn't use in the project though!).
3. Pratima Bedadala (AM.EN.U4CSE161504)
    * Contribution:
      * Handled the circuit part of the project.
    * Learning:
      * Learned what is relay and how does it work.
