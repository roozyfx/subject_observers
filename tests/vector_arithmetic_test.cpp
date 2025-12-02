#include <gtest/gtest.h>

#include <limits>
#include <numeric>
#include <sstream>
#include <string>

#include "utils/vectormath.h"

TEST(VectorTest, BasicOperations) {
  fx::Vector x(1., 0., 0.);
  fx::Vector y(0., 1., 0.);
  fx::Vector z(0., 0., 1.);
  fx::Vector xy(1., 1., 0.);
  fx::Vector xz(1., 0., 1.);
  fx::Vector yz(0., 1., 1.);
  EXPECT_TRUE((x + y).IsAlmostEqual(xy));
  EXPECT_TRUE((x + z).IsAlmostEqual(xz));
  EXPECT_TRUE((z + y).IsAlmostEqual(yz));

  fx::Vector v(4., 3., 2.);
  fx::Vector u(2., 3., 4.);

  fx::Vector vcopy(v);
  fx::Vector vOpAssign(v);
  EXPECT_TRUE(vcopy.IsAlmostEqual(v));
  EXPECT_TRUE(vOpAssign.IsAlmostEqual(vcopy));

  EXPECT_TRUE((v + u).IsAlmostEqual(fx::Vector(6., 6., 6.)));
  v += u;
  EXPECT_TRUE(v.IsAlmostEqual(fx::Vector(6., 6., 6.)));

  v = vcopy;
  float k = 3.5f;
  EXPECT_TRUE((k * v).IsAlmostEqual(v * k));
  EXPECT_TRUE((k * v).IsAlmostEqual(fx::Vector(14., 10.5, 7.)));
  v *= k;

  EXPECT_TRUE(v.IsAlmostEqual(fx::Vector(14., 10.5, 7.)));
  EXPECT_EQ(vOpAssign[0], 4.);
  EXPECT_EQ(vOpAssign[1], 3.);
  EXPECT_EQ(vOpAssign[2], 2.);
  std::stringstream ss;
  ss << vOpAssign;
  std::string str = ss.str();
  EXPECT_STREQ(str.c_str(), "4, 3, 2");
}

TEST(PointTest, BasicOperations) {
  fx::Point p(-4., 0., 12.);
  fx::Point q(-2., .0001, -2.);

  fx::Point pcopy(p);
  fx::Point pOpAssign(p);
  EXPECT_TRUE(pcopy.IsAlmostEqual(p));
  EXPECT_TRUE(pOpAssign.IsAlmostEqual(pcopy));

  fx::Vector v(1., 10., 100.);
  p += 10 * v;
  EXPECT_TRUE(p.IsAlmostEqual(fx::Point(6., 100., 1012.)));
  p += 0 * v;
  EXPECT_TRUE(p.IsAlmostEqual(fx::Point(6., 100., 1012.)));
}