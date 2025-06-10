### TestC.nc
```
module TestC
{
    // 사용(uses)하는 인터페이스들을 정의
    
    uses {
        interface Boot;                          // 시스템 부팅 이벤트 인터페이스
        interface Leds;                          // LED 제어 인터페이스
        interface Timer<TMilli> as MilliTimer;   // 밀리초 단위 주기 타이머

        interface SplitControl as RadioControl;  // 라디오 모듈 제어 (시작/중지)
        interface AMSend as RadioSend;           // Active Message 송신 인터페이스

        interface Read<uint16_t> as Temp;        // 온도 센서 인터페이스
        interface Read<uint16_t> as Humi;        // 습도 센서 인터페이스
        interface Read<uint16_t> as Illu;        // 조도 센서 인터페이스

        interface Battery;                       // 배터리 전압 측정 인터페이스
    }
}


implementation
{
    message_t testMsgBffr;  // 메시지 버퍼 변수 선언
    test_data_msg_t *testMsg;  // 메시지 포인터 (사용자 정의 메시지 구조체를 가리킴)

    uint32_t seqNo;     // 메시지 순번
    uint8_t step;       // 센서 읽기 단계 (0: Temp, 1: Humi, 2: Illu, 3: Battery)

    // 타이머 시작을 위한 비동기 작업(task)
    task void startTimer();

    // 시스템이 부팅되면 호출됨
    event void Boot.booted() {
        testMsg = (test_data_msg_t *) call RadioSend.getPayload(
                        &testMsgBffr, sizeof(test_data_msg_t)); // 메시지 payload 메모리 할당 ( 메시지 버퍼 초기화)
        testMsg->srcID = TOS_NODE_ID;  // 현재 노드 ID 설정

        seqNo = 0;  // 초기 순번 설정

        post startTimer(); // 주기 타이머를 시작하기 위한 타이머 시작 task 호출
    }

    // 타이머 시작
    task void startTimer() {
        call MilliTimer.startPeriodic(TEST_PERIOD); // 주기적으로 타이머 이벤트 발생
    }

    // 라디오 시작을 위한 task
    task void radioOn();

    // 타이머 이벤트 발생 시 라디오 시작 task 호출
    event void MilliTimer.fired() {
        post radioOn(); // 라디오 모듈 시작 이후 센서 읽기 및 데이터 전송 작업 
    }

    // 라디오 시작 요청
    void startDone();

    task void radioOn() {
        if (call RadioControl.start() != SUCCESS) startDone(); // 시작 실패시 직접 처리
    }

    // 라디오 시작 완료 이벤트
    event void RadioControl.startDone(error_t error) {
        startDone();
    }

    // 센서 읽기 task를 시작
    task void readTask();

    // 라디오 시작 완료 처리
    void startDone() {
        step = 0;              // 센서 읽기 순서 초기화
        post readTask();       // 센서 읽기 task 호출
        call Leds.led0Toggle(); // LED0 상태 반전
    }

    // 메시지 전송 완료 후 처리 함수
    void sendDone();

    // 무선 메시지 송신 task
    task void sendTask() {
        testMsg->seqNo = seqNo++; // 순번 증가
        testMsg->type = 2;        // 메시지 타입 설정 (2: THL - Temp/Humi/Light)

        // 메시지 전송 요청
        if (call RadioSend.send(AM_BROADCAST_ADDR, &testMsgBffr,
            sizeof(test_data_msg_t)) != SUCCESS) sendDone(); // 실패 시 수동 완료 처리
        call Leds.led2Toggle(); // LED2 상태 반전
    }

    // 메시지 송신 완료 이벤트
    event void RadioSend.sendDone(message_t* msg, error_t error) {
        sendDone();
    }

    // 라디오 종료를 위한 task
    task void radioOff();

    // 메시지 송신 완료 처리 함수
    void sendDone() {
        call Leds.led0Off();  // LED0 끔
        call Leds.led1Off();  // LED1 끔
        call Leds.led2Off();  // LED2 끔
        post radioOff();      // 라디오 종료 task 호출
    }

    // 라디오 중지 완료 처리 함수
    void stopDone();

    // 라디오 종료 task
    task void radioOff() {
        if (call RadioControl.stop() != SUCCESS) stopDone(); // 실패 시 직접 호출
    }

    // 라디오 종료 완료 이벤트
    event void RadioControl.stopDone(error_t error) {
        stopDone();
    }

    // 라디오 종료 후 추가 작업 없음 (비워둠)
    void stopDone() {
    }

    // 센서 데이터 읽기 task
    task void readTask() {
        switch(step) {
            case 0:
                call Temp.read(); break;   // 온도 센서 읽기
            case 1:
                call Humi.read(); break;   // 습도 센서 읽기
            case 2:
                call Illu.read(); break;   // 조도 센서 읽기
            default:
                testMsg->battery = call Battery.getVoltage(); // 배터리 전압 측정
                post sendTask();          // 송신 task 호출
                break;
        }
        step += 1; // 다음 단계로 이동
    }

    // 온도 센서 읽기 완료 이벤트
    event void Temp.readDone(error_t error, uint16_t val) {
        testMsg->Temp = error == SUCCESS ? val : 0xFFFA; // 성공 시 측정값을 testMsg에 저장, 에러시 특수값 0xFFFA를 저장 
        post readTask(); // 다음 센서 읽기
    }

    // 습도 센서 읽기 완료 이벤트
    event void Humi.readDone(error_t error, uint16_t val) {
        testMsg->Humi = error == SUCCESS ? val : 0xFFFB;
        post readTask();
    }

    // 조도 센서 읽기 완료 이벤트
    event void Illu.readDone(error_t error, uint16_t val){
        testMsg->Illu = error == SUCCESS ? val : 0xFFFC;
        post readTask();
    }

  // 기능 추가 예시
  // 조도 값이 특정 값보다 낮으면 LED1 키고 크면 LED!을 끈다.
  // event void Illu.readDone(error_t error, uint16_t val){
  //   testMsg->Illu = error == SUCCESS ? val : 0xFFFC;

    // if (error == SUCCESS && val < 300) {
    //  call Leds.led1On();
    // } else {
    //  call Leds.led1Off();
    // }

    // post readTask();
// }

}

```
### 역할
- TinyOS에서 센서 데이터를 주기적으로 읽고 무선으로 송신하는 기능을 수행한다.

### 정리, 모듈의 동작 흐름
1. 시스템 부팅 -> ``Boot.booted()`` 호출
2. 메시지 버퍼 초기화 -> 주기적 타이머 시작
3. 타이머 발생 시 -> 라디오 ON -> 센서 순차 읽기 ( 온도 -> 습도 -> 조도(빛의 세기) -> 배터리)
4. 데이터 준비 후 -> 무선 송신 -> 라디오 OFF

### TinyOS에서 ``task``와 ``event``의 차이점
- ``task``: 백그라운드에서 실행된다. 선점되지 않는다. 예 : ``sendTask``, ``readTask``, ``radioOn``등
- ``event``: 하드웨어나 시스템에서 발생한 비동기적 신호에 의해 호출된다. 예 : ``Boot.booted()``, ``Timer.fired()``,``RadioSend.sendDone()``등

