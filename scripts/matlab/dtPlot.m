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
      function obj = dtPlot( filename, varargin )
        obj.filename_ = filename;
        obj.fontName_ = 'Helvetica';
        obj.fontSize_ = 8;
        obj.lw_  = 0.5;
        obj.lwH_ = 1;        
        obj.lc_  = [150 150 150]/256;
        obj.lcH_ = [0 0 0];
        obj.fig_ = figure('Visible', 'Off');
          
        if ( length(varargin)==4 )
          obj.fontName_ = varargin{1};
          obj.fontSize_ = str2double(varargin(2));
          obj.lw_  = str2double(varargin(3));
          obj.lwH_ = str2double(varargin(4));
        end

      end
      function [] = MakeCurrent( obj )
        %figure( obj.fig_ );
        set(groot,'CurrentFigure',obj.fig_);
      end      
      function fig = GiveSinglePlot(obj, sizeX, sizeY, labelX, labelY)
        fig = obj.fig_;
        %figure(obj.fig_);
        obj.MakeCurrent
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
        %figure(obj.fig_);
        obj.MakeCurrent
        epssetup(sizeX, sizeY, 0, [1 1 1], 1, 0);
        fig = obj.fig_;
      end       
      function [] = Close(obj)       
        close(obj.fig_);
      end
      function [ obj ] = ChangeFilename(obj, str)
        obj.filename_ = str;
      end      
      function [] = SaveEps(obj)
        %figure( obj.fig_ );
        %hold on;
        obj.MakeCurrent
        print(obj.fig_,  '-depsc', obj.filename_);
        %hold off;
      end
      function [] = PlotTextAtPoints(obj, x, y, str)
        %figure( obj.fig_ );
        obj.MakeCurrent
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
        %figure( obj.fig_ );
        obj.MakeCurrent
        hold on;
        plot( x, y, str, 'Linewidth', obj.lw_);
        hold off;
      end
      function [ ] = Hide(obj)
        obj.fig_.Visible = 'off';
      end
      function [ ] = Show(obj)
        obj.fig_.Visible = 'on';
      end      
   end
end