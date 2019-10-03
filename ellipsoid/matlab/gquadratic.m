function g = gquadratic(x,j)
% example of a gradient of quadratic function
A = [1 2 3; 4 5 6; 7 8 9];
P = A'* A;
c = [1 3 5]';

switch (j)
  case 0,                      % gradient of objective function
    g = P*(x-c);
  case 1,                      % gradient of first constraint function
    g = [-1 -2 -3]';
end

% END of gfquadratic
