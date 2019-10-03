function phase=eval_phase(W1,W3,W5,W6,W7,W8,I8,Cc)
global c_phase a_phase_W1 a_phase_W3 a_phase_W5 a_phase_W6
global a_phase_W7 a_phase_W8 a_phase_I8 a_phase_Cc

phase = c_phase * W1^a_phase_W1 * W3^a_phase_W3 * W5^a_phase_W5...
      * W6^a_phase_W6 * W7^a_phase_W7 * W8^a_phase_W8...
        * I8^a_phase_I8 * Cc^a_phase_Cc;
