function vod5=eval_vdsat5(c,a_W8,a_Ibias, W8,Ibias)
    vod5 = c * W8^a_W8 * Ibias^a_Ibias;
