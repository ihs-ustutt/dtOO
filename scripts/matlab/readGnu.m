function [ data ] = readGnu(filename, format);
fprintf('Open file %s with format %s\n', filename, format);
fid = fopen(filename);

newline = sprintf('\n');
line = fgets(fid);
i = 0;
j = 1;
k = 0;
data = {};
data{1} = zeros(0,0);
while ischar(line)
  i = i + 1;
    if strcmp(newline, line)
%      disp( [num2str( size(data, 2) ), ' == ', num2str(j)] );  
      if size( data, 2 ) == j
        j = j + 1;
        k = 0;
      end
%      disp( [num2str(i), ' Empty line: increase j to ', num2str(j)] );  
    else
%        disp( [num2str(i), 'Non-empty line'] );
      if strfind(line, '#')
%        disp('Comment');
      else
        k = k + 1;
        tmp = sscanf(line, format);
        for l=1:size(tmp)
          data{j}(k, l) = tmp(l);
        end
%        data{j}(k, 2) = tmp(2);
%        data{j}(k, 3) = tmp(3);
      end
    end
    line = fgets(fid);
end
fclose(fid);

%data