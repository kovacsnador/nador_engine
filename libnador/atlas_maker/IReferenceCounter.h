#ifndef __REFERENCE_COUNTER_H__
#define __REFERENCE_COUNTER_H__

namespace am {

	namespace api {

		class IReferenceCounter {
		public:
			IReferenceCounter()
				: counter(1)
			{
				//no func
			}

			void grab() const { 
				++counter;
			}

			bool drop() const {
				--counter;
				if (!counter) {
					delete this;
					return true;
				}
				return false;
			
			}

			virtual ~IReferenceCounter() {}

		private:
			mutable int counter;
		};
	}
}

#endif //!__REFERENCE_COUNTER_H__
