function f = linfrac(x, j)
% example of a quasi-convex function
a = [1 2 3]';
b = 4;
c = [5 6 7]';
d = 8;

switch (j)
  case 0,                             % objective function
    f = (a'*x + b) / (c'*x + d);
  case 1,                             % first constraint function
    f = 3 - ([5 6 7]*x+8);
end

% END of linfrac
