class RCSwitch {
  public:
    // Attributes
    int nReceiverInterrupt;
    int nTransmitterPin;
    int nRepeatTransmit;
    char nProtocol;
    int nPulseLength;
    static unsigned long nReceivedValue;

    //int LOW=0;
    //int HIGH=1;

    RCSwitch();


    void enableTransmit(int nTransmitterPin);
    void disableTransmit();
    void setPulseLength(int nPulseLength);
    void setRepeatTransmit(int nRepeatTransmit);

    char* getCodeWordA(char* sGroup, int nSwitchNumber, bool bStatus);
    char* getCodeWordB(int nAddressCode, int nChannelCode, bool bStatus);

    // TriState
    void sendTriState(char* sCodeWord);
    void sendSync();
    void sendT0();
    void sendT1();
    void sendTF();

    void transmit(int nHighPulses, int nLowPulses);

    void switchOn(int nGroupNumber, int nSwitchNumber);
    void switchOff(int nGroupNumber, int nSwitchNumber);
    void switchOn(char* sGroup, int nSwitchNumber);
    void switchOff(char* sGroup, int nSwitchNumber);

    // TODO
    //void digitalWrite(int nPin, int nValue);
    void delayMicroseconds(int mUsec);
};
