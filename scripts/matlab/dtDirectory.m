classdef dtDirectory
   properties (SetAccess = private, GetAccess = public)
     dirpattern_
     dir_
     file_
     ext_
   end
   methods     
      function obj = dtDirectory( dirpattern )
% dtDirectory Constructor        
        obj.dirpattern_ = dirpattern;
        [obj.dir_, obj.file_, obj.ext_] = fileparts( dirpattern );
      end
      function [ dirname ] = DirectoryName( obj )
% DirectoryName Get the name of the directory as string. Directory pattern
%               has to be unique.
        dirname = '';

        if ( isdir( obj.dirpattern_ ) )
          dirname = obj.dirpattern_;
        end
        
        if ( isempty( dirname ) )
          dirList = dir( obj.dirpattern_ );
          
          if ( size( dirList, 1 ) > 1 )
            throw(  ...
              MException( ...
                'dtDirectory:DirectoryName', ...
                 sprintf('> %s < is not a unique file or directory.', obj.dirpattern_) ...
              ) ...
            );
          else
            if ( ~dirList(1).isdir )
              throw(  ...
                MException( ...
                  'dtDirectory:DirectoryName', ...
                   sprintf('> %s < is not a directory.', obj.dirpattern_) ...
                ) ...
              );            
            end
          end
          dirname = [obj.dir_, '/', dirList(1).name];
        end
      end       
   end
end