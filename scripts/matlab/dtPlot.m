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
        
        % Veraendern der Figure
        set(gcf,'PaperType','a4');                          % a4-Papier waehlen
        set(gcf,'PaperOrientation','portrait');             % Portrait
        set(gcf,'PaperUnits','centimeters');                % eps-Einheit Zentimeter
        set(gcf,'PaperPosition', [4 4 sizeX sizeY]);  % Groesse des spaeteren eps-files
        set(gcf,'Units','centimeters');                     % Figure-Einheit Zentimeter
        fp=get(gcf,'Position');                             % Figure Pos. holen
        set(gcf,'Position',[fp(1) fp(2) sizeX sizeY]);% Figure wysiwyg machen

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
%        epssetup(sizeX, sizeY, 0, [1 1 1], 1, 0);
        fig = obj.fig_;
      end       
      function [] = Close(obj)       
        close(obj.fig_);
      end
      function [ obj ] = ChangeFilename(obj, str)
        obj.filename_ = str;
      end      
      function [] = SaveEps(obj)
        obj.MakeCurrent
%        epssetup(sizeX, sizeY);        
        print(obj.fig_,  '-depsc2', '-painters', strcat(obj.filename_,'.eps') );
%       print -depsc2 -painters test3.eps
%          exportfig( ...
%            obj.fig_, strcat(obj.filename_,'.eps') ...
%          );
      end
      function [] = SavePng(obj)
        obj.MakeCurrent
        print(obj.fig_,  '-dpng', '-popengl', strcat(obj.filename_,'.png'));
%        print(obj.fig_,'-dpdf','-r300', strcat(obj.filename_,'.pdf'));
      end   
      function [] = SaveJpg(obj, varargin)
        res = 120;
        if ( length(varargin)==1 )
          res = varargin{1};
        end        
        obj.MakeCurrent

        oldscreenunits = get(gcf,'Units');
        oldpaperunits = get(gcf,'PaperUnits');
        oldpaperpos = get(gcf,'PaperPosition');
        set(gcf,'Units','pixels');
        scrpos = get(gcf,'Position');
        newpos = scrpos/100;
        set(gcf,'PaperUnits','inches','PaperPosition',newpos)
%         print('-djpeg', filename, '-r100');
        print( ...
          obj.fig_,  ...
          '-djpeg100', ...
          strcat('-r', num2str(res)), ...
          strcat(obj.filename_,'.jpg') ...
        );

        drawnow

        set( ...
          gcf,'Units',oldscreenunits, 'PaperUnits', oldpaperunits, ...
          'PaperPosition',oldpaperpos ...
        );
     
        
%         print( ...
%           obj.fig_,  ...
%           '-djpeg100', ...
%           strcat('-r', num2str(res)), ...
%           strcat(obj.filename_,'.jpg') ...
%         ); %-r1200
%        print(obj.fig_,'-dpdf','-r300', strcat(obj.filename_,'.pdf'));
      end            
      function [] = PlotTextAtPoints(obj, x, y, str, varargin)
        sScale = 1.;
        cScale = 0.;
        if ( length(varargin)==1 )
          sScale = varargin{1};
        elseif ( length(varargin)==2 )
          sScale = varargin{1};
          cScale = varargin{2};
        end            
        %figure( obj.fig_ );
        obj.MakeCurrent
        hold all;
        for j = 1:length(x)    
          plot( x(j), y(j),'o','MarkerSize', 0.00001 );
          text( ...
            x(j), y(j), str{j}, ...
            'HorizontalAlignment','center', ...
            'FontSize', obj.fontSize_, 'FontName', obj.fontName_, ...
            'Interpreter', 'latex', ...
            'Color', cScale * [1 1 1] ...
          );
        end
      end
      function [] = PlotAndSizeMarkers(obj, x, y, scale, str)
        obj.MakeCurrent
        hold on;
        for j = 1:length(x)
          plot( x(j), y(j), str, 'MarkerSize', scale(j));
        end
        hold off;
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
      function [legh,objh,outh,outm] = Legend(obj, legendLabels, loc)
        obj.MakeCurrent();
        hold on;
        [legh,objh,outh,outm] ...
        = ...
        legend(legendLabels, 'Location', loc, 'Interpreter', 'latex');
        hold off;
      end
   end
end