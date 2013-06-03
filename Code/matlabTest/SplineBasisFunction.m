function u=SplineBasisFunction(l, k)
% Compute value of the k-th spline basis functions at points l (normalized to
% interval units on the grid).
% l an k are mxn matrices, where n is the number of points, and n is the
% dimensionality of the point.
%
% if size(k, 2)==1
%     k=k';
% end
% if size(k, 2) ~= size(l,2)
%     error('SplineBasisFunct:wrongInputs', 'Length of vector k must be equal to the number of columns in l')
% end
t=abs(l+1-k);
u=zeros(size(t))+((2-t).^3).*(t>=1 & t<2)+(4-6*t.^2+3*t.^3).*(t<1);

 %   u=(2-t).^3;
  %  u=4-6*t.^2+3*t.^3;
