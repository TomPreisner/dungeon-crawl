#include <gtest/gtest.h>
#include <Eigen/Dense>

TEST(EigenTest, VectorTest) {
  // Vector creation types
  Eigen::Vector3d vec3(1,2,3);
  EXPECT_EQ(vec3[0], 1);
  EXPECT_EQ(vec3[1], 2);
  EXPECT_EQ(vec3[2], 3);
  
  Eigen::VectorXd vec4(4);
  vec4 << 4, 5, 6, 1;
  EXPECT_EQ(vec4[0], 4);
  EXPECT_EQ(vec4[1], 5);
  EXPECT_EQ(vec4[2], 6);
  EXPECT_EQ(vec4[3], 1);
  
  Eigen::VectorXd vec4_a{{7, 8, 9, 2}}; //< no single {} brackets allowed
  EXPECT_EQ(vec4_a[0], 7);
  EXPECT_EQ(vec4_a[1], 8);
  EXPECT_EQ(vec4_a[2], 9);
  EXPECT_EQ(vec4_a[3], 2);

  // Vector simple math operations
  Eigen::Vector3d v(1, 2, 3);
  Eigen::Vector3d w(1, 0, 0);
  Eigen::Vector3d res = -v + w -v;
  EXPECT_EQ(res[0], -1);
  EXPECT_EQ(res[1], -4);
  EXPECT_EQ(res[2], -6);

  res = 0.1 * v;
  EXPECT_FLOAT_EQ(res[0], 0.1);
  EXPECT_FLOAT_EQ(res[1], 0.2);
  EXPECT_FLOAT_EQ(res[2], 0.3);

  v *= 2;
  EXPECT_EQ(v[0], 2);
  EXPECT_EQ(v[1], 4);
  EXPECT_EQ(v[2], 6);
  
  //  Vector dot and cross products
  Eigen::Vector3d t(1, 2, 3);
  Eigen::Vector3d s(0, 1, 2);

  float dot = t.dot(s);
  EXPECT_FLOAT_EQ(dot, 8.0);
  EXPECT_EQ(t.adjoint() * s, 8);  // automatic conversion of the inner product to a scalar
 
  Eigen::Vector3d cross = t.cross(s);
  EXPECT_EQ(cross[0], 1);
  EXPECT_EQ(cross[1], -2);
  EXPECT_EQ(cross[2], 1);
  
  Eigen::Vector2d t2(1, 2);
  Eigen::Vector2d s2(0, 1);
  double cp = t2.cross(s2);  // returning a scalar between size-2 vectors
  EXPECT_FLOAT_EQ(cp, 1.0);
}

