classdef dtValueVector3Field < dtVector3Field
   properties (SetAccess = private, GetAccess = public)
      coord_
      value_
      sf_
      q_
      mag_
      A_
      APos_
      ANeg_      
      Q_
      QPos_
      QNeg_
      intSf_
      intQSf_      
      trf_
   end
   methods
      function obj = dtValueVector3Field( initField )
        obj@dtVector3Field( initField.filename_ );

        [obj.coord_, obj.value_, obj.sf_, obj.q_] = obj.Field();

        obj.mag_ = dt.Mag( obj.value_ );        
             
        obj.A_ = dt.SumMag( obj.sf_ );
        obj.APos_ = dt.SumMag( obj.sf_( find(obj.q_>0), : ) );
        obj.ANeg_ = dt.SumMag( obj.sf_( find(obj.q_<0), : ) );        
        obj.Q_ = dt.SumMag( obj.q_ );
        obj.QPos_ = dt.Sum( obj.q_( find(obj.q_>0) ) );
        obj.QNeg_ = dt.Sum( obj.q_( find(obj.q_<0) ) );
        for i=1:3
          obj.intSf_(i) = dt.Sum( obj.value_(:,i) .* dt.Mag(obj.sf_) );
          obj.intQSf_(i) = dt.Sum( obj.value_(:,i) .* obj.q_ );    
        end
      end 
      function obj = Cyl( obj )
        obj.trf_ = dtCylTrf( obj.coord_ );
      end
      function [ r ] = R( obj )
        if( ~isa(obj.trf_,'dtCylTrf') ); 
          throw(  ...
            MException( ...
              'dtValueVector3Field:R', ...
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
              'dtValueVector3Field:Phi', ...
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
              'dtValueVector3Field:Z', ...
              'trf_ is not a dtCylTrf' ...
            ) ...
          );
        end
        z = obj.trf_.T3();
      end       
   end             
end