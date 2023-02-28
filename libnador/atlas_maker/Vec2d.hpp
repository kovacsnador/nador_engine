#ifndef __VEC_2D_HPP__
#define __VEC_2D_HPP__


namespace am {

	namespace math {

		template<typename T>
		class Vec2d {
		public:

			using value_type = T;

			Vec2d() : _x(0), _y(0) {
				//no func
			}

			Vec2d(value_type x, value_type y) : _x(x), _y(y) {
				//no func
			}

			Vec2d(const Vec2d<value_type>& copy) {
				_x = copy._x;
				_y = copy._y;
			}

			Vec2d(Vec2d<value_type>&& move) noexcept {
				_x = move._x;
				_y = move._y;
				move._x = 0;
				move._y = 0;
			}

			Vec2d<value_type>& operator=(const Vec2d<value_type>& a) {
				_x = a._x;
				_y = a._y;
				return *this;
			}

			Vec2d<value_type> operator*(value_type value) const {
				return Vec2d<value_type>(_x * value, _y * value);
			}

			value_type X() const {
				return _x;
			}

			value_type Y() const {
				return _y;
			}

			void SetX(const value_type x) {
				_x = x;
			}

			void SetY(const value_type y) {
				_y = y;
			}

		private:
			value_type _x;
			value_type _y;
		};

	}

}


#endif // !__VEC_2D_HPP__
