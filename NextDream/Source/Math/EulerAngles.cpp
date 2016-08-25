#include "EulerAngles.h"

#include "DreMath.h"
#include "Quaternion.h"
#include "Matrix4.h"

namespace dream
{

		Quaternion EulerAngles::EularAnglesToQuaternion() const 
		{
			Quaternion temp;
			f32 sp = 0.0f, sb = 0.0f, sh = 0.0f;
			f32 cp = 0.0f, cb = 0.0f, ch = 0.0f;

			/*SinCos( pitch_ * 0.5f, &sp, &cp );
			SinCos( bank_ * 0.5f, &sb, &cb );
			SinCos( heading_ * 0.5f, &sh, &ch );*/

			temp.mW = ch * cp * cb + sh * sp * sb;
			temp.mX = -ch * sp * cb - sh * cp * sb;
			temp.mY = ch * sp * sb - sh * cb * cp;
			temp.mZ = sh * sp * cb - ch * cp * sb;

			return temp;
		}

		Matrix4 EulerAngles::EularAnglesToMatrix4() const
		{
			Matrix4 temp;

			temp.mM[0][0] = Math::Cos(heading_)*Math::Cos(bank_) + Math::Sin(heading_)*Math::Sin(pitch_)*Math::Sin(bank_);
			temp.mM[0][1] = Math::Sin(bank_)*Math::Cos(pitch_);
			temp.mM[0][2] = -Math::Sin(heading_)*Math::Cos(bank_) + Math::Cos(heading_)*Math::Sin(pitch_)*Math::Sin(bank_);

			temp.mM[1][0] = -Math::Cos(heading_)*Math::Sin(bank_) + Math::Sin(heading_)*Math::Sin(pitch_)*Math::Cos(bank_); 
			temp.mM[1][1] = Math::Cos(bank_)*Math::Cos(pitch_);
			temp.mM[1][2] = Math::Sin(bank_)*Math::Sin(heading_) + Math::Cos(heading_)*Math::Sin(pitch_)*Math::Cos(bank_);

			temp.mM[2][0] = Math::Sin(heading_)*Math::Cos(pitch_);
			temp.mM[2][1] = -Math::Sin(pitch_);
			temp.mM[2][2] = Math::Cos(heading_)*Math::Cos(pitch_);

			temp.mM[3][0] = temp.mM[3][1] = temp.mM[3][2] = 0.0f; 

			return temp;
		}

}	// end namespace dream;