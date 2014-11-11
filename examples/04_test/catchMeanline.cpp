//-----------------------------------------------------------------------------
// FirstRealEA.cpp
//-----------------------------------------------------------------------------
//*
// Still an instance of a VERY simple Real-coded  Genetic Algorithm
// (see FirstBitGA.cpp) but now with  Breeder - and Combined Ops


//	@Alex!!!, in dieser Datei hab ich eigentlich nur die eoLib verwendet, ab Zeile 204 hab ich ein paar STL Dinge verwendet
//	(z.B. ofstream) dies dürfte ja aber eigentlich auch nicht relevant sein
//
//-----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// standard includes
#include <stdexcept>  // runtime_error
#include <iostream>   // cout
#include <fstream>

// the general include for eo
#include <eo>
#include <es.h>
#include <eoRankingSelect.h>
#include <eoEvalFuncCounter.h>
#include <utils/eoRealVectorBounds.h>


#include <eoFitCountContinue.h>
#include <eoGenCountContinue.h>

#include <exceptionHandling/eGeneral.h>


// REPRESENTATION
//-----------------------------------------------------------------------------
// define your individuals
//typedef eoReal<eoMinimizingFitness> Indi;       

// Use functions from namespace std
using namespace std;

// EVALFUNC
//-----------------------------------------------------------------------------
// a simple fitness function that computes the euclidian norm of a real vector
// Now in a separate file, and declared as binary_value(const vector<bool> &)

#include <ffMeanline.h>

