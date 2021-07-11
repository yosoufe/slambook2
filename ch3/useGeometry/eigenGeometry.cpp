#include <iostream>
#include <cmath>

using namespace std;

#include <Eigen/Core>
#include <Eigen/Geometry>

using namespace Eigen;

// 本程序演示了 Eigen 几何模块的使用方法

int main(int argc, char **argv) {

	// The Eigen/Geometry module provides a variety of rotation and translation representations
	// 3D rotation matrix directly using Matrix3d or Matrix3f
  Matrix3d rotation_matrix = Matrix3d::Identity();
	// The rotation vector uses AngleAxis, the underlying layer is not directly Matrix, but the operation can be treated as a matrix (because the operator is overloaded)
  AngleAxisd rotation_vector(M_PI / 4, Vector3d(0, 0, 1));     //沿 Z 轴旋转 45 度
  cout.precision(3);
  cout << "rotation matrix =\n" << rotation_vector.matrix() << endl;   //用matrix()转换成矩阵
	// can also be assigned directly
  rotation_matrix = rotation_vector.toRotationMatrix();
	// coordinate transformation with AngleAxis
  Vector3d v(1, 0, 0);
  Vector3d v_rotated = rotation_vector * v;
  cout << "(1,0,0) after rotation (by angle axis) = " << v_rotated.transpose() << endl;
	// Or use a rotation matrix
  v_rotated = rotation_matrix * v;
  cout << "(1,0,0) after rotation (by matrix) = " << v_rotated.transpose() << endl;

	// Euler angle: You can convert the rotation matrix directly into Euler angles
  Vector3d euler_angles = rotation_matrix.eulerAngles(2, 1, 0); // ZYX顺序，即yaw-pitch-roll顺序
  cout << "yaw pitch roll = " << euler_angles.transpose() << endl;

	// Euclidean transformation matrix using Eigen::Isometry
  Isometry3d T = Isometry3d::Identity();                // 虽然称为3d，实质上是4＊4的矩阵
  T.rotate(rotation_vector);                                     // 按照rotation_vector进行旋转
  T.pretranslate(Vector3d(1, 3, 4));                     // 把平移向量设成(1,3,4)
  cout << "Transform matrix = \n" << T.matrix() << endl;

	// Use the transformation matrix for coordinate transformation
  Vector3d v_transformed = T * v;                              // 相当于R*v+t
  cout << "v tranformed = " << v_transformed.transpose() << endl;

	// For affine and projective transformations, use Eigen::Affine3d and Eigen::Projective3d.

  // Quaternion
	// You can assign AngleAxis directly to quaternions, and vice versa
  Quaterniond q = Quaterniond(rotation_vector);
  cout << "quaternion from rotation vector = " << q.coeffs().transpose()
       << endl;
	// Note that the order of coeffs is (x, y, z, w), w is the real part, the first three are the imaginary part
	// can also assign a rotation matrix to it
  q = Quaterniond(rotation_matrix);
  cout << "quaternion from rotation matrix = " << q.coeffs().transpose() << endl;
	// Rotate a vector with a quaternion and use overloaded multiplication
  v_rotated = q * v; // 注意数学上是qvq^{-1}
  cout << "(1,0,0) after rotation = " << v_rotated.transpose() << endl;
	// expressed by regular vector multiplication, it should be calculated as follows
  cout << "should be equal to " << (q * Quaterniond(0, 1, 0, 0) * q.inverse()).coeffs().transpose() << endl;

  return 0;
}
