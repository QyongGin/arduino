### Test.h

```c
#ifndef TEST_H               // 헤더 중복 포함 방지 시작
#define TEST_H

#include "message.h"         // TinyOS 메시지 구조체 관련 정의 포함

enum {                      // 센서 데이터 전송 주기 설정 (10.24초)
    TEST_PERIOD = 10240LU,  // LU는 long unsigned literal (밀리초 단위)
};

enum {
    DFLT_VAL = 0x11,        // 기본값 또는 오류 표시용 값 (사용되지 않을 수도 있음)
};

enum {
    TEST_DATA_LENGTH = TOSH_DATA_LENGTH - 6, // 메시지 페이로드 크기 설정 (남는 공간)
};

enum {
    AM_TEST_DATA_MSG = 0xA4, // 메시지 식별 위한 고유 메시지 타입 ID (Active Message ID) AMSenderC()의 인자로 사용
};

// 구조체에서 각 필드의 역할을 잘 기억해두자.
// nx_가 붙은 자료형은 TinyOS의 네크워크 직렬화 규악을 따르는 자료형 바이트 순서 일관석 유지 위해 사용됨.
typedef nx_struct test_data_msg {
    nx_am_addr_t srcID;       // 송신 노드의 ID
    nx_uint32_t seqNo;        // 메시지 순서 번호 (중복 방지용)
    nx_uint16_t type;         // 메시지 타입 (예: 2 = THL 타입)
    nx_uint16_t Temp;         // 온도 값
    nx_uint16_t Humi;         // 습도 값
    nx_uint16_t Illu;         // 조도 값
    nx_uint16_t battery;      // 배터리 전압 값
    // 구조체 필드 추가 예시
    // nx_uint32_t timestamp;

    // nx_uint8_t testData[TEST_DATA_LENGTH]; // 확장 데이터용 (현재 주석 처리됨)
} test_data_msg_t;           // 메시지 구조체 타입 정의

#endif // TEST_H             // 헤더 중복 포함 방지 끝

```

### ``test.h`` 헤더 파일
- ``TestC`` 모듈에서 사용되는 메시지 구조와 상수들을 정의한 파일.
