#ifndef __I_IMAGE_H__
#define __I_IMAGE_H__

#include "SimpleImage.h"

namespace am {

	class IImage {
	public:
		IImage()
		: _is_atlas(false)
		{
			_id = IMAGE_CONTAINER.PushBack(this);
		}

		virtual int		GetWidth() const = 0;
		virtual int		GetHeight() const = 0;
		virtual id_t	GetId() const { return _id; };

		virtual std::string GetName() const = 0;

		virtual const byte_t* GetRow(const int row) const = 0;
		virtual const byte_t* GetData() const { return nullptr; }

		virtual sp::SimpleImage GetSimpleImageClone() const = 0;
		virtual int				SetSprectrum(const int channel) = 0;
		virtual int				GetSprectrum() const = 0;
		virtual bool			IsAtlas() const { return _is_atlas; }

		virtual ~IImage() { }

	protected:
		id_t _id;
		bool _is_atlas;
	};


}


#endif //!__I_IMAGE_H__
