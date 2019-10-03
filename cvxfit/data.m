% 3d example taken from ConvexFit web site
x1 = [0.028963,0.050093,0.084268,0.151581,0.288844,0.703774,1.730860 ];
x2 = [0.002568,0.003184,0.005181,0.006104 ];
x3 = [0.001500,0.003306,0.007287,0.016062,0.035404,0.078035,0.172000 ];
z(0+1,0+1,0+1) = 6.401900e-002;
z(0+1,0+1,1+1) = 7.648700e-002;
z(0+1,0+1,2+1) = 1.016700e-001;
z(0+1,0+1,3+1) = 1.563760e-001;
z(0+1,0+1,4+1) = 2.756570e-001;
z(0+1,0+1,5+1) = 5.353870e-001;
z(0+1,0+1,6+1) = 1.107990e+000;
z(0+1,1+1,0+1) = 6.481500e-002;
z(0+1,1+1,1+1) = 7.720700e-002;
z(0+1,1+1,2+1) = 1.026500e-001;
z(0+1,1+1,3+1) = 1.565290e-001;
z(0+1,1+1,4+1) = 2.750870e-001;
z(0+1,1+1,5+1) = 5.351400e-001;
z(0+1,1+1,6+1) = 1.108550e+000;
z(0+1,2+1,0+1) = 5.905600e-002;
z(0+1,2+1,1+1) = 6.958600e-002;
z(0+1,2+1,2+1) = 9.422200e-002;
z(0+1,2+1,3+1) = 1.560930e-001;
z(0+1,2+1,4+1) = 3.180510e-001;
z(0+1,2+1,5+1) = 7.459810e-001;
z(0+1,2+1,6+1) = 1.878290e+000;
z(0+1,3+1,0+1) = 5.974900e-002;
z(0+1,3+1,1+1) = 7.018000e-002;
z(0+1,3+1,2+1) = 9.505000e-002;
z(0+1,3+1,3+1) = 1.566320e-001;
z(0+1,3+1,4+1) = 3.186670e-001;
z(0+1,3+1,5+1) = 7.465990e-001;
z(0+1,3+1,6+1) = 1.878900e+000;
z(1+1,0+1,0+1) = 6.932400e-002;
z(1+1,0+1,1+1) = 8.172000e-002;
z(1+1,0+1,2+1) = 1.069020e-001;
z(1+1,0+1,3+1) = 1.616770e-001;
z(1+1,0+1,4+1) = 2.809160e-001;
z(1+1,0+1,5+1) = 5.415030e-001;
z(1+1,0+1,6+1) = 1.113340e+000;
z(1+1,1+1,0+1) = 6.914300e-002;
z(1+1,1+1,1+1) = 8.154000e-002;
z(1+1,1+1,2+1) = 1.068560e-001;
z(1+1,1+1,3+1) = 1.609840e-001;
z(1+1,1+1,4+1) = 2.806520e-001;
z(1+1,1+1,5+1) = 5.410210e-001;
z(1+1,1+1,6+1) = 1.113100e+000;
z(1+1,2+1,0+1) = 6.443800e-002;
z(1+1,2+1,1+1) = 7.495300e-002;
z(1+1,2+1,2+1) = 9.968000e-002;
z(1+1,2+1,3+1) = 1.614330e-001;
z(1+1,2+1,4+1) = 3.234350e-001;
z(1+1,2+1,5+1) = 7.514350e-001;
z(1+1,2+1,6+1) = 1.883760e+000;
z(1+1,3+1,0+1) = 6.414400e-002;
z(1+1,3+1,1+1) = 7.467800e-002;
z(1+1,3+1,2+1) = 9.932600e-002;
z(1+1,3+1,3+1) = 1.611540e-001;
z(1+1,3+1,4+1) = 3.231100e-001;
z(1+1,3+1,5+1) = 7.510320e-001;
z(1+1,3+1,6+1) = 1.883350e+000;
z(2+1,0+1,0+1) = 7.567300e-002;
z(2+1,0+1,1+1) = 8.822000e-002;
z(2+1,0+1,2+1) = 1.137810e-001;
z(2+1,0+1,3+1) = 1.680290e-001;
z(2+1,0+1,4+1) = 2.859500e-001;
z(2+1,0+1,5+1) = 5.474810e-001;
z(2+1,0+1,6+1) = 1.119250e+000;
z(2+1,1+1,0+1) = 7.459200e-002;
z(2+1,1+1,1+1) = 8.706600e-002;
z(2+1,1+1,2+1) = 1.124110e-001;
z(2+1,1+1,3+1) = 1.666580e-001;
z(2+1,1+1,4+1) = 2.854500e-001;
z(2+1,1+1,5+1) = 5.455990e-001;
z(2+1,1+1,6+1) = 1.118780e+000;
z(2+1,2+1,0+1) = 7.100100e-002;
z(2+1,2+1,1+1) = 8.153000e-002;
z(2+1,2+1,2+1) = 1.062200e-001;
z(2+1,2+1,3+1) = 1.680640e-001;
z(2+1,2+1,4+1) = 3.300940e-001;
z(2+1,2+1,5+1) = 7.581050e-001;
z(2+1,2+1,6+1) = 1.890440e+000;
z(2+1,3+1,0+1) = 6.979700e-002;
z(2+1,3+1,1+1) = 8.044400e-002;
z(2+1,3+1,2+1) = 1.054240e-001;
z(2+1,3+1,3+1) = 1.671110e-001;
z(2+1,3+1,4+1) = 3.291120e-001;
z(2+1,3+1,5+1) = 7.570410e-001;
z(2+1,3+1,6+1) = 1.889370e+000;
z(3+1,0+1,0+1) = 8.454900e-002;
z(3+1,0+1,1+1) = 9.721000e-002;
z(3+1,0+1,2+1) = 1.227180e-001;
z(3+1,0+1,3+1) = 1.767700e-001;
z(3+1,0+1,4+1) = 2.960340e-001;
z(3+1,0+1,5+1) = 5.565490e-001;
z(3+1,0+1,6+1) = 1.128900e+000;
z(3+1,1+1,0+1) = 8.310900e-002;
z(3+1,1+1,1+1) = 9.572200e-002;
z(3+1,1+1,2+1) = 1.213950e-001;
z(3+1,1+1,3+1) = 1.757510e-001;
z(3+1,1+1,4+1) = 2.947250e-001;
z(3+1,1+1,5+1) = 5.552720e-001;
z(3+1,1+1,6+1) = 1.127100e+000;
z(3+1,2+1,0+1) = 8.060400e-002;
z(3+1,2+1,1+1) = 9.131700e-002;
z(3+1,2+1,2+1) = 1.161530e-001;
z(3+1,2+1,3+1) = 1.780830e-001;
z(3+1,2+1,4+1) = 3.402170e-001;
z(3+1,2+1,5+1) = 7.682570e-001;
z(3+1,2+1,6+1) = 1.900630e+000;
z(3+1,3+1,0+1) = 7.858700e-002;
z(3+1,3+1,1+1) = 8.939200e-002;
z(3+1,3+1,2+1) = 1.145570e-001;
z(3+1,3+1,3+1) = 1.763000e-001;
z(3+1,3+1,4+1) = 3.383580e-001;
z(3+1,3+1,5+1) = 7.663400e-001;
z(3+1,3+1,6+1) = 1.898680e+000;
z(4+1,0+1,0+1) = 9.572600e-002;
z(4+1,0+1,1+1) = 1.088790e-001;
z(4+1,0+1,2+1) = 1.350500e-001;
z(4+1,0+1,3+1) = 1.891680e-001;
z(4+1,0+1,4+1) = 3.089350e-001;
z(4+1,0+1,5+1) = 5.696380e-001;
z(4+1,0+1,6+1) = 1.140370e+000;
z(4+1,1+1,0+1) = 9.370400e-002;
z(4+1,1+1,1+1) = 1.067800e-001;
z(4+1,1+1,2+1) = 1.328520e-001;
z(4+1,1+1,3+1) = 1.868370e-001;
z(4+1,1+1,4+1) = 3.057970e-001;
z(4+1,1+1,5+1) = 5.660700e-001;
z(4+1,1+1,6+1) = 1.137960e+000;
z(4+1,2+1,0+1) = 9.358000e-002;
z(4+1,2+1,1+1) = 1.048780e-001;
z(4+1,2+1,2+1) = 1.303000e-001;
z(4+1,2+1,3+1) = 1.920020e-001;
z(4+1,2+1,4+1) = 3.542050e-001;
z(4+1,2+1,5+1) = 7.822950e-001;
z(4+1,2+1,6+1) = 1.914700e+000;
z(4+1,3+1,0+1) = 9.064700e-002;
z(4+1,3+1,1+1) = 1.017890e-001;
z(4+1,3+1,2+1) = 1.271840e-001;
z(4+1,3+1,3+1) = 1.890300e-001;
z(4+1,3+1,4+1) = 3.511060e-001;
z(4+1,3+1,5+1) = 7.791680e-001;
z(4+1,3+1,6+1) = 1.911550e+000;
z(5+1,0+1,0+1) = 1.110700e-001;
z(5+1,0+1,1+1) = 1.249880e-001;
z(5+1,0+1,2+1) = 1.519470e-001;
z(5+1,0+1,3+1) = 2.072010e-001;
z(5+1,0+1,4+1) = 3.264700e-001;
z(5+1,0+1,5+1) = 5.860590e-001;
z(5+1,0+1,6+1) = 1.158750e+000;
z(5+1,1+1,0+1) = 1.063070e-001;
z(5+1,1+1,1+1) = 1.203760e-001;
z(5+1,1+1,2+1) = 1.478630e-001;
z(5+1,1+1,3+1) = 2.030340e-001;
z(5+1,1+1,4+1) = 3.206210e-001;
z(5+1,1+1,5+1) = 5.828040e-001;
z(5+1,1+1,6+1) = 1.153570e+000;
z(5+1,2+1,0+1) = 1.113660e-001;
z(5+1,2+1,1+1) = 1.237940e-001;
z(5+1,2+1,2+1) = 1.501030e-001;
z(5+1,2+1,3+1) = 2.117610e-001;
z(5+1,2+1,4+1) = 3.737710e-001;
z(5+1,2+1,5+1) = 8.020580e-001;
z(5+1,2+1,6+1) = 1.934480e+000;
z(5+1,3+1,0+1) = 1.067600e-001;
z(5+1,3+1,1+1) = 1.191580e-001;
z(5+1,3+1,2+1) = 1.457640e-001;
z(5+1,3+1,3+1) = 2.077550e-001;
z(5+1,3+1,4+1) = 3.697090e-001;
z(5+1,3+1,5+1) = 7.978290e-001;
z(5+1,3+1,6+1) = 1.930230e+000;
z(6+1,0+1,0+1) = 1.064890e-001;
z(6+1,0+1,1+1) = 1.234070e-001;
z(6+1,0+1,2+1) = 1.528400e-001;
z(6+1,0+1,3+1) = 2.099130e-001;
z(6+1,0+1,4+1) = 3.296260e-001;
z(6+1,0+1,5+1) = 5.903830e-001;
z(6+1,0+1,6+1) = 1.163260e+000;
z(6+1,1+1,0+1) = 9.707700e-002;
z(6+1,1+1,1+1) = 1.137850e-001;
z(6+1,1+1,2+1) = 1.438140e-001;
z(6+1,1+1,3+1) = 2.014310e-001;
z(6+1,1+1,4+1) = 3.205220e-001;
z(6+1,1+1,5+1) = 5.847500e-001;
z(6+1,1+1,6+1) = 1.155220e+000;
z(6+1,2+1,0+1) = 1.134080e-001;
z(6+1,2+1,1+1) = 1.292110e-001;
z(6+1,2+1,2+1) = 1.598390e-001;
z(6+1,2+1,3+1) = 2.234930e-001;
z(6+1,2+1,4+1) = 3.853140e-001;
z(6+1,2+1,5+1) = 8.133190e-001;
z(6+1,2+1,6+1) = 1.945910e+000;
z(6+1,3+1,0+1) = 1.062580e-001;
z(6+1,3+1,1+1) = 1.215140e-001;
z(6+1,3+1,2+1) = 1.519880e-001;
z(6+1,3+1,3+1) = 2.163060e-001;
z(6+1,3+1,4+1) = 3.787570e-001;
z(6+1,3+1,5+1) = 8.067960e-001;
z(6+1,3+1,6+1) = 1.939220e+000;
