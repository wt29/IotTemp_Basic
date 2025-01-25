
# IotaTemp_basic

Wemos D1 mini and SHT30 Sensor logs to the internet. The older DHT12 is also supported if you have one handy.

I've switched to VSCode - it seems somewhat easier to deal with if you are playing with other languages.

Uses a Wemos/Lolin D1 mini hooked up to the Wemos DHT v3.0.0 shield that logs temp and humidity up to an EmonCMS server. 
Currently, I'm running a local Pi4 as a server so having my API keys isn't a security issue.

You can create your own EmonCMS server, the code is open source or create an account at emoncms.org. If you decide to use AWS, <b>pro tip:</b> Reserved Instances are your friend.

Create a local data.h and substitute your values as shown in the top of the ino file.

Once you have an account, substitute the values in the #data.h recompile and upload.

Your values should appear in emoncms/inputs almost immediately.

## BOM

<b>WeMOS D1 Mini</b>  - the newer variants such as the C3 should also work.

TFT 1.4 Shield <- no longer available. The OLED shields will work but probably need some minor code changes

SHT 30 Temp and Humidity Shield

TFT I2C connection shield - Not strictly required but is makes remote connection of the Temp shield possible.

https://lolin.aliexpress.com/store/1331105
