#ifndef __SIMPLE_ATLAS_H__
#define __SIMPLE_ATLAS_H__

#include <cassert>
#include <memory>
#include <vector>

#include "Vec2d.hpp"
#include "Stack.hpp"
#include "SimpleImage.h"
#include "Png.h"
#include "IImage.h"

namespace am {

	namespace sp {

		const int SIMPLE_ATLAS_DIFF = 3;		//differenc between simple atlas and normal atlas

		struct ImageInSAtlas {
		public:
			ImageInSAtlas(const math::Vec2d<int>& pos, const id_t id)
				: _id(id)
				, _pos(pos)
			{
				//no func
			}

			int GetWidth() const { return IMAGE_CONTAINER.GetById(_id)->GetWidth(); }
			int GetHeight() const { return IMAGE_CONTAINER.GetById(_id)->GetHeight(); }

			id_t GetId() const { return _id; }
			const math::Vec2d<int>& GetPosition() const { return _pos; }

		private:
			math::Vec2d<int>	_pos;
			id_t				_id;
		};

		using image_container_t = std::vector<ImageInSAtlas>;

		template<typename T = byte_t>
		class SimpleAtlas {
		public:
			using value_type = T;

			SimpleAtlas(const int width, const int height)
				: _dims(width + SIMPLE_ATLAS_DIFF, height + SIMPLE_ATLAS_DIFF)
				, _EMPTY(0)
				, _FULL(1)
				, _SIZE((width + SIMPLE_ATLAS_DIFF) * (height + SIMPLE_ATLAS_DIFF))
				, _temp((width + SIMPLE_ATLAS_DIFF) * (height + SIMPLE_ATLAS_DIFF))
			{
				_atlas = std::make_unique<value_type[]>(_SIZE);
				_InitAtlasWithDefValue();
			}

			SimpleAtlas(const SimpleAtlas<value_type>& copy)
				: _EMPTY(copy._EMPTY)
				, _FULL(copy._FULL)
				, _SIZE(copy._SIZE)
				, _dims(copy._dims)
				, _temp((copy._dims.X()) * (copy._dims.Y()))
			{
				_CopyAtlasData(copy);
				_images = copy._images;
			}

			SimpleAtlas<value_type>& 
			operator=(const SimpleAtlas<value_type>& copy) = delete;

			int
			GetWidth() const {
				return _dims.X() - SIMPLE_ATLAS_DIFF;
			}

			int
			GetHeight() const {
				return _dims.Y() - SIMPLE_ATLAS_DIFF;
			}

			const image_container_t&
			GetImages() const {
				return _images;
			}

			bool
			PutImageInAtlas(const SimpleImage& image) {
				value_type* ptr = _SearchLeftTop();
				while (ptr != nullptr) {
					if (_TryToPutImageInAtlas(ptr, image)) {
						return true;
					}
					ptr = _SearchLeftTopFrom(ptr);
				}
				return false;
			}

			void
			Clear() {
				_images.clear();
				_temp.Clear();
				_InitAtlasWithDefValue();
			}

			bool
			Empty() {
				return _images.empty();
			}

			~SimpleAtlas() {
				//no func
			}

		private:

			void
			_InitAtlasWithDefValue() {
				value_type* iterator = &_atlas[0];
				for (int y = 0; y < _dims.Y(); ++y) {
					for (int x = 0; x < _dims.X(); ++x) {
						if (y == 0 || (y + 1) == _dims.Y() || x == 0 || (x + 1) == _dims.X()) {
							*iterator = _FULL;
						}
						else {
							*iterator = _EMPTY;
						}
						++iterator;
					}
				}
			}

			value_type*
			_SearchLeftTop() {
				value_type* ptr = &_atlas[_dims.X()];
				while (ptr < &_atlas[_SIZE]) {
					assert(ptr != nullptr);
					if (*ptr == _EMPTY) {
						return ptr;
					}
					++ptr;
				}
				return nullptr;
			}

			value_type*
			_SearchLeftTopFrom(value_type* p) {
				assert(p != nullptr);
				value_type* ptr = p;
				bool find_full = false;
				while (ptr < &_atlas[_SIZE]) {		
					assert(ptr != nullptr);
					if (!find_full) {
						if (*ptr == _FULL) {
							find_full = true;
						}
					}
					else if (*ptr == _EMPTY) {
						return ptr;
					}
					++ptr;
				}
				return nullptr;
			}

			math::Vec2d<int> 
			_GetCoordinates(const value_type* p) {
				assert(p != nullptr);
				assert(p < &_atlas[_SIZE]);
				int diff = p - &_atlas[0];
				math::Vec2d<int> back;
				back.SetY((diff / _dims.X()) - 1);
				back.SetX((diff % _dims.X()) - 1);
				return std::move(back);
			}

			bool
			_TryToPutImageInAtlas(const value_type* const node, const SimpleImage& image) {
				assert(node != nullptr);
				assert(*node != _FULL);
				if (!_FastImageCheck(node, image)) {
					return false;
				}
				_temp.Clear();
				for (int y = 0; y < image.GetHeight(); ++y) {
					value_type* p = _JumpRow(node, y);
					for (int x = 0; x < image.GetWidth(); ++x) {
						if (*p == _EMPTY) {
							*p = _FULL;
							_temp.PushBack(p);
						}
						else {
							_temp.SetAllDataWidthValue(&_EMPTY);	//rollback
							return false;
						}
						++p;
					}
				}
				_PutImageInAtlas(node, image);
				return true;
			}

			void
			_PutImageInAtlas(const value_type* p, const SimpleImage& image) {
				_images.push_back({ _GetCoordinates(p), image.GetId() });
			}

			void
			_CopyAtlasData(const SimpleAtlas<value_type>& copy) {
				_atlas.reset();
				_atlas = std::make_unique<value_type[]>(_SIZE);
				for (int i = 0; i < _SIZE; ++i) {
					_atlas[i] = copy._atlas[i];
				}
			}

			value_type*
			_JumpRow(const value_type* const from, const int row) {
				return const_cast<value_type*>(from + (row * _dims.Y()));
			}

			bool
			_FastDimensionCheck(const value_type* const node, const SimpleImage& image) {
				math::Vec2d<int> coord = _GetCoordinates(node);
				if (coord.X() + image.GetWidth() > _dims.X() - 1) {
					return false;
				}
				if (coord.Y() + image.GetHeight() > _dims.Y() - 1) {
					return false;
				}
				return true;
			}

			bool
			_FastImageCheck(const value_type* const node, const SimpleImage& image) {	//node is left top
				if (!_FastDimensionCheck(node, image)) {
					return false;
				}
				if (*node == _FULL) {
					return false;
				}
				if (*(node + image.GetWidth() - 1) == _FULL) {
					return false;
				}
				if (*_JumpRow(node, image.GetHeight() - 1) == _FULL) {
					return false;
				}
				if (*(_JumpRow(node, image.GetHeight() - 1) + image.GetWidth() - 1) == _FULL) {
					return false;
				}
				return true;
			}


			//------------------------------------------------------------------------
			std::unique_ptr<value_type[]>					_atlas;
			utils::Stack<value_type*, const value_type*>	_temp;
			image_container_t								_images;
			const math::Vec2d<int>							_dims;
			const int										_SIZE;
			const value_type								_EMPTY;
			const value_type								_FULL;
		};

	}

}

#endif //!__SIMPLE_ATLAS_H__

