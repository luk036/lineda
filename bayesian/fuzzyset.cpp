#include "fuzzyset.hpp"
#include "membership.hpp"

/** Examples taken from the book "AI for game developers" TP311.52/B772 */
void example_fuzzyset() {
  fzset<gradeFunc> Low(gradeFunc(40.5, 60.4));
  fzset<reverseGradeFunc> High(reverseGradeFunc(40.5, 60.4));

  // Inputs
  double x = 50.6;
  double y = 46.6;

  fzlogic a = Low(x) && Low(y) || VERY(High(x)) && !(High(y));
  fzlogic b = x > 40.7 || Low(x + y) && NOT_VERY(a); // mixed

  fzset<gradeFunc> Overweight(gradeFunc(175., 195.));
  double w = 170; // input

  fzlogic active(0.8);
  fzlogic exercise = Overweight(w) && !(active);
  fzlogic moderate_diet = Overweight(w) && active;

  fzlogic in_melee_range(0.5);
  fzlogic uninjured(0.8);
  fzlogic hard(0.2);
  fzlogic out_of_range(0.3);
  fzlogic wimp(0.2);

  fzlogic attack = (in_melee_range && uninjured) && !(hard);
  fzlogic do_nothing = (!in_melee_range) && uninjured;
  fzlogic flee = (!out_of_range && !uninjured) && (!wimp);
}
