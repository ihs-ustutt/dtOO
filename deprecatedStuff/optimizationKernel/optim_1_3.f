c      program Optimierer
      subroutine f_optim_1_3()
c Arbeitet noch nicht parallel
c
      implicit none

      integer iiter,niter
      integer npar
      integer n,nred,nsimp1,nsimp2,ngen,nzuf,npp,restart

      integer i,j,ii
      integer tmp

      integer mpar,mset
      parameter (mpar=50,mset=100)
      
      real pamin,pamax,pa,x,xx,erg,eps,xxp,k

      character*70 text

      real schwer,rad,radmax
      dimension pamin(mpar),pamax(mpar),pa(mset,mpar),xx(mset,mpar)
      dimension erg(mset),schwer(mpar)


c Steuerfile einlesen
      call stfein(mpar,npar,pamin,pamax,ii,
     * n,nred,mset,niter,eps,restart)

      npp=(n-nred)/6
      nsimp1=2*npp
      nsimp2=npp
      ngen=2*npp

      nzuf=n-nred-nsimp1-nsimp2-ngen
      Write(*,*) n,nred,ngen,nsimp1,nsimp2,nzuf,eps,npar

      iiter=1

c Konditionierung von rand() mit ii*n
         do i=1,ii*n*npar
            x=rand()
         end do
      text="./opti_shell "//char(0)


c Protokollfile, darin werden die Datensätze gespeichert
      if(restart.eq.1) then
         open(20,file="Protokoll",STATUS='unknown',POSITION='APPEND')
      else
         open(20,file="Protokoll")
      end if


c==========================================
c Start der Optimierung
c==========================================

c Erster Parameter-Satz
c restart == 0: Erster Parameter-Satz ganz zufaellig
      if (restart.eq.0) then
         open(10,file="opti_para_set.dat")
         write(10,*) n
         do i=1,n
            do j=1,npar
               x=rand()
               xx(i,j)=x
               pa(i,j)=pamin(j)+x*(pamax(j)-pamin(j))
            end do

            write(10,*) npar
            do j=1,npar
               write(10,*) pa(i,j)
            end do  
         end do
         close (10)

c==========================================
c Berechnung des ersten Parameter-Satzes

c          call system(text)
      call c_optimizer_callback()      
c==========================================

          open(10,file="opti_erg.dat")
          do i=1,n
             read(10,*) erg(i)
             end do   
          close(10)

c Sortieren der Parametersaetze nach der Qualitaetsfunktion
          call sorti(pa,erg,xx,n,npar,mset,mpar)

c Protokollfile
          write(20,*) "Zufaelliger Satz"
          do i=1,n
             write(20,*) erg(i),(pa(i,j),j=1,npar)
          end do

c restart == 1: Einlesen des letzten, überlebten Parameter-Satzes
c der vorherigen Optimierung (opti_aktuell.dat)
      else 
         open(10,file="opti_aktuell.dat")
         read(10,*) iiter
         read(10,*) tmp
         do i=1,n
             read(10,*) tmp
             read(10,*) erg(i)
             do j=1,npar
                  read(10,*) pa(i,j)
                  xx(i,j)=(pa(i,j)-pamin(j))/(pamax(j)-pamin(j))
            end do  
         end do
         close (10)

c Konditionierung von rand() damit gleicher Iterverlauf wie ohne restart
         tmp=n*npar+(iiter-1)*npar*(ngen+nsimp2+nzuf)
         do i=1,tmp
             x=rand()
         end do

      end if



c==========================================
c Beginn Optimierungsschleife
c==========================================

100   continue

c Abpruefung der Iterationsanzahl
      if(iiter.gt.niter) then
         Write(*,*) "Iterationsanzahl ueberschritten"
c         stop
         goto 200
      end if


c Schreiben der letzten Generation auf File opti_aktuell.dat

      open(30,file="opti_aktuell.dat")
      write(30,*) iiter
      write(30,*) nred
      do i=1,n
         write(30,*) i
         write(30,*) erg(i)
         do j=1,npar
           write(30,*) pa(i,j)
         end do
      end do
      close(30)


c Berechnung des Schwerpunktes der ueberlebenden Saetze

      do j=1,npar
         schwer(j)=0.
      end do
      do i=1,nred
         do j=1,npar
            schwer(j)=schwer(j)+xx(i,j)
         end do
      end do
      do j=1,npar
         schwer(j)=schwer(j)/float(nred)
      end do


c Berechnung des maximalen Streu-Radius vom Schwerpunkt
      radmax=0.

      do i=1,nred

         rad=0.
         do j=1,npar
            rad=rad+(xx(i,j)-schwer(j))**2
         end do
         rad=sqrt(rad)

         if(radmax.lt.rad) radmax=rad
      end do

      write(*,*) "Iter:",iiter," Maximaler Radius",radmax
      write(*,*) "Bester Wert:",erg(1),(pa(1,j),j=1,npar)

c Ausgabe in der Protokolldatei
      write(20,*) "Iteration",iiter," Maximaler Radius",radmax
      do i=1,n
         write(20,*) erg(i),(pa(i,j),j=1,npar)
      end do

        
c
c Alle in engem Radius => Konvergenz
      if(radmax.lt.eps) goto 200

