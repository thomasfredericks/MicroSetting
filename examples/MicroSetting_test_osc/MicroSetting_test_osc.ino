#include <MicroOscSlip.h>
#include <MicroSetting.h>
#include <Preferences.h>

THIS EXAMPLE DOES NOT WORK AND WILL NOT COMPILE
IT IS ONLY A MODEL ON HOW I WOULD LIKE THINS TO WORK

MicroOscSlip<128> myOsc(&Serial);

Preferences preferences;

MicroSetting intSetting = MicroSetting("int", 0, 255, 127);

enum labeledSettingEnum { ABC, DEF, GHI };
const char* labeledSettingLabels[3] = { "ABC", "DEF", "GHI"};
MicroSetting labeledSetting = MicroSetting("Type", labeledSettingLabels, SIZE_OF_ARRAY(labeledSettingLabels));

MicroSetting * groupArray[2] = { &intSetting, &labeledSetting };
MicroSettingGroup groupSetting = MicroSettingGroup("Group", groupArray, SIZE_OF_ARRAY(groupArray));

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  groupSetting.restoreEachFrom(&preferences);
}

void myOscMessageReceived(MicroOscMessage & oscMessage) {
  groupSetting.matchEach(&oscMessage);
}

void loop() {
  // put your main code here, to run repeatedly:
   
   myOsc.onOscMessageReceived(myOscMessageReceived);
   
   groupSetting.sendEach(&myOsc);

   delay(50);


}
