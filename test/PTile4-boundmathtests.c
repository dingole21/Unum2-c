#include "PTile4-test.h"
#include <stdio.h>

void checkadditiveinverse(PBound *a, PBound *b){
  PBound inv_copy = __EMPTYBOUND;
  pcopy(&inv_copy, a);
  additiveinverse(&inv_copy);
  assert(eq(&inv_copy, b));
}

void checkaddition(PBound *a, PBound *b, PBound *res){
  PBound add_res = __EMPTYBOUND;
  add(&add_res, a, b);
  assert(eq(res, &add_res));
}

void checkmultiplicativeinverse(PBound *a, PBound *b){
  PBound inv_copy = __EMPTYBOUND;
  pcopy(&inv_copy, a);
  multiplicativeinverse(&inv_copy);
  assert(eq(&inv_copy, b));
}

void checkmultiplication(PBound *a, PBound *b, PBound *res){
  PBound mul_res = __EMPTYBOUND;
  mul(&mul_res, a, b);
  assert(eq(res, &mul_res));
}

void PTile4_bound_math_tests(){
  /****************************************************************************
   * ADDITION                                                                 *
   ****************************************************************************/

  /* PART 1:          */
  /* additive inverses*/

  //general additive inverse.
  PBound pb_0011_0100 = {pf0011, pf0100, STDBOUND};
  PBound pb_1100_1101 = {pf1100, pf1101, STDBOUND};
  //julia:  @test -(ooll → oloo) == (lloo → llol)
  checkadditiveinverse(&pb_0011_0100, &pb_1100_1101);

  //of things that cross zero.
  PBound pb_1110_0001 = {pf1110, pf0001, STDBOUND};
  PBound pb_1111_0010 = {pf1111, pf0010, STDBOUND};
  //julia:  @test -(lllo → oool) == (llll → oolo)
  checkadditiveinverse(&pb_0011_0100, &pb_1100_1101);

  //of things that cross infinity.
  PBound pb_0110_1011 = {pf0110, pf1011, STDBOUND};
  PBound pb_0101_1010 = {pf0101, pf1010, STDBOUND};
  //julia:  @test -(ollo → loll) == (olol → lolo)
  checkadditiveinverse(&pb_0110_1011, &pb_0101_1010);

  /* PART 2:          */
  /* actual addition  */

  //1: general addition with easy bounds.
  // equation to be tested:  (0.5 1] + (0.5 1] == (1 2]
  // julia:  @test (ooll → oloo) + (ooll → oloo) == (olol → ollo)
  PBound add_1_lft = {pf0011, pf0100, STDBOUND};
  PBound add_1_res = {pf0101, pf0110, STDBOUND};
  checkaddition(&add_1_lft, &add_1_lft, &add_1_res);

  //2: addition annihilates exact ends.
  // equation to be tested:  (0.5 1] + [0.5 1] == (1 2]
  // julia: (ooll → oloo) + (oolo → oloo) == (olol → ollo)
  PBound add_2_rgt = {pf0010, pf0100, STDBOUND};
  checkaddition(&add_1_lft, &add_2_rgt, &add_1_res);

  //3: addition annihilates exact ends, on the other side too.
  // equation to be tested:  [0.5 1] + (0.5 1] == (1 2]
  // julia: (oolo → oloo) + (ooll → oloo) == (olol → ollo)
  checkaddition(&add_2_rgt, &add_1_lft, &add_1_res);

  //4: addition annihilates exact ends, reversewise.
  // equation to be tested:  [0.5 1) + (0.5 1] == (1 2)
  // julia: (oolo → ooll) + (ooll → oloo) == ▾(olol)
  PBound add_4_lft = {pf0010, pf0011, STDBOUND};
  PBound add_4_res = {pf0101, pf0000, SINGLETON};
  checkaddition(&add_4_lft, &add_1_lft, &add_4_res);

  //5: addition compresses to the "many" interval.
  // equation to be tested:  (0 2) + [2] == (2 inf)
  // julia: (oool → olol) + ▾(ollo) == ▾(olll)
  PBound add_5_lft = {pf0001, pf0101, STDBOUND};
  PBound add_5_rgt = {pf0110, pf0000, SINGLETON};
  PBound add_5_res = {pf0111, pf0000, SINGLETON};
  checkaddition(&add_5_lft, &add_5_rgt, &add_5_res);

  //6: "inf" is an additive annihilator.
  // equation to be tested:  (0 2) + [inf] == [inf]
  // julia: (oool → olol) + ▾(looo) == ▾(looo)
  PBound add_6_lft = {pf0001, pf0101, STDBOUND};
  PBound add_6_rgt = {pf1000, pf0000, SINGLETON};
  PBound add_6_res = {pf1000, pf0000, SINGLETON};
  checkaddition(&add_6_lft, &add_6_rgt, &add_6_res);

  //7: addition against something that rounds infinity can result in allreals.
  // equation to be tested:  (2 -2) + (2 inf) == allreals
  // julia: (olll → lool) + ▾(olll) == ℝᵖ(PBound4)
  PBound add_7_lft = {pf0111, pf1001, STDBOUND};
  PBound add_7_rgt = {pf0111, pf0000, SINGLETON};
  PBound add_7_res = {pf0000, pf0000, ALLREALS};
  checkaddition(&add_7_lft, &add_7_rgt, &add_7_res);

  //8: addition can sometimes result in odd allreals.
  // equation to be tested:  (2 1] + [1] == allreals
  // julia: (olll → oloo) + ▾(oloo) == ℝᵖ(PBound4)
  PBound add_8_lft = {pf0111, pf0100, STDBOUND};
  PBound add_8_rgt = {pf0100, pf0000, SINGLETON};
  checkaddition(&add_8_lft, &add_8_rgt, &add_7_res);

  /****************************************************************************
   * MULTIPLICATION                                                           *
   ****************************************************************************/

  /* PART 1:                 */
  /* multiplicative inverses */

  //general multiplicative inverse.
  PBound pb_0100_0101 = {pf0100, pf0101, STDBOUND};
  //julia:  /(ooll → oloo) == (oloo → olol)
  checkmultiplicativeinverse(&pb_0011_0100, &pb_0100_0101);

  //when the source interval rounds zero.
  PBound pb_1100_0100 = {pf1100, pf0100, STDBOUND};
  PBound pb_0100_1100 = {pf0100, pf1100, STDBOUND};
  //julia:  /(lloo → oloo) == (oloo → lloo)
  checkmultiplicativeinverse(&pb_1100_0100, &pb_0100_1100);

  PBound pb_1110_0110 = {pf1110, pf0110, STDBOUND};
  PBound pb_0010_1010 = {pf0010, pf1010, STDBOUND};
  //julia:  /(lllo → ollo) == (oolo → lolo)
  checkmultiplicativeinverse(&pb_1110_0110, &pb_0010_1010);

  PBound pb_0100_0011 = {pf0100, pf0011, STDBOUND};
  PBound pb_0101_0100 = {pf0101, pf0100, STDBOUND};
  //julia:  /(oloo → ooll) == (olol → oloo)
  checkmultiplicativeinverse(&pb_0100_0011, &pb_0101_0100);

  /* PART 2:                 */
  /* actual multiplication   */

  //1: general multiplication with easy bounds.
  // equation to be tested:  (0.5 1] * (0.5 1] == (0 1]
  // julia:  @test (ooll → oloo) * (ooll → oloo) == (oool → oloo)
  PBound mul_1_lft = {pf0011, pf0100, STDBOUND};
  PBound mul_1_res = {pf0001, pf0100, STDBOUND};
  checkmultiplication(&mul_1_lft, &mul_1_lft, &mul_1_res);

  //2: flipping the sign.
  // equation to be tested:  (0.5 1] * [-1 0.5) == [-1 0)
  // julia:  @test (ooll → oloo) * (oloo → olol) == (lloo → llll)
  PBound mul_2_rgt = {pf1100, pf1101, STDBOUND};
  PBound mul_2_res = {pf1100, pf1111, STDBOUND};
  checkmultiplication(&mul_1_lft, &mul_2_rgt, &mul_2_res);

  //3: bound endpoint annihilation
  // equation to be tested:  (0.5 2) * [0.5 1] == (0 2)
  // julia:  @test (ooll → olol) * (oolo → oloo) == (oool → olol)
  PBound mul_3_lft = {pf0011, pf0101, STDBOUND};
  PBound mul_3_rgt = {pf0010, pf0100, STDBOUND};
  PBound mul_3_res = {pf0001, pf0101, STDBOUND};
  checkmultiplication(&mul_3_lft, &mul_3_rgt, &mul_3_res);

  //4: more bound endpoint annihilation
  // equation to be tested:  (0.5 2) * [0 0.5] == [0 1)
  // julia:  @test (oool → ollo) * (oooo → oool) == (oooo → ooll)
  PBound mul_4_lft = {pf0001, pf0110, STDBOUND};
  PBound mul_4_rgt = {pf0000, pf0001, STDBOUND};
  PBound mul_4_res = {pf0000, pf0011, STDBOUND};
  checkmultiplication(&mul_4_lft, &mul_4_rgt, &mul_4_res);

  //5: do bounds behave with unbounded open intervals?
  // equation to be tested:  (0.5 inf) * (0.5 2) == (0 inf
  // julia:  @test (ooll → olll) * (ooll → olol) == (oool → olll)
  PBound mul_5_lft = {pf0011, pf0111, STDBOUND};
  PBound mul_5_res = {pf0001, pf0111, STDBOUND};
  checkmultiplication(&mul_5_lft, &mul_4_lft, &mul_5_res);

  //6: do bounds behave with infinity-terminated intervals?
  // equation to be tested:  (0.5 inf] * (0.5 2) == (0 inf]
  // julia:  @test (ooll → looo) * (ooll → olol) == (oool → looo)
  PBound mul_6_lft = {pf0011, pf1000, STDBOUND};
  PBound mul_6_res = {pf0001, pf1000, STDBOUND};
  checkmultiplication(&mul_6_lft, &mul_4_lft, &mul_6_res);

  //7: basic multiplication on something that rounds zero?
  // equation to be tested:  [-2 2] * (0 1/2) == (-1 1)
  // julia:  @test (lolo → ollo) * ▾(oool) == (llol → ooll)
  PBound mul_7_lft = {pf1010, pf0110, STDBOUND};
  PBound mul_7_rgt = {pf0001, pf0000, SINGLETON};
  PBound mul_7_res = {pf1101, pf0011, STDBOUND};
  checkmultiplication(&mul_7_lft, &mul_7_rgt, &mul_7_res);

  //8: basic multiplication on something that rounds zero?
  // equation to be tested:  [-2 2) * (0 1] == [-2 2)
  // julia:  @test (lolo → olol) * (oool → oloo) == (lolo → olol)
  PBound mul_8_lft = {pf1010, pf0101, STDBOUND};
  PBound mul_8_rgt = {pf0001, pf0100, STDBOUND};
  PBound mul_8_res = {pf1010, pf0101, STDBOUND};
  checkmultiplication(&mul_8_lft, &mul_8_rgt, &mul_8_res);

  //9: basic multiplication on something that rounds zero?
  // equation to be tested:  [-2 2) * [-1 0] == [-2 2)
  // julia:  @test (lolo → olol) * (lloo → oooo) == (loll → ollo)
  PBound mul_9_rgt = {pf1100, pf0000, STDBOUND};
  PBound mul_9_res = {pf1011, pf0110, STDBOUND};
  checkmultiplication(&mul_8_lft, &mul_9_rgt, &mul_9_res);

  //10: basic multiplication on something that rounds zero?
  // equation to be tested: [-2 2) * inf == allreals
  // julia:  @test (lolo → ollo) * ▾(looo) == ℝᵖ(PBound4)
  PBound mul_10_lft = {pf1010, pf0110, STDBOUND};
  PBound mul_10_rgt = {pf1000, pf0000, SINGLETON};
  PBound mul_10_res = {pf0000, pf0000, ALLREALS};
  checkmultiplication(&mul_10_lft, &mul_10_rgt, &mul_10_res);

  //11: basic multiplication on something that rounds infinity?
  // equation to be tested: [2 -2] * [1/2 2) == [1 -1]
  // julia:  @test (ollo → lolo) * (oolo → ooll) == (oloo → lloo)
  PBound mul_11_lft = {pf0110, pf1010, STDBOUND};
  PBound mul_11_rgt = {pf0010, pf0011, STDBOUND};
  PBound mul_11_res = {pf0100, pf1100, STDBOUND};
  checkmultiplication(&mul_11_lft, &mul_11_rgt, &mul_11_res);

  //12: rounding infinity, with bound annihilation.
  // equation to be tested: (2 -2] * [1/2 2) == (1 -1]
  // julia:  @test (olll → lolo) * (oolo → ooll) == (olol → lloo)
  PBound mul_12_lft = {pf0111, pf1010, STDBOUND};
  PBound mul_12_res = {pf0101, pf1100, STDBOUND};
  checkmultiplication(&mul_12_lft, &mul_11_rgt, &mul_12_res);

  //13: rounding infinity, with bound status swapping because of sign changes.
  // equation to be tested: (2 -2] * (-2 -1/2] == (1 -1)
  // julia:  @test (olll → lolo) * (loll → lllo) == (oloo → loll)
  PBound mul_13_rgt = {pf1011, pf1110, STDBOUND};
  PBound mul_13_res = {pf0100, pf1011, STDBOUND};
  checkmultiplication(&mul_12_lft, &mul_13_rgt, &mul_13_res);

  //14:  rounding infinity, times zero is allreals.
  // equation to be tested: (2 -2] * [0] == allreals
  // julia:  @test (olll → lolo) * ▾(oooo) == ℝᵖ(PBound4)
  PBound mul_14_rgt = {pf0000, pf0000, SINGLETON};
  checkmultiplication(&mul_12_lft, &mul_14_rgt, &mul_10_res);

  //15:  rounding infinity, times a zero-bounded interval is allreals.
  // equation to be tested: (2 -2] * [0 1) == allreals
  // julia:  @test (olll → lolo) * (oooo → ooll) == ℝᵖ(PBound4)
  PBound mul_15_rgt = {pf0000, pf0011, STDBOUND};
  checkmultiplication(&mul_12_lft, &mul_15_rgt, &mul_10_res);

  //16:  Bounds that rounding infinity and zero become allreals, by zero mult
  // equation to be tested: [2 1/2] * [0] = allreals
  // julia:  @test (ollo → oolo) * ▾(oooo) == ℝᵖ(PBound4)
  PBound mul_16_lft = {pf0110, pf0010, STDBOUND};
  checkmultiplication(&mul_16_lft, &mul_14_rgt, &mul_10_res);

  //17:  Bounds that rounding infinity and zero become allreals, by inf mult
  // equation to be tested: [2 1/2] * [inf] = allreals
  // julia:  @test (ollo → oolo) * ▾(looo) == ℝᵖ(PBound4)
  PBound mul_17_rgt = {pf1000, pf0000, SINGLETON};
  checkmultiplication(&mul_16_lft, &mul_17_rgt, &mul_10_res);

  //18:  Bounds that rounding infinity and zero become allreals, by edge zero mult
  // equation to be tested: [2 1/2] * (0 1/2) = allreals
  // julia:  @test (ollo → oolo) * (oooo → oool) == ℝᵖ(PBound4)
  PBound mul_18_rgt = {pf0000, pf0001, STDBOUND};
  checkmultiplication(&mul_16_lft, &mul_18_rgt, &mul_10_res);

  //19:  Bounds that rounding infinity and zero become allreals, by inf mult
  // equation to be tested: [2 1/2] * (0 1/2) = allreals
  // julia:  @test (ollo → oolo) * (olll → looo) == ℝᵖ(PBound4)
  PBound mul_19_rgt = {pf0111, pf1000, STDBOUND};
  checkmultiplication(&mul_16_lft, &mul_19_rgt, &mul_10_res);

  //20:  Bounds that rounding infinity and zero become allreals, by zero mult
  // equation to be tested: [2 1/2] * (0 1/2) = allreals
  // julia:  @test (ollo → oolo) * (llll → oool) == ℝᵖ(PBound4)
  PBound mul_20_rgt = {pf1111, pf0001, STDBOUND};
  checkmultiplication(&mul_16_lft, &mul_20_rgt, &mul_10_res);

  //21:  Bounds that rounding infinity and zero become allreals, by inf mult
  // equation to be tested: [2 1/2] * (0 1/2) = allreals
  // julia:  @test (ollo → oolo) * (llll → oool) == ℝᵖ(PBound4)
  PBound mul_21_rgt = {pf0111, pf1001, STDBOUND};
  checkmultiplication(&mul_16_lft, &mul_21_rgt, &mul_10_res);

  //22: Bound mult can exclude single exacts.
  // equation to be tested: (2 1/2) * [1/2, 2] = R\[1]
  // julia:  @test (olll → oool) * (oolo → ollo) == (olol → ooll)
  PBound mul_22_lft = {pf0111, pf0001, STDBOUND};
  PBound mul_22_rgt = {pf0010, pf0110, STDBOUND};
  PBound mul_22_res = {pf0101, pf0011, STDBOUND};
  checkmultiplication(&mul_22_lft, &mul_22_rgt, &mul_22_res);

  //23: Wrap around to stitch together single exacts.
  // equation to be tested: [2 1/2] * [1/2, 2] = allreals
  // julia:  @test (ollo → oolo) * (oolo → ollo) == ℝᵖ(PBound4)
  PBound mul_23_lft = {pf0110, pf0010, STDBOUND};
  checkmultiplication(&mul_23_lft, &mul_22_rgt, &mul_10_res);

  //24: Wrap around to stitch together single exacts.
  // equation to be tested: [2 1/2] * [1/2, 2) = allreals
  // julia:  @test (ollo → oolo) * (oolo → olol) == ℝᵖ(PBound4)
  PBound mul_24_rgt = {pf0110, pf0101, STDBOUND};
  checkmultiplication(&mul_23_lft, &mul_24_rgt, &mul_10_res);

  //25: Wrap around to stitch together single exacts.
  // equation to be tested: [2 1/2) * [1/2, 2) = allreals
  // julia:  @test (ollo → oolo) * (oolo → olol) == ℝᵖ(PBound4)
  checkmultiplication(&mul_22_lft, &mul_24_rgt, &mul_10_res);

  //26: Wrap around to stitch together single exacts.
  // equation to be tested: (2 1/2) * [1/2, 2) = allreals
  // julia:  @test (olol → oolo) * (oolo → olol) == ℝᵖ(PBound4)
  PBound mul_26_lft = {pf0101, pf0010, STDBOUND};
  checkmultiplication(&mul_22_lft, &mul_24_rgt, &mul_10_res);
}
