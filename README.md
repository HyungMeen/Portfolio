## 📁 HyungMeen's Embedded Portfolio

안녕하세요, 임베디드 시스템 개발자 박형민입니다.  
이 리포지토리는 제가 실제로 다룬 MCU/칩 기반의 프로젝트들을 정리한 포트폴리오입니다.  
각 폴더에 기능 테스트 코드가 있으며, 내부 `README.md`에서 구현 내용을 확인할 수 있습니다.

---

### 📌 MCU별 프로젝트 분류

#### 🔷 MEC1521

| 프로젝트명 | 설명 | 링크 |
|------------|------|------|
| UART Logger | MEC1521의 UART를 사용하여 외부 장치 디버깅 및 로그 수집 |
| GPIO Toggle | MEC1521의 GPIO를 주기적으로 토글하여 LED 점멸 확인 |

---

#### 🔷 ATmega128A

| 프로젝트명 | 설명 | 링크 |
|------------|------|------|
| Smart Fan Controller | 온도 센서를 기반으로 팬을 자동 제어 (PWM 제어) |
| UART 7-Segment Display | UART 수신값을 7세그먼트에 표시 |

---

#### 🔷 STM32L451VE

| 하위 프로젝트 | 구현한 기능 요약 |
|----------------|------------------|
| sensor_system | - DHT11 온습도 센서 제어<br>- LSM6DSM 가속도 센서 연동<br>- UART 명령어(`get`, `set`) 처리<br>- 기준 온/습도 설정 + LED 제어<br>- TIM6/EXTI/DWT 타이머 사용 |
| can_test | - CAN 통신 송수신 테스트<br>- EXTI 인터럽트 버튼으로 송신<br>- 수신 데이터 확인 후 GPIO 제어 |

🔗 자세한 구현 내용:
- [sensor_system](./STM32L451VE/sensor_system)
- [can_test](./STM32L451VE/can_test)

---

#### 📁 폴더 구성 예시

- `MEC1521/` : MEC1521 기반 Zephyr 테스트 코드  
- `ATmega128A/` : Atmel Studio 기반 AVR 코드  
- `STM32L451VE/` : STM32CubeIDE 기반 HAL 프로젝트, 하위에 `/can_test`, '/sensor_system 기능별 폴더 구성
