classdef dtGroupedCircAverageVector3Plot < dtPlot
  properties
  end
  methods
    function obj = dtGroupedCircAverageVector3Plot( filename, varargin )    
      obj@dtPlot(filename, varargin);
    end
    function [ ] = ShowGrouped( obj, circAverage, nX, nY, varargin )
% ShowGrouped Show subplot of all draft tube inlet profiles
%   with dimension.
%   states : Cell array of strings of states (e.g. {'X1_4', 'X1_9'})
%   nX : dimension of subplot
%   nY : dimension of subplot
%   c1Range : y range of first component plot
%   c2Range : y range of second component plot
%   c3Range : y range of third component plot
      cRange = {};
      cLabel = {};
      if ( length(varargin)==7 )
        cRange{1} = str2num( varargin{1} );
        cRange{2} = str2num( varargin{2} );
        cRange{3} = str2num( varargin{3} );
        xLabel = varargin{4};
        cLabel{1} = varargin{5};
        cLabel{2} = varargin{6};
        cLabel{3} = varargin{7};
      elseif ( length(varargin)==3 )
        cRange{1} = str2num( varargin{1} );
        xLabel = varargin{2};
        cLabel{1} = varargin{3};
      else
        throw(  ...
          MException( ...
            'dtGroupedCircAverageVector3Plot:ShowGrouped', ...
            sprintf('Wrong number of input arguments.') ...
          ) ...
        );        
      end
      
      dtP = dtPlot.empty(0); 
      for i=1:length(cLabel)
        dtP(i) = dtPlot('');
        dtP(i).GiveSinglePlot(7,7,'','');
      end
     
      axC = gobjects(3, nX, nY);
      %for curInd=1:length(circAverage)
      curInd = 1;
      while ( curInd <= length(circAverage) )
        for i=1:nX
          for j=1:nY
            for k=1:length(cLabel)            
              dtP(k).MakeCurrent();
              axC(k, i, j) = subplot( nX, nY, (i-1) * nY + j );
              hold on;        
              plot( ...
                circAverage(curInd).binRMean_, circAverage(curInd).binQ_(:,k), ...
                'k-' ...
              );
              hold off;
            end           
            curInd = curInd + 1;
            if (curInd > length(circAverage)); break; end
          end
          if (curInd > length(circAverage)); break; end
        end
      end
      xRange ...
      = ...
      [ min( circAverage(1).binRMean_ ) max( circAverage(1).binRMean_ ) ];
      for i=1:nX
        for j=1:nY
          for k=1:length(cLabel)          
            axC(k,i,j).XGrid = 'on';
            axC(k,i,j).YGrid = 'on';

            axC(k,i,j).XAxis.TickValues = [];            
            axC(k,i,j).XAxis.Limits = xRange;
            if ( ~isempty( cRange{k} ) )
              axC(k,i,j).YAxis.TickValues = cRange{k};            
              axC(k,i,j).YAxis.Limits = [ min(cRange{k}) max(cRange{k}) ];
            end

            axC(k,i,j).XLabel.Interpreter = 'latex';
            axC(k,i,j).YLabel.Interpreter = 'latex';

            axC(k,i,j).XLabel.String = xLabel;
            axC(k,i,j).YLabel.String = cLabel{k};
          end
        end
      end
      for i=1:length(cLabel)
        dtP(i).Show;
      end
    end   
    function [ ] = ShowGroupedWithNumbers( obj, circAverage, nX, nY, varargin )
% ShowGrouped Show subplot of all draft tube inlet profiles
%   with dimension.
%   states : Cell array of strings of states (e.g. {'X1_4', 'X1_9'})
%   nX : dimension of subplot
%   nY : dimension of subplot
%   c1Range : y range of first component plot
%   c2Range : y range of second component plot
%   c3Range : y range of third component plot
      cRange = {};
      cLabel = {};
      if ( length(varargin)==7 )
        cRange{1} = str2num( varargin{1} );
        cRange{2} = str2num( varargin{2} );
        cRange{3} = str2num( varargin{3} );
        xLabel = varargin{4};
        cLabel{1} = varargin{5};
        cLabel{2} = varargin{6};
        cLabel{3} = varargin{7};
      elseif ( length(varargin)==3 )
        cRange{1} = str2num( varargin{1} );
        xLabel = varargin{2};
        cLabel{1} = varargin{3};
      else
        throw(  ...
          MException( ...
            'dtGroupedCircAverageVector3Plot:ShowGrouped', ...
            sprintf('Wrong number of input arguments.') ...
          ) ...
        );        
      end
      
      dtP = dtPlot.empty(0); 
      for i=1:length(cLabel)
        dtP(i) = dtPlot('');
        dtP(i).GiveSinglePlot(7,7,'','');
      end
     
      axC = gobjects(3, nX, nY);
      %for curInd=1:length(circAverage)
      curInd = 1;
      while ( curInd <= length(circAverage) )
        for i=1:nX
          for j=1:nY
            for k=1:length(cLabel)            
              dtP(k).MakeCurrent();
              axC(k, i, j) = subplot( nX, nY, (i-1) * nY + j );
              hold on;        
              plot( ...
                circAverage(curInd).binRMean_, circAverage(curInd).binQ_(:,k), ...
                'k:' ...
              );
%              plot(x,y,'-r')
              hold on
              for ii = 1:length( circAverage(curInd).binRMean_ )
                  text( ...
                    circAverage(curInd).binRMean_(ii), ...
                    circAverage(curInd).binQ_(ii,k), ...
                    num2str(curInd),'Color','k' ...
                  );
              end            
              hold off;
            end           
            curInd = curInd + 1;
            if (curInd > length(circAverage)); break; end
          end
          if (curInd > length(circAverage)); break; end
        end
      end
      xRange ...
      = ...
      [ min( circAverage(1).binRMean_ ) max( circAverage(1).binRMean_ ) ];
      for i=1:nX
        for j=1:nY
          for k=1:length(cLabel)          
            axC(k,i,j).XGrid = 'on';
            axC(k,i,j).YGrid = 'on';

            axC(k,i,j).XAxis.TickValues = [];            
            axC(k,i,j).XAxis.Limits = xRange;
            if ( ~isempty( cRange{k} ) )
              axC(k,i,j).YAxis.TickValues = cRange{k};            
              axC(k,i,j).YAxis.Limits = [ min(cRange{k}) max(cRange{k}) ];
            end

            axC(k,i,j).XLabel.Interpreter = 'latex';
            axC(k,i,j).YLabel.Interpreter = 'latex';

            axC(k,i,j).XLabel.String = xLabel;
            axC(k,i,j).YLabel.String = cLabel{k};
          end
        end
      end
      for i=1:length(cLabel)
        dtP(i).Show;
      end
    end       
  end
end