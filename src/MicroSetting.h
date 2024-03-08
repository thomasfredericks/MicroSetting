
#ifndef LOG
#define LOG(...)
#endif


#ifdef ESP32
#include <Preferences.h>
#endif

#ifndef SIZE_OF_ARRAY
#define SIZE_OF_ARRAY(X)  (sizeof(X) / sizeof(X[0]))
#endif

// ===================================================
class MicroSetting {
public:
  enum levels {
    KEY,
    VALUE
  };

protected:
  const char * name_;
  int intMin_ = 0;
  int intRange_ = 1;
  int intValue_ = 0;
  char type_ = 'n'; // none
  const char ** labels_ = 0;


public:

  const char * getName() {
    return name_;
  }

  static int signedIntModulo(int value, int modulo) {
    while ( value < 0 ) value += modulo;
    return (value % modulo);
  }

  const char * getLabel() {
    if (labels_) return labels_[intValue_];
    return "";
  }



  void printValueTo(Print * printer)  {
    switch (type_) {
    case 'e':
      printer->print(labels_[intValue_]);
      break;
    case 'i' :
      printer->print(intValue_);
      break;
    }
  }

  void printTo(Print * printer, char * valueSeparator)  {
    printer->print(name_);
    if (valueSeparator) printer->print(valueSeparator);
    printValueTo(printer);
  }

  virtual void setInt(int i) {
    intValue_ = MicroSetting::signedIntModulo(i - intMin_, intRange_ ) + intMin_;
  }

  void rotate(int amount)  {
    setInt(intValue_ + amount);
  }


  int getInt() {
    return intValue_;
  }

  MicroSetting(const char * name) {
    name_ = name;
    type_ = 'n'; // none/nill
  }

  MicroSetting(const char * name, int range) {
    name_ = name;
    type_ = 'i';
    intMin_ = 0;
    intRange_ = range;
    intValue_ = intMin_;
  }

  MicroSetting(const char * name, int min, int range) {
    name_ = name;
    type_ = 'i';
    intMin_ = min;
    intRange_ = range;
    intValue_ = intMin_;
  }

  MicroSetting(const char * name, int min, int range, int start) {
    name_ = name;
    type_ = 'i';
    intMin_ = min;
    intRange_ = range;
    intValue_ = start;
  }

  MicroSetting(const char * name, const char ** labels, int count) {
    name_ = name;
    type_ = 'e'; //enum
    labels_ = labels;
    intMin_ = 0;
    intRange_ = count;
    intValue_ = intValue_;
  }

#ifdef ESP32
public:
  virtual void storeTo(Preferences * preferences) {

    switch (type_) {
    case 'e':
    case 'i' :
      preferences->putInt(name_, intValue_);
      break;
    }
  }
  virtual void restoreFrom(Preferences * preferences) {

    switch (type_) {
    case 'e':
    case 'i' :
      setInt(preferences->getInt(name_, intValue_));
      break;
    }
  }


#endif
};


class MicroSettingGroup {

  size_t count_;
  size_t current_;
  const char * name_ ;
  MicroSetting ** settings_ = 0;
  MicroSetting::levels level_ = MicroSetting::KEY; //key or value

public:

  void rotate(int amount) {
    if ( level_ == MicroSetting::KEY) current_ = MicroSetting::signedIntModulo(current_ + amount, count_);
    else settings_[current_]->rotate(amount);
  }

  void rotateSetting(int amount) {
    current_ = MicroSetting::signedIntModulo(current_ + amount, count_);
  }

  void rotateValue(int amount) {
    settings_[current_]->rotate(amount);
  }

  void toggleLevel() {
    if ( level_ == MicroSetting::KEY) level_ = MicroSetting::VALUE ;
    else level_ = MicroSetting::KEY;
  }
  /*
    bool settingEntered() {
      return status_ == kIntMenuTrigger;
    }
    bool valueChanged() {
      return status_ == kMSGValueChanged;
    }
    */
  MicroSetting::levels getLevel() {
    return level_;
  }

  void setLevel(MicroSetting::levels level) {

    level_ = level;

  }
  // shouldRename to getCurrentSetting
  MicroSetting * getCurrent() {
    return settings_[current_];
  }

  void setIndex(int index) {
    current_ = constrain(index, 0, count_);
  }

  int getIndex() {
    return current_;
  }

  void printEachTo(Print * printer, char * valueSeparator, char * settingSeparator)  {
    for (int i = 0; i < count_; i++) {
      if ( i && settingSeparator) printer->print(settingSeparator);

      settings_[i]->printTo(printer, valueSeparator);
    }
  }

  MicroSettingGroup(const char * name, MicroSetting ** settings, size_t count) {
    name_ = name;
    settings_ = settings;
    count_ = count;
    current_ = 0;
  }
  const char * getName() {
    return name_;
  }

#ifdef ESP32
  void storeEachTo(Preferences * preferences) {
    for (int i = 0; i < count_; i++) {
      settings_[i]->storeTo(preferences);
    }
  }

  void restoreEachFrom(Preferences * preferences) {
    for (int i = 0; i < count_; i++) {
      LOG("Restoring", i, settings_[i]->getName() );
      settings_[i]->restoreFrom(preferences);
    }
  }
#endif
};


