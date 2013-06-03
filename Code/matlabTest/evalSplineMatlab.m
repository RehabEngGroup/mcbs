function [lmt ma]=evalSplineMatlab(C, x)
noMuscles=length(C);

if ndims(x)~=2 
  error('evalSplineMatlab:wrongInputs', 'x must be a matrix')
end
noSamples=size(x, 1);
lmt=zeros(noSamples, noMuscles);
if nargout >1
  ma=zeros(noSamples, noMuscles, size(x,2));
end

for musc=1:noMuscles
    %disp(['Muscle' num2str(musc)])
    noDof=length(C(musc).n);
    if size(x, 2)>noDof
      % issue warning?
    elseif size(x, 2)<noDof
      % error
    end
    %check for lengths of a, b, c?
      h=(C(musc).b-C(musc).a)./double(C(musc).n);
    positionInInterval=(x(:,[noDof:-1:1])-repmat(C(musc).a, noSamples, 1))./repmat(h, noSamples, 1);
    noCoeffs=length(C(musc).C);
    if noCoeffs~=prod(double(C(musc).n)+3)
%error
end
    indicesC=zeros(noCoeffs, noDof);
    indicesC(:,1)=mod(0:size(indicesC, 1)-1, double(C(musc).n(1))+3);
    for i=2:noDof
        indicesC(:,i)=mod(floor((0:noCoeffs-1)/prod((double(C(musc).n(1:i-1))+3))), (double(C(musc).n(i))+3)); 
    end
  
    minIndexBasis=floor(positionInInterval);
    maxIndexBasis=min(positionInInterval, repmat(double(C(musc).n), noSamples,1))+3;

    for j=1:noSamples
        indicesOk=true(noCoeffs, 1);
        for i=1:noDof
            indicesOk=indicesOk & (indicesC(:, i)<maxIndexBasis(j,i) & indicesC(:, i)>=minIndexBasis(j,i));
        end
        indicesOkList=find(indicesOk);
        for k=1:length(indicesOkList)
            lmt(j, musc)=lmt(j, musc)+C(musc).C(indicesOkList(k))*prod(SplineBasisFunction(positionInInterval(j,:), indicesC(indicesOkList(k),:)));
	    if nargout>1
	      for i=1:noDof
		      complemIndices=(1:noDof)~=i;
ma(j, musc, i)=ma(j, musc, i)+C(musc).C(indicesOkList(k))*prod(SplineBasisFunction(positionInInterval(j,complemIndices), indicesC(indicesOkList(k),complemIndices)))*SplineBasisDerivative(positionInInterval(j, i), indicesC(indicesOkList(k), i))/h(i);	      
end %for i
	    end %if calculate ma
        end % for k indicesOk
    end % for j noSamples
end %for musc
