# 🔌 ATmega128 USB 전원 제어 시스템

ATmega128을 사용하여 USB 포트의 전원을 제어하는 프로젝트입니다.  
TWI 통신을 통해 상위 MCU로부터 명령을 받아 전원 상태를 제어하며, 타이머와 외부 인터럽트를 활용합니다.

---

## 📌 주요 기능

| 기능 | 설명 |
|------|------|
| TWI Slave 동작 | 상위 마스터(MCU)로부터 명령 수신 |
| USB Power ON/OFF | 수신 명령에 따라 GPIO를 통해 USB 전원 제어 |
| 타이머 기반 상태 처리 | 타이머로 상태를 주기적으로 확인 및 동작 |
| 외부 인터럽트 | 특정 이벤트 발생 시 인터럽트 처리 루틴 수행 |

---

## ⚙️ 개발 환경

- MCU: ATmega128
- 언어: C (AVR-GCC)
- 개발 환경: Microchip Studio
- 인터페이스: I2C(TWI), GPIO, Timer, External Interrupt

---

## 📂 주요 파일

| 파일명 | 설명 |
|--------|------|
| `main.c` | 전체 시스템 제어 루틴 포함 |
| `twi_slave_init()` | TWI 슬레이브 초기화 |
| `TIMER0_Init()` | 타이머 초기화 |
| `EXT_INT0_Init()` | 외부 인터럽트 설정 |

---
