classdef dtValueScalarField < dtScalarField
   properties (SetAccess = private, GetAccess = public)
      coord_
      value_
      sf_
      q_
      A_
      APos_
      ANeg_      
      Q_
      absQ_
      QPos_
      QNeg_
      intSf_
      intQSf_
      absIntSf_
      absIntQSf_
      trf_
   end
   methods
      function obj = dtValueScalarField( initField )
        obj@dtScalarField( initField.filename_ );

        [obj.coord_, obj.value_, obj.sf_, obj.q_] = obj.Field();
        
        obj.A_ = dt.SumMag( obj.sf_ );
        obj.APos_ = dt.SumMag( obj.sf_( find(obj.q_>0), : ) );
        obj.ANeg_ = dt.SumMag( obj.sf_( find(obj.q_<0), : ) );          
        obj.Q_ = dt.Sum( obj.q_ );
        obj.QPos_ = dt.Sum( obj.q_( find(obj.q_>0) ) );
        obj.QNeg_ = dt.Sum( obj.q_( find(obj.q_<0) ) );        
        obj.intSf_ = dt.Sum( obj.value_ .* dt.Mag(obj.sf_) );
        obj.intQSf_ = dt.Sum( obj.value_ .* obj.q_ );
        obj.absIntSf_ = abs( obj.intSf_ );
        obj.absIntQSf_ = abs( obj.intQSf_ );
        obj.absQ_ = abs( obj.Q_ );
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