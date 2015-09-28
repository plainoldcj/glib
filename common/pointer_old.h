/*
solution:	glib
project:	common
file:		pointer_old.h
author:		cj
*/

/*
DEPRECATED, use pointer.h!!!
*/

#pragma once

#ifndef NULL
#define NULL 0
#endif

namespace GEN {

	template<typename TYPE>
	class Pointer {
	private:
		struct Impl {
			TYPE* native;
			unsigned count;

			Impl(TYPE* const native) : native(native), count(1) { }
			
			template<typename IMPLICIT>
			Impl(IMPLICIT* const native) : native(native), count(1) { }
		} *_impl;

		void Drop(void) {
			if(_impl) {
				_impl->count--;
				if(0 >= _impl->count) {
					delete _impl;
				}
			}
			_impl = NULL;
		}
	public:
		Pointer(void) : _impl(NULL) { }

		template<typename IMPLICIT>
		explicit Pointer(IMPLICIT* const native) : _impl(NULL) {
			_impl = new Impl(native);
		}

		explicit Pointer(TYPE* const native) : _impl(NULL) { 
			_impl = new Impl(native);
		}
		
		Pointer(const Pointer& other) : _impl(other._impl) { 
			if(_impl) _impl->count++;
		}
		
		~Pointer(void) {
			Drop();
		}

		Pointer& operator=(const Pointer& other) {
			if(&other == this) return *this;

			Drop();
			_impl = other._impl;
			if(_impl) _impl->count++;

			return *this;
		}

		bool operator!(void) const {
			return IsNull();
		}

		bool IsNull(void) const {
			return NULL == _impl || NULL == _impl->native;
		}

		TYPE& operator*(void) { return *_impl->native; }
		const TYPE& operator*(void) const { return *_impl->native; }
		TYPE* operator->(void) { return _impl->native; }
		const TYPE* operator->(void) const { return _impl->native; }
	};

} // namespace GEN