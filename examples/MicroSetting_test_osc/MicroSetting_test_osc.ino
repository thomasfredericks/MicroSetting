#include <MicroOscSlip.h>
#include <MicroSetting.h>
#include <Preferences.h>



MicroOscSlip<128> myOsc(&Serial);

Preferences preferences;

MicroSetting intSetting = MicroSetting("int", 0, 255, 127);

enum labeledSettingEnum { ABC,
                          DEF,
                          GHI };
const char* labeledSettingLabels[3] = { "ABC", "DEF", "GHI" };
MicroSetting labeledSetting = MicroSetting("Type", labeledSettingLabels, SIZE_OF_ARRAY(labeledSettingLabels));


class MicroSettingGroupOsc : public MicroSettingGroup {

public:
  // WE CURRENTLY USE AN EXTERNAL SPLIT CHAR ARRAY, BUT THIS COULD BE CHANGED... BY MAKING FRIENDS...
  bool matchHierarchy(MicroOscMessage* oscMessage, const char* split) {
    if (strcmp(name_, split) == 0) {
      split = strtok(NULL, "/");
      for (int i = 0; i < count_; i++) {
        MicroSetting* setting = settings_[i];
        if (strcmp(setting->getName(), split) == 0) {
          // WE COULD COMPARE TYPES HERE
          setting->setInt(oscMessage->nextAsInt());
          return true;
        }
      }
    }
    return false;
  }
  MicroSettingGroupOsc(const char * name, MicroSetting ** settings, size_t count) : MicroSettingGroup(name, settings, count) {

  }
};



MicroSetting* groupArray[2] = { &intSetting, &labeledSetting };
MicroSettingGroupOsc groupSetting = MicroSettingGroupOsc("Group", groupArray, SIZE_OF_ARRAY(groupArray));

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  groupSetting.restoreEachFrom(&preferences);
}

void myOscMessageReceived(MicroOscMessage& oscMessage) {
  // WE CURRENTLY HAVE TO COPY THE ADDRESS, BUT THIS WILL COULD BE CHANGED... BY MAKING FRIENDS...
  char addrBuffer[64];
  oscMessage.copyAddress(addrBuffer, (size_t) SIZE_OF_ARRAY(addrBuffer));

  char* split = strtok(addrBuffer, "/");

  // If we match we stop!
  if (groupSetting.matchHierarchy(&oscMessage, split)) return;
}

void loop() {
  // put your main code here, to run repeatedly:

  myOsc.onOscMessageReceived(myOscMessageReceived);

  // THIS DOES NOT WORK YET : groupSetting.sendEach(&myOsc);

  delay(50);
}
