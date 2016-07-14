classdef dtIOSystem
   properties (SetAccess = private, GetAccess = public)
     nSamples_
     nX_
     x_
     minX_
     maxX_     
     nY_
     y_
     yName_
     yNameFig_
     xName_
     xNameFig_
   end
   methods
      function [ obj ] = dtIOSystem( nSamples )
% dtIOSystem Constructor
        obj = obj.Update( nSamples);
      end
      function [ obj ] = Update(obj, nSamples )
% Update Updates system to given number of samples
        obj.nSamples_ = nSamples;
        obj.nX_ = 0;
        obj.nY_ = 0;
        obj.x_ = zeros(obj.nSamples_, obj.nX_);
        obj.y_ = zeros(obj.nSamples_, obj.nX_);
        obj.yName_ = {};
        obj.yNameFig_ = {};        
        obj.xName_ = {};
        obj.xNameFig_ = {};  
      end
      function [ obj ] = SetY( obj, yName, yNameFig, y )
% SetY Set an output to the system  
        %
        % check size
        %
        if (obj.nSamples_~=length(y))
           throw(  ...
             MException( 'dtIOSystem:SetY', sprintf('Length of obj.y_ and y is not equal.') ) ...
           );
        end
        
        %
        % set name and nameFig and extent y_
        %
        obj.yName_{end+1} = yName;
        obj.yNameFig_{end+1} = yNameFig;
        obj.y_(:,end+1) = y;
        obj.nY_ = obj.nY_ + 1;
      end
      function [ obj ] = SetX( obj, xName, xNameFig, x )
% SetX Set an input to the system
        %
        % check size
        %
        if (obj.nSamples_~=length(x))
           throw(  ...
             MException( 'dtIOSystem:SetX', sprintf('Length of obj.x_ and x is not equal.') ) ...
           );
        end
        
        %
        % set name and nameFig and extent y_
        %
        obj.xName_{end+1} = xName;
        obj.xNameFig_{end+1} = xNameFig;
        obj.x_(:,end+1) = x;
        obj.nX_ = obj.nX_ + 1;
        
        obj.minX_ = min(obj.x_);
        obj.maxX_ = max(obj.x_);
      end  
      function [ thisX ] = OneX( obj, xName )
% OneX Return single input
        x_i = find( ismember(obj.xName_, xName )==1 );
        if ( isempty(x_i) || length(x_i)>1 )
           throw(  ...
             MException( 'dtIOSystem:OneX', sprintf('x_i is empty or length(x_i)>1.') ) ...
           );
        end
        
        thisX = obj.x_(:,x_i);
      end      
      function [ thisY ] = OneY( obj, yName )
% OneY Return single output
        y_i = find( ismember(obj.yName_, yName )==1 );
        if ( isempty(y_i) || length(y_i)>1 )
           throw(  ...
             MException( 'dtIOSystem:OneY', sprintf('y_i is empty or length(y_i)>1.') ) ...
           );
        end
        
        thisY = obj.y_(:,y_i);
      end            
   end
end