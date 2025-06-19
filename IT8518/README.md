# 🧠 IT8518 EC 제어 프로젝트 (비공개 소스 기반)

ITE IT8518 Embedded Controller(EC)를 분석하고,  
LPC 인터페이스를 통해 다양한 하드웨어 기능을 직접 제어한 프로젝트입니다.

> 🔐 본 프로젝트는 소스코드 보안상 공개하지 않습니다.

---

## 🔧 주요 구현 기능

| 기능 | 설명 |
|------|------|
| 🔌 **LPC 인터페이스 제어** | 0x2E/0x2F, 0x4E/0x4F, 0x6E/0x6F 포트를 통해 EC Bank/Index 접근 |
| 💡 **GPIO 제어** | LED, FAN 등의 하드웨어를 EC GPIO 레지스터를 통해 직접 On/Off |
| ⚙️ **PWM 설정** | 레지스터 기반 PWM Duty 조절 (예: FAN 속도 제어) |
| 🧠 **EC RAM 제어** | SIO 포트를 통해 EC RAM 주소 접근 및 Read/Write |
| 📦 **InpOut32 직접 제작** | LPC 접근용 `InpOut32.dll`을 직접 Visual Studio로 빌드하여 사용 |

---

## ⚙️ 개발 환경

| 항목 | 내용 |
|------|------|
| 💻 개발 OS | Windows 11 |
| 🛠️ 빌드 환경 | Windows 7 (레거시 SDK 및 드라이버 호환성 때문) |
| 언어/툴 | C, Visual Studio |
| 라이브러리 | 직접 제작한 `InpOut32.dll` |
| 대상 칩셋 | ITE IT8518 |
| 접근 방식 | LPC (Low Pin Count) 인터페이스

