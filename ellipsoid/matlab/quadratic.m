function f = quadratic(x,j)
% example of a quadratic function
A = [1 2 3; 4 5 6; 7 8 9];
P = A'* A;
c = [1 3 5]';

switch (j)
  case 0,                             % objective function
    f = 0.5*(x - c)' * P * (x - c);
  case 1,                             % first constraint function
    f = ([-1 -2 -3]*x+8);
end
% END of quadratic
