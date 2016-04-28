classdef dtStateParser
   properties (SetAccess = private, GetAccess = public)
     cV_
     stateLabel_
     filename_
     handle_
   end
   methods
      function obj = dtStateParser( filename )
        obj.cV_ = {};
        obj.stateLabel_ = {};
        obj.filename_ = filename;%strrep(filename, '.xml', '');
        obj.handle_ = {};
      end     
      function obj = AddToHandle( obj, varargin )
        %obj.cV_{end+1} = cV;
        for i=2:nargin
          obj.handle_{end+1} = varargin{i-1};
        end
      end
      function obj = ClearHandle( obj )
        obj.handle_ = {};
      end      
      function [ sH ] = ValueOfHandle( obj, state )
        stateId = find( ismember(obj.stateLabel_, state )==1 )
        
        %obj.cV_{end+1} = cV;
        %obj.handle_{end+1} = label;
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
        stateId = find( ismember(obj.stateLabel_, state )==1 )
        
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
        stateId = find( ismember(obj.stateLabel_, state )==1 )
        
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
        stateId = find( ismember(obj.stateLabel_, state )==1 )
        
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
          fprintf('Name of %s contains *.', obj.filename_);
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
            cvMin = thisAttributes(id).Value;            

            % get max
            thisAttributes = thisCV.Attributes;
            id = find(strcmp({thisAttributes.Name}, 'max')==1);
            cvMax = thisAttributes(id).Value;  

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