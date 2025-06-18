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

#### 🔷 STM32F103

STM32 HAL을 기반으로 다양한 기능을 하나의 소스에 통합하여 테스트한 프로젝트입니다.

| 기능 | 설명 | 링크 |
|------|------|------|
| 온습도 센서 제어 | DHT11 센서를 직접 제어하여 온도/습도 수집 | [API](./STM32/API) |
| 가속도 센서 | LSM6DSM을 통해 Roll/Pitch 계산 및 LED 제어 | [API](./STM32/API) |
| UART 명령 인터페이스 | Ubuntu에서 `sudo` 명령으로 get/set 기능 수행 | [API](./STM32/API) |
| 인터럽트 처리 | EXTI를 통한 버튼 이벤트 및 TIM6 기반 주기 제어 | [API](./STM32/API) |

> 📎 자세한 명령 구조와 흐름도는 [STM32/API](./STM32/API) 폴더 내 `README.md` 참고

---

#### 📁 폴더 구성 예시

- `MEC1521/` : MEC1521 기반 Zephyr 테스트 코드  
- `ATmega128A/` : Atmel Studio 기반 AVR 코드  
- `STM32/` : STM32CubeIDE 기반 HAL 프로젝트, 하위에 `/API` 등 기능별 폴더 구성
