#ifndef RFID_H
#define RFID_H
#pragma once

#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>
#include <component.h>

#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16

class RFID : public Component
{
private:
    MFRC522 *mfrc522;
    MFRC522::MIFARE_Key chave;

public:
    RFID(u_int ss, u_int rst)
    {
        mfrc522 = new MFRC522(ss, rst);
    };
    void iniciar();
    bool aguardar_cartao();
    String ler_id();
    String ler_dados(byte bloco);
    void escrever_dados(byte bloco);
};

#endif