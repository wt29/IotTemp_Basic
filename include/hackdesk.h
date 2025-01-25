#define NODENAME "HackDesk"
#define APARRAY {"chapman","HackDesk"}
#define PASSARRAY {"BigFurryCat","BigFurryCat","BigFurryCat"}
#define APCOUNT 2
#define SENSORCOUNT 2
#define SHT
#define IOTAWATT
#define IW_VALUE "http://192.168.1.100/query?select=[time.local.iso,Mains.watts]&begin=m-1m&end=m&group=m&format=csv"
#define IW_SOLAR "http://192.168.1.100/query?select=[time.local.iso,Solar.watts]&begin=m-1m&end=m&group=m&format=csv"
