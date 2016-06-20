classdef dtLSSVM < dtIOSystem
   properties (SetAccess = private, GetAccess = public)
     yp_
     alpha_
     b_
     gam_
     sig2_
     model_
   end
   methods
      function obj = dtLSSVM(parser, states, xName, yName)
        %
        % dimension checks
        %
        if (length(yName)~=1)
          throw(  ...
            MException( 'dtLSSVM:dtLSSVM', sprintf('Length of yName is not 1.') ) ...
          );          
        end        
        
        inRes = [];
        outRes = [];
        for i=1:length(xName)
          inRes(end+1) = find( ismember(parser.resultName_, xName{i} )==1 );
        end
        outRes = find( ismember(parser.resultName_, yName{1} )==1 );
        if length(inRes)~=length(xName)
          throw(  ...
            MException( 'dtLSSVM:dtLSSVM', sprintf('Length of inRes and xName is not equal.') ) ...
          );          
        end

        resMat = zeros( length(states), length(inRes)+1 );
        for i=1:length(states)
          res = parser.ValueOfResult(states{i});
          resMat(i, 1:end) = res([inRes outRes]);
        end
        
        %
        % create empty IOSystem
        %
        obj@dtIOSystem( length(states) );
        
        %
        % input
        %
        for i=1:length(xName); 
          obj ...
          = ...
          obj.SetX( ...
            parser.resultName_( inRes(i) ), ...
            parser.resultNameFig_( inRes(i) ), ...
            resMat(:,i) ...
          ); 
        end
        
        %
        % output
        %
        obj ...
        = ...
        obj.SetY( ...
          parser.resultName_( outRes ), ...
          parser.resultNameFig_( outRes ), ...
          resMat(:,end) ...
        ); 

        %
        % create lssvm
        %
       [ obj.yp_, obj.alpha_, obj.b_, obj.gam_, obj.sig2_, obj.model_] = lssvm(obj.x_, obj.y_, 'f');      
      end
      function [ y ] = SimLSSVM( obj, x )
        [y, z, obj.model_] = simlssvm( obj.model_, x );
      end
   end
end