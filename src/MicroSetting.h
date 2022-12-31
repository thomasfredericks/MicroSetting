

// NOTES AND TODO
// ===============

/*
Creating a string list and an enum list from a C++ macro - Stack Overflow
https://stackoverflow.com/questions/5530248/creating-a-string-list-and-an-enum-list-from-a-c-macro



#define SETTING_LABELED_6(name, v1, v2, v3, v4, v5, v6)\
    enum name { v1, v2, v3, v4, v5, v6} ;\
    const char *name##Labels[6] = { #v1, #v2, #v3, #v4, #v5, #v6};\
    int name##Count = 6;

SETTING_LABELED_6(Week, Sun, Mon, Tue, Wed, Thu, Fri);

*/

/*
int NAME_Setting
int NAME_Labels
int NAME_

SETTING( MODE , MIN , MAX ) 
int # = MIN;
int # ## Max = MAX;
int # ## Min = MIN;

SETTING_LABELED( MODE , COUNT , ... ) 
int # = MIN;
int # ## Max = MAX;
int # ## Min = MIN;

ROTATE_SETTING

TRANSLATE_SETTING
*/

class MicroSetting {
  private:
    int start;
    int range;
    int value;
    const char ** labels = 0;
    int signedIntModulo(int value, int modulo) {
        while ( value < 0 ) value += modulo;
        return (value % modulo);
    }

  public:

    PuppeteerSetting(int start, int range) {
      this->start = start;    
      this->range = range;
      this->value = start;
    }
    PuppeteerSetting(int range) {
      this->start = 0;
      this->range = range;
    }
    PuppeteerSetting(int range,const char ** labels) {
      this->start = 0;
      this->range = range;
      this->labels = labels;
    }
    void move(int amount) {
      value = constrain( value + amount, start, start+range);
    }
    void rotate(int amount) {
      value = signedIntModulo(value + amount - start, range ) + start;
    }
    int getValue() {
      return value;
    }
    const char * getLabel() {
      if ( labels != 0 ) return labels[value];
      return "";
    }
    bool isLabeled(){
      return labels != 0;
    }
    void setValue(int i){
      value = constrain(i,start,start+range); // NOT SURE IF THIS IS RIGHT...
    }
};
