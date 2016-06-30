classdef dtPlot
   properties (SetAccess = private, GetAccess = public)
     filename_
     fontName_
     fontSize_
     lc_
     lcH_
     lw_
     lwH_
     fig_
   end
   methods
      function obj = dtPlot( filename )
        obj.filename_ = filename;
        obj.fontName_ = 'Helvetica';
        obj.fontSize_ = 8;
        obj.lc_  = [150 150 150]/256;
        obj.lcH_ = [0 0 0];
        obj.lw_  = 0.5;
        obj.lwH_ = 1;
        obj.fig_ = figure();
      end
      function [] = MakeCurrent( obj )
        figure( obj.fig_ );
      end      
      function fig = GiveSinglePlot(obj, sizeX, sizeY, labelX, labelY)
        fig = obj.fig_;
        figure(obj.fig_);
        epssetup(sizeX, sizeY, 0, [1 1 1], 1, 0);

        if ~isempty(labelX)
          xlabel(...
            labelX,'FontSize', ...
            obj.fontSize_,'FontName',obj.fontName_, 'Interpreter', 'latex' ...
          );        
        end
        if ~isempty(labelY)
          ylabel(...
            labelY,'FontSize', ...
            obj.fontSize_,'FontName',obj.fontName_, 'Interpreter', 'latex' ...
          );
        end
      end 
      function [ fig ] = AdjustSize(obj, sizeX, sizeY)
        figure(obj.fig_);
        epssetup(sizeX, sizeY, 0, [1 1 1], 1, 0);
        fig = obj.fig_;
      end       
      function [] = Close(obj)       
        close(obj.fig_);
      end
      function [] = SaveEps(obj)
        figure( obj.fig_ );
        hold on;
        print(obj.fig_,  '-depsc', obj.filename_);
      end
      function [] = PlotTextAtPoints(obj, x, y, str)
        figure( obj.fig_ );
        hold all;
        for j = 1:length(x)    
          plot( x(j), y(j),'o','MarkerSize', 0.00001 );
          text( ...
            x(j), y(j), str{j}, ...
            'HorizontalAlignment','center', ...
            'FontSize', obj.fontSize_, 'FontName', obj.fontName_, ...
            'Interpreter', 'latex' ...
          );     
        end
      end
      function [] = Plot(obj, x, y, str)
        figure( obj.fig_ );
        hold all;
        plot( x, y, str, 'Linewidth', obj.lw_);
      end      
   end
end