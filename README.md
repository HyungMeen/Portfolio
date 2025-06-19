## 📁 HyungMin's Embedded Portfolio

안녕하세요, 임베디드 시스템 개발자 **박형민**입니다.  
이 리포지토리는 제가 실제로 다룬 MCU 기반 프로젝트들을 정리한 포트폴리오입니다.  
각 폴더에는 테스트 코드가 포함되어 있으며, 세부 내용은 각 폴더 `README.md`에서 확인할 수 있습니다.

---

## 🕒 프로젝트 진행 타임라인

| 📆 시점          | 🧠 MCU / 프로젝트           |
|------------------|-----------------------------|
| 2023.11 ~ 2023.12 | `STM32L451VE / sensor_system` |
| 2024.01 ~ 2024.03 | `ATmega128A / USB_PowerControl` |
| 2024.01 ~ 2024.05 | `IT8518`                      |
| 2024.02 ~ 2024.03 | `STM32L451VE / can_test`      |
| 2024.02 ~ 2024.03 | `STM32H503RB / fdcan_test`    |
| 2024.10 ~ 2024.12 | `ATmega1280`                  |
| 2025.01 ~ 2025.04 | `ATmega128A / segment_display` |
| 2024.06 ~ 현재    | `MEC1521`                     |
| 2025.05 ~ 현재    | `MEC1723`                     |

---

## 🔷 MCU별 프로젝트 정리

---

### 🟦 ATmega128A

| 카테고리       | 구현한 기능 |
|----------------|-------------|
| I2C 슬레이브   | 마스터로부터 전압 수신 후 릴레이 제어 |
| 세그먼트 표시  | UART 수신값을 2채널(3자리씩) 7세그먼트에 출력 |

🔗 세부 폴더:
- [USB_PowerControl](./ATmega128A/USB_PowerControl)
- [segment_display](./ATmega128A/segment_display)

📁 폴더: `ATmega128A/`  
개발환경: Microchip Studio (코드 작성) / AVR Studio 4 + USBasp (업로드)

---

### 🟦 ATmega1280

| 카테고리             | 구현한 기능 |
|----------------------|-------------|
| I2C 수신 + 디스플레이 | 6채널 전압 수신 후 7세그먼트에 멀티플렉싱 출력 |

🔗 세부 폴더:
- [Atmega1280](./ATmega1280)
  
📁 폴더: `ATmega1280/`  
개발환경: Microchip Studio (코드 작성) / AVR Studio 4 + USBasp (업로드)

---

### 🔷 IT8518

| 구현 항목 | 설명 |
|-----------|------|
| EC GPIO 제어 | InpOut32.dll을 사용하여 GPIO 직접 On/Off |
| PWM 제어 | 레지스터 접근을 통한 PWM Duty 조절 |
| Port80 Read | BIOS POST 코드 수신 및 표시 |
| EC RAM 제어 | SIO 레지스터를 통해 EC RAM 데이터 읽기/쓰기 |
| 개발 환경 | Windows 7 + Legacy ITE SDK + 직접 제작한 InpOut32.dll |

🔗 세부 폴더:  
- [IT8518](./IT8518)

📁 폴더: `IT8518/`  
개발환경: Visual Studio + Windows 7

---

### 🟦 STM32L451VE

| 카테고리          | 구현한 기능                                              |
|-------------------|-----------------------------------------------------------|
| 센서 제어          | DHT11, LSM6DSM 가속도 센서 연동                           |
| 시스템 제어        | 기준 온/습도 설정 및 조건 제어                            |
| UART 명령 인터페이스 | `get`, `set` 명령 처리                                    |
| 타이밍/인터럽트     | TIM6, EXTI 버튼, DWT Timer 활용                           |
| CAN 통신           | bxCAN 송수신 및 필터링 테스트                             |

🔗 세부 폴더:
- [sensor_system](./STM32L451VE/sensor_system)
- [can_test](./STM32L451VE/can_test)

📁 폴더: `STM32L451VE/`  
개발환경: STM32CubeIDE + HAL

---

### 🟦 STM32H503RB

| 기능       | 설명                                 |
|------------|--------------------------------------|
| FDCAN      | TX/RX 송수신, 필터 설정, LED 인터럽트 연동 |

🔗 세부 폴더:
- [STM32H503RB](./STM32H503RB)

📁 폴더: `STM32H503RB/`  
개발환경: STM32CubeIDE + HAL

---

### 🟦 MEC1521

| 프로젝트명      | 설명                                      |
|----------------|-------------------------------------------|
| UART Logger     | UART로 외부 장치 로그 수집 및 디버깅             |
| GPIO Toggle     | 주기적인 GPIO 토글로 LED 제어 테스트            |

📁 폴더: `MEC1521/`  
프레임워크: Zephyr RTOS

---

### 🟦 MEC1723

| 기능     | 설명                                  |
|----------|---------------------------------------|
| SPI/ROM  | Dediprog 기반 ROM 플래시 이미지 작업      |
| GPIO/UART| MEC1723의 기본 주변장치 테스트 진행 중     |

📁 폴더: `MEC1723/`  
프레임워크: Zephyr RTOS

---

### 📁 폴더 구조 예시

```
Portfolio/
├── ATmega128A/
├── STM32L451VE/
│   ├── can_test/
│   └── sensor_system/
├── STM32H503RB/
├── MEC1521/
├── MEC1723/
└── README.md
```

