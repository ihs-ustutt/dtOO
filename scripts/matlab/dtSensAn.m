classdef dtSensAn < dtIOSystem
  % dtSensAn Create a sensitivita analysis.
  %   Create an dtIOSystem to performa a sensitivity analysis. Give first
  %   and last sample number, sampling strategy and design type. Removes
  %   failed samples according to parser, automatically.
  %
   properties (SetAccess = private, GetAccess = public)
     firstSample_
     lastSample_
     state_
     prefixStr_
     rSA_
     samplingStrategySA_
     designTypeSA_
     failed_
     minX_
     maxX_
   end
   methods
     function obj = dtSensAn( wDir, caseName, prefixStr, firstSample, lastSample, samplingStrategySA, designTypeSA )
% dtSensAn Constructor
        obj@dtIOSystem(0);

        obj.firstSample_ = firstSample;
        obj.lastSample_ = lastSample;
        obj.prefixStr_ = prefixStr;
        obj.samplingStrategySA_ = samplingStrategySA;
        obj.designTypeSA_ = designTypeSA;
        
        %
        % create state array
        %
        obj.state_ = dtState.empty(0);
        c = 0;
        for i=firstSample:1:lastSample
          c = c + 1;
          obj.state_(end+1) ...
          = ...
          dtState( ...
            strcat(wDir, '/', dtState.GiveStateDirectoyName(caseName, prefixStr, i) ), prefixStr ...
          );
        end
        obj.failed_ = zeros(length(obj.state_), 1);
      end
      function [ obj ] = SetFailed( obj, failedList )
% SetFailed Mark labels of cell array as failed
        for i = 1:length(failedList)
          ind = dtState.GiveIndex(failedList{i});
          fprintf('Fail state = %s, ind = %i\n', failedList{i}, ind);
          obj.failed_( ind + 1 ) = 1;
        end
      end  
      function [ obj ] = SetFailedIndices( obj, failedIndices )       
% SetFailedIndices Mark indices of array as failed
        for i = 1:length(failedIndices)
          ind = failedIndices(i);%dtState.GiveIndex(failedList{i});
            
          fprintf('Fail state = %s, ind = %i\n', obj.state_(i).Label, ind);
          obj.failed_( ind ) = 1;
        end
      end
      function [ failed ] = FailedSampleStates( obj )
% FailedSampleStates Return states of failed samples
        failed = {};
        
        for i = 1:length(obj.failed_)
          if obj.failed_(i)==1
            failed{end+1} = obj.state_(i).Label;
          end
        end
      end
      function obj = MakeIOSystem( obj, parser )
% MakeIOSystem Creates the underlying IOSystem
%   Does not take care of failed states. Failed states are removed in the
%   underlying dtIOSystem.
%   parser : 
         nSamples = obj.lastSample_ - obj.firstSample_ + 1;
         nX = length( parser.handle_ );
         nY = length( parser.resultName_ );
        
        %
        % create state array
        %
        X = zeros(nSamples, nX);
        Y = zeros(nSamples, nY);
        for c=1:length(obj.state_)
          X(c,1:nX) = parser.ValueOfHandle(obj.state_(c).Label);
          Y(c,1:nY) = parser.ValueOfResult(obj.state_(c).Label);          
          if ( parser.HasState( obj.state_(c).Label ) && obj.state_(c).ok_ && ~obj.failed_(c) )
          else
            Y(c,1:nY) = nan;
            obj.failed_(c) = 1;
          end
        end
        
        obj.minX_ = min( X(find(obj.failed_==0), : ) );
        obj.maxX_ = max( X(find(obj.failed_==0), : ) );

        obj.rSA_ = nSamples / (nX+1); 
%         [ obj.rSA_, X, Y, obj.failed_ ] ...
%         = ...
%         destroyBastardsEET( obj.rSA_, X, Y, obj.designTypeSA_, obj.failed_ );
        [Y] = obj.SetFailToNaN( Y, obj.failed_ );
        %
        % update IOSystem
        %
        obj = obj.Update( size(X,1) );

        for i=1:length(parser.handle_)
          obj = obj.SetX( parser.handle_{i}, parser.handleFig_{i}, X(:,i) );
        end
        for i=1:length(parser.resultName_)
          obj = obj.SetY( parser.resultName_{i}, parser.resultNameFig_{i}, Y(:,i) );
        end
      end
      function [ mi, sigma, EE, SEM_x, SEM_y ] = CalculateEEIndices( obj, outputName )
% CalculateEEIndices Calculates std and mean of EE for sensitivity analysis
%   Calculate the indices for an output. mi is the mean and sigma is the
%   std of the distribution.
        [ mi, sigma, EE ] ...
        = ...
        EET_indices( ...
          obj.rSA_, obj.minX_, obj.maxX_, obj.x_, obj.OneY(outputName), obj.designTypeSA_ ...
        ); 

        SEM = 2./sqrt(obj.rSA_);

        if ( SEM * max(sigma) > max(mi) )
          SEM_x = linspace(min(mi),max(mi), 5);
          SEM_y = SEM_x ./ SEM;
        else
          SEM_y = linspace(min(sigma), max(sigma), 5);
          SEM_x = SEM_y .* SEM;    
        end  
      end
      function [ ret ] = ShowPlot( obj, outputName, filename )
% ShowPlot Shows an EE plot for an output.
        [mi, sigma, EE, sx, sy] = obj.CalculateEEIndices(outputName);
        ret = dtPlot(filename);
        ret.GiveSinglePlot(16, 16, '$$ \mu $$', '$$ \sigma $$');
        ret.PlotTextAtPoints( mi, sigma, obj.xNameFig_ );
        ret.Plot( sx, sy, 'k--');
      end
      function [ Y ] = SetFailToNaN( obj, Y, failed )
% ShowPlot Shows an EE plot for an output.
        Y( find(failed==1) ) = nan;
      end       
   end
end