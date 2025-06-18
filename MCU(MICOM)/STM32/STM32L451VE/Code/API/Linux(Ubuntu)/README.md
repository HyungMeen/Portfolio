### 📡 UART 명령 기반 API 문서 (Ubuntu ↔ STM32)

이 문서는 Ubuntu 환경에서 STM32로 UART 명령을 전송하여  
온도, 기울기, 습도 데이터를 요청하거나 기준값을 설정하는 명령어 구조를 정리한 문서입니다.

---

### 🟢 현재 측정값 요청 (get 명령)

```bash
sudo [파일명] get temp acc rh
```

| 파라미터 | 설명 |
|----------|------|
| temp     | 현재 온도 (DHT11 센서 측정값) |
| acc      | 현재 기울기 (LSM6DSM 센서 기반 Roll/Pitch 평균값) |
| rh       | 현재 습도 (DHT11 센서 측정값) |

📸 **실행 결과 예시:**

![get result](https://github.com/user-attachments/assets/6a119071-4710-43a6-8801-f9576565d7dc)

---

### 🟡 기준값 설정 (set 명령)

```bash
sudo [파일명] set temp rh
```

| 파라미터 | 설명 |
|----------|------|
| temp     | 기준 온도 설정 (초과 시 LED 점등 등 동작 수행) |
| rh       | 기준 습도 설정 (초과 시 LED 점등 등 동작 수행) |

📸 **실행 결과 예시:**

![set result](https://github.com/user-attachments/assets/f7961052-7eb5-4b00-b52b-2e7a402a825a)

---

### 🧠 시스템 흐름도

아래는 STM32에서 get/set 명령을 처리하는 전체 동작 구조입니다:

![Flowchart_stm32](https://github.com/user-attachments/assets/19b13c28-3689-4364-9632-b105a0cbc76d)

---

### ✅ 요약

- Ubuntu에서 `sudo` 명령을 통해 STM32로 UART 명령 전송  
- STM32는 명령 해석 후 센서 데이터 응답 또는 설정값 반영  
- UART 기반의 센서 제어, 기준값 설정, 상태 모니터링을 하나의 흐름으로 구성  
