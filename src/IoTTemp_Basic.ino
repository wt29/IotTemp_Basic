/* 
IOT Temp - the little weather station that could.  Read/Display/Log. 

Featuring the LOLIN D1 ESP 8266,  and associated shields as desired.
https://lolin.aliexpress.com/store/1331105?spm=a2g0o.detail.1000007.1.277c6380JG6A1m

Use the Lolin D1 R1 in the Arduino Boards

You will need a file "data.h" - copy the template below.

If you have lots of these keep the individual configs in myDeviceName.h for easy reference to save remembering config and board versions
and edit data.h to use that file.  eg #include "Library.h"

Also add that file.h or *.h to the .gitignore.
This is done to ensure that wifi and server passwords dont get uploaded to github.

When it first boots up it shows more information that is useful for any debugging. 
Also accesible via a webserver either on its http://ipaddress or http://nodename.local


-------------------------------------
//template for data.h

//** Node and Network Setup
#define NODENAME "<Your NodeName>";                 // eg "Kitchen"  Required and UNIQUE per site.  Also used to find mdns eg NODENAME.local

#define WIFI                                        // Default is true to enable WiFi

#define APARRAY {"AP1","AP2","AP3"}                 // Array of APs
#define PASSARRAY {"Pass1","Pass2","Pass3"}         // Array of passwords
#define APCOUNT 3                                   // Number of APs to try

#define HOST "<Your emoncms host fqdn>";            // eg  "emoncms.org" Required for logging. Note:just the host not the protocol
#define MYAPIKEY "<Your emoncms API write key>";    // Required Get it from your MyAccount details in your emoncms instance

#define TIMESERVER "0.au.pool.ntp.org";              // Pick a time server
#define TZOFFSET 36000;                              // Add your timezone offset in hours*3600 i.e. seconds

//Enable the following block to your data.h to set fixed IP addresses. Configure as required
//#define STATIC_IP
//IPAddress staticIP( 192,168,1,22 );
//IPAddress gateway( 192,168,1,1 );
//IPAddress subnet(255,255,255,0 );
//IPAddress dns1( 8,8,8,8 );


// **Configuration and Shield Options

//#define CONNECTOR_100 // 100 series shield otherwise defaults to 1.1.0.  
                        // Note that some shields show 1.1.0 but are really version 1.0.0.  
                        // If your TFT stays "white" or is blank on bootup then you probably have a 1.0.0 regardless of branding.

//#define HEADLESS      // Define if you don't have a display. Defaults to true


// **Sensors
#define SENSORCOUNT  <how many sensors in your config>  // Normally 2 (temp+humidity).  This allows for BIG letters on the temp and humidity

//- Air Quality
//#define AIRQUALITY    // enable SGP30 Shield 
                        // TVOC: (Total Volatile Organic Compound) concentration within a range of 0 to 60,000 parts per billion (ppb)
                        // Note: Note that TVOC in ppm is  EU standard.  https://help.atmotube.com/faq/5-iaq-standards/
                        // [.] Action: Discuss - best way.  We can report as ppm but emon rounds to 2decimal places so we lose granularity...?
                        // eCO2: (equivalent calculated carbon-dioxide) concentration  400-60000 ppm
                        //
                        // While this shield can be run in isolation its better to get the actual temp and humidity inputs
                        // for more accurate calculations - in our case we feed it data from our temp/humidity shield.
                        // https://www.wemos.cc/en/latest/d1_mini_shield/sgp30.html
                        // Library Manager should find it but...  https://github.com/adafruit/Adafruit_SGP30
                        // docs: https://adafruit.github.io/Adafruit_SGP30/html/class_adafruit___s_g_p30.html#a3cea979c8b14138cef092f13102b0e22

                        
//- temperature and humidity
//#define HASDHT12        // If you have the older DHT12 otherwise will default to SHT30
                          // DHT12 temperature and humidity sensor was originally used but humidity not accurate. Deprecated!
                          // SHT30 is default and best bang for buck at present.

//- Barometer
//#define BMP                 // Define to enable Barometric Air Pressure Shield Libraries and Logging 
//#define LOCALALTITUDE 300;  // Required if using BMP.  Enter your local altitude in meters eg 300
                              // The reported pressure is corrected  by currentSensorReading + ((117/1000)*YourLocalAltitude).  
                              // Notes Barometric Pressure readings need to be calibrated by 117 for every rise of 1000m above sea level.  
                              // eg for 300m abobe sea level, the calc is 0.117 * 300 = 35

//- Light meter
//placeHolder for now

//- water
//placeHolder for now

//- wind speed
//- wind angle 
//placeHolders for now

//#define BFDLOGGING    // Define to enable logging BushFireFactor to the server. 
                        // Non Scientific but useful enough.  Plan to incorporate wind speed/rainfall in future.

//#define BRFACTOR 1;   // Bushfire Rating Factor (Multiplier).  Default is 44 (for granularity/graphing purposes/100).  Define (uncomment) your own value.

#define IOTAWATT        // Just for fun you can connect to an IotaWatt and show a current feed value
#define IW_VALUE   "http://<youIotaWattIP>/query?select=[time.local.iso,<Your Feed Name>.watts]&begin=m-1m&end=m&group=m&format=csv"
#define IW_SOLAR   "http://<youIotaWattIP>/query?select=[time.local.iso,<Your Solar Feed Name>.watts]&begin=m-1m&end=m&group=m&format=csv"

// --end of data.h

-------------------------------------

Trying to do this in both Arduino IDE and PlatformIO is too hard - Stick to Arduino IDE

Additional Libraries for DHT12 and SHT30 etc.  Download and save to user documents
They show a warning on compile but are fine.
https://github.com/wemos

*/

