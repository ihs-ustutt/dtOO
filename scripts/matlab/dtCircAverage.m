classdef dtCircAverage
   properties (SetAccess = private, GetAccess = public)
      rMin_
      rMax_
      nBins_
      sumWeight_
      bin_
      binMin_
      binMax_
      binMean_
      binSumWeight_
      binNValues_
      binRMean_
   end
   methods
      function obj = dtCircAverage(r, phi, z, value, weight, nBins)
        obj.rMin_ = min(r);
        obj.rMax_ = max(r);
        obj.nBins_ = nBins;
        obj.sumWeight_ = sum(weight);
        obj.bin_ = zeros(obj.nBins_, 1);
        obj.binMin_ = zeros(obj.nBins_, 1);
        obj.binMin_(:) = max(value);
        obj.binMax_ = zeros(obj.nBins_, 1);
        obj.binMax_(:) = min(value);
        obj.binMean_ = zeros(obj.nBins_, 1);
        obj.binSumWeight_ = zeros(obj.nBins_, 1);
        obj.binNValues_ = zeros(obj.nBins_, 1);
        obj.binRMean_ = zeros(obj.nBins_, 1);
        
        bins = discretize(r, linspace(obj.rMin_, obj.rMax_, obj.nBins_+1));
        
        for i=1:size(bins)
          thisBin = bins(i);
          obj.binNValues_( thisBin ) = obj.binNValues_( thisBin ) + 1;
          obj.binSumWeight_( thisBin ) = obj.binSumWeight_( thisBin ) + weight(i);
          obj.bin_( thisBin ) = obj.bin_( thisBin ) + weight(i) * value(i);
          if (value(i) < obj.binMin_( thisBin ))
            obj.binMin_( thisBin ) = value(i);
          end
          if (value(i) > obj.binMax_( thisBin ))
            obj.binMax_( thisBin ) = value(i);
          end
          obj.binMean_( thisBin ) = obj.binMean_( thisBin ) + value(i);
          obj.binRMean_( thisBin ) = obj.binRMean_( thisBin ) + r(i);
        end
        for i=1:obj.nBins_
          obj.binMean_(i) = obj.binMean_(i) / obj.binNValues_(i);
          obj.binRMean_(i) = obj.binRMean_(i) / obj.binNValues_(i);
          obj.bin_(i) = obj.bin_(i) / obj.binSumWeight_(i);
        end
      end 
      function v = Value( obj, r )
        bins = discretize(r, linspace(obj.rMin_, obj.rMax_, obj.nBins_+1));
        v = obj.bin_( bins ); 
      end
   end
end