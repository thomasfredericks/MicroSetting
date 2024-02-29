
#ifdef ESP32
#include <Preferences.h>
#endif

class MicroSetting {

  int start;
  int range;
  int value;
  int defaultValue;
  const char ** labels = 0;
  const char * name ;

// void initializeAsInt();
// void initializeAsLabeled();

public:

  static int signedIntModulo(int value, int modulo) {
    while ( value < 0 ) value += modulo;
    return (value % modulo);
  }

  MicroSetting(const char * settingName, int startValue, int valueRange, int startValue) {
    name = settingName;
    start = startValue;
    range = valueRange;
    defaultValue = startValue;
    setInt(startValue);
  }

  MicroSetting(const char * settingName, int valueRange, int startValue) {
    name = settingName;
    start = 0;
    range = valueRange;
    defaultValue = startValue;
    setInt(startValue);
  }
  
  MicroSetting(const char * settingName, int labelCount, const char ** settingLabels, int startIndex) {
    name = settingName;
    start = 0;
    range = labelCount;
    labels = settingLabels;
    defaultValue = startValue;
    setInt(startValue);
  }

  const char * getName() {
    return name;
  }


  /*
  void move(int amount) {
    value = constrain( value + amount, start, start+range);
  }
  */
  void rotate(int amount) {
    value = signedIntModulo(value + amount - start, range ) + start;
  }
  int getInt() {
    return value;
  }
  const char * getLabel() {
    if ( labels != 0 ) return labels[value];
    return "";
  }
  bool isLabeled() {
    return labels != 0;
  }
  void setInt(int i) {
    value = constrain(i, start, start + range); // NOT SURE IF THIS IS RIGHT...
  }
#ifdef ESP32
  void storeIn(Preferences preferences) {
    preferences->putInt(name, value);
  }

  void restoreFrom(Preferences preferences) {
    setInt(preferences->getInt(name, value, startValue));
  }
#endif

};

//template <int MAXCOUNT>
class MicroSettingContainer {
  //MicroSetting *  microSettings[MAXCOUNT];
  size_t count;
  size_t current;
  const char * name ;
  MicroSetting ** settings = 0;

public:

  void rotate(int amount) {
    current = MicroSetting::signedIntModulo(current + amount, count);
  }
  MicroSetting * getCurrentMicroSetting() {
    return settings[current];
  }
  /*
  MicroSettingContainer(const char * name) {
    this->name = name;
  }
  */
  void setCurrent(int index) {
    current = MicroSetting::signedIntModulo(index, count);
  }

  MicroSettingContainer(const char * containerName, size_t microSettingArrayCount, MicroSetting ** microSettingArray, int startIndex) {
    name = containerName;
    settings = microSettingArray;
    count = microSettingArrayCount;
    setCurrent(startIndex);
  }
  const char * getName() {
    return name;
  }

#ifdef ESP32
  void storeIn(Preferences preferences) {
    for (int i = 0; i < count; i++) {
      settings[i]->storeIn(preferences);
    }
  }

  void restoreFrom(Preferences preferences) {
    for (int i = 0; i < count; i++) {
      settings[i]->restoreFrom(preferences);
    }
  }
#endif
};

