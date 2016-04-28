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
          error('size(field,2) = %d', size(field,2) );
        end
      end
      function ret = SumMag( field )
        ret = sum( dt.Mag(field) );
      end          
      function ret = Sum( field )
        ret = sum( field );
      end                
   end
end