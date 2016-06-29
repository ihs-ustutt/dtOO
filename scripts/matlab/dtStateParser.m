classdef dtStateParser
   properties (SetAccess = private, GetAccess = public)
     cV_
     stateLabel_
     filename_
     handle_
     handleFig_
     epsSizeX_
     epsSizeY_
     fontName_;
     fontSize_;
     lc_;
     lcH_;
     lw_;
     lwH_;
   end
   methods
      function obj = dtStateParser( filename )
        obj.cV_ = {};
        obj.stateLabel_ = {};
        obj.filename_ = filename;%strrep(filename, '.xml', '');
        obj.handle_ = {};
        obj.epsSizeX_ = 7.;
        obj.epsSizeY_ = 6.;
        obj.fontName_ = 'Helvetica';
        obj.fontSize_ = 8;
        obj.lc_  = [150 150 150]/256;
        obj.lcH_ = [0 0 0];
        obj.lw_  = 0.5;
        obj.lwH_ = 1;             
      end
      function obj = adjustPlotSize(obj, sizeX, sizeY)
        obj.epsSizeX_ = sizeX;
        obj.epsSizeY_ = sizeY;
      end
      function fig = GivePlot(obj, labelX, labelY)
        fig = figure('Visible', 'Off');
        epssetup(obj.epsSizeX_, obj.epsSizeY_, 0, [1 1 1], 1, 0);

        xlabel(...
          labelX,'FontSize', ...
          obj.fontSize_,'FontName',obj.fontName_, 'Interpreter', 'latex' ...
        );        
        ylabel(...
          labelY,'FontSize', ...
          obj.fontSize_,'FontName',obj.fontName_, 'Interpreter', 'latex' ...
        );      
      end 
      function [] = SavePlot(obj, fig, filename)
        print(fig,  '-depsc', filename);
        close(fig);
      end
      function obj = AddToHandle( obj, varargin )
        %obj.cV_{end+1} = cV;
        for i=2:nargin
          if iscell(varargin{i-1})
            thisCell = varargin{i-1};
            for j=1:length(thisCell)
              obj.handle_{end+1} = thisCell{j};  
            end
          else
            obj.handle_{end+1} = varargin{i-1};
          end
        end
      end
      function obj = AddToHandleFig( obj, varargin )
        for i=2:nargin
          if iscell(varargin{i-1})
            thisCell = varargin{i-1};
            for j=1:length(thisCell)
              obj.handleFig_{end+1} = thisCell{j};  
            end
          else
            obj.handleFig_{end+1} = varargin{i-1};
          end
        end
      end      
      function obj = ClearHandle( obj )
        obj.handle_ = {};
        obj.handleFig_ = {};
      end      
      function [ sH ] = ValueOfHandle( obj, state )
        stateId = find( ismember(obj.stateLabel_, state )==1 );
        
        cV_label = containers.Map('KeyType','char','ValueType','int32');
        for i=1:size( obj.cV_{stateId}, 2 )
          cV_label( obj.cV_{stateId}{i}.label_ ) = i;
        end
        sH = [];
        for i=1:size( obj.handle_, 2 )
          str = obj.handle_{i};
          sH(end+1) = obj.cV_{stateId}{ cV_label( str ) }.value_;
        end
      end      
      function [ sH ] = MinOfHandle( obj, state )
        stateId = find( ismember(obj.stateLabel_, state )==1 );
        
        %obj.cV_{end+1} = cV;
        %obj.handle_{end+1} = label;
        cV_label = containers.Map('KeyType','char','ValueType','int32');
        for i=1:size( obj.cV_{stateId}, 2 )
          cV_label( obj.cV_{stateId}{i}.label_ ) = i;
        end
        sH = [];
        for i=1:size( obj.handle_, 2 )
          str = obj.handle_{i};
          sH(end+1) = obj.cV_{stateId}{ cV_label( str ) }.min_;
        end
      end            
      function [ sH ] = MaxOfHandle( obj, state )
        stateId = find( ismember(obj.stateLabel_, state )==1 );
        
        %obj.cV_{end+1} = cV;
        %obj.handle_{end+1} = label;
        cV_label = containers.Map('KeyType','char','ValueType','int32');
        for i=1:size( obj.cV_{stateId}, 2 )
          cV_label( obj.cV_{stateId}{i}.label_ ) = i;
        end
        sH = [];
        for i=1:size( obj.handle_, 2 )
          str = obj.handle_{i};
          sH(end+1) = obj.cV_{stateId}{ cV_label( str ) }.max_;
        end
      end                  
      function [ sH ] = ValuePercentOfHandle( obj, state )
        stateId = find( ismember(obj.stateLabel_, state )==1 );
        
        %obj.cV_{end+1} = cV;
        %obj.handle_{end+1} = label;
        cV_label = containers.Map('KeyType','char','ValueType','int32');
        for i=1:size( obj.cV_{stateId}, 2 )
          cV_label( obj.cV_{stateId}{i}.label_ ) = i;
        end
        sH = [];
        for i=1:size( obj.handle_, 2 )
          str = obj.handle_{i};
          sH(end+1) ...
          = ...
          (obj.cV_{stateId}{ cV_label( str ) }.value_ - obj.cV_{stateId}{ cV_label( str ) }.min_) ...
          / ...
          (obj.cV_{stateId}{ cV_label( str ) }.max_ - obj.cV_{stateId}{ cV_label( str ) }.min_);
        end
      end                        
      function obj = Parse( obj )
        fileToParse = {};
        
        [pathstr, name, ext] = fileparts( obj.filename_ );        
        fprintf('Pathstr = %s\n', pathstr);
        fprintf('Name    = %s\n', name);
        fprintf('Ext     = %s\n', ext);
        
        if ( strfind(name, '*') )
          fprintf('Name of %s contains *.\n', obj.filename_);
          cFiles = dir(pathstr);
          
          for i=1:size(cFiles)
            if ( ~cFiles(i).isdir )
              if( strfind( cFiles(i).name, strrep(name,'*','')) )
                if( strfind( cFiles(i).name, '.xml') )
                  fileToParse{end+1} = [pathstr, '/', cFiles(i).name];%obj.filename_;
                end
              end
            end
          end
        else
          fileToParse{end+1} = [pathstr, name, ext];%obj.filename_;
        end
        
        for i=1:size(fileToParse,2)
          fprintf('Parse %s.\n', fileToParse{i});
          xmlIn = xml2struct(fileToParse{i});
          
