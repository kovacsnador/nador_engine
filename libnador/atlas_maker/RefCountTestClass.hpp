#pragma once

#include <cstdlib>

#include "IReferenceCounter.h"

using byte_t = char;

namespace am {

	namespace test {

		class RefTest : public api::IReferenceCounter {
		
		public:
			RefTest() : _size(10) {
				_data = (byte_t*)malloc(_size);
			}

			RefTest(const RefTest& copy) {
				grab();
				_data = copy._data;
				_size = copy._size;
			}

			RefTest(RefTest&& copy) {
				grab();
				_data = copy._data;
				_size = copy._size;
				copy._data = nullptr;
				copy._size = 10;
			}

			RefTest& operator=(const RefTest& copy) {
				grab();
				_data = copy._data;
				_size = copy._size;
				return *this;
			}

			~RefTest() {
				if (drop()) {
					free(_data);
				}
			}

		private:
			byte_t* _data;
			size_t	_size;

		};

	}

}
