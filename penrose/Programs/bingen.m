function B = bingen(n)
% Generate all binary sequences of length n in lexicographical order
B = [0; 1];
c0 = 0; 
c1 = 1;
for i=1:n-1,
   c0 = [c0;c0];
   c1 = [c1;c1];
   B = [c0,B; c1,B];
end
