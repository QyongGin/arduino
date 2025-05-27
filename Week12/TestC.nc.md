### TestC.nc
```
  1 module TestC
  2 {
  3         uses {
  4                 interface Boot;
  5                 interface Leds;
  6                 interface Timer<TMilli> as MilliTimer;
  7
  8                 interface SplitControl as RadioControl;
  9                 interface AMSend as RadioSend;
 10
 11                 interface Read<uint16_t> as Temp;
 12                 interface Read<uint16_t> as Humi;
 13                 interface Read<uint16_t> as Illu;
 14
 15                 interface Battery;
 16             }
 17 }
 18
 19 implementation
 20 {
 21         message_t testMsgBffr;
 22         test_data_msg_t *testMsg;
 23
 24         uint32_t seqNo;
 25         uint8_t step;
 26
 27
 28         task void startTimer();
 29         event void Boot.booted() {
 30                 testMsg = (test_data_msg_t *) call RadioSend.getPayload(
 31                                 &testMsgBffr, sizeof(test_data_msg_t));
 32                 testMsg->srcID = TOS_NODE_ID;
 33
 34                 seqNo = 0;
 35
 36                 post startTimer();
 37             }
 38
 39         task void startTimer() {
 40                 call MilliTimer.startPeriodic(TEST_PERIOD);
 41         }
 42
 43         task void radioOn();
 44         event void MilliTimer.fired() {
 45                 post radioOn();
 46         }
 47
 48         void startDone();
 49         task void radioOn() {
 50                 if (call RadioControl.start() != SUCCESS) startDone();
 51         }
 52
 53         event void RadioControl.startDone(error_t error) {
 54                 startDone();
 55         }
 56
 57         task void readTask();
 58         void startDone() {
 59                 step = 0;
 60                 post readTask();
 61                 call Leds.led0Toggle();
 62             }
 63
 64         void sendDone();
 65         task void sendTask() {
 66                 testMsg->seqNo = seqNo++;
 67                 testMsg->type = 2; //THL type 2
 68
 69                 if (call RadioSend.send(AM_BROADCAST_ADDR, &testMsgBffr,
 70                     sizeof(test_data_msg_t)) != SUCCESS) sendDone();
 71                 call Leds.led2Toggle();
 72         }
 73
 74         event void RadioSend.sendDone(message_t* msg, error_t error) {
 75                 sendDone();
 76         }
 77
 78         task void radioOff();
 79         void sendDone() {
 80                 call Leds.led0Off();
 81                 call Leds.led1Off();
 82                 call Leds.led2Off();
 83                 post radioOff();
 84         }
 85
 86         void stopDone();
 87         task void radioOff() {
 88                 if (call RadioControl.stop() != SUCCESS) stopDone();
 89         }
 90
 91         event void RadioControl.stopDone(error_t error) {
 92                 stopDone();
 93         }
 94
 95         void stopDone() {
 96         }
 97         task void readTask() {
 98                 switch(step) {
 99                         case 0:
100                                 call Temp.read(); break;
101                         case 1:
102                                 call Humi.read(); break;
103                         case 2:
104                                 call Illu.read(); break;
105                         default:
106                                 testMsg->battery = call Battery.getVoltage();
107                                 post sendTask();
108                                 break;
109                 }
110                 step += 1;
111         }
112
113         event void Temp.readDone(error_t error, uint16_t val) {
114                 //if (error != SUCCESS) call Leds.led0On();
115                 testMsg->Temp = error == SUCCESS ? val : 0xFFFA;
116                 post readTask();
117         }
118         event void Humi.readDone(error_t error, uint16_t val) {
119                 //if (error != SUCCESS) call Leds.led1On();
120                 testMsg->Humi = error == SUCCESS ? val : 0xFFFB;
121                 post readTask();
122         }
```
123         event void Illu.readDone(error_t error, uint16_t val){
124                 testMsg->Illu = error == SUCCESS ? val : 0xFFFC;
125                 post readTask();
126         }
127 }
