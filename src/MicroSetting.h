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

    MicroSetting(int start, int range) {
      this->start = start;    
      this->range = range;
      this->value = start;
    }
    MicroSetting(int range) {
      this->start = 0;
      this->range = range;
    }
    MicroSetting(int range,const char ** labels) {
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
