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
%         if (length(yName)~=1)
%           throw(  ...
%             MException( 'dtLSSVM:dtLSSVM', sprintf('Length of yName is not 1.') ) ...
%           );          
%         end        
        
        inRes = [];
        outRes = [];
        for i=1:length(xName)
          inRes(end+1) = find( ismember(parser.resultName_, xName{i} )==1 );
        end
        %outRes = find( ismember(parser.resultName_, yName{1} )==1 );
        for i=1:length(yName)
          outRes(end+1) = find( ismember(parser.resultName_, yName{i} )==1 );
        end        
        if length(inRes)~=length(xName)
          throw(  ...
            MException( 'dtLSSVM:dtLSSVM', sprintf('Length of inRes and xName is not equal.') ) ...
          );          
        end

        resMat = zeros( length(states), length(inRes)+length(outRes) );
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
        offset = length(xName);
        for i=1:length(yName);         
          obj ...
          = ...
          obj.SetY( ...
            parser.resultName_( outRes ), ...
            parser.resultNameFig_( outRes ), ...
            resMat(:,offset+i) ...
          ); 
        end

        %
        % create lssvm
        %
       [ obj.yp_, obj.alpha_, obj.b_, obj.gam_, obj.sig2_, obj.model_] = lssvm(obj.x_, obj.y_, 'f');      
      end
      function [ y ] = SimLSSVM( obj, x )
        [y, z, obj.model_] = simlssvm( obj.model_, x );
      end
      function [ y ] = SimLSSVMLimitRange( obj, x, range )
        y = [];
        thisXMin = obj.minX_ - range * abs(obj.minX_);
        thisXMax = obj.maxX_ + range * abs(obj.maxX_);
        for i=1:size(x,1)
          xUp = find( x(i,:)>thisXMax );
          xDown = find( x(i,:)<thisXMin );
          sumL = length(xUp) + length(xDown)
          if ( sumL == 0 )
            [y(end+1,1), z, obj.model_] = simlssvm( obj.model_, x(i,:) );
          else
            y(end+1,1) = nan();
          end
        end
      end      
   end
end