#define VERSION 1.46            // 1.46 Uses the multiAP array method of connecting to WiFI
                                // 1.45 Minor - should now show the correct hostname in DHCP server
                                // 1.44 EEProm now maintains max/min for temperature and humidity
                                // 1.40 IotaWatt integration - Interesting for http GET
                                // 1.37 Internet time, uptime and Max/Min temps during run, remote reboot (just because I can) 
                                // 1.35 Very minor - cleanup comments and some old logic.
                                // 1.34 Got bored one evening. Changed the text size for the basic temp and RH to LARGE. See new SENSORCOUNT define
                                // 1.33 Got rid of unnecessary stuff on the display. Added something cute to the webserver.
                                // 1.32 Temperature controlled Heading!
                                // 1.31 Enable SGP30 Shield V1.0.0 AIR QUALITY SENSOR
                                // 1.30 Change to BMPaltitude to calibrate from defined LOCALALTITUDE in data.h              
                                // 1.29 Add BMPaltitude to data.h.  Tweak to comments for consistency.  Expanded notes in template and cleaned up code a little more.
                                // 1.28 Moved the Static IP options to the data.h. Sensor now defaults to SHT30
                                // 1.27 Removed Real Time Clock (RTC) routines. Only useful if RTC and SD Card logging available.
                                // 1.26 Tweaks to wording for data.h.  New Defaults for CONNECTOR and BFD logging.  Prep for new shields. Mild code refactor.
                                // 1.25 WebClient
                                // 1.24 Pressure and headless operation 
                                // 1.23 Bushfire danger feeds - now defaults to 44
                                // 1.25 edit bushFireRatingFactor to taste
                                // 1.42 Iotawatt integration Dunno what happened to 1.3x
                                // 1.43 Iotawatt solar output and hostname
                                // 1.44 EEPROM Access for Max and Min stuff beyond reboots
                                

#warning Setup your data.h.  Refer to the provided template at top of this file.

#include <EEPROM.h>          // Going to save some Max/Min stuff
#define EEPROM_SIZE 32       // 4 bytes each for TempMax, TMaxEpoch, TempMin, TMinEpoch, HumidityMax, HMaxEpoch, HumidityMin, HMinEpoch

#define WIFI                 // this will just run as a display device if this is disabled. Note: no historic max/mins as it can't find a timeserver without WiFi
//Node and Network Setup
#ifdef WIFI
  #include <ESP8266WiFi.h>
  #include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
  #include <WiFiClient.h>
  #include <ESP8266mDNS.h>
  #include <ESP8266WebServer.h>   // Include the WebServer library
  #include <ArduinoOTA.h>
  #include <NTPClient.h>          // EasyNTPClient by Harsha Alva - its in the Library Manager.
  #include <WiFiUdp.h>
  #include <ESP8266HTTPClient.h>
#endif

// Needed to move this here as the IPAddress types aren't declared until the WiFi libs are loaded
#include "data.h"             // Create this file from template above.  
                              // Pete: so we don't need a push after burning a new cfg - in the "data.h" just shove "#include "hackdesk.h" (or whatever)
                              // This means we dont keep uploading API key+password to GitHub. (data.h should be ignored in repository)
#ifndef SENSORCOUNT
 #define SENSORCOUNT     2   // That will assume more than just 2 sensors.  Less sensors = bigger font size on TFT display
#endif

#ifndef HEADLESS                 // no screen
 #include <Adafruit_GFX.h>       // Core graphics library
 #include <Adafruit_ST7735.h>    // Hardware-specific library
#endif

const char* nodeName = NODENAME;
// const char* ssid = LOCALSSID;
// const char* password = PASSWORD;
const char* passwords[] = PASSARRAY;
const char* accessPoints[] = APARRAY;
const char* host = HOST;
const char* APIKEY = MYAPIKEY;
#ifdef SENSORCOUNT
 const int numberOfSensors = SENSORCOUNT;
#endif

boolean showIP = true;    // Only show the WiFi/IP details on first run through after bootup (troubleshooting)

//Configuration and Shield Options

//connector shield version (Load Library and Instantiate)
#ifndef CONNECTOR_100    // the v1.1.0 connector board has different CS and DC values
  #define TFT_CS     D4
  #define TFT_DC     D3
#else                    // Otherwise use the 1.0.0 values
  #define TFT_CS     D0
  #define TFT_DC     D8
