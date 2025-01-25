#define APARRAY {"shed","HackDesk"}
#define PASSARRAY {"BigFurryCat","BigFurryCat"}
#define APCOUNT 2
#define NODENAME "Shed"
#define CONNECTOR_100
#define SENSORCOUNT 2
#define DHT
#define IOTAWATT
#define IW_VALUE "http://192.168.1.100/query?select=[time.local.iso,PoolCircuit.watts]&begin=m-1m&end=m&group=m&format=csv"
#define IW_SOLAR "http://192.168.1.100/query?select=[time.local.iso,Solar.watts]&begin=m-1m&end=m&group=m&format=csv"
#warning Shed 