TEST(EigenTest, MatrixTest) {
  Eigen::Matrix2d a;
  a << 1, 2, 3, 4;
  Eigen::MatrixXd b(2, 2);
  b << 2, 3, 1, 4;

  EXPECT_EQ(a(0, 0), 1);
  EXPECT_EQ(a(0, 1), 2);
  EXPECT_EQ(a(1, 0), 3);
  EXPECT_EQ(a(1, 1), 4);
  
  EXPECT_EQ(b(0, 0), 2);
  EXPECT_EQ(b(0, 1), 3);
  EXPECT_EQ(b(1, 0), 1);
  EXPECT_EQ(b(1, 1), 4);

  //////////////////////////////////////////////////////////
  Eigen::MatrixXd add = a + b;
  Eigen::MatrixXd sub = a - b;
  a += b;
  Eigen::MatrixXd mult = a * 2.5;

  EXPECT_EQ(add(0, 0), 3);
  EXPECT_EQ(add(0, 1), 5);
  EXPECT_EQ(add(1, 0), 4);
  EXPECT_EQ(add(1, 1), 8);
  
  EXPECT_EQ(sub(0, 0), -1);
  EXPECT_EQ(sub(0, 1), -1);
  EXPECT_EQ(sub(1, 0), 2);
  EXPECT_EQ(sub(1, 1), 0);

  EXPECT_EQ(a(0, 0), 3);
  EXPECT_EQ(a(0, 1), 5);
  EXPECT_EQ(a(1, 0), 4);
  EXPECT_EQ(a(1, 1), 8);

  EXPECT_EQ(add, a);

  EXPECT_FLOAT_EQ(mult(0, 0), 7.5);
  EXPECT_FLOAT_EQ(mult(0, 1), 12.5);
  EXPECT_FLOAT_EQ(mult(1, 0), 10.0);
  EXPECT_FLOAT_EQ(mult(1, 1), 20.0);
  
  a *= 2.5;
  EXPECT_EQ(mult, a);

  //////////////////////////////////////////////////////////
  Eigen::MatrixXd trans = a.transpose();
  EXPECT_FLOAT_EQ(trans(0, 0), 7.5);
  EXPECT_FLOAT_EQ(trans(0, 1), 10.0);
  EXPECT_FLOAT_EQ(trans(1, 0), 12.5);
  EXPECT_FLOAT_EQ(trans(1, 1), 20.0);

  Eigen::MatrixXd conj = a.conjugate();
  EXPECT_FLOAT_EQ(conj(0, 0), 7.5);
  EXPECT_FLOAT_EQ(conj(0, 1), 12.5);
  EXPECT_FLOAT_EQ(conj(1, 0), 10.0);
  EXPECT_FLOAT_EQ(conj(1, 1), 20.0);

  Eigen::MatrixXd adj = a.adjoint();
  EXPECT_FLOAT_EQ(adj(0, 0), 7.5);
  EXPECT_FLOAT_EQ(adj(0, 1), 10.0);
  EXPECT_FLOAT_EQ(adj(1, 0), 12.5);
  EXPECT_FLOAT_EQ(adj(1, 1), 20.0);

  //////////////////////////////////////////////////////////
  Eigen::MatrixXf c(2, 3);
  c << 1, 2, 3, 4, 5, 6;
  
  EXPECT_EQ(c(0, 0), 1);
  EXPECT_EQ(c(0, 1), 2);
  EXPECT_EQ(c(0, 2), 3);
  EXPECT_EQ(c(1, 0), 4);
  EXPECT_EQ(c(1, 1), 5);
  EXPECT_EQ(c(1, 2), 6);
 
  c.transposeInPlace();

  EXPECT_EQ(c(0, 0), 1);
  EXPECT_EQ(c(0, 1), 4);
  EXPECT_EQ(c(1, 0), 2);
  EXPECT_EQ(c(1, 1), 5);
  EXPECT_EQ(c(2, 0), 3);
  EXPECT_EQ(c(2, 1), 6);

  //////////////////////////////////////////////////////////
  Eigen::Matrix2d mat;
  mat << 1, 2, 3, 4;
  
  EXPECT_EQ(mat(0, 0), 1);
  EXPECT_EQ(mat(0, 1), 2);
  EXPECT_EQ(mat(1, 0), 3);
  EXPECT_EQ(mat(1, 1), 4);
  
  EXPECT_FLOAT_EQ( mat.sum(), 10.0);
  EXPECT_FLOAT_EQ(mat.prod(), 24.0);
  EXPECT_FLOAT_EQ(mat.mean(), 2.5);
  EXPECT_FLOAT_EQ(mat.minCoeff(), 1.0);
  EXPECT_FLOAT_EQ(mat.maxCoeff(), 4.0);
  EXPECT_FLOAT_EQ(mat.trace(), 5.0);

  //////////////////////////////////////////////////////////
  mat << 1, 2, 3, 4;
  Eigen::Vector2d u(-1, 1), v(2, 0);

  Eigen::MatrixXd mat_mult = mat * mat;
  Eigen::MatrixXd mat_mult_vec = mat * u;
  Eigen::MatrixXd vecTrans_mult_mat = u.transpose() * mat;
  Eigen::MatrixXd vecTrans_mult_vec = u.transpose() * v;
  Eigen::MatrixXd vec_mult_vecTrans = u * v.transpose();
  mat = mat * mat;

  EXPECT_EQ(mat_mult(0, 0), 7);
  EXPECT_EQ(mat_mult(0, 1), 10);
  EXPECT_EQ(mat_mult(1, 0), 15);
  EXPECT_EQ(mat_mult(1, 1), 22);
  
  EXPECT_EQ(mat(0, 0), 7);
  EXPECT_EQ(mat(0, 1), 10);
  EXPECT_EQ(mat(1, 0), 15);
  EXPECT_EQ(mat(1, 1), 22);
  
  EXPECT_EQ(mat_mult_vec.rows(), 2);
  EXPECT_EQ(mat_mult_vec.cols(), 1);
  EXPECT_EQ(mat_mult_vec(0, 0), 1);
  EXPECT_EQ(mat_mult_vec(1, 0), 1);
  
  EXPECT_EQ(vecTrans_mult_mat.rows(), 1);
  EXPECT_EQ(vecTrans_mult_mat.cols(), 2);
  EXPECT_EQ(vecTrans_mult_mat(0, 0), 2);
  EXPECT_EQ(vecTrans_mult_mat(0, 1), 2);
  
  EXPECT_EQ(vecTrans_mult_vec.rows(), 1);
  EXPECT_EQ(vecTrans_mult_vec.cols(), 1);
  EXPECT_EQ(vecTrans_mult_vec(0, 0), -2);
  
  EXPECT_EQ(vec_mult_vecTrans.rows(), 2);
  EXPECT_EQ(vec_mult_vecTrans.cols(), 2);
  EXPECT_EQ(vec_mult_vecTrans(0, 0), -2);
  EXPECT_EQ(vec_mult_vecTrans(0, 1), 0);
  EXPECT_EQ(vec_mult_vecTrans(1, 0), 2);
  EXPECT_EQ(vec_mult_vecTrans(1, 1), 0);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
 