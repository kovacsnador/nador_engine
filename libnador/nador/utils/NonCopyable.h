#ifndef __NON_COPYABLE_H__
#define __NON_COPYABLE_H__

namespace nador
{
    class NonCopyable
    {
    protected:
        /*!
         * NonCopyable default constructor.
         */
        constexpr NonCopyable() = default;

        /*!
         * NonCopyable default destructor.
         */
        ~NonCopyable() = default;

        /*!
         * NonCopyable copy destructor deleted.
         */
        NonCopyable(const NonCopyable&) = delete;
        
        /*!
         * NonCopyable copy destructor deleted.
         */
        NonCopyable& operator=(const NonCopyable&) = delete;
    };
}

#endif // !__NON_COPYABLE_H__

