# Rozšiřující modul HAPCAN - Čtečka NFC tagů

Tento repozitář obsahuje kompletní hardwarové a softwarové podklady pro vývoj a realizaci nového rozšiřujícího modulu do systému domácí automatizace HAPCAN. Modul umožňuje bezkontaktní identifikaci uživatele pomocí technologie RFID a následné řízení výstupních prvků chytré domácnosti přes sběrnici CAN. Projekt vznikl jako součást bakalářské práce.

<p align="center">
  <img width="40%" alt="DPS - Modul-3D-TOP" src="https://github.com/user-attachments/assets/32bfc24e-4efe-4e4c-bd67-89a0c52af81c" />
  <br>
  <i>Obr. 1: Modul - 3D model</i>
</p>



## Funkce modulu
- Bezkontaktní ovládání: Řízení výstupního akčního prvku pomocí RFID technologie
- Správa přístupů: Určování přístupových práv přes oficiální aplikaci HAPCAN Programmer
- Univerzální výstup: Schopnost ovládat libovolný akční člen pracující s napětím 24 V
- Snadná instalace: Umístění v ochranném krytu do elektroinstalační krabice ve zdi
- Volba připojení: Do sběrnice CAN lze modul zapojit pomocí RJ45 nebo šroubovací svorkovnice
- Plná kompatibilita: Zařízení respektuje protokol HAPCAN a dokáže s ním komunikovat
- Indikace tagu: Indikační LED signalizují přiložení tagu

# Hardware
Schéma zapojení a návrh desky plošných spojů (DPS) byly vytvořeny v programu KiCad. Veškeré zdrojové soubory projektu i kompletní podklady pro výrobu jsou k dispozici v příslušných složkách.

# Software

Kompletní Firmware byl vyvinut v prostředí Arduino IDE a dělí se na dvě části. Obsluhu NFC čtečky zajišťuje zdrojový soubor Hapcan_NFC.cpp. Komunikaci po sběrnici CAN, konfiguraci modulu a interakci s aplikací Hapcan Programmer, obstarává druhý zdrojový soubor HapcanProtocol.cpp. Oběma částem přísluší hlavičkové soubory (.h) pro deklaraci funkcí a import knihoven. Pro pochopení základní logiky programu jsou níže uvedeny vývojové diagramy obou hlavních procesů. Kompletní zdrojové kódy jsou umístěny v přiložené složce.


Obsluha čtečky

Obsluha protokolu HAPCAN



<p align="center">
  <img width="80%" src="https://github.com/user-attachments/files/26648070/HapcanProtocol-vyvojovy.diagram.pdf" alt="Náhled" />
</p>


# Kontrukce krytu, Typické zapojení

Kompletní modul se umisťuje do navrženého ochranného krytu. Ten plní nejen bezpečnostní funkci, ale zároveň fixuje NFC čtečku v optimální poloze pro spolehlivé načítání tagů. Takto sestavený modul je koncipován pro snadnou montáž do běžné elektroinstalační krabice pod omítku. Detailní postup sestavení a instalace je popsán v přiložené dokumentaci, přičemž 3D modely potřebné pro výrobu krytu jsou součástí elektronických příloh práce.



Zde je fotka typického zapojení


Zde je foto kompletně sestaveného modulu



# Strom souborů
