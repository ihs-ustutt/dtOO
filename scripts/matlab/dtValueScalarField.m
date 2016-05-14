classdef dtValueScalarField < dtScalarField
   properties (SetAccess = private, GetAccess = public)
      coord_
      value_
      sf_
      q_
      A_
      Q_
      intSf_
      intQSf_
      trf_
   end
   methods
      function obj = dtValueScalarField( initField )
        obj@dtScalarField( initField.filename_ );

        [obj.coord_, obj.value_, obj.sf_, obj.q_] = obj.Field();
        
         obj.A_ = dt.SumMag( obj.sf_ );
         obj.Q_ = dt.SumMag( obj.q_ );
         obj.intSf_ = dt.SumMag( obj.value_ .* dt.Mag(obj.sf_) );
         obj.intQSf_ = dt.SumMag( obj.value_ .* obj.q_ );
      end
      function value = Min( obj )
        value = min(obj.value_);
      end
      function value = Max( obj )
        value = max(obj.value_);
      end    
      function obj = Cyl( obj )
        obj.trf_ = dtCylTrf( obj.coord_ );
      end
      function [ r ] = R( obj )
        if( ~isa(obj.trf_,'dtCylTrf') ); 
          throw( ...
            MException( ...
              'dtValueScalarField:R', ...
              'trf_ is not a dtCylTrf' ...
            ) ...
          );
        end
        r = obj.trf_.T1();
      end
      function [ phi ] = Phi( obj )
        if( ~isa(obj.trf_,'dtCylTrf') ); 
          throw(  ...
            MException( ...
              'dtValueScalarField:Phi', ...
              'trf_ is not a dtCylTrf' ...
            ) ...
          );
        end
        phi = obj.trf_.T2();
      end        
      function [ z ] = Z( obj )
        if( ~isa(obj.trf_,'dtCylTrf') ); 
          throw(  ...
            MException( ...
              'dtValueScalarField:Z', ...
              'trf_ is not a dtCylTrf' ...
            ) ...
          );
        end
        z = obj.trf_.T3();
      end       
   end  
end