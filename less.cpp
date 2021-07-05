#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include "less.h"

using namespace Gecode;

class Less : public Propagator {
 protected:
        Int::IntView x0, x1;

 public:
        Less(Home home, Int::IntView y0, Int::IntView y1)
            :Propagator(home), x0(y0), x1(y1) {
                x0.subscribe(home, *this, Int::PC_INT_DOM);
                x1.subscribe(home, *this, Int::PC_INT_DOM);
        }

        static ExecStatus post(Space& home,
            Int::IntView x0, Int::IntView x1) {  // initial check
            if (x0 == x1)  // nothing to compare
                return ES_FAILED;

            // exist i, {i} from x0 > {min} from x1 
            GECODE_ME_CHECK(x0.le(home, x1.max()));
            GECODE_ME_CHECK(x1.gr(home, x0.min()));

            if (x1.min() < x0.max())
                (void) new (home) Less(home, x0, x1);

            return ES_OK;  // if(x0.max() < x1.min()) is included, always true
        }

        virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
            GECODE_ME_CHECK(x0.le(home, x1.max()));
            GECODE_ME_CHECK(x1.gr(home, x0.min()));

            if (x0.max() < x1.min())
                return home.ES_SUBSUMED(*this);
            else
                return ES_FIX;
        }

        Less(Space& home, Less& p)
            :Propagator(home, p) {
                x0.update(home, p.x0);
                x1.update(home, p.x1);
        }

        virtual Propagator* copy(Space& home) { 
            return new (home) Less(home, *this);
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

                    // check y0 < y1;
void less(Space& home, IntVar y0, IntVar y1) {
    GECODE_POST;
    GECODE_ES_FAIL(Less::post(home, y0, y1));
}