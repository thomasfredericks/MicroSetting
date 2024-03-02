#include <MicroOscSlip.h>
#include <MicroSetting.h>
#include <Preferences.h>

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

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Group contents:");
  groupSetting.printEachTo(&Serial, "=", "\n");
  Serial.println("------------");
  
  Serial.println("labeledSetting:");
  Serial.println( labeledSetting.getName() );
  Serial.println( labeledSetting.getInt() );
  Serial.println( labeledSetting.getLabel() );
  Serial.println("------------");

   Serial.println("intSetting:");
  Serial.println( intSetting.getName() );
  Serial.println( intSetting.getInt() );
  Serial.println("------------");

  delay(1000);
  
  Serial.println("Rotating setting values");
  labeledSetting.rotate(1);
  intSetting.rotate(1);


}
