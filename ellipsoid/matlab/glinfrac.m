function g = glinfrac(x, j)
% quasigradient of linear fractional function
a = [1 2 3]';
b = 4;
c = [5 6 7]';
d = 8;
f = linfrac(x,0);

switch (j)
  case 0,                      % gradient of objective function
    g = a - f*c;               % quasigradient
  case 1,                      % gradient of first constraint function
    g = -1*[5 6 7]';
end

% END of glinfrac
