#ifndef __STACK_HPP__
#define __STACK_HPP__

#include <memory>
#include <stdexcept>

namespace am {

	namespace utils {

		template<typename T, typename T2>
		class Stack {
		public:
			using value_type = T;
			using value_type2 = T2;

			Stack() {
				//no func
			}

			Stack(const size_t size)
				: _size(size)
			{
				_buffer = std::make_unique<value_type[]>(_size);
				_iterator = &_buffer[0];
			}

			void
			PushBack(const value_type elem) {
				if (_iterator < &_buffer[_size]) {
					*_iterator = elem;
					++_iterator;
					return;
				}
				throw std::runtime_error("Stack pushback out of range!");
			}

			void
			SetAllDataWidthValue(value_type2 def) {
				for (value_type* ptr = &_buffer[0]; ptr < _iterator; ++ptr) {
					**ptr = *def;
				}
			}

			void
			Clear() {
				_iterator = &_buffer[_size - 1];
				while (_iterator >= &_buffer[0]) {
					*_iterator = 0;
					--_iterator;
				}
				_iterator = &_buffer[0];
			}


		private:
			std::unique_ptr<value_type[]>	_buffer;
			value_type*						_iterator;
			const size_t					_size;
		};


	}


}


#endif //! __STACK_HPP__
