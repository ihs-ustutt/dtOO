classdef dtCircAverage
   properties (SetAccess = private, GetAccess = public)
      rMin_
      rMax_
      nBins_
      %sumWeight_
      binA_
      binQ_
      binMin_
      binMax_
      binMean_
      binSumA_
      binSumQ_
      binNValues_
      binRMean_
   end
   methods
      function obj = dtCircAverage(field, nBins)
        r = field.R;
        value = field.value_;
        
        obj.rMin_ = min(r);
        obj.rMax_ = max(r);
        obj.nBins_ = nBins;
        
        % determine number of bins
        % make sure that no bin is empty
        bins = [];
        for i=1:obj.nBins_
          bins = discretize(r, linspace(obj.rMin_, obj.rMax_, obj.nBins_+1));
          if ( size( unique( sort(bins) ), 1 ) ~= obj.nBins_ )
            obj.nBins_ = obj.nBins_ - 1;
            bins = [];
          else
            break;
          end          
        end
        
        obj.binA_ = zeros(obj.nBins_, size(value,2));
        obj.binQ_ = zeros(obj.nBins_, size(value,2));
        obj.binMin_ = zeros(obj.nBins_, size(value,2));
        obj.binMax_ = zeros(obj.nBins_, size(value,2));
        obj.binMean_ = zeros(obj.nBins_, size(value,2));
        obj.binSumA_ = zeros(obj.nBins_, 1);
        obj.binSumQ_ = zeros(obj.nBins_, 1);
        obj.binNValues_ = zeros(obj.nBins_, 1);
        obj.binRMean_ = zeros(obj.nBins_, 1);        
        
        for j=1:size(value,2)
          obj.binMin_(:,j) = max(value(:,j));
          obj.binMax_(:,j) = min(value(:,j));
        end
        
        fprintf('Create %d bins ( init %d bins )\n', obj.nBins_, nBins);
        
        % put values in bins
        a = dt.Mag( field.sf_ );
        for i=1:size(bins)
          thisBin = bins(i);
          obj.binNValues_( thisBin ) = obj.binNValues_( thisBin ) + 1;
          obj.binSumA_( thisBin ) = obj.binSumA_( thisBin ) + a(i);
          obj.binSumQ_( thisBin ) = obj.binSumQ_( thisBin ) + field.q_(i);
        
          for j=1:size(value,2)          
            obj.binA_( thisBin, j ) = obj.binA_( thisBin, j ) + a(i) * value(i,j);
            obj.binQ_( thisBin, j ) = obj.binQ_( thisBin, j ) + field.q_(i) * value(i,j);
            if (value(i, j) < obj.binMin_( thisBin, j ))
              obj.binMin_( thisBin, j ) = value(i,j);
            end
            if (value(i, j) > obj.binMax_( thisBin, j ))
              obj.binMax_( thisBin, j ) = value(i,j);
            end
            obj.binMean_( thisBin, j ) = obj.binMean_( thisBin, j ) + value(i,j);
          end
          obj.binRMean_( thisBin ) = obj.binRMean_( thisBin ) + r(i);
        end
                
        for i=1:obj.nBins_
          obj.binRMean_(i) = obj.binRMean_(i) / obj.binNValues_(i);
          for j=1:size(value,2)           
            obj.binMean_(i,j) = obj.binMean_(i,j) / obj.binNValues_(i);
            obj.binA_(i,j) = obj.binA_(i,j) / obj.binSumA_(i);
            obj.binQ_(i,j) = obj.binQ_(i,j) / obj.binSumQ_(i);
          end
        end
      end 
      function v = ValueA( obj, r )
        bins = discretize(r, linspace(obj.rMin_, obj.rMax_, obj.nBins_+1));
        v = obj.binA_( bins, : ); 
      end
      function v = ValueQ( obj, r )
        bins = discretize(r, linspace(obj.rMin_, obj.rMax_, obj.nBins_+1));
        v = obj.binQ_( bins, : ); 
      end  
      function [] = WriteCSV( obj, field )
        [pathstr, name, ext] = fileparts( field.filename_ );
        
        fileStr = strcat(pathstr, '/', name, '_dtCircAverageCSV', ext);
        fprintf('Write CSV > %s <\n', fileStr);

        fileID = fopen(fileStr,'w');
        
        fprintf(fileID,'%s\n', '[Data]');
        fprintf(...
          fileID, ...
          '%s\n', ...
          'R [ m ], Velocity Radial [ m s^-1 ], Velocity Circumferential [ m s^-1 ], Velocity Axial [ m s^-1 ]' ...
        );

        %fprintf(fileID,'%6s %12s\n','x','exp(x)');
        
        fprintf(...
          fileID, ...
          '%16.8e, %16.8e, %16.8e, %16.8e\n', ...
          0.9 * obj.binRMean_(1), obj.binQ_(1,1), obj.binQ_(1,2), obj.binQ_(1,3) ...
        );            
        for ii=1:obj.nBins_
          fprintf(...
            fileID, ...
            '%16.8e, %16.8e, %16.8e, %16.8e\n', ...
            obj.binRMean_(ii), obj.binQ_(ii,1), obj.binQ_(ii,2), obj.binQ_(ii,3) ...
          );          
        end
        fprintf(...
          fileID, ...
          '%16.8e, %16.8e, %16.8e, %16.8e\n', ...
          1.1 * obj.binRMean_(end), obj.binQ_(end,1), obj.binQ_(end,2), obj.binQ_(end,3) ...
        );                    
        fclose(fileID);
      end
   end
end