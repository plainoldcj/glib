/*
solution:	glib
project:	common
file:		pointer.h
author:		cj
*/

#pragma once

#ifndef NULL
#define NULL 0
#endif

namespace GEN {

	template<typename TYPE>
	class Pointer {
		template<typename DERIVED> 
		friend class Pointer;
	private:
		TYPE* _native;
		unsigned* _count;
	public:
		Pointer(void) : _native(NULL) { }

		explicit Pointer(TYPE* const native) : _native(NULL) { 
			_count = new unsigned(1);
			_native = native;
		}
		
		template<typename IMPLICIT>
		explicit Pointer(IMPLICIT* const native) : _native(NULL) {
			_count = new unsigned(1);
			_native = native;
		}
		
		Pointer(const Pointer& other) : _native(other._native), _count(other._count) {
			if(NULL != _native) {
				*_count++;
			}
		}

		template<typename IMPLICIT>
		Pointer(const Pointer<IMPLICIT>& other) : _native(other._native), _count(other._count) {
			if(NULL != _native) {
				*_count++;
			}
		}

		~Pointer(void) { Drop(); }

		Pointer& operator=(const Pointer& other) {
			if(&other == this) return *this;

			Drop();
			_native = other._native;
			_count = other._count;
			if(NULL != _native) {
				*_count++;
			}
			return *this;
		}

		void Drop(void) {
			if(NULL != _native) {
				*_count--;
				if(0 >= *_count) {
					delete _native;
				}
				_native = NULL;
			}
		}

		bool operator!(void) const {
			return IsNull();
		}

		bool IsNull(void) const {
			return NULL == _native;
		}

		TYPE& operator*(void) { return *_native; }
		const TYPE& operator*(void) const { return *_native; }
		TYPE* operator->(void) { return _native; }
		const TYPE* operator->(void) const { return _native; }
	};

} // namespace GEN