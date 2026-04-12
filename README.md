# Rozšiřující modul HAPCAN - Čtečka NFC tagů

Tento repozitář obsahuje kompletní hardwarové a softwarové podklady pro vývoj a realizaci nového rozšiřujícího modulu do systému domácí automatizace HAPCAN. Modul umožňuje bezkontaktní identifikaci uživatele pomocí technologie RFID a následné řízení výstupních prvků chytré domácnosti přes sběrnici CAN. Projekt vznikl jako součást bakalářské práce.

<p align="center">
  <img width="40%" alt="DPS - Modul-3D-TOP" src="https://github.com/user-attachments/assets/32bfc24e-4efe-4e4c-bd67-89a0c52af81c" />
  <br>
  <i>Obr. 1: Modul - 3D vizualizace</i>
</p>


## Funkce modulu
- <ins>Bezkontaktní ovládání:</ins> Řízení výstupního akčního prvku pomocí RFID technologie
- <ins>Správa přístupů:</ins> Určování přístupových práv přes oficiální aplikaci HAPCAN Programmer
- <ins>Univerzální výstup:</ins> Schopnost ovládat libovolný akční člen pracující s napětím 24 V
- <ins>Snadná instalace:</ins> Umístění v ochranném krytu do elektroinstalační krabice ve zdi
- <ins>Volba připojení:</ins> Do sběrnice CAN lze modul zapojit pomocí RJ45 nebo šroubovací svorkovnice
- <ins>Plná kompatibilita:</ins> Zařízení respektuje protokol HAPCAN a dokáže s ním komunikovat
- <ins>Indikace tagu:</ins> Indikační LED signalizují přiložení tagu


# Hardware
Schéma zapojení a návrh desky plošných spojů (DPS) byly vytvořeny v programu KiCad. Veškeré zdrojové soubory projektu i kompletní podklady pro výrobu jsou k dispozici v příslušných složkách.


| Model DPS - TOP | Model DPS - BOT |
| :---: | :---: |
| <img width="60%" alt="DPS - JLCPCB - TOP" src="https://github.com/user-attachments/assets/11aba09b-88bc-48cc-9fa7-2b01672ae39d" /> | <img width="60%" alt="DPS - JLCPCB - BOT" src="https://github.com/user-attachments/assets/1573caef-8063-4d5f-86e3-649a53c6a4c2" />|
<p align="center">
  <i> Obr. 2: Vizualizace modulu DPS </i>
</p>
  

# Software

Kompletní Firmware byl vyvinut v prostředí Arduino IDE a dělí se na dvě části. Obsluhu NFC čtečky zajišťuje zdrojový soubor Hapcan_NFC.cpp. Komunikaci po sběrnici CAN, konfiguraci modulu a interakci s aplikací Hapcan Programmer, obstarává druhý zdrojový soubor HapcanProtocol.cpp. Oběma částem přísluší hlavičkové soubory pro deklaraci funkcí a import knihoven. Pro pochopení základní logiky programu jsou níže uvedeny vývojové diagramy obou hlavních procesů. Kompletní zdrojové kódy jsou umístěny v přiložené složce.

<p align="center">
  <img width="50%" alt="Hapcan_NFC-vývojový diagram drawio" src="https://github.com/user-attachments/assets/8f330b48-8d9e-4592-b60a-06143c630d38" />
  <br>
  <i>Obr. 3: Vývojový diagram - Obsluha čtečky</i>
</p>


<p align="center">
  <img width="50%" alt="HapcanProtocol-vývojový diagram drawio (1)" src="https://github.com/user-attachments/assets/16f24a4d-6b0b-4fc2-9c68-fbed4c1e641f" />
  <br>
  <i>Obr. 4: Vývojový diagram - Obsluha protokolu HAPCAN </i>
</p>

# Konstrukce modulu

Kompletní modul se umisťuje do navrženého ochranného krytu. Ten plní nejen bezpečnostní funkci, ale zároveň fixuje NFC čtečku v optimální poloze pro spolehlivé načítání tagů. Takto sestavený modul je koncipován pro snadnou montáž do běžné elektroinstalační krabice pod omítku. Detailní postup sestavení a instalace je popsán v přiložené dokumentaci, přičemž 3D modely potřebné pro výrobu krytu jsou součástí elektronických příloh práce. 

Následující schéma znázorňuje příklad zapojení s využitím elektrického zámku. Pro korektní funkci modulu je nezbytnou podmínkou jeho
připojení ke sběrnici CAN a propojení s Button modulem.

<p align="center">
<img width="50%" alt="Typické zapojení modulu" src="https://github.com/user-attachments/assets/699a4f12-1f2b-4a39-83d2-07cffca22f24" />
  <br>
  <i>Obr. 5: Typické zapojení</i>
</p>


<p align="center">
  <img width="40%" alt="Typické zapojení modulu" src="https://github.com/user-attachments/assets/1b67e641-791f-4e68-b1b6-8cba85b5f1b9" />
  <br>
  <i>Obr. 6: Sestavený modul </i>
</p>

# Struktura souborů

<pre>
Hardware/
└── KiCad 9.0/                   # Projekty návrhu DPS
    ├── Modul - DPS.kicad_pcb
    ├── Modul - DPS.kicad_pro
    └── Modul - DPS.kicad_sch
Software/
└── Arduino IDE 2.3.8/           # Zdrojové soubory firmwaru
    ├── HAPCAN.ino
    ├── HAPCAN_NFC.cpp
    ├── HAPCAN_NFC.h
    ├── HAPCANProtocol.cpp
    └── HAPCANProtocol.h
3D tisk/
└── Inventor 2026/               # 3D model ochranného krytu
    ├── Kryt - horní díl.ipt
    ├── Kryt - soustava.iam
    └── Kryt - spodní díl.ipt

Dokumentace/                 # PDF dokumentace modulu
    └── Dokumentace.pdf
</pre>
