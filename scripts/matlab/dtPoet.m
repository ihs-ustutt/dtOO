classdef (Sealed) dtPoet < handle
   properties (SetAccess = private, GetAccess = public)
     poem_
     currentVerse_
   end
   methods (Access = private)
      function obj = dtPoet
% dtPoet Constructor        
        obj.poem_ = containers.Map('KeyType','char','ValueType','char');
        obj.currentVerse_ = '';
      end
   end
   methods (Static)
     function [ singleObj, rInit ] = Create( )
% Create Create singleton object. Initialize if empty, otherwise return existing
%   instance.
       rInit = 0;
       persistent localObj
       if isempty(localObj) || ~isvalid(localObj)
         localObj = dtPoet;
         rInit = 1;
         fprintf('Create new dtPoet\n');
       end
       singleObj = localObj;
     end
     function [] = Murder()
% Murder Kill current singleton       
       thisPoet = dtPoet.Create();
       if ~isempty(thisPoet) && isvalid(thisPoet)
         fprintf('Murder an existing dtPoet\n');
       end
       delete(thisPoet);
     end
     function [] = Add(key, type)
% Add Add a key to current verse if poet is in a verse state.
       thisPoet = dtPoet.Create();
       if isempty(thisPoet.currentVerse_)
         thisPoet.poem_( key ) = type;
       else
         thisPoet.poem_( strcat(thisPoet.currentVerse_, key) ) = type;
       end
     end
     function [] = AddGroup(keyG, typeG)
% AddGroup Add a group of keys to current verse if poet is in a verse state.
       thisPoet = dtPoet.Create();
       for i=1:length(typeG)
         key = strcat( keyG,'_',num2str(i) );
         type = typeG{i};
         if isempty(thisPoet.currentVerse_)
           thisPoet.poem_( key ) = type;
         else
           thisPoet.poem_( strcat(thisPoet.currentVerse_, key) ) = type;
         end
       end
     end     
     function [] = ChangeVerse( verse )
% ChangeVerse Change the current verse state.       
       thisPoet = dtPoet.Create();
       thisPoet.currentVerse_ = verse;
     end
     function [ val ] = GetFromVerse( verse, key )
% GetFromVerse Get a key from a specified verse
       thisPoet = dtPoet.Create();
       val = thisPoet.poem_( strcat(verse, key) );
     end      
     function [ val ] = GetGroupFromVerse( verse, keyG )
% GetFromVerse Get a key from a specified verse
       thisPoet = dtPoet.Create();
       val = {};
       i = 1;
       while true
         key = strcat( keyG, '_', num2str(i) );
         if thisPoet.poem_.isKey( strcat(verse, key) )
           val{i} = thisPoet.poem_( strcat(verse, key) );
           i = i+1;
         else
           break;
         end
       end
     end           
     function [ val ] = Get( key )
% Get Get a key from the current verse
       thisPoet = dtPoet.Create();
       val = thisPoet.poem_( strcat(thisPoet.currentVerse_, key) );
     end
     function [ ] = Recite( )
% Recite Recite the poet
       thisPoet = dtPoet.Create();
       keySet = keys( thisPoet.poem_ );
       valueSet = values( thisPoet.poem_ );
       
       for i=1:length(keySet)
         fprintf( 'poem_[ %s ] = %s\n', keySet{i}, valueSet{i} );
       end
     end     
   end
end