%           fprintf('Head node = %s\n', xmlIn.Name);
          for j=1:size(xmlIn.Children,2)
            if ( strcmp( xmlIn.Children(j).Name, 'state' ) )
              cStruct = xmlIn.Children(j).Attributes;
              id = find(strcmp({cStruct.Name}, 'label')==1);
              stateLabel = cStruct(id).Value;
              %fprintf('Find state = %s \n', stateLabel);
              %cStruct(id).Value
              obj.cV_{end+1} = obj.extractCV( xmlIn.Children(j) );
              obj.stateLabel_{end+1} = stateLabel;
            end
          end
        end
      end
      function [] = XPlot( obj, stateList )
        % extract all states
        XAll = [];
        for i=1:size(obj.stateLabel_,2)
          XAll(i, 1:size(obj.handle_,2)) = obj.ValuePercentOfHandle( obj.stateLabel_{i} );
        end
        
        % extract states to highlight
        XHigh = [];
        for i=1:size(stateList,2)
          XHigh(i, 1:size(obj.handle_,2)) = obj.ValuePercentOfHandle( stateList{i} );
        end
        
        %figure
        fig = figure();
        epssetup(obj.epsSizeX_, obj.epsSizeY_, 0, [1 1 1], 1, 0);

        XAll = XAll';
        XHigh = XHigh';
         
        plot(XAll,'Color',obj.lc_,'LineWidth',obj.lw_);
        for j =1:size(XHigh,2)
          hold on
          plot(XHigh(:,j),'Color',obj.lcH_,'LineWidth',obj.lwH_);
        end

        % Costumize horizontal axis:
        M = size(XAll,1);
        set( ...
          gca,'XTick',1:M,'XTickLabel', obj.handleFig_, 'XLim',[0.7 M+0.3], ...
          'TickLabelInterpreter', 'latex' ...
        );
        % Add vertical grid:
        set(gca,'XGrid','On','GridLineStyle','-')
        % Costumize font
        set(gca,'FontSize',obj.fontSize_,'FontName',obj.fontName_)
        ylabel(...
          'Input [-]','FontSize', ...
          obj.fontSize_,'FontName',obj.fontName_, 'Interpreter', 'latex' ...
        );
%        print(fig,  '-depsc', filename);
      end
      function hasState = HasState(obj, state )
        hasState = 1;
        if isempty( find( ismember(obj.stateLabel_, state )==1 ) )
          hasState = 0;        
        end
      end
      function [ stateIndex ] = StateIndex(obj, state )
        %ind = find( ismember(obj.stateLabel_, state )==1 );
        if ischar(state)
          stateIndex = find( strcmp(obj.stateLabel_, state ) );
        elseif iscell(state)
          stateIndex = zeros(length(state),1);
          for i=1:length(state)
            stateIndex(i) = find( strcmp(obj.stateLabel_, state{i} ) );
          end
        else          
            throw(  ...
              MException( ...
                'dtStateParser:StateIndex', sprintf('> %s < is not a state label.', state) ...
              ) ...
            );            
        end
      end  
      function ret = NumberOfStates(obj )
        ret = length( obj.stateLabel_ );
      end        
   end
   methods (Static)
      function [ cV ] = extractCV( aState )
        cV = {};
        for i=1:size( aState.Children, 2 )
          if ( strcmp( aState.Children(i).Name, 'constValue' ) )
            thisCV = aState.Children(i);
            
            % get label
            thisAttributes = thisCV.Attributes;
            id = find(strcmp({thisAttributes.Name}, 'label')==1);
            cvLabel = thisAttributes(id).Value;
            
            % get min
            thisAttributes = thisCV.Attributes;
            id = find(strcmp({thisAttributes.Name}, 'min')==1);
            cVMin = '0.';            
            if ~isempty(id)
              cvMin = thisAttributes(id).Value;                          
            end

            % get max
            thisAttributes = thisCV.Attributes;
            id = find(strcmp({thisAttributes.Name}, 'max')==1);
            cVMax = '0.';            
            if ~isempty(id)            
              cvMax = thisAttributes(id).Value;  
            end

            % get value
            thisAttributes = thisCV.Attributes;
            id = find(strcmp({thisAttributes.Name}, 'value')==1);
            cvValue = thisAttributes(id).Value;  
             
            cV{end+1} ...
            = ...
            dtCV( ...
              cvLabel, ...
              str2double(cvValue), str2double(cvMin), str2double(cvMax) ...
            );
            
          end
        end
      end
   end
end