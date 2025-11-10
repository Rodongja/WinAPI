# 2D 게임 엔진 샘플 프로젝트

C++ 기반 2D 게임 엔진 샘플 프로젝트입니다.  
객체, 물리, AI, 이벤트 시스템을 포함한 구조로 되어 있습니다.

---

## 프로젝트 개요

- 게임 객체(`CObject`)와 몬스터(`CMonster`) 생성
- AI 상태 기반 행동 (`IDLE`, `PATROL`, `TRACE`)
- 물리 (`CRigidBody`) 및 중력
- 이벤트 기반 객체 관리 (`CREATE_OBJECT`, `DELETE_OBJECT`, `CHANGE_AI_STATE`, `SCENE_CHANGE`)
- 씬 관리 (`CCore`, `CScene`)

---

## 주요 클래스

| 클래스 | 설명 |
|--------|------|
| `CObject` | 모든 게임 객체의 기본 클래스 |
| `CMonster` | 몬스터 객체, AI 상태 관리 |
| `AI` | 상태 기반 AI 관리 |
| `CState` | AI 상태 추상 클래스 (`IDLE`, `PATROL`, `TRACE`) |
| `CRigidBody` | 물리 시뮬레이션 |
| `CGravity` | 중력 적용 |
| `CMonFactory` | 몬스터 생성 전용 팩토리 |
| `SelectGDI` | GDI 객체 선택/복원 관리 |

---

## 이벤트

| 이벤트 | 설명 |
|--------|------|
| `CREATE_OBJECT` | 객체 생성 |
| `DELETE_OBJECT` | 객체 삭제 |
| `SCENE_CHANGE` | 씬 전환 |
| `CHANGE_AI_STATE` | AI 상태 전환 |

---

## 유틸리티 함수

- `Safe_Delete_Vec` / `Safe_Delete_Map` : 포인터 안전 삭제
- `SaveWString` / `LoadWString` : wstring 파일 저장/로드
- `CreateObject`, `DeleteObject`, `ChangeScene`, `ChageAIState` : 이벤트 등록

---

## 사용 예제

### 몬스터 생성
```cpp
CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, Vec2(500, 300));
CreateObject(pMon, GROUP_TYPE::MONSTER);
