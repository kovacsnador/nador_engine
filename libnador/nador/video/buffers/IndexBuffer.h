#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#include <memory>

#include "nador/video/IVideo.h"

namespace nador
{
	class IndexBuffer
	{
	public:
		/*!
		 * Index buffer constructor.
		 *
		 * \param data	The index data to upload.
		 * \param count	The index count.
		 */
		IndexBuffer(const uint32_t* data, uint32_t count);

		/*!
		 * Index buffer destructor.
		 */
		~IndexBuffer();

		/*!
		 * Bind the Index buffer.
		 */
		void Bind() const;

		/*!
		 * Unbind the Index buffer.
		 */
		void Unbind() const;

		/*!
		 * Gets the index count.
		 *
		 * \return	The index count.
		 */
		uint32_t GetCount();

	private:
		uint32_t		_id;
		const IVideo*	_video;
		uint32_t		_count;
	};

	CREATE_PTR_TYPES(IndexBuffer);

}

#endif // !__INDEX_BUFFER_H__

