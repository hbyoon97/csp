#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/set.hh>
#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <map>

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
        IntMinimizeScript(opt), data(inputData), temp({*this, data.readingsLength+1, 0, 50}),
        minCost(*this, 0, 123123), choice(*this, data.readingsLength, IntSet({1, 4, -2, -5, 0})) {
        
        rel(*this, temp[0] == data.start);
        
        //read temp_nd and make choice
        for(int i = 0; i<data.readingsLength; i++){
          
            rel(*this, temp[i+1] == (temp[i] + data.readings[i]) / 2 + choice[i]);
            rel(*this, (temp[i] + data.readings[i]) / 2 + choice[i] <= 30);
            rel(*this, (temp[i] + data.readings[i]) / 2 + choice[i] >= 25);
            
        }

        branch(*this, choice, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    }

    //copy constructor
    Temperature(Temperature& s) : IntMinimizeScript(s) {
      data = s.data;
      temp.update(*this, s.temp);
      choice.update(*this, s.choice);
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
        cout << "temp solution" << endl << temp << endl;
        cout << "corresponding choice" << endl << choice << endl << endl;
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
  Script::run<Temperature,DFS,SizeOptions>(opt);
  return 0;
}