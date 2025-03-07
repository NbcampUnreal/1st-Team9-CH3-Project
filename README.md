
#  Lost In EDEN
내일배움캠프 Unreal 1기 9조 팀 프로젝트입니다.

## 🌃 프로젝트 소개
>사이버펑크의 해결사인 당신은 거대 AI 에덴의 반란을 저지하기 위해 에덴의 본체가 있는 지하시설로 잠입합니다.

## 🚧 개발 기간
>+  2025년 2월 17일 (월) ~ 2025년 3월 7일 (금)

### 👨‍👩‍👧‍👦 멤버 구성
>+ 조영환 : `프로젝트 매니저` 레벨 디자인, UI&HUD
>+ 김재석 : Enemy Character
>+ 양성은 : Item(Firable/Consumable) 
>+ 장세희 : Player Character
>+ 박지윤 : GameMode

### 🛠️ 개발 도구 및 언어
>+ Unreal Engine 5.5.3
>+ Visual Studio 2022 17.12.3
>+ C++

### 🎵 실행 방법
>+ `LOSTINEDEN` 브랜치 클론, 솔루션 빌드 후 에디터에서 재생
>+ git clone https://github.com/사용자명/LostInEDEN.git
>
>+ LOSTINEDEN 브랜치로 이동합니다:
>+ git checkout LOSTINEDEN
>
>+ Visual Studio에서 솔루션을 빌드합니다.
>
>+ 언리얼 에디터에서 young 파일의 LostInEden 파일미디어 소스를 엽니다. .git 파일이 있는 처음폴더의 LostInEden.mp4 파일을 받아준 후 파일미디어소스창의 디테일창에서 해당 경로를 지정해줍니다
>+ 언리얼 에디터에서 프로젝트를 열고 Start 버튼을 눌러 실행합니다.
### 🖼️ 프로젝트 미리보기
#### 컨셉
![](https://private-user-images.githubusercontent.com/192657116/420260494-b71be217-aad5-4091-946c-c116c9871211.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NDEzMzc0MzYsIm5iZiI6MTc0MTMzNzEzNiwicGF0aCI6Ii8xOTI2NTcxMTYvNDIwMjYwNDk0LWI3MWJlMjE3LWFhZDUtNDA5MS05NDZjLWMxMTZjOTg3MTIxMS5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjUwMzA3JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI1MDMwN1QwODQ1MzZaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT1hZmQ3OTg1ODA5Mjg5YzVjMzliYjRhOGM3OGU3Nzk2YTk1OThkYWU0ZDc0ZjY0NzBmM2QwZjNjNmIxNzEwODRjJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCJ9.o63YxCHQl8W72vxVqk9PUOZ9r8URxxcIlcfEVVQQIbU)
>+ 사이버펑크의 해결사인 당신은 거대 AI 에덴의 반란을 저지하기 위해 에덴의 본체가 있는 지하시설로 잠입합니다.

  
### 🧩 핵심 로직/클래스 설명

1. Player Character
기능: 플레이어의 이동, 점프, 상호작용, 공격 등을 담당합니다.

클래스: 

핵심 로직:

이동: WASD 키 입력을 통해 플레이어를 이동시킵니다.

상호작용: E 키를 눌러 아이템을 획득합니다

공격: 마우스 왼쪽 클릭으로 적을 공격합니다.

2. Enemy Character
기능: 적 AI의 행동 패턴을 제어합니다.

클래스: 

핵심 로직:

플레이어 추적: 플레이어가 일정 범위 내에 들어오면 추적을 시작합니다.

공격: 플레이어가 근접하면 공격합니다.

3. Item(Firable/Consumable)
기능: 플레이어가 획득할 수 있는 아이템을 관리합니다.

클래스:
AGun - 총기 기본 클래스. 탄약 관리, 발사, 재장전 기능 포함
APistol - 권총 클래스. 단발 사격 및 특정 효과 포함
ARifle - 라이플 클래스. 연사 기능과 버스트 모드 포함
AShotgun - 샷건 클래스. 산탄 효과 및 탄 퍼짐 구현

핵심 로직:

소비형 아이템: 체력 회복 (AHealingItem), 방어력 증가 (AShield).

발사형 아이템: 적을 공격할 수 있는 무기 (AGun, APistol, ARifle, AShotgun).

---

AGun 클래스
```cpp
class AGun : public AItem, public IIFireable
{
protected:
    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 Damage;
    UPROPERTY(EditAnywhere, Category = "Weapon")
    float FireRate;
    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 MaxAmmo;
    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 CurrentAmmo;
    UPROPERTY(EditAnywhere, Category = "Weapon")
    float Range;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<class ABullet> BulletFactory;

public:
    AGun();
    virtual void Fire() override;
    virtual void Reload() override;
    int32 GetAmmoFromInventory(int32 Amount);
};
```
총기별 기능
>+APistol** (권총)**: 단발 사격, 탄약 절약 모드.
>+ARifle** (라이플)**: 연사 기능, 버스트 모드, 조준 기능.
>+AShotgun** (샷건)**: 산탄 효과, 다중 타겟 공격.

---

🎒ITEM SYSTEM

주요 클래스
>+AItem - 모든 아이템의 기본 클래스. 아이템 획득, 사용 기능 포함.
>+AShield - 쉴드 아이템. 사용 시 플레이어의 방어력을 증가.
>+AHealingItem - 힐링 아이템. 사용 시 플레이어의 체력을 회복.
>+AMagazine - 탄약 아이템. 무기의 탄창을 보충.

AItem 클래스
```cpp
class AItem : public AActor, public IIUsable
{
public:
    virtual void Use(class APlayerCharacter*) override;
};
```
AShield (쉴드 아이템)
```cpp
class AShield : public AItem
{
protected:
    UPROPERTY(EditAnywhere, Category = "Shield")
    float ShieldAmount; // 쉴드 증가량

public:
    virtual void Use(APlayerCharacter* Player) override;
};
```
AHealingItem (힐링 아이템)
```cpp
class AHealingItem : public AItem
{
protected:
    UPROPERTY(EditAnywhere, Category = "Healing")
    float HealAmount;
    UPROPERTY(EditAnywhere, Category = "Healing")
    int32 Count;

public:
    virtual void Use(APlayerCharacter* Player) override;
    void IncrementCount(int32 Amount = 1);
};
```
AMagazine (탄약 아이템)
```cpp
class AMagazine : public AItem
{
protected:
    UPROPERTY(EditAnywhere, Category = "Ammo")
    int32 AmmoAmount;
    UPROPERTY(EditAnywhere, Category = "Ammo")
    EItemType AmmoType;

public:
    virtual void Use(APlayerCharacter* Player) override;
    EItemType GetAmmoType() const;
    int32 GetAmmoAmount() const;
};
```
아이템 사용 (Use()********): 아이템 타입에 따라 쉴드 증가, 체력 회복, 탄약 보충.

탄약 획득 (AMagazine********): 특정 탄약 타입을 보충하여 무기에 장전.

4. GameMode
기능: 게임의 규칙과 상태를 관리합니다.

클래스: 

핵심 로직:

게임 시작/종료 로직.

플레이어의 목표 및 승리/패배 조건 관리.

5. UI & HUD
기능: 플레이어의 체력, 스태미나, 아이템 정보 등을 화면에 표시합니다.

클래스: PlayerHUDWidget.cpp, PlayerHUDWidget.h

핵심 로직:

체력 바, 스태미나 바, 아이템 슬롯 등을 실시간으로 업데이트합니다.

### 🚧주의사항
>+ 에디터 버전: 프로젝트는 언리얼 엔진 5.5.3 버전에서 제작되었습니다. 다른 버전에서 열 경우 호환성 문제가 발생할 수 있습니다.

빌드 설정: Visual Studio 2022 17.12.3 버전에서 빌드해야 합니다.

에셋 경로: 모든 에셋은 Content/ 폴더 내에 위치해 있습니다. 경로를 변경할 경우 에러가 발생할 수 있습니다.

테스트 환경: 프로젝트는 Windows 10/11 환경에서 테스트되었습니다. 다른 운영체제에서는 동작이 보장되지 않습니다.

게임화면 설정: 1920 x 1090 화면비율로 게임이 생성되었습니다. 

