# 🚗 STM32H503RB - FDCAN 송수신 테스트

이 프로젝트는 STM32H503RB 보드를 사용하여 FDCAN 통신의 송수신 기능을 테스트한 코드입니다.  
FDCAN1 인터페이스를 통해 버튼 입력으로 CAN 메시지를 전송하고, 수신된 데이터에 따라 LED를 제어합니다.

---

## 🔧 개발 환경

- **MCU**: STM32H503RB (NUCLEO-H503RB 보드 기준)
- **IDE**: STM32CubeIDE
- **Driver**: HAL (stm32h5xx_hal_fdcan.c)
- **CAN 모듈**: FDCAN1

---

## 📌 주요 구현 기능

| 기능 | 설명 |
|------|------|
| FDCAN 송신 | 버튼(PB0) 누르면 FDCAN 메시지 전송 |
| FDCAN 수신 | 수신 메시지 데이터에 따라 LED ON/OFF |
| 인터럽트 설정 | 수신 시 인터럽트 기반 처리 |
| 필터 설정 | 표준 ID 수신 필터 등록 |

---

## 📎 사용 핀 구성

| 핀 이름 | 기능 | 연결 대상 |
|---------|------|-----------|
| **PB10** | FDCAN1_TX | 외부 CAN 트랜시버 (TX) |
| **PB12** | FDCAN1_RX | 외부 CAN 트랜시버 (RX) |
| **PC2**  | 버튼 (EXTI) | 송신 트리거 버튼 |
| **PA5**  | LED | 수신 값이 0xAA일 경우 점등 |
| **PC8**  | LED | 수신 값이 0x55일 경우 점등 |

---

## 🧪 테스트 방식

1. 외부 트랜시버를 통해 다른 CAN 장치와 연결
2. 버튼(PB0) 누르면 0x321 ID로 `0xAA` 또는 `0x55` 전송
3. 수신된 데이터에 따라 LED 점등 조건 분기

---

## 💡 참고 사항

- 필터는 표준 ID 0x321로 설정되어 있음
- DWT Delay는 사용하지 않음
- 수신/송신 결과는 LED로 확인 가능

---

## 🔍 관련 문서

- [`CAN 통신 정리 문서 (H503RB)`](정리.docx)

