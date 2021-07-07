#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/set.hh>
#include <iostream>
#include <sstream>
#include <string>
#include <array>

using namespace Gecode;
using namespace std;

class Data {
    public:
        int readings[5] = {35, 35, 20, 20, 20};
        int start = 25;
        int readingsLength = sizeof(readings) / sizeof(readings[0]);
} inputData;

class Temperature : public IntMinimizeScript {
  protected:
    Data data;
    IntVarArray temp;
    IntVar minCost;
    IntVarArray choice;

  public: //home space

    // constructor
    Temperature(const SizeOptions& opt) : 
        IntMinimizeScript(opt), data(inputData), temp(*this, data.readingsLength+1, 21, 35),
        minCost(*this, 0, 789789), choice(*this, data.readingsLength, IntSet({1, 4, -2, -5, 0})) {

        // initialize temp_nd
        int temp_nd[data.readingsLength+1];
        temp_nd[0] = data.start;
        for(int i = 1; i<data.readingsLength+1; i++){
            temp_nd[i] = avg(temp_nd[i-1], data.readings[i-1]);
            cout << temp_nd[i] << endl;
        }
        
        for(int i = 0; i<data.readingsLength; i++){
            if(choice[i].size() == 1){
                rel(*this, choice[i], IRT_LE, 4);
            }
        }

        // branch(*this, temp, INT_VAL_MIN());
    }

    //copy constructor
    Temperature(Temperature& s) : IntMinimizeScript(s) {
      temp.update(*this, s.temp);
      minCost.update(*this, s.minCost);
    }

    virtual IntVar cost(void) const{
        return minCost;
    }

    //virtual copy 
    virtual Space* copy(void){
      return new Temperature(*this);
    }

    // Print solution
    virtual void
    print(std::ostream& os) const {
        //
    } 

    // average function
    int avg(int a, int b){
       return (a + b) / 2; 
    }
};

int main(int argc, char* argv[]) {
  SizeOptions opt("Sequence");
  opt.ipl(IPL_VAL);
  opt.solutions(0);
  opt.parse(argc, argv);
  // BAB will give optimal
  Script::run<Temperature,BAB,SizeOptions>(opt);
  return 0;
}