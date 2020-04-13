#include "TestHarness.h"
#include "DgRNG_Global.h"
#include "DgRNG_Local.h"

TEST(Stack_RNG_Local, creation_RNG_Local)
{
  Dg::RNG_Local rng;
  rng.SetSeed(34395849);
  rng.SetSeed(34395849, 987562);
  bool coin = rng.FlipCoin();
  uint32_t val = rng.GetUint();
  val = rng.GetUniform(1, 5432);
  float fval = rng.GetGamma<float>(1.0f, 2.0f);
  fval = rng.GetNormal<float>();
  fval = rng.GetNormal<float>(2.0f, 0.5f);
}

TEST(Stack_RNG_Global, creation_RNG_Global)
{
  Dg::RNG_Global rng;
  rng.SetSeed(34395849);
  rng.SetSeed(34395849, 987562);
  bool coin = rng.FlipCoin();
  uint32_t val = rng.GetUint();
  val = rng.GetUniform(1, 5432);
  float fval = rng.GetGamma<float>(1.0f, 2.0f);
  fval = rng.GetNormal<float>();
  fval = rng.GetNormal<float>(2.0f, 0.5f);
}