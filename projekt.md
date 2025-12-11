# Workoutové hodiny — semestrální projekt

**Autor:** Marek Ševčík  
**Studijní obor:** Kybernetická bezpečnost — Univerzita obrany  
**Rok:** 2025

---

## Přehled projektu
Cílem projektu je vytvořit malé embedded zařízení („workoutové hodiny“) řízené mikrokontrolérem (Raspberry Pi Pico), které bude vykonávat intervalové tréninky (např. Tabata, EMOM, custom workouts). Zařízení produkuje zvukovou zpětnou vazbu (pípnutí) pro označení začátku a konce pracovního intervalu a přestávky. Konfigurace workoutů probíhá přes uživatelské rozhraní běžící na PC, které pošle zvolený plán do zařízení přes USB (sériová komunikace).

Projekt ukazuje:
- Embedded vývoj (C/C++ pro Raspberry Pi Pico)
- Desktopovou C++ aplikaci (UI) pro výběr a správu workoutů
- Komunikační protokol mezi PC a zařízením (JSON přes USB/Serial)
- Základy návrhu hardware (buzzer, tranzistor, napájení)

---

## Funkce
- Výběr workoutu z přednastavených plánů nebo možnost nahrát vlastní plán.
- Generování pípnutí pro začátek/ukončení `work` a `rest` intervalů.
- Uložení workoutů jako JSON soubory.
- Sériová komunikace PC ↔ Pico (JSON payload).
- Možnost přidat jednoduché vizuální potvrzení v PC UI (např. odpočet).

---

## Hardware
- **Raspberry Pi Pico** .  
- **Active buzzer module 3.5–5.5 V**   
- **NPN tranzistor** (2N2222 / BC337) a **rezistor 1 kΩ** pro řízení bzučáku bezpečně z GPIO.  
- **Breadboard + dupont kabely**, **USB kabel**, **header piny**.  

> Poznámka: Proč active buzzer? Je jednodušší — stačí ho napájet a pípne. 

---

## Software — architektura
### PC (Desktop UI)
- Jazyk: C++ (Qt / ImGui / nebo konzolová verze pro rychlý prototyp)
- Funkce:
  - načítání a editace workout JSON (`work_time`, `rest_time`, `rounds`, `name`)
  - volba workoutu a odeslání do zařízení
  - zobrazit aktuální stav (připojeno / běží / pauza)
- Komunikace: sériový port (USB CDC) — JSON zprávy

### Device (Raspberry Pi Pico)
- Jazyk: C/C++ (Pico SDK) 
- Funkce:
  - čtení JSON konfigurace ze sériového portu
  - parsování polí: `name`, `work_time`, `rest_time`, `rounds`
  - časovač pro intervaly (přes hardware timer nebo software)
  - řízení PWM výstupu na pinu připojeném k buzzeru
  - echo / logging přes sériový port (pro debug)

---

## Formát JSON (příklad)
```json
{
  "name": "Tabata",
  "work_time": 20,
  "rest_time": 10,
  "rounds": 8
}