// GENERAL
//-----------------------------------------------------------------------------

  vector<double> catchMeanline(int argc, char **argv)
{
  
/*  
// PARAMETRES
  const unsigned int SEED = 3;	// seed for random number generator
  const double _p = 2.; //
  const double _e = 1.; //
  float _rate = 1.;
  const double _conv_r = 1;  //Konvergenzradius
  const unsigned int VEC_SIZE = 3; // Number of object variables in genotypes _ entspricht Anzahl der benötigten Freiheitsgrad
  const unsigned int POP_SIZE = 25; // Size of population

  const unsigned int MAX_GEN = 5; // Maximum number of generation before STOP

//Crossover-Parameter  
  const float P_CROSS = 0.1;	// Crossover probability
  const double hypercubeRate = 0.3;     // relative weight for hypercube Xover
  const double segmentRate = 0.4;  // relative weight for segment Xover
  
//Mutationsparameter  
  const float P_MUT = 0.9;	// mutation probability
  const double mutMin = 0.;	// lower mutation bound
  const double mutMax = 1.;	// upper mutation bound
  const unsigned _dim = 3;	// dimension of VectorBounds
  const double _p_change = 1.;	// the one probability to change all coordinates (mutation bound)

  const double EPSILON = 1.2;	// range for real uniform mutation
  double SIGMA = 0.3;	    	// std dev. for normal mutation

// some parameters for chosing among different operators
  const double uniformMutRate = 0.8;  // relative weight for uniform mutation
  const double detMutRate = 0.2;      // relative weight for det-uniform mutation
  const double normalMutRate = 1.5;   // relative weight for normal mutation */
  std::vector <double> solution (3, 1.);
  std::vector <double> ind (3, 1.);
  //std::vector <double> *_ind;
 /*
// GENERAL
  //////////////////////////
  //  Random seed
  //////////////////////////
  //reproducible random seed: if you don't change SEED above,
  // you'll aways get the same result, NOT a random run
  rng.reseed(SEED);

// EVAL
  /////////////////////////////
  // Fitness function
  ////////////////////////////
  // Evaluation: from a plain C++ fn to an EvalFunc Object
  // you need to give the full description of the function
  //eoEvalFuncPtr<Indi, double, const vector<double>& > eval(  ffMeanline); //<Individuum,Fittnes,xxx>Funktionsaufruf der Fittnesfunction
  

// INIT
  ////////////////////////////////
  // Initilisation of population
  ////////////////////////////////
  // based on a uniform generator
  eoUniformGenerator<double> uGen(0 , 1);		//Grenzen für Individuen
  eoInitFixedLength<Indi> random(VEC_SIZE, uGen);
   // Initialization of the population
  eoPop<Indi> pop(POP_SIZE, random);                    //eoPop und damit auch pop ist ein vector, nach sort.pop kann pop(1) mit parseAndWrite ins XML ausgelsen werden

  
  */
  std::cout<<"Start 1"<<std::endl;
  
  //*ind[0]=0.1;
  //*ind[1]=0.2;
  //*ind[2]=0.3;
  std::cout<<"Start 2"<<std::endl;
  
  for(int ii=0; ii<50000; ii++){
    std::cout<<"Aufruf: " << ii << std::endl;
    ffMeanline(ind);
 }
  
  /*
  // and evaluate it in one loop
  apply<Indi>(eval, pop);	// STL syntax

// OUTPUT
  // sort pop before printing it!
  pop.sort();
  // Print (sorted) intial population (raw printout)Fitness
  cout << "Initial Population" << endl;
  cout << pop;

// ENGINE
  /////////////////////////////////////
  // selection and replacement
  ////////////////////////////////////
// SELECT
  // The robust tournament selection
  eoRankingSelect<Indi> selectRanked(_p, _e);		
  // is now encapsulated in a eoSelectPerc (entage)
  eoSelectPerc<Indi> select(selectRanked, _rate);// by default rate==1

// REPLACE
  // And we now have the full slection/replacement - though with
  // no replacement (== generational replacement) at the moment :-)
  eoGenerationalReplacement<Indi> replace;
  replace(pop, pop);

// OPERATORS
  //////////////////////////////////////
  // The variation operators
  //////////////////////////////////////
// CROSSOVER
  // uniform chooce on segment made by the parents
  eoSegmentCrossover<Indi> xoverS;
  // uniform choice in hypercube built by the parents
  eoHypercubeCrossover<Indi> xoverA;
  // Combine them with relative weights
  eoPropCombinedQuadOp<Indi> xover(xoverS, segmentRate);
  xover.add(xoverA, hypercubeRate);

// MUTATION
  // setting Bounds
  eoRealVectorBounds * mutBounds =   new eoRealVectorBounds( _dim, mutMin, mutMax);
  
  
  
  // offspring(i) uniformly chosen in [parent(i)-epsilon, parent(i)+epsilon]
  //eoUniformMutation<Indi>  mutationU(EPSILON);
  eoUniformMutation<Indi>  mutationU(*mutBounds, EPSILON, _p_change);
  // k (=1) coordinates of parents are uniformly modified
  eoDetUniformMutation<Indi>  mutationD(*mutBounds, EPSILON, _p_change);
  // all coordinates of parents are normally modified (stDev SIGMA)
  eoNormalMutation<Indi>  mutationN(*mutBounds, SIGMA, _p_change);
  // Combine them with relative weights
  eoPropCombinedMonOp<Indi> mutation(mutationU, uniformMutRate);
  mutation.add(mutationD, detMutRate);
  mutation.add(mutationN, normalMutRate, true);

// STOP
// CHECKPOINT
  //////////////////////////////////////
  // termination conditions: use more than one
  /////////////////////////////////////
  // stop after MAX_GEN generations
  eoGenCountContinue<Indi> genCont(MAX_GEN);
  // do MIN_GEN gen., then stop after STEADY_GEN gen. without improvement
  //eoSteadyFitContinue<Indi> steadyCont(MIN_GEN, STEADY_GEN);			ausmarkiert
  // stop when fitness reaches a target (here VEC_SIZE)
  eoFitCountContinue<Indi> fitCount(_conv_r);
  // do stop when one of the above says so
  eoCombinedContinue<Indi> continuator(genCont);
  //continuator.add(steadyCont);						ausmarkiert
  continuator.add(fitCount);

  // The operators are  encapsulated into an eoTRansform object
  eoSGATransform<Indi> transform(xover, P_CROSS, mutation, P_MUT);

// GENERATION
  /////////////////////////////////////////
  // the algorithm
  ////////////////////////////////////////

  
  
  // Easy EA requires
  // selection, transformation, eval, replacement, and stopping criterion
  eoEasyEA<Indi> gga(continuator, eval, select, transform, replace);
  

  // Apply algo to pop - that's it!
  cout << "\n        Here we go\n\n";
  gga(pop);

  
//@Alex Wenn dann brauchst du Änderungen ab hier:  

// OUTPUT
  // Print (sorted) intial population
  pop.sort();
  std::cout << pop[0].fitness()<<std::endl;
  cout << "FINAL Population\n" << pop << endl;
  std::cout << pop[0][0] << " " << pop[0][1] << " " << pop[0][2] << std::endl;
  
  
  //Write Solution into txt-file
  double alphaone = pop[0][0];
  double alphatwo = pop[0][1];
  double delta_y = pop[0][2];
  
  std::ofstream paramstr("Solution.txt", ios::trunc);
  //paramstr.open ("Solution.txt");
  paramstr << alphaone << std::endl << alphatwo << std::endl << delta_y;
  paramstr.close();
*/
  
  solution[0] = 1.;
  solution[1] = 2.;
  solution[2] = 3.;
  
    
  return solution;
  
}




