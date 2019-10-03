function design = write_random(c, r)

    W1 = c.W1 - r*c.W1 + 2*r*c.W1*rand(1,1);
    W3 = c.W3 - r*c.W3 + 2*r*c.W3*rand(1,1);
    W5 = c.W5 - r*c.W5 + 2*r*c.W5*rand(1,1);
    W6 = c.W6 - r*c.W6 + 2*r*c.W6*rand(1,1);
    W7 = c.W7 - r*c.W7 + 2*r*c.W7*rand(1,1);
    W8 = c.W8 - r*c.W8 + 2*r*c.W8*rand(1,1);
    rI = 0.05;
    I8 = c.I8 - rI*c.I8 + 2*rI*c.I8*rand(1,1);
    Cc = c.Cc - rI*c.Cc + 2*r*c.Cc*rand(1,1);

    design = struct(...
        'L', c.L, 'W1', W1, 'W3', W3, 'W5', W5, 'W6', W6,...
        'W7', W7, 'W8', W8, 'Rc', c.Rc, 'Cc', Cc, 'I8', I8...
        );
    
    write_design(design);
    
