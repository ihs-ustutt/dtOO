classdef dtFile
   properties (SetAccess = private, GetAccess = public)
     filepattern_
     dir_
     file_
     ext_
   end
   methods
      function obj = dtFile( filepattern )
% dtFile Constructor                
        obj.filepattern_ = filepattern;
        [obj.dir_, obj.file_, obj.ext_] = fileparts( filepattern );
      end
      function [ filename ] = FileName( obj )       
% FileName Get the name of the file as string. File pattern has to be unique.
        filename = '';

        if ( exist( obj.filepattern_, 'file' ) )
          filename = obj.filepattern_;
        end
        
        if ( isempty( filename ) )        
          dirList = dir( obj.filepattern_ );

          if ( size( dirList, 1 ) > 1 )
            throw(  ...
              MException( ...
                'dtFile:FileName', ...
                 sprintf('> %s < is not a unique file or directory.', obj.filepattern_) ...
              ) ...
            );
          else
            if ( dirList(1).isdir )
              throw(  ...
                MException( ...
                  'dtFile:FileName', ...
                   sprintf('> %s < is a unique directory.', obj.filepattern_) ...
                ) ...
              );            
            end
          end

          filename = [obj.dir_, '/', dirList(1).name];
        end
      end       
   end
end