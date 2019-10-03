function vod1=eval_vdsat1(c,a_W1,a_W5,a_W8,a_Ibias, W1,W5,W8,Ibias)
    vod1 = c * W1^a_W1 * W5^a_W5 * W8^a_W8 * Ibias^a_Ibias;