c
c==========================================
c Neue Parameter

      iiter=iiter+1

c Neue Parameter nach der Evolutionstheorie
c Reproduktion
      open(10,file="opti_para_set.dat")
      write(10,*) n-nred
      do i =1,ngen
         do j=1,npar
            xxp=rand()
            x=xx(i,j)*xxp+(1-xxp)*xx(i+1,j)
            if(x.lt.0) x=0.
            if(x.gt.1) x=1.
            xx(nred+i,j)=x
            pa(nred+i,j)=pamin(j)+x*(pamax(j)-pamin(j))
         end do
         write(10,*) npar
         do j=1,npar
            write(10,*) pa(i+nred,j)
         end do
      end do


c Neue Parameter Nach SIMPLEX
c
      do i=1,nsimp1
         do j=1,npar
            x=2.*schwer(j)-xx(i+nred+ngen,j)
            if(x.lt.0) x=0.
            if(x.gt.1) x=1.

            xx(nred+ngen+i,j)=x
            pa(nred+ngen+i,j)=pamin(j)+x*(pamax(j)-pamin(j))
         end do
        
         write(10,*) npar
         do j=1,npar
            write(10,*) pa(i+nred+ngen,j)
         end do
            
      end do

      do i=1,nsimp2
         do j=1,npar
            xxp=(rand()-.5)*2.
            x=xx(1,j)+2*radmax*xxp*sqrt(abs(xxp))
            if(x.lt.0) x=0.
            if(x.gt.1) x=1.

            xx(nred+ngen+nsimp1+i,j)=x
            pa(nred+ngen+nsimp1+i,j)
     *       =pamin(j)+x*(pamax(j)-pamin(j))
         end do
        
         write(10,*) npar
         do j=1,npar
            write(10,*) pa(i+nred+nsimp1+ngen,j)
          end do
            
       end do



c Neue Parameter zufaellig im gesamten Gebiet
      do i=1,nzuf
         do j=1,npar
            x=rand()
            xx(nred+ngen+nsimp1+nsimp2+i,j)=x
            pa(nred+ngen+nsimp1+nsimp2+i,j)
     *       =pamin(j)+x*(pamax(j)-pamin(j))
         end do

         write(10,*) npar
         do j=1,npar
            write(10,*) pa(i+nred+nsimp1+nsimp2+ngen,j)
         end do
      end do
      close(10)

c==========================================
c Berechnung der Parameter-Saetze

c      call system(text)
      call c_optimizer_callback()
c==========================================
 
c Einlesen der Ergebnisse
      open(10,file="opti_erg.dat")
      do i=1,n-nred
         read(10,*) erg(i+nred)
      end do
      close(10)

c Sortieren der Parametersaetze nach der Qualitaetsfunktion
c die ersten NRED Saetze ueberleben
      call sorti(pa,erg,xx,n,npar,mset,mpar)
      
      goto 100

200   continue        
      close (20)
      end



c
c Einlesen des Steuerfiles
c
       subroutine stfein(mpar,npar,pamin,pamax,ii,n,nred,mset,
     * niter,eps,restart)

      implicit none
      integer i,mpar,npar,ii,n,nred,mset,niter,restart
      real pamin,pamax,eps
      dimension pamin(mpar),pamax(mpar)

      open(10,file="opti.stf",status='old')

c Anzahl Parameter npar, npar darf nicht groeszer sein als mpar
c 
      read (10,*) npar
      if(npar.gt.mpar) then
         write(*,*) "Zu viele Parameter, Dimension aendern!"
c         stop
         goto 400
      end if

c Parametergrenzen MIN MAX
      do i=1,npar
         read(10,*) pamin(i),pamax(i)
      end do

c Startzahl fuer Zufallsgenerator
      read(10,*) ii

c Generationsgroesse n, n darf nicht groeszer sein als mset
      read(10,*) n
      if(n.gt.mset) then
         write(*,*) "zu viele Samples, Dimensioniert:",mset
c         stop
         goto 400
      end if

c Survival Anzahl nred, 1 < nred < n 
      read (10,*) nred
      if(nred.ge.n.or.nred.lt.1) then
         write(*,*) "Survival Rate falsch!"
c         stop
         goto 400
      end if

c Iterationsanzahl
      read(10,*) niter

c Abbruchkriterium
      read(10,*) eps

c Restart 0=nein; 1=ja
      read(10,*) restart

      close (10)
400   continue      
      return
      end 

      subroutine sorti(pa,erg,xx,n,npar,mset,mpar)

      implicit none

      real xx,pa,erg,paz,ergz,xxz,ex
      integer n,npar,mset,mpar,i,j,jj
      dimension erg(mset),xx(mset,mpar)
      dimension pa(mset,mpar)

      do i=1,n
         jj=i
         ex=erg(i)
         do j=i,n
            if(erg(j).lt.ex) then
               jj=j
               ex=erg(j)
            end if  
         end do

         ergz=erg(i)
         erg(i)=erg(jj)
         erg(jj)=ergz

         do j=1,npar
            paz=pa(i,j)
            xxz=xx(i,j)
            pa(i,j)=pa(jj,j)
            xx(i,j)=xx(jj,j)
            pa(jj,j)=paz 
            xx(jj,j)=xxz
         end do
      end do
      return
      end