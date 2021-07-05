#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include "nqueen_propagator.h"

using namespace Gecode;

class Nqueen_propagator : public Propagator {
 protected:
        ViewArray<Int::IntView> m;

 public:
        Nqueen_propagator(Home home, ViewArray<Int::IntView> m)
            :Propagator(home), m(m) {
                // x0.subscribe(home, *this, Int::PC_INT_DOM);
                // x1.subscribe(home, *this, Int::PC_INT_DOM);
                m.subscribe(home, *this, Int::PC_INT_DOM);
        }

        static ExecStatus post(Space& home,
            ViewArray<Int::IntView> m) {  
            if (x0 == x1)  
                return ES_FAILED;

            for(int i=0; i<m.width(); i++){
                // GECODE_ME_CHECK(m(i, j).
            }

            return ES_OK;  
        }

        virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
            GECODE_ME_CHECK(x0.le(home, x1.max()));
            GECODE_ME_CHECK(x1.gr(home, x0.min()));

            if (x0.max() < x1.min())
                return home.ES_SUBSUMED(*this);
            else
                return ES_FIX;
        }

        Nqueen_propagator(Space& home, Nqueen_propagator& p)
            :Propagator(home, p) {
                x0.update(home, p.x0);
                x1.update(home, p.x1);
        }

        virtual Propagator* copy(Space& home) { 
            return new (home) Nqueen_propagator(home, *this);
        }

        virtual PropCost cost(const Space&, const ModEventDelta&) const { 
            return PropCost::binary(PropCost::LO);
        }

        virtual void reschedule(Space& home) {
            x0.reschedule(home, *this, Int::PC_INT_DOM);
            x1.reschedule(home, *this, Int::PC_INT_DOM);
        }

        virtual size_t dispose(Space& home) {  
            x0.cancel(home, *this, Int::PC_INT_DOM);
            x1.cancel(home, *this, Int::PC_INT_DOM);
            (void) Propagator::dispose(home);
            return sizeof(*this);
        }

};

void nqueen_propagator(Space& home, ViewArray<Int::IntView> m) {
    GECODE_POST;
    GECODE_ES_FAIL(Nqueen_propagator::post(home, m));
}