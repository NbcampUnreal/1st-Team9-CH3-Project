
#  Lost In EDEN
내일배움캠프 Unreal 1기 9조 팀 프로젝트입니다.

## 🌃 프로젝트 소개
> 사이버펑크 테마의 3인칭 슈터 게임입니다. 적을 물리치며 앞으로 나아가, 보스를 물리치세요!

## 🚧 개발 기간
>+  2025년 2월 17일 (월) ~ 2025년 3월 7일 (금)

### 👨‍👩‍👧‍👦 멤버 구성
>+ 조영환 : `프로젝트 매니저` 레벨 디자인, UI&HUD
>+ 김재석 : Enemy Character
>+ 양성은 : Item(Firable/Usable) 
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

🎮 기능
- 플레이어의 이동, 점프, 상호작용, 공격 등을 담당합니다.
- 쉴드 시스템 및 데미지 처리, 총기 및 탄약 관리 등 다양한 기능을 제공합니다.

🏷️ 클래스: `APlayerCharacter`, `GunManager`

핵심 로직

---

🏃‍♂️ **이동**
- **기능**: WASD 키 입력을 통해 플레이어를 이동시킵니다.
- **설명**: 플레이어는 WASD 또는 방향키를 사용하여 이동할 수 있습니다. `APlayerCharacter` 클래스는 `Move` 메소드를 통해 이동을 처리합니다.

---

🏃‍♂️ **달리기**

기능: 왼쪽 쉬프트 버튼을 눌러 플레이어가 달리도록 합니다.
설명: 플레이어는 왼쪽 쉬프트 버튼을 눌러 달리기를 시작할 수 있습니다. 달리기 중에는 이동 속도가 증가하여 빠르게 이동할 수 있습니다. APlayerCharacter 클래스는 StartSprint()와 StopSprint() 메소드를 통해 달리기 상태를 관리합니다.

---

🧘‍♂️ **앉기**

- **기능**: `C` 키를 눌러 플레이어가 앉는 동작을 수행합니다.
- **설명**: 플레이어는 `C` 키를 눌러 앉을 수 있으며, 앉기 상태에서는 이동 속도가 감소합니다. `APlayerCharacter` 클래스에서 `DoCrouch` 메소드를 통해 앉기와 일어나는 동작을 처리합니다.

---

🦘 **점프**
- **기능**: 스페이스바를 눌러 플레이어가 점프할 수 있습니다.
- **설명**: 플레이어는 스페이스바를 눌러 점프할 수 있으며, `Jump()`와 `StopJumping()` 메소드를 통해 점프의 시작과 중지를 관리합니다.

---

🤝 **상호작용**
- **기능**: `E` 키를 눌러 근처 아이템을 획득합니다.
- **설명**: 플레이어는 `E` 키를 눌러 근처의 아이템을 획득하고, 해당 아이템을 인벤토리에 추가하거나 사용합니다.

---

⚔️ **공격**
- **기능**: 마우스 왼쪽 클릭으로 적을 공격합니다.
- **설명**: 마우스 왼쪽 클릭을 통해 무기를 사용하여 적을 공격할 수 있습니다. 플레이어가 장착한 무기에 따라 공격 방식이 달라집니다.
>+ 권총, 샷건 : 단발 발사
>+ 소총 : 연발 발사

---

🔄 **무기 변경**
- **기능**: 마우스 휠을 사용하여 무기를 변경할 수 있습니다.
- **설명**: 플레이어는 마우스 휠을 통해 장착된 무기를 빠르게 변경할 수 있습니다. `SelectWeapon()` 메소드를 통해 총기 목록을 순차적으로 변경하고, 새로운 무기를 장착합니다.

---

⚙️ 재장전

- **기능**: `R` 키를 눌러 현재 장착된 총기의 탄약을 재장전합니다.
- **설명**: 플레이어는 `R` 키를 눌러 장착한 총기의 탄약을 재장전할 수 있습니다. 재장전은 탄약이 부족하거나 총기가 장전되지 않았을 때 실행되며, `APlayerCharacter` 클래스의 `ReloadAmmo` 메소드에서 처리됩니다.

---

💊 **체력 회복**
- **기능**: 숫자키 1번을 눌러 체력을 회복할 수 있습니다.
- **설명**: 플레이어는 숫자키 1번을 눌러 체력을 회복할 수 있습니다. `UseItem()` 메소드로 체력 회복 아이템을 사용하고, 플레이어의 체력을 증가시킵니다.

---

🛡️ **쉴드 시스템**
- **기능**: 플레이어는 쉴드를 사용하여 피해를 줄일 수 있습니다.
- **설명**: 쉴드는 플레이어에게 추가적인 보호막을 제공하며, 체력이 감소하기 전에 쉴드가 먼저 소진됩니다. `Shield` 클래스는 쉴드의 사용과 회복을 관리합니다.

---

💥 **데미지 처리**
- **기능**: 플레이어는 데미지를 받거나 치명타를 당할 수 있습니다.
- **설명**: 플레이어는 적의 공격, 환경적인 위험 등으로부터 데미지를 받으며, `TakeDamage` 메소드로 데미지를 처리합니다. 데미지가 발생하면 체력이 감소하고, 체력이 0 이하로 떨어지면 사망하게 됩니다.

---

📦 **Inventory 시스템**
- **기능**: 총기 및 아이템을 관리합니다.
- **설명**: 플레이어는 인벤토리 시스템을 통해 총기와 아이템을 관리할 수 있습니다. 인벤토리는 `GunManager`를 통해 총기들을 관리하며, `AmmoInventory`는 TMap을 사용하여 각 총기에 필요한 탄약을 저장하고 관리합니다.

>🔫 **GunManager (총기 관리)**
>- **기능**: 플레이어의 총기를 관리합니다.
>- **설명**: `GunManager`는 플레이어가 사용할 수 있는 총기를 관리합니다. 각 총기는 서로 다른 특성과 능력치를 가지고 있으며, 무기 종류에 따라 공격 방식이 달라집니다.

>💣 **AmmoInventory (탄약 관리)**
>- **기능**: 플레이어가 가진 탄약을 관리합니다.
>- **설명**: `AmmoInventory`는 `TMap`을 사용하여 총기별로 탄약을 관리합니다. 예를 들어, 소총은 5.56mm 탄약을 사용하고, 권총은 9mm 탄약을 사용할 수 있습니다. 이를 통해 총기별로 탄약 수를 쉽게 관리할 수 있습니다.

---

**기타**

🛠️ **GunManager 및 AmmoInventory 구현**
- `GunManager` 클래스는 `TArray`나 `TMap`을 사용하여 총기를 관리하고, 각 총기의 특성 및 장비 상태를 추적합니다.
- `AmmoInventory`는 `TMap`을 사용하여 각 총기에 대한 탄약 수를 관리합니다. 이를 통해 총기별로 탄약 수를 쉽게 관리할 수 있습니다.

---

**요약**
`APlayerCharacter` 클래스는 플레이어 캐릭터의 동작을 관리하는 핵심 클래스입니다. 이동, 점프, 공격 등 기본적인 기능 외에도, 쉴드 시스템과 데미지 처리, 그리고 총기 및 탄약 관리 시스템을 통해 게임 내에서 플레이어가 직면할 다양한 상황을 관리합니다. 이러한 기능들은 게임의 플레이 경험을 더욱 몰입감 있게 만들어 줍니다. 🎮

---

**2. Enemy Character**
> **캐릭터 클래스**
>+ BP_BaseEnemyCharacter : 기본 캐릭터 클래스, `OnDead`, `OnStunned`, `Heal`, `EventAnyDamage` 등 적 캐릭터의 기본 기능 설정
>+ BP_BaseEnemyCharacter_Melee,Ranged : 종류별 세부 클래스. 저마다 다른 `Attack`, `EquipWeapon`, `UnequipWeapon` 등을 세부 구현
>+ BPI_EnemyAI : 블루프린트 인터페이스, 적 캐릭터들의 공통 기능 설정. C++ 인터페이스와 다르게 원하는 것만 구현 가능

> **컨트롤러 클래스**
>+ AIC_BaseEnemy : AIController 클래스, `SetStateAs(Passive/Attacking/Stunned/Investigation/Dead)` 등 상태를 변경

> **기타 클래스**
>+ LootDrop : 캐릭터의 `OnDead`시 호출, 적의 아이템 드랍 여부와 종류를 결정하는 클래스
>+ BP_PatrolRoute : 순찰 경로를 표시하는 클래스. `Spline`으로 순찰 경로 시각화

> **핵심 로직**
>+ 상태 기반 행동 결정 : Behavior Tree에서 캐릭터의 상태`(Passive/Attacking/Stunned/Investigation/Dead)`에 따라 행동을 결정
>+ EQS 활용 최적 위치 선정 : 플레이와의 거리, 라인 트레이스 여부를 이용하여 최정의 자리로 이동하여 공격 수행

3. Item(Firable/Usable) 
기능: 플레이어가 획득할 수 있는 아이템을 관리합니다.

클래스:  
>AGun - 총기 기본 클래스. 탄약 관리, 발사, 재장전 기능 포함.  
>APistol - 권총 클래스. 단발 사격 및 특정 효과 포함.  
>ARifle - 라이플 클래스. 연사 기능과 버스트 모드 포함.  
>AShotgun - 샷건 클래스. 산탄 효과 및 탄 퍼짐 구현.  
>AMagazine - 탄약 아이템. 무기의 탄창을 보충.  
>AShield - 쉴드 아이템. 사용 시 플레이어의 방어력을 증가.  
>AHealingItem - 힐링 아이템. 사용 시 플레이어의 체력을 회복.  
>AItem - 모든 아이템의 기본 클래스. 아이템 획득, 사용 기능 포함.  

핵심 로직:

💊소비형 아이템: 체력 회복 (AHealingItem), 방어력 증가 (AShield).

🔫발사형 아이템: 적을 공격할 수 있는 무기 (AGun, APistol, ARifle, AShotgun).

---

🔫AGun 클래스
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
>APistol (권총): 단발 사격, 탄약 절약 모드.\
>ARifle (라이플): 연사 기능, 버스트 모드, 조준 기능.\
>AShotgun (샷건): 산탄 효과, 다중 타겟 공격.

---

🎒ITEM SYSTEM

주요 클래스
>AItem - 모든 아이템의 기본 클래스. 아이템 획득, 사용 기능 포함.\
>AShield - 쉴드 아이템. 사용 시 플레이어의 방어력을 증가.\
>AHealingItem - 힐링 아이템. 사용 시 플레이어의 체력을 회복.\
>AMagazine - 탄약 아이템. 무기의 탄창을 보충.

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
>아이템 사용 (Use()): 아이템 타입에 따라 쉴드 증가, 체력 회복, 탄약 보충.\
>탄약 획득 (AMagazine): 특정 탄약 타입을 보충하여 무기에 장전.

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

