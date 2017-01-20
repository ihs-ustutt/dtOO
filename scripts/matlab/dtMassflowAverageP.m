function [] = dtMassflowAverageP( filename, nBins )
  pres = dtValueScalarField( dtScalarField(filename ) ).Cyl
  pres_ca = dtCircAverage( pres, nBins )
  [tDir, tName, tSuffix] = fileparts(filename);
  
  p = dtPlot( tName, 'Arial', '10', '1', '1' );
  p.GiveSinglePlot( 8, 8, '$$ r \; [m] $$', '$$ p \; [ \frac{m^2}{s^2} ] $$');
  hold on;      
  grid on;
  plot( pres.R, pres.value_, 'k.', 'Color', p.lc_ );
  plot( pres_ca.binRMean_, pres_ca.binQ_(:,1), '-k', 'Linewidth', 2 );
  p.SaveJpg();
       
  pres_ca.WriteCSV( pres, 'Pressure [ Pa ]', '%16.8e' )
end