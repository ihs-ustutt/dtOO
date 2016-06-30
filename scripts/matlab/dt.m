classdef dt
   properties
      
   end
   methods (Static)
      function mag = Mag( field )
        if ( size(field, 2)==1 )
          mag = abs(field);
        elseif ( size(field, 2)==3 )
          mag ...
          = ...
          sqrt(...
              field(:,1).*field(:,1) ...
            + field(:,2).*field(:,2) ...
            + field(:,3).*field(:,3) ...
          );
        else
          throw( ...
            MException( 'dt:Mag', 'size(field,2) = %d', size(field,2) ) ...
          );
        end
      end
      function ret = SumMag( field )
        ret = sum( dt.Mag(field) );
      end          
      function ret = Sum( field )
        ret = sum( field );
      end   
      function [ind] = ExtractPareto(xx, yy, nBins)
        ind = zeros(nBins,1);
        xBin = discretize( xx, linspace(min(xx), max(xx), nBins+1) );
        for i=1:nBins
          XWithInd = [ yy( find( xBin==i ) ) find( xBin==i ) ];
          [v, tmpInd] = max( XWithInd(:,1) );
          ind(i,1) = XWithInd(tmpInd,2);
        end
      end
   end
end