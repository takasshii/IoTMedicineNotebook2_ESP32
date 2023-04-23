#line 1 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/weight/weight.h"
#ifndef WEIGHT_H_
#define WEIGHT_H_

#include <HX711.h>

typedef enum {
  INITIAL_COMPLETED_WEIGHT,
  MEASURING_WEIGHT,
  COMPLETED_WEIGHT,
} WEIGHT_STATE;

class Weight {
  private:
    HX711 scale;
    typedef void (*FuncPtrInt)(WEIGHT_STATE);
 
  public:
    FuncPtrInt callBackEvent;
    Weight();
    Weight(FuncPtrInt f);
    void initWeight();
    double measureWeight();
};

#endif