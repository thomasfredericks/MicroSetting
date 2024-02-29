class MicroSetting {

  int start;
  int range;
  int value;
  const char ** labels = 0;
  const char * name ;

public:

  static int signedIntModulo(int value, int modulo) {
    while ( value < 0 ) value += modulo;
    return (value % modulo);
  }

  MicroSetting(const char * settingName, int startValue, int valueRange) {
    name = settingName;
    start = startValue;
    range = valueRange;
  }
  MicroSetting(const char * settingName, int valueRange) {
    name = settingName;
    start = 0;
    range = valueRange;
  }
  MicroSetting(const char * settingName, int labelCount, const char ** settingLabels) {
    name = settingName;
    start = 0;
    range = labelCount;
    labels = settingLabels;
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
  MicroSettingContainer(const char * containerName, size_t microSettingArrayCount, MicroSetting ** microSettingArray) {
    name = containerName;
    settings = microSettingArray;
    count = microSettingArrayCount;
  }
  const char * getName() {
    return name;
  }
  void setCurrent(int index) {
    current = MicroSetting::signedIntModulo(index, count);
  }
};
