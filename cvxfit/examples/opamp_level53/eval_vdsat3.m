function vod3=eval_vdsat3(c,a_W3,a_W5,a_W8,a_Ibias, W3,W5,W8,Ibias)
    vod3 = c * W3^a_W3 * W5^a_W5 * W8^a_W8 * Ibias^a_Ibias;
