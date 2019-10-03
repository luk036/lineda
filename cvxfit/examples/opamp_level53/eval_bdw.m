function bdw=eval_bdw(W1,W3,W5,W6,W7,W8,I8,Cc)
global c_bdw a_bdw_W1 a_bdw_W3 a_bdw_W5 a_bdw_W6
global a_bdw_W7 a_bdw_W8 a_bdw_I8 a_bdw_Cc

bdw = c_bdw * W1^a_bdw_W1 * W3^a_bdw_W3 * W5^a_bdw_W5...
      * W6^a_bdw_W6 * W7^a_bdw_W7 * W8^a_bdw_W8 * I8^a_bdw_I8 * Cc^a_bdw_Cc;
