#include "eoLib.h"

#include <stdexcept>  // runtime_error 
#include <logMe/logMe.h>
#include "optimizableInterface.h"
#include "eoLibEval.h"
#include <interfaceHeaven/vectorHandling.h>
#include <constValueHeaven/constValue.h>

#include <eo>
#include <es.h>

namespace dtOO { 
  // individual
  typedef eoReal< double > Indi;
	
  eoLib::eoLib() {
    _SEED = 42; // seed for random number generator
    _POP_SIZE = 50; // Size of population
    _TOURN_SIZE = 10; // size for tournament selection
    _MAX_GEN = 100; // Maximum number of generation before STOP
    _CROSS_RATE = 0.8; // Crossover rate
    _EPSI = 0.01;  // range for real uniform mutation
    _MUT_RATE = 0.5;    // mutation rate    
  }

  eoLib::eoLib(const eoLib& orig) {
  }

  eoLib::~eoLib() {
  }
  
  void eoLib::optimizeInterface( optimizableInterface * const optiInt ) {
    //////////////////////////
    //  Random seed
    //////////////////////////
    //reproducible random seed: if you don't change SEED above,
    // you'll aways get the same result, NOT a random run
    rng.reseed(_SEED);
    
    eoLibEval< Indi > eval( optiInt );
    ////////////////////////////////
    // Initilisation of population
    ////////////////////////////////
    // declare the population
    eoPop<Indi> pop;

    //
    // value
    //
    Indi vVal; 
    Indi vMin; 
    Indi vMax;
    float radius = 1.00;
    for (unsigned ivar=0; ivar<optiInt->getNParameters(); ivar++) {
      float val = optiInt->getRefToParameters()[ivar]->getValue();
      float min = optiInt->getRefToParameters()[ivar]->getMin();
      float max = optiInt->getRefToParameters()[ivar]->getMax();
      double r = static_cast< double >( (val-min) / (max-min) );
      vVal.push_back(r);
      if ( (r - radius) < min ) {
        vMin.push_back(min);
      }
      else {
        vMin.push_back( r-radius );
      }
      if ( (r + radius) > max ) {
        vMax.push_back(max);
      }
      else {
        vMax.push_back( r+radius );
      }      
    }
    eval(vVal);         
    pop.push_back(vVal);    
    eval(vMin);                                  // evaluate it
    pop.push_back(vMin);                // and put it in the population    
    eval(vMax);         
    pop.push_back(vMax);
       
    // fill it!
    for (unsigned int igeno=3; igeno<_POP_SIZE; igeno++) {
      Indi v;                  // void individual, to be filled
      for (unsigned ivar=0; ivar<optiInt->getNParameters(); ivar++) {
        double r = rng.uniform(); // new value, random in [-1,1)
        v.push_back(r);            // append that random value to v
      }
      eval(v);                                  // evaluate it
      pop.push_back(v);                // and put it in the population
    }
    // sort pop before printing it!
    pop.sort();
    // Print (sorted) intial population (raw printout)
    cout << "Initial Population" << endl;
    cout << pop;
    
    /////////////////////////////////////
    // selection and replacement
    ////////////////////////////////////
    // The robust tournament selection
    eoDetTournamentSelect<Indi> select(_TOURN_SIZE);            // T_SIZE in [2,POP_SIZE]
    // eoSGA uses generational replacement by default
    // so no replacement procedure has to be given

    //////////////////////////////////////
    // termination condition
    /////////////////////////////////////
    // stop after MAX_GEN generations
    eoGenContinue<Indi> continuator(_MAX_GEN);

    //////////////////////////////////////
    // The variation operators
    //////////////////////////////////////
    // offspring(i) uniformly chosen in [parent(i)-epsilon, parent(i)+epsilon]
    eoUniformMutation<Indi>  mutation(_EPSI);
    // offspring(i) is a linear combination of parent(i)
    eoSegmentCrossover<Indi> xover;

    eoCheckPoint< Indi > checkpoint(continuator);
    eoValueParam< unsigned > generationCounter(0, "Gen.");
    eoIncrementor< unsigned > increment( generationCounter.value() );
    checkpoint.add(increment);
    
    eoBestFitnessStat< Indi > bestStat;
    eoAverageStat< Indi > averageStat;
    eoSecondMomentStats< Indi > secondStat;
    eoQuadDistance< Indi > dist;
    eoFDCStat< Indi > fdcStat(dist);
    
    checkpoint.add(bestStat);
    checkpoint.add(averageStat);
    checkpoint.add(secondStat);
    checkpoint.add(fdcStat);
    
    eoStdoutMonitor monitor;
    checkpoint.add(monitor);
    monitor.add(generationCounter);
//    monitor.add( &(eval.operator()) );
    monitor.add(bestStat);
    monitor.add(secondStat);
    monitor.add(fdcStat);
    
    /////////////////////////////////////////
    // the algorithm
    ////////////////////////////////////////
    // standard Generational GA requires
    // selection, evaluation, crossover and mutation, stopping criterion

    eoSGA<Indi> gga(select, xover, _CROSS_RATE, mutation, _MUT_RATE,
                                      eval, checkpoint);
    // Apply algo to pop - that's it!
    gga(pop);

    // Print (sorted) intial population
    pop.sort();
    
    //
    // best value
    //
    Indi bestVal = pop.best_element(); 
//    for (unsigned ivar=0; ivar<optiInt->getNParameters(); ivar++) {
      eval(bestVal);//optiInt->getRefToParameters()[ivar]->setValue( static_cast< float >(bestVal[ivar]) );
//    }
    

    cout << "FINAL Population\n" << pop << endl;    
  }
}