#endif

//headless vs TFT?
#ifndef HEADLESS
  #define TFT_RST    -1       // you can also connect this to the Arduino reset. in which case, set this #define pin to -1!
  Adafruit_ST7735 tft = Adafruit_ST7735( TFT_CS, TFT_DC, TFT_RST);    // Instance of tft
#endif

//air quality shield 
#ifdef AIRQUALITY //Air Quality Shield
  #include "Adafruit_SGP30.h"
  Adafruit_SGP30 sgp30;         //SPG3030 Air quality instance
  float AQ_TVOC;                //  ppb Total Volatile Organic Components
  float AQ_eCO2;                //  ppm estimated concentration of carbon dioxide calculated from known TVOC concentration
#endif

#ifdef BMP    // Barometric Pressure shield
  #include <LOLIN_HP303B.h>
  LOLIN_HP303B HP303B;         // HP303B BMP instance
  int32_t pressure;
  int16_t bmpRet;

  //setup and calculate the correct barometer pressure for your altitude based on data.h
  int localAltitude = LOCALALTITUDE;
  float BMPCorrection = ( localAltitude * 0.117 );
  float pressureMSL;
#endif

//temperature and humidity shield
#ifndef HASDHT12
  #include <WEMOS_SHT3X.h>      // SHT30 Current best bang for back in typical human/environment temp and humidity ranges
  SHT3X sht30(0x45);
#else
  #include <WEMOS_DHT12.h>      // depreciated DHT12 temperature and humidity sensor. 
  DHT12 dht12;                  
#endif

//Do we want to log BushFire Danger Stuff? 
#ifdef BFDLOGGING     
  //nothing needed for now
  //logging so we need to setup BRFactor etc.
  //BushFire Rating Factor (amount to multiply the result by for logging purposes)
  #ifndef BRFACTOR
    int brFactor = 44;
  #else
    int brFactor = BRFACTOR;
  #endif
#endif
 
//Working Variables etc.
#define CELSIUS             // Comment out if you prefer Fahrenheit
float TempC;
float TempF;
float Humidity;

float maxTemp;  // Force it to start
float minTemp;  // force it to start
float maxHumidity;  // Force it to start
float minHumidity;  // force it to start

unsigned long maxTempEpoch;
unsigned long minTempEpoch;
unsigned long maxHumidityEpoch;
unsigned long minHumidityEpoch;

#ifdef WIFI
  WiFiClient client;              // Instance of WiFi Client
  ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
  ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
  void handleRoot();              // function prototypes for HTTP handlers
  void handleNotFound();
  void readEEPROM();
  void restEEPROM();

  const long utcOffsetInSeconds = TZOFFSET;       // Sydney is 10 hours ahead
  const char* timeServer = TIMESERVER;
  char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, timeServer, utcOffsetInSeconds );
  unsigned long startEpochTime = 50000;    // How long have we been running for?
  String startFormattedTime;               // For display purposes
#endif

int waitForWiFi = 20000 ;         // How long to wait for the WiFi to connect - 10 Seconds should be enough 
int startWiFi;
int connectMillis = millis();     // this gets reset after every successful data push

int poll = 60000;           // Poll the sensor every 60 seconds (or so)
int lastRun = millis() - (poll + 1);

#ifdef IOTAWATT
 String IWApi;
 String IW_payload;
 int IW_value;
 #ifdef IW_SOLAR
 int IW_solar;
 #endif
#endif

//-----------------
void setup()
{
  Serial.begin(115200); //baud rate
  Serial.println();

  EEPROM.begin(32);                 // this number in "begin()" is ESP8266. EEPROM is emulated so you need buffer to emulate.
  EEPROM.get(0,maxTemp);            // Should read 4 bytes for each of these thangs
  if ( isnan( maxTemp ) ) { maxTemp = 0; }  // Having issues during devel with "nan" (not a number) being written to EEPROM
  EEPROM.get(4,maxTempEpoch);            
  EEPROM.get(8,minTemp);       
  if ( isnan( minTemp ) ) { minTemp = 100; }     
  EEPROM.get(12,minTempEpoch);            
  EEPROM.get(16,maxHumidity);         
  if ( isnan( maxHumidity ) ) { maxHumidity = 0; }
  EEPROM.get(20,maxHumidityEpoch);         
  EEPROM.get(24,minHumidity);        
  if ( isnan( minHumidity ) ) { minHumidity = 100; }
  EEPROM.get(28,minHumidityEpoch);       

#ifdef AIRQUALITY
  sgp30.begin(); // startup/calibrate the air quality shield
  sgp30.IAQinit();
#endif

#ifdef BMP
  HP303B.begin(); // I2C address = 0x77
#endif

#ifndef HEADLESS
  tft.initR(INITR_144GREENTAB);
  tft.setTextWrap(false);     // Allow text to run off right edge
  tft.setRotation( 1 );       // Portrait mode
#endif

#ifdef WIFI

timeClient.begin();
timeClient.forceUpdate();

if (MDNS.begin( nodeName )) {              // Start the mDNS responder for <nodeName>.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
server.on("/reboot",rebootDevice);        // Kick over remotely
server.on("/reseteeprom",resetEEPROM);    // Reset the EEPROM values
server.on("/readeeprom",readEEPROM);      // Read and display the EEPROM values
server.on("/writeeeprom",writeEEPROM);      // Read and display the EEPROM values
server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

server.begin();                           // Actually start the server
Serial.println("HTTP server started");

ArduinoOTA.begin();                       // Remote updates
ArduinoOTA.setHostname( nodeName );

#endif
}       // Setup


//------------------------
void loop() {

#ifdef WIFI 
 server.handleClient();                    // Listen for HTTP requests from clients
 ArduinoOTA.handle();
#endif

if ( millis() > lastRun + poll ) {        // only want this happening every so often - see Poll value

// Attempt to set the start time correctly on "non pro" D1
// It seems to keep returning ~10000 (10K )which is the timezone offset until inited correctly 
// The Wemos D1 seem slow to update this it may take a couple of loops to sync correctly. The D1 Pro seems fine.

if ( startEpochTime < 500000 ) {             
  timeClient.update();                     
  unsigned long epochTime = timeClient.getEpochTime();
  Serial.println( epochTime );
  startEpochTime = epochTime;
  startFormattedTime = timeClient.getFormattedTime();
}

#ifndef HASDHT12
  Serial.println("Reading SHT30 Temperature/Humidity Shield");
 if( !(sht30.get() == 0 ) ){
#else  
  Serial.println("Reading DHT12 Temperature/Humidity Shield");
 if( !(dht12.get() == 0 ) ){
#endif
  Serial.println("Cannot read Temperature/Humidity Shield");
  #ifndef HEADLESS
    tft.println("Temp/Humidity Shield Error");
  #endif
  
 }
 else
 {
  
#ifndef HASDHT12
  TempC = sht30.cTemp;
  TempF = sht30.fTemp;
  Humidity = sht30.humidity;
#else
  TempC = dht12.cTemp;
  TempF = dht12.fTemp;
  Humidity = dht12.humidity;
#endif  
 if ( startEpochTime > 1630929506 ) {   // no point recording this until time is useful
  if (TempC > maxTemp ) {
    maxTemp = TempC;
    maxTempEpoch = timeClient.getEpochTime() ;
    EEPROM.put( 0, maxTemp );
    EEPROM.put( 4, maxTempEpoch );
    EEPROM.commit();
   }

   if (TempC < minTemp ) {   
     minTemp = TempC;
     minTempEpoch = timeClient.getEpochTime() ;
     EEPROM.put( 8, minTemp );
     EEPROM.put( 12, minTempEpoch );
     EEPROM.commit();

   }
  if (Humidity > maxHumidity ) {
    maxHumidity = Humidity;
    maxHumidityEpoch = timeClient.getEpochTime() ;
    EEPROM.put( 16, maxHumidity );
    EEPROM.put( 20, maxHumidityEpoch );
    EEPROM.commit();
   }

   if (Humidity < minHumidity ) {   
    minHumidity = Humidity;
    minHumidityEpoch = timeClient.getEpochTime() ;
    EEPROM.put( 24, minHumidity );
    EEPROM.put( 28, minHumidityEpoch );
    EEPROM.commit();

   }
  }
  Serial.println();
  Serial.print("Temperature in Celsius : ");
  Serial.println(TempC);
  Serial.print("Temperature in Fahrenheit : ");
  Serial.println(TempF);
  Serial.print("Relative Humidity : ");
  Serial.println(Humidity);

#ifdef AIRQUALITY
  Serial.println("Reading sgp30 Air Quality Shield");

  //set the absolute humidity to enable humidity compensation for the air quality signals
  sgp30.setHumidity(Humidity);
  
  if (sgp30.IAQmeasure())
  {
    AQ_eCO2 = sgp30.eCO2;
    AQ_TVOC = (sgp30.TVOC); // [] Do we end up converting from ppb to ppm  ??
    Serial.print("TVOC "); Serial.print(AQ_TVOC); Serial.print(" ppb\t");
    Serial.print("eCO2 "); Serial.print(AQ_eCO2); Serial.println(" ppm");
    Serial.print("Raw H2 "); Serial.print(sgp30.rawH2); Serial.print(" \t");
    Serial.print("Raw Ethanol "); Serial.print(sgp30.rawEthanol); Serial.println("");
   }
  else
  {
  Serial.println("Cannot read AQ Sensor");
  #ifndef HEADLESS
    tft.println("AQ  Sensor Error");
  #endif
  }
#endif

#ifdef BMP
  Serial.println("Reading HP303B Barometric Pressure Shield");
  bmpRet = HP303B.measurePressureOnce(pressure, 7);  
  pressure = pressure/100;                    // only interested in millbars not pascals
  pressureMSL = ( pressure + BMPCorrection ); // adjust for altitude defined in data.h
  Serial.print("Pressure mBar : ");
  Serial.println(pressureMSL);
#endif

  Serial.print("Free Heap : ");
  Serial.println(ESP.getFreeHeap());

#ifndef HEADLESS
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  if (showIP) {
    tft.println(" IoT Temp");          // We know it's an IoT Temp !
  }
  tft.setTextColor(ST7735_WHITE);
  if (numberOfSensors < 4 && !showIP) {
    tft.println("");  //PB Needed an extra line on the screen
    tft.setTextSize(3);
    tft.print("T " );
  }
  else
  {  
  tft.print("Tmp " );
  }
// Set the color of the temp - just because we can
  if (TempC < 10 ) {
    tft.setTextColor(ST7735_BLUE);   // Its chilly
  }
  else if (TempC < 18) {
    tft.setTextColor(ST7735_GREEN);   // Still cold
  }
  else if (TempC < 26) {
    tft.setTextColor(ST7735_YELLOW);   // Nice temps
  }
  else if (TempC < 33) {
    tft.setTextColor(ST7735_ORANGE);   // Warming up
  }
    else {
    tft.setTextColor(ST7735_RED);      // Hot
  }
  
//  tft.setTextColor(ST7735_GREEN);

  #ifdef CELSIUS
    tft.println(TempC);
  #else
    tft.println(TempF);
  #endif
  
  tft.setTextColor(ST7735_WHITE);
  if ((numberOfSensors < 4) && !showIP ) {
   tft.print("H ");
  }   
  else
  {
    tft.print("R/H ");
  }
  if (Humidity > 80) {
    tft.setTextColor(ST7735_RED);     // Humid!
  }
  else if (Humidity > 50) {
    tft.setTextColor(ST7735_GREEN);   // Still high but bearable
  }
  else {
     tft.setTextColor(ST7735_BLUE);    // Nice     
  }
  tft.println(Humidity);
  
#ifdef IOTAWATT
  tft.setTextColor(ST7735_WHITE);
  if ((numberOfSensors < 4) && !showIP ) {
   tft.print("W ");
  }   
  else
  {
    tft.print("W ");
  }
  if (IW_value < 0 ) {
    tft.setTextColor(ST7735_GREEN);   // Power to the grid
  }
  else if (IW_value < 500 ) {
    tft.setTextColor(ST7735_YELLOW);   // Low usage
  }
  else if (IW_value < 2000 ) {
    tft.setTextColor(ST7735_ORANGE);   // Probably not bad
  }
    else {
    tft.setTextColor(ST7735_RED);      // Sucking down the juice
  }
  
  tft.println( IW_value );
 #ifdef IW_SOLAR
  tft.setTextColor(ST7735_WHITE);
  tft.print("S ");
  tft.setTextColor(ST7735_GREEN);
  tft.println( IW_solar );
 #endif 

#endif
  #ifdef BMP
    tft.setTextColor(ST7735_WHITE);
    if ((SENSORCOUNT < 4) && !showIP ) {
     tft.print("P ");
    }   
    else
    {
    tft.print("mBar ");
    }
    tft.setTextColor(ST7735_GREEN);
    int intMSL = pressureMSL;
    tft.println( intMSL );
  #endif    
  tft.setTextSize(2);   // Enough of those oversized letters
  #ifdef AIRQUALITY
    if (showIP){
     tft.setTextSize(1); // Only needs to be small for one pass until we don't show WiFi details
    }
    tft.setTextColor(ST7735_WHITE);
    tft.print("eCO2 ");
    tft.setTextColor(ST7735_GREEN);
    int inteCO2 = AQ_eCO2;
    tft.println( inteCO2 );
    tft.setTextColor(ST7735_WHITE);
    tft.print("TVOC ");
    tft.setTextColor(ST7735_GREEN);
    int intTVOC = AQ_TVOC;
    tft.println( intTVOC );
    tft.println("");
    tft.setTextSize(2); //back to defaults
  #endif 
     
  if (showIP){   // again - only show this in the first iteration
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.print(" Node:");
  tft.setTextColor(ST7735_GREEN);
  tft.println(nodeName);
  }
#endif

#ifdef WIFI
  if (WiFi.status() != WL_CONNECTED){

  for (int i = 0; i <= APCOUNT; i++) {
    wifiMulti.addAP( accessPoints[i], passwords[i] );     // Add all the APs and passwords from the arrays
  }
    connectWiFi();
  }
  if (WiFi.status() != WL_CONNECTED ) {
    #ifndef HEADLESS
       tft.setTextColor(ST7735_RED);
       tft.println("");
       tft.print(" Error:");
       tft.setTextColor(ST7735_GREEN);
       tft.println("No Wifi Conn");
    #endif  
  }
  else
  {
 #ifdef IOTAWATT

      HTTPClient http;
      String serverPath = IW_VALUE; 
      http.begin(client,serverPath);
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("IotaWatt HTTP Response code: ");
        Serial.println(httpResponseCode);
        IW_payload = http.getString();
        IW_value = IW_payload.substring(IW_payload.indexOf(',')+ 1).toInt();
        
        Serial.println(IW_value);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      // Free resources
      http.end();
      }
#ifdef IW_SOLAR
      serverPath = IW_SOLAR; 
      http.begin(client,serverPath);
      
      // Send HTTP GET request
      httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("IotaWatt HTTP Response code: ");
        Serial.println(httpResponseCode);
        IW_payload = http.getString();
        IW_solar = IW_payload.substring(IW_payload.indexOf(',')+ 1).toInt();
        
        Serial.println(IW_solar);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();

#endif
 
    #endif


   #ifndef HEADLESS
    if (showIP) {
       tft.setTextColor(ST7735_WHITE);
       tft.print(" SSID:" );
       tft.setTextColor(ST7735_GREEN);
       tft.println( WiFi.SSID() );
       tft.setTextColor(ST7735_WHITE);
       tft.print("   IP:" );
       tft.setTextColor(ST7735_GREEN);
       tft.println( WiFi.localIP() );
       showIP = false;
    }
    #endif
    Serial.printf("\n[Connecting to %s ... ", host, "\n");
      
    if (client.connect(host, 80))     {
      Serial.println("Connected]");
      Serial.println("[Sending a request]");

    String request  = "GET " ;
           request += "/input/post?node=";
           request += nodeName;
           request += "&fulljson={\"temp\":";
  #ifdef CELSIUS
           request += TempC ;
  #else
           request += TempF ;
  #endif         
           request += ",\"humidity\":" ;
           request += Humidity ;

  #ifdef BFDLOGGING
           request += ",\"BFD\":" ;
           request += ((1/Humidity)*TempC*brFactor) ;
  #endif

  #ifdef AIRQUALITY
          request += ",\"eCO2\":" ;
          request += inteCO2 ;
          request += ",\"TVOC\":" ;
          request += intTVOC ;          
  #endif
  
  #ifdef BMP
           request += ",\"Pressure\":" ;
           request += pressureMSL ;
  #endif
           request += "}&apikey=";
           request += APIKEY; 

    Serial.println( request );
    client.println( request );

    Serial.println("[Response:]");

    while (client.connected()) {
     if (client.available()) {
      String resp = "Null";
      resp = client.readStringUntil('\n');  // See what the host responds with.
      Serial.println( resp );

     }
    }
   }
   else
   {
    Serial.println("Connection failed!]");
    tft.setTextColor(ST7735_RED);
    tft.print( " " );  
    tft.println( host );
    tft.print(" Connection failed");
   }
#endif  // def WIFI
  }  
     lastRun = millis();
  
 }    // Wifi Status 
 }    // Sensor Read

}     // Loop

void tftPrint ( char* value, bool newLine, int color ) {
#ifndef HEADLESS   // just don't do anything
  tft.setTextColor( color );
  if (newLine) {
    tft.println( value );
  }
  else
  {
  tft.print( value);
  }
#endif
}

void connectWiFi() {

#ifdef STATIC_IP
  WiFi.config( staticIP, gateway, subnet, dns1 );
#endif
  WiFi.hostname( nodeName );     // This will show up in your DHCP server
  while (wifiMulti.run() != WL_CONNECTED) {

    startWiFi = millis() ;        // When we started waiting

    while ((WiFi.status() != WL_CONNECTED) && ( (millis() - startWiFi) < waitForWiFi ))
    {
      millisDelay(500);
      Serial.print(".");
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("IP Address: ");
    Serial.println( WiFi.localIP());
    Serial.printf("Connection status: %d\n", WiFi.status());
  }

}
/*
void connectWiFi() {

#ifdef STATIC_IP  
 WiFi.config( staticIP, gateway, subnet, dns1 );
#endif
  String newHostName = NODENAME;
  WiFi.setHostname( newHostName.c_str() );     // This will show up in your DHCP server
  WiFi.begin(ssid, password);

  String strDebug = ssid ;
  strDebug += "  ";
  strDebug +=  password;
  Serial.println( strDebug );
  
  startWiFi = millis() ;        // When we started waiting
  // Loop and wait 
  while ((WiFi.status() != WL_CONNECTED) && ( (millis() - startWiFi) < waitForWiFi ))
  {
    delay(500);
    Serial.print(".");
  }

  tft.print("");
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println( WiFi.localIP());
  Serial.printf("Connection status: %d\n", WiFi.status());

}
*/
#ifdef WIFI
void handleRoot() {
  String url = "<a href=http://" + String(host) + ">"+host+"</a></b><br>";
  String response = "<h2>Welcome to IoT Temp on node " + String(nodeName) + "</h2>";
         response += "<p></p><table style=\"width:600\">";
         response += "<tr><td>Temperature </td><td><b>" + String(TempC) + "C</b></td></tr>";
         response += "<tr><td>Humidity </td><td><b>" + String(Humidity) + " %RH</b></td></tr>";
         response += "<tr><td>Maximum temperature recorded </td><td><b>" + String(maxTemp) + "</b> on <b>" + fullDate( maxTempEpoch ) + "</b></td></tr>";
         response += "<tr><td>Minimum temperature recorded </td><td><b>" + String(minTemp) + "</b> on <b>" + fullDate( minTempEpoch ) + "</b></td></tr>";
         response += "<tr><td>Maximum humidity recorded </td><td><b>" + String(maxHumidity) + "</b> on <b>" + fullDate( maxHumidityEpoch ) + "</b></td></tr>";
         response += "<tr><td>Minimum humidity recorded </td><td><b>" + String(minHumidity) + "</b> on <b>" + fullDate( minHumidityEpoch ) + "</b></td></tr>";

  #ifdef BMP
         response += "<tr><td>Air Pressure local </td><td><b>" + String(pressure) + " millibars</b></td></tr>";
         response += "<tr><td>Air Pressure MSL </td><td><b>" + String(pressureMSL) + " millibars</b></td></tr>";
  #endif
  #ifdef BFDLOGGING
         response += "<tr><td>Bushfire Rating </td><td><b>"+ String((1/Humidity)*TempC*brFactor) + " </b></td></tr>";
  #endif
  #ifdef AIRQUALITY
         // response += "<br>";
         response += "<tr><td>Air Quality eCO2 </td><td><b>"+ String(AQ_eCO2) + " ppm</b></td></tr>";
         response += "<tr><td>Air Quality TVOC </td><td><b>"+ String(AQ_TVOC) + " ppb</b></td></tr>";
  #endif
         response += "<tr><td>Device started </td><td><b>" + fullDate( startEpochTime ) + "</b></td></tr>";
         response += "<tr><td>Current time </td><td><b>" + fullDate( timeClient.getEpochTime()) + "</b></td></tr>";
  #ifdef IOTAWATT
         response += "<tr><td>IotaWatt Value </td><td><b>" + String( IW_value ) + "</b></td></tr>";  
    #ifdef IW_SOLAR       
         response += "<tr><td>IotaWatt Solar </td><td><b>" + String( IW_solar ) + "</b></td></tr>";  
    #endif
  #endif              
 
         int runSecs = timeClient.getEpochTime() - startEpochTime;
         int upDays = abs( runSecs / 86400 );
         int upHours = abs( runSecs - ( upDays * 86400 ) ) / 3600;
         int upMins = abs( ( runSecs - (upDays * 86400) - ( upHours*3600 ) ) / 60 ) ;
         int upSecs = abs( runSecs - (upDays * 86400) - ( upHours*3600 ) - ( upMins*60 ) );
         String upTime = String(upDays) + "d " + String( upHours ) + "h " + String(upMins) + "m " +String(upSecs) + "s";
         response += "<tr><td>Uptime  </td><td><b>" + upTime + "</b></td></tr>";

         response += "<tr><td>Currently logging to </td><td>" + url + "</td></tr>";
         response += "<tr><td>Local IP is: </td><td><b>" + WiFi.localIP().toString() + "</b></td></tr>";
         response += "<tr><td>Free Heap Space </td><td><b>" + String(ESP.getFreeHeap()) + " bytes</b></td></tr>";
         response += "<tr><td>Software Version </td><td><b>" + String(VERSION) + "</b></td></tr>";
         response += "</table>";
         
  server.send(200, "text/html", response );   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
#endif

void rebootDevice(){
  server.send(200, "text/html", "<h1>Rebooting " + String(nodeName) + " in 5 seconds</h1>"); // Warn em
  delay( 5000 );
  ESP.restart();
}

void resetEEPROM(){    // Sometimes it just doesn't seem right....
  server.send(200, "text/html", "<h1>Resetting EEPROM</h1>"); // Warn em
  EEPROM.put( 0, 0.00f );
  EEPROM.put( 4, 0L );
  EEPROM.put( 8, 0.00f );
  EEPROM.put( 12, 0L );
  EEPROM.put( 16, 0.00f );
  EEPROM.put( 20, 0L );
  EEPROM.put( 24, 0.00f );
  EEPROM.put( 28, 0L );

}

void readEEPROM(){    // Whats in the EEPROM
  float mxT, miT, mxH, miH;
  long mxTE, miTE, mxHE, miHE;
  EEPROM.get( 0, mxT );
  EEPROM.get( 4, mxTE );
  EEPROM.get( 8, miT );
  EEPROM.get( 12, miTE );
  EEPROM.get( 16, mxH );
  EEPROM.get( 20, mxHE );
  EEPROM.get( 24, miH );
  EEPROM.get( 28, miHE );
  server.send(200, "text/html", 
      String(mxT) + " " + String(mxTE) + " " + 
      String(miT)+ " " + String(miTE)+ " " + 
      String(mxH)+ " " + String(mxHE)+ " " + 
      String(miH)+ " " + String(miHE)  ); 

}

void writeEEPROM(){    // Flush the values
  EEPROM.put( 0, maxTemp );
  EEPROM.put( 4, maxTempEpoch );
  EEPROM.put( 8, minTemp );
  EEPROM.put( 12, minTempEpoch );
  EEPROM.put( 16, maxHumidity );
  EEPROM.put( 20, maxHumidityEpoch );
  EEPROM.put( 24, minHumidity );
  EEPROM.put( 28, minHumidityEpoch );
  EEPROM.commit();
  server.send(200, "text/html", 
      String(maxTemp) + " " + String(maxTempEpoch) + " " + 
      String(minTemp)+ " " + String(minTempEpoch)+ " " + 
      String(maxHumidity)+ " " + String(maxHumidityEpoch)+ " " + 
      String(minHumidity)+ " " + String(minHumidityEpoch)  ); 

}



String getInternetTime() {
  timeClient.update();
  delay(200);
  Serial.print( "Formatted Time" );
  Serial.println( timeClient.getFormattedTime() );
  return String( timeClient.getFormattedTime() );
}

String fullDate ( unsigned long epoch ){

static unsigned char month_days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
static unsigned char week_days[7] = {4,5,6,0,1,2,3}; //Thu=4, Fri=5, Sat=6, Sun=0, Mon=1, Tue=2, Wed=3

unsigned char ntp_hour, ntp_minute, ntp_second, ntp_week_day, ntp_date, ntp_month, leap_days, leap_year_ind ;
String dow, sMonth;
unsigned short temp_days;

unsigned int ntp_year, days_since_epoch, day_of_year; 

    leap_days=0; 
    leap_year_ind=0;
    
    ntp_second = epoch%60;
    epoch /= 60;
    ntp_minute = epoch%60;
    epoch /= 60;
    ntp_hour  = epoch%24;
    epoch /= 24;
        
    days_since_epoch = epoch;      //number of days since epoch
    ntp_week_day = week_days[days_since_epoch%7];  //Calculating WeekDay
     
    ntp_year = 1970+(days_since_epoch/365); // ball parking year, may not be accurate!
 
    int i;
    for (i=1972; i<ntp_year; i+=4)      // Calculating number of leap days since epoch/1970
       if(((i%4==0) && (i%100!=0)) || (i%400==0)) leap_days++;
            
    ntp_year = 1970+((days_since_epoch - leap_days)/365); // Calculating accurate current year by (days_since_epoch - extra leap days)
    day_of_year = ((days_since_epoch - leap_days)%365)+1;
  
   
    if(((ntp_year%4==0) && (ntp_year%100!=0)) || (ntp_year%400==0))  
     {
        month_days[1]=29;     //February = 29 days for leap years
        leap_year_ind = 1;    //if current year is leap, set indicator to 1 
       }
        else month_days[1]=28; //February = 28 days for non-leap years 

         temp_days=0;
   
    for (ntp_month=0 ; ntp_month <= 11 ; ntp_month++) //calculating current Month
       {
           if (day_of_year <= temp_days) break; 
           temp_days = temp_days + month_days[ntp_month];
        }
    
    temp_days = temp_days - month_days[ntp_month-1]; //calculating current Date
    ntp_date = day_of_year - temp_days;
    
   
    switch(ntp_week_day) {
                         
                         case 0: dow = "Sunday";
                                 break;
                         case 1: dow = "Monday" ;
                                 break;
                         case 2: dow = "Tuesday";
                                 break;
                         case 3: dow = "Wednesday";
                                 break;
                         case 4: dow = "Thursday";
                                 break;
                         case 5: dow = "Friday";
                                 break;
                         case 6: dow = "Saturday";
                                 break;
                         default: break;        
                         }
  
  switch(ntp_month) {
                         
                         case 1: sMonth = "January";
                                 break;
                         case 2: sMonth = "February";
                                 break;
                         case 3: sMonth = "March";
                                 break;
                         case 4: sMonth = "April";
                                 break;
                         case 5: sMonth = "May";
                                 break;
                         case 6: sMonth = "June";
                                 break;
                         case 7: sMonth = "July";
                                 break;
                         case 8: sMonth = "August";
                                 break;
                         case 9: sMonth = "September";
                                 break;
                         case 10: sMonth = "October";
                                 break;
                         case 11: sMonth = "November";
                                 break;
                         case 12: sMonth = "December";       
                         default: break;        
                         }
/*  
  printf(" %2d",ntp_date);
  printf(", %d\n",ntp_year);
  printf("TIME = %2d : %2d : %2d\n\n", ntp_hour,ntp_minute,ntp_second)  ;
  printf("Days since Epoch: %d\n",days_since_epoch);
  printf("Number of Leap days since EPOCH: %d\n",leap_days);
  printf("Day of year = %d\n", day_of_year);
  printf("Is Year Leap? %d\n",leap_year_ind);
*/
  String sNTPM = String( "00" + ntp_minute );
  sNTPM = sNTPM.substring( sNTPM.length() -2, sNTPM.length() );
  return String( dow + " " + ntp_date + " " + sMonth + " " + ntp_year + " " + ntp_hour + ":" + ntp_minute + ":" + ntp_second  );
}

// Wait around for a bit
void millisDelay ( int mDelay )
{
  int now = millis();
  do {
    // Do nothing
  } while ( millis() < now + mDelay);

}
