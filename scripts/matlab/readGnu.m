function [ data ] = readGnu(filename, format);

  fprintf('Open file %s with format %s\n', filename, format);

  fid = fopen(filename);
  
  C = textscan(fid, format, 'Delimiter', '\n', 'CommentStyle','#');
  
  if isnan(C{1}); error('Nan is not yet supported'); end
  
  data = {};
  data{1} = [ C{1:end} ];
  
  fclose(fid); 
end