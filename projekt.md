# Workoutové hodiny — semestrální projekt

**Autor:** Marek Ševčík  
**Studijní obor:** Kybernetická bezpečnost — Univerzita obrany  
**Rok:** 2026

---

## 1. Přehled projektu
Cílem projektu je vytvořit samostatné embedded zařízení („workoutové hodiny“) postavené na platformě **Arduino**. Zařízení umožňuje provádět intervalové tréninky (např. Tabata, HIIT) s vizuální zpětnou vazbou na **LCD 20x4** a zvukovou signalizací pomocí bzučáku. 

Zařízení je navrženo jako **hybridní**:
* **Autonomní režim:** Ovládání pomocí fyzických tlačítek a výběr z přednastavených workoutů uložených v paměti (nezávislost na PC).
* **Sériový režim:** Komunikace s PC (Python FastAPI server) přes USB pro vzdálenou správu a monitoring stavu - není zcela hotov a 100% funkční.

---

## 2. Funkce a vlastnosti
* **Víceúrovňové menu:** Pohodlné listování v seznamu workoutů pomocí tlačítek.
* **Stavový automat:** Implementace stavů `IDLE`, `RUNNING` a `STOPPING` pro robustní řízení běhu.
* **Zvuková signalizace:** Akustické upozornění na začátek/konec fáze a odpočet posledních 3 sekund.
* **Optimalizovaný vizuální výstup:** LCD displej zobrazuje název cviku, aktuální kolo, zbývající čas a fázi (WORK/REST).
* **Customizable Loading Screen:** Snadno upravitelná úvodní obrazovka s možností ASCII grafiky.

---

## 3. Hardware
* **Mikrokontrolér:** Arduino Uno.
* **Displej:** LCD 20x4 s I2C převodníkem (0x27).
* **Ovládací prvky:** 3x mikrospínač (PBS-12B) zapojený jako `INPUT_PULLUP`.
* **Akustický výstup:** Active Buzzer (připojen na digitální pin).
* **Napájení:** 5V přes USB (Powerbanka nebo PC).

### Zapojení pinů (Pinout)
| Komponenta | Pin Arduina | Poznámka |
| :--- | :--- | :--- |
| **LCD (SDA)** | A4 | Datová linka I2C |
| **LCD (SCL)** | A5 | Hodinová linka I2C |
| **Buzzer** | 10 | PWM / Digitální výstup |
| **Button NEXT** | 2 | Internal Pull-up (spíná proti GND) |
| **Button START** | 3 | Internal Pull-up (spíná proti GND) |
| **Button STOP** | 4 | Internal Pull-up (spíná proti GND) |

---

## 4. Software — Architektura

### Arduino (C++)
Kód je strukturován do modulů pro přehlednost a snadnou údržbu:
* `WorkoutTimer.ino`: Hlavní smyčka, zpracování příkazů a řízení stavů.
* `lcd.cpp / .h`: Správa displeje, funkce pro statické i dynamické vykreslování.
* `workouts.cpp / .h`: Databáze tréninkových plánů (statické pole struktur) a API pro správu.
* `buzzer.cpp / .h`: Definice tónů a délky pípání.

### Python (Reference Server)
* **Framework:** FastAPI + Uvicorn.
* **Komunikace:** Knihovna `pyserial`.
* **Účel:** Slouží jako vzdálené rozhraní pro monitoring a logování průběhu tréninku.

---

## 5. Ukázka datové struktury (Workout)
```cpp
struct Workout {
  char name[20];       // Název workoutu
  int workSeconds;     // Doba zátěže
  int restSeconds;     // Doba odpočinku
  int rounds;          // Počet kol
};
