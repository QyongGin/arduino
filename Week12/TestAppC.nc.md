### TestAppC.nc

```c
 // Test 모듈을 포함한다 (TestC 모듈 사용)
includes Test;

configuration TestAppC
{
}

implementation
{
    // 애플리케이션에서 사용할 컴포넌트 정의
    components TestC, MainC;                 // Test 모듈과 시스템 초기화를 위한 Main 모듈
    components LedsC, new TimerMilliC();     // LED 제어와 1ms 단위 타이머 사용

    // 무선 통신 관련 컴포넌트
    components ActiveMessageC as AMC;        // Active Message (무선 패킷 전송) 기본 컴포넌트
    components new AMSenderC(AM_TEST_DATA_MSG) as AMSC; // 지정된 메시지 타입으로 송신하기 위한 컴포넌트

    // 부트 시퀀스 연결
    TestC.Boot -> MainC;                     // 시스템 부팅 시 TestC의 Boot 인터페이스가 MainC에 연결됨
    TestC.Leds -> LedsC;                     // LED 제어를 위한 인터페이스 연결, 상태 표시용으로 LED 사용 
    TestC.MilliTimer -> TimerMilliC;         // millisecond 단위 타이머 연결, 주기적인 센서 데이터 수집 등을 위해 TimerMilliC 사용 

    // 무선 통신 인터페이스 연결, AMSenderC, ActiveMessageC 를 통해 데이터 송신 
    TestC.RadioControl -> AMC;              // 라디오 (무선 통신) 제어 인터페이스 연결
    TestC.RadioSend -> AMSC;                // 무선 송신 인터페이스를 AMSender에 연결

    // 온습도 센서 컴포넌트 연결 (Sensirion SHT11 센서 사용)
    components new SensirionSht11C() as Sht11Ch0C; // 온습도 센서를 사용하는 컴포넌트 생성
    TestC.Temp -> Sht11Ch0C.Temperature;    // 온도 측정 인터페이스 연결
    TestC.Humi -> Sht11Ch0C.Humidity;       // 습도 측정 인터페이스 연결

    // 조도 센서 컴포넌트 연결
    components new IlluAdcC() as Illu;       // 조도 센서 (ADC 기반) 컴포넌트 생성, IlluAdcC를 통해 빛의 세기 측정
    TestC.Illu -> Illu;                      // TestC에서 조도 측정 인터페이스 연결

    // 배터리 측정 컴포넌트 연결
    components BatteryC;                     // 배터리 전압 측정 컴포넌트, BatteryC를 통해 배터리 전압 측정 
    TestC.Battery -> BatteryC;               // 배터리 인터페이스 연결
}

```
### 역할
- 실제 애플리케이션의 구성도(설계도) 역할을 한다.
- 어떤 컴포넌트들이 사용되는지 각각 어떻게 연결되는지 정의한다.
- 각각의 연결은 모듈(TestC)안에서 정의한 인터페이스와 해당 기능을 제공하는 컴포넌트를 연결하는 방식.
