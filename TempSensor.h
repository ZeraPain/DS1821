#pragma once
/**
@file
Realisiert das Lesen von DS1821 One Wire Temperatur Sensoren, die an einen PSOC angeschlossen werden.
Die Implementierung bassiert auf der Softwareumsetzung und blockiert bei einem Fehler nicht. Interrupts werden aufgrund des Kritischen Timings abgeschaltet!
Zu beachten ist die Strucktur im TopDesign. Es können 255 Sensoren ausgelesen werden, allerdings muss dann das TopDesign entsprechend angepasst werden.
@author David Tarnov
@date 07.01.2017
*/

/**One Wire Bus 0*/
#define Bus_0 (1 << 0)
/**One Wire Bus 1*/
#define Bus_1 (1 << 1)
/**One Wire Bus 2*/
#define Bus_2 (1 << 2)
/**One Wire Bus 3*/
#define Bus_3 (1 << 3)
/**One Wire Bus 4*/
#define Bus_4 (1 << 4)
/**One Wire Bus 5*/
#define Bus_5 (1 << 5)
/**One Wire Bus 6*/
#define Bus_6 (1 << 6)
/**One Wire Bus 7*/
#define Bus_7 (1 << 7)
/**Alle One Wire Buse*/
#define Bus_ALL 255u
/**Kein One Wire Bus*/
#define Bus_NONE 0u

/**Kommando um Temperature High register zu schreiben*/
#define CMD_Write_TH 0x01
/**Kommando um Temperature Low register zu schreiben*/
#define CMD_Write_TL 0x02
/**Kommando um Temperature High register zu lesen */
#define CMD_Read_TH 0xA1
/**Kommando um Temperature High register zu lesen */
#define CMD_Read_TL 0xA2
/**Kommando um Status register zu schreiben */
#define CMD_Write_Status 0x0C
/**Kommando um Status register zu lesen */
#define CMD_Read_Status 0xAC
/**Kommando Startet die Temperaturmessung und konvertiert diese in Wert */
#define CMD_Start_Convert 0xEE
/**Kommando Stopt die Temperaturmessung*/
#define CMD_Stop_Convert 0x22
/**Liest die Temperatur aus*/
#define CMD_Read_Temperature 0xAA

#define LOW 0u
#define HIGH 1u

#define ResetPulse 800u

/**
Dieser Befehl schreibt das TH (HIGH TEMPERATURE) register. Wenn diese funktion ausgeführt wird werden 8 bits in das TH register geschrieben
@param[in] value  der geschrieben werden soll
@param[in] bus  der angesprochen werden soll(Defines)
*/
void Write_TH(uint8 value, uint8 bus);


/**
Dieser Befehl schreibt das TL (LOW TEMPERATURE) register. Wenn diese funktion ausgeführt wird werden 8 bits in das TL register geschrieben
@param[in] value der geschrieben werden soll
@param[in] bus der angesprochen werden soll(Defines)
*/
void Write_TL(uint8 value, uint8 bus);



/**
Dieser Befehl liest das TH (HIGH TEMPERATURE) register. Wenn diese funktion ausgeführt wird werden 8 bits des TH registers gelesen
@param[in] bus der angesprochen werden soll(Defines)
@return 8 bits des TH Registers
*/
uint8 Read_TH(uint8 bus);

/**
Dieser Befehl liest das TL (LOW TEMPERATURE) register. Wenn diese funktion ausgeführt wird werden 8 bits des TL registers gelesen
@param[in] bus  der angesprochen werden soll(Defines)
@return 8 bits des TL Registers
*/
uint8 Read_TL(uint8 bus);

/**
Dieser Befehl startet die Konvertierung der Temperatur. Im One-Shot Mode wird die Konvertierung einmal durchgeführt und der Sensor verbleibt dann im Ruhezustand. Im Vortlaufenenden Modus konvertiert der Temperatursensor dauerhaft die Temperatur
@param[in] bus  der angesprochen werden soll(Defines)
*/
void Start_Convert(uint8 bus);


/**
Dieser Befehl stopt die Konvertierung der Temperatur, wenn der Temperatursensor im Vortlaufenende Modus ist. Wenn der Befehl ausgeführt wurtde, wird die Temperaturmessung durchgeführt und der Sensor geht dann in den Ruhezustand.
@param[in] bus  der angesprochen werden soll(Defines)
*/
void Stop_Convert(uint8 bus);


/**
Dieser Befehl schreibt das Statusregister. Dieses wird verwendet um die Werte des THF und TLF flags sowie die Einstellungen des T/R, POL und 1  SHOT bits zurückzusetzen. Wenn der Befehl ausgeführt wird werden 8 Bits in das Statusregister geschrieben.
@param[in] value der ins Status register geschrieben werden soll
@param[in] bus der angesprochen werden soll(Defines)
*/
void Write_Status(uint8 value, uint8 bus);

/**
Dieser Befehl liest das Statusregister. Wenn der Befehl ausgeführt wird wird das Statusregister zurückgegeben.
@param[in] bus der angesprochen werden soll(Defines)
@return 8 Bits des Statusregisters
*/
uint8 Read_Status(uint8 bus);


/**
Dieser Befehl liest das Register, in welchem das Ergebniss der letzte Temperatur Konvertierung steht.
@param[in] force = 1 wird das Ergebniss sofort zurück geliefert, mit force=0 wird gewartet bis der Sensor das Ermitteln der Temperatur abgeschlossen hat
@param[in] bus der angesprochen werden soll(Defines)
*/
uint8 Read_Temperature(uint8 bus, uint8 force